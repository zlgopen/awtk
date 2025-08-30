//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef NANOVG_GL_UTILS_H
#define NANOVG_GL_UTILS_H

struct NVGLUframebuffer {
	NVGcontext* ctx;
	GLuint fbo;
	GLuint rbo;
	GLuint self_fbo;
	GLuint temp_fbo;
	GLuint mass_rbo;
	GLuint mass_stencil_rbo;
	GLuint texture;
	int image;
	uint32_t samples;
	uint32_t width;
	uint32_t height;
};
typedef struct NVGLUframebuffer NVGLUframebuffer;

// Helper function to create GL frame buffer to render to.
static void nvgluBindFramebuffer(NVGLUframebuffer* fb);
static NVGLUframebuffer* nvgluCreateFramebuffer(NVGcontext* ctx, int w, int h, int imageFlags, int samples);
static void nvgluDeleteFramebuffer(NVGLUframebuffer* fb);

#endif // NANOVG_GL_UTILS_H

#ifdef NANOVG_GL_IMPLEMENTATION

#if defined(NANOVG_GL3) || defined(NANOVG_GLES2) || defined(NANOVG_GLES3)
// FBO is core in OpenGL 3>.
#	define NANOVG_FBO_VALID 1
#elif defined(NANOVG_GL2)
// On OS X including glext defines FBO on GL2 too.
#	ifdef __APPLE__
#		include <OpenGL/glext.h>
#		define NANOVG_FBO_VALID 1
#	endif
#endif

static GLint defaultFBO = -1;

static int nvgluGetCurrFramebuffer() {
#ifdef NANOVG_FBO_VALID
	GLint defaultFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
	return defaultFBO;
#else
	return -1;
#endif
}

static NVGLUframebuffer* nvgluCreateFramebuffer(NVGcontext* ctx, int w, int h, int imageFlags, int samples)
{
#ifdef NANOVG_FBO_VALID
	GLint defaultFBO;
	GLint defaultRBO;
	NVGLUframebuffer* fb = NULL;

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &defaultRBO);

	fb = (NVGLUframebuffer*)malloc(sizeof(NVGLUframebuffer));
	if (fb == NULL) goto error;
	memset(fb, 0, sizeof(NVGLUframebuffer));

	fb->image = nvgCreateImageRGBA(ctx, w, h, imageFlags | NVG_IMAGE_FLIPY | NVG_IMAGE_PREMULTIPLIED, NULL);

#if defined NANOVG_GL2
	fb->texture = nvglImageHandleGL2(ctx, fb->image);
#elif defined NANOVG_GL3
	fb->texture = nvglImageHandleGL3(ctx, fb->image);
#elif defined NANOVG_GLES2
	fb->texture = nvglImageHandleGLES2(ctx, fb->image);
#elif defined NANOVG_GLES3
	fb->texture = nvglImageHandleGLES3(ctx, fb->image);
#endif

	fb->ctx = ctx;
	fb->width = w;
	fb->height = h;
	// frame buffer object
	glGenFramebuffers(1, &fb->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
#if defined(NANOVG_GL3) || defined(NANOVG_GLES3)
	fb->samples = samples;
	if (samples > 0) { 
		glGenRenderbuffers(1, &fb->mass_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, fb->mass_rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fb->mass_rbo);

		glGenRenderbuffers(1, &fb->mass_stencil_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, fb->mass_stencil_rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_STENCIL_INDEX8, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->mass_stencil_rbo);

		fb->self_fbo = fb->fbo;
		// 创建中间FBO用于解析多重采样结果
		glGenFramebuffers(1, &fb->temp_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fb->temp_fbo);
	}
#endif
	// render buffer object
	glGenRenderbuffers(1, &fb->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, w, h);

	// combine all
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
#ifdef GL_DEPTH24_STENCIL8
		// If GL_STENCIL_INDEX8 is not supported, try GL_DEPTH24_STENCIL8 as a fallback.
		// Some graphics cards require a depth buffer along with a stencil.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
#endif // GL_DEPTH24_STENCIL8
			goto error;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
	return fb;
error:
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
	nvgluDeleteFramebuffer(fb);
	return NULL;
#else
	NVG_NOTUSED(ctx);
	NVG_NOTUSED(w);
	NVG_NOTUSED(h);
	NVG_NOTUSED(imageFlags);
	return NULL;
#endif
}

static void nvgluBindFramebuffer(NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
	if (defaultFBO == -1) glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
	if (fb != NULL && fb->fbo != fb->self_fbo && fb->temp_fbo > 0 && fb->samples > 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->self_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->temp_fbo);
    // 执行多重采样解析
		glBlitFramebuffer(0, 0, fb->width, fb->height, 0, 0, fb->width, fb->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fb != NULL ? fb->fbo : defaultFBO);
#else
	NVG_NOTUSED(fb);
#endif
}

static void nvgluReadCurrentFramebufferData(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int width, unsigned int height, NVGLUframebuffer* fb, void* pixels)
{
	if (fb != NULL) {
		if (fb->temp_fbo > 0 && fb->samples > 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, fb->temp_fbo);
		} else if (fb->samples == 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
		}
	}
	if(x + w <= width && y + h <= height && pixels != NULL) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
}

static void nvgluDeleteFramebuffer(NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
	if (fb == NULL) return;
	if (fb->fbo != 0)
		glDeleteFramebuffers(1, &fb->fbo);
	if (fb->rbo != 0)
		glDeleteRenderbuffers(1, &fb->rbo);
	if (fb->image >= 0)
		nvgDeleteImage(fb->ctx, fb->image);
	if (fb->temp_fbo != 0) 
		glDeleteFramebuffers(1, &fb->temp_fbo);
	if (fb->mass_rbo != 0)
		glDeleteRenderbuffers(1, &fb->mass_rbo);
	if (fb->mass_stencil_rbo != 0)
		glDeleteRenderbuffers(1, &fb->mass_stencil_rbo);
	fb->ctx = NULL;
	fb->fbo = 0;
	fb->rbo = 0;
	fb->texture = 0;
	fb->image = -1;
	free(fb);
#else
	NVG_NOTUSED(fb);
#endif
}

#endif // NANOVG_GL_IMPLEMENTATION
