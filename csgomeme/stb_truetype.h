
#if 0
#define STB_TRUETYPE_IMPLEMENTATION  
#include "stb_truetype.h"

unsigned char ttf_buffer[1 << 20];
unsigned char temp_bitmap[512 * 512];

stbtt_bakedchar cdata[96]; GLuint ftex;

void my_stbtt_initfont(void)
{
	fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);       glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void my_stbtt_print(float x, float y, char *text)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glBegin(GL_QUADS);
	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);         glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y0);
			glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y0);
			glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y1);
			glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y1);
		}
		++text;
	}
	glEnd();
}
#endif
#if 0
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION  
#include "stb_truetype.h"

char ttf_buffer[1 << 25];

int main(int argc, char **argv)
{
	stbtt_fontinfo font;
	unsigned char *bitmap;
	int w, h, i, j, c = (argc > 1 ? atoi(argv[1]) : ((char)(0x1182 + 2620 - 0x1b5d))), s = (argc > 2 ? atoi(argv[2]) : 20);

	fread(ttf_buffer, 1, 1 << 25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/arialbd.ttf", "rb"));

	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
	bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0, 0);

	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i)
			putchar(" .:ioVM@"[bitmap[j*w + i] >> 5]);
		putchar('\n');
	}
	return 0;
}
#endif 
#if 0
char buffer[24 << 20];
unsigned char screen[20][79];

int main(int arg, char **argv)
{
	stbtt_fontinfo font;
	int i, j, ascent, baseline, ch = 0;
	float scale, xpos = 2;    char *text = "Heljo World!";
	fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
	stbtt_InitFont(&font, buffer, 0);

	scale = stbtt_ScaleForPixelHeight(&font, 15);
	stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
	baseline = (int)(ascent*scale);

	while (text[ch]) {
		int advance, lsb, x0, y0, x1, y1;
		float x_shift = xpos - (float)floor(xpos);
		stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
		stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
		stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[ch]);
		xpos += (advance * scale);
		if (text[ch + 1])
			xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
		++ch;
	}

	for (j = 0; j < 20; ++j) {
		for (i = 0; i < 78; ++i)
			putchar(" .:ioVM@"[screen[j][i] >> 5]);
		putchar('\n');
	}

	return 0;
}
#endif



#ifdef STB_TRUETYPE_IMPLEMENTATION

#ifndef stbtt_uint8
typedef unsigned char   stbtt_uint8;
typedef signed   char   stbtt_int8;
typedef unsigned short  stbtt_uint16;
typedef signed   short  stbtt_int16;
typedef unsigned int    stbtt_uint32;
typedef signed   int    stbtt_int32;

#endif

typedef char stbtt__check_size32[sizeof(stbtt_int32) == 4 ? 1 : -1];
typedef char stbtt__check_size16[sizeof(stbtt_int16) == 2 ? 1 : -1];


#ifndef STBTT_ifloor

#include <math.h>

#define STBTT_ifloor(x)   ((int) floor(x))

#define STBTT_iceil(x)    ((int) ceil(x))

#endif


#ifndef STBTT_sqrt

#include <math.h>

#define STBTT_sqrt(x)      sqrt(x)

#endif


#ifndef STBTT_fabs

#include <math.h>

#define STBTT_fabs(x)      fabs(x)

#endif


#ifndef STBTT_malloc

#include <stdlib.h>

#define STBTT_malloc(x,u)  ((void)(u),malloc(x))

#define STBTT_free(x,u)    ((void)(u),free(x))

#endif


#ifndef STBTT_assert

#include <assert.h>

#define STBTT_assert(x)    assert(x)

#endif


#ifndef STBTT_strlen

#include <string.h>

#define STBTT_strlen(x)    strlen(x)

#endif


#ifndef STBTT_memcpy

#include <memory.h>

#define STBTT_memcpy       memcpy

#define STBTT_memset       memset

#endif
#endif


#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#define __STB_INCLUDE_STB_TRUETYPE_H__

#ifdef STBTT_STATIC
#define STBTT_DEF static
#else
#define STBTT_DEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct
	{
		unsigned short x0, y0, x1, y1;    float xoff, yoff, xadvance;
	} stbtt_bakedchar;

	STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char *data, int offset, float pixel_height, unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata);
	typedef struct
	{
		float x0, y0, s0, t0;    float x1, y1, s1, t1;
	} stbtt_aligned_quad;

	STBTT_DEF void stbtt_GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule);



	typedef struct
	{
		unsigned short x0, y0, x1, y1;    float xoff, yoff, xadvance;
		float xoff2, yoff2;
	} stbtt_packedchar;

	typedef struct stbtt_pack_context stbtt_pack_context;
	typedef struct stbtt_fontinfo stbtt_fontinfo;
#ifndef STB_RECT_PACK_VERSION
	typedef struct stbrp_rect stbrp_rect;
#endif

	STBTT_DEF int  stbtt_PackBegin(stbtt_pack_context *spc, unsigned char *pixels, int width, int height, int stride_in_bytes, int padding, void *alloc_context);

	STBTT_DEF void stbtt_PackEnd(stbtt_pack_context *spc);

#define STBTT_POINT_SIZE(x)   (-(x))

	STBTT_DEF int  stbtt_PackFontRange(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, float font_size,
		int first_unicode_char_in_range, int num_chars_in_range, stbtt_packedchar *chardata_for_range);

	typedef struct
	{
		float font_size;
		int first_unicode_codepoint_in_range;     int *array_of_unicode_codepoints;          int num_chars;
		stbtt_packedchar *chardata_for_range;    unsigned char h_oversample, v_oversample;
	} stbtt_pack_range;

	STBTT_DEF int  stbtt_PackFontRanges(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, stbtt_pack_range *ranges, int num_ranges);

	STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context *spc, unsigned int h_oversample, unsigned int v_oversample);

	STBTT_DEF void stbtt_GetPackedQuad(stbtt_packedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int align_to_integer);

	STBTT_DEF int  stbtt_PackFontRangesGatherRects(stbtt_pack_context *spc, stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects);
	STBTT_DEF void stbtt_PackFontRangesPackRects(stbtt_pack_context *spc, stbrp_rect *rects, int num_rects);
	STBTT_DEF int  stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context *spc, stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects);

	struct stbtt_pack_context {
		void *user_allocator_context;
		void *pack_info;
		int   width;
		int   height;
		int   stride_in_bytes;
		int   padding;
		unsigned int   h_oversample, v_oversample;
		unsigned char *pixels;
		void  *nodes;
	};


	STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char *data, int index);


	struct stbtt_fontinfo
	{
		void           * userdata;
		unsigned char  * data;                 int              fontstart;
		int numGlyphs;
		int loca, head, glyf, hhea, hmtx, kern;    int index_map;                        int indexToLocFormat;
	};

	STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data, int offset);



	STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint);



	STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float pixels);

	STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *info, float pixels);

	STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap);

	STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo *info, int *x0, int *y0, int *x1, int *y1);

	STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing);

	STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2);

	STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo *info, int codepoint, int *x0, int *y0, int *x1, int *y1);

	STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing);
	STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2);
	STBTT_DEF int  stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1);



#ifndef STBTT_vmove 
	enum {
		STBTT_vmove = 1,
		STBTT_vline,
		STBTT_vcurve
	};
#endif

#ifndef stbtt_vertex 

#define stbtt_vertex_type short 
	typedef struct
	{
		stbtt_vertex_type x, y, cx, cy;
		unsigned char type, padding;
	} stbtt_vertex;
#endif

	STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo *info, int glyph_index);

	STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo *info, int unicode_codepoint, stbtt_vertex **vertices);
	STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **vertices);

	STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo *info, stbtt_vertex *vertices);


	STBTT_DEF void stbtt_FreeBitmap(unsigned char *bitmap, void *userdata);

	STBTT_DEF unsigned char *stbtt_GetCodepointBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int codepoint, int *width, int *height, int *xoff, int *yoff);

	STBTT_DEF unsigned char *stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int *width, int *height, int *xoff, int *yoff);

	STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint);

	STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint);

	STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1);

	STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1);

	STBTT_DEF unsigned char *stbtt_GetGlyphBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int glyph, int *width, int *height, int *xoff, int *yoff);
	STBTT_DEF unsigned char *stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff);
	STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph);
	STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph);
	STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1);
	STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1);


	typedef struct
	{
		int w, h, stride;
		unsigned char *pixels;
	} stbtt__bitmap;

	STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata);


	STBTT_DEF int stbtt_FindMatchingFont(const unsigned char *fontdata, const char *name, int flags);
#define STBTT_MACSTYLE_DONTCARE     0
#define STBTT_MACSTYLE_BOLD         1
#define STBTT_MACSTYLE_ITALIC       2
#define STBTT_MACSTYLE_UNDERSCORE   4
#define STBTT_MACSTYLE_NONE         8   

	STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char *s1, int len1, const char *s2, int len2);

	STBTT_DEF const char *stbtt_GetFontNameString(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID);

	enum {
		STBTT_PLATFORM_ID_UNICODE = 0,
		STBTT_PLATFORM_ID_MAC = 1,
		STBTT_PLATFORM_ID_ISO = 2,
		STBTT_PLATFORM_ID_MICROSOFT = 3
	};

	enum {
		STBTT_UNICODE_EID_UNICODE_1_0 = 0,
		STBTT_UNICODE_EID_UNICODE_1_1 = 1,
		STBTT_UNICODE_EID_ISO_10646 = 2,
		STBTT_UNICODE_EID_UNICODE_2_0_BMP = 3,
		STBTT_UNICODE_EID_UNICODE_2_0_FULL = 4
	};

	enum {
		STBTT_MS_EID_SYMBOL = 0,
		STBTT_MS_EID_UNICODE_BMP = 1,
		STBTT_MS_EID_SHIFTJIS = 2,
		STBTT_MS_EID_UNICODE_FULL = 10
	};

	enum {
		STBTT_MAC_EID_ROMAN = 0, STBTT_MAC_EID_ARABIC = 4,
		STBTT_MAC_EID_JAPANESE = 1, STBTT_MAC_EID_HEBREW = 5,
		STBTT_MAC_EID_CHINESE_TRAD = 2, STBTT_MAC_EID_GREEK = 6,
		STBTT_MAC_EID_KOREAN = 3, STBTT_MAC_EID_RUSSIAN = 7
	};

	enum {
		STBTT_MS_LANG_ENGLISH = 0x0409, STBTT_MS_LANG_ITALIAN = 0x0410,
		STBTT_MS_LANG_CHINESE = 0x0804, STBTT_MS_LANG_JAPANESE = 0x0411,
		STBTT_MS_LANG_DUTCH = 0x0413, STBTT_MS_LANG_KOREAN = 0x0412,
		STBTT_MS_LANG_FRENCH = 0x040c, STBTT_MS_LANG_RUSSIAN = 0x0419,
		STBTT_MS_LANG_GERMAN = 0x0407, STBTT_MS_LANG_SPANISH = 0x0409,
		STBTT_MS_LANG_HEBREW = 0x040d, STBTT_MS_LANG_SWEDISH = 0x041d
	};

	enum {
		STBTT_MAC_LANG_ENGLISH = 0, STBTT_MAC_LANG_JAPANESE = 11,
		STBTT_MAC_LANG_ARABIC = 12, STBTT_MAC_LANG_KOREAN = 23,
		STBTT_MAC_LANG_DUTCH = 4, STBTT_MAC_LANG_RUSSIAN = 32,
		STBTT_MAC_LANG_FRENCH = 1, STBTT_MAC_LANG_SPANISH = 6,
		STBTT_MAC_LANG_GERMAN = 2, STBTT_MAC_LANG_SWEDISH = 5,
		STBTT_MAC_LANG_HEBREW = 10, STBTT_MAC_LANG_CHINESE_SIMPLIFIED = 33,
		STBTT_MAC_LANG_ITALIAN = 3, STBTT_MAC_LANG_CHINESE_TRAD = 19
	};

#ifdef __cplusplus
}
#endif

#endif 


#ifdef STB_TRUETYPE_IMPLEMENTATION

#ifndef STBTT_MAX_OVERSAMPLE
#define STBTT_MAX_OVERSAMPLE   8
#endif

#if STBTT_MAX_OVERSAMPLE > 255
#error "STBTT_MAX_OVERSAMPLE cannot be > 255"
#endif

typedef int stbtt__test_oversample_pow2[(STBTT_MAX_OVERSAMPLE & (STBTT_MAX_OVERSAMPLE - 1)) == 0 ? 1 : -1];

#ifndef STBTT_RASTERIZER_VERSION
#define STBTT_RASTERIZER_VERSION 2
#endif



#define ttBYTE(p)     (* (stbtt_uint8 *) (p))
#define ttCHAR(p)     (* (stbtt_int8 *) (p))
#define ttFixed(p)    ttLONG(p)

#if defined(STB_TRUETYPE_BIGENDIAN) && !defined(ALLOW_UNALIGNED_TRUETYPE)


#define ttUSHORT(p)   (* (stbtt_uint16 *) (p))

#define ttSHORT(p)    (* (stbtt_int16 *) (p))

#define ttULONG(p)    (* (stbtt_uint32 *) (p))

#define ttLONG(p)     (* (stbtt_int32 *) (p))

#else

static stbtt_uint16 ttUSHORT(const stbtt_uint8 *p) { return p[0] * 256 + p[1]; }
static stbtt_int16 ttSHORT(const stbtt_uint8 *p) { return p[0] * 256 + p[1]; }
static stbtt_uint32 ttULONG(const stbtt_uint8 *p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }
static stbtt_int32 ttLONG(const stbtt_uint8 *p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }

#endif

#define stbtt_tag4(p,c0,c1,c2,c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define stbtt_tag(p,str)           stbtt_tag4(p,str[0],str[1],str[2],str[3])

static int stbtt__isfont(const stbtt_uint8 *font)
{
	if (stbtt_tag4(font, ((char)(0x127 + 7930 - 0x1ff0)), 0, 0, 0))  return 1;    if (stbtt_tag(font, "typ1"))   return 1;    if (stbtt_tag(font, "OTTO"))   return 1;    if (stbtt_tag4(font, 0, 1, 0, 0)) return 1;    return 0;
}

static stbtt_uint32 stbtt__find_table(stbtt_uint8 *data, stbtt_uint32 fontstart, const char *tag)
{
	stbtt_int32 num_tables = ttUSHORT(data + fontstart + 4);
	stbtt_uint32 tabledir = fontstart + 12;
	stbtt_int32 i;
	for (i = 0; i < num_tables; ++i) {
		stbtt_uint32 loc = tabledir + 16 * i;
		if (stbtt_tag(data + loc + 0, tag))
			return ttULONG(data + loc + 8);
	}
	return 0;
}

STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char *font_collection, int index)
{
	if (stbtt__isfont(font_collection))
		return index == 0 ? 0 : -1;

	if (stbtt_tag(font_collection, "ttcf")) {
		if (ttULONG(font_collection + 4) == 0x00010000 || ttULONG(font_collection + 4) == 0x00020000) {
			stbtt_int32 n = ttLONG(font_collection + 8);
			if (index >= n)
				return -1;
			return ttULONG(font_collection + 12 + index * 4);
		}
	}
	return -1;
}

STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data2, int fontstart)
{
	stbtt_uint8 *data = (stbtt_uint8 *)data2;
	stbtt_uint32 cmap, t;
	stbtt_int32 i, numTables;

	info->data = data;
	info->fontstart = fontstart;

	cmap = stbtt__find_table(data, fontstart, "cmap");          info->loca = stbtt__find_table(data, fontstart, "loca");    info->head = stbtt__find_table(data, fontstart, "head");    info->glyf = stbtt__find_table(data, fontstart, "glyf");    info->hhea = stbtt__find_table(data, fontstart, "hhea");    info->hmtx = stbtt__find_table(data, fontstart, "hmtx");    info->kern = stbtt__find_table(data, fontstart, "kern");    if (!cmap || !info->loca || !info->head || !info->glyf || !info->hhea || !info->hmtx)
		return 0;

	t = stbtt__find_table(data, fontstart, "maxp");
	if (t)
		info->numGlyphs = ttUSHORT(data + t + 4);
	else
		info->numGlyphs = 0xffff;

	numTables = ttUSHORT(data + cmap + 2);
	info->index_map = 0;
	for (i = 0; i < numTables; ++i) {
		stbtt_uint32 encoding_record = cmap + 4 + 8 * i;
		switch (ttUSHORT(data + encoding_record)) {
		case STBTT_PLATFORM_ID_MICROSOFT:
			switch (ttUSHORT(data + encoding_record + 2)) {
			case STBTT_MS_EID_UNICODE_BMP:
			case STBTT_MS_EID_UNICODE_FULL:
				info->index_map = cmap + ttULONG(data + encoding_record + 4);
				break;
			}
			break;
		case STBTT_PLATFORM_ID_UNICODE:
			info->index_map = cmap + ttULONG(data + encoding_record + 4);
			break;
		}
	}
	if (info->index_map == 0)
		return 0;

	info->indexToLocFormat = ttUSHORT(data + info->head + 50);
	return 1;
}

STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint)
{
	stbtt_uint8 *data = info->data;
	stbtt_uint32 index_map = info->index_map;

	stbtt_uint16 format = ttUSHORT(data + index_map + 0);
	if (format == 0) {
		stbtt_int32 bytes = ttUSHORT(data + index_map + 2);
		if (unicode_codepoint < bytes - 6)
			return ttBYTE(data + index_map + 6 + unicode_codepoint);
		return 0;
	}
	else if (format == 6) {
		stbtt_uint32 first = ttUSHORT(data + index_map + 6);
		stbtt_uint32 count = ttUSHORT(data + index_map + 8);
		if ((stbtt_uint32)unicode_codepoint >= first && (stbtt_uint32)unicode_codepoint < first + count)
			return ttUSHORT(data + index_map + 10 + (unicode_codepoint - first) * 2);
		return 0;
	}
	else if (format == 2) {
		STBTT_assert(0);       return 0;
	}
	else if (format == 4) {
		stbtt_uint16 segcount = ttUSHORT(data + index_map + 6) >> 1;
		stbtt_uint16 searchRange = ttUSHORT(data + index_map + 8) >> 1;
		stbtt_uint16 entrySelector = ttUSHORT(data + index_map + 10);
		stbtt_uint16 rangeShift = ttUSHORT(data + index_map + 12) >> 1;

		stbtt_uint32 endCount = index_map + 14;
		stbtt_uint32 search = endCount;

		if (unicode_codepoint > 0xffff)
			return 0;

		if (unicode_codepoint >= ttUSHORT(data + search + rangeShift * 2))
			search += rangeShift * 2;

		search -= 2;
		while (entrySelector) {
			stbtt_uint16 end;
			searchRange >>= 1;
			end = ttUSHORT(data + search + searchRange * 2);
			if (unicode_codepoint > end)
				search += searchRange * 2;
			--entrySelector;
		}
		search += 2;

		{
			stbtt_uint16 offset, start;
			stbtt_uint16 item = (stbtt_uint16)((search - endCount) >> 1);

			STBTT_assert(unicode_codepoint <= ttUSHORT(data + endCount + 2 * item));
			start = ttUSHORT(data + index_map + 14 + segcount * 2 + 2 + 2 * item);
			if (unicode_codepoint < start)
				return 0;

			offset = ttUSHORT(data + index_map + 14 + segcount * 6 + 2 + 2 * item);
			if (offset == 0)
				return (stbtt_uint16)(unicode_codepoint + ttSHORT(data + index_map + 14 + segcount * 4 + 2 + 2 * item));

			return ttUSHORT(data + offset + (unicode_codepoint - start) * 2 + index_map + 14 + segcount * 6 + 2 + 2 * item);
		}
	}
	else if (format == 12 || format == 13) {
		stbtt_uint32 ngroups = ttULONG(data + index_map + 12);
		stbtt_int32 low, high;
		low = 0; high = (stbtt_int32)ngroups;
		while (low < high) {
			stbtt_int32 mid = low + ((high - low) >> 1);          stbtt_uint32 start_char = ttULONG(data + index_map + 16 + mid * 12);
			stbtt_uint32 end_char = ttULONG(data + index_map + 16 + mid * 12 + 4);
			if ((stbtt_uint32)unicode_codepoint < start_char)
				high = mid;
			else if ((stbtt_uint32)unicode_codepoint > end_char)
				low = mid + 1;
			else {
				stbtt_uint32 start_glyph = ttULONG(data + index_map + 16 + mid * 12 + 8);
				if (format == 12)
					return start_glyph + unicode_codepoint - start_char;
				else                return start_glyph;
			}
		}
		return 0;
	}
	STBTT_assert(0);
	return 0;
}

STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo *info, int unicode_codepoint, stbtt_vertex **vertices)
{
	return stbtt_GetGlyphShape(info, stbtt_FindGlyphIndex(info, unicode_codepoint), vertices);
}

static void stbtt_setvertex(stbtt_vertex *v, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy)
{
	v->type = type;
	v->x = (stbtt_int16)x;
	v->y = (stbtt_int16)y;
	v->cx = (stbtt_int16)cx;
	v->cy = (stbtt_int16)cy;
}

static int stbtt__GetGlyfOffset(const stbtt_fontinfo *info, int glyph_index)
{
	int g1, g2;

	if (glyph_index >= info->numGlyphs) return -1;    if (info->indexToLocFormat >= 2)    return -1;
	if (info->indexToLocFormat == 0) {
		g1 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
		g2 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
	}
	else {
		g1 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4);
		g2 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4 + 4);
	}

	return g1 == g2 ? -1 : g1;
}

STBTT_DEF int stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
	int g = stbtt__GetGlyfOffset(info, glyph_index);
	if (g < 0) return 0;

	if (x0) *x0 = ttSHORT(info->data + g + 2);
	if (y0) *y0 = ttSHORT(info->data + g + 4);
	if (x1) *x1 = ttSHORT(info->data + g + 6);
	if (y1) *y1 = ttSHORT(info->data + g + 8);
	return 1;
}

STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo *info, int codepoint, int *x0, int *y0, int *x1, int *y1)
{
	return stbtt_GetGlyphBox(info, stbtt_FindGlyphIndex(info, codepoint), x0, y0, x1, y1);
}

STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo *info, int glyph_index)
{
	stbtt_int16 numberOfContours;
	int g = stbtt__GetGlyfOffset(info, glyph_index);
	if (g < 0) return 1;
	numberOfContours = ttSHORT(info->data + g);
	return numberOfContours == 0;
}

static int stbtt__close_shape(stbtt_vertex *vertices, int num_vertices, int was_off, int start_off,
	stbtt_int32 sx, stbtt_int32 sy, stbtt_int32 scx, stbtt_int32 scy, stbtt_int32 cx, stbtt_int32 cy)
{
	if (start_off) {
		if (was_off)
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + scx) >> 1, (cy + scy) >> 1, cx, cy);
		stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, scx, scy);
	}
	else {
		if (was_off)
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, cx, cy);
		else
			stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, sx, sy, 0, 0);
	}
	return num_vertices;
}

STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
	stbtt_int16 numberOfContours;
	stbtt_uint8 *endPtsOfContours;
	stbtt_uint8 *data = info->data;
	stbtt_vertex *vertices = 0;
	int num_vertices = 0;
	int g = stbtt__GetGlyfOffset(info, glyph_index);

	*pvertices = NULL;

	if (g < 0) return 0;

	numberOfContours = ttSHORT(data + g);

	if (numberOfContours > 0) {
		stbtt_uint8 flags = 0, flagcount;
		stbtt_int32 ins, i, j = 0, m, n, next_move, was_off = 0, off, start_off = 0;
		stbtt_int32 x, y, cx, cy, sx, sy, scx, scy;
		stbtt_uint8 *points;
		endPtsOfContours = (data + g + 10);
		ins = ttUSHORT(data + g + 10 + numberOfContours * 2);
		points = data + g + 10 + numberOfContours * 2 + 2 + ins;

		n = 1 + ttUSHORT(endPtsOfContours + numberOfContours * 2 - 2);

		m = n + 2 * numberOfContours;        vertices = (stbtt_vertex *)STBTT_malloc(m * sizeof(vertices[0]), info->userdata);
		if (vertices == 0)
			return 0;

		next_move = 0;
		flagcount = 0;


		off = m - n;

		for (i = 0; i < n; ++i) {
			if (flagcount == 0) {
				flags = *points++;
				if (flags & 8)
					flagcount = *points++;
			}
			else
				--flagcount;
			vertices[off + i].type = flags;
		}

		x = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 2) {
				stbtt_int16 dx = *points++;
				x += (flags & 16) ? dx : -dx;
			}
			else {
				if (!(flags & 16)) {
					x = x + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].x = (stbtt_int16)x;
		}

		y = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 4) {
				stbtt_int16 dy = *points++;
				y += (flags & 32) ? dy : -dy;
			}
			else {
				if (!(flags & 32)) {
					y = y + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].y = (stbtt_int16)y;
		}

		num_vertices = 0;
		sx = sy = cx = cy = scx = scy = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			x = (stbtt_int16)vertices[off + i].x;
			y = (stbtt_int16)vertices[off + i].y;

			if (next_move == i) {
				if (i != 0)
					num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);

				start_off = !(flags & 1);
				if (start_off) {
					scx = x;
					scy = y;
					if (!(vertices[off + i + 1].type & 1)) {
						sx = (x + (stbtt_int32)vertices[off + i + 1].x) >> 1;
						sy = (y + (stbtt_int32)vertices[off + i + 1].y) >> 1;
					}
					else {
						sx = (stbtt_int32)vertices[off + i + 1].x;
						sy = (stbtt_int32)vertices[off + i + 1].y;
						++i;
					}
				}
				else {
					sx = x;
					sy = y;
				}
				stbtt_setvertex(&vertices[num_vertices++], STBTT_vmove, sx, sy, 0, 0);
				was_off = 0;
				next_move = 1 + ttUSHORT(endPtsOfContours + j * 2);
				++j;
			}
			else {
				if (!(flags & 1)) {
					if (was_off)                   stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + x) >> 1, (cy + y) >> 1, cx, cy);
					cx = x;
					cy = y;
					was_off = 1;
				}
				else {
					if (was_off)
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, x, y, cx, cy);
					else
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, x, y, 0, 0);
					was_off = 0;
				}
			}
		}
		num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
	}
	else if (numberOfContours == -1) {
		int more = 1;
		stbtt_uint8 *comp = data + g + 10;
		num_vertices = 0;
		vertices = 0;
		while (more) {
			stbtt_uint16 flags, gidx;
			int comp_num_verts = 0, i;
			stbtt_vertex *comp_verts = 0, *tmp = 0;
			float mtx[6] = { 1,0,0,1,0,0 }, m, n;

			flags = ttSHORT(comp); comp += 2;
			gidx = ttSHORT(comp); comp += 2;

			if (flags & 2) {
				if (flags & 1) {
					mtx[4] = ttSHORT(comp); comp += 2;
					mtx[5] = ttSHORT(comp); comp += 2;
				}
				else {
					mtx[4] = ttCHAR(comp); comp += 1;
					mtx[5] = ttCHAR(comp); comp += 1;
				}
			}
			else {
				STBTT_assert(0);
			}
			if (flags & (1 << 3)) {
				mtx[0] = mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
			}
			else if (flags & (1 << 6)) {
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}
			else if (flags & (1 << 7)) {
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[2] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}

			m = (float)STBTT_sqrt(mtx[0] * mtx[0] + mtx[1] * mtx[1]);
			n = (float)STBTT_sqrt(mtx[2] * mtx[2] + mtx[3] * mtx[3]);

			comp_num_verts = stbtt_GetGlyphShape(info, gidx, &comp_verts);
			if (comp_num_verts > 0) {
				for (i = 0; i < comp_num_verts; ++i) {
					stbtt_vertex* v = &comp_verts[i];
					stbtt_vertex_type x, y;
					x = v->x; y = v->y;
					v->x = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->y = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
					x = v->cx; y = v->cy;
					v->cx = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->cy = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
				}
				tmp = (stbtt_vertex*)STBTT_malloc((num_vertices + comp_num_verts) * sizeof(stbtt_vertex), info->userdata);
				if (!tmp) {
					if (vertices) STBTT_free(vertices, info->userdata);
					if (comp_verts) STBTT_free(comp_verts, info->userdata);
					return 0;
				}
				if (num_vertices > 0) STBTT_memcpy(tmp, vertices, num_vertices * sizeof(stbtt_vertex));
				STBTT_memcpy(tmp + num_vertices, comp_verts, comp_num_verts * sizeof(stbtt_vertex));
				if (vertices) STBTT_free(vertices, info->userdata);
				vertices = tmp;
				STBTT_free(comp_verts, info->userdata);
				num_vertices += comp_num_verts;
			}
			more = flags & (1 << 5);
		}
	}
	else if (numberOfContours < 0) {
		STBTT_assert(0);
	}
	else {
	}

	*pvertices = vertices;
	return num_vertices;
}

STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing)
{
	stbtt_uint16 numOfLongHorMetrics = ttUSHORT(info->data + info->hhea + 34);
	if (glyph_index < numOfLongHorMetrics) {
		if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * glyph_index);
		if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * glyph_index + 2);
	}
	else {
		if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * (numOfLongHorMetrics - 1));
		if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * numOfLongHorMetrics + 2 * (glyph_index - numOfLongHorMetrics));
	}
}

STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
{
	stbtt_uint8 *data = info->data + info->kern;
	stbtt_uint32 needle, straw;
	int l, r, m;

	if (!info->kern)
		return 0;
	if (ttUSHORT(data + 2) < 1)       return 0;
	if (ttUSHORT(data + 8) != 1)       return 0;

	l = 0;
	r = ttUSHORT(data + 10) - 1;
	needle = glyph1 << 16 | glyph2;
	while (l <= r) {
		m = (l + r) >> 1;
		straw = ttULONG(data + 18 + (m * 6));       if (needle < straw)
			r = m - 1;
		else if (needle > straw)
			l = m + 1;
		else
			return ttSHORT(data + 22 + (m * 6));
	}
	return 0;
}

STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2)
{
	if (!info->kern)       return 0;
	return stbtt_GetGlyphKernAdvance(info, stbtt_FindGlyphIndex(info, ch1), stbtt_FindGlyphIndex(info, ch2));
}

STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing)
{
	stbtt_GetGlyphHMetrics(info, stbtt_FindGlyphIndex(info, codepoint), advanceWidth, leftSideBearing);
}

STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap)
{
	if (ascent) *ascent = ttSHORT(info->data + info->hhea + 4);
	if (descent) *descent = ttSHORT(info->data + info->hhea + 6);
	if (lineGap) *lineGap = ttSHORT(info->data + info->hhea + 8);
}

STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo *info, int *x0, int *y0, int *x1, int *y1)
{
	*x0 = ttSHORT(info->data + info->head + 36);
	*y0 = ttSHORT(info->data + info->head + 38);
	*x1 = ttSHORT(info->data + info->head + 40);
	*y1 = ttSHORT(info->data + info->head + 42);
}

STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float height)
{
	int fheight = ttSHORT(info->data + info->hhea + 4) - ttSHORT(info->data + info->hhea + 6);
	return (float)height / fheight;
}

STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *info, float pixels)
{
	int unitsPerEm = ttUSHORT(info->data + info->head + 18);
	return pixels / unitsPerEm;
}

STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo *info, stbtt_vertex *v)
{
	STBTT_free(v, info->userdata);
}


STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	int x0 = 0, y0 = 0, x1, y1;    if (!stbtt_GetGlyphBox(font, glyph, &x0, &y0, &x1, &y1)) {
		if (ix0) *ix0 = 0;
		if (iy0) *iy0 = 0;
		if (ix1) *ix1 = 0;
		if (iy1) *iy1 = 0;
	}
	else {
		if (ix0) *ix0 = STBTT_ifloor(x0 * scale_x + shift_x);
		if (iy0) *iy0 = STBTT_ifloor(-y1 * scale_y + shift_y);
		if (ix1) *ix1 = STBTT_iceil(x1 * scale_x + shift_x);
		if (iy1) *iy1 = STBTT_iceil(-y0 * scale_y + shift_y);
	}
}

STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, glyph, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}

STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, stbtt_FindGlyphIndex(font, codepoint), scale_x, scale_y, shift_x, shift_y, ix0, iy0, ix1, iy1);
}

STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetCodepointBitmapBoxSubpixel(font, codepoint, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}


typedef struct stbtt__hheap_chunk
{
	struct stbtt__hheap_chunk *next;
} stbtt__hheap_chunk;

typedef struct stbtt__hheap
{
	struct stbtt__hheap_chunk *head;
	void   *first_free;
	int    num_remaining_in_head_chunk;
} stbtt__hheap;

static void *stbtt__hheap_alloc(stbtt__hheap *hh, size_t size, void *userdata)
{
	if (hh->first_free) {
		void *p = hh->first_free;
		hh->first_free = *(void **)p;
		return p;
	}
	else {
		if (hh->num_remaining_in_head_chunk == 0) {
			int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
			stbtt__hheap_chunk *c = (stbtt__hheap_chunk *)STBTT_malloc(sizeof(stbtt__hheap_chunk) + size * count, userdata);
			if (c == NULL)
				return NULL;
			c->next = hh->head;
			hh->head = c;
			hh->num_remaining_in_head_chunk = count;
		}
		--hh->num_remaining_in_head_chunk;
		return (char *)(hh->head) + size * hh->num_remaining_in_head_chunk;
	}
}

static void stbtt__hheap_free(stbtt__hheap *hh, void *p)
{
	*(void **)p = hh->first_free;
	hh->first_free = p;
}

static void stbtt__hheap_cleanup(stbtt__hheap *hh, void *userdata)
{
	stbtt__hheap_chunk *c = hh->head;
	while (c) {
		stbtt__hheap_chunk *n = c->next;
		STBTT_free(c, userdata);
		c = n;
	}
}

typedef struct stbtt__edge {
	float x0, y0, x1, y1;
	int invert;
} stbtt__edge;


typedef struct stbtt__active_edge
{
	struct stbtt__active_edge *next;

#if STBTT_RASTERIZER_VERSION==1
	int x, dx;
	float ey;
	int direction;

#elif STBTT_RASTERIZER_VERSION==2
	float fx, fdx, fdy;
	float direction;
	float sy;
	float ey;

#else

#error "Unrecognized value of STBTT_RASTERIZER_VERSION"

#endif
} stbtt__active_edge;

#if STBTT_RASTERIZER_VERSION == 1
#define STBTT_FIXSHIFT   10
#define STBTT_FIX        (1 << STBTT_FIXSHIFT)
#define STBTT_FIXMASK    (STBTT_FIX-1)

static stbtt__active_edge *stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata)
{
	stbtt__active_edge *z = (stbtt__active_edge *)stbtt__hheap_alloc(hh, sizeof(*z), userdata);
	float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
	STBTT_assert(z != NULL);
	if (!z) return z;

	if (dxdy < 0)
		z->dx = -STBTT_ifloor(STBTT_FIX * -dxdy);
	else
		z->dx = STBTT_ifloor(STBTT_FIX * dxdy);

	z->x = STBTT_ifloor(STBTT_FIX * e->x0 + z->dx * (start_point - e->y0));    z->x -= off_x * STBTT_FIX;

	z->ey = e->y1;
	z->next = 0;
	z->direction = e->invert ? 1 : -1;
	return z;
}
#elif STBTT_RASTERIZER_VERSION == 2
static stbtt__active_edge *stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata)
{
	stbtt__active_edge *z = (stbtt__active_edge *)stbtt__hheap_alloc(hh, sizeof(*z), userdata);
	float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
	STBTT_assert(z != NULL);
	if (!z) return z;
	z->fdx = dxdy;
	z->fdy = dxdy != 0.0f ? (1.0f / dxdy) : 0.0f;
	z->fx = e->x0 + dxdy * (start_point - e->y0);
	z->fx -= off_x;
	z->direction = e->invert ? 1.0f : -1.0f;
	z->sy = e->y0;
	z->ey = e->y1;
	z->next = 0;
	return z;
}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#if STBTT_RASTERIZER_VERSION == 1
static void stbtt__fill_active_edges(unsigned char *scanline, int len, stbtt__active_edge *e, int max_weight)
{
	int x0 = 0, w = 0;

	while (e) {
		if (w == 0) {
			x0 = e->x; w += e->direction;
		}
		else {
			int x1 = e->x; w += e->direction;
			if (w == 0) {
				int i = x0 >> STBTT_FIXSHIFT;
				int j = x1 >> STBTT_FIXSHIFT;

				if (i < len && j >= 0) {
					if (i == j) {
						scanline[i] = scanline[i] + (stbtt_uint8)((x1 - x0) * max_weight >> STBTT_FIXSHIFT);
					}
					else {
						if (i >= 0)                      scanline[i] = scanline[i] + (stbtt_uint8)(((STBTT_FIX - (x0 & STBTT_FIXMASK)) * max_weight) >> STBTT_FIXSHIFT);
						else
							i = -1;
						if (j < len)                      scanline[j] = scanline[j] + (stbtt_uint8)(((x1 & STBTT_FIXMASK) * max_weight) >> STBTT_FIXSHIFT);
						else
							j = len;
						for (++i; i < j; ++i)                      scanline[i] = scanline[i] + (stbtt_uint8)max_weight;
					}
				}
			}
		}

		e = e->next;
	}
}

static void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
	stbtt__hheap hh = { 0, 0, 0 };
	stbtt__active_edge *active = NULL;
	int y, j = 0;
	int max_weight = (255 / vsubsample);     int s;    unsigned char scanline_data[512], *scanline;

	if (result->w > 512)
		scanline = (unsigned char *)STBTT_malloc(result->w, userdata);
	else
		scanline = scanline_data;

	y = off_y * vsubsample;
	e[n].y0 = (off_y + result->h) * (float)vsubsample + 1;

	while (j < result->h) {
		STBTT_memset(scanline, 0, result->w);
		for (s = 0; s < vsubsample; ++s) {
			float scan_y = y + 0.5f;
			stbtt__active_edge **step = &active;

			while (*step) {
				stbtt__active_edge * z = *step;
				if (z->ey <= scan_y) {
					*step = z->next;                STBTT_assert(z->direction);
					z->direction = 0;
					stbtt__hheap_free(&hh, z);
				}
				else {
					z->x += z->dx;                step = &((*step)->next);
				}
			}

			for (;;) {
				int changed = 0;
				step = &active;
				while (*step && (*step)->next) {
					if ((*step)->x > (*step)->next->x) {
						stbtt__active_edge *t = *step;
						stbtt__active_edge *q = t->next;

						t->next = q->next;
						q->next = t;
						*step = q;
						changed = 1;
					}
					step = &(*step)->next;
				}
				if (!changed) break;
			}

			while (e->y0 <= scan_y) {
				if (e->y1 > scan_y) {
					stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y, userdata);
					if (z != NULL) {
						if (active == NULL)
							active = z;
						else if (z->x < active->x) {
							z->next = active;
							active = z;
						}
						else {
							stbtt__active_edge *p = active;
							while (p->next && p->next->x < z->x)
								p = p->next;
							z->next = p->next;
							p->next = z;
						}
					}
				}
				++e;
			}

			if (active)
				stbtt__fill_active_edges(scanline, result->w, active, max_weight);

			++y;
		}
		STBTT_memcpy(result->pixels + j * result->stride, scanline, result->w);
		++j;
	}

	stbtt__hheap_cleanup(&hh, userdata);

	if (scanline != scanline_data)
		STBTT_free(scanline, userdata);
}

#elif STBTT_RASTERIZER_VERSION == 2

static void stbtt__handle_clipped_edge(float *scanline, int x, stbtt__active_edge *e, float x0, float y0, float x1, float y1)
{
	if (y0 == y1) return;
	STBTT_assert(y0 < y1);
	STBTT_assert(e->sy <= e->ey);
	if (y0 > e->ey) return;
	if (y1 < e->sy) return;
	if (y0 < e->sy) {
		x0 += (x1 - x0) * (e->sy - y0) / (y1 - y0);
		y0 = e->sy;
	}
	if (y1 > e->ey) {
		x1 += (x1 - x0) * (e->ey - y1) / (y1 - y0);
		y1 = e->ey;
	}

	if (x0 == x)
		STBTT_assert(x1 <= x + 1);
	else if (x0 == x + 1)
		STBTT_assert(x1 >= x);
	else if (x0 <= x)
		STBTT_assert(x1 <= x);
	else if (x0 >= x + 1)
		STBTT_assert(x1 >= x + 1);
	else
		STBTT_assert(x1 >= x && x1 <= x + 1);

	if (x0 <= x && x1 <= x)
		scanline[x] += e->direction * (y1 - y0);
	else if (x0 >= x + 1 && x1 >= x + 1)
		;
	else {
		STBTT_assert(x0 >= x && x0 <= x + 1 && x1 >= x && x1 <= x + 1);
		scanline[x] += e->direction * (y1 - y0) * (1 - ((x0 - x) + (x1 - x)) / 2);
	}
}

static void stbtt__fill_active_edges_new(float *scanline, float *scanline_fill, int len, stbtt__active_edge *e, float y_top)
{
	float y_bottom = y_top + 1;

	while (e) {

		STBTT_assert(e->ey >= y_top);

		if (e->fdx == 0) {
			float x0 = e->fx;
			if (x0 < len) {
				if (x0 >= 0) {
					stbtt__handle_clipped_edge(scanline, (int)x0, e, x0, y_top, x0, y_bottom);
					stbtt__handle_clipped_edge(scanline_fill - 1, (int)x0 + 1, e, x0, y_top, x0, y_bottom);
				}
				else {
					stbtt__handle_clipped_edge(scanline_fill - 1, 0, e, x0, y_top, x0, y_bottom);
				}
			}
		}
		else {
			float x0 = e->fx;
			float dx = e->fdx;
			float xb = x0 + dx;
			float x_top, x_bottom;
			float sy0, sy1;
			float dy = e->fdy;
			STBTT_assert(e->sy <= y_bottom && e->ey >= y_top);

			if (e->sy > y_top) {
				x_top = x0 + dx * (e->sy - y_top);
				sy0 = e->sy;
			}
			else {
				x_top = x0;
				sy0 = y_top;
			}
			if (e->ey < y_bottom) {
				x_bottom = x0 + dx * (e->ey - y_top);
				sy1 = e->ey;
			}
			else {
				x_bottom = xb;
				sy1 = y_bottom;
			}

			if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {

				if ((int)x_top == (int)x_bottom) {
					float height;
					int x = (int)x_top;
					height = sy1 - sy0;
					STBTT_assert(x >= 0 && x < len);
					scanline[x] += e->direction * (1 - ((x_top - x) + (x_bottom - x)) / 2)  * height;
					scanline_fill[x] += e->direction * height;
				}
				else {
					int x, x1, x2;
					float y_crossing, step, sign, area;
					if (x_top > x_bottom) {
						float t;
						sy0 = y_bottom - (sy0 - y_top);
						sy1 = y_bottom - (sy1 - y_top);
						t = sy0, sy0 = sy1, sy1 = t;
						t = x_bottom, x_bottom = x_top, x_top = t;
						dx = -dx;
						dy = -dy;
						t = x0, x0 = xb, xb = t;
					}

					x1 = (int)x_top;
					x2 = (int)x_bottom;
					y_crossing = (x1 + 1 - x0) * dy + y_top;

					sign = e->direction;
					area = sign * (y_crossing - sy0);
					scanline[x1] += area * (1 - ((x_top - x1) + (x1 + 1 - x1)) / 2);

					step = sign * dy;
					for (x = x1 + 1; x < x2; ++x) {
						scanline[x] += area + step / 2;
						area += step;
					}
					y_crossing += dy * (x2 - (x1 + 1));

					STBTT_assert(STBTT_fabs(area) <= 1.01f);

					scanline[x2] += area + sign * (1 - ((x2 - x2) + (x_bottom - x2)) / 2) * (sy1 - y_crossing);

					scanline_fill[x2] += sign * (sy1 - sy0);
				}
			}
			else {
				int x;
				for (x = 0; x < len; ++x) {

					float y0 = y_top;
					float x1 = (float)(x);
					float x2 = (float)(x + 1);
					float x3 = xb;
					float y3 = y_bottom;
					float y1, y2;

					y1 = (x - x0) / dx + y_top;
					y2 = (x + 1 - x0) / dx + y_top;

					if (x0 < x1 && x3 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else if (x3 < x1 && x0 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x0 < x1 && x3 > x1) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x3 < x1 && x0 > x1) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x0 < x2 && x3 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else if (x3 < x2 && x0 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x3, y3);
					}
				}
			}
		}
		e = e->next;
	}
}

static void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
	(void)vsubsample;
	stbtt__hheap hh = { 0, 0, 0 };
	stbtt__active_edge *active = NULL;
	int y, j = 0, i;
	float scanline_data[129], *scanline, *scanline2;

	if (result->w > 64)
		scanline = (float *)STBTT_malloc((result->w * 2 + 1) * sizeof(float), userdata);
	else
		scanline = scanline_data;

	scanline2 = scanline + result->w;

	y = off_y;
	e[n].y0 = (float)(off_y + result->h) + 1;

	while (j < result->h) {
		float scan_y_top = y + 0.0f;
		float scan_y_bottom = y + 1.0f;
		stbtt__active_edge **step = &active;

		STBTT_memset(scanline, 0, result->w * sizeof(scanline[0]));
		STBTT_memset(scanline2, 0, (result->w + 1) * sizeof(scanline[0]));

		while (*step) {
			stbtt__active_edge * z = *step;
			if (z->ey <= scan_y_top) {
				*step = z->next;             STBTT_assert(z->direction);
				z->direction = 0;
				stbtt__hheap_free(&hh, z);
			}
			else {
				step = &((*step)->next);
			}
		}

		while (e->y0 <= scan_y_bottom) {
			if (e->y0 != e->y1) {
				stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y_top, userdata);
				if (z != NULL) {
					STBTT_assert(z->ey >= scan_y_top);
					z->next = active;
					active = z;
				}
			}
			++e;
		}

		if (active)
			stbtt__fill_active_edges_new(scanline, scanline2 + 1, result->w, active, scan_y_top);

		{
			float sum = 0;
			for (i = 0; i < result->w; ++i) {
				float k;
				int m;
				sum += scanline2[i];
				k = scanline[i] + sum;
				k = (float)STBTT_fabs(k) * 255 + 0.5f;
				m = (int)k;
				if (m > 255) m = 255;
				result->pixels[j*result->stride + i] = (unsigned char)m;
			}
		}
		step = &active;
		while (*step) {
			stbtt__active_edge *z = *step;
			z->fx += z->fdx;          step = &((*step)->next);
		}

		++y;
		++j;
	}

	stbtt__hheap_cleanup(&hh, userdata);

	if (scanline != scanline_data)
		STBTT_free(scanline, userdata);
}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#define STBTT__COMPARE(a,b)  ((a)->y0 < (b)->y0)

static void stbtt__sort_edges_ins_sort(stbtt__edge *p, int n)
{
	int i, j;
	for (i = 1; i < n; ++i) {
		stbtt__edge t = p[i], *a = &t;
		j = i;
		while (j > 0) {
			stbtt__edge *b = &p[j - 1];
			int c = STBTT__COMPARE(a, b);
			if (!c) break;
			p[j] = p[j - 1];
			--j;
		}
		if (i != j)
			p[j] = t;
	}
}

static void stbtt__sort_edges_quicksort(stbtt__edge *p, int n)
{

	while (n > 12) {
		stbtt__edge t;
		int c01, c12, c, m, i, j;


		m = n >> 1;
		c01 = STBTT__COMPARE(&p[0], &p[m]);
		c12 = STBTT__COMPARE(&p[m], &p[n - 1]);

		if (c01 != c12) {

			int z;
			c = STBTT__COMPARE(&p[0], &p[n - 1]);


			z = (c == c12) ? 0 : n - 1;
			t = p[z];
			p[z] = p[m];
			p[m] = t;
		}


		t = p[0];
		p[0] = p[m];
		p[m] = t;


		i = 1;
		j = n - 1;
		for (;;) {


			for (;; ++i) {
				if (!STBTT__COMPARE(&p[i], &p[0])) break;
			}
			for (;; --j) {
				if (!STBTT__COMPARE(&p[0], &p[j])) break;
			}

			if (i >= j) break;
			t = p[i];
			p[i] = p[j];
			p[j] = t;

			++i;
			--j;
		}

		if (j < (n - i)) {
			stbtt__sort_edges_quicksort(p, j);
			p = p + i;
			n = n - i;
		}
		else {
			stbtt__sort_edges_quicksort(p + i, n - i);
			n = j;
		}
	}
}

static void stbtt__sort_edges(stbtt__edge *p, int n)
{
	stbtt__sort_edges_quicksort(p, n);
	stbtt__sort_edges_ins_sort(p, n);
}

typedef struct
{
	float x, y;
} stbtt__point;

static void stbtt__rasterize(stbtt__bitmap *result, stbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void *userdata)
{
	float y_scale_inv = invert ? -scale_y : scale_y;
	stbtt__edge *e;
	int n, i, j, k, m;
#if STBTT_RASTERIZER_VERSION == 1
	int vsubsample = result->h < 8 ? 15 : 5;
#elif STBTT_RASTERIZER_VERSION == 2
	int vsubsample = 1;
#else

#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

	n = 0;
	for (i = 0; i < windings; ++i)
		n += wcount[i];

	e = (stbtt__edge *)STBTT_malloc(sizeof(*e) * (n + 1), userdata);    if (e == 0) return;
	n = 0;

	m = 0;
	for (i = 0; i < windings; ++i) {
		stbtt__point *p = pts + m;
		m += wcount[i];
		j = wcount[i] - 1;
		for (k = 0; k < wcount[i]; j = k++) {
			int a = k, b = j;
			if (p[j].y == p[k].y)
				continue;
			e[n].invert = 0;
			if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
				e[n].invert = 1;
				a = j, b = k;
			}
			e[n].x0 = p[a].x * scale_x + shift_x;
			e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
			e[n].x1 = p[b].x * scale_x + shift_x;
			e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
			++n;
		}
	}

	stbtt__sort_edges(e, n);

	stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata);

	STBTT_free(e, userdata);
}

static void stbtt__add_point(stbtt__point *points, int n, float x, float y)
{
	if (!points) return;    points[n].x = x;
	points[n].y = y;
}

static int stbtt__tesselate_curve(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
{
	float mx = (x0 + 2 * x1 + x2) / 4;
	float my = (y0 + 2 * y1 + y2) / 4;
	float dx = (x0 + x2) / 2 - mx;
	float dy = (y0 + y2) / 2 - my;
	if (n > 16)       return 1;
	if (dx*dx + dy*dy > objspace_flatness_squared) {
		stbtt__tesselate_curve(points, num_points, x0, y0, (x0 + x1) / 2.0f, (y0 + y1) / 2.0f, mx, my, objspace_flatness_squared, n + 1);
		stbtt__tesselate_curve(points, num_points, mx, my, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, x2, y2, objspace_flatness_squared, n + 1);
	}
	else {
		stbtt__add_point(points, *num_points, x2, y2);
		*num_points = *num_points + 1;
	}
	return 1;
}

static stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours, void *userdata)
{
	stbtt__point *points = 0;
	int num_points = 0;

	float objspace_flatness_squared = objspace_flatness * objspace_flatness;
	int i, n = 0, start = 0, pass;

	for (i = 0; i < num_verts; ++i)
		if (vertices[i].type == STBTT_vmove)
			++n;

	*num_contours = n;
	if (n == 0) return 0;

	*contour_lengths = (int *)STBTT_malloc(sizeof(**contour_lengths) * n, userdata);

	if (*contour_lengths == 0) {
		*num_contours = 0;
		return 0;
	}

	for (pass = 0; pass < 2; ++pass) {
		float x = 0, y = 0;
		if (pass == 1) {
			points = (stbtt__point *)STBTT_malloc(num_points * sizeof(points[0]), userdata);
			if (points == NULL) goto error;
		}
		num_points = 0;
		n = -1;
		for (i = 0; i < num_verts; ++i) {
			switch (vertices[i].type) {
			case STBTT_vmove:
				if (n >= 0)
					(*contour_lengths)[n] = num_points - start;
				++n;
				start = num_points;

				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vline:
				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vcurve:
				stbtt__tesselate_curve(points, &num_points, x, y,
					vertices[i].cx, vertices[i].cy,
					vertices[i].x, vertices[i].y,
					objspace_flatness_squared, 0);
				x = vertices[i].x, y = vertices[i].y;
				break;
			}
		}
		(*contour_lengths)[n] = num_points - start;
	}

	return points;
error:
	STBTT_free(points, userdata);
	STBTT_free(*contour_lengths, userdata);
	*contour_lengths = 0;
	*num_contours = 0;
	return NULL;
}

STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata)
{
	float scale = scale_x > scale_y ? scale_y : scale_x;
	int winding_count, *winding_lengths;
	stbtt__point *windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
	if (windings) {
		stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
		STBTT_free(winding_lengths, userdata);
		STBTT_free(windings, userdata);
	}
}

STBTT_DEF void stbtt_FreeBitmap(unsigned char *bitmap, void *userdata)
{
	STBTT_free(bitmap, userdata);
}

STBTT_DEF unsigned char *stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	int ix0, iy0, ix1, iy1;
	stbtt__bitmap gbm;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);

	if (scale_x == 0) scale_x = scale_y;
	if (scale_y == 0) {
		if (scale_x == 0) {
			STBTT_free(vertices, info->userdata);
			return NULL;
		}
		scale_y = scale_x;
	}

	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, &ix1, &iy1);

	gbm.w = (ix1 - ix0);
	gbm.h = (iy1 - iy0);
	gbm.pixels = NULL;
	if (width) *width = gbm.w;
	if (height) *height = gbm.h;
	if (xoff) *xoff = ix0;
	if (yoff) *yoff = iy0;

	if (gbm.w && gbm.h) {
		gbm.pixels = (unsigned char *)STBTT_malloc(gbm.w * gbm.h, info->userdata);
		if (gbm.pixels) {
			gbm.stride = gbm.w;

			stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
		}
	}
	STBTT_free(vertices, info->userdata);
	return gbm.pixels;
}

STBTT_DEF unsigned char *stbtt_GetGlyphBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, glyph, width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph)
{
	int ix0, iy0;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);
	stbtt__bitmap gbm;

	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, 0, 0);
	gbm.pixels = output;
	gbm.w = out_w;
	gbm.h = out_h;
	gbm.stride = out_stride;

	if (gbm.w && gbm.h)
		stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);

	STBTT_free(vertices, info->userdata);
}

STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, glyph);
}

STBTT_DEF unsigned char *stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint), width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint));
}

STBTT_DEF unsigned char *stbtt_GetCodepointBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetCodepointBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, codepoint, width, height, xoff, yoff);
}

STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint)
{
	stbtt_MakeCodepointBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, codepoint);
}


STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char *data, int offset, float pixel_height, unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata)
{
	float scale;
	int x, y, bottom_y, i;
	stbtt_fontinfo f;
	f.userdata = NULL;
	if (!stbtt_InitFont(&f, data, offset))
		return -1;
	STBTT_memset(pixels, 0, pw*ph);    x = y = 1;
	bottom_y = 1;

	scale = stbtt_ScaleForPixelHeight(&f, pixel_height);

	for (i = 0; i < num_chars; ++i) {
		int advance, lsb, x0, y0, x1, y1, gw, gh;
		int g = stbtt_FindGlyphIndex(&f, first_char + i);
		stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
		stbtt_GetGlyphBitmapBox(&f, g, scale, scale, &x0, &y0, &x1, &y1);
		gw = x1 - x0;
		gh = y1 - y0;
		if (x + gw + 1 >= pw)
			y = bottom_y, x = 1;       if (y + gh + 1 >= ph)          return -i;
		STBTT_assert(x + gw < pw);
		STBTT_assert(y + gh < ph);
		stbtt_MakeGlyphBitmap(&f, pixels + x + y*pw, gw, gh, pw, scale, scale, g);
		chardata[i].x0 = (stbtt_int16)x;
		chardata[i].y0 = (stbtt_int16)y;
		chardata[i].x1 = (stbtt_int16)(x + gw);
		chardata[i].y1 = (stbtt_int16)(y + gh);
		chardata[i].xadvance = scale * advance;
		chardata[i].xoff = (float)x0;
		chardata[i].yoff = (float)y0;
		x = x + gw + 1;
		if (y + gh + 1 > bottom_y)
			bottom_y = y + gh + 1;
	}
	return bottom_y;
}

STBTT_DEF void stbtt_GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule)
{
	float d3d_bias = opengl_fillrule ? 0 : -0.5f;
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_bakedchar *b = chardata + char_index;
	int round_x = STBTT_ifloor((*xpos + b->xoff) + 0.5f);
	int round_y = STBTT_ifloor((*ypos + b->yoff) + 0.5f);

	q->x0 = round_x + d3d_bias;
	q->y0 = round_y + d3d_bias;
	q->x1 = round_x + b->x1 - b->x0 + d3d_bias;
	q->y1 = round_y + b->y1 - b->y0 + d3d_bias;

	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;

	*xpos += b->xadvance;
}


#ifndef STB_RECT_PACK_VERSION
#ifdef _MSC_VER
#define STBTT__NOTUSED(v)  (void)(v)
#else
#define STBTT__NOTUSED(v)  (void)sizeof(v)
#endif

typedef int stbrp_coord;


typedef struct
{
	int width, height;
	int x, y, bottom_y;
} stbrp_context;

typedef struct
{
	unsigned char x;
} stbrp_node;

struct stbrp_rect
{
	stbrp_coord x, y;
	int id, w, h, was_packed;
};

static void stbrp_init_target(stbrp_context *con, int pw, int ph, stbrp_node *nodes, int num_nodes)
{
	con->width = pw;
	con->height = ph;
	con->x = 0;
	con->y = 0;
	con->bottom_y = 0;
	STBTT__NOTUSED(nodes);
	STBTT__NOTUSED(num_nodes);
}

static void stbrp_pack_rects(stbrp_context *con, stbrp_rect *rects, int num_rects)
{
	int i;
	for (i = 0; i < num_rects; ++i) {
		if (con->x + rects[i].w > con->width) {
			con->x = 0;
			con->y = con->bottom_y;
		}
		if (con->y + rects[i].h > con->height)
			break;
		rects[i].x = con->x;
		rects[i].y = con->y;
		rects[i].was_packed = 1;
		con->x += rects[i].w;
		if (con->y + rects[i].h > con->bottom_y)
			con->bottom_y = con->y + rects[i].h;
	}
	for (; i < num_rects; ++i)
		rects[i].was_packed = 0;
}
#endif


STBTT_DEF int stbtt_PackBegin(stbtt_pack_context *spc, unsigned char *pixels, int pw, int ph, int stride_in_bytes, int padding, void *alloc_context)
{
	stbrp_context *context = (stbrp_context *)STBTT_malloc(sizeof(*context), alloc_context);
	int            num_nodes = pw - padding;
	stbrp_node    *nodes = (stbrp_node    *)STBTT_malloc(sizeof(*nodes) * num_nodes, alloc_context);

	if (context == NULL || nodes == NULL) {
		if (context != NULL) STBTT_free(context, alloc_context);
		if (nodes != NULL) STBTT_free(nodes, alloc_context);
		return 0;
	}

	spc->user_allocator_context = alloc_context;
	spc->width = pw;
	spc->height = ph;
	spc->pixels = pixels;
	spc->pack_info = context;
	spc->nodes = nodes;
	spc->padding = padding;
	spc->stride_in_bytes = stride_in_bytes != 0 ? stride_in_bytes : pw;
	spc->h_oversample = 1;
	spc->v_oversample = 1;

	stbrp_init_target(context, pw - padding, ph - padding, nodes, num_nodes);

	if (pixels)
		STBTT_memset(pixels, 0, pw*ph);
	return 1;
}

STBTT_DEF void stbtt_PackEnd(stbtt_pack_context *spc)
{
	STBTT_free(spc->nodes, spc->user_allocator_context);
	STBTT_free(spc->pack_info, spc->user_allocator_context);
}

STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context *spc, unsigned int h_oversample, unsigned int v_oversample)
{
	STBTT_assert(h_oversample <= STBTT_MAX_OVERSAMPLE);
	STBTT_assert(v_oversample <= STBTT_MAX_OVERSAMPLE);
	if (h_oversample <= STBTT_MAX_OVERSAMPLE)
		spc->h_oversample = h_oversample;
	if (v_oversample <= STBTT_MAX_OVERSAMPLE)
		spc->v_oversample = v_oversample;
}

#define STBTT__OVER_MASK  (STBTT_MAX_OVERSAMPLE-1)

static void stbtt__h_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_w = w - kernel_width;
	int j;
	STBTT_memset(buffer, 0, STBTT_MAX_OVERSAMPLE);    for (j = 0; j < h; ++j) {
		int i;
		unsigned int total;
		STBTT_memset(buffer, 0, kernel_width);

		total = 0;

		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 4);
			}
			break;
		case 5:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 5);
			}
			break;
		default:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / kernel_width);
			}
			break;
		}

		for (; i < w; ++i) {
			STBTT_assert(pixels[i] == 0);
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i] = (unsigned char)(total / kernel_width);
		}

		pixels += stride_in_bytes;
	}
}

static void stbtt__v_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_h = h - kernel_width;
	int j;
	STBTT_memset(buffer, 0, STBTT_MAX_OVERSAMPLE);    for (j = 0; j < w; ++j) {
		int i;
		unsigned int total;
		STBTT_memset(buffer, 0, kernel_width);

		total = 0;

		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 4);
			}
			break;
		case 5:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 5);
			}
			break;
		default:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
			}
			break;
		}

		for (; i < h; ++i) {
			STBTT_assert(pixels[i*stride_in_bytes] == 0);
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
		}

		pixels += 1;
	}
}

static float stbtt__oversample_shift(int oversample)
{
	if (!oversample)
		return 0.0f;

	return (float)-(oversample - 1) / (2.0f * (float)oversample);
}

STBTT_DEF int stbtt_PackFontRangesGatherRects(stbtt_pack_context *spc, stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects)
{
	int i, j, k;

	k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
		ranges[i].h_oversample = (unsigned char)spc->h_oversample;
		ranges[i].v_oversample = (unsigned char)spc->v_oversample;
		for (j = 0; j < ranges[i].num_chars; ++j) {
			int x0, y0, x1, y1;
			int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
			int glyph = stbtt_FindGlyphIndex(info, codepoint);
			stbtt_GetGlyphBitmapBoxSubpixel(info, glyph,
				scale * spc->h_oversample,
				scale * spc->v_oversample,
				0, 0,
				&x0, &y0, &x1, &y1);
			rects[k].w = (stbrp_coord)(x1 - x0 + spc->padding + spc->h_oversample - 1);
			rects[k].h = (stbrp_coord)(y1 - y0 + spc->padding + spc->v_oversample - 1);
			++k;
		}
	}

	return k;
}

STBTT_DEF int stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context *spc, stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects)
{
	int i, j, k, return_value = 1;

	int old_h_over = spc->h_oversample;
	int old_v_over = spc->v_oversample;

	k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
		float recip_h, recip_v, sub_x, sub_y;
		spc->h_oversample = ranges[i].h_oversample;
		spc->v_oversample = ranges[i].v_oversample;
		recip_h = 1.0f / spc->h_oversample;
		recip_v = 1.0f / spc->v_oversample;
		sub_x = stbtt__oversample_shift(spc->h_oversample);
		sub_y = stbtt__oversample_shift(spc->v_oversample);
		for (j = 0; j < ranges[i].num_chars; ++j) {
			stbrp_rect *r = &rects[k];
			if (r->was_packed) {
				stbtt_packedchar *bc = &ranges[i].chardata_for_range[j];
				int advance, lsb, x0, y0, x1, y1;
				int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
				int glyph = stbtt_FindGlyphIndex(info, codepoint);
				stbrp_coord pad = (stbrp_coord)spc->padding;

				r->x += pad;
				r->y += pad;
				r->w -= pad;
				r->h -= pad;
				stbtt_GetGlyphHMetrics(info, glyph, &advance, &lsb);
				stbtt_GetGlyphBitmapBox(info, glyph,
					scale * spc->h_oversample,
					scale * spc->v_oversample,
					&x0, &y0, &x1, &y1);
				stbtt_MakeGlyphBitmapSubpixel(info,
					spc->pixels + r->x + r->y*spc->stride_in_bytes,
					r->w - spc->h_oversample + 1,
					r->h - spc->v_oversample + 1,
					spc->stride_in_bytes,
					scale * spc->h_oversample,
					scale * spc->v_oversample,
					0, 0,
					glyph);

				if (spc->h_oversample > 1)
					stbtt__h_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
						r->w, r->h, spc->stride_in_bytes,
						spc->h_oversample);

				if (spc->v_oversample > 1)
					stbtt__v_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
						r->w, r->h, spc->stride_in_bytes,
						spc->v_oversample);

				bc->x0 = (stbtt_int16)r->x;
				bc->y0 = (stbtt_int16)r->y;
				bc->x1 = (stbtt_int16)(r->x + r->w);
				bc->y1 = (stbtt_int16)(r->y + r->h);
				bc->xadvance = scale * advance;
				bc->xoff = (float)x0 * recip_h + sub_x;
				bc->yoff = (float)y0 * recip_v + sub_y;
				bc->xoff2 = (x0 + r->w) * recip_h + sub_x;
				bc->yoff2 = (y0 + r->h) * recip_v + sub_y;
			}
			else {
				return_value = 0;
			}

			++k;
		}
	}

	spc->h_oversample = old_h_over;
	spc->v_oversample = old_v_over;

	return return_value;
}

STBTT_DEF void stbtt_PackFontRangesPackRects(stbtt_pack_context *spc, stbrp_rect *rects, int num_rects)
{
	stbrp_pack_rects((stbrp_context *)spc->pack_info, rects, num_rects);
}

STBTT_DEF int stbtt_PackFontRanges(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, stbtt_pack_range *ranges, int num_ranges)
{
	stbtt_fontinfo info;
	int i, j, n, return_value = 1;
	stbrp_rect    *rects;

	for (i = 0; i < num_ranges; ++i)
		for (j = 0; j < ranges[i].num_chars; ++j)
			ranges[i].chardata_for_range[j].x0 =
			ranges[i].chardata_for_range[j].y0 =
			ranges[i].chardata_for_range[j].x1 =
			ranges[i].chardata_for_range[j].y1 = 0;

	n = 0;
	for (i = 0; i < num_ranges; ++i)
		n += ranges[i].num_chars;

	rects = (stbrp_rect *)STBTT_malloc(sizeof(*rects) * n, spc->user_allocator_context);
	if (rects == NULL)
		return 0;

	info.userdata = spc->user_allocator_context;
	stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(fontdata, font_index));

	n = stbtt_PackFontRangesGatherRects(spc, &info, ranges, num_ranges, rects);

	stbtt_PackFontRangesPackRects(spc, rects, n);

	return_value = stbtt_PackFontRangesRenderIntoRects(spc, &info, ranges, num_ranges, rects);

	STBTT_free(rects, spc->user_allocator_context);
	return return_value;
}

STBTT_DEF int stbtt_PackFontRange(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, float font_size,
	int first_unicode_codepoint_in_range, int num_chars_in_range, stbtt_packedchar *chardata_for_range)
{
	stbtt_pack_range range;
	range.first_unicode_codepoint_in_range = first_unicode_codepoint_in_range;
	range.array_of_unicode_codepoints = NULL;
	range.num_chars = num_chars_in_range;
	range.chardata_for_range = chardata_for_range;
	range.font_size = font_size;
	return stbtt_PackFontRanges(spc, fontdata, font_index, &range, 1);
}

STBTT_DEF void stbtt_GetPackedQuad(stbtt_packedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int align_to_integer)
{
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_packedchar *b = chardata + char_index;

	if (align_to_integer) {
		float x = (float)STBTT_ifloor((*xpos + b->xoff) + 0.5f);
		float y = (float)STBTT_ifloor((*ypos + b->yoff) + 0.5f);
		q->x0 = x;
		q->y0 = y;
		q->x1 = x + b->xoff2 - b->xoff;
		q->y1 = y + b->yoff2 - b->yoff;
	}
	else {
		q->x0 = *xpos + b->xoff;
		q->y0 = *ypos + b->yoff;
		q->x1 = *xpos + b->xoff2;
		q->y1 = *ypos + b->yoff2;
	}

	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;

	*xpos += b->xadvance;
}



static stbtt_int32 stbtt__CompareUTF8toUTF16_bigendian_prefix(const stbtt_uint8 *s1, stbtt_int32 len1, const stbtt_uint8 *s2, stbtt_int32 len2)
{
	stbtt_int32 i = 0;

	while (len2) {
		stbtt_uint16 ch = s2[0] * 256 + s2[1];
		if (ch < 0x80) {
			if (i >= len1) return -1;
			if (s1[i++] != ch) return -1;
		}
		else if (ch < 0x800) {
			if (i + 1 >= len1) return -1;
			if (s1[i++] != 0xc0 + (ch >> 6)) return -1;
			if (s1[i++] != 0x80 + (ch & 0x3f)) return -1;
		}
		else if (ch >= 0xd800 && ch < 0xdc00) {
			stbtt_uint32 c;
			stbtt_uint16 ch2 = s2[2] * 256 + s2[3];
			if (i + 3 >= len1) return -1;
			c = ((ch - 0xd800) << 10) + (ch2 - 0xdc00) + 0x10000;
			if (s1[i++] != 0xf0 + (c >> 18)) return -1;
			if (s1[i++] != 0x80 + ((c >> 12) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c) & 0x3f)) return -1;
			s2 += 2;          len2 -= 2;
		}
		else if (ch >= 0xdc00 && ch < 0xe000) {
			return -1;
		}
		else {
			if (i + 2 >= len1) return -1;
			if (s1[i++] != 0xe0 + (ch >> 12)) return -1;
			if (s1[i++] != 0x80 + ((ch >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((ch) & 0x3f)) return -1;
		}
		s2 += 2;
		len2 -= 2;
	}
	return i;
}

STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char *s1, int len1, const char *s2, int len2)
{
	return len1 == stbtt__CompareUTF8toUTF16_bigendian_prefix((const stbtt_uint8*)s1, len1, (const stbtt_uint8*)s2, len2);
}

STBTT_DEF const char *stbtt_GetFontNameString(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID)
{
	stbtt_int32 i, count, stringOffset;
	stbtt_uint8 *fc = font->data;
	stbtt_uint32 offset = font->fontstart;
	stbtt_uint32 nm = stbtt__find_table(fc, offset, "name");
	if (!nm) return NULL;

	count = ttUSHORT(fc + nm + 2);
	stringOffset = nm + ttUSHORT(fc + nm + 4);
	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		if (platformID == ttUSHORT(fc + loc + 0) && encodingID == ttUSHORT(fc + loc + 2)
			&& languageID == ttUSHORT(fc + loc + 4) && nameID == ttUSHORT(fc + loc + 6)) {
			*length = ttUSHORT(fc + loc + 8);
			return (const char *)(fc + stringOffset + ttUSHORT(fc + loc + 10));
		}
	}
	return NULL;
}

static int stbtt__matchpair(stbtt_uint8 *fc, stbtt_uint32 nm, stbtt_uint8 *name, stbtt_int32 nlen, stbtt_int32 target_id, stbtt_int32 next_id)
{
	stbtt_int32 i;
	stbtt_int32 count = ttUSHORT(fc + nm + 2);
	stbtt_int32 stringOffset = nm + ttUSHORT(fc + nm + 4);

	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		stbtt_int32 id = ttUSHORT(fc + loc + 6);
		if (id == target_id) {
			stbtt_int32 platform = ttUSHORT(fc + loc + 0), encoding = ttUSHORT(fc + loc + 2), language = ttUSHORT(fc + loc + 4);

			if (platform == 0 || (platform == 3 && encoding == 1) || (platform == 3 && encoding == 10)) {
				stbtt_int32 slen = ttUSHORT(fc + loc + 8);
				stbtt_int32 off = ttUSHORT(fc + loc + 10);

				stbtt_int32 matchlen = stbtt__CompareUTF8toUTF16_bigendian_prefix(name, nlen, fc + stringOffset + off, slen);
				if (matchlen >= 0) {
					if (i + 1 < count && ttUSHORT(fc + loc + 12 + 6) == next_id && ttUSHORT(fc + loc + 12) == platform && ttUSHORT(fc + loc + 12 + 2) == encoding && ttUSHORT(fc + loc + 12 + 4) == language) {
						slen = ttUSHORT(fc + loc + 12 + 8);
						off = ttUSHORT(fc + loc + 12 + 10);
						if (slen == 0) {
							if (matchlen == nlen)
								return 1;
						}
						else if (matchlen < nlen && name[matchlen] == ((char)(0xe17 + 4804 - 0x20bb))) {
							++matchlen;
							if (stbtt_CompareUTF8toUTF16_bigendian((char*)(name + matchlen), nlen - matchlen, (char*)(fc + stringOffset + off), slen))
								return 1;
						}
					}
					else {
						if (matchlen == nlen)
							return 1;
					}
				}
			}

		}
	}
	return 0;
}

static int stbtt__matches(stbtt_uint8 *fc, stbtt_uint32 offset, stbtt_uint8 *name, stbtt_int32 flags)
{
	stbtt_int32 nlen = (stbtt_int32)STBTT_strlen((char *)name);
	stbtt_uint32 nm, hd;
	if (!stbtt__isfont(fc + offset)) return 0;

	if (flags) {
		hd = stbtt__find_table(fc, offset, "head");
		if ((ttUSHORT(fc + hd + 44) & 7) != (flags & 7)) return 0;
	}

	nm = stbtt__find_table(fc, offset, "name");
	if (!nm) return 0;

	if (flags) {
		if (stbtt__matchpair(fc, nm, name, nlen, 16, -1))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, -1))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
	}
	else {
		if (stbtt__matchpair(fc, nm, name, nlen, 16, 17))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, 2))  return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
	}

	return 0;
}

STBTT_DEF int stbtt_FindMatchingFont(const unsigned char *font_collection, const char *name_utf8, stbtt_int32 flags)
{
	stbtt_int32 i;
	for (i = 0;; ++i) {
		stbtt_int32 off = stbtt_GetFontOffsetForIndex(font_collection, i);
		if (off < 0) return off;
		if (stbtt__matches((stbtt_uint8 *)font_collection, off, (stbtt_uint8*)name_utf8, flags))
			return off;
	}
}

#endif 



