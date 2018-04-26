/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "picasso_global.h"

#if ENABLE(FREE_TYPE2)
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <string.h>

#if defined(WINCE) || defined(WIN32)
#define strncasecmp _strnicmp
#endif

#if defined(__ANDROID__)
#include <expat.h>
#endif

#define MAX_PATH_LEN 512

#define MAX_FONT_PATH_LENGTH MAX_PATH_LEN
#define MAX_FONT_NAME_LENGTH MAX_PATH_LEN

#define MAX_CONFIG_LINE    MAX_PATH_LEN

#if ENABLE(FONT_CONFIG)
#include <fontconfig/fontconfig.h>
#else //not fontconfig
#if defined(WINCE)
#include <windows.h>
#define CONFIG_FILE GetFilePath(L"font_config.cfg")
#if defined(LOAD_FONT_WITH_PATH)
#define CONFIG_PATH(path) path
#else
#define CONFIG_PATH(path) GetFontPath(path)
#endif
static TCHAR g_path[MAX_PATH_LEN];
static inline LPTSTR GetFilePath(LPTSTR file)
{
    TCHAR *p = 0;
    GetModuleFileName(NULL, g_path, MAX_PATH_LEN);
    p = wcsrchr(g_path, '\\');
    p++; *p = 0;
    lstrcat (g_path, file);
    return g_path;
}

static inline char* GetFontPath(const char* name)
{
    TCHAR *p = 0;
    static char p_path[MAX_PATH_LEN];
    GetModuleFileName(NULL, g_path, MAX_PATH_LEN);
    p = wcsrchr(g_path, '\\');
    p++; *p = 0;

    int len = ::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, g_path, -1, p_path, MAX_PATH_LEN, NULL, NULL);

    if ((len + strlen(name)) > (MAX_PATH_LEN-1))
        return 0;

    strcat(p_path, name);
    return p_path;
}
#define OPENFILE   _wfopen
#define F(txt)    L##txt
#else
#define F(txt)   txt
#define OPENFILE   fopen
#define CONFIG_FILE "font_config.cfg"
#define CONFIG_PATH(path) path
#endif
#endif //FONT_CONFIG

namespace gfx {

struct font_item {
    char font_name[MAX_FONT_NAME_LENGTH];
    char font_path[MAX_FONT_PATH_LENGTH];
};

/*
 * Note : _load_fonts function init the global font map at initialize time.
 * it only be call once and not need thread lock.
 */
typedef picasso::pod_bvector<font_item*> font_map;

static font_map g_font_map;

static font_item* get_font_item(const char* name, const char* path)
{
    font_item* f = (font_item*)mem_calloc(1, sizeof(font_item));
    if (f) {
        strncpy(f->font_name, name, MAX_FONT_NAME_LENGTH-1);
        strncpy(f->font_path, path, MAX_FONT_PATH_LENGTH-1);
        return f;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

#if ENABLE(FONT_CONFIG)
static void load_font_from_fontconfig(void)
{
    FcConfig* config = FcInitLoadConfigAndFonts();
    FcFontSet* fontset = NULL;
    // get application fonts
    fontset = FcConfigGetFonts(config, FcSetApplication);
    if (fontset) {
        FcValue fvalue, dvalue;
        for (int i = 0; i < fontset->nfont; i++) {
            if (FcResultMatch == FcPatternGet(fontset->fonts[i], FC_FAMILY, 0, &fvalue)) {
                if (FcResultMatch == FcPatternGet(fontset->fonts[i], FC_FILE, 0, &dvalue)) {
                    font_item* font = get_font_item((const char*)fvalue.u.s, (const char*)dvalue.u.s);
                    g_font_map.add(font);
                }
            }
        }
    }

    // get system fonts
    fontset = FcConfigGetFonts(config, FcSetSystem);
    if (fontset) {
        FcValue fvalue, dvalue;
        for (int i = 0; i < fontset->nfont; i++) {
            if (FcResultMatch == FcPatternGet(fontset->fonts[i], FC_FAMILY, 0, &fvalue)) {
                if (FcResultMatch == FcPatternGet(fontset->fonts[i], FC_FILE, 0, &dvalue)) {
                    font_item* font = get_font_item((const char*)fvalue.u.s, (const char*)dvalue.u.s);
                    g_font_map.add(font);
                }
            }
        }
    }
    FcConfigDestroy(config);
}
#elif defined(__ANDROID__)
// this is only work on android version <= 4.4 not support >= 5.0

#define NO_TAG 0
#define NAMESET_TAG 1
#define FILESET_TAG 2

struct ParserContext {
    ParserContext(XML_Parser* p, font_map* m)
        : parser(p), map(m), remaining_names(0), current_tag(NO_TAG)
    {
    }

    XML_Parser *parser; // The expat parser doing the work
    font_map * map;
    int remaining_names;
    int current_tag;
};

static void text_callback(void *data, const char *s, int len)
{
    ParserContext* context = (ParserContext*)data;

    if (context->current_tag == NAMESET_TAG || context->current_tag == FILESET_TAG) {
        switch (context->current_tag)
        {
        case NAMESET_TAG:
            {
                // add a new font item.
                font_item* f = (font_item*)mem_calloc(1, sizeof(font_item));
                strncpy(f->font_name, s, MIN(len, MAX_FONT_NAME_LENGTH-1));
                context->map->add(f);
                context->remaining_names++;
            }
            break;
        case FILESET_TAG:
            {
                for (int i = 0; i < context->remaining_names; i++) {
                    char* path = context->map->at(context->map->size()-i-1)->font_path;
                    char buffer[MAX_FONT_PATH_LENGTH] = {0};
                    strncpy(buffer, s, MIN(len, MAX_FONT_PATH_LENGTH-1));
                    snprintf(path, MAX_FONT_PATH_LENGTH-1, "/system/fonts/%s", buffer);
                }
                context->remaining_names = 0;
            }
            break;
        default:
            // do nothing.
            break;
        }
    }
}

static void start_callback(void *data, const char *tag, const char **atts)
{
    ParserContext* context = (ParserContext*)data;

    int len = strlen(tag);
    if (strncmp(tag, "family", len) == 0) {
        context->current_tag = NO_TAG;
        context->remaining_names = 0;
    } else if (len == 7 && strncmp(tag, "nameset", len) == 0) {
        context->current_tag = NAMESET_TAG;
        context->remaining_names = 0;
    } else if (len == 7 && strncmp(tag, "fileset", len) == 0) {
        context->current_tag = FILESET_TAG;
    } else if (strncmp(tag, "name", len) == 0 && context->current_tag == NAMESET_TAG) {
        // add a new font_item to map and set name.
        XML_SetCharacterDataHandler(*context->parser, text_callback);
    } else if (strncmp(tag, "file", len) == 0 && context->current_tag == FILESET_TAG) {
        // set the font file to all remaining items.
        // FIXME: it is only first font file will be set for names.
        if (context->remaining_names > 0) {
            XML_SetCharacterDataHandler(*context->parser, text_callback);
        }
    }
}

static void end_callback(void *data, const char *tag)
{
    ParserContext* context = (ParserContext*)data;
    int len = strlen(tag);
    if (strncmp(tag, "family", len)== 0) {
        context->current_tag = NO_TAG;
        context->remaining_names = 0;
    } else if (len == 7 && strncmp(tag, "nameset", len) == 0) {
        context->current_tag = NO_TAG;
    } else if (len == 7 && strncmp(tag, "fileset", len) == 0) {
        context->current_tag = NO_TAG;
    } else if ((strncmp(tag, "name", len) == 0 && context->current_tag == NAMESET_TAG) ||
            (strncmp(tag, "file", len) == 0 && context->current_tag == FILESET_TAG)) {
        // Disable the arbitrary text handler installed to load Name data
        XML_SetCharacterDataHandler(*context->parser, NULL);
    }
}

static void parse_config_file(const char* file, font_map& map)
{
    FILE* fp = NULL;
    fp = fopen(file, "r");
    if (!fp)
        return; // file not found.

    XML_Parser parser = XML_ParserCreate(NULL);
    ParserContext* context = new ParserContext(&parser, &map);
    XML_SetUserData(parser, context);
    XML_SetElementHandler(parser, start_callback, end_callback);

    char buffer[512] = {0};
    bool done = false;
    while (!done) {
        fgets(buffer, sizeof(buffer), fp);
        int len = strlen(buffer);
        if (feof(fp) != 0) {
            done = true;
        }
        XML_Parse(parser, buffer, len, done);
    }
    XML_ParserFree(parser);
    delete context;
    fclose(fp);
}

// font config files
#define SYSTEM_FONTS_FILE "/system/etc/system_fonts.xml"
#define FALLBACK_FONTS_FILE "/system/etc/fallback_fonts.xml"
#define VENDOR_FONTS_FILE "/vendor/etc/fallback_fonts.xml"

static void load_font_from_android(void)
{
    parse_config_file(SYSTEM_FONTS_FILE, g_font_map);
    parse_config_file(FALLBACK_FONTS_FILE, g_font_map);
    parse_config_file(VENDOR_FONTS_FILE, g_font_map);
}
#else
static void write_default(void)
{
    FILE* pf = 0;

    if ((pf = OPENFILE(CONFIG_FILE, F("a+")))) {

        fprintf(pf, "[%s]\n", "sung");
        fprintf(pf, "path=%s\n", "sung.ttf");
        fprintf(pf, "[%s]\n", "arial");
        fprintf(pf, "path=%s\n", "arial.ttf");

        fclose(pf);
    }
}

static void load_font_from_file(FILE * f)
{
    char buf[MAX_CONFIG_LINE+1];
    char* tname = 0;
    char* tpath = 0;
    while (!feof(f)) {
        (void)fgets(buf, MAX_CONFIG_LINE, f);
        char* ps = strchr(buf, '[');
        if (ps) {
            char* pe = strchr(buf, ']');
            if (pe) {
                tname = (char*)mem_malloc(pe-ps);
                strncpy(tname, ps+1, pe-ps-1);
                tname[pe-ps-1] = '\0';

                (void)fgets(buf, MAX_CONFIG_LINE, f);
                ps = strstr(buf, "path=");
                if (ps) {
                    char* pe = strchr(buf, '\n');
                    if (pe) {
                        tpath = (char*)mem_malloc(pe-ps);
                        strncpy(tpath, ps+5, pe-ps-5);
                        tpath[pe-ps-5] = '\0';

                        font_item* font = get_font_item(tname, CONFIG_PATH(tpath));
                        g_font_map.add(font);

                        mem_free(tpath);
                    }
                }
                mem_free(tname);
            }
        }
    }
}
#endif //ENABLE(FONT_CONFIG)

FT_Library g_library = 0;

bool _load_fonts(void)
{
#if ENABLE(FONT_CONFIG)
    load_font_from_fontconfig();
#elif defined(__ANDROID__)
    load_font_from_android();
#else
    FILE *pf = 0;

    if ((pf = OPENFILE(CONFIG_FILE, F("r")))) {

        load_font_from_file(pf);

        fclose(pf);

    } else {
        // not found config file.
        write_default();
    }
#endif

    if (!g_font_map.size()) {

        font_item* ansi_font = get_font_item("arial", "arial.ttf");
        font_item* uni_font = get_font_item("sung", "sung.ttf");

        g_font_map.add(uni_font);
        g_font_map.add(ansi_font);
    }

    if (FT_Init_FreeType(&g_library) == 0)
        return true;
    else
        return false;
}

void _free_fonts(void)
{
    if (g_library)
        FT_Done_FreeType(g_library);

    for (unsigned int i = 0; i < g_font_map.size(); i++)
        mem_free(g_font_map[i]);

    g_font_map.remove_all();
}

char * _font_by_name(const char* face)
{
    for (unsigned int i = 0; i < g_font_map.size(); i++)
        if (strncasecmp(face, g_font_map[i]->font_name, MAX_FONT_NAME_LENGTH-1) == 0)
            return g_font_map[i]->font_path;

    return g_font_map[0]->font_path;
}

}

bool platform_font_init(void)
{
#if ENABLE(FONT_CONFIG)
    FcInit();
#endif
    return gfx::_load_fonts();
}

void platform_font_shutdown(void)
{
    gfx::_free_fonts();
#if ENABLE(FONT_CONFIG)
    FcFini();
#endif
}

#endif /* FREE_TYPE2 */
