#include "nanovg_bgfx_ex.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "SDL_syswm.h"
#include "nanovg_bgfx.h"

bool sdlSetWindow(SDL_Window* _window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(_window, &wmi) )
		{
			return false;
		}

		bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		pd.ndt          = wmi.info.x11.display;
		pd.nwh          = (void*)(uintptr_t)wmi.info.x11.window;
#	elif BX_PLATFORM_OSX
		pd.ndt          = NULL;
		pd.nwh          = wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
		pd.ndt          = NULL;
		pd.nwh          = wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
		pd.ndt          = wmi.info.vivante.display;
		pd.nwh          = wmi.info.vivante.window;
#	endif // BX_PLATFORM_
		pd.context      = NULL;
		pd.backBuffer   = NULL;
		pd.backBufferDS = NULL;
		bgfx::setPlatformData(pd);

		return true;
	}

NVGcontext* nvgCreateBGFX(int32_t _edgeaa, uint16_t _viewId ,uint32_t _width, uint32_t _height, SDL_Window* _window) {

		if (!sdlSetWindow(_window))
		{
			return NULL;
		}
		//bgfx::renderFrame();
		bgfx::Init init;
		init.type     = bgfx::RendererType::Count;
		init.vendorId = BGFX_PCI_ID_NONE;
		init.resolution.width  = _width;
		init.resolution.height = _height;
		init.resolution.reset  = BGFX_RESET_VSYNC;
		bgfx::init(init);

		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
			);
		
	NVGcontext* vg = nvgCreate(_edgeaa, _viewId, NULL);
	bgfx::setViewMode(0, bgfx::ViewMode::Sequential);
	return vg;
}

uint32_t renderBGFXFrame(int32_t _msecs)
{
	return (uint32_t)bgfx::renderFrame(_msecs);
}

void setBGFXViewRect(uint16_t _viewId, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height)
{
	bgfx::setViewRect(_viewId, _x, _y, _width, _height);
}

void touchBGFX(uint16_t _viewId)
{
	bgfx::touch(_viewId);
}

uint32_t frameBGFX(bool _capture)
{
	return bgfx::frame(_capture);
}

void resetBGFX(uint32_t _width, uint32_t _height, uint32_t _flags)
{
	bgfx::reset(_width, _height, _flags);
}

void nvgDeleteBGFX(NVGcontext* _ctx)
{
	nvgDeleteInternal(_ctx);
}

void* nvgluCreateFramebufferByViewId(NVGcontext* ctx, int32_t width, int32_t height, int32_t imageFlags, uint16_t viewId)
{
	NVGLUframebuffer* framebuffer = nvgluCreateFramebuffer(ctx, width, height, imageFlags);

	if (framebuffer != NULL)
	{
		nvgluSetViewFramebuffer(viewId, framebuffer);
	}

	return framebuffer;
}

///
void nvgluBindFramebufferEx(NVGLUframebuffer* _framebuffer)
{
	nvgluBindFramebuffer(_framebuffer);
}

///
void nvgluDeleteFramebufferEx(NVGLUframebuffer* _framebuffer)
{
	nvgluDeleteFramebuffer(_framebuffer);
}


uint32_t getBgfxFboViewId(NVGLUframebuffer* _framebuffer)
{
	return _framebuffer->viewId;
}

uint32_t getBgfxFboImage(NVGLUframebuffer* _framebuffer)
{
	return _framebuffer->image;
}

