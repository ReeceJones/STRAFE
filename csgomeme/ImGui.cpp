



#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <ctype.h>      
#include <stdlib.h>     
#include <stdio.h>      
#include <limits.h>     
#if defined(_MSC_VER) && _MSC_VER <= 1500 
#include <stddef.h>     
#else
#include <stdint.h>     
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4127) // condition expression is constant
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wformat-nonliteral"      // warning : format string is not a string literal              // passing non-literal to vsnformat(). yes, user passing incorrect format strings can crash the code.
#pragma clang diagnostic ignored "-Wexit-time-destructors"  // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global destructor           // similar to above, not sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast" // warning : cast to 'void *' from smaller integer type 'int' //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat"                   // warning: format '%p' expects argument of type 'void*', but argument 6 has type 'ImGuiWindow*'
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#endif


static void             LogRenderedText(const ImVec2& ref_pos, const char* text, const char* text_end = NULL);

static void             PushMultiItemsWidths(int components, float w_full = 0.0f);
static float            GetDraggedColumnOffset(int column_index);

static bool             IsKeyPressedMap(ImGuiKey key, bool repeat = true);

static void             SetCurrentFont(ImFont* font);
static void             SetCurrentWindow(ImGuiWindow* window);
static void             SetWindowScrollY(ImGuiWindow* window, float new_scroll_y);
static void             SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiSetCond cond);
static void             SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiSetCond cond);
static void             SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiSetCond cond);
static ImGuiWindow*     FindHoveredWindow(ImVec2 pos, bool excluding_childs);
static ImGuiWindow*     CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags);
static inline bool      IsWindowContentHoverable(ImGuiWindow* window);
static void             ClearSetNextWindowData();
static void             CheckStacksSize(ImGuiWindow* window, bool write);
static void             Scrollbar(ImGuiWindow* window, bool horizontal);

static void             AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list);
static void             AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window);
static void             AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window);

static ImGuiIniData*    FindWindowSettings(const char* name);
static ImGuiIniData*    AddWindowSettings(const char* name);
static void             LoadSettings();
static void             SaveSettings();
static void             MarkSettingsDirty();

static void             PushColumnClipRect(int column_index = -1);
static ImRect           GetVisibleRect();

static bool             BeginPopupEx(const char* str_id, ImGuiWindowFlags extra_flags);
static void             CloseInactivePopups();
static void             ClosePopupToLevel(int remaining);
static void             ClosePopup(ImGuiID id);
static bool             IsPopupOpen(ImGuiID id);
static ImGuiWindow*     GetFrontMostModalRootWindow();
static ImVec2           FindBestPopupWindowPos(const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& rect_to_avoid);

static bool             InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data);
static int              InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
static ImVec2           InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);

static inline void      DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size);
static inline void      DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size);
static void             DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2);
static bool             DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format);


static const char*      GetClipboardTextFn_DefaultImpl();
static void             SetClipboardTextFn_DefaultImpl(const char* text);
static void             ImeSetInputScreenPosFn_DefaultImpl(int x, int y);


static ImGuiContext     GImDefaultContext;
static ImFontAtlas      GImDefaultFontAtlas;

ImGuiContext*           GImGui = &GImDefaultContext;


ImGuiStyle::ImGuiStyle()
{
	Alpha = 1.0f;                 WindowPadding = ImVec2(8, 8);          WindowMinSize = ImVec2(32, 32);        WindowRounding = 9.0f;                 WindowTitleAlign = ImGuiAlign_Left;      ChildWindowRounding = 0.0f;                 FramePadding = ImVec2(4, 3);          FrameRounding = 0.0f;                 ItemSpacing = ImVec2(8, 4);          ItemInnerSpacing = ImVec2(4, 4);          TouchExtraPadding = ImVec2(0, 0);          IndentSpacing = 21.0f;                ColumnsMinSpacing = 6.0f;                 ScrollbarSize = 16.0f;                ScrollbarRounding = 9.0f;                 GrabMinSize = 10.0f;                GrabRounding = 0.0f;                 DisplayWindowPadding = ImVec2(22, 22);        DisplaySafeAreaPadding = ImVec2(4, 4);          AntiAliasedLines = true;                 AntiAliasedShapes = true;                 CurveTessellationTol = 1.25f;
	Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);       Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	Colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	Colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
	Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
	Colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
	Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

ImGuiIO::ImGuiIO()
{
	memset(this, 0, sizeof(*this));

	DisplaySize = ImVec2(-1.0f, -1.0f);
	DeltaTime = 1.0f / 60.0f;
	IniSavingRate = 5.0f;
	IniFilename = "imgui.ini";
	LogFilename = "imgui_log.txt";
	Fonts = &GImDefaultFontAtlas;
	FontGlobalScale = 1.0f;
	DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	MousePos = ImVec2(-1, -1);
	MousePosPrev = ImVec2(-1, -1);
	MouseDoubleClickTime = 0.30f;
	MouseDoubleClickMaxDist = 6.0f;
	MouseDragThreshold = 6.0f;
	for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++)
		MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
	for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++)
		KeysDownDuration[i] = KeysDownDurationPrev[i] = -1.0f;
	for (int i = 0; i < ImGuiKey_COUNT; i++)
		KeyMap[i] = -1;
	KeyRepeatDelay = 0.250f;
	KeyRepeatRate = 0.050f;
	UserData = NULL;

	RenderDrawListsFn = NULL;
	MemAllocFn = malloc;
	MemFreeFn = free;
	GetClipboardTextFn = GetClipboardTextFn_DefaultImpl;       SetClipboardTextFn = SetClipboardTextFn_DefaultImpl;
	ImeSetInputScreenPosFn = ImeSetInputScreenPosFn_DefaultImpl;


#ifdef __APPLE__
	OSXBehaviors = true;
#endif
}

void ImGuiIO::AddInputCharacter(ImWchar c)
{
	const int n = ImStrlenW(InputCharacters);
	if (n + 1 < IM_ARRAYSIZE(InputCharacters))
	{
		InputCharacters[n] = c;
		InputCharacters[n + 1] = '\0';
	}
}

void ImGuiIO::AddInputCharactersUTF8(const char* utf8_chars)
{
	const int wchars_buf_len = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar);
	ImWchar wchars[wchars_buf_len];
	ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL);
	for (int i = 0; i < wchars_buf_len && wchars[i] != 0; i++)
		AddInputCharacter(wchars[i]);
}


#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))               

#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

bool ImIsPointInTriangle(const ImVec2& p, const ImVec2& a, const ImVec2& b, const ImVec2& c)
{
	bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
	bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
	bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;
	return ((b1 == b2) && (b2 == b3));
}

int ImStricmp(const char* str1, const char* str2)
{
	int d;
	while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; }
	return d;
}

int ImStrnicmp(const char* str1, const char* str2, int count)
{
	int d = 0;
	while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; }
	return d;
}

void ImStrncpy(char* dst, const char* src, int count)
{
	if (count < 1) return;
	strncpy(dst, src, (size_t)count);
	dst[count - 1] = 0;
}

char* ImStrdup(const char *str)
{
	size_t len = strlen(str) + 1;
	void* buff = ImGui::MemAlloc(len);
	return (char*)memcpy(buff, (const void*)str, len);
}

int ImStrlenW(const ImWchar* str)
{
	int n = 0;
	while (*str++) n++;
	return n;
}

const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin) {
	while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n')
		buf_mid_line--;
	return buf_mid_line;
}

const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
	if (!needle_end)
		needle_end = needle + strlen(needle);

	const char un0 = (char)toupper(*needle);
	while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end))
	{
		if (toupper(*haystack) == un0)
		{
			const char* b = needle + 1;
			for (const char* a = haystack + 1; b < needle_end; a++, b++)
				if (toupper(*a) != toupper(*b))
					break;
			if (b == needle_end)
				return haystack;
		}
		haystack++;
	}
	return NULL;
}


int ImFormatString(char* buf, int buf_size, const char* fmt, ...)
{
	IM_ASSERT(buf_size > 0);
	va_list args;
	va_start(args, fmt);
	int w = vsnprintf(buf, buf_size, fmt, args);
	va_end(args);
	if (w == -1 || w >= buf_size)
		w = buf_size - 1;
	buf[w] = 0;
	return w;
}

int ImFormatStringV(char* buf, int buf_size, const char* fmt, va_list args)
{
	IM_ASSERT(buf_size > 0);
	int w = vsnprintf(buf, buf_size, fmt, args);
	if (w == -1 || w >= buf_size)
		w = buf_size - 1;
	buf[w] = 0;
	return w;
}

ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
	static ImU32 crc32_lut[256] = { 0 };
	if (!crc32_lut[1])
	{
		const ImU32 polynomial = 0xedb88320;
		for (ImU32 i = 0; i < 256; i++)
		{
			ImU32 crc = i;
			for (ImU32 j = 0; j < 8; j++)
				crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
			crc32_lut[i] = crc;
		}
	}

	seed = ~seed;
	ImU32 crc = seed;
	const unsigned char* current = (const unsigned char*)data;

	if (data_size > 0)
	{
		while (data_size--)
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xff) ^ *current++];
	}
	else
	{
		while (unsigned char c = *current++)
		{
			if (c == ((char)(0x8b2 + 6169 - 0x20a8)) && current[0] == ((char)(0x22c7 + 660 - 0x2538)) && current[1] == ((char)(0x1276 + 2703 - 0x1ce2)))
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xff) ^ c];
		}
	}
	return ~crc;
}


int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
	unsigned int c = (unsigned int)-1;
	const unsigned char* str = (const unsigned char*)in_text;
	if (!(*str & 0x80))
	{
		c = (unsigned int)(*str++);
		*out_char = c;
		return 1;
	}
	if ((*str & 0xe0) == 0xc0)
	{
		*out_char = 0xfffd;         if (in_text_end && in_text_end - (const char*)str < 2) return 1;
		if (*str < 0xc2) return 2;
		c = (unsigned int)((*str++ & 0x1f) << 6);
		if ((*str & 0xc0) != 0x80) return 2;
		c += (*str++ & 0x3f);
		*out_char = c;
		return 2;
	}
	if ((*str & 0xf0) == 0xe0)
	{
		*out_char = 0xfffd;         if (in_text_end && in_text_end - (const char*)str < 3) return 1;
		if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
		if (*str == 0xed && str[1] > 0x9f) return 3;         c = (unsigned int)((*str++ & 0x0f) << 12);
		if ((*str & 0xc0) != 0x80) return 3;
		c += (unsigned int)((*str++ & 0x3f) << 6);
		if ((*str & 0xc0) != 0x80) return 3;
		c += (*str++ & 0x3f);
		*out_char = c;
		return 3;
	}
	if ((*str & 0xf8) == 0xf0)
	{
		*out_char = 0xfffd;         if (in_text_end && in_text_end - (const char*)str < 4) return 1;
		if (*str > 0xf4) return 4;
		if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
		if (*str == 0xf4 && str[1] > 0x8f) return 4;         c = (unsigned int)((*str++ & 0x07) << 18);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (unsigned int)((*str++ & 0x3f) << 12);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (unsigned int)((*str++ & 0x3f) << 6);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (*str++ & 0x3f);
		if ((c & 0xfffff800) == 0xd800) return 4;
		*out_char = c;
		return 4;
	}
	*out_char = 0;
	return 0;
}

int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
	ImWchar* buf_out = buf;
	ImWchar* buf_end = buf + buf_size;
	while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c;
		in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
		if (c == 0)
			break;
		if (c < 0x10000)                *buf_out++ = (ImWchar)c;
	}
	*buf_out = 0;
	if (in_text_remaining)
		*in_text_remaining = in_text;
	return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
	int char_count = 0;
	while ((!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c;
		in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
		if (c == 0)
			break;
		if (c < 0x10000)
			char_count++;
	}
	return char_count;
}

static inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
	if (c < 0x80)
	{
		buf[0] = (char)c;
		return 1;
	}
	if (c < 0x800)
	{
		if (buf_size < 2) return 0;
		buf[0] = (char)(0xc0 + (c >> 6));
		buf[1] = (char)(0x80 + (c & 0x3f));
		return 2;
	}
	if (c >= 0xdc00 && c < 0xe000)
	{
		return 0;
	}
	if (c >= 0xd800 && c < 0xdc00)
	{
		if (buf_size < 4) return 0;
		buf[0] = (char)(0xf0 + (c >> 18));
		buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
		buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[3] = (char)(0x80 + ((c) & 0x3f));
		return 4;
	}
	{
		if (buf_size < 3) return 0;
		buf[0] = (char)(0xe0 + (c >> 12));
		buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[2] = (char)(0x80 + ((c) & 0x3f));
		return 3;
	}
}

static inline int ImTextCountUtf8BytesFromChar(unsigned int c)
{
	if (c < 0x80) return 1;
	if (c < 0x800) return 2;
	if (c >= 0xdc00 && c < 0xe000) return 0;
	if (c >= 0xd800 && c < 0xdc00) return 4;
	return 3;
}

int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
	char* buf_out = buf;
	const char* buf_end = buf + buf_size;
	while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c = (unsigned int)(*in_text++);
		if (c < 0x80)
			*buf_out++ = (char)c;
		else
			buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
	}
	*buf_out = 0;
	return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
	int bytes_count = 0;
	while ((!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c = (unsigned int)(*in_text++);
		if (c < 0x80)
			bytes_count++;
		else
			bytes_count += ImTextCountUtf8BytesFromChar(c);
	}
	return bytes_count;
}

ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in)
{
	float s = 1.0f / 255.0f;
	return ImVec4((in & 0xff) * s, ((in >> 8) & 0xff) * s, ((in >> 16) & 0xff) * s, (in >> 24) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4& in)
{
	ImU32 out;
	out = ((ImU32)IM_F32_TO_INT8_SAT(in.x));
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << 8;
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << 16;
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << 24;
	return out;
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul)
{
	ImVec4 c = GImGui->Style.Colors[idx];
	c.w *= GImGui->Style.Alpha * alpha_mul;
	return ImGui::ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(const ImVec4& col)
{
	ImVec4 c = col;
	c.w *= GImGui->Style.Alpha;
	return ImGui::ColorConvertFloat4ToU32(c);
}

void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	float K = 0.f;
	if (g < b)
	{
		const float tmp = g; g = b; b = tmp;
		K = -1.f;
	}
	if (r < g)
	{
		const float tmp = r; r = g; g = tmp;
		K = -2.f / 6.f - K;
	}

	const float chroma = r - (g < b ? g : b);
	out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
	out_s = chroma / (r + 1e-20f);
	out_v = r;
}

void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

void* ImLoadFileToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
	IM_ASSERT(filename && file_open_mode);
	if (out_file_size)
		*out_file_size = 0;

	FILE* f;
	if ((f = fopen(filename, file_open_mode)) == NULL)
		return NULL;

	long file_size_signed;
	if (fseek(f, 0, SEEK_END) || (file_size_signed = ftell(f)) == -1 || fseek(f, 0, SEEK_SET))
	{
		fclose(f);
		return NULL;
	}

	int file_size = (int)file_size_signed;
	void* file_data = ImGui::MemAlloc(file_size + padding_bytes);
	if (file_data == NULL)
	{
		fclose(f);
		return NULL;
	}
	if (fread(file_data, 1, (size_t)file_size, f) != (size_t)file_size)
	{
		fclose(f);
		ImGui::MemFree(file_data);
		return NULL;
	}
	if (padding_bytes > 0)
		memset((void *)(((char*)file_data) + file_size), 0, padding_bytes);

	fclose(f);
	if (out_file_size)
		*out_file_size = file_size;

	return file_data;
}


void ImGuiStorage::Clear()
{
	Data.clear();
}

static ImVector<ImGuiStorage::Pair>::iterator LowerBound(ImVector<ImGuiStorage::Pair>& data, ImGuiID key)
{
	ImVector<ImGuiStorage::Pair>::iterator first = data.begin();
	ImVector<ImGuiStorage::Pair>::iterator last = data.end();
	int count = (int)(last - first);
	while (count > 0)
	{
		int count2 = count / 2;
		ImVector<ImGuiStorage::Pair>::iterator mid = first + count2;
		if (mid->key < key)
		{
			first = ++mid;
			count -= count2 + 1;
		}
		else
		{
			count = count2;
		}
	}
	return first;
}

int ImGuiStorage::GetInt(ImGuiID key, int default_val) const
{
	ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
	if (it == Data.end() || it->key != key)
		return default_val;
	return it->val_i;
}

bool ImGuiStorage::GetBool(ImGuiID key, bool default_val) const
{
	return GetInt(key, default_val ? 1 : 0) != 0;
}

float ImGuiStorage::GetFloat(ImGuiID key, float default_val) const
{
	ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
	if (it == Data.end() || it->key != key)
		return default_val;
	return it->val_f;
}

void* ImGuiStorage::GetVoidPtr(ImGuiID key) const
{
	ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
	if (it == Data.end() || it->key != key)
		return NULL;
	return it->val_p;
}

int* ImGuiStorage::GetIntRef(ImGuiID key, int default_val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
		it = Data.insert(it, Pair(key, default_val));
	return &it->val_i;
}

bool* ImGuiStorage::GetBoolRef(ImGuiID key, bool default_val)
{
	return (bool*)GetIntRef(key, default_val ? 1 : 0);
}

float* ImGuiStorage::GetFloatRef(ImGuiID key, float default_val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
		it = Data.insert(it, Pair(key, default_val));
	return &it->val_f;
}

void** ImGuiStorage::GetVoidPtrRef(ImGuiID key, void* default_val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
		it = Data.insert(it, Pair(key, default_val));
	return &it->val_p;
}

void ImGuiStorage::SetInt(ImGuiID key, int val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
	{
		Data.insert(it, Pair(key, val));
		return;
	}
	it->val_i = val;
}

void ImGuiStorage::SetBool(ImGuiID key, bool val)
{
	SetInt(key, val ? 1 : 0);
}

void ImGuiStorage::SetFloat(ImGuiID key, float val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
	{
		Data.insert(it, Pair(key, val));
		return;
	}
	it->val_f = val;
}

void ImGuiStorage::SetVoidPtr(ImGuiID key, void* val)
{
	ImVector<Pair>::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
	{
		Data.insert(it, Pair(key, val));
		return;
	}
	it->val_p = val;
}

void ImGuiStorage::SetAllInt(int v)
{
	for (int i = 0; i < Data.Size; i++)
		Data[i].val_i = v;
}


ImGuiTextFilter::ImGuiTextFilter(const char* default_filter)
{
	if (default_filter)
	{
		ImStrncpy(InputBuf, default_filter, IM_ARRAYSIZE(InputBuf));
		Build();
	}
	else
	{
		InputBuf[0] = 0;
		CountGrep = 0;
	}
}

bool ImGuiTextFilter::Draw(const char* label, float width)
{
	if (width != 0.0f)
		ImGui::PushItemWidth(width);
	bool value_changed = ImGui::InputText(label, InputBuf, IM_ARRAYSIZE(InputBuf));
	if (width != 0.0f)
		ImGui::PopItemWidth();
	if (value_changed)
		Build();
	return value_changed;
}

void ImGuiTextFilter::TextRange::split(char separator, ImVector<TextRange>& out)
{
	out.resize(0);
	const char* wb = b;
	const char* we = wb;
	while (we < e)
	{
		if (*we == separator)
		{
			out.push_back(TextRange(wb, we));
			wb = we + 1;
		}
		we++;
	}
	if (wb != we)
		out.push_back(TextRange(wb, we));
}

void ImGuiTextFilter::Build()
{
	Filters.resize(0);
	TextRange input_range(InputBuf, InputBuf + strlen(InputBuf));
	input_range.split(((char)(0x688 + 5342 - 0x1b3a)), Filters);

	CountGrep = 0;
	for (int i = 0; i != Filters.Size; i++)
	{
		Filters[i].trim_blanks();
		if (Filters[i].empty())
			continue;
		if (Filters[i].front() != ((char)(0x2512 + 150 - 0x257b)))
			CountGrep += 1;
	}
}

bool ImGuiTextFilter::PassFilter(const char* text, const char* text_end) const
{
	if (Filters.empty())
		return true;

	if (text == NULL)
		text = "";

	for (int i = 0; i != Filters.Size; i++)
	{
		const TextRange& f = Filters[i];
		if (f.empty())
			continue;
		if (f.front() == ((char)(0x2bb + 7281 - 0x1eff)))
		{
			if (ImStristr(text, text_end, f.begin() + 1, f.end()) != NULL)
				return false;
		}
		else
		{
			if (ImStristr(text, text_end, f.begin(), f.end()) != NULL)
				return true;
		}
	}

	if (CountGrep == 0)
		return true;

	return false;
}


#ifndef va_copy
#define va_copy(dest, src) (dest = src)
#endif

void ImGuiTextBuffer::appendv(const char* fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);

	int len = vsnprintf(NULL, 0, fmt, args);             if (len <= 0)
		return;

	const int write_off = Buf.Size;
	const int needed_sz = write_off + len;
	if (write_off + len >= Buf.Capacity)
	{
		int double_capacity = Buf.Capacity * 2;
		Buf.reserve(needed_sz > double_capacity ? needed_sz : double_capacity);
	}

	Buf.resize(needed_sz);
	ImFormatStringV(&Buf[write_off] - 1, len + 1, fmt, args_copy);
}

void ImGuiTextBuffer::append(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	appendv(fmt, args);
	va_end(args);
}


ImGuiSimpleColumns::ImGuiSimpleColumns()
{
	Count = 0;
	Spacing = Width = NextWidth = 0.0f;
	memset(Pos, 0, sizeof(Pos));
	memset(NextWidths, 0, sizeof(NextWidths));
}

void ImGuiSimpleColumns::Update(int count, float spacing, bool clear)
{
	IM_ASSERT(Count <= IM_ARRAYSIZE(Pos));
	Count = count;
	Width = NextWidth = 0.0f;
	Spacing = spacing;
	if (clear) memset(NextWidths, 0, sizeof(NextWidths));
	for (int i = 0; i < Count; i++)
	{
		if (i > 0 && NextWidths[i] > 0.0f)
			Width += Spacing;
		Pos[i] = (float)(int)Width;
		Width += NextWidths[i];
		NextWidths[i] = 0.0f;
	}
}

float ImGuiSimpleColumns::DeclColumns(float w0, float w1, float w2) {
	NextWidth = 0.0f;
	NextWidths[0] = ImMax(NextWidths[0], w0);
	NextWidths[1] = ImMax(NextWidths[1], w1);
	NextWidths[2] = ImMax(NextWidths[2], w2);
	for (int i = 0; i < 3; i++)
		NextWidth += NextWidths[i] + ((i > 0 && NextWidths[i] > 0.0f) ? Spacing : 0.0f);
	return ImMax(Width, NextWidth);
}

float ImGuiSimpleColumns::CalcExtraSpace(float avail_w)
{
	return ImMax(0.0f, avail_w - Width);
}


static void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height)
{
	ImGui::SetCursorPosY(pos_y);
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;          window->DC.PrevLineHeight = (line_height - GImGui->Style.ItemSpacing.y);        if (window->DC.ColumnsCount > 1)
		window->DC.ColumnsCellMinY = window->DC.CursorPos.y;
}

void ImGuiListClipper::Begin(int count, float items_height)
{
	StartPosY = ImGui::GetCursorPosY();
	ItemsHeight = items_height;
	ItemsCount = count;
	StepNo = 0;
	DisplayEnd = DisplayStart = -1;
	if (ItemsHeight > 0.0f)
	{
		ImGui::CalcListClipping(ItemsCount, ItemsHeight, &DisplayStart, &DisplayEnd);         if (DisplayStart > 0)
			SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight, ItemsHeight);         StepNo = 2;
	}
}

void ImGuiListClipper::End()
{
	if (ItemsCount < 0)
		return;
	if (ItemsCount < INT_MAX)
		SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight, ItemsHeight);     ItemsCount = -1;
	StepNo = 3;
}

bool ImGuiListClipper::Step()
{
	if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems)
	{
		ItemsCount = -1;
		return false;
	}
	if (StepNo == 0) {
		DisplayStart = 0;
		DisplayEnd = 1;
		StartPosY = ImGui::GetCursorPosY();
		StepNo = 1;
		return true;
	}
	if (StepNo == 1) {
		if (ItemsCount == 1) { ItemsCount = -1; return false; }
		float items_height = ImGui::GetCursorPosY() - StartPosY;
		IM_ASSERT(items_height > 0.0f);           Begin(ItemsCount - 1, items_height);
		DisplayStart++;
		DisplayEnd++;
		StepNo = 3;
		return true;
	}
	if (StepNo == 2) {
		IM_ASSERT(DisplayStart >= 0 && DisplayEnd >= 0);
		StepNo = 3;
		return true;
	}
	if (StepNo == 3)         End();
	return false;
}


ImGuiWindow::ImGuiWindow(const char* name)
{
	Name = ImStrdup(name);
	ID = ImHash(name, 0);
	IDStack.push_back(ID);
	MoveId = GetID("#MOVE");

	Flags = 0;
	IndexWithinParent = 0;
	PosFloat = Pos = ImVec2(0.0f, 0.0f);
	Size = SizeFull = ImVec2(0.0f, 0.0f);
	SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
	WindowPadding = ImVec2(0.0f, 0.0f);
	Scroll = ImVec2(0.0f, 0.0f);
	ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
	ScrollTargetCenterRatio = ImVec2(0.5f, 0.5f);
	ScrollbarX = ScrollbarY = false;
	ScrollbarSizes = ImVec2(0.0f, 0.0f);
	BorderSize = 0.0f;
	Active = WasActive = false;
	Accessed = false;
	Collapsed = false;
	SkipItems = false;
	BeginCount = 0;
	PopupId = 0;
	AutoFitFramesX = AutoFitFramesY = -1;
	AutoFitOnlyGrows = false;
	AutoPosLastDirection = -1;
	HiddenFrames = 0;
	SetWindowPosAllowFlags = SetWindowSizeAllowFlags = SetWindowCollapsedAllowFlags = ImGuiSetCond_Always | ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing;
	SetWindowPosCenterWanted = false;

	LastFrameActive = -1;
	ItemWidthDefault = 0.0f;
	FontWindowScale = 1.0f;

	DrawList = (ImDrawList*)ImGui::MemAlloc(sizeof(ImDrawList));
	IM_PLACEMENT_NEW(DrawList) ImDrawList();
	DrawList->_OwnerName = Name;
	RootWindow = NULL;
	RootNonPopupWindow = NULL;
	ParentWindow = NULL;

	FocusIdxAllCounter = FocusIdxTabCounter = -1;
	FocusIdxAllRequestCurrent = FocusIdxTabRequestCurrent = INT_MAX;
	FocusIdxAllRequestNext = FocusIdxTabRequestNext = INT_MAX;
}

ImGuiWindow::~ImGuiWindow()
{
	DrawList->~ImDrawList();
	ImGui::MemFree(DrawList);
	DrawList = NULL;
	ImGui::MemFree(Name);
	Name = NULL;
}

ImGuiID ImGuiWindow::GetID(const char* str, const char* str_end)
{
	ImGuiID seed = IDStack.back();
	ImGuiID id = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
	ImGui::KeepAliveID(id);
	return id;
}

ImGuiID ImGuiWindow::GetID(const void* ptr)
{
	ImGuiID seed = IDStack.back();
	ImGuiID id = ImHash(&ptr, sizeof(void*), seed);
	ImGui::KeepAliveID(id);
	return id;
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const char* str, const char* str_end)
{
	ImGuiID seed = IDStack.back();
	return ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
}


static void SetCurrentWindow(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	g.CurrentWindow = window;
	if (window)
		g.FontSize = window->CalcFontSize();
}

ImGuiWindow* ImGui::GetParentWindow()
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.CurrentWindowStack.Size >= 2);
	return g.CurrentWindowStack[(unsigned int)g.CurrentWindowStack.Size - 2];
}

void ImGui::SetActiveID(ImGuiID id, ImGuiWindow* window = NULL)
{
	ImGuiContext& g = *GImGui;
	g.ActiveId = id;
	g.ActiveIdAllowOverlap = false;
	g.ActiveIdIsJustActivated = true;
	g.ActiveIdWindow = window;
}

void ImGui::SetHoveredID(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	g.HoveredId = id;
	g.HoveredIdAllowOverlap = false;
}

void ImGui::KeepAliveID(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	if (g.ActiveId == id)
		g.ActiveIdIsAlive = true;
}

void ImGui::ItemSize(const ImVec2& size, float text_offset_y)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const float line_height = ImMax(window->DC.CurrentLineHeight, size.y);
	const float text_base_offset = ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
	window->DC.CursorPosPrevLine = ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
	window->DC.CursorPos = ImVec2((float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX), (float)(int)(window->DC.CursorPos.y + line_height + g.Style.ItemSpacing.y));
	window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
	window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);


	window->DC.PrevLineHeight = line_height;
	window->DC.PrevLineTextBaseOffset = text_base_offset;
	window->DC.CurrentLineHeight = window->DC.CurrentLineTextBaseOffset = 0.0f;
}

void ImGui::ItemSize(const ImRect& bb, float text_offset_y)
{
	ItemSize(bb.GetSize(), text_offset_y);
}

bool ImGui::ItemAdd(const ImRect& bb, const ImGuiID* id)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.LastItemId = id ? *id : 0;
	window->DC.LastItemRect = bb;
	window->DC.LastItemHoveredAndUsable = window->DC.LastItemHoveredRect = false;
	if (IsClippedEx(bb, id, false))
		return false;

	ImGuiContext& g = *GImGui;
	if (IsMouseHoveringRect(bb.Min, bb.Max))
	{
		window->DC.LastItemHoveredRect = true;
		if (g.HoveredRootWindow == window->RootWindow)
			if (g.ActiveId == 0 || (id && g.ActiveId == *id) || g.ActiveIdAllowOverlap || (g.ActiveId == window->MoveId))
				if (IsWindowContentHoverable(window))
					window->DC.LastItemHoveredAndUsable = true;
	}

	return true;
}

bool ImGui::IsClippedEx(const ImRect& bb, const ImGuiID* id, bool clip_even_when_logged)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindowRead();

	if (!bb.Overlaps(window->ClipRect))
		if (!id || *id != GImGui->ActiveId)
			if (clip_even_when_logged || !g.LogEnabled)
				return true;
	return false;
}

bool ImGui::IsHovered(const ImRect& bb, ImGuiID id, bool flatten_childs)
{
	ImGuiContext& g = *GImGui;
	if (g.HoveredId == 0 || g.HoveredId == id || g.HoveredIdAllowOverlap)
	{
		ImGuiWindow* window = GetCurrentWindowRead();
		if (g.HoveredWindow == window || (flatten_childs && g.HoveredRootWindow == window->RootWindow))
			if ((g.ActiveId == 0 || g.ActiveId == id || g.ActiveIdAllowOverlap) && IsMouseHoveringRect(bb.Min, bb.Max))
				if (IsWindowContentHoverable(g.HoveredRootWindow))
					return true;
	}
	return false;
}

bool ImGui::FocusableItemRegister(ImGuiWindow* window, bool is_active, bool tab_stop)
{
	ImGuiContext& g = *GImGui;

	const bool allow_keyboard_focus = window->DC.AllowKeyboardFocus;
	window->FocusIdxAllCounter++;
	if (allow_keyboard_focus)
		window->FocusIdxTabCounter++;

	if (tab_stop && window->FocusIdxAllRequestNext == INT_MAX && window->FocusIdxTabRequestNext == INT_MAX && is_active && IsKeyPressedMap(ImGuiKey_Tab))
	{
		window->FocusIdxTabRequestNext = window->FocusIdxTabCounter + (g.IO.KeyShift ? (allow_keyboard_focus ? -1 : 0) : +1);
	}

	if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent)
		return true;

	if (allow_keyboard_focus)
		if (window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent)
			return true;

	return false;
}

void ImGui::FocusableItemUnregister(ImGuiWindow* window)
{
	window->FocusIdxAllCounter--;
	window->FocusIdxTabCounter--;
}

ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y)
{
	ImGuiContext& g = *GImGui;
	ImVec2 content_max;
	if (size.x < 0.0f || size.y < 0.0f)
		content_max = g.CurrentWindow->Pos + GetContentRegionMax();
	if (size.x <= 0.0f)
		size.x = (size.x == 0.0f) ? default_x : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) + size.x;
	if (size.y <= 0.0f)
		size.y = (size.y == 0.0f) ? default_y : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) + size.y;
	return size;
}

float ImGui::CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x)
{
	if (wrap_pos_x < 0.0f)
		return 0.0f;

	ImGuiWindow* window = GetCurrentWindowRead();
	if (wrap_pos_x == 0.0f)
		wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
	else if (wrap_pos_x > 0.0f)
		wrap_pos_x += window->Pos.x - window->Scroll.x;
	return ImMax(wrap_pos_x - pos.x, 1.0f);
}


void* ImGui::MemAlloc(size_t sz)
{
	GImGui->IO.MetricsAllocs++;
	return GImGui->IO.MemAllocFn(sz);
}

void ImGui::MemFree(void* ptr)
{
	if (ptr) GImGui->IO.MetricsAllocs--;
	return GImGui->IO.MemFreeFn(ptr);
}

const char* ImGui::GetClipboardText()
{
	return GImGui->IO.GetClipboardTextFn ? GImGui->IO.GetClipboardTextFn() : "";
}

void ImGui::SetClipboardText(const char* text)
{
	if (GImGui->IO.SetClipboardTextFn)
		GImGui->IO.SetClipboardTextFn(text);
}

const char* ImGui::GetVersion()
{
	return IMGUI_VERSION;
}

ImGuiContext* ImGui::GetCurrentContext()
{
	return GImGui;
}

void ImGui::SetCurrentContext(ImGuiContext* ctx)
{
	GImGui = ctx;
}

ImGuiContext* ImGui::CreateContext(void* (*malloc_fn)(size_t), void(*free_fn)(void*))
{
	if (!malloc_fn) malloc_fn = malloc;
	ImGuiContext* ctx = (ImGuiContext*)malloc_fn(sizeof(ImGuiContext));
	IM_PLACEMENT_NEW(ctx) ImGuiContext();
	ctx->IO.MemAllocFn = malloc_fn;
	ctx->IO.MemFreeFn = free_fn ? free_fn : free;
	return ctx;
}

void ImGui::DestroyContext(ImGuiContext* ctx)
{
	void(*free_fn)(void*) = ctx->IO.MemFreeFn;
	ctx->~ImGuiContext();
	free_fn(ctx);
	if (GImGui == ctx)
		GImGui = NULL;
}

ImGuiIO& ImGui::GetIO()
{
	return GImGui->IO;
}

ImGuiStyle& ImGui::GetStyle()
{
	return GImGui->Style;
}

ImDrawData* ImGui::GetDrawData()
{
	return GImGui->RenderDrawData.Valid ? &GImGui->RenderDrawData : NULL;
}

float ImGui::GetTime()
{
	return GImGui->Time;
}

int ImGui::GetFrameCount()
{
	return GImGui->FrameCount;
}

void ImGui::NewFrame()
{
	ImGuiContext& g = *GImGui;

	IM_ASSERT(g.IO.DeltaTime >= 0.0f);                   IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f);
	IM_ASSERT(g.IO.Fonts->Fonts.Size > 0);               IM_ASSERT(g.IO.Fonts->Fonts[0]->IsLoaded());         IM_ASSERT(g.Style.CurveTessellationTol > 0.0f);
	if (!g.Initialized)
	{
		g.LogClipboard = (ImGuiTextBuffer*)ImGui::MemAlloc(sizeof(ImGuiTextBuffer));
		IM_PLACEMENT_NEW(g.LogClipboard) ImGuiTextBuffer();

		IM_ASSERT(g.Settings.empty());
		LoadSettings();
		g.Initialized = true;
	}

	SetCurrentFont(g.IO.Fonts->Fonts[0]);

	g.Time += g.IO.DeltaTime;
	g.FrameCount += 1;
	g.Tooltip[0] = '\0';
	g.OverlayDrawList.Clear();
	g.OverlayDrawList.PushTextureID(g.IO.Fonts->TexID);
	g.OverlayDrawList.PushClipRectFullScreen();

	g.RenderDrawData.Valid = false;
	g.RenderDrawData.CmdLists = NULL;
	g.RenderDrawData.CmdListsCount = g.RenderDrawData.TotalVtxCount = g.RenderDrawData.TotalIdxCount = 0;

	if (g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0)
		g.IO.MousePos = ImVec2(-9999.0f, -9999.0f);
	if ((g.IO.MousePos.x < 0 && g.IO.MousePos.y < 0) || (g.IO.MousePosPrev.x < 0 && g.IO.MousePosPrev.y < 0))           g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
	else
		g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
	g.IO.MousePosPrev = g.IO.MousePos;
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
	{
		g.IO.MouseClicked[i] = g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
		g.IO.MouseReleased[i] = !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
		g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
		g.IO.MouseDownDuration[i] = g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f ? 0.0f : g.IO.MouseDownDuration[i] + g.IO.DeltaTime) : -1.0f;
		g.IO.MouseDoubleClicked[i] = false;
		if (g.IO.MouseClicked[i])
		{
			if (g.Time - g.IO.MouseClickedTime[i] < g.IO.MouseDoubleClickTime)
			{
				if (ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]) < g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
					g.IO.MouseDoubleClicked[i] = true;
				g.IO.MouseClickedTime[i] = -FLT_MAX;
			}
			else
			{
				g.IO.MouseClickedTime[i] = g.Time;
			}
			g.IO.MouseClickedPos[i] = g.IO.MousePos;
			g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
		}
		else if (g.IO.MouseDown[i])
		{
			g.IO.MouseDragMaxDistanceSqr[i] = ImMax(g.IO.MouseDragMaxDistanceSqr[i], ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]));
		}
	}
	memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration, sizeof(g.IO.KeysDownDuration));
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
		g.IO.KeysDownDuration[i] = g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f ? 0.0f : g.IO.KeysDownDuration[i] + g.IO.DeltaTime) : -1.0f;

	g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
	g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
	g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
	g.IO.Framerate = 1.0f / (g.FramerateSecPerFrameAccum / (float)IM_ARRAYSIZE(g.FramerateSecPerFrame));

	g.HoveredIdPreviousFrame = g.HoveredId;
	g.HoveredId = 0;
	g.HoveredIdAllowOverlap = false;
	if (!g.ActiveIdIsAlive && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0)
		SetActiveID(0);
	g.ActiveIdPreviousFrame = g.ActiveId;
	g.ActiveIdIsAlive = false;
	g.ActiveIdIsJustActivated = false;

	if (g.MovedWindowMoveId && g.MovedWindowMoveId == g.ActiveId)
	{
		KeepAliveID(g.MovedWindowMoveId);
		IM_ASSERT(g.MovedWindow && g.MovedWindow->RootWindow);
		IM_ASSERT(g.MovedWindow->RootWindow->MoveId == g.MovedWindowMoveId);
		if (g.IO.MouseDown[0])
		{
			if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoMove))
			{
				g.MovedWindow->PosFloat += g.IO.MouseDelta;
				if (!(g.MovedWindow->Flags & ImGuiWindowFlags_NoSavedSettings))
					MarkSettingsDirty();
			}
			FocusWindow(g.MovedWindow);
		}
		else
		{
			SetActiveID(0);
			g.MovedWindow = NULL;
			g.MovedWindowMoveId = 0;
		}
	}
	else
	{
		g.MovedWindow = NULL;
		g.MovedWindowMoveId = 0;
	}

	if (g.SettingsDirtyTimer > 0.0f)
	{
		g.SettingsDirtyTimer -= g.IO.DeltaTime;
		if (g.SettingsDirtyTimer <= 0.0f)
			SaveSettings();
	}

	g.HoveredWindow = g.MovedWindow ? g.MovedWindow : FindHoveredWindow(g.IO.MousePos, false);
	if (g.HoveredWindow && (g.HoveredWindow->Flags & ImGuiWindowFlags_ChildWindow))
		g.HoveredRootWindow = g.HoveredWindow->RootWindow;
	else
		g.HoveredRootWindow = g.MovedWindow ? g.MovedWindow->RootWindow : FindHoveredWindow(g.IO.MousePos, true);

	if (ImGuiWindow* modal_window = GetFrontMostModalRootWindow())
	{
		g.ModalWindowDarkeningRatio = ImMin(g.ModalWindowDarkeningRatio + g.IO.DeltaTime * 6.0f, 1.0f);
		ImGuiWindow* window = g.HoveredRootWindow;
		while (window && window != modal_window)
			window = window->ParentWindow;
		if (!window)
			g.HoveredRootWindow = g.HoveredWindow = NULL;
	}
	else
	{
		g.ModalWindowDarkeningRatio = 0.0f;
	}

	int mouse_earliest_button_down = -1;
	bool mouse_any_down = false;
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
	{
		if (g.IO.MouseClicked[i])
			g.IO.MouseDownOwned[i] = (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
		mouse_any_down |= g.IO.MouseDown[i];
		if (g.IO.MouseDown[i])
			if (mouse_earliest_button_down == -1 || g.IO.MouseClickedTime[mouse_earliest_button_down] > g.IO.MouseClickedTime[i])
				mouse_earliest_button_down = i;
	}
	bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) || g.IO.MouseDownOwned[mouse_earliest_button_down];
	if (g.CaptureMouseNextFrame != -1)
		g.IO.WantCaptureMouse = (g.CaptureMouseNextFrame != 0);
	else
		g.IO.WantCaptureMouse = (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) || (g.ActiveId != 0) || (!g.OpenPopupStack.empty());
	g.IO.WantCaptureKeyboard = (g.CaptureKeyboardNextFrame != -1) ? (g.CaptureKeyboardNextFrame != 0) : (g.ActiveId != 0);
	g.IO.WantTextInput = (g.ActiveId != 0 && g.InputTextState.Id == g.ActiveId);
	g.MouseCursor = ImGuiMouseCursor_Arrow;
	g.CaptureMouseNextFrame = g.CaptureKeyboardNextFrame = -1;
	g.OsImePosRequest = ImVec2(1.0f, 1.0f);
	if (!mouse_avail_to_imgui)
		g.HoveredWindow = g.HoveredRootWindow = NULL;

	if (g.HoveredWindow && g.IO.MouseWheel != 0.0f && !g.HoveredWindow->Collapsed)
	{
		ImGuiWindow* window = g.HoveredWindow;
		if (g.IO.KeyCtrl)
		{
			if (g.IO.FontAllowUserScaling)
			{
				float new_font_scale = ImClamp(window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
				float scale = new_font_scale / window->FontWindowScale;
				window->FontWindowScale = new_font_scale;

				const ImVec2 offset = window->Size * (1.0f - scale) * (g.IO.MousePos - window->Pos) / window->Size;
				window->Pos += offset;
				window->PosFloat += offset;
				window->Size *= scale;
				window->SizeFull *= scale;
			}
		}
		else if (!(window->Flags & ImGuiWindowFlags_NoScrollWithMouse))
		{
			const int scroll_lines = (window->Flags & ImGuiWindowFlags_ComboBox) ? 3 : 5;
			SetWindowScrollY(window, window->Scroll.y - g.IO.MouseWheel * window->CalcFontSize() * scroll_lines);
		}
	}

	if (g.ActiveId == 0 && g.FocusedWindow != NULL && g.FocusedWindow->Active && IsKeyPressedMap(ImGuiKey_Tab, false))
		g.FocusedWindow->FocusIdxTabRequestNext = 0;

	for (int i = 0; i != g.Windows.Size; i++)
	{
		ImGuiWindow* window = g.Windows[i];
		window->WasActive = window->Active;
		window->Active = false;
		window->Accessed = false;
	}

	if (g.FocusedWindow && !g.FocusedWindow->WasActive)
		for (int i = g.Windows.Size - 1; i >= 0; i--)
			if (g.Windows[i]->WasActive && !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow))
			{
				FocusWindow(g.Windows[i]);
				break;
			}

	g.CurrentWindowStack.resize(0);
	g.CurrentPopupStack.resize(0);
	CloseInactivePopups();

	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Debug");
}

void ImGui::Shutdown()
{
	ImGuiContext& g = *GImGui;

	if (g.IO.Fonts)         g.IO.Fonts->Clear();

	if (!g.Initialized)
		return;

	SaveSettings();

	for (int i = 0; i < g.Windows.Size; i++)
	{
		g.Windows[i]->~ImGuiWindow();
		ImGui::MemFree(g.Windows[i]);
	}
	g.Windows.clear();
	g.WindowsSortBuffer.clear();
	g.CurrentWindowStack.clear();
	g.FocusedWindow = NULL;
	g.HoveredWindow = NULL;
	g.HoveredRootWindow = NULL;
	for (int i = 0; i < g.Settings.Size; i++)
		ImGui::MemFree(g.Settings[i].Name);
	g.Settings.clear();
	g.ColorModifiers.clear();
	g.StyleModifiers.clear();
	g.FontStack.clear();
	g.OpenPopupStack.clear();
	g.CurrentPopupStack.clear();
	for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
		g.RenderDrawLists[i].clear();
	g.OverlayDrawList.ClearFreeMemory();
	g.ColorEditModeStorage.Clear();
	if (g.PrivateClipboard)
	{
		ImGui::MemFree(g.PrivateClipboard);
		g.PrivateClipboard = NULL;
	}
	g.InputTextState.Text.clear();
	g.InputTextState.InitialText.clear();
	g.InputTextState.TempTextBuffer.clear();

	if (g.LogFile && g.LogFile != stdout)
	{
		fclose(g.LogFile);
		g.LogFile = NULL;
	}
	if (g.LogClipboard)
	{
		g.LogClipboard->~ImGuiTextBuffer();
		ImGui::MemFree(g.LogClipboard);
	}

	g.Initialized = false;
}

static ImGuiIniData* FindWindowSettings(const char* name)
{
	ImGuiContext& g = *GImGui;
	ImGuiID id = ImHash(name, 0);
	for (int i = 0; i != g.Settings.Size; i++)
	{
		ImGuiIniData* ini = &g.Settings[i];
		if (ini->Id == id)
			return ini;
	}
	return NULL;
}

static ImGuiIniData* AddWindowSettings(const char* name)
{
	GImGui->Settings.resize(GImGui->Settings.Size + 1);
	ImGuiIniData* ini = &GImGui->Settings.back();
	ini->Name = ImStrdup(name);
	ini->Id = ImHash(name, 0);
	ini->Collapsed = false;
	ini->Pos = ImVec2(FLT_MAX, FLT_MAX);
	ini->Size = ImVec2(0, 0);
	return ini;
}

static void LoadSettings()
{
	ImGuiContext& g = *GImGui;
	const char* filename = g.IO.IniFilename;
	if (!filename)
		return;

	int file_size;
	char* file_data = (char*)ImLoadFileToMemory(filename, "rb", &file_size, 1);
	if (!file_data)
		return;

	ImGuiIniData* settings = NULL;
	const char* buf_end = file_data + file_size;
	for (const char* line_start = file_data; line_start < buf_end; )
	{
		const char* line_end = line_start;
		while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
			line_end++;

		if (line_start[0] == ((char)(0x19c6 + 2328 - 0x2283)) && line_end > line_start && line_end[-1] == ((char)(0x66f + 8147 - 0x25e5)))
		{
			char name[64];
			ImFormatString(name, IM_ARRAYSIZE(name), "%.*s", (int)(line_end - line_start - 2), line_start + 1);
			settings = FindWindowSettings(name);
			if (!settings)
				settings = AddWindowSettings(name);
		}
		else if (settings)
		{
			float x, y;
			int i;
			if (sscanf(line_start, "Pos=%f,%f", &x, &y) == 2)
				settings->Pos = ImVec2(x, y);
			else if (sscanf(line_start, "Size=%f,%f", &x, &y) == 2)
				settings->Size = ImMax(ImVec2(x, y), g.Style.WindowMinSize);
			else if (sscanf(line_start, "Collapsed=%d", &i) == 1)
				settings->Collapsed = (i != 0);
		}

		line_start = line_end + 1;
	}

	ImGui::MemFree(file_data);
}

static void SaveSettings()
{
	ImGuiContext& g = *GImGui;
	const char* filename = g.IO.IniFilename;
	if (!filename)
		return;

	for (int i = 0; i != g.Windows.Size; i++)
	{
		ImGuiWindow* window = g.Windows[i];
		if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
			continue;
		ImGuiIniData* settings = FindWindowSettings(window->Name);
		settings->Pos = window->Pos;
		settings->Size = window->SizeFull;
		settings->Collapsed = window->Collapsed;
	}

	FILE* f = fopen(filename, "wt");
	if (!f)
		return;
	for (int i = 0; i != g.Settings.Size; i++)
	{
		const ImGuiIniData* settings = &g.Settings[i];
		if (settings->Pos.x == FLT_MAX)
			continue;
		const char* name = settings->Name;
		if (const char* p = strstr(name, "###"))              name = p;
		fprintf(f, "[%s]\n", name);
		fprintf(f, "Pos=%d,%d\n", (int)settings->Pos.x, (int)settings->Pos.y);
		fprintf(f, "Size=%d,%d\n", (int)settings->Size.x, (int)settings->Size.y);
		fprintf(f, "Collapsed=%d\n", settings->Collapsed);
		fprintf(f, "\n");
	}

	fclose(f);
}

static void MarkSettingsDirty()
{
	ImGuiContext& g = *GImGui;
	if (g.SettingsDirtyTimer <= 0.0f)
		g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

static int ChildWindowComparer(const void* lhs, const void* rhs)
{
	const ImGuiWindow* a = *(const ImGuiWindow**)lhs;
	const ImGuiWindow* b = *(const ImGuiWindow**)rhs;
	if (int d = (a->Flags & ImGuiWindowFlags_Popup) - (b->Flags & ImGuiWindowFlags_Popup))
		return d;
	if (int d = (a->Flags & ImGuiWindowFlags_Tooltip) - (b->Flags & ImGuiWindowFlags_Tooltip))
		return d;
	if (int d = (a->Flags & ImGuiWindowFlags_ComboBox) - (b->Flags & ImGuiWindowFlags_ComboBox))
		return d;
	return (a->IndexWithinParent - b->IndexWithinParent);
}

static void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window)
{
	out_sorted_windows.push_back(window);
	if (window->Active)
	{
		int count = window->DC.ChildWindows.Size;
		if (count > 1)
			qsort(window->DC.ChildWindows.begin(), (size_t)count, sizeof(ImGuiWindow*), ChildWindowComparer);
		for (int i = 0; i < count; i++)
		{
			ImGuiWindow* child = window->DC.ChildWindows[i];
			if (child->Active)
				AddWindowToSortedBuffer(out_sorted_windows, child);
		}
	}
}

static void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list)
{
	if (draw_list->CmdBuffer.empty())
		return;

	ImDrawCmd& last_cmd = draw_list->CmdBuffer.back();
	if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL)
	{
		draw_list->CmdBuffer.pop_back();
		if (draw_list->CmdBuffer.empty())
			return;
	}

	IM_ASSERT(draw_list->_VtxWritePtr == draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
	IM_ASSERT(draw_list->_IdxWritePtr == draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
	IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

	IM_ASSERT((int64_t)draw_list->_VtxCurrentIdx <= ((int64_t)1L << (sizeof(ImDrawIdx) * 8)));
	out_render_list.push_back(draw_list);
	GImGui->IO.MetricsRenderVertices += draw_list->VtxBuffer.Size;
	GImGui->IO.MetricsRenderIndices += draw_list->IdxBuffer.Size;
}

static void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window)
{
	AddDrawListToRenderList(out_render_list, window->DrawList);
	for (int i = 0; i < window->DC.ChildWindows.Size; i++)
	{
		ImGuiWindow* child = window->DC.ChildWindows[i];
		if (!child->Active)             continue;
		if ((child->Flags & ImGuiWindowFlags_Popup) && child->HiddenFrames > 0)
			continue;
		AddWindowToRenderList(out_render_list, child);
	}
}

void ImGui::PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DrawList->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
	window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::PopClipRect()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DrawList->PopClipRect();
	window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::EndFrame()
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.Initialized);                           IM_ASSERT(g.FrameCountEnded != g.FrameCount);
	if (g.Tooltip[0])
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(g.Tooltip);
		ImGui::EndTooltip();
	}

	if (g.IO.ImeSetInputScreenPosFn && ImLengthSqr(g.OsImePosRequest - g.OsImePosSet) > 0.0001f)
	{
		g.IO.ImeSetInputScreenPosFn((int)g.OsImePosRequest.x, (int)g.OsImePosRequest.y);
		g.OsImePosSet = g.OsImePosRequest;
	}

	IM_ASSERT(g.CurrentWindowStack.Size == 1);        if (g.CurrentWindow && !g.CurrentWindow->Accessed)
		g.CurrentWindow->Active = false;
	ImGui::End();

	if (g.ActiveId == 0 && g.HoveredId == 0 && g.IO.MouseClicked[0])
	{
		if (!(g.FocusedWindow && !g.FocusedWindow->WasActive && g.FocusedWindow->Active)) {
			if (g.HoveredRootWindow != NULL)
			{
				FocusWindow(g.HoveredWindow);
				if (!(g.HoveredWindow->Flags & ImGuiWindowFlags_NoMove))
				{
					g.MovedWindow = g.HoveredWindow;
					g.MovedWindowMoveId = g.HoveredRootWindow->MoveId;
					SetActiveID(g.MovedWindowMoveId, g.HoveredRootWindow);
				}
			}
			else if (g.FocusedWindow != NULL && GetFrontMostModalRootWindow() == NULL)
			{
				FocusWindow(NULL);
			}
		}
	}

	g.WindowsSortBuffer.resize(0);
	g.WindowsSortBuffer.reserve(g.Windows.Size);
	for (int i = 0; i != g.Windows.Size; i++)
	{
		ImGuiWindow* window = g.Windows[i];
		if (window->Active && (window->Flags & ImGuiWindowFlags_ChildWindow))                   continue;
		AddWindowToSortedBuffer(g.WindowsSortBuffer, window);
	}
	IM_ASSERT(g.Windows.Size == g.WindowsSortBuffer.Size);      g.Windows.swap(g.WindowsSortBuffer);

	g.IO.MouseWheel = 0.0f;
	memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));

	g.FrameCountEnded = g.FrameCount;
}

void ImGui::Render()
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.Initialized);
	if (g.FrameCountEnded != g.FrameCount)
		ImGui::EndFrame();
	g.FrameCountRendered = g.FrameCount;

	if (g.Style.Alpha > 0.0f)
	{
		g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = g.IO.MetricsActiveWindows = 0;
		for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
			g.RenderDrawLists[i].resize(0);
		for (int i = 0; i != g.Windows.Size; i++)
		{
			ImGuiWindow* window = g.Windows[i];
			if (window->Active && window->HiddenFrames <= 0 && (window->Flags & (ImGuiWindowFlags_ChildWindow)) == 0)
			{
				g.IO.MetricsActiveWindows++;
				if (window->Flags & ImGuiWindowFlags_Popup)
					AddWindowToRenderList(g.RenderDrawLists[1], window);
				else if (window->Flags & ImGuiWindowFlags_Tooltip)
					AddWindowToRenderList(g.RenderDrawLists[2], window);
				else
					AddWindowToRenderList(g.RenderDrawLists[0], window);
			}
		}

		int n = g.RenderDrawLists[0].Size;
		int flattened_size = n;
		for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
			flattened_size += g.RenderDrawLists[i].Size;
		g.RenderDrawLists[0].resize(flattened_size);
		for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
		{
			ImVector<ImDrawList*>& layer = g.RenderDrawLists[i];
			if (layer.empty())
				continue;
			memcpy(&g.RenderDrawLists[0][n], &layer[0], layer.Size * sizeof(ImDrawList*));
			n += layer.Size;
		}

		if (g.IO.MouseDrawCursor)
		{
			const ImGuiMouseCursorData& cursor_data = g.MouseCursorData[g.MouseCursor];
			const ImVec2 pos = g.IO.MousePos - cursor_data.HotOffset;
			const ImVec2 size = cursor_data.Size;
			const ImTextureID tex_id = g.IO.Fonts->TexID;
			g.OverlayDrawList.PushTextureID(tex_id);
			g.OverlayDrawList.AddImage(tex_id, pos + ImVec2(1, 0), pos + ImVec2(1, 0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 48));
			g.OverlayDrawList.AddImage(tex_id, pos + ImVec2(2, 0), pos + ImVec2(2, 0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 48));
			g.OverlayDrawList.AddImage(tex_id, pos, pos + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 255));
			g.OverlayDrawList.AddImage(tex_id, pos, pos + size, cursor_data.TexUvMin[0], cursor_data.TexUvMax[0], IM_COL32(255, 255, 255, 255));
			g.OverlayDrawList.PopTextureID();
		}
		if (!g.OverlayDrawList.VtxBuffer.empty())
			AddDrawListToRenderList(g.RenderDrawLists[0], &g.OverlayDrawList);

		g.RenderDrawData.Valid = true;
		g.RenderDrawData.CmdLists = (g.RenderDrawLists[0].Size > 0) ? &g.RenderDrawLists[0][0] : NULL;
		g.RenderDrawData.CmdListsCount = g.RenderDrawLists[0].Size;
		g.RenderDrawData.TotalVtxCount = g.IO.MetricsRenderVertices;
		g.RenderDrawData.TotalIdxCount = g.IO.MetricsRenderIndices;

		if (g.RenderDrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
			g.IO.RenderDrawListsFn(&g.RenderDrawData);
	}
}

const char* ImGui::FindRenderedTextEnd(const char* text, const char* text_end)
{
	const char* text_display_end = text;
	if (!text_end)
		text_end = (const char*)-1;

	while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != ((char)(0x92b + 5657 - 0x1f21)) || text_display_end[1] != ((char)(0x13b4 + 260 - 0x1495))))
		text_display_end++;
	return text_display_end;
}

void ImGui::LogText(const char* fmt, ...)
{
	ImGuiContext& g = *GImGui;
	if (!g.LogEnabled)
		return;

	va_list args;
	va_start(args, fmt);
	if (g.LogFile)
	{
		vfprintf(g.LogFile, fmt, args);
	}
	else
	{
		g.LogClipboard->appendv(fmt, args);
	}
	va_end(args);
}

static void LogRenderedText(const ImVec2& ref_pos, const char* text, const char* text_end)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();

	if (!text_end)
		text_end = ImGui::FindRenderedTextEnd(text, text_end);

	const bool log_new_line = ref_pos.y > window->DC.LogLinePosY + 1;
	window->DC.LogLinePosY = ref_pos.y;

	const char* text_remaining = text;
	if (g.LogStartDepth > window->DC.TreeDepth)          g.LogStartDepth = window->DC.TreeDepth;
	const int tree_depth = (window->DC.TreeDepth - g.LogStartDepth);
	for (;;)
	{
		const char* line_end = text_remaining;
		while (line_end < text_end)
			if (*line_end == '\n')
				break;
			else
				line_end++;
		if (line_end >= text_end)
			line_end = NULL;

		const bool is_first_line = (text == text_remaining);
		bool is_last_line = false;
		if (line_end == NULL)
		{
			is_last_line = true;
			line_end = text_end;
		}
		if (line_end != NULL && !(is_last_line && (line_end - text_remaining) == 0))
		{
			const int char_count = (int)(line_end - text_remaining);
			if (log_new_line || !is_first_line)
				ImGui::LogText(IM_NEWLINE "%*s%.*s", tree_depth * 4, "", char_count, text_remaining);
			else
				ImGui::LogText(" %.*s", char_count, text_remaining);
		}

		if (is_last_line)
			break;
		text_remaining = line_end + 1;
	}
}

void ImGui::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	const char* text_display_end;
	if (hide_text_after_hash)
	{
		text_display_end = FindRenderedTextEnd(text, text_end);
	}
	else
	{
		if (!text_end)
			text_end = text + strlen(text);         text_display_end = text_end;
	}

	const int text_len = (int)(text_display_end - text);
	if (text_len > 0)
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end);
		if (g.LogEnabled)
			LogRenderedText(pos, text, text_display_end);
	}
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	if (!text_end)
		text_end = text + strlen(text);
	const int text_len = (int)(text_end - text);
	if (text_len > 0)
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_end, wrap_width);
		if (g.LogEnabled)
			LogRenderedText(pos, text, text_end);
	}
}

void ImGui::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, ImGuiAlign align, const ImVec2* clip_min, const ImVec2* clip_max)
{
	const char* text_display_end = FindRenderedTextEnd(text, text_end);
	const int text_len = (int)(text_display_end - text);
	if (text_len == 0)
		return;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	ImVec2 pos = pos_min;
	const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

	if (!clip_max) clip_max = &pos_max;
	bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
	if (!clip_min) clip_min = &pos_min; else need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

	if (align & ImGuiAlign_Center) pos.x = ImMax(pos.x, (pos.x + pos_max.x - text_size.x) * 0.5f);
	else if (align & ImGuiAlign_Right) pos.x = ImMax(pos.x, pos_max.x - text_size.x);
	if (align & ImGuiAlign_VCenter) pos.y = ImMax(pos.y, (pos.y + pos_max.y - text_size.y) * 0.5f);

	if (need_clipping)
	{
		ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
	}
	else
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
	}
	if (g.LogEnabled)
		LogRenderedText(pos, text, text_display_end);
}

void ImGui::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding)
{
	ImGuiWindow* window = GetCurrentWindow();

	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
	if (border && (window->Flags & ImGuiWindowFlags_ShowBorders))
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding);
		window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding);
	}
}

void ImGui::RenderCollapseTriangle(ImVec2 p_min, bool is_open, float scale, bool shadow)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	const float h = g.FontSize * 1.00f;
	const float r = h * 0.40f * scale;
	ImVec2 center = p_min + ImVec2(h*0.50f, h*0.50f*scale);

	ImVec2 a, b, c;
	if (is_open)
	{
		center.y -= r*0.25f;
		a = center + ImVec2(0, 1)*r;
		b = center + ImVec2(-0.866f, -0.5f)*r;
		c = center + ImVec2(0.866f, -0.5f)*r;
	}
	else
	{
		a = center + ImVec2(1, 0)*r;
		b = center + ImVec2(-0.500f, 0.866f)*r;
		c = center + ImVec2(-0.500f, -0.866f)*r;
	}

	if (shadow && (window->Flags & ImGuiWindowFlags_ShowBorders) != 0)
		window->DrawList->AddTriangleFilled(a + ImVec2(2, 2), b + ImVec2(2, 2), c + ImVec2(2, 2), GetColorU32(ImGuiCol_BorderShadow));
	window->DrawList->AddTriangleFilled(a, b, c, GetColorU32(ImGuiCol_Text));
}

void ImGui::RenderBullet(ImVec2 pos)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DrawList->AddCircleFilled(pos, GImGui->FontSize*0.20f, GetColorU32(ImGuiCol_Text), 8);
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	ImVec2 a, b, c;
	float start_x = (float)(int)(g.FontSize * 0.307f + 0.5f);
	float rem_third = (float)(int)((g.FontSize - start_x) / 3.0f);
	a.x = pos.x + 0.5f + start_x;
	b.x = a.x + rem_third;
	c.x = a.x + rem_third * 3.0f;
	b.y = pos.y - 1.0f + (float)(int)(g.Font->Ascent * (g.FontSize / g.Font->FontSize) + 0.5f) + (float)(int)(g.Font->DisplayOffset.y);
	a.y = b.y - rem_third;
	c.y = b.y - rem_third * 2.0f;

	window->DrawList->PathLineTo(a);
	window->DrawList->PathLineTo(b);
	window->DrawList->PathLineTo(c);
	window->DrawList->PathStroke(col, false);
}

ImVec2 ImGui::CalcTextSize(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
	ImGuiContext& g = *GImGui;

	const char* text_display_end;
	if (hide_text_after_double_hash)
		text_display_end = FindRenderedTextEnd(text, text_end);          else
		text_display_end = text_end;

	ImFont* font = g.Font;
	const float font_size = g.FontSize;
	if (text == text_display_end)
		return ImVec2(0.0f, font_size);
	ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

	const float font_scale = font_size / font->FontSize;
	const float character_spacing_x = 1.0f * font_scale;
	if (text_size.x > 0.0f)
		text_size.x -= character_spacing_x;
	text_size.x = (float)(int)(text_size.x + 0.95f);

	return text_size;
}

void ImGui::CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindowRead();
	if (g.LogEnabled)
	{
		*out_items_display_start = 0;
		*out_items_display_end = items_count;
		return;
	}
	if (window->SkipItems)
	{
		*out_items_display_start = *out_items_display_end = 0;
		return;
	}

	const ImVec2 pos = window->DC.CursorPos;
	int start = (int)((window->ClipRect.Min.y - pos.y) / items_height);
	int end = (int)((window->ClipRect.Max.y - pos.y) / items_height);
	start = ImClamp(start, 0, items_count);
	end = ImClamp(end + 1, start, items_count);
	*out_items_display_start = start;
	*out_items_display_end = end;
}

static ImGuiWindow* FindHoveredWindow(ImVec2 pos, bool excluding_childs)
{
	ImGuiContext& g = *GImGui;
	for (int i = g.Windows.Size - 1; i >= 0; i--)
	{
		ImGuiWindow* window = g.Windows[i];
		if (!window->Active)
			continue;
		if (window->Flags & ImGuiWindowFlags_NoInputs)
			continue;
		if (excluding_childs && (window->Flags & ImGuiWindowFlags_ChildWindow) != 0)
			continue;

		ImRect bb(window->WindowRectClipped.Min - g.Style.TouchExtraPadding, window->WindowRectClipped.Max + g.Style.TouchExtraPadding);
		if (bb.Contains(pos))
			return window;
	}
	return NULL;
}

bool ImGui::IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindowRead();

	ImRect rect_clipped(r_min, r_max);
	if (clip)
		rect_clipped.Clip(window->ClipRect);

	const ImRect rect_for_touch(rect_clipped.Min - g.Style.TouchExtraPadding, rect_clipped.Max + g.Style.TouchExtraPadding);
	return rect_for_touch.Contains(g.IO.MousePos);
}

bool ImGui::IsMouseHoveringWindow()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredWindow == g.CurrentWindow;
}

bool ImGui::IsMouseHoveringAnyWindow()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredWindow != NULL;
}

bool ImGui::IsPosHoveringAnyWindow(const ImVec2& pos)
{
	return FindHoveredWindow(pos, false) != NULL;
}

static bool IsKeyPressedMap(ImGuiKey key, bool repeat)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return ImGui::IsKeyPressed(key_index, repeat);
}

int ImGui::GetKeyIndex(ImGuiKey key)
{
	IM_ASSERT(key >= 0 && key < ImGuiKey_COUNT);
	return GImGui->IO.KeyMap[key];
}

bool ImGui::IsKeyDown(int key_index)
{
	if (key_index < 0) return false;
	IM_ASSERT(key_index >= 0 && key_index < IM_ARRAYSIZE(GImGui->IO.KeysDown));
	return GImGui->IO.KeysDown[key_index];
}

bool ImGui::IsKeyPressed(int key_index, bool repeat)
{
	ImGuiContext& g = *GImGui;
	if (key_index < 0) return false;
	IM_ASSERT(key_index >= 0 && key_index < IM_ARRAYSIZE(g.IO.KeysDown));
	const float t = g.IO.KeysDownDuration[key_index];
	if (t == 0.0f)
		return true;

	if (repeat && t > g.IO.KeyRepeatDelay)
	{
		float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
		if ((fmodf(t - delay, rate) > rate*0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate*0.5f))
			return true;
	}
	return false;
}

bool ImGui::IsKeyReleased(int key_index)
{
	ImGuiContext& g = *GImGui;
	if (key_index < 0) return false;
	IM_ASSERT(key_index >= 0 && key_index < IM_ARRAYSIZE(g.IO.KeysDown));
	if (g.IO.KeysDownDurationPrev[key_index] >= 0.0f && !g.IO.KeysDown[key_index])
		return true;
	return false;
}

bool ImGui::IsMouseDown(int button)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	return g.IO.MouseDown[button];
}

bool ImGui::IsMouseClicked(int button, bool repeat)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	const float t = g.IO.MouseDownDuration[button];
	if (t == 0.0f)
		return true;

	if (repeat && t > g.IO.KeyRepeatDelay)
	{
		float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
		if ((fmodf(t - delay, rate) > rate*0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate*0.5f))
			return true;
	}

	return false;
}

bool ImGui::IsMouseReleased(int button)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	return g.IO.MouseReleased[button];
}

bool ImGui::IsMouseDoubleClicked(int button)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	return g.IO.MouseDoubleClicked[button];
}

bool ImGui::IsMouseDragging(int button, float lock_threshold)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	if (!g.IO.MouseDown[button])
		return false;
	if (lock_threshold < 0.0f)
		lock_threshold = g.IO.MouseDragThreshold;
	return g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold;
}

ImVec2 ImGui::GetMousePos()
{
	return GImGui->IO.MousePos;
}

ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup()
{
	ImGuiContext& g = *GImGui;
	if (g.CurrentPopupStack.Size > 0)
		return g.OpenPopupStack[g.CurrentPopupStack.Size - 1].MousePosOnOpen;
	return g.IO.MousePos;
}

ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	if (lock_threshold < 0.0f)
		lock_threshold = g.IO.MouseDragThreshold;
	if (g.IO.MouseDown[button])
		if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
			return g.IO.MousePos - g.IO.MouseClickedPos[button];         return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
	g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor()
{
	return GImGui->MouseCursor;
}

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
	GImGui->MouseCursor = cursor_type;
}

void ImGui::CaptureKeyboardFromApp(bool capture)
{
	GImGui->CaptureKeyboardNextFrame = capture ? 1 : 0;
}

void ImGui::CaptureMouseFromApp(bool capture)
{
	GImGui->CaptureMouseNextFrame = capture ? 1 : 0;
}

bool ImGui::IsItemHovered()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemHoveredAndUsable;
}

bool ImGui::IsItemHoveredRect()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemHoveredRect;
}

bool ImGui::IsItemActive()
{
	ImGuiContext& g = *GImGui;
	if (g.ActiveId)
	{
		ImGuiWindow* window = GetCurrentWindowRead();
		return g.ActiveId == window->DC.LastItemId;
	}
	return false;
}

bool ImGui::IsItemClicked(int mouse_button)
{
	return IsMouseClicked(mouse_button) && IsItemHovered();
}

bool ImGui::IsAnyItemHovered()
{
	return GImGui->HoveredId != 0 || GImGui->HoveredIdPreviousFrame != 0;
}

bool ImGui::IsAnyItemActive()
{
	return GImGui->ActiveId != 0;
}

bool ImGui::IsItemVisible()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImRect r(window->ClipRect);
	return r.Overlaps(window->DC.LastItemRect);
}

void ImGui::SetItemAllowOverlap()
{
	ImGuiContext& g = *GImGui;
	if (g.HoveredId == g.CurrentWindow->DC.LastItemId)
		g.HoveredIdAllowOverlap = true;
	if (g.ActiveId == g.CurrentWindow->DC.LastItemId)
		g.ActiveIdAllowOverlap = true;
}

ImVec2 ImGui::GetItemRectMin()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.Min;
}

ImVec2 ImGui::GetItemRectMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.Max;
}

ImVec2 ImGui::GetItemRectSize()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.GetSize();
}

ImVec2 ImGui::CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge, float outward)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImRect rect = window->DC.LastItemRect;
	rect.Expand(outward);
	return rect.GetClosestPoint(pos, on_edge);
}

void ImGui::SetTooltipV(const char* fmt, va_list args)
{
	ImGuiContext& g = *GImGui;
	ImFormatStringV(g.Tooltip, IM_ARRAYSIZE(g.Tooltip), fmt, args);
}

void ImGui::SetTooltip(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SetTooltipV(fmt, args);
	va_end(args);
}

static ImRect GetVisibleRect()
{
	ImGuiContext& g = *GImGui;
	if (g.IO.DisplayVisibleMin.x != g.IO.DisplayVisibleMax.x && g.IO.DisplayVisibleMin.y != g.IO.DisplayVisibleMax.y)
		return ImRect(g.IO.DisplayVisibleMin, g.IO.DisplayVisibleMax);
	return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

void ImGui::BeginTooltip()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("##Tooltip", NULL, flags);
}

void ImGui::EndTooltip()
{
	IM_ASSERT(GetCurrentWindowRead()->Flags & ImGuiWindowFlags_Tooltip);       ImGui::End();
}

static bool IsPopupOpen(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	const bool is_open = g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == id;
	return is_open;
}

void ImGui::OpenPopupEx(const char* str_id, bool reopen_existing)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID(str_id);
	int current_stack_size = g.CurrentPopupStack.Size;
	ImGuiPopupRef popup_ref = ImGuiPopupRef(id, window, window->GetID("##menus"), g.IO.MousePos);     if (g.OpenPopupStack.Size < current_stack_size + 1)
		g.OpenPopupStack.push_back(popup_ref);
	else if (reopen_existing || g.OpenPopupStack[current_stack_size].PopupId != id)
	{
		g.OpenPopupStack.resize(current_stack_size + 1);
		g.OpenPopupStack[current_stack_size] = popup_ref;
	}
}

void ImGui::OpenPopup(const char* str_id)
{
	ImGui::OpenPopupEx(str_id, false);
}

static void CloseInactivePopups()
{
	ImGuiContext& g = *GImGui;
	if (g.OpenPopupStack.empty())
		return;

	int n = 0;
	if (g.FocusedWindow)
	{
		for (n = 0; n < g.OpenPopupStack.Size; n++)
		{
			ImGuiPopupRef& popup = g.OpenPopupStack[n];
			if (!popup.Window)
				continue;
			IM_ASSERT((popup.Window->Flags & ImGuiWindowFlags_Popup) != 0);
			if (popup.Window->Flags & ImGuiWindowFlags_ChildWindow)
				continue;

			bool has_focus = false;
			for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
				has_focus = (g.OpenPopupStack[m].Window && g.OpenPopupStack[m].Window->RootWindow == g.FocusedWindow->RootWindow);
			if (!has_focus)
				break;
		}
	}
	if (n < g.OpenPopupStack.Size)           g.OpenPopupStack.resize(n);
}

static ImGuiWindow* GetFrontMostModalRootWindow()
{
	ImGuiContext& g = *GImGui;
	for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
		if (ImGuiWindow* front_most_popup = g.OpenPopupStack.Data[n].Window)
			if (front_most_popup->Flags & ImGuiWindowFlags_Modal)
				return front_most_popup;
	return NULL;
}

static void ClosePopupToLevel(int remaining)
{
	ImGuiContext& g = *GImGui;
	if (remaining > 0)
		ImGui::FocusWindow(g.OpenPopupStack[remaining - 1].Window);
	else
		ImGui::FocusWindow(g.OpenPopupStack[0].ParentWindow);
	g.OpenPopupStack.resize(remaining);
}

static void ClosePopup(ImGuiID id)
{
	if (!IsPopupOpen(id))
		return;
	ImGuiContext& g = *GImGui;
	ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}

void ImGui::CloseCurrentPopup()
{
	ImGuiContext& g = *GImGui;
	int popup_idx = g.CurrentPopupStack.Size - 1;
	if (popup_idx < 0 || popup_idx > g.OpenPopupStack.Size || g.CurrentPopupStack[popup_idx].PopupId != g.OpenPopupStack[popup_idx].PopupId)
		return;
	while (popup_idx > 0 && g.OpenPopupStack[popup_idx].Window && (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
		popup_idx--;
	ClosePopupToLevel(popup_idx);
}

static inline void ClearSetNextWindowData()
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowPosCond = g.SetNextWindowSizeCond = g.SetNextWindowContentSizeCond = g.SetNextWindowCollapsedCond = 0;
	g.SetNextWindowSizeConstraint = g.SetNextWindowFocus = false;
}

static bool BeginPopupEx(const char* str_id, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const ImGuiID id = window->GetID(str_id);
	if (!IsPopupOpen(id))
	{
		ClearSetNextWindowData();         return false;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGuiWindowFlags flags = extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

	char name[32];
	if (flags & ImGuiWindowFlags_ChildMenu)
		ImFormatString(name, 20, "##menu_%d", g.CurrentPopupStack.Size);        else
		ImFormatString(name, 20, "##popup_%08x", id);
	bool is_open = ImGui::Begin(name, NULL, flags);
	if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
		g.CurrentWindow->Flags &= ~ImGuiWindowFlags_ShowBorders;
	if (!is_open)         ImGui::EndPopup();

	return is_open;
}

bool ImGui::BeginPopup(const char* str_id)
{
	if (GImGui->OpenPopupStack.Size <= GImGui->CurrentPopupStack.Size) {
		ClearSetNextWindowData();         return false;
	}
	return BeginPopupEx(str_id, ImGuiWindowFlags_ShowBorders);
}

bool ImGui::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const ImGuiID id = window->GetID(name);
	if (!IsPopupOpen(id))
	{
		ClearSetNextWindowData();         return false;
	}

	ImGuiWindowFlags flags = extra_flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	bool is_open = ImGui::Begin(name, p_open, flags);
	if (!is_open || (p_open && !*p_open)) {
		ImGui::EndPopup();
		if (is_open)
			ClosePopup(id);
		return false;
	}

	return is_open;
}

void ImGui::EndPopup()
{
	ImGuiWindow* window = GetCurrentWindow();
	IM_ASSERT(window->Flags & ImGuiWindowFlags_Popup);      IM_ASSERT(GImGui->CurrentPopupStack.Size > 0);
	ImGui::End();
	if (!(window->Flags & ImGuiWindowFlags_Modal))
		ImGui::PopStyleVar();
}

bool ImGui::BeginPopupContextItem(const char* str_id, int mouse_button)
{
	if (IsItemHovered() && IsMouseClicked(mouse_button))
		OpenPopupEx(str_id, false);
	return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextWindow(bool also_over_items, const char* str_id, int mouse_button)
{
	if (!str_id) str_id = "window_context_menu";
	if (IsMouseHoveringWindow() && IsMouseClicked(mouse_button))
		if (also_over_items || !IsAnyItemHovered())
			OpenPopupEx(str_id, true);
	return BeginPopup(str_id);
}

bool ImGui::BeginPopupContextVoid(const char* str_id, int mouse_button)
{
	if (!str_id) str_id = "void_context_menu";
	if (!IsMouseHoveringAnyWindow() && IsMouseClicked(mouse_button))
		OpenPopupEx(str_id, true);
	return BeginPopup(str_id);
}

bool ImGui::BeginChild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

	const ImVec2 content_avail = GetContentRegionAvail();
	ImVec2 size = ImFloor(size_arg);
	if (size.x <= 0.0f)
	{
		if (size.x == 0.0f)
			flags |= ImGuiWindowFlags_ChildWindowAutoFitX;
		size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x);
	}
	if (size.y <= 0.0f)
	{
		if (size.y == 0.0f)
			flags |= ImGuiWindowFlags_ChildWindowAutoFitY;
		size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
	}
	if (border)
		flags |= ImGuiWindowFlags_ShowBorders;
	flags |= extra_flags;

	char title[256];
	ImFormatString(title, IM_ARRAYSIZE(title), "%s.%s", window->Name, str_id);

	bool ret = ImGui::Begin(title, NULL, size, -1.0f, flags);

	if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
		GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

	return ret;
}

bool ImGui::BeginChild(ImGuiID id, const ImVec2& size, bool border, ImGuiWindowFlags extra_flags)
{
	char str_id[32];
	ImFormatString(str_id, IM_ARRAYSIZE(str_id), "child_%08x", id);
	bool ret = ImGui::BeginChild(str_id, size, border, extra_flags);
	return ret;
}

void ImGui::EndChild()
{
	ImGuiWindow* window = GetCurrentWindow();

	IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);       if ((window->Flags & ImGuiWindowFlags_ComboBox) || window->BeginCount > 1)
	{
		ImGui::End();
	}
	else
	{
		ImVec2 sz = GetWindowSize();
		if (window->Flags & ImGuiWindowFlags_ChildWindowAutoFitX)             sz.x = ImMax(4.0f, sz.x);
		if (window->Flags & ImGuiWindowFlags_ChildWindowAutoFitY)
			sz.y = ImMax(4.0f, sz.y);

		ImGui::End();

		window = GetCurrentWindow();
		ImRect bb(window->DC.CursorPos, window->DC.CursorPos + sz);
		ItemSize(sz);
		ItemAdd(bb, NULL);
	}
}

bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_FrameBg]);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, style.FrameRounding);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
	return ImGui::BeginChild(id, size, (g.CurrentWindow->Flags & ImGuiWindowFlags_ShowBorders) ? true : false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
}

void ImGui::EndChildFrame()
{
	ImGui::EndChild();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();
}

static void CheckStacksSize(ImGuiWindow* window, bool write)
{
	ImGuiContext& g = *GImGui;
	int* p_backup = &window->DC.StackSizesBackup[0];
	{ int current = window->IDStack.Size;       if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushID/PopID Mismatch!");                   p_backup++; } { int current = window->DC.GroupStack.Size; if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginGroup/EndGroup Mismatch!");            p_backup++; } { int current = g.CurrentPopupStack.Size;   if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginMenu/EndMenu or BeginPopup/EndPopup Mismatch"); p_backup++; } { int current = g.ColorModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushStyleColor/PopStyleColor Mismatch!");   p_backup++; } { int current = g.StyleModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushStyleVar/PopStyleVar Mismatch!");       p_backup++; } { int current = g.FontStack.Size;           if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushFont/PopFont Mismatch!");               p_backup++; }        IM_ASSERT(p_backup == window->DC.StackSizesBackup + IM_ARRAYSIZE(window->DC.StackSizesBackup));
}

static ImVec2 FindBestPopupWindowPos(const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& r_inner)
{
	const ImGuiStyle& style = GImGui->Style;

	ImVec2 safe_padding = style.DisplaySafeAreaPadding;
	ImRect r_outer(GetVisibleRect());
	r_outer.Reduce(ImVec2((size.x - r_outer.GetWidth() > safe_padding.x * 2) ? safe_padding.x : 0.0f, (size.y - r_outer.GetHeight() > safe_padding.y * 2) ? safe_padding.y : 0.0f));
	ImVec2 base_pos_clamped = ImClamp(base_pos, r_outer.Min, r_outer.Max - size);

	for (int n = (*last_dir != -1) ? -1 : 0; n < 4; n++) {
		const int dir = (n == -1) ? *last_dir : n;
		ImRect rect(dir == 0 ? r_inner.Max.x : r_outer.Min.x, dir == 1 ? r_inner.Max.y : r_outer.Min.y, dir == 3 ? r_inner.Min.x : r_outer.Max.x, dir == 2 ? r_inner.Min.y : r_outer.Max.y);
		if (rect.GetWidth() < size.x || rect.GetHeight() < size.y)
			continue;
		*last_dir = dir;
		return ImVec2(dir == 0 ? r_inner.Max.x : dir == 3 ? r_inner.Min.x - size.x : base_pos_clamped.x, dir == 1 ? r_inner.Max.y : dir == 2 ? r_inner.Min.y - size.y : base_pos_clamped.y);
	}

	*last_dir = -1;
	ImVec2 pos = base_pos;
	pos.x = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
	pos.y = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
	return pos;
}

ImGuiWindow* ImGui::FindWindowByName(const char* name)
{
	ImGuiContext& g = *GImGui;
	ImGuiID id = ImHash(name, 0);
	for (int i = 0; i < g.Windows.Size; i++)
		if (g.Windows[i]->ID == id)
			return g.Windows[i];
	return NULL;
}

static ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags)
{
	ImGuiContext& g = *GImGui;

	ImGuiWindow* window = (ImGuiWindow*)ImGui::MemAlloc(sizeof(ImGuiWindow));
	IM_PLACEMENT_NEW(window) ImGuiWindow(name);
	window->Flags = flags;

	if (flags & ImGuiWindowFlags_NoSavedSettings)
	{
		window->Size = window->SizeFull = size;
	}
	else
	{
		window->PosFloat = ImVec2(60, 60);
		window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

		ImGuiIniData* settings = FindWindowSettings(name);
		if (!settings)
		{
			settings = AddWindowSettings(name);
		}
		else
		{
			window->SetWindowPosAllowFlags &= ~ImGuiSetCond_FirstUseEver;
			window->SetWindowSizeAllowFlags &= ~ImGuiSetCond_FirstUseEver;
			window->SetWindowCollapsedAllowFlags &= ~ImGuiSetCond_FirstUseEver;
		}

		if (settings->Pos.x != FLT_MAX)
		{
			window->PosFloat = settings->Pos;
			window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
			window->Collapsed = settings->Collapsed;
		}

		if (ImLengthSqr(settings->Size) > 0.00001f && !(flags & ImGuiWindowFlags_NoResize))
			size = settings->Size;
		window->Size = window->SizeFull = size;
	}

	if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0)
	{
		window->AutoFitFramesX = window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = false;
	}
	else
	{
		if (window->Size.x <= 0.0f)
			window->AutoFitFramesX = 2;
		if (window->Size.y <= 0.0f)
			window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
	}

	if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
		g.Windows.insert(g.Windows.begin(), window);     else
		g.Windows.push_back(window);
	return window;
}

static void ApplySizeFullWithConstraint(ImGuiWindow* window, ImVec2 new_size)
{
	ImGuiContext& g = *GImGui;
	if (g.SetNextWindowSizeConstraint)
	{
		ImRect cr = g.SetNextWindowSizeConstraintRect;
		new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
		new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
		if (g.SetNextWindowSizeConstraintCallback)
		{
			ImGuiSizeConstraintCallbackData data;
			data.UserData = g.SetNextWindowSizeConstraintCallbackUserData;
			data.Pos = window->Pos;
			data.CurrentSize = window->SizeFull;
			data.DesiredSize = new_size;
			g.SetNextWindowSizeConstraintCallback(&data);
			new_size = data.DesiredSize;
		}
	}
	if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
		new_size = ImMax(new_size, g.Style.WindowMinSize);
	window->SizeFull = new_size;
}

bool ImGui::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
	return ImGui::Begin(name, p_open, ImVec2(0.f, 0.f), -1.0f, flags);
}

bool ImGui::Begin(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha, ImGuiWindowFlags flags)
{
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	IM_ASSERT(name != NULL);                            IM_ASSERT(g.Initialized);                           IM_ASSERT(g.FrameCountEnded != g.FrameCount);
	if (flags & ImGuiWindowFlags_NoInputs)
		flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

	bool window_is_new = false;
	ImGuiWindow* window = FindWindowByName(name);
	if (!window)
	{
		window = CreateNewWindow(name, size_on_first_use, flags);
		window_is_new = true;
	}

	const int current_frame = ImGui::GetFrameCount();
	const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
	if (first_begin_of_the_frame)
		window->Flags = (ImGuiWindowFlags)flags;
	else
		flags = window->Flags;

	ImGuiWindow* parent_window = !g.CurrentWindowStack.empty() ? g.CurrentWindowStack.back() : NULL;
	g.CurrentWindowStack.push_back(window);
	SetCurrentWindow(window);
	CheckStacksSize(window, true);
	IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));

	bool window_was_active = (window->LastFrameActive == current_frame - 1);       if (flags & ImGuiWindowFlags_Popup)
	{
		ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
		window_was_active &= (window->PopupId == popup_ref.PopupId);
		window_was_active &= (window == popup_ref.Window);
		popup_ref.Window = window;
		g.CurrentPopupStack.push_back(popup_ref);
		window->PopupId = popup_ref.PopupId;
	}

	const bool window_appearing_after_being_hidden = (window->HiddenFrames == 1);

	bool window_pos_set_by_api = false, window_size_set_by_api = false;
	if (g.SetNextWindowPosCond)
	{
		const ImVec2 backup_cursor_pos = window->DC.CursorPos;                          if (!window_was_active || window_appearing_after_being_hidden) window->SetWindowPosAllowFlags |= ImGuiSetCond_Appearing;
		window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.SetNextWindowPosCond) != 0;
		if (window_pos_set_by_api && ImLengthSqr(g.SetNextWindowPosVal - ImVec2(-FLT_MAX, -FLT_MAX)) < 0.001f)
		{
			window->SetWindowPosCenterWanted = true;                                        window->SetWindowPosAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);
		}
		else
		{
			SetWindowPos(window, g.SetNextWindowPosVal, g.SetNextWindowPosCond);
		}
		window->DC.CursorPos = backup_cursor_pos;
		g.SetNextWindowPosCond = 0;
	}
	if (g.SetNextWindowSizeCond)
	{
		if (!window_was_active || window_appearing_after_being_hidden) window->SetWindowSizeAllowFlags |= ImGuiSetCond_Appearing;
		window_size_set_by_api = (window->SetWindowSizeAllowFlags & g.SetNextWindowSizeCond) != 0;
		SetWindowSize(window, g.SetNextWindowSizeVal, g.SetNextWindowSizeCond);
		g.SetNextWindowSizeCond = 0;
	}
	if (g.SetNextWindowContentSizeCond)
	{
		window->SizeContentsExplicit = g.SetNextWindowContentSizeVal;
		g.SetNextWindowContentSizeCond = 0;
	}
	else if (first_begin_of_the_frame)
	{
		window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
	}
	if (g.SetNextWindowCollapsedCond)
	{
		if (!window_was_active || window_appearing_after_being_hidden) window->SetWindowCollapsedAllowFlags |= ImGuiSetCond_Appearing;
		SetWindowCollapsed(window, g.SetNextWindowCollapsedVal, g.SetNextWindowCollapsedCond);
		g.SetNextWindowCollapsedCond = 0;
	}
	if (g.SetNextWindowFocus)
	{
		ImGui::SetWindowFocus();
		g.SetNextWindowFocus = false;
	}

	int root_idx, root_non_popup_idx;
	for (root_idx = g.CurrentWindowStack.Size - 1; root_idx > 0; root_idx--)
		if (!(g.CurrentWindowStack[root_idx]->Flags & ImGuiWindowFlags_ChildWindow))
			break;
	for (root_non_popup_idx = root_idx; root_non_popup_idx > 0; root_non_popup_idx--)
		if (!(g.CurrentWindowStack[root_non_popup_idx]->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)))
			break;
	window->ParentWindow = parent_window;
	window->RootWindow = g.CurrentWindowStack[root_idx];
	window->RootNonPopupWindow = g.CurrentWindowStack[root_non_popup_idx];
	if (first_begin_of_the_frame)
	{
		window->Active = true;
		window->IndexWithinParent = 0;
		window->BeginCount = 0;
		window->ClipRect = ImVec4(-FLT_MAX, -FLT_MAX, +FLT_MAX, +FLT_MAX);
		window->LastFrameActive = current_frame;
		window->IDStack.resize(1);

		window->DrawList->Clear();
		window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
		ImRect fullscreen_rect(GetVisibleRect());
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup)))
			PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
		else
			PushClipRect(fullscreen_rect.Min, fullscreen_rect.Max, true);

		if (!window_was_active)
		{
			window->AutoPosLastDirection = -1;
			if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
				window->PosFloat = g.IO.MousePos;
		}

		if (!(flags & ImGuiWindowFlags_NoTitleBar) && !(flags & ImGuiWindowFlags_NoCollapse))
		{
			ImRect title_bar_rect = window->TitleBarRect();
			if (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) && g.IO.MouseDoubleClicked[0])
			{
				window->Collapsed = !window->Collapsed;
				if (!(flags & ImGuiWindowFlags_NoSavedSettings))
					MarkSettingsDirty();
				FocusWindow(window);
			}
		}
		else
		{
			window->Collapsed = false;
		}


		window->SizeContents.x = (float)(int)((window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.x - window->Pos.x) + window->Scroll.x));
		window->SizeContents.y = (float)(int)((window->SizeContentsExplicit.y != 0.0f) ? window->SizeContentsExplicit.y : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.y - window->Pos.y) + window->Scroll.y));

		if (window->HiddenFrames > 0)
			window->HiddenFrames--;
		if ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0 && !window_was_active)
		{
			window->HiddenFrames = 1;
			if (flags & ImGuiWindowFlags_AlwaysAutoResize)
			{
				if (!window_size_set_by_api)
					window->Size = window->SizeFull = ImVec2(0.f, 0.f);
				window->SizeContents = ImVec2(0.f, 0.f);
			}
		}

		window->WindowPadding = ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup))) ? ImVec2(0, 0) : style.WindowPadding;

		ImVec2 size_auto_fit;
		if ((flags & ImGuiWindowFlags_Tooltip) != 0)
		{
			size_auto_fit = window->SizeContents + window->WindowPadding - ImVec2(0.0f, style.ItemSpacing.y);
		}
		else
		{
			size_auto_fit = ImClamp(window->SizeContents + window->WindowPadding, style.WindowMinSize, ImMax(style.WindowMinSize, g.IO.DisplaySize - g.Style.DisplaySafeAreaPadding));

			if (size_auto_fit.x < window->SizeContents.x && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar))
				size_auto_fit.y += style.ScrollbarSize;
			if (size_auto_fit.y < window->SizeContents.y && !(flags & ImGuiWindowFlags_NoScrollbar))
				size_auto_fit.x += style.ScrollbarSize;
			size_auto_fit.y = ImMax(size_auto_fit.y - style.ItemSpacing.y, 0.0f);
		}

		if (window->Collapsed)
		{
			if (window->AutoFitFramesX > 0)
				window->SizeFull.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
			if (window->AutoFitFramesY > 0)
				window->SizeFull.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
		}
		else
		{
			if ((flags & ImGuiWindowFlags_AlwaysAutoResize) && !window_size_set_by_api)
			{
				window->SizeFull = size_auto_fit;
			}
			else if ((window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0) && !window_size_set_by_api)
			{
				if (window->AutoFitFramesX > 0)
					window->SizeFull.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
				if (window->AutoFitFramesY > 0)
					window->SizeFull.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
				if (!(flags & ImGuiWindowFlags_NoSavedSettings))
					MarkSettingsDirty();
			}
		}

		ApplySizeFullWithConstraint(window, window->SizeFull);
		window->Size = window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;


		if (flags & ImGuiWindowFlags_ChildWindow)
		{
			window->IndexWithinParent = parent_window->DC.ChildWindows.Size;
			parent_window->DC.ChildWindows.push_back(window);
		}
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup))
		{
			window->Pos = window->PosFloat = parent_window->DC.CursorPos;
			window->Size = window->SizeFull = size_on_first_use;
		}

		bool window_pos_center = false;
		window_pos_center |= (window->SetWindowPosCenterWanted && window->HiddenFrames == 0);
		window_pos_center |= ((flags & ImGuiWindowFlags_Modal) && !window_pos_set_by_api && window_appearing_after_being_hidden);
		if (window_pos_center)
		{
			SetWindowPos(ImMax(style.DisplaySafeAreaPadding, fullscreen_rect.GetCenter() - window->SizeFull * 0.5f));
		}
		else if (flags & ImGuiWindowFlags_ChildMenu)
		{
			IM_ASSERT(window_pos_set_by_api);
			ImRect rect_to_avoid;
			if (parent_window->DC.MenuBarAppending)
				rect_to_avoid = ImRect(-FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight(), FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight() + parent_window->MenuBarHeight());
			else
				rect_to_avoid = ImRect(parent_window->Pos.x + style.ItemSpacing.x, -FLT_MAX, parent_window->Pos.x + parent_window->Size.x - style.ItemSpacing.x - parent_window->ScrollbarSizes.x, FLT_MAX);             window->PosFloat = FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
		}
		else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_appearing_after_being_hidden)
		{
			ImRect rect_to_avoid(window->PosFloat.x - 1, window->PosFloat.y - 1, window->PosFloat.x + 1, window->PosFloat.y + 1);
			window->PosFloat = FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
		}

		if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api)
		{
			ImRect rect_to_avoid(g.IO.MousePos.x - 16, g.IO.MousePos.y - 8, g.IO.MousePos.x + 24, g.IO.MousePos.y + 24);             window->PosFloat = FindBestPopupWindowPos(g.IO.MousePos, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
			if (window->AutoPosLastDirection == -1)
				window->PosFloat = g.IO.MousePos + ImVec2(2, 2);
		}

		if (!(flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Tooltip))
		{
			if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f) {
				ImVec2 padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
				window->PosFloat = ImMax(window->PosFloat + window->Size, padding) - window->Size;
				window->PosFloat = ImMin(window->PosFloat, g.IO.DisplaySize - padding);
			}
		}
		window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

		if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) && !(flags & ImGuiWindowFlags_AlwaysAutoResize))
			window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
		else
			window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);

		window->FocusIdxAllRequestCurrent = (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1) ? INT_MAX : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter + 1)) % (window->FocusIdxAllCounter + 1);
		window->FocusIdxTabRequestCurrent = (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1) ? INT_MAX : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter + 1)) % (window->FocusIdxTabCounter + 1);
		window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
		window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;

		if (window->ScrollTarget.x < FLT_MAX)
		{
			window->Scroll.x = window->ScrollTarget.x;
			window->ScrollTarget.x = FLT_MAX;
		}
		if (window->ScrollTarget.y < FLT_MAX)
		{
			float center_ratio = window->ScrollTargetCenterRatio.y;
			window->Scroll.y = window->ScrollTarget.y - ((1.0f - center_ratio) * (window->TitleBarHeight() + window->MenuBarHeight())) - (center_ratio * window->SizeFull.y);
			window->ScrollTarget.y = FLT_MAX;
		}
		window->Scroll = ImMax(window->Scroll, ImVec2(0.0f, 0.0f));
		if (!window->Collapsed && !window->SkipItems)
			window->Scroll = ImMin(window->Scroll, ImMax(ImVec2(0.0f, 0.0f), window->SizeContents - window->SizeFull + window->ScrollbarSizes));

		if ((flags & ImGuiWindowFlags_Modal) != 0 && window == GetFrontMostModalRootWindow())
			window->DrawList->AddRectFilled(fullscreen_rect.Min, fullscreen_rect.Max, GetColorU32(ImGuiCol_ModalWindowDarkening, g.ModalWindowDarkeningRatio));

		ImRect title_bar_rect = window->TitleBarRect();
		const float window_rounding = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
		if (window->Collapsed)
		{
			RenderFrame(title_bar_rect.GetTL(), title_bar_rect.GetBR(), GetColorU32(ImGuiCol_TitleBgCollapsed), true, window_rounding);
		}
		else
		{
			ImU32 resize_col = 0;
			const float resize_corner_size = ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
			if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && !(flags & ImGuiWindowFlags_NoResize))
			{
				const ImVec2 br = window->Rect().GetBR();
				const ImRect resize_rect(br - ImVec2(resize_corner_size * 0.75f, resize_corner_size * 0.75f), br);
				const ImGuiID resize_id = window->GetID("#RESIZE");
				bool hovered, held;
				ButtonBehavior(resize_rect, resize_id, &hovered, &held, ImGuiButtonFlags_FlattenChilds);
				resize_col = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);

				if (hovered || held)
					g.MouseCursor = ImGuiMouseCursor_ResizeNWSE;

				if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0])
				{
					ApplySizeFullWithConstraint(window, size_auto_fit);
					if (!(flags & ImGuiWindowFlags_NoSavedSettings))
						MarkSettingsDirty();
					SetActiveID(0);
				}
				else if (held)
				{
					ApplySizeFullWithConstraint(window, (g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize()) - window->Pos);
					if (!(flags & ImGuiWindowFlags_NoSavedSettings))
						MarkSettingsDirty();
				}

				window->Size = window->SizeFull;
				title_bar_rect = window->TitleBarRect();
			}

			window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) || ((window->SizeContents.y > window->Size.y + style.ItemSpacing.y) && !(flags & ImGuiWindowFlags_NoScrollbar));
			window->ScrollbarX = (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) || ((window->SizeContents.x > window->Size.x - (window->ScrollbarY ? style.ScrollbarSize : 0.0f) - window->WindowPadding.x) && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
			window->ScrollbarSizes = ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
			window->BorderSize = (flags & ImGuiWindowFlags_ShowBorders) ? 1.0f : 0.0f;

			ImGuiCol bg_color_idx = ImGuiCol_WindowBg;
			if ((flags & ImGuiWindowFlags_ComboBox) != 0)
				bg_color_idx = ImGuiCol_ComboBg;
			else if ((flags & ImGuiWindowFlags_Tooltip) != 0 || (flags & ImGuiWindowFlags_Popup) != 0)
				bg_color_idx = ImGuiCol_PopupBg;
			else if ((flags & ImGuiWindowFlags_ChildWindow) != 0)
				bg_color_idx = ImGuiCol_ChildWindowBg;
			ImVec4 bg_color = style.Colors[bg_color_idx];
			if (bg_alpha >= 0.0f)
				bg_color.w = bg_alpha;
			bg_color.w *= style.Alpha;
			if (bg_color.w > 0.0f)
				window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()), window->Pos + window->Size, ColorConvertFloat4ToU32(bg_color), window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? 15 : 4 | 8);

			if (!(flags & ImGuiWindowFlags_NoTitleBar))
				window->DrawList->AddRectFilled(title_bar_rect.GetTL(), title_bar_rect.GetBR(), GetColorU32((g.FocusedWindow && window->RootNonPopupWindow == g.FocusedWindow->RootNonPopupWindow) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg), window_rounding, 1 | 2);

			if (flags & ImGuiWindowFlags_MenuBar)
			{
				ImRect menu_bar_rect = window->MenuBarRect();
				window->DrawList->AddRectFilled(menu_bar_rect.GetTL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_MenuBarBg), (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f, 1 | 2);
				if (flags & ImGuiWindowFlags_ShowBorders)
					window->DrawList->AddLine(menu_bar_rect.GetBL() - ImVec2(0, 0), menu_bar_rect.GetBR() - ImVec2(0, 0), GetColorU32(ImGuiCol_Border));
			}

			if (window->ScrollbarX)
				Scrollbar(window, true);
			if (window->ScrollbarY)
				Scrollbar(window, false);

			if (!(flags & ImGuiWindowFlags_NoResize))
			{
				const ImVec2 br = window->Rect().GetBR();
				window->DrawList->PathLineTo(br + ImVec2(-resize_corner_size, -window->BorderSize));
				window->DrawList->PathLineTo(br + ImVec2(-window->BorderSize, -resize_corner_size));
				window->DrawList->PathArcToFast(ImVec2(br.x - window_rounding - window->BorderSize, br.y - window_rounding - window->BorderSize), window_rounding, 0, 3);
				window->DrawList->PathFill(resize_col);
			}

			if (flags & ImGuiWindowFlags_ShowBorders)
			{
				window->DrawList->AddRect(window->Pos + ImVec2(1, 1), window->Pos + window->Size + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), window_rounding);
				window->DrawList->AddRect(window->Pos, window->Pos + window->Size, GetColorU32(ImGuiCol_Border), window_rounding);
				if (!(flags & ImGuiWindowFlags_NoTitleBar))
					window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(1, 0), title_bar_rect.GetBR() - ImVec2(1, 0), GetColorU32(ImGuiCol_Border));
			}
		}

		window->ContentsRegionRect.Min.x = -window->Scroll.x + window->WindowPadding.x;
		window->ContentsRegionRect.Min.y = -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
		window->ContentsRegionRect.Max.x = -window->Scroll.x - window->WindowPadding.x + (window->SizeContentsExplicit.x != 0.0f ? window->SizeContentsExplicit.x : (window->Size.x - window->ScrollbarSizes.x));
		window->ContentsRegionRect.Max.y = -window->Scroll.y - window->WindowPadding.y + (window->SizeContentsExplicit.y != 0.0f ? window->SizeContentsExplicit.y : (window->Size.y - window->ScrollbarSizes.y));

		window->DC.IndentX = 0.0f + window->WindowPadding.x - window->Scroll.x;
		window->DC.GroupOffsetX = 0.0f;
		window->DC.ColumnsOffsetX = 0.0f;
		window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.IndentX + window->DC.ColumnsOffsetX, window->TitleBarHeight() + window->MenuBarHeight() + window->WindowPadding.y - window->Scroll.y);
		window->DC.CursorPos = window->DC.CursorStartPos;
		window->DC.CursorPosPrevLine = window->DC.CursorPos;
		window->DC.CursorMaxPos = window->DC.CursorStartPos;
		window->DC.CurrentLineHeight = window->DC.PrevLineHeight = 0.0f;
		window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;
		window->DC.MenuBarAppending = false;
		window->DC.MenuBarOffsetX = ImMax(window->WindowPadding.x, style.ItemSpacing.x);
		window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
		window->DC.ChildWindows.resize(0);
		window->DC.LayoutType = ImGuiLayoutType_Vertical;
		window->DC.ItemWidth = window->ItemWidthDefault;
		window->DC.TextWrapPos = -1.0f;         window->DC.AllowKeyboardFocus = true;
		window->DC.ButtonRepeat = false;
		window->DC.ItemWidthStack.resize(0);
		window->DC.AllowKeyboardFocusStack.resize(0);
		window->DC.ButtonRepeatStack.resize(0);
		window->DC.TextWrapPosStack.resize(0);
		window->DC.ColumnsCurrent = 0;
		window->DC.ColumnsCount = 1;
		window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
		window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.ColumnsStartPosY;
		window->DC.TreeDepth = 0;
		window->DC.StateStorage = &window->StateStorage;
		window->DC.GroupStack.resize(0);
		window->MenuColumns.Update(3, style.ItemSpacing.x, !window_was_active);

		if (window->AutoFitFramesX > 0)
			window->AutoFitFramesX--;
		if (window->AutoFitFramesY > 0)
			window->AutoFitFramesY--;

		if (!window_was_active && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
			if (!(flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Tooltip)) || (flags & ImGuiWindowFlags_Popup))
				FocusWindow(window);

		if (!(flags & ImGuiWindowFlags_NoTitleBar))
		{
			if (p_open != NULL)
			{
				const float pad = 2.0f;
				const float rad = (window->TitleBarHeight() - pad*2.0f) * 0.5f;
				if (CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad), rad))
					*p_open = false;
			}

			const ImVec2 text_size = CalcTextSize(name, NULL, true);
			if (!(flags & ImGuiWindowFlags_NoCollapse))
				RenderCollapseTriangle(window->Pos + style.FramePadding, !window->Collapsed, 1.0f, true);

			ImVec2 text_min = window->Pos + style.FramePadding;
			ImVec2 text_max = window->Pos + ImVec2(window->Size.x - style.FramePadding.x, style.FramePadding.y * 2 + text_size.y);
			ImVec2 clip_max = ImVec2(window->Pos.x + window->Size.x - (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x), text_max.y);             bool pad_left = (flags & ImGuiWindowFlags_NoCollapse) == 0;
			bool pad_right = (p_open != NULL);
			if (style.WindowTitleAlign & ImGuiAlign_Center) pad_right = pad_left;
			if (pad_left) text_min.x += g.FontSize + style.ItemInnerSpacing.x;
			if (pad_right) text_max.x -= g.FontSize + style.ItemInnerSpacing.x;
			ImVec2 clip_min = ImVec2(text_min.x, window->Pos.y);
			RenderTextClipped(text_min, text_max, name, NULL, &text_size, style.WindowTitleAlign, &clip_min, &clip_max);
		}

		window->WindowRectClipped = window->Rect();
		window->WindowRectClipped.Clip(window->ClipRect);


	}

	const ImRect title_bar_rect = window->TitleBarRect();
	const float border_size = window->BorderSize;
	ImRect clip_rect;     clip_rect.Min.x = ImFloor(0.5f + title_bar_rect.Min.x + ImMax(border_size, ImFloor(window->WindowPadding.x*0.5f)));
	clip_rect.Min.y = ImFloor(0.5f + title_bar_rect.Max.y + window->MenuBarHeight() + border_size);
	clip_rect.Max.x = ImFloor(0.5f + window->Pos.x + window->Size.x - window->ScrollbarSizes.x - ImMax(border_size, ImFloor(window->WindowPadding.x*0.5f)));
	clip_rect.Max.y = ImFloor(0.5f + window->Pos.y + window->Size.y - window->ScrollbarSizes.y - border_size);
	PushClipRect(clip_rect.Min, clip_rect.Max, true);

	if (first_begin_of_the_frame)
		window->Accessed = false;
	window->BeginCount++;
	g.SetNextWindowSizeConstraint = false;

	if (flags & ImGuiWindowFlags_ChildWindow)
	{
		IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);
		window->Collapsed = parent_window && parent_window->Collapsed;

		if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
			window->Collapsed |= (window->WindowRectClipped.Min.x >= window->WindowRectClipped.Max.x || window->WindowRectClipped.Min.y >= window->WindowRectClipped.Max.y);

		if (window->Collapsed)
			window->Active = false;
	}
	if (style.Alpha <= 0.0f)
		window->Active = false;

	window->SkipItems = (window->Collapsed || !window->Active) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0;
	return !window->SkipItems;
}

void ImGui::End()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	Columns(1, "#CloseColumns");
	PopClipRect();
	if (!(window->Flags & ImGuiWindowFlags_ChildWindow))            LogFinish();

	g.CurrentWindowStack.pop_back();
	if (window->Flags & ImGuiWindowFlags_Popup)
		g.CurrentPopupStack.pop_back();
	CheckStacksSize(window, false);
	SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
}

static void Scrollbar(ImGuiWindow* window, bool horizontal)
{
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(horizontal ? "#SCROLLX" : "#SCROLLY");

	bool other_scrollbar = (horizontal ? window->ScrollbarY : window->ScrollbarX);
	float other_scrollbar_size_w = other_scrollbar ? style.ScrollbarSize : 0.0f;
	const ImRect window_rect = window->Rect();
	const float border_size = window->BorderSize;
	ImRect bb = horizontal
		? ImRect(window->Pos.x + border_size, window_rect.Max.y - style.ScrollbarSize, window_rect.Max.x - other_scrollbar_size_w - border_size, window_rect.Max.y - border_size)
		: ImRect(window_rect.Max.x - style.ScrollbarSize, window->Pos.y + border_size, window_rect.Max.x - border_size, window_rect.Max.y - other_scrollbar_size_w - border_size);
	if (!horizontal)
		bb.Min.y += window->TitleBarHeight() + ((window->Flags & ImGuiWindowFlags_MenuBar) ? window->MenuBarHeight() : 0.0f);

	float window_rounding = (window->Flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
	int window_rounding_corners;
	if (horizontal)
		window_rounding_corners = 8 | (other_scrollbar ? 0 : 4);
	else
		window_rounding_corners = (((window->Flags & ImGuiWindowFlags_NoTitleBar) && !(window->Flags & ImGuiWindowFlags_MenuBar)) ? 2 : 0) | (other_scrollbar ? 0 : 4);
	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_ScrollbarBg), window_rounding, window_rounding_corners);
	bb.Reduce(ImVec2(ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f), ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));

	float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();
	float scroll_v = horizontal ? window->Scroll.x : window->Scroll.y;
	float win_size_avail_v = (horizontal ? window->Size.x : window->Size.y) - other_scrollbar_size_w;
	float win_size_contents_v = horizontal ? window->SizeContents.x : window->SizeContents.y;

	const float grab_h_pixels = ImMin(ImMax(scrollbar_size_v * ImSaturate(win_size_avail_v / ImMax(win_size_contents_v, win_size_avail_v)), style.GrabMinSize), scrollbar_size_v);
	const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

	bool held = false;
	bool hovered = false;
	const bool previously_held = (g.ActiveId == id);
	ImGui::ButtonBehavior(bb, id, &hovered, &held);

	float scroll_max = ImMax(1.0f, win_size_contents_v - win_size_avail_v);
	float scroll_ratio = ImSaturate(scroll_v / scroll_max);
	float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
	if (held && grab_h_norm < 1.0f)
	{
		float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
		float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
		float* click_delta_to_grab_center_v = horizontal ? &g.ScrollbarClickDeltaToGrabCenter.x : &g.ScrollbarClickDeltaToGrabCenter.y;

		const float clicked_v_norm = ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
		ImGui::SetHoveredID(id);

		bool seek_absolute = false;
		if (!previously_held)
		{
			if (clicked_v_norm >= grab_v_norm && clicked_v_norm <= grab_v_norm + grab_h_norm)
			{
				*click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm*0.5f;
			}
			else
			{
				seek_absolute = true;
				*click_delta_to_grab_center_v = 0.0f;
			}
		}

		const float scroll_v_norm = ImSaturate((clicked_v_norm - *click_delta_to_grab_center_v - grab_h_norm*0.5f) / (1.0f - grab_h_norm));
		scroll_v = (float)(int)(0.5f + scroll_v_norm * scroll_max);        if (horizontal)
			window->Scroll.x = scroll_v;
		else
			window->Scroll.y = scroll_v;

		scroll_ratio = ImSaturate(scroll_v / scroll_max);
		grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

		if (seek_absolute)
			*click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm*0.5f;
	}

	const ImU32 grab_col = ImGui::GetColorU32(held ? ImGuiCol_ScrollbarGrabActive : hovered ? ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);
	if (horizontal)
		window->DrawList->AddRectFilled(ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y), ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, bb.Max.y), grab_col, style.ScrollbarRounding);
	else
		window->DrawList->AddRectFilled(ImVec2(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm)), ImVec2(bb.Max.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels), grab_col, style.ScrollbarRounding);
}

void ImGui::FocusWindow(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;

	g.FocusedWindow = window;

	if (!window)
		return;

	if (window->RootWindow)
		window = window->RootWindow;

	if (window->Flags & ImGuiWindowFlags_Popup)         if (g.ActiveId != 0 && g.ActiveIdWindow && g.ActiveIdWindow->RootWindow != window)
		SetActiveID(0);

	if ((window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus) || g.Windows.back() == window)
		return;
	for (int i = 0; i < g.Windows.Size; i++)
		if (g.Windows[i] == window)
		{
			g.Windows.erase(g.Windows.begin() + i);
			break;
		}
	g.Windows.push_back(window);
}

void ImGui::PushItemWidth(float item_width)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault : item_width);
	window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

static void PushMultiItemsWidths(int components, float w_full)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImGuiStyle& style = GImGui->Style;
	if (w_full <= 0.0f)
		w_full = ImGui::CalcItemWidth();
	const float w_item_one = ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
	const float w_item_last = ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));
	window->DC.ItemWidthStack.push_back(w_item_last);
	for (int i = 0; i < components - 1; i++)
		window->DC.ItemWidthStack.push_back(w_item_one);
	window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ItemWidthStack.pop_back();
	window->DC.ItemWidth = window->DC.ItemWidthStack.empty() ? window->ItemWidthDefault : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	float w = window->DC.ItemWidth;
	if (w < 0.0f)
	{
		float width_to_right_edge = GetContentRegionAvail().x;
		w = ImMax(1.0f, width_to_right_edge + w);
	}
	w = (float)(int)w;
	return w;
}

static void SetCurrentFont(ImFont* font)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(font && font->IsLoaded());        IM_ASSERT(font->Scale > 0.0f);
	g.Font = font;
	g.FontBaseSize = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
	g.FontSize = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;
	g.FontTexUvWhitePixel = g.Font->ContainerAtlas->TexUvWhitePixel;
}

void ImGui::PushFont(ImFont* font)
{
	ImGuiContext& g = *GImGui;
	if (!font)
		font = g.IO.Fonts->Fonts[0];
	SetCurrentFont(font);
	g.FontStack.push_back(font);
	g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void  ImGui::PopFont()
{
	ImGuiContext& g = *GImGui;
	g.CurrentWindow->DrawList->PopTextureID();
	g.FontStack.pop_back();
	SetCurrentFont(g.FontStack.empty() ? g.IO.Fonts->Fonts[0] : g.FontStack.back());
}

void ImGui::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.AllowKeyboardFocus = allow_keyboard_focus;
	window->DC.AllowKeyboardFocusStack.push_back(allow_keyboard_focus);
}

void ImGui::PopAllowKeyboardFocus()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.AllowKeyboardFocusStack.pop_back();
	window->DC.AllowKeyboardFocus = window->DC.AllowKeyboardFocusStack.empty() ? true : window->DC.AllowKeyboardFocusStack.back();
}

void ImGui::PushButtonRepeat(bool repeat)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ButtonRepeat = repeat;
	window->DC.ButtonRepeatStack.push_back(repeat);
}

void ImGui::PopButtonRepeat()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ButtonRepeatStack.pop_back();
	window->DC.ButtonRepeat = window->DC.ButtonRepeatStack.empty() ? false : window->DC.ButtonRepeatStack.back();
}

void ImGui::PushTextWrapPos(float wrap_pos_x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.TextWrapPos = wrap_pos_x;
	window->DC.TextWrapPosStack.push_back(wrap_pos_x);
}

void ImGui::PopTextWrapPos()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.TextWrapPosStack.pop_back();
	window->DC.TextWrapPos = window->DC.TextWrapPosStack.empty() ? -1.0f : window->DC.TextWrapPosStack.back();
}

void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
	ImGuiContext& g = *GImGui;
	ImGuiColMod backup;
	backup.Col = idx;
	backup.PreviousValue = g.Style.Colors[idx];
	g.ColorModifiers.push_back(backup);
	g.Style.Colors[idx] = col;
}

void ImGui::PopStyleColor(int count)
{
	ImGuiContext& g = *GImGui;
	while (count > 0)
	{
		ImGuiColMod& backup = g.ColorModifiers.back();
		g.Style.Colors[backup.Col] = backup.PreviousValue;
		g.ColorModifiers.pop_back();
		count--;
	}
}

static float* GetStyleVarFloatAddr(ImGuiStyleVar idx)
{
	ImGuiContext& g = *GImGui;
	switch (idx)
	{
	case ImGuiStyleVar_Alpha: return &g.Style.Alpha;
	case ImGuiStyleVar_WindowRounding: return &g.Style.WindowRounding;
	case ImGuiStyleVar_ChildWindowRounding: return &g.Style.ChildWindowRounding;
	case ImGuiStyleVar_FrameRounding: return &g.Style.FrameRounding;
	case ImGuiStyleVar_IndentSpacing: return &g.Style.IndentSpacing;
	case ImGuiStyleVar_GrabMinSize: return &g.Style.GrabMinSize;
	}
	return NULL;
}

static ImVec2* GetStyleVarVec2Addr(ImGuiStyleVar idx)
{
	ImGuiContext& g = *GImGui;
	switch (idx)
	{
	case ImGuiStyleVar_WindowPadding: return &g.Style.WindowPadding;
	case ImGuiStyleVar_WindowMinSize: return &g.Style.WindowMinSize;
	case ImGuiStyleVar_FramePadding: return &g.Style.FramePadding;
	case ImGuiStyleVar_ItemSpacing: return &g.Style.ItemSpacing;
	case ImGuiStyleVar_ItemInnerSpacing: return &g.Style.ItemInnerSpacing;
	}
	return NULL;
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, float val)
{
	ImGuiContext& g = *GImGui;
	float* pvar = GetStyleVarFloatAddr(idx);
	IM_ASSERT(pvar != NULL);     ImGuiStyleMod backup;
	backup.Var = idx;
	backup.PreviousValue = ImVec2(*pvar, 0.0f);
	g.StyleModifiers.push_back(backup);
	*pvar = val;
}


void ImGui::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
	ImGuiContext& g = *GImGui;
	ImVec2* pvar = GetStyleVarVec2Addr(idx);
	IM_ASSERT(pvar != NULL);     ImGuiStyleMod backup;
	backup.Var = idx;
	backup.PreviousValue = *pvar;
	g.StyleModifiers.push_back(backup);
	*pvar = val;
}

void ImGui::PopStyleVar(int count)
{
	ImGuiContext& g = *GImGui;
	while (count > 0)
	{
		ImGuiStyleMod& backup = g.StyleModifiers.back();
		if (float* pvar_f = GetStyleVarFloatAddr(backup.Var))
			*pvar_f = backup.PreviousValue.x;
		else if (ImVec2* pvar_v = GetStyleVarVec2Addr(backup.Var))
			*pvar_v = backup.PreviousValue;
		g.StyleModifiers.pop_back();
		count--;
	}
}

const char* ImGui::GetStyleColName(ImGuiCol idx)
{
	switch (idx)
	{
	case ImGuiCol_Text: return "Text";
	case ImGuiCol_TextDisabled: return "TextDisabled";
	case ImGuiCol_WindowBg: return "WindowBg";
	case ImGuiCol_ChildWindowBg: return "ChildWindowBg";
	case ImGuiCol_PopupBg: return "PopupBg";
	case ImGuiCol_Border: return "Border";
	case ImGuiCol_BorderShadow: return "BorderShadow";
	case ImGuiCol_FrameBg: return "FrameBg";
	case ImGuiCol_FrameBgHovered: return "FrameBgHovered";
	case ImGuiCol_FrameBgActive: return "FrameBgActive";
	case ImGuiCol_TitleBg: return "TitleBg";
	case ImGuiCol_TitleBgCollapsed: return "TitleBgCollapsed";
	case ImGuiCol_TitleBgActive: return "TitleBgActive";
	case ImGuiCol_MenuBarBg: return "MenuBarBg";
	case ImGuiCol_ScrollbarBg: return "ScrollbarBg";
	case ImGuiCol_ScrollbarGrab: return "ScrollbarGrab";
	case ImGuiCol_ScrollbarGrabHovered: return "ScrollbarGrabHovered";
	case ImGuiCol_ScrollbarGrabActive: return "ScrollbarGrabActive";
	case ImGuiCol_ComboBg: return "ComboBg";
	case ImGuiCol_CheckMark: return "CheckMark";
	case ImGuiCol_SliderGrab: return "SliderGrab";
	case ImGuiCol_SliderGrabActive: return "SliderGrabActive";
	case ImGuiCol_Button: return "Button";
	case ImGuiCol_ButtonHovered: return "ButtonHovered";
	case ImGuiCol_ButtonActive: return "ButtonActive";
	case ImGuiCol_Header: return "Header";
	case ImGuiCol_HeaderHovered: return "HeaderHovered";
	case ImGuiCol_HeaderActive: return "HeaderActive";
	case ImGuiCol_Column: return "Column";
	case ImGuiCol_ColumnHovered: return "ColumnHovered";
	case ImGuiCol_ColumnActive: return "ColumnActive";
	case ImGuiCol_ResizeGrip: return "ResizeGrip";
	case ImGuiCol_ResizeGripHovered: return "ResizeGripHovered";
	case ImGuiCol_ResizeGripActive: return "ResizeGripActive";
	case ImGuiCol_CloseButton: return "CloseButton";
	case ImGuiCol_CloseButtonHovered: return "CloseButtonHovered";
	case ImGuiCol_CloseButtonActive: return "CloseButtonActive";
	case ImGuiCol_PlotLines: return "PlotLines";
	case ImGuiCol_PlotLinesHovered: return "PlotLinesHovered";
	case ImGuiCol_PlotHistogram: return "PlotHistogram";
	case ImGuiCol_PlotHistogramHovered: return "PlotHistogramHovered";
	case ImGuiCol_TextSelectedBg: return "TextSelectedBg";
	case ImGuiCol_ModalWindowDarkening: return "ModalWindowDarkening";
	}
	IM_ASSERT(0);
	return "Unknown";
}

bool ImGui::IsWindowHovered()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredWindow == g.CurrentWindow && IsWindowContentHoverable(g.HoveredRootWindow);
}

bool ImGui::IsWindowFocused()
{
	ImGuiContext& g = *GImGui;
	return g.FocusedWindow == g.CurrentWindow;
}

bool ImGui::IsRootWindowFocused()
{
	ImGuiContext& g = *GImGui;
	return g.FocusedWindow == g.CurrentWindow->RootWindow;
}

bool ImGui::IsRootWindowOrAnyChildFocused()
{
	ImGuiContext& g = *GImGui;
	return g.FocusedWindow && g.FocusedWindow->RootWindow == g.CurrentWindow->RootWindow;
}

bool ImGui::IsRootWindowOrAnyChildHovered()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredRootWindow && (g.HoveredRootWindow == g.CurrentWindow->RootWindow) && IsWindowContentHoverable(g.HoveredRootWindow);
}

float ImGui::GetWindowWidth()
{
	ImGuiWindow* window = GImGui->CurrentWindow;
	return window->Size.x;
}

float ImGui::GetWindowHeight()
{
	ImGuiWindow* window = GImGui->CurrentWindow;
	return window->Size.y;
}

ImVec2 ImGui::GetWindowPos()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	return window->Pos;
}

static void SetWindowScrollY(ImGuiWindow* window, float new_scroll_y)
{
	window->DC.CursorMaxPos.y += window->Scroll.y;
	window->Scroll.y = new_scroll_y;
	window->DC.CursorMaxPos.y -= window->Scroll.y;
}

static void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiSetCond cond)
{
	if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
		return;
	window->SetWindowPosAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);
	window->SetWindowPosCenterWanted = false;

	const ImVec2 old_pos = window->Pos;
	window->PosFloat = pos;
	window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
	window->DC.CursorPos += (window->Pos - old_pos);        window->DC.CursorMaxPos += (window->Pos - old_pos);
}

void ImGui::SetWindowPos(const ImVec2& pos, ImGuiSetCond cond)
{
	ImGuiWindow* window = GetCurrentWindow();
	SetWindowPos(window, pos, cond);
}

void ImGui::SetWindowPos(const char* name, const ImVec2& pos, ImGuiSetCond cond)
{
	ImGuiWindow* window = FindWindowByName(name);
	if (window)
		SetWindowPos(window, pos, cond);
}

ImVec2 ImGui::GetWindowSize()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->Size;
}

static void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiSetCond cond)
{
	if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
		return;
	window->SetWindowSizeAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

	if (size.x > 0.0f)
	{
		window->AutoFitFramesX = 0;
		window->SizeFull.x = size.x;
	}
	else
	{
		window->AutoFitFramesX = 2;
		window->AutoFitOnlyGrows = false;
	}
	if (size.y > 0.0f)
	{
		window->AutoFitFramesY = 0;
		window->SizeFull.y = size.y;
	}
	else
	{
		window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = false;
	}
}

void ImGui::SetWindowSize(const ImVec2& size, ImGuiSetCond cond)
{
	SetWindowSize(GImGui->CurrentWindow, size, cond);
}

void ImGui::SetWindowSize(const char* name, const ImVec2& size, ImGuiSetCond cond)
{
	ImGuiWindow* window = FindWindowByName(name);
	if (window)
		SetWindowSize(window, size, cond);
}

static void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiSetCond cond)
{
	if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
		return;
	window->SetWindowCollapsedAllowFlags &= ~(ImGuiSetCond_Once | ImGuiSetCond_FirstUseEver | ImGuiSetCond_Appearing);

	window->Collapsed = collapsed;
}

void ImGui::SetWindowCollapsed(bool collapsed, ImGuiSetCond cond)
{
	SetWindowCollapsed(GImGui->CurrentWindow, collapsed, cond);
}

bool ImGui::IsWindowCollapsed()
{
	return GImGui->CurrentWindow->Collapsed;
}

void ImGui::SetWindowCollapsed(const char* name, bool collapsed, ImGuiSetCond cond)
{
	ImGuiWindow* window = FindWindowByName(name);
	if (window)
		SetWindowCollapsed(window, collapsed, cond);
}

void ImGui::SetWindowFocus()
{
	FocusWindow(GImGui->CurrentWindow);
}

void ImGui::SetWindowFocus(const char* name)
{
	if (name)
	{
		if (ImGuiWindow* window = FindWindowByName(name))
			FocusWindow(window);
	}
	else
	{
		FocusWindow(NULL);
	}
}

void ImGui::SetNextWindowPos(const ImVec2& pos, ImGuiSetCond cond)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowPosVal = pos;
	g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowPosCenter(ImGuiSetCond cond)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowPosVal = ImVec2(-FLT_MAX, -FLT_MAX);
	g.SetNextWindowPosCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiSetCond cond)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowSizeVal = size;
	g.SetNextWindowSizeCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeConstraintCallback custom_callback, void* custom_callback_user_data)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowSizeConstraint = true;
	g.SetNextWindowSizeConstraintRect = ImRect(size_min, size_max);
	g.SetNextWindowSizeConstraintCallback = custom_callback;
	g.SetNextWindowSizeConstraintCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2& size)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowContentSizeVal = size;
	g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowContentWidth(float width)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowContentSizeVal = ImVec2(width, g.SetNextWindowContentSizeCond ? g.SetNextWindowContentSizeVal.y : 0.0f);
	g.SetNextWindowContentSizeCond = ImGuiSetCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond)
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowCollapsedVal = collapsed;
	g.SetNextWindowCollapsedCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::SetNextWindowFocus()
{
	ImGuiContext& g = *GImGui;
	g.SetNextWindowFocus = true;
}

ImVec2 ImGui::GetContentRegionMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImVec2 mx = window->ContentsRegionRect.Max;
	if (window->DC.ColumnsCount != 1)
		mx.x = GetColumnOffset(window->DC.ColumnsCurrent + 1) - window->WindowPadding.x;
	return mx;
}

ImVec2 ImGui::GetContentRegionAvail()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return GetContentRegionMax() - (window->DC.CursorPos - window->Pos);
}

float ImGui::GetContentRegionAvailWidth()
{
	return GetContentRegionAvail().x;
}

ImVec2 ImGui::GetWindowContentRegionMin()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Min;
}

ImVec2 ImGui::GetWindowContentRegionMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Max;
}

float ImGui::GetWindowContentRegionWidth()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Max.x - window->ContentsRegionRect.Min.x;
}

float ImGui::GetTextLineHeight()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize;
}

float ImGui::GetTextLineHeightWithSpacing()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + g.Style.ItemSpacing.y;
}

float ImGui::GetItemsLineHeightWithSpacing()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

ImDrawList* ImGui::GetWindowDrawList()
{
	ImGuiWindow* window = GetCurrentWindow();
	return window->DrawList;
}

ImFont* ImGui::GetFont()
{
	return GImGui->Font;
}

float ImGui::GetFontSize()
{
	return GImGui->FontSize;
}

ImVec2 ImGui::GetFontTexUvWhitePixel()
{
	return GImGui->FontTexUvWhitePixel;
}

void ImGui::SetWindowFontScale(float scale)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->FontWindowScale = scale;
	g.FontSize = window->CalcFontSize();
}

ImVec2 ImGui::GetCursorPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos - window->Pos + window->Scroll;
}

float ImGui::GetCursorPosX()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float ImGui::GetCursorPosY()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void ImGui::SetCursorPos(const ImVec2& local_pos)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
	window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

void ImGui::SetCursorPosX(float x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
	window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
	window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
}

ImVec2 ImGui::GetCursorStartPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorStartPos - window->Pos;
}

ImVec2 ImGui::GetCursorScreenPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos;
}

void ImGui::SetCursorScreenPos(const ImVec2& screen_pos)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos = screen_pos;
	window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

float ImGui::GetScrollX()
{
	return GImGui->CurrentWindow->Scroll.x;
}

float ImGui::GetScrollY()
{
	return GImGui->CurrentWindow->Scroll.y;
}

float ImGui::GetScrollMaxX()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->SizeContents.x - window->SizeFull.x - window->ScrollbarSizes.x;
}

float ImGui::GetScrollMaxY()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->SizeContents.y - window->SizeFull.y - window->ScrollbarSizes.y;
}

void ImGui::SetScrollX(float scroll_x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->ScrollTarget.x = scroll_x;
	window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->ScrollTarget.y = scroll_y + window->TitleBarHeight() + window->MenuBarHeight();     window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
	ImGuiWindow* window = GetCurrentWindow();
	IM_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
	window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
	if (center_y_ratio <= 0.0f && window->ScrollTarget.y <= window->WindowPadding.y)            window->ScrollTarget.y = 0.0f;
	window->ScrollTargetCenterRatio.y = center_y_ratio;
}

void ImGui::SetScrollHere(float center_y_ratio)
{
	ImGuiWindow* window = GetCurrentWindow();
	float target_y = window->DC.CursorPosPrevLine.y + (window->DC.PrevLineHeight * center_y_ratio) + (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) * 2.0f);     SetScrollFromPosY(target_y - window->Pos.y, center_y_ratio);
}

void ImGui::SetKeyboardFocusHere(int offset)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
	window->FocusIdxTabRequestNext = INT_MAX;
}

void ImGui::SetStateStorage(ImGuiStorage* tree)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.StateStorage = tree ? tree : &window->StateStorage;
}

ImGuiStorage* ImGui::GetStateStorage()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.StateStorage;
}

void ImGui::TextV(const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	TextUnformatted(g.TempBuffer, text_end);
}

void ImGui::Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextV(fmt, args);
	va_end(args);
}

void ImGui::TextColoredV(const ImVec4& col, const char* fmt, va_list args)
{
	PushStyleColor(ImGuiCol_Text, col);
	TextV(fmt, args);
	PopStyleColor();
}

void ImGui::TextColored(const ImVec4& col, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextColoredV(col, fmt, args);
	va_end(args);
}

void ImGui::TextDisabledV(const char* fmt, va_list args)
{
	PushStyleColor(ImGuiCol_Text, GImGui->Style.Colors[ImGuiCol_TextDisabled]);
	TextV(fmt, args);
	PopStyleColor();
}

void ImGui::TextDisabled(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextDisabledV(fmt, args);
	va_end(args);
}

void ImGui::TextWrappedV(const char* fmt, va_list args)
{
	bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);        if (need_wrap) PushTextWrapPos(0.0f);
	TextV(fmt, args);
	if (need_wrap) PopTextWrapPos();
}

void ImGui::TextWrapped(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextWrappedV(fmt, args);
	va_end(args);
}

void ImGui::TextUnformatted(const char* text, const char* text_end)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	IM_ASSERT(text != NULL);
	const char* text_begin = text;
	if (text_end == NULL)
		text_end = text + strlen(text);
	const float wrap_pos_x = window->DC.TextWrapPos;
	const bool wrap_enabled = wrap_pos_x >= 0.0f;
	if (text_end - text > 2000 && !wrap_enabled)
	{
		const char* line = text;
		const float line_height = GetTextLineHeight();
		const ImVec2 text_pos = window->DC.CursorPos + ImVec2(0.0f, window->DC.CurrentLineTextBaseOffset);
		const ImRect clip_rect = window->ClipRect;
		ImVec2 text_size(0, 0);

		if (text_pos.y <= clip_rect.Max.y)
		{
			ImVec2 pos = text_pos;

			if (!g.LogEnabled)
			{
				int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
				if (lines_skippable > 0)
				{
					int lines_skipped = 0;
					while (line < text_end && lines_skipped < lines_skippable)
					{
						const char* line_end = strchr(line, '\n');
						if (!line_end)
							line_end = text_end;
						line = line_end + 1;
						lines_skipped++;
					}
					pos.y += lines_skipped * line_height;
				}
			}

			if (line < text_end)
			{
				ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (IsClippedEx(line_rect, NULL, false))
						break;

					const ImVec2 line_size = CalcTextSize(line, line_end, false);
					text_size.x = ImMax(text_size.x, line_size.x);
					RenderText(pos, line, line_end, false);
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					line_rect.Min.y += line_height;
					line_rect.Max.y += line_height;
					pos.y += line_height;
				}

				int lines_skipped = 0;
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					lines_skipped++;
				}
				pos.y += lines_skipped * line_height;
			}

			text_size.y += (pos - text_pos).y;
		}

		ImRect bb(text_pos, text_pos + text_size);
		ItemSize(bb);
		ItemAdd(bb, NULL);
	}
	else
	{
		const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
		const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

		ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);
		ImRect bb(text_pos, text_pos + text_size);
		ItemSize(text_size);
		if (!ItemAdd(bb, NULL))
			return;

		RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
	}
}

void ImGui::AlignFirstTextHeightToWidgets()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	ItemSize(ImVec2(0, g.FontSize + g.Style.FramePadding.y * 2), g.Style.FramePadding.y);
	SameLine(0, 0);
}

void ImGui::LabelTextV(const char* label, const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2));
	const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y * 2) + label_size);
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, NULL))
		return;

	const char* value_text_begin = &g.TempBuffer[0];
	const char* value_text_end = value_text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin, value_text_end, NULL, ImGuiAlign_VCenter);
	if (label_size.x > 0.0f)
		RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);
}

void ImGui::LabelText(const char* label, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	LabelTextV(label, fmt, args);
	va_end(args);
}

static inline bool IsWindowContentHoverable(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	if (ImGuiWindow* focused_window = g.FocusedWindow)
		if (ImGuiWindow* focused_root_window = focused_window->RootWindow)
			if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) != 0 && focused_root_window->WasActive && focused_root_window != window->RootWindow)
				return false;

	return true;
}

bool ImGui::ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	if (flags & ImGuiButtonFlags_Disabled)
	{
		if (out_hovered) *out_hovered = false;
		if (out_held) *out_held = false;
		if (g.ActiveId == id) SetActiveID(0);
		return false;
	}

	if ((flags & (ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick)) == 0)
		flags |= ImGuiButtonFlags_PressedOnClickRelease;

	bool pressed = false;
	bool hovered = IsHovered(bb, id, (flags & ImGuiButtonFlags_FlattenChilds) != 0);
	if (hovered)
	{
		SetHoveredID(id);
		if (!(flags & ImGuiButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
		{
			if ((flags & ImGuiButtonFlags_PressedOnClickRelease) && g.IO.MouseClicked[0])
			{
				SetActiveID(id, window);                 FocusWindow(window);
				g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
			}
			if (((flags & ImGuiButtonFlags_PressedOnClick) && g.IO.MouseClicked[0]) || ((flags & ImGuiButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[0]))
			{
				pressed = true;
				SetActiveID(0);
				FocusWindow(window);
			}
			if ((flags & ImGuiButtonFlags_PressedOnRelease) && g.IO.MouseReleased[0])
			{
				if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))                      pressed = true;
				SetActiveID(0);
			}

			if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id && g.IO.MouseDownDuration[0] > 0.0f && IsMouseClicked(0, true))
				pressed = true;
		}
	}

	bool held = false;
	if (g.ActiveId == id)
	{
		if (g.IO.MouseDown[0])
		{
			held = true;
		}
		else
		{
			if (hovered && (flags & ImGuiButtonFlags_PressedOnClickRelease))
				if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))                      pressed = true;
			SetActiveID(0);
		}
	}

	if (hovered && (flags & ImGuiButtonFlags_AllowOverlapMode) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
		hovered = pressed = held = false;

	if (out_hovered) *out_hovered = hovered;
	if (out_held) *out_held = held;

	return pressed;
}

bool ImGui::ButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, &id))
		return false;

	if (window->DC.ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, ImGuiAlign_Center | ImGuiAlign_VCenter);


	return pressed;
}

bool ImGui::Button(const char* label, const ImVec2& size_arg)
{
	return ButtonEx(label, size_arg, 0);
}

bool ImGui::SmallButton(const char* label)
{
	ImGuiContext& g = *GImGui;
	float backup_padding_y = g.Style.FramePadding.y;
	g.Style.FramePadding.y = 0.0f;
	bool pressed = ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_AlignTextBaseLine);
	g.Style.FramePadding.y = backup_padding_y;
	return pressed;
}

bool ImGui::InvisibleButton(const char* str_id, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(str_id);
	ImVec2 size = CalcItemSize(size_arg, 0.0f, 0.0f);
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	ItemSize(bb);
	if (!ItemAdd(bb, &id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	return pressed;
}

bool ImGui::CloseButton(ImGuiID id, const ImVec2& pos, float radius)
{
	ImGuiWindow* window = GetCurrentWindow();

	const ImRect bb(pos - ImVec2(radius, radius), pos + ImVec2(radius, radius));

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_CloseButtonActive : hovered ? ImGuiCol_CloseButtonHovered : ImGuiCol_CloseButton);
	const ImVec2 center = bb.GetCenter();
	window->DrawList->AddCircleFilled(center, ImMax(2.0f, radius), col, 12);

	const float cross_extent = (radius * 0.7071f) - 1.0f;
	if (hovered)
	{
		window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent), center + ImVec2(-cross_extent, -cross_extent), GetColorU32(ImGuiCol_Text));
		window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent), center + ImVec2(-cross_extent, +cross_extent), GetColorU32(ImGuiCol_Text));
	}

	return pressed;
}

void ImGui::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	if (border_col.w > 0.0f)
		bb.Max += ImVec2(2, 2);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return;

	if (border_col.w > 0.0f)
	{
		window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
		window->DrawList->AddImage(user_texture_id, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col));
	}
	else
	{
		window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
	}
}

bool ImGui::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	PushID((void *)user_texture_id);
	const ImGuiID id = window->GetID("#image");
	PopID();

	const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
	const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
	ItemSize(bb);
	if (!ItemAdd(bb, &id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	if (bg_col.w > 0.0f)
		window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
	window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

	return pressed;
}

void ImGui::LogToTTY(int max_depth)
{
	ImGuiContext& g = *GImGui;
	if (g.LogEnabled)
		return;
	ImGuiWindow* window = GetCurrentWindowRead();

	g.LogEnabled = true;
	g.LogFile = stdout;
	g.LogStartDepth = window->DC.TreeDepth;
	if (max_depth >= 0)
		g.LogAutoExpandMaxDepth = max_depth;
}

void ImGui::LogToFile(int max_depth, const char* filename)
{
	ImGuiContext& g = *GImGui;
	if (g.LogEnabled)
		return;
	ImGuiWindow* window = GetCurrentWindowRead();

	if (!filename)
	{
		filename = g.IO.LogFilename;
		if (!filename)
			return;
	}

	g.LogFile = fopen(filename, "ab");
	if (!g.LogFile)
	{
		IM_ASSERT(g.LogFile != NULL);         return;
	}
	g.LogEnabled = true;
	g.LogStartDepth = window->DC.TreeDepth;
	if (max_depth >= 0)
		g.LogAutoExpandMaxDepth = max_depth;
}

void ImGui::LogToClipboard(int max_depth)
{
	ImGuiContext& g = *GImGui;
	if (g.LogEnabled)
		return;
	ImGuiWindow* window = GetCurrentWindowRead();

	g.LogEnabled = true;
	g.LogFile = NULL;
	g.LogStartDepth = window->DC.TreeDepth;
	if (max_depth >= 0)
		g.LogAutoExpandMaxDepth = max_depth;
}

void ImGui::LogFinish()
{
	ImGuiContext& g = *GImGui;
	if (!g.LogEnabled)
		return;

	LogText(IM_NEWLINE);
	g.LogEnabled = false;
	if (g.LogFile != NULL)
	{
		if (g.LogFile == stdout)
			fflush(g.LogFile);
		else
			fclose(g.LogFile);
		g.LogFile = NULL;
	}
	if (g.LogClipboard->size() > 1)
	{
		if (g.IO.SetClipboardTextFn)
			g.IO.SetClipboardTextFn(g.LogClipboard->begin());
		g.LogClipboard->clear();
	}
}

void ImGui::LogButtons()
{
	ImGuiContext& g = *GImGui;

	PushID("LogButtons");
	const bool log_to_tty = Button("Log To TTY"); SameLine();
	const bool log_to_file = Button("Log To File"); SameLine();
	const bool log_to_clipboard = Button("Log To Clipboard"); SameLine();
	PushItemWidth(80.0f);
	PushAllowKeyboardFocus(false);
	SliderInt("Depth", &g.LogAutoExpandMaxDepth, 0, 9, NULL);
	PopAllowKeyboardFocus();
	PopItemWidth();
	PopID();

	if (log_to_tty)
		LogToTTY(g.LogAutoExpandMaxDepth);
	if (log_to_file)
		LogToFile(g.LogAutoExpandMaxDepth, g.IO.LogFilename);
	if (log_to_clipboard)
		LogToClipboard(g.LogAutoExpandMaxDepth);
}

bool ImGui::TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags)
{
	if (flags & ImGuiTreeNodeFlags_Leaf)
		return true;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiStorage* storage = window->DC.StateStorage;

	bool is_open;
	if (g.SetNextTreeNodeOpenCond != 0)
	{
		if (g.SetNextTreeNodeOpenCond & ImGuiSetCond_Always)
		{
			is_open = g.SetNextTreeNodeOpenVal;
			storage->SetInt(id, is_open);
		}
		else
		{
			const int stored_value = storage->GetInt(id, -1);
			if (stored_value == -1)
			{
				is_open = g.SetNextTreeNodeOpenVal;
				storage->SetInt(id, is_open);
			}
			else
			{
				is_open = stored_value != 0;
			}
		}
		g.SetNextTreeNodeOpenCond = 0;
	}
	else
	{
		is_open = storage->GetInt(id, (flags & ImGuiTreeNodeFlags_DefaultOpen) ? 1 : 0) != 0;
	}

	if (g.LogEnabled && !(flags & ImGuiTreeNodeFlags_NoAutoOpenOnLog) && window->DC.TreeDepth < g.LogAutoExpandMaxDepth)
		is_open = true;

	return is_open;
}

bool ImGui::TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
	const ImVec2 padding = display_frame ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

	if (!label_end)
		label_end = FindRenderedTextEnd(label);
	const ImVec2 label_size = CalcTextSize(label, label_end, false);

	const float text_base_offset_y = ImMax(0.0f, window->DC.CurrentLineTextBaseOffset - padding.y);     const float frame_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
	ImRect bb = ImRect(window->DC.CursorPos, ImVec2(window->Pos.x + GetContentRegionMax().x, window->DC.CursorPos.y + frame_height));
	if (display_frame)
	{
		bb.Min.x -= (float)(int)(window->WindowPadding.x*0.5f) - 1;
		bb.Max.x += (float)(int)(window->WindowPadding.x*0.5f) - 1;
	}

	const float text_offset_x = (g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2));       const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);       ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

	const ImRect interact_bb = display_frame ? bb : ImRect(bb.Min.x, bb.Min.y, bb.Min.x + text_width + style.ItemSpacing.x * 2, bb.Max.y);
	bool is_open = TreeNodeBehaviorIsOpen(id, flags);
	if (!ItemAdd(interact_bb, &id))
	{
		if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
			TreePushRawID(id);
		return is_open;
	}

	ImGuiButtonFlags button_flags = ImGuiButtonFlags_NoKeyModifiers | ((flags & ImGuiTreeNodeFlags_AllowOverlapMode) ? ImGuiButtonFlags_AllowOverlapMode : 0);
	if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
		button_flags |= ImGuiButtonFlags_PressedOnDoubleClick | ((flags & ImGuiTreeNodeFlags_OpenOnArrow) ? ImGuiButtonFlags_PressedOnClickRelease : 0);
	bool hovered, held, pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
	if (pressed && !(flags & ImGuiTreeNodeFlags_Leaf))
	{
		bool toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick));
		if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
			toggled |= IsMouseHoveringRect(interact_bb.Min, ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y));
		if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
			toggled |= g.IO.MouseDoubleClicked[0];
		if (toggled)
		{
			is_open = !is_open;
			window->DC.StateStorage->SetInt(id, is_open);
		}
	}
	if (flags & ImGuiTreeNodeFlags_AllowOverlapMode)
		SetItemAllowOverlap();

	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
	const ImVec2 text_pos = bb.Min + ImVec2(text_offset_x, padding.y + text_base_offset_y);
	if (display_frame)
	{
		RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
		RenderCollapseTriangle(bb.Min + padding + ImVec2(0.0f, text_base_offset_y), is_open, 1.0f, true);
		if (g.LogEnabled)
		{
			const char log_prefix[] = "\n##";
			const char log_suffix[] = "##";
			LogRenderedText(text_pos, log_prefix, log_prefix + 3);
			RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
			LogRenderedText(text_pos, log_suffix + 1, log_suffix + 3);
		}
		else
		{
			RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
		}
	}
	else
	{
		if (hovered || (flags & ImGuiTreeNodeFlags_Selected))
			RenderFrame(bb.Min, bb.Max, col, false);

		if (flags & ImGuiTreeNodeFlags_Bullet)
			RenderBullet(bb.Min + ImVec2(text_offset_x * 0.5f, g.FontSize*0.50f + text_base_offset_y));
		else if (!(flags & ImGuiTreeNodeFlags_Leaf))
			RenderCollapseTriangle(bb.Min + ImVec2(padding.x, g.FontSize*0.15f + text_base_offset_y), is_open, 0.70f, false);
		if (g.LogEnabled)
			LogRenderedText(text_pos, ">");
		RenderText(text_pos, label, label_end, false);
	}

	if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		TreePushRawID(id);
	return is_open;
}

bool ImGui::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	return TreeNodeBehavior(window->GetID(label), flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen, label);
}

bool ImGui::CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	if (p_open && !*p_open)
		return false;

	ImGuiID id = window->GetID(label);
	bool is_open = TreeNodeBehavior(id, flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen | (p_open ? ImGuiTreeNodeFlags_AllowOverlapMode : 0), label);
	if (p_open)
	{
		ImGuiContext& g = *GImGui;
		float button_sz = g.FontSize * 0.5f;
		if (CloseButton(window->GetID((void*)(intptr_t)(id + 1)), ImVec2(ImMin(window->DC.LastItemRect.Max.x, window->ClipRect.Max.x) - g.Style.FramePadding.x - button_sz, window->DC.LastItemRect.Min.y + g.Style.FramePadding.y + button_sz), button_sz))
			*p_open = false;
	}

	return is_open;
}

bool ImGui::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	return TreeNodeBehavior(window->GetID(label), flags, label, NULL);
}

bool ImGui::TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	return TreeNodeBehavior(window->GetID(str_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	return TreeNodeBehavior(window->GetID(ptr_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
	return TreeNodeExV(str_id, 0, fmt, args);
}

bool ImGui::TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
	return TreeNodeExV(ptr_id, 0, fmt, args);
}

bool ImGui::TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(str_id, flags, fmt, args);
	va_end(args);
	return is_open;
}

bool ImGui::TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
	va_end(args);
	return is_open;
}

bool ImGui::TreeNode(const char* str_id, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(str_id, 0, fmt, args);
	va_end(args);
	return is_open;
}

bool ImGui::TreeNode(const void* ptr_id, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
	va_end(args);
	return is_open;
}

bool ImGui::TreeNode(const char* label)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	return TreeNodeBehavior(window->GetID(label), 0, label, NULL);
}

void ImGui::TreeAdvanceToLabelPos()
{
	ImGuiContext& g = *GImGui;
	g.CurrentWindow->DC.CursorPos.x += GetTreeNodeToLabelSpacing();
}

float ImGui::GetTreeNodeToLabelSpacing()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + (g.Style.FramePadding.x * 2.0f);
}

void ImGui::SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond)
{
	ImGuiContext& g = *GImGui;
	g.SetNextTreeNodeOpenVal = is_open;
	g.SetNextTreeNodeOpenCond = cond ? cond : ImGuiSetCond_Always;
}

void ImGui::PushID(const char* str_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->IDStack.push_back(window->GetID(str_id));
}

void ImGui::PushID(const char* str_id_begin, const char* str_id_end)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->IDStack.push_back(window->GetID(str_id_begin, str_id_end));
}

void ImGui::PushID(const void* ptr_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PushID(int int_id)
{
	const void* ptr_id = (void*)(intptr_t)int_id;
	ImGuiWindow* window = GetCurrentWindow();
	window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PopID()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->IDStack.pop_back();
}

ImGuiID ImGui::GetID(const char* str_id)
{
	return GImGui->CurrentWindow->GetID(str_id);
}

ImGuiID ImGui::GetID(const char* str_id_begin, const char* str_id_end)
{
	return GImGui->CurrentWindow->GetID(str_id_begin, str_id_end);
}

ImGuiID ImGui::GetID(const void* ptr_id)
{
	return GImGui->CurrentWindow->GetID(ptr_id);
}

void ImGui::Bullet()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float line_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y * 2), g.FontSize);
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize, line_height));
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
	{
		SameLine(0, style.FramePadding.x * 2);
		return;
	}

	RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, line_height*0.5f));
	SameLine(0, style.FramePadding.x * 2);
}

void ImGui::BulletTextV(const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const char* text_begin = g.TempBuffer;
	const char* text_end = text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	const ImVec2 label_size = CalcTextSize(text_begin, text_end, false);
	const float text_base_offset_y = ImMax(0.0f, window->DC.CurrentLineTextBaseOffset);     const float line_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y * 2), g.FontSize);
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize + (label_size.x > 0.0f ? (label_size.x + style.FramePadding.x * 2) : 0.0f), ImMax(line_height, label_size.y)));      ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return;

	RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, line_height*0.5f));
	RenderText(bb.Min + ImVec2(g.FontSize + style.FramePadding.x * 2, text_base_offset_y), text_begin, text_end, false);
}

void ImGui::BulletText(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	BulletTextV(fmt, args);
	va_end(args);
}

static inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size)
{
	if (data_type == ImGuiDataType_Int)
		ImFormatString(buf, buf_size, display_format, *(int*)data_ptr);
	else if (data_type == ImGuiDataType_Float)
		ImFormatString(buf, buf_size, display_format, *(float*)data_ptr);
}

static inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size)
{
	if (data_type == ImGuiDataType_Int)
	{
		if (decimal_precision < 0)
			ImFormatString(buf, buf_size, "%d", *(int*)data_ptr);
		else
			ImFormatString(buf, buf_size, "%.*d", decimal_precision, *(int*)data_ptr);
	}
	else if (data_type == ImGuiDataType_Float)
	{
		if (decimal_precision < 0)
			ImFormatString(buf, buf_size, "%f", *(float*)data_ptr);             else
			ImFormatString(buf, buf_size, "%.*f", decimal_precision, *(float*)data_ptr);
	}
}

static void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2) {
	if (data_type == ImGuiDataType_Int)
	{
		if (op == ((char)(0x1ee1 + 1800 - 0x25be)))
			*(int*)value1 = *(int*)value1 + *(const int*)value2;
		else if (op == ((char)(0x5b8 + 580 - 0x7cf)))
			*(int*)value1 = *(int*)value1 - *(const int*)value2;
	}
	else if (data_type == ImGuiDataType_Float)
	{
		if (op == ((char)(0xf4d + 1880 - 0x167a)))
			*(float*)value1 = *(float*)value1 + *(const float*)value2;
		else if (op == ((char)(0x224c + 623 - 0x248e)))
			*(float*)value1 = *(float*)value1 - *(const float*)value2;
	}
}

static bool DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format)
{
	while (ImCharIsSpace(*buf))
		buf++;

	char op = buf[0];
	if (op == ((char)(0x13c6 + 3244 - 0x2047)) || op == ((char)(0x6cb + 260 - 0x7a5)) || op == ((char)(0x1520 + 3963 - 0x246c)))
	{
		buf++;
		while (ImCharIsSpace(*buf))
			buf++;
	}
	else
	{
		op = 0;
	}
	if (!buf[0])
		return false;

	if (data_type == ImGuiDataType_Int)
	{
		if (!scalar_format)
			scalar_format = "%d";
		int* v = (int*)data_ptr;
		const int old_v = *v;
		int arg0 = *v;
		if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1)
			return false;

		float arg1 = 0.0f;
		if (op == ((char)(0x17ad + 2366 - 0x20c0))) { if (sscanf(buf, "%f", &arg1) == 1) *v = (int)(arg0 + arg1); }
		else if (op == ((char)(0x955 + 5066 - 0x1cf5))) { if (sscanf(buf, "%f", &arg1) == 1) *v = (int)(arg0 * arg1); }
		else if (op == ((char)(0x7b9 + 7232 - 0x23ca))) { if (sscanf(buf, "%f", &arg1) == 1 && arg1 != 0.0f) *v = (int)(arg0 / arg1); }
		else { if (sscanf(buf, scalar_format, &arg0) == 1) *v = arg0; }                             return (old_v != *v);
	}
	else if (data_type == ImGuiDataType_Float)
	{
		scalar_format = "%f";
		float* v = (float*)data_ptr;
		const float old_v = *v;
		float arg0 = *v;
		if (op && sscanf(initial_value_buf, scalar_format, &arg0) < 1)
			return false;

		float arg1 = 0.0f;
		if (sscanf(buf, scalar_format, &arg1) < 1)
			return false;
		if (op == ((char)(0x12f1 + 393 - 0x144f))) { *v = arg0 + arg1; }
		else if (op == ((char)(0x10ed + 4896 - 0x23e3))) { *v = arg0 * arg1; }
		else if (op == ((char)(0xfe8 + 3758 - 0x1e67))) { if (arg1 != 0.0f) *v = arg0 / arg1; }
		else { *v = arg1; }                                   return (old_v != *v);
	}

	return false;
}

bool ImGui::InputScalarAsWidgetReplacement(const ImRect& aabb, const char* label, ImGuiDataType data_type, void* data_ptr, ImGuiID id, int decimal_precision)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	SetActiveID(g.ScalarAsInputTextId, window);
	SetHoveredID(0);
	FocusableItemUnregister(window);

	char buf[32];
	DataTypeFormatString(data_type, data_ptr, decimal_precision, buf, IM_ARRAYSIZE(buf));
	bool text_value_changed = InputTextEx(label, buf, IM_ARRAYSIZE(buf), aabb.GetSize(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll);
	if (g.ScalarAsInputTextId == 0)
	{
		IM_ASSERT(g.ActiveId == id);            g.ScalarAsInputTextId = g.ActiveId;
		SetHoveredID(id);
	}
	else if (g.ActiveId != g.ScalarAsInputTextId)
	{
		g.ScalarAsInputTextId = 0;
	}
	if (text_value_changed)
		return DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, NULL);
	return false;
}

int ImGui::ParseFormatPrecision(const char* fmt, int default_precision)
{
	int precision = default_precision;
	while ((fmt = strchr(fmt, ((char)(0xef7 + 2352 - 0x1802)))) != NULL)
	{
		fmt++;
		if (fmt[0] == ((char)(0xa9a + 2771 - 0x1548))) { fmt++; continue; }         while (*fmt >= ((char)(0x12b3 + 3206 - 0x1f09)) && *fmt <= ((char)(0x1034 + 4523 - 0x21a6)))
			fmt++;
		if (*fmt == ((char)(0x1d6 + 670 - 0x446)))
		{
			precision = atoi(fmt + 1);
			if (precision < 0 || precision > 10)
				precision = default_precision;
		}
		break;
	}
	return precision;
}

float ImGui::RoundScalar(float value, int decimal_precision)
{
	static const float min_steps[10] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f };
	float min_step = (decimal_precision >= 0 && decimal_precision < 10) ? min_steps[decimal_precision] : powf(10.0f, (float)-decimal_precision);
	bool negative = value < 0.0f;
	value = fabsf(value);
	float remainder = fmodf(value, min_step);
	if (remainder <= min_step*0.5f)
		value -= remainder;
	else
		value += (min_step - remainder);
	return negative ? -value : value;
}

bool ImGui::SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	const ImGuiStyle& style = g.Style;

	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	const bool is_non_linear = fabsf(power - 1.0f) > 0.0001f;
	const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

	const float grab_padding = 2.0f;
	const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
	float grab_sz;
	if (decimal_precision > 0)
		grab_sz = ImMin(style.GrabMinSize, slider_sz);
	else
		grab_sz = ImMin(ImMax(1.0f * (slider_sz / (v_max - v_min + 1.0f)), style.GrabMinSize), slider_sz);      const float slider_usable_sz = slider_sz - grab_sz;
	const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz*0.5f;
	const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz*0.5f;

	float linear_zero_pos = 0.0f;       if (v_min * v_max < 0.0f)
	{
		const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
		const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
		linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
	}
	else
	{
		linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
	}

	bool value_changed = false;
	if (g.ActiveId == id)
	{
		if (g.IO.MouseDown[0])
		{
			const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
			float normalized_pos = ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f);
			if (!is_horizontal)
				normalized_pos = 1.0f - normalized_pos;

			float new_value;
			if (is_non_linear)
			{
				if (normalized_pos < linear_zero_pos)
				{
					float a = 1.0f - (normalized_pos / linear_zero_pos);
					a = powf(a, power);
					new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
				}
				else
				{
					float a;
					if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
						a = (normalized_pos - linear_zero_pos) / (1.0f - linear_zero_pos);
					else
						a = normalized_pos;
					a = powf(a, power);
					new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
				}
			}
			else
			{
				new_value = ImLerp(v_min, v_max, normalized_pos);
			}

			new_value = RoundScalar(new_value, decimal_precision);
			if (*v != new_value)
			{
				*v = new_value;
				value_changed = true;
			}
		}
		else
		{
			SetActiveID(0);
		}
	}

	float grab_t;
	if (is_non_linear)
	{
		float v_clamped = ImClamp(*v, v_min, v_max);
		if (v_clamped < 0.0f)
		{
			const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
			grab_t = (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
		}
		else
		{
			const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
			grab_t = linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
		}
	}
	else
	{
		grab_t = (ImClamp(*v, v_min, v_max) - v_min) / (v_max - v_min);
	}

	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(ImVec2(grab_pos - grab_sz*0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz*0.5f, frame_bb.Max.y - grab_padding));
	else
		grab_bb = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz*0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz*0.5f));
	window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

	return value_changed;
}

bool ImGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w * .8, label_size.y + style.FramePadding.y*2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	if (!ItemAdd(total_bb, &id))
	{
		ItemSize(total_bb, style.FramePadding.y);
		return false;
	}

	const bool hovered = IsHovered(frame_bb, id);
	if (hovered)
		SetHoveredID(id);

	if (!display_format)
		display_format = "%.3f";
	int decimal_precision = ParseFormatPrecision(display_format, 3);

	bool start_text_input = false;
	const bool tab_focus_requested = FocusableItemRegister(window, g.ActiveId == id);
	if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
	{
		SetActiveID(id, window);
		FocusWindow(window);

		if (tab_focus_requested || g.IO.KeyCtrl)
		{
			start_text_input = true;
			g.ScalarAsInputTextId = 0;
		}
	}
	if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
		return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);

	ItemSize(total_bb, style.FramePadding.y);

	const bool value_changed = SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision);

	char value_buf[64];
	const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
	RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImGuiAlign_Center | ImGuiAlign_VCenter);

	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

bool ImGui::VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(frame_bb, &id))
		return false;

	const bool hovered = IsHovered(frame_bb, id);
	if (hovered)
		SetHoveredID(id);

	if (!display_format)
		display_format = "%.3f";
	int decimal_precision = ParseFormatPrecision(display_format, 3);

	if (hovered && g.IO.MouseClicked[0])
	{
		SetActiveID(id, window);
		FocusWindow(window);
	}

	bool value_changed = SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision, ImGuiSliderFlags_Vertical);

	char value_buf[64];
	char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
	RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, NULL, ImGuiAlign_Center);
	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

bool ImGui::SliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max)
{
	float v_deg = (*v_rad) * 360.0f / (2 * IM_PI);
	bool value_changed = SliderFloat(label, &v_deg, v_degrees_min, v_degrees_max, "%.0f deg", 1.0f);
	*v_rad = v_deg * (2 * IM_PI) / 360.0f;
	return value_changed;
}

bool ImGui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format)
{
	if (!display_format)
		display_format = "%.0f";
	float v_f = (float)*v;
	bool value_changed = SliderFloat(label, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
	*v = (int)v_f;
	return value_changed;
}

bool ImGui::VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format)
{
	if (!display_format)
		display_format = "%.0f";
	float v_f = (float)*v;
	bool value_changed = VSliderFloat(label, size, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
	*v = (int)v_f;
	return value_changed;
}

bool ImGui::SliderFloatN(const char* label, float* v, int components, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= SliderFloat("##v", &v[i], v_min, v_max, display_format, power);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* display_format, float power)
{
	return SliderFloatN(label, v, 2, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* display_format, float power)
{
	return SliderFloatN(label, v, 3, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* display_format, float power)
{
	return SliderFloatN(label, v, 4, v_min, v_max, display_format, power);
}

bool ImGui::SliderIntN(const char* label, int* v, int components, int v_min, int v_max, const char* display_format)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= SliderInt("##v", &v[i], v_min, v_max, display_format);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* display_format)
{
	return SliderIntN(label, v, 2, v_min, v_max, display_format);
}

bool ImGui::SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* display_format)
{
	return SliderIntN(label, v, 3, v_min, v_max, display_format);
}

bool ImGui::SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* display_format)
{
	return SliderIntN(label, v, 4, v_min, v_max, display_format);
}

bool ImGui::DragBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_speed, float v_min, float v_max, int decimal_precision, float power)
{
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

	bool value_changed = false;

	if (g.ActiveId == id)
	{
		if (g.IO.MouseDown[0])
		{
			if (g.ActiveIdIsJustActivated)
			{
				g.DragCurrentValue = *v;
				g.DragLastMouseDelta = ImVec2(0.f, 0.f);
			}

			float v_cur = g.DragCurrentValue;
			const ImVec2 mouse_drag_delta = GetMouseDragDelta(0, 1.0f);
			if (fabsf(mouse_drag_delta.x - g.DragLastMouseDelta.x) > 0.0f)
			{
				float speed = v_speed;
				if (speed == 0.0f && (v_max - v_min) != 0.0f && (v_max - v_min) < FLT_MAX)
					speed = (v_max - v_min) * g.DragSpeedDefaultRatio;
				if (g.IO.KeyShift && g.DragSpeedScaleFast >= 0.0f)
					speed = speed * g.DragSpeedScaleFast;
				if (g.IO.KeyAlt && g.DragSpeedScaleSlow >= 0.0f)
					speed = speed * g.DragSpeedScaleSlow;

				float delta = (mouse_drag_delta.x - g.DragLastMouseDelta.x) * speed;
				if (fabsf(power - 1.0f) > 0.001f)
				{
					float v0_abs = v_cur >= 0.0f ? v_cur : -v_cur;
					float v0_sign = v_cur >= 0.0f ? 1.0f : -1.0f;
					float v1 = powf(v0_abs, 1.0f / power) + (delta * v0_sign);
					float v1_abs = v1 >= 0.0f ? v1 : -v1;
					float v1_sign = v1 >= 0.0f ? 1.0f : -1.0f;                              v_cur = powf(v1_abs, power) * v0_sign * v1_sign;
				}
				else
				{
					v_cur += delta;
				}
				g.DragLastMouseDelta.x = mouse_drag_delta.x;

				if (v_min < v_max)
					v_cur = ImClamp(v_cur, v_min, v_max);
				g.DragCurrentValue = v_cur;
			}

			v_cur = RoundScalar(v_cur, decimal_precision);
			if (*v != v_cur)
			{
				*v = v_cur;
				value_changed = true;
			}
		}
		else
		{
			SetActiveID(0);
		}
	}

	return value_changed;
}

bool ImGui::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
	const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	if (!ItemAdd(total_bb, &id))
	{
		ItemSize(total_bb, style.FramePadding.y);
		return false;
	}

	const bool hovered = IsHovered(frame_bb, id);
	if (hovered)
		SetHoveredID(id);

	if (!display_format)
		display_format = "%.3f";
	int decimal_precision = ParseFormatPrecision(display_format, 3);

	bool start_text_input = false;
	const bool tab_focus_requested = FocusableItemRegister(window, g.ActiveId == id);
	if (tab_focus_requested || (hovered && (g.IO.MouseClicked[0] | g.IO.MouseDoubleClicked[0])))
	{
		SetActiveID(id, window);
		FocusWindow(window);

		if (tab_focus_requested || g.IO.KeyCtrl || g.IO.MouseDoubleClicked[0])
		{
			start_text_input = true;
			g.ScalarAsInputTextId = 0;
		}
	}
	if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
		return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);

	ItemSize(total_bb, style.FramePadding.y);
	const bool value_changed = DragBehavior(frame_bb, id, v, v_speed, v_min, v_max, decimal_precision, power);

	char value_buf[64];
	const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
	RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImGuiAlign_Center | ImGuiAlign_VCenter);

	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);

	return value_changed;
}

bool ImGui::DragFloatN(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
	return DragFloatN(label, v, 2, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
	return DragFloatN(label, v, 3, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
	return DragFloatN(label, v, 4, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min, float v_max, const char* display_format, const char* display_format_max, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	PushID(label);
	BeginGroup();
	PushMultiItemsWidths(2);

	bool value_changed = DragFloat("##min", v_current_min, v_speed, (v_min >= v_max) ? -FLT_MAX : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), display_format, power);
	PopItemWidth();
	SameLine(0, g.Style.ItemInnerSpacing.x);
	value_changed |= DragFloat("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? FLT_MAX : v_max, display_format_max ? display_format_max : display_format, power);
	PopItemWidth();
	SameLine(0, g.Style.ItemInnerSpacing.x);

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();
	PopID();

	return value_changed;
}

bool ImGui::DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* display_format)
{
	if (!display_format)
		display_format = "%.0f";
	float v_f = (float)*v;
	bool value_changed = DragFloat(label, &v_f, v_speed, (float)v_min, (float)v_max, display_format);
	*v = (int)v_f;
	return value_changed;
}

bool ImGui::DragIntN(const char* label, int* v, int components, float v_speed, int v_min, int v_max, const char* display_format)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= DragInt("##v", &v[i], v_speed, v_min, v_max, display_format);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* display_format)
{
	return DragIntN(label, v, 2, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* display_format)
{
	return DragIntN(label, v, 3, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* display_format)
{
	return DragIntN(label, v, 4, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min, int v_max, const char* display_format, const char* display_format_max)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	PushID(label);
	BeginGroup();
	PushMultiItemsWidths(2);

	bool value_changed = DragInt("##min", v_current_min, v_speed, (v_min >= v_max) ? INT_MIN : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), display_format);
	PopItemWidth();
	SameLine(0, g.Style.ItemInnerSpacing.x);
	value_changed |= DragInt("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? INT_MAX : v_max, display_format_max ? display_format_max : display_format);
	PopItemWidth();
	SameLine(0, g.Style.ItemInnerSpacing.x);

	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();
	PopID();

	return value_changed;
}

void ImGui::PlotEx(ImGuiPlotType plot_type, const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	if (graph_size.x == 0.0f)
		graph_size.x = CalcItemWidth();
	if (graph_size.y == 0.0f)
		graph_size.y = label_size.y + (style.FramePadding.y * 2);

	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
	const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, NULL))
		return;

	if (scale_min == FLT_MAX || scale_max == FLT_MAX)
	{
		float v_min = FLT_MAX;
		float v_max = -FLT_MAX;
		for (int i = 0; i < values_count; i++)
		{
			const float v = values_getter(data, i);
			v_min = ImMin(v_min, v);
			v_max = ImMax(v_max, v);
		}
		if (scale_min == FLT_MAX)
			scale_min = v_min;
		if (scale_max == FLT_MAX)
			scale_max = v_max;
	}

	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
	int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

	int v_hovered = -1;
	if (IsHovered(inner_bb, 0))
	{
		const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
		const int v_idx = (int)(t * item_count);
		IM_ASSERT(v_idx >= 0 && v_idx < values_count);

		const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
		const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
		if (plot_type == ImGuiPlotType_Lines)
			SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx + 1, v1);
		else if (plot_type == ImGuiPlotType_Histogram)
			SetTooltip("%d: %8.4g", v_idx, v0);
		v_hovered = v_idx;
	}

	const float t_step = 1.0f / (float)res_w;

	float v0 = values_getter(data, (0 + values_offset) % values_count);
	float t0 = 0.0f;
	ImVec2 tp0 = ImVec2(t0, 1.0f - ImSaturate((v0 - scale_min) / (scale_max - scale_min)));
	const ImU32 col_base = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
	const ImU32 col_hovered = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

	for (int n = 0; n < res_w; n++)
	{
		const float t1 = t0 + t_step;
		const int v1_idx = (int)(t0 * item_count + 0.5f);
		IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
		const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
		const ImVec2 tp1 = ImVec2(t1, 1.0f - ImSaturate((v1 - scale_min) / (scale_max - scale_min)));

		ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
		ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, 1.0f));
		if (plot_type == ImGuiPlotType_Lines)
		{
			window->DrawList->AddLine(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
		}
		else if (plot_type == ImGuiPlotType_Histogram)
		{
			if (pos1.x >= pos0.x + 2.0f)
				pos1.x -= 1.0f;
			window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
		}

		t0 = t1;
		tp0 = tp1;
	}

	if (overlay_text)
		RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImGuiAlign_Center);

	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData
{
	const float* Values;
	int Stride;

	ImGuiPlotArrayGetterData(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter(void* data, int idx)
{
	ImGuiPlotArrayGetterData* plot_data = (ImGuiPlotArrayGetterData*)data;
	const float v = *(float*)(void*)((unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
	return v;
}

void ImGui::PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
	ImGuiPlotArrayGetterData data(values, stride);
	PlotEx(ImGuiPlotType_Lines, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
	PlotEx(ImGuiPlotType_Lines, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotHistogram(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
	ImGuiPlotArrayGetterData data(values, stride);
	PlotEx(ImGuiPlotType_Histogram, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
	PlotEx(ImGuiPlotType_Histogram, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::ProgressBar(float fraction, const ImVec2& size_arg, const char* overlay)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	ImVec2 pos = window->DC.CursorPos;
	ImRect bb(pos, pos + CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y*2.0f));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, NULL))
		return;

	fraction = ImSaturate(fraction);
	RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	bb.Reduce(ImVec2(window->BorderSize, window->BorderSize));
	const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
	RenderFrame(bb.Min, fill_br, GetColorU32(ImGuiCol_PlotHistogram), false, style.FrameRounding);

	char overlay_buf[32];
	if (!overlay)
	{
		ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
		overlay = overlay_buf;
	}

	ImVec2 overlay_size = CalcTextSize(overlay, NULL);
	if (overlay_size.x > 0.0f)
		RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImGuiAlign_Left | ImGuiAlign_VCenter, &bb.Min, &bb.Max);
}

bool ImGui::Checkbox(const char* label, bool* v)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
	ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ItemAdd(total_bb, &id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
		*v = !(*v);

	RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
	if (*v)
	{
		const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
		const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
		window->DrawList->AddRectFilled(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad), GetColorU32(ImGuiCol_CheckMark), style.FrameRounding);
	}

	if (g.LogEnabled)
		LogRenderedText(text_bb.GetTL(), *v ? "[x]" : "[ ]");
	if (label_size.x > 0.0f)
		RenderText(text_bb.GetTL(), label);

	return pressed;
}

bool ImGui::CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
	bool v = ((*flags & flags_value) == flags_value);
	bool pressed = Checkbox(label, &v);
	if (pressed)
	{
		if (v)
			*flags |= flags_value;
		else
			*flags &= ~flags_value;
	}

	return pressed;
}

bool ImGui::RadioButton(const char* label, bool active)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2 - 1, label_size.y + style.FramePadding.y * 2 - 1));
	ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb.Add(text_bb);
	}

	if (!ItemAdd(total_bb, &id))
		return false;

	ImVec2 center = check_bb.GetCenter();
	center.x = (float)(int)center.x + 0.5f;
	center.y = (float)(int)center.y + 0.5f;
	const float radius = check_bb.GetHeight() * 0.5f;

	bool hovered, held;
	bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

	window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
	if (active)
	{
		const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
		const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
		window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(ImGuiCol_CheckMark), 16);
	}

	if (window->Flags & ImGuiWindowFlags_ShowBorders)
	{
		window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(ImGuiCol_BorderShadow), 16);
		window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 16);
	}

	if (g.LogEnabled)
		LogRenderedText(text_bb.GetTL(), active ? "(x)" : "( )");
	if (label_size.x > 0.0f)
		RenderText(text_bb.GetTL(), label);

	return pressed;
}

bool ImGui::RadioButton(const char* label, int* v, int v_button)
{
	const bool pressed = RadioButton(label, *v == v_button);
	if (pressed)
	{
		*v = v_button;
	}
	return pressed;
}

static int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
	int line_count = 0;
	const char* s = text_begin;
	while (char c = *s++)         if (c == '\n')
		line_count++;
	s--;
	if (s[0] != '\n' && s[0] != '\r')
		line_count++;
	*out_text_end = s;
	return line_count;
}

static ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
	ImFont* font = GImGui->Font;
	const float line_height = GImGui->FontSize;
	const float scale = line_height / font->FontSize;

	ImVec2 text_size = ImVec2(0, 0);
	float line_width = 0.0f;

	const ImWchar* s = text_begin;
	while (s < text_end)
	{
		unsigned int c = (unsigned int)(*s++);
		if (c == '\n')
		{
			text_size.x = ImMax(text_size.x, line_width);
			text_size.y += line_height;
			line_width = 0.0f;
			if (stop_on_new_line)
				break;
			continue;
		}
		if (c == '\r')
			continue;

		const float char_width = font->GetCharAdvance((unsigned short)c) * scale;
		line_width += char_width;
	}

	if (text_size.x < line_width)
		text_size.x = line_width;

	if (out_offset)
		*out_offset = ImVec2(line_width, text_size.y + line_height);
	if (line_width > 0 || text_size.y == 0.0f)                                text_size.y += line_height;

	if (remaining)
		*remaining = s;

	return text_size;
}

namespace ImGuiStb
{

	static int     STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING* obj) { return obj->CurLenW; }
	static ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING* obj, int idx) { return obj->Text[idx]; }
	static float   STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING* obj, int line_start_idx, int char_idx) { ImWchar c = obj->Text[line_start_idx + char_idx]; if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE; return GImGui->Font->GetCharAdvance(c) * (GImGui->FontSize / GImGui->Font->FontSize); }
	static int     STB_TEXTEDIT_KEYTOTEXT(int key) { return key >= 0x10000 ? 0 : key; }
	static ImWchar STB_TEXTEDIT_NEWLINE = '\n';
	static void    STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, STB_TEXTEDIT_STRING* obj, int line_start_idx)
	{
		const ImWchar* text = obj->Text.Data;
		const ImWchar* text_remaining = NULL;
		const ImVec2 size = InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
		r->x0 = 0.0f;
		r->x1 = size.x;
		r->baseline_y_delta = size.y;
		r->ymin = 0.0f;
		r->ymax = size.y;
		r->num_chars = (int)(text_remaining - (text + line_start_idx));
	}

	static bool is_separator(unsigned int c) { return ImCharIsSpace(c) || c == ((char)(0x1538 + 3675 - 0x2367)) || c == ((char)(0x103 + 5748 - 0x173c)) || c == ((char)(0x142b + 3411 - 0x2156)) || c == ((char)(0x16c9 + 1273 - 0x1b99)) || c == ((char)(0x17c8 + 436 - 0x1901)) || c == ((char)(0x772 + 6230 - 0x1f4b)) || c == ((char)(0x11a0 + 5479 - 0x26ac)) || c == ((char)(0x1662 + 3875 - 0x2528)) || c == ((char)(0x13f0 + 1010 - 0x1766)); }
	static int  is_word_boundary_from_right(STB_TEXTEDIT_STRING* obj, int idx) { return idx > 0 ? (is_separator(obj->Text[idx - 1]) && !is_separator(obj->Text[idx])) : 1; }
	static int  STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING* obj, int idx) { while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; }
#ifdef __APPLE__    
	static int  is_word_boundary_from_left(STB_TEXTEDIT_STRING* obj, int idx) { return idx > 0 ? (!is_separator(obj->Text[idx - 1]) && is_separator(obj->Text[idx])) : 1; }
	static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx) { int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++; return idx > len ? len : idx; }
#else
	static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx) { int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; }
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT   STB_TEXTEDIT_MOVEWORDLEFT_IMPL    
#define STB_TEXTEDIT_MOVEWORDRIGHT  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

	static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
	{
		ImWchar* dst = obj->Text.Data + pos;

		obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
		obj->CurLenW -= n;

		const ImWchar* src = obj->Text.Data + pos + n;
		while (ImWchar c = *src++)
			*dst++ = c;
		*dst = '\0';
	}

	static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
	{
		const int text_len = obj->CurLenW;
		IM_ASSERT(pos <= text_len);
		if (new_text_len + text_len + 1 > obj->Text.Size)
			return false;

		const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
		if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA)
			return false;

		ImWchar* text = obj->Text.Data;
		if (pos != text_len)
			memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
		memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

		obj->CurLenW += new_text_len;
		obj->CurLenA += new_text_len_utf8;
		obj->Text[obj->CurLenW] = '\0';

		return true;
	}

#define STB_TEXTEDIT_K_LEFT         0x10000 
#define STB_TEXTEDIT_K_RIGHT        0x10001 
#define STB_TEXTEDIT_K_UP           0x10002 
#define STB_TEXTEDIT_K_DOWN         0x10003 
#define STB_TEXTEDIT_K_LINESTART    0x10004 
#define STB_TEXTEDIT_K_LINEEND      0x10005 
#define STB_TEXTEDIT_K_TEXTSTART    0x10006 
#define STB_TEXTEDIT_K_TEXTEND      0x10007 
#define STB_TEXTEDIT_K_DELETE       0x10008 
#define STB_TEXTEDIT_K_BACKSPACE    0x10009 
#define STB_TEXTEDIT_K_UNDO         0x1000a 
#define STB_TEXTEDIT_K_REDO         0x1000b 
#define STB_TEXTEDIT_K_WORDLEFT     0x1000c 
#define STB_TEXTEDIT_K_WORDRIGHT    0x1000d 
#define STB_TEXTEDIT_K_SHIFT        0x20000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"

}

void ImGuiTextEditState::OnKeyPressed(int key)
{
	stb_textedit_key(this, &StbState, key);
	CursorFollow = true;
	CursorAnimReset();
}

void ImGuiTextEditCallbackData::DeleteChars(int pos, int bytes_count)
{
	IM_ASSERT(pos + bytes_count <= BufTextLen);
	char* dst = Buf + pos;
	const char* src = Buf + pos + bytes_count;
	while (char c = *src++)
		*dst++ = c;
	*dst = '\0';

	if (CursorPos + bytes_count >= pos)
		CursorPos -= bytes_count;
	else if (CursorPos >= pos)
		CursorPos = pos;
	SelectionStart = SelectionEnd = CursorPos;
	BufDirty = true;
	BufTextLen -= bytes_count;
}

void ImGuiTextEditCallbackData::InsertChars(int pos, const char* new_text, const char* new_text_end)
{
	const int new_text_len = new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
	if (new_text_len + BufTextLen + 1 >= BufSize)
		return;

	if (BufTextLen != pos)
		memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
	memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
	Buf[BufTextLen + new_text_len] = '\0';

	if (CursorPos >= pos)
		CursorPos += new_text_len;
	SelectionStart = SelectionEnd = CursorPos;
	BufDirty = true;
	BufTextLen += new_text_len;
}

static bool InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	unsigned int c = *p_char;

	if (c < 128 && c != ((char)(0x1000 + 123 - 0x105b)) && !isprint((int)(c & 0xff)))
	{
		bool pass = false;
		pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline));
		pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
		if (!pass)
			return false;
	}

	if (c >= 0xe000 && c <= 0xf8ff)         return false;

	if (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank))
	{
		if (flags & ImGuiInputTextFlags_CharsDecimal)
			if (!(c >= ((char)(0x11fa + 4133 - 0x21ef)) && c <= ((char)(0x967 + 6895 - 0x241d))) && (c != ((char)(0x6d2 + 7837 - 0x2541))) && (c != ((char)(0x24f + 1006 - 0x610))) && (c != ((char)(0xf33 + 4757 - 0x219d))) && (c != ((char)(0x2f7 + 503 - 0x4c4))) && (c != ((char)(0x2289 + 855 - 0x25b1))))
				return false;

		if (flags & ImGuiInputTextFlags_CharsHexadecimal)
			if (!(c >= ((char)(0xf52 + 1182 - 0x13c0)) && c <= ((char)(0x1534 + 3105 - 0x211c))) && !(c >= ((char)(0x144a + 3090 - 0x1ffb)) && c <= ((char)(0xb48 + 1403 - 0x105d))) && !(c >= ((char)(0x2334 + 125 - 0x2370)) && c <= ((char)(0x864 + 4514 - 0x19c0))))
				return false;

		if (flags & ImGuiInputTextFlags_CharsUppercase)
			if (c >= ((char)(0x561 + 1019 - 0x8fb)) && c <= ((char)(0x699 + 5629 - 0x1c1c)))
				*p_char = (c += (unsigned int)(((char)(0xf11 + 3407 - 0x1c1f)) - ((char)(0x612 + 6612 - 0x1f85))));

		if (flags & ImGuiInputTextFlags_CharsNoBlank)
			if (ImCharIsSpace(c))
				return false;
	}

	if (flags & ImGuiInputTextFlags_CallbackCharFilter)
	{
		ImGuiTextEditCallbackData callback_data;
		memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
		callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
		callback_data.EventChar = (ImWchar)c;
		callback_data.Flags = flags;
		callback_data.UserData = user_data;
		if (callback(&callback_data) != 0)
			return false;
		*p_char = callback_data.EventChar;
		if (!callback_data.EventChar)
			return false;
	}

	return true;
}

bool ImGui::InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline)));     IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput)));
	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
	const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
	const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? GetTextLineHeight() * 8.0f : label_size.y) + style.FramePadding.y*2.0f);     const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

	ImGuiWindow* draw_window = window;
	if (is_multiline)
	{
		BeginGroup();
		if (!BeginChildFrame(id, frame_bb.GetSize()))
		{
			EndChildFrame();
			EndGroup();
			return false;
		}
		draw_window = GetCurrentWindow();
		size.x -= draw_window->ScrollbarSizes.x;
	}
	else
	{
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, &id))
			return false;
	}

	if (is_password)
	{
		const ImFont::Glyph* glyph = g.Font->FindGlyph(((char)(0x57a + 5237 - 0x19c5)));
		ImFont* password_font = &g.InputTextPasswordFont;
		password_font->FontSize = g.Font->FontSize;
		password_font->Scale = g.Font->Scale;
		password_font->DisplayOffset = g.Font->DisplayOffset;
		password_font->Ascent = g.Font->Ascent;
		password_font->Descent = g.Font->Descent;
		password_font->ContainerAtlas = g.Font->ContainerAtlas;
		password_font->FallbackGlyph = glyph;
		password_font->FallbackXAdvance = glyph->XAdvance;
		IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexXAdvance.empty() && password_font->IndexLookup.empty());
		PushFont(password_font);
	}

	ImGuiTextEditState& edit_state = g.InputTextState;

	const bool focus_requested = FocusableItemRegister(window, g.ActiveId == id, (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) == 0);        const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = IsHovered(frame_bb, id);
	if (hovered)
	{
		SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}
	const bool user_clicked = hovered && io.MouseClicked[0];
	const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");

	bool select_all = (g.ActiveId != id) && (flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
	if (focus_requested || user_clicked || user_scrolled)
	{
		if (g.ActiveId != id)
		{
			const int prev_len_w = edit_state.CurLenW;
			edit_state.Text.resize(buf_size + 1);                    edit_state.InitialText.resize(buf_size + 1);             ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);             edit_state.CursorAnimReset();

			const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
			if (recycle_state)
			{
				edit_state.CursorClamp();
			}
			else
			{
				edit_state.Id = id;
				edit_state.ScrollX = 0.0f;
				stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
				if (!is_multiline && focus_requested_by_code)
					select_all = true;
			}
			if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
				edit_state.StbState.insert_mode = true;
			if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
				select_all = true;
		}
		SetActiveID(id, window);
		FocusWindow(window);
	}
	else if (io.MouseClicked[0])
	{
		if (g.ActiveId == id)
			SetActiveID(0);
	}

	bool value_changed = false;
	bool enter_pressed = false;

	if (g.ActiveId == id)
	{
		if (!is_editable && !g.ActiveIdIsJustActivated)
		{
			edit_state.Text.resize(buf_size + 1);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);
			edit_state.CursorClamp();
		}

		edit_state.BufSizeA = buf_size;

		g.ActiveIdAllowOverlap = !io.MouseDown[0];

		const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
		const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize*0.5f));

		const bool osx_double_click_selects_words = io.OSXBehaviors;              if (select_all || (hovered && !osx_double_click_selects_words && io.MouseDoubleClicked[0]))
		{
			edit_state.SelectAll();
			edit_state.SelectedAllMouseLock = true;
		}
		else if (hovered && osx_double_click_selects_words && io.MouseDoubleClicked[0])
		{
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
		}
		else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
		{
			stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
		}
		else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
		{
			stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
			edit_state.CursorFollow = true;
		}
		if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
			edit_state.SelectedAllMouseLock = false;

		if (io.InputCharacters[0])
		{
			if (!(io.KeyCtrl && !io.KeyAlt) && is_editable)
			{
				for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
					if (unsigned int c = (unsigned int)io.InputCharacters[n])
					{
						if (!InputTextFilterCharacter(&c, flags, callback, user_data))
							continue;
						edit_state.OnKeyPressed((int)c);
					}
			}

			memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
		}

		bool cancel_edit = false;
		const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
		const bool is_shortcut_key_only = (io.OSXBehaviors ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt && !io.KeyShift;         const bool is_wordmove_key_down = io.OSXBehaviors ? io.KeyAlt : io.KeyCtrl;                             const bool is_startend_key_down = io.OSXBehaviors && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;
		if (IsKeyPressedMap(ImGuiKey_LeftArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_RightArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline) { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline) { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Home)) { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_End)) { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
		{
			if (!edit_state.HasSelection())
			{
				if (is_wordmove_key_down) edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
				else if (io.OSXBehaviors && io.KeySuper && !io.KeyAlt && !io.KeyCtrl) edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
			}
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Enter))
		{
			bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
			if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
			{
				SetActiveID(0);
				enter_pressed = true;
			}
			else if (is_editable)
			{
				unsigned int c = '\n';                 if (InputTextFilterCharacter(&c, flags, callback, user_data))
					edit_state.OnKeyPressed((int)c);
			}
		}
		else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift && !io.KeyAlt && is_editable)
		{
			unsigned int c = '\t';             if (InputTextFilterCharacter(&c, flags, callback, user_data))
				edit_state.OnKeyPressed((int)c);
		}
		else if (IsKeyPressedMap(ImGuiKey_Escape)) { SetActiveID(0); cancel_edit = true; }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Z) && is_editable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO); edit_state.ClearSelection(); }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Y) && is_editable) { edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO); edit_state.ClearSelection(); }
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_A)) { edit_state.SelectAll(); edit_state.CursorFollow = true; }
		else if (is_shortcut_key_only && !is_password && ((IsKeyPressedMap(ImGuiKey_X) && is_editable) || IsKeyPressedMap(ImGuiKey_C)) && (!is_multiline || edit_state.HasSelection()))
		{
			const bool cut = IsKeyPressedMap(ImGuiKey_X);
			if (cut && !edit_state.HasSelection())
				edit_state.SelectAll();

			if (io.SetClipboardTextFn)
			{
				const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
				const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
				edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data + ib, edit_state.Text.Data + ie);
				io.SetClipboardTextFn(edit_state.TempTextBuffer.Data);
			}

			if (cut)
			{
				edit_state.CursorFollow = true;
				stb_textedit_cut(&edit_state, &edit_state.StbState);
			}
		}
		else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_V) && is_editable)
		{
			if (const char* clipboard = io.GetClipboardTextFn ? io.GetClipboardTextFn() : NULL)
			{
				const int clipboard_len = (int)strlen(clipboard);
				ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
				int clipboard_filtered_len = 0;
				for (const char* s = clipboard; *s; )
				{
					unsigned int c;
					s += ImTextCharFromUtf8(&c, s, NULL);
					if (c == 0)
						break;
					if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data))
						continue;
					clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
				}
				clipboard_filtered[clipboard_filtered_len] = 0;
				if (clipboard_filtered_len > 0) {
					stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
					edit_state.CursorFollow = true;
				}
				ImGui::MemFree(clipboard_filtered);
			}
		}

		if (cancel_edit)
		{
			if (is_editable)
			{
				ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
				value_changed = true;
			}
		}
		else
		{
			if (is_editable)
			{
				edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
			}

			if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
			{
				IM_ASSERT(callback != NULL);

				ImGuiInputTextFlags event_flag = 0;
				ImGuiKey event_key = ImGuiKey_COUNT;
				if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
				{
					event_flag = ImGuiInputTextFlags_CallbackCompletion;
					event_key = ImGuiKey_Tab;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_UpArrow;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_DownArrow;
				}
				else if (flags & ImGuiInputTextFlags_CallbackAlways)
					event_flag = ImGuiInputTextFlags_CallbackAlways;

				if (event_flag)
				{
					ImGuiTextEditCallbackData callback_data;
					memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
					callback_data.EventFlag = event_flag;
					callback_data.Flags = flags;
					callback_data.UserData = user_data;
					callback_data.ReadOnly = !is_editable;

					callback_data.EventKey = event_key;
					callback_data.Buf = edit_state.TempTextBuffer.Data;
					callback_data.BufTextLen = edit_state.CurLenA;
					callback_data.BufSize = edit_state.BufSizeA;
					callback_data.BufDirty = false;

					ImWchar* text = edit_state.Text.Data;
					const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
					const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
					const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

					callback(&callback_data);

					IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data);                      IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
					IM_ASSERT(callback_data.Flags == flags);
					if (callback_data.CursorPos != utf8_cursor_pos)            edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
					if (callback_data.SelectionStart != utf8_selection_start)  edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
					if (callback_data.SelectionEnd != utf8_selection_end)      edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
					if (callback_data.BufDirty)
					{
						IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf));                         edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
						edit_state.CurLenA = callback_data.BufTextLen;                          edit_state.CursorAnimReset();
					}
				}
			}

			if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
			{
				ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
				value_changed = true;
			}
		}
	}

	const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf; buf = NULL;

	if (!is_multiline)
		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y);     ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
	ImVec2 text_size(0.f, 0.f);
	const bool is_currently_scrolling = (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
	if (g.ActiveId == id || is_currently_scrolling)
	{
		edit_state.CursorAnim += io.DeltaTime;

		const ImWchar* text_begin = edit_state.Text.Data;
		ImVec2 cursor_offset, select_start_offset;

		{
			const ImWchar* searches_input_ptr[2];
			searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
			searches_input_ptr[1] = NULL;
			int searches_remaining = 1;
			int searches_result_line_number[2] = { -1, -999 };
			if (edit_state.StbState.select_start != edit_state.StbState.select_end)
			{
				searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
				searches_result_line_number[1] = -1;
				searches_remaining++;
			}

			searches_remaining += is_multiline ? 1 : 0;
			int line_count = 0;
			for (const ImWchar* s = text_begin; *s != 0; s++)
				if (*s == '\n')
				{
					line_count++;
					if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_number[0] = line_count; if (--searches_remaining <= 0) break; }
					if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_number[1] = line_count; if (--searches_remaining <= 0) break; }
				}
			line_count++;
			if (searches_result_line_number[0] == -1) searches_result_line_number[0] = line_count;
			if (searches_result_line_number[1] == -1) searches_result_line_number[1] = line_count;

			cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
			cursor_offset.y = searches_result_line_number[0] * g.FontSize;
			if (searches_result_line_number[1] >= 0)
			{
				select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
				select_start_offset.y = searches_result_line_number[1] * g.FontSize;
			}

			if (is_multiline)
				text_size = ImVec2(size.x, line_count * g.FontSize);
		}

		if (edit_state.CursorFollow)
		{
			if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
			{
				const float scroll_increment_x = size.x * 0.25f;
				if (cursor_offset.x < edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
				else if (cursor_offset.x - size.x >= edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
			}
			else
			{
				edit_state.ScrollX = 0.0f;
			}

			if (is_multiline)
			{
				float scroll_y = draw_window->Scroll.y;
				if (cursor_offset.y - g.FontSize < scroll_y)
					scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
				else if (cursor_offset.y - size.y >= scroll_y)
					scroll_y = cursor_offset.y - size.y;
				draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y);                   draw_window->Scroll.y = scroll_y;
				render_pos.y = draw_window->DC.CursorPos.y;
			}
		}
		edit_state.CursorFollow = false;
		const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

		if (edit_state.StbState.select_start != edit_state.StbState.select_end)
		{
			const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
			const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

			float bg_offy_up = is_multiline ? 0.0f : -1.0f;                float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
			ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg);
			ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
			for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
			{
				if (rect_pos.y > clip_rect.w + g.FontSize)
					break;
				if (rect_pos.y < clip_rect.y)
				{
					while (p < text_selected_end)
						if (*p++ == '\n')
							break;
				}
				else
				{
					ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
					if (rect_size.x <= 0.0f) rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)((char)(0x33d + 3902 - 0x125b))) * 0.50f);                     ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
					rect.Clip(clip_rect);
					if (rect.Overlaps(clip_rect))
						draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
				}
				rect_pos.x = render_pos.x - render_scroll.x;
				rect_pos.y += g.FontSize;
			}
		}

		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, GetColorU32(ImGuiCol_Text), buf_display, buf_display + edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);

		bool cursor_is_visible = (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
		ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
		ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
		if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
			draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));

		if (is_editable)
			g.OsImePosRequest = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
	}
	else
	{
		const char* buf_end = NULL;
		if (is_multiline)
			text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize);         draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
	}

	if (is_multiline)
	{
		Dummy(text_size + ImVec2(0.0f, g.FontSize));         EndChildFrame();
		EndGroup();
		if (g.ActiveId == id || is_currently_scrolling)             window->DC.LastItemId = g.ActiveId;
	}

	if (is_password)
		PopFont();

	if (g.LogEnabled && !is_password)
		LogRenderedText(render_pos, buf_display, NULL);

	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		return enter_pressed;
	else
		return value_changed;
}

bool ImGui::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline));     return InputTextEx(label, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
}

bool ImGui::InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	return InputTextEx(label, buf, (int)buf_size, size, flags | ImGuiInputTextFlags_Multiline, callback, user_data);
}

bool ImGui::InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	BeginGroup();
	PushID(label);
	const ImVec2 button_sz = ImVec2(g.FontSize, g.FontSize) + style.FramePadding*2.0f;
	if (step_ptr)
		PushItemWidth(ImMax(1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x) * 2));

	char buf[64];
	DataTypeFormatString(data_type, data_ptr, scalar_format, buf, IM_ARRAYSIZE(buf));

	bool value_changed = false;
	if (!(extra_flags & ImGuiInputTextFlags_CharsHexadecimal))
		extra_flags |= ImGuiInputTextFlags_CharsDecimal;
	extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
	if (InputText("", buf, IM_ARRAYSIZE(buf), extra_flags))
		value_changed = DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, scalar_format);

	if (step_ptr)
	{
		PopItemWidth();
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx("-", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, ((char)(0x89 + 2069 - 0x871)), data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
			value_changed = true;
		}
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx("+", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, ((char)(0x901 + 4517 - 0x1a7b)), data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
			value_changed = true;
		}
	}
	PopID();

	if (label_size.x > 0)
	{
		SameLine(0, style.ItemInnerSpacing.x);
		RenderText(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), label);
		ItemSize(label_size, style.FramePadding.y);
	}
	EndGroup();

	return value_changed;
}

bool ImGui::InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	char display_format[16];
	if (decimal_precision < 0)
		strcpy(display_format, "%f");          else
		ImFormatString(display_format, 16, "%%.%df", decimal_precision);
	return InputScalarEx(label, ImGuiDataType_Float, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), display_format, extra_flags);
}

bool ImGui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags extra_flags)
{
	const char* scalar_format = (extra_flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
	return InputScalarEx(label, ImGuiDataType_Int, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), scalar_format, extra_flags);
}

bool ImGui::InputFloatN(const char* label, float* v, int components, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= InputFloat("##v", &v[i], 0, 0, decimal_precision, extra_flags);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 2, decimal_precision, extra_flags);
}

bool ImGui::InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 3, decimal_precision, extra_flags);
}

bool ImGui::InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
	return InputFloatN(label, v, 4, decimal_precision, extra_flags);
}

bool ImGui::InputIntN(const char* label, int* v, int components, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	bool value_changed = false;
	BeginGroup();
	PushID(label);
	PushMultiItemsWidths(components);
	for (int i = 0; i < components; i++)
	{
		PushID(i);
		value_changed |= InputInt("##v", &v[i], 0, 0, extra_flags);
		SameLine(0, g.Style.ItemInnerSpacing.x);
		PopID();
		PopItemWidth();
	}
	PopID();

	window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
	TextUnformatted(label, FindRenderedTextEnd(label));
	EndGroup();

	return value_changed;
}

bool ImGui::InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 2, extra_flags);
}

bool ImGui::InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 3, extra_flags);
}

bool ImGui::InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags)
{
	return InputIntN(label, v, 4, extra_flags);
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char** items = (const char**)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

bool ImGui::Combo(const char* label, int* current_item, const char** items, int items_count, int height_in_items)
{
	const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
	return value_changed;
}

bool ImGui::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
	int items_count = 0;
	const char* p = items_separated_by_zeros;           while (*p)
	{
		p += strlen(p) + 1;
		items_count++;
	}
	bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
	return value_changed;
}

bool ImGui::Combo(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w * .8, label_size.y + style.FramePadding.y*2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, &id))
		return false;

	const float arrow_size = (g.FontSize + style.FramePadding.x * 2.0f);
	const bool hovered = IsHovered(frame_bb, id);
	bool popup_open = IsPopupOpen(id);
	bool popup_opened_now = false;

	const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	RenderFrame(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y), frame_bb.Max, GetColorU32(popup_open || hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), true, style.FrameRounding);     RenderCollapseTriangle(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y) + style.FramePadding, true);

	if (*current_item >= 0 && *current_item < items_count)
	{
		const char* item_text;
		if (items_getter(data, *current_item, &item_text))
			RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max, item_text, NULL, NULL);
	}

	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if (hovered)
	{
		SetHoveredID(id);
		if (g.IO.MouseClicked[0])
		{
			SetActiveID(0);
			if (IsPopupOpen(id))
			{
				ClosePopup(id);
			}
			else
			{
				FocusWindow(window);
				OpenPopup(label);
				popup_open = popup_opened_now = true;
			}
		}
	}

	bool value_changed = false;
	if (IsPopupOpen(id))
	{
		if (height_in_items < 0)
			height_in_items = 7;

		float popup_height = (label_size.y + style.ItemSpacing.y) * ImMin(items_count, height_in_items) + (style.FramePadding.y * 3);
		float popup_y1 = frame_bb.Max.y;
		float popup_y2 = ImClamp(popup_y1 + popup_height, popup_y1, g.IO.DisplaySize.y - style.DisplaySafeAreaPadding.y);
		if ((popup_y2 - popup_y1) < ImMin(popup_height, frame_bb.Min.y - style.DisplaySafeAreaPadding.y))
		{
			popup_y1 = ImClamp(frame_bb.Min.y - popup_height, style.DisplaySafeAreaPadding.y, frame_bb.Min.y);
			popup_y2 = frame_bb.Min.y;
		}
		ImRect popup_rect(ImVec2(frame_bb.Min.x, popup_y1), ImVec2(frame_bb.Max.x, popup_y2));
		SetNextWindowPos(popup_rect.Min);
		SetNextWindowSize(popup_rect.GetSize());
		PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);

		const ImGuiWindowFlags flags = ImGuiWindowFlags_ComboBox | ((window->Flags & ImGuiWindowFlags_ShowBorders) ? ImGuiWindowFlags_ShowBorders : 0);
		if (BeginPopupEx(label, flags))
		{
			Spacing();
			for (int i = 0; i < items_count; i++)
			{
				PushID((void*)(intptr_t)i);
				const bool item_selected = (i == *current_item);
				const char* item_text;
				if (!items_getter(data, i, &item_text))
					item_text = "*Unknown item*";
				if (Selectable(item_text, item_selected))
				{
					SetActiveID(0);
					value_changed = true;
					*current_item = i;
				}
				if (item_selected && popup_opened_now)
					SetScrollHere();
				PopID();
			}
			EndPopup();
		}
		PopStyleVar();
	}
	return value_changed;
}

bool ImGui::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1)
		PopClipRect();

	ImGuiID id = window->GetID(label);
	ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
	ImVec2 pos = window->DC.CursorPos;
	pos.y += window->DC.CurrentLineTextBaseOffset;
	ImRect bb(pos, pos + size);
	ItemSize(bb);

	ImVec2 window_padding = window->WindowPadding;
	float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
	float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
	ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
	ImRect bb_with_spacing(pos, pos + size_draw);
	if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
		bb_with_spacing.Max.x += window_padding.x;

	float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
	float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
	float spacing_R = style.ItemSpacing.x - spacing_L;
	float spacing_D = style.ItemSpacing.y - spacing_U;
	bb_with_spacing.Min.x -= spacing_L;
	bb_with_spacing.Min.y -= spacing_U;
	bb_with_spacing.Max.x += spacing_R;
	bb_with_spacing.Max.y += spacing_D;
	if (!ItemAdd(bb_with_spacing, &id))
	{
		if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1)
			PushColumnClipRect();
		return false;
	}

	ImGuiButtonFlags button_flags = 0;
	if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick;
	if (flags & ImGuiSelectableFlags_MenuItem) button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease;
	if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
	if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
	if (flags & ImGuiSelectableFlags_Disabled)
		selected = false;

	if (hovered || selected)
	{
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
		RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
	}

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1)
	{
		PushColumnClipRect();
		bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
	}

	if (flags & ImGuiSelectableFlags_Disabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
	RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size);
	if (flags & ImGuiSelectableFlags_Disabled) PopStyleColor();

	if (pressed && !(flags & ImGuiSelectableFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
		CloseCurrentPopup();
	return pressed;
}

bool ImGui::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	if (Selectable(label, *p_selected, flags, size_arg))
	{
		*p_selected = !*p_selected;
		return true;
	}
	return false;
}

bool ImGui::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = GetStyle();
	const ImGuiID id = GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
	ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	window->DC.LastItemRect = bb;

	BeginGroup();
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	BeginChildFrame(id, frame_bb.GetSize());
	return true;
}

bool ImGui::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
	if (height_in_items < 0)
		height_in_items = ImMin(items_count, 7);
	float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

	ImVec2 size;
	size.x = 0.0f;
	size.y = GetTextLineHeightWithSpacing() * height_in_items_f + GetStyle().ItemSpacing.y;
	return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter()
{
	ImGuiWindow* parent_window = GetParentWindow();
	const ImRect bb = parent_window->DC.LastItemRect;
	const ImGuiStyle& style = GetStyle();

	EndChildFrame();

	SameLine();
	parent_window->DC.CursorPos = bb.Min;
	ItemSize(bb, style.FramePadding.y);
	EndGroup();
}

bool ImGui::ListBox(const char* label, int* current_item, const char** items, int items_count, int height_items)
{
	const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
	return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
	if (!ListBoxHeader(label, items_count, height_in_items))
		return false;

	bool value_changed = false;
	ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing());
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);
			const char* item_text;
			if (!items_getter(data, i, &item_text))
				item_text = "*Unknown item*";

			PushID(i);
			if (Selectable(item_text, item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			PopID();
		}
	ListBoxFooter();
	return value_changed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImVec2 pos = window->DC.CursorPos;
	ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 shortcut_size = shortcut ? CalcTextSize(shortcut, NULL) : ImVec2(0.0f, 0.0f);
	float w = window->MenuColumns.DeclColumns(label_size.x, shortcut_size.x, (float)(int)(g.FontSize * 1.20f));     float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);

	bool pressed = Selectable(label, false, ImGuiSelectableFlags_MenuItem | ImGuiSelectableFlags_DrawFillAvailWidth | (enabled ? 0 : ImGuiSelectableFlags_Disabled), ImVec2(w, 0.0f));
	if (shortcut_size.x > 0.0f)
	{
		PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
		RenderText(pos + ImVec2(window->MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
		PopStyleColor();
	}

	if (selected)
		RenderCheckMark(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f, 0.0f), GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled));

	return pressed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
	if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled))
	{
		if (p_selected)
			*p_selected = !*p_selected;
		return true;
	}
	return false;
}

bool ImGui::BeginMainMenuBar()
{
	ImGuiContext& g = *GImGui;
	SetNextWindowPos(ImVec2(0.0f, 0.0f));
	SetNextWindowSize(ImVec2(g.IO.DisplaySize.x, g.FontBaseSize + g.Style.FramePadding.y * 2.0f));
	PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
	if (!Begin("##MainMenuBar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar)
		|| !BeginMenuBar())
	{
		End();
		PopStyleVar(2);
		return false;
	}
	g.CurrentWindow->DC.MenuBarOffsetX += g.Style.DisplaySafeAreaPadding.x;
	return true;
}

void ImGui::EndMainMenuBar()
{
	EndMenuBar();
	End();
	PopStyleVar(2);
}

bool ImGui::BeginMenuBar()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	if (!(window->Flags & ImGuiWindowFlags_MenuBar))
		return false;

	IM_ASSERT(!window->DC.MenuBarAppending);
	BeginGroup();     PushID("##menubar");
	ImRect rect = window->MenuBarRect();
	PushClipRect(ImVec2(ImFloor(rect.Min.x + 0.5f), ImFloor(rect.Min.y + window->BorderSize + 0.5f)), ImVec2(ImFloor(rect.Max.x + 0.5f), ImFloor(rect.Max.y + 0.5f)), false);
	window->DC.CursorPos = ImVec2(rect.Min.x + window->DC.MenuBarOffsetX, rect.Min.y);    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
	window->DC.MenuBarAppending = true;
	AlignFirstTextHeightToWidgets();
	return true;
}

void ImGui::EndMenuBar()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
	IM_ASSERT(window->DC.MenuBarAppending);
	PopClipRect();
	PopID();
	window->DC.MenuBarOffsetX = window->DC.CursorPos.x - window->MenuBarRect().Min.x;
	window->DC.GroupStack.back().AdvanceCursor = false;
	EndGroup();
	window->DC.LayoutType = ImGuiLayoutType_Vertical;
	window->DC.MenuBarAppending = false;
}

bool ImGui::BeginMenu(const char* label, bool enabled)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImGuiWindow* backed_focused_window = g.FocusedWindow;

	bool pressed;
	bool menu_is_open = IsPopupOpen(id);
	bool menuset_is_open = !(window->Flags & ImGuiWindowFlags_Popup) && (g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].ParentMenuSet == window->GetID("##menus"));
	if (menuset_is_open)
		g.FocusedWindow = window;

	ImVec2 popup_pos, pos = window->DC.CursorPos;
	if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
	{
		popup_pos = ImVec2(pos.x - window->WindowPadding.x, pos.y - style.FramePadding.y + window->MenuBarHeight());
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
		PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
		float w = label_size.x;
		pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
		PopStyleVar();
		SameLine();
		window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
	}
	else
	{
		popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
		float w = window->MenuColumns.DeclColumns(label_size.x, 0.0f, (float)(int)(g.FontSize * 1.20f));         float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
		pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | ImGuiSelectableFlags_DrawFillAvailWidth | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
		if (!enabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
		RenderCollapseTriangle(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f, 0.0f), false);
		if (!enabled) PopStyleColor();
	}

	bool hovered = enabled && IsHovered(window->DC.LastItemRect, id);
	if (menuset_is_open)
		g.FocusedWindow = backed_focused_window;

	bool want_open = false, want_close = false;
	if (window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu))
	{
		bool moving_within_opened_triangle = false;
		if (g.HoveredWindow == window && g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].ParentWindow == window)
		{
			if (ImGuiWindow* next_window = g.OpenPopupStack[g.CurrentPopupStack.Size].Window)
			{
				ImRect next_window_rect = next_window->Rect();
				ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
				ImVec2 tb = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
				ImVec2 tc = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
				float extra = ImClamp(fabsf(ta.x - tb.x) * 0.30f, 5.0f, 30.0f);                 ta.x += (window->Pos.x < next_window->Pos.x) ? -0.5f : +0.5f;                   tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);                            tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
				moving_within_opened_triangle = ImIsPointInTriangle(g.IO.MousePos, ta, tb, tc);
			}
		}

		want_close = (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id && !moving_within_opened_triangle);
		want_open = (!menu_is_open && hovered && !moving_within_opened_triangle) || (!menu_is_open && hovered && pressed);
	}
	else if (menu_is_open && pressed && menuset_is_open) {
		want_close = true;
		want_open = menu_is_open = false;
	}
	else if (pressed || (hovered && menuset_is_open && !menu_is_open))         want_open = true;
	if (!enabled)         want_close = true;
	if (want_close && IsPopupOpen(id))
		ClosePopupToLevel(GImGui->CurrentPopupStack.Size);

	if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.CurrentPopupStack.Size)
	{
		OpenPopup(label);
		return false;
	}

	menu_is_open |= want_open;
	if (want_open)
		OpenPopup(label);

	if (menu_is_open)
	{
		SetNextWindowPos(popup_pos, ImGuiSetCond_Always);
		ImGuiWindowFlags flags = ImGuiWindowFlags_ShowBorders | ((window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) ? ImGuiWindowFlags_ChildMenu | ImGuiWindowFlags_ChildWindow : ImGuiWindowFlags_ChildMenu);
		menu_is_open = BeginPopupEx(label, flags);
	}

	return menu_is_open;
}

void ImGui::EndMenu()
{
	EndPopup();
}

bool ImGui::ColorButton(const ImVec4& col, bool small_height, bool outline_border)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID("#colorbutton");
	const float square_size = g.FontSize;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(square_size + style.FramePadding.y * 2, square_size + (small_height ? 0 : style.FramePadding.y * 2)));
	ItemSize(bb, small_height ? 0.0f : style.FramePadding.y);
	if (!ItemAdd(bb, &id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);
	RenderFrame(bb.Min, bb.Max, GetColorU32(col), outline_border, style.FrameRounding);

	if (hovered)
		SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col.x, col.y, col.z, col.w, IM_F32_TO_INT8_SAT(col.x), IM_F32_TO_INT8_SAT(col.y), IM_F32_TO_INT8_SAT(col.z), IM_F32_TO_INT8_SAT(col.z));

	return pressed;
}

bool ImGui::ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!ColorEdit4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

bool ImGui::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w_full = CalcItemWidth();
	const float square_sz_with_spacing = (flags & ImGuiColorEditFlags_NoColorSquare) ? 0.0f : (g.FontSize + style.FramePadding.y * 2.0f + style.ItemInnerSpacing.x);

	if (!(flags & ImGuiColorEditFlags_ModeMask_))
		flags |= ImGuiColorEditFlags_RGB;

	if (flags & ImGuiColorEditFlags_NoSliders)
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | (g.ColorEditModeStorage.GetInt(id, (flags & ImGuiColorEditFlags_ModeMask_)) & ImGuiColorEditFlags_ModeMask_);

	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags_ModeMask_)));
	float f[4] = { col[0], col[1], col[2], col[3] };
	if (flags & ImGuiColorEditFlags_HSV)
		ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

	int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	bool value_changed = false;
	int components = alpha ? 4 : 3;

	BeginGroup();
	PushID(label);

	if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0)
	{
		const float w_items_all = w_full - square_sz_with_spacing;
		const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= CalcTextSize("M:999").x);
		const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		const char* fmt_table[3][4] =
		{
			{ "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" },{ "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" },{ "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" } };
		const char** fmt = hide_prefix ? fmt_table[0] : (flags & ImGuiColorEditFlags_HSV) ? fmt_table[2] : fmt_table[1];

		PushItemWidth(w_item_one);
		for (int n = 0; n < components; n++)
		{
			if (n > 0)
				SameLine(0, style.ItemInnerSpacing.x);
			if (n + 1 == components)
				PushItemWidth(w_item_last);
			value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
		}
		PopItemWidth();
		PopItemWidth();
	}
	else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0)
	{
		const float w_slider_all = w_full - square_sz_with_spacing;
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
		ImGui::PushItemWidth(w_slider_all);
		if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
		{
			value_changed |= true;
			char* p = buf;
			while (*p == ((char)(0x20dd + 38 - 0x20e0)) || ImCharIsSpace(*p))
				p++;
			i[0] = i[1] = i[2] = i[3] = 0;
			if (alpha)
				sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]);             else
				sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
		}
		PopItemWidth();
	}

	const char* label_display_end = FindRenderedTextEnd(label);

	bool picker_active = false;
	if (!(flags & ImGuiColorEditFlags_NoColorSquare))
	{
		if (!(flags & ImGuiColorEditFlags_NoSliders))
			SameLine(0, style.ItemInnerSpacing.x);

		const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
		if (ColorButton(col_display))
		{
			if (!(flags & ImGuiColorEditFlags_NoPicker))
			{
				OpenPopup("picker");
				SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
			}
		}
		else if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
		{
			OpenPopup("context");
		}

		if (BeginPopup("picker"))
		{
			picker_active = true;
			if (label != label_display_end)
				TextUnformatted(label, label_display_end);
			PushItemWidth(256.0f + (alpha ? 2 : 1) * (style.ItemInnerSpacing.x));
			value_changed |= ColorPicker4("##picker", col, (flags & ImGuiColorEditFlags_Alpha) | (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX));
			PopItemWidth();
			EndPopup();
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions) && BeginPopup("context"))
		{
			if (MenuItem("Edit as RGB", NULL, (flags & ImGuiColorEditFlags_RGB) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_RGB));
			if (MenuItem("Edit as HSV", NULL, (flags & ImGuiColorEditFlags_HSV) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HSV));
			if (MenuItem("Edit as Hexadecimal", NULL, (flags & ImGuiColorEditFlags_HEX) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HEX));
			EndPopup();
		}

		if (IsItemHovered())
			SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0], col[1], col[2], col[3], IM_F32_TO_INT8_SAT(col[0]), IM_F32_TO_INT8_SAT(col[1]), IM_F32_TO_INT8_SAT(col[2]), IM_F32_TO_INT8_SAT(col[3]));
	}

	if (label != label_display_end)
	{
		SameLine(0, style.ItemInnerSpacing.x);
		TextUnformatted(label, label_display_end);
	}

	if (!picker_active)
	{
		for (int n = 0; n < 4; n++)
			f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed)
		{
			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}
	}

	PopID();
	EndGroup();

	return value_changed;
}

bool ImGui::ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!ColorPicker4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

bool ImGui::ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushID(label);
	ImGui::BeginGroup();

	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	float bars_width = ImGui::GetWindowFontSize() * 1.0f;                                                               float sv_picker_size = ImMax(bars_width * 2, ImGui::CalcItemWidth() - (alpha ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x));     float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;

	if (IsItemHovered())
		SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0], col[1], col[2], col[3], IM_F32_TO_INT8_SAT(col[0]), IM_F32_TO_INT8_SAT(col[1]), IM_F32_TO_INT8_SAT(col[2]), IM_F32_TO_INT8_SAT(col[3]));

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	bool value_changed = false, hsv_changed = false;
	ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
	if (ImGui::IsItemActive())
	{
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
	if (IsItemActive())
	{
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	if (alpha)
	{
		SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (IsItemActive())
		{
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}

	const char* label_display_end = FindRenderedTextEnd(label);
	if (label != label_display_end)
	{
		SameLine(0, style.ItemInnerSpacing.x);
		TextUnformatted(label, label_display_end);
	}

	if (hsv_changed)
		ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	if (!(flags & ImGuiColorEditFlags_NoSliders))
	{
		if ((flags & ImGuiColorEditFlags_ModeMask_) == 0)
			flags = ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX;
		ImGui::PushItemWidth((alpha ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags = (alpha ? ImGuiColorEditFlags_Alpha : 0) | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoColorSquare;
		if (flags & ImGuiColorEditFlags_RGB)
			value_changed |= ImGui::ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
		if (flags & ImGuiColorEditFlags_HSV)
			value_changed |= ImGui::ColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
		if (flags & ImGuiColorEditFlags_HEX)
			value_changed |= ImGui::ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
		ImGui::PopItemWidth();
	}

	if (value_changed)
	{
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	ImVec4 hue_color_f(1, 1, 1, 1);
	ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_colors[] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 1, bar0_line_y), IM_COL32_WHITE);

	if (alpha)
	{
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size), IM_COL32_WHITE, IM_COL32_WHITE, IM_COL32_BLACK, IM_COL32_BLACK);
		draw_list->AddLine(ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 1, bar1_line_y), IM_COL32_WHITE);
	}

	ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);

	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IM_COL32_WHITE);

	EndGroup();
	PopID();

	return value_changed;
}

void ImGui::Separator()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	if (window->DC.ColumnsCount > 1)
		PopClipRect();

	float x1 = window->Pos.x;
	float x2 = window->Pos.x + window->Size.x;
	if (!window->DC.GroupStack.empty())
		x1 += window->DC.IndentX;

	const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y));
	ItemSize(ImVec2(0.0f, 0.0f));     if (!ItemAdd(bb, NULL))
	{
		if (window->DC.ColumnsCount > 1)
			PushColumnClipRect();
		return;
	}

	window->DrawList->AddLine(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border));

	ImGuiContext& g = *GImGui;
	if (g.LogEnabled)
		LogText(IM_NEWLINE "--------------------------------");

	if (window->DC.ColumnsCount > 1)
	{
		PushColumnClipRect();
		window->DC.ColumnsCellMinY = window->DC.CursorPos.y;
	}
}

void ImGui::Spacing()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ItemSize(ImVec2(0, 0));
}

void ImGui::Dummy(const ImVec2& size)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	ItemSize(bb);
	ItemAdd(bb, NULL);
}

bool ImGui::IsRectVisible(const ImVec2& size)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}

void ImGui::BeginGroup()
{
	ImGuiWindow* window = GetCurrentWindow();

	window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
	ImGuiGroupData& group_data = window->DC.GroupStack.back();
	group_data.BackupCursorPos = window->DC.CursorPos;
	group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
	group_data.BackupIndentX = window->DC.IndentX;
	group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
	group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
	group_data.BackupLogLinePosY = window->DC.LogLinePosY;
	group_data.AdvanceCursor = true;

	window->DC.GroupOffsetX = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
	window->DC.IndentX = window->DC.GroupOffsetX;
	window->DC.CursorMaxPos = window->DC.CursorPos;
	window->DC.CurrentLineHeight = 0.0f;
	window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
}

void ImGui::EndGroup()
{
	ImGuiWindow* window = GetCurrentWindow();
	ImGuiStyle& style = GetStyle();

	IM_ASSERT(!window->DC.GroupStack.empty());
	ImGuiGroupData& group_data = window->DC.GroupStack.back();

	ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
	group_bb.Max.y -= style.ItemSpacing.y;          group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

	window->DC.CursorPos = group_data.BackupCursorPos;
	window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
	window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
	window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
	window->DC.IndentX = group_data.BackupIndentX;
	window->DC.GroupOffsetX = window->DC.IndentX;
	window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;

	if (group_data.AdvanceCursor)
	{
		window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);              ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
		ItemAdd(group_bb, NULL);
	}

	window->DC.GroupStack.pop_back();

}

void ImGui::SameLine(float pos_x, float spacing_w)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	if (pos_x != 0.0f)
	{
		if (spacing_w < 0.0f) spacing_w = 0.0f;
		window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffsetX + window->DC.ColumnsOffsetX;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	else
	{
		if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
		window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	window->DC.CurrentLineHeight = window->DC.PrevLineHeight;
	window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	if (window->DC.CurrentLineHeight > 0.0f)             ItemSize(ImVec2(0, 0));
	else
		ItemSize(ImVec2(0.0f, GImGui->FontSize));
}

void ImGui::NextColumn()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems || window->DC.ColumnsCount <= 1)
		return;

	ImGuiContext& g = *GImGui;
	PopItemWidth();
	PopClipRect();

	window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
	if (++window->DC.ColumnsCurrent < window->DC.ColumnsCount)
	{
		window->DC.ColumnsOffsetX = GetColumnOffset(window->DC.ColumnsCurrent) - window->DC.IndentX + g.Style.ItemSpacing.x;
		window->DrawList->ChannelsSetCurrent(window->DC.ColumnsCurrent);
	}
	else
	{
		window->DC.ColumnsCurrent = 0;
		window->DC.ColumnsOffsetX = 0.0f;
		window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY;
		window->DrawList->ChannelsSetCurrent(0);
	}
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
	window->DC.CursorPos.y = window->DC.ColumnsCellMinY;
	window->DC.CurrentLineHeight = 0.0f;
	window->DC.CurrentLineTextBaseOffset = 0.0f;

	PushColumnClipRect();
	PushItemWidth(GetColumnWidth() * 0.65f);
}

int ImGui::GetColumnIndex()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsCurrent;
}

int ImGui::GetColumnsCount()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsCount;
}

static float GetDraggedColumnOffset(int column_index)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	IM_ASSERT(column_index > 0);     IM_ASSERT(g.ActiveId == window->DC.ColumnsSetId + ImGuiID(column_index));

	float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x - window->Pos.x;
	x = ImClamp(x, ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing, ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

	return (float)(int)x;
}

float ImGui::GetColumnOffset(int column_index)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindowRead();
	if (column_index < 0)
		column_index = window->DC.ColumnsCurrent;

	if (g.ActiveId)
	{
		const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
		if (g.ActiveId == column_id)
			return GetDraggedColumnOffset(column_index);
	}

	IM_ASSERT(column_index < window->DC.ColumnsData.Size);
	const float t = window->DC.ColumnsData[column_index].OffsetNorm;
	const float x_offset = window->DC.ColumnsMinX + t * (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
	return (float)(int)x_offset;
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (column_index < 0)
		column_index = window->DC.ColumnsCurrent;

	IM_ASSERT(column_index < window->DC.ColumnsData.Size);
	const float t = (offset - window->DC.ColumnsMinX) / (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
	window->DC.ColumnsData[column_index].OffsetNorm = t;

	const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
	window->DC.StateStorage->SetFloat(column_id, t);
}

float ImGui::GetColumnWidth(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	if (column_index < 0)
		column_index = window->DC.ColumnsCurrent;

	float w = GetColumnOffset(column_index + 1) - GetColumnOffset(column_index);
	return w;
}

static void PushColumnClipRect(int column_index)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (column_index < 0)
		column_index = window->DC.ColumnsCurrent;

	float x1 = ImFloor(0.5f + window->Pos.x + ImGui::GetColumnOffset(column_index) - 1.0f);
	float x2 = ImFloor(0.5f + window->Pos.x + ImGui::GetColumnOffset(column_index + 1) - 1.0f);
	ImGui::PushClipRect(ImVec2(x1, -FLT_MAX), ImVec2(x2, +FLT_MAX), true);
}

void ImGui::Columns(int columns_count, const char* id, bool border)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	IM_ASSERT(columns_count >= 1);

	if (window->DC.ColumnsCount != 1)
	{
		if (window->DC.ColumnsCurrent != 0)
			ItemSize(ImVec2(0, 0));           PopItemWidth();
		PopClipRect();
		window->DrawList->ChannelsMerge();

		window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
		window->DC.CursorPos.y = window->DC.ColumnsCellMaxY;
	}

	if (window->DC.ColumnsCount != columns_count && window->DC.ColumnsCount != 1 && window->DC.ColumnsShowBorders && !window->SkipItems)
	{
		const float y1 = window->DC.ColumnsStartPosY;
		const float y2 = window->DC.CursorPos.y;
		for (int i = 1; i < window->DC.ColumnsCount; i++)
		{
			float x = window->Pos.x + GetColumnOffset(i);
			const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(i);
			const ImRect column_rect(ImVec2(x - 4, y1), ImVec2(x + 4, y2));
			if (IsClippedEx(column_rect, &column_id, false))
				continue;

			bool hovered, held;
			ButtonBehavior(column_rect, column_id, &hovered, &held);
			if (hovered || held)
				g.MouseCursor = ImGuiMouseCursor_ResizeEW;

			const ImU32 col = GetColorU32(held ? ImGuiCol_ColumnActive : hovered ? ImGuiCol_ColumnHovered : ImGuiCol_Column);
			const float xi = (float)(int)x;
			window->DrawList->AddLine(ImVec2(xi, y1 + 1.0f), ImVec2(xi, y2), col);

			if (held)
			{
				if (g.ActiveIdIsJustActivated)
					g.ActiveIdClickOffset.x -= 4;                   x = GetDraggedColumnOffset(i);
				SetColumnOffset(i, x);
			}
		}
	}

	PushID(0x11223347 + (id ? 0 : columns_count));
	window->DC.ColumnsSetId = window->GetID(id ? id : "columns");
	PopID();

	window->DC.ColumnsCurrent = 0;
	window->DC.ColumnsCount = columns_count;
	window->DC.ColumnsShowBorders = border;

	const float content_region_width = (window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : window->Size.x;
	window->DC.ColumnsMinX = window->DC.IndentX;     window->DC.ColumnsMaxX = content_region_width - window->Scroll.x - ((window->Flags & ImGuiWindowFlags_NoScrollbar) ? 0 : g.Style.ScrollbarSize);    window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
	window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.CursorPos.y;
	window->DC.ColumnsOffsetX = 0.0f;
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);

	if (window->DC.ColumnsCount != 1)
	{
		window->DC.ColumnsData.resize(columns_count + 1);
		for (int column_index = 0; column_index < columns_count + 1; column_index++)
		{
			const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
			KeepAliveID(column_id);
			const float default_t = column_index / (float)window->DC.ColumnsCount;
			const float t = window->DC.StateStorage->GetFloat(column_id, default_t);                  window->DC.ColumnsData[column_index].OffsetNorm = t;
		}
		window->DrawList->ChannelsSplit(window->DC.ColumnsCount);
		PushColumnClipRect();
		PushItemWidth(GetColumnWidth() * 0.65f);
	}
	else
	{
		window->DC.ColumnsData.resize(0);
	}
}

void ImGui::Indent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX += (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::Unindent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX -= (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::TreePush(const char* str_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	PushID(str_id ? str_id : "#TreePush");
}

void ImGui::TreePush(const void* ptr_id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	PushID(ptr_id ? ptr_id : (const void*)"#TreePush");
}

void ImGui::TreePushRawID(ImGuiID id)
{
	ImGuiWindow* window = GetCurrentWindow();
	Indent();
	window->DC.TreeDepth++;
	window->IDStack.push_back(id);
}

void ImGui::TreePop()
{
	ImGuiWindow* window = GetCurrentWindow();
	Unindent();
	window->DC.TreeDepth--;
	PopID();
}

void ImGui::Value(const char* prefix, bool b)
{
	Text("%s: %s", prefix, (b ? "true" : "false"));
}

void ImGui::Value(const char* prefix, int v)
{
	Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, unsigned int v)
{
	Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, float v, const char* float_format)
{
	if (float_format)
	{
		char fmt[64];
		ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
		Text(fmt, prefix, v);
	}
	else
	{
		Text("%s: %.3f", prefix, v);
	}
}

void ImGui::ValueColor(const char* prefix, const ImVec4& v)
{
	Text("%s: (%.2f,%.2f,%.2f,%.2f)", prefix, v.x, v.y, v.z, v.w);
	SameLine();
	ColorButton(v, true);
}

void ImGui::ValueColor(const char* prefix, ImU32 v)
{
	Text("%s: %08X", prefix, v);
	SameLine();

	ImVec4 col;
	col.x = (float)((v >> 0) & 0xff) / 255.0f;
	col.y = (float)((v >> 8) & 0xff) / 255.0f;
	col.z = (float)((v >> 16) & 0xff) / 255.0f;
	col.w = (float)((v >> 24) & 0xff) / 255.0f;
	ColorButton(col, true);
}


#if defined(_WIN32) && !defined(_WINDOWS_) && (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS) || !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS))
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#endif

static const char* GetClipboardTextFn_DefaultImpl()
{
	static ImVector<char> buf_local;
	buf_local.clear();
	if (!OpenClipboard(NULL))
		return NULL;
	HANDLE wbuf_handle = GetClipboardData(CF_UNICODETEXT);
	if (wbuf_handle == NULL)
		return NULL;
	if (ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle))
	{
		int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
		buf_local.resize(buf_len);
		ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
	}
	GlobalUnlock(wbuf_handle);
	CloseClipboard();
	return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(const char* text)
{
	if (!OpenClipboard(NULL))
		return;
	const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
	HGLOBAL wbuf_handle = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
	if (wbuf_handle == NULL)
		return;
	ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle);
	ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
	GlobalUnlock(wbuf_handle);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, wbuf_handle);
	CloseClipboard();
}

#else

static const char* GetClipboardTextFn_DefaultImpl()
{
	return GImGui->PrivateClipboard;
}

static void SetClipboardTextFn_DefaultImpl(const char* text)
{
	ImGuiContext& g = *GImGui;
	if (g.PrivateClipboard)
	{
		ImGui::MemFree(g.PrivateClipboard);
		g.PrivateClipboard = NULL;
	}
	const char* text_end = text + strlen(text);
	g.PrivateClipboard = (char*)ImGui::MemAlloc((size_t)(text_end - text) + 1);
	memcpy(g.PrivateClipboard, text, (size_t)(text_end - text));
	g.PrivateClipboard[(int)(text_end - text)] = 0;
}

#endif

#if defined(_WIN32) && !defined(__GNUC__) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y)
{
	if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
		if (HIMC himc = ImmGetContext(hwnd))
		{
			COMPOSITIONFORM cf;
			cf.ptCurrentPos.x = x;
			cf.ptCurrentPos.y = y;
			cf.dwStyle = CFS_FORCE_POSITION;
			ImmSetCompositionWindow(himc, &cf);
		}
}

#else

static void ImeSetInputScreenPosFn_DefaultImpl(int, int) {}

#endif


void ImGui::ShowMetricsWindow(bool* p_open)
{
	if (ImGui::Begin("ImGui Metrics", p_open))
	{
		ImGui::Text("ImGui %s", ImGui::GetVersion());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", ImGui::GetIO().MetricsRenderVertices, ImGui::GetIO().MetricsRenderIndices, ImGui::GetIO().MetricsRenderIndices / 3);
		ImGui::Text("%d allocations", ImGui::GetIO().MetricsAllocs);
		static bool show_clip_rects = true;
		ImGui::Checkbox("Show clipping rectangles when hovering a ImDrawCmd", &show_clip_rects);
		ImGui::Separator();

		struct Funcs
		{
			static void NodeDrawList(ImDrawList* draw_list, const char* label)
			{
				bool node_open = ImGui::TreeNode(draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label, draw_list->_OwnerName ? draw_list->_OwnerName : "", draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size, draw_list->CmdBuffer.Size);
				if (draw_list == ImGui::GetWindowDrawList())
				{
					ImGui::SameLine();
					ImGui::TextColored(ImColor(255, 100, 100), "CURRENTLY APPENDING");                     if (node_open) ImGui::TreePop();
					return;
				}
				if (!node_open)
					return;

				ImDrawList* overlay_draw_list = &GImGui->OverlayDrawList;                   overlay_draw_list->PushClipRectFullScreen();
				int elem_offset = 0;
				for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.begin(); pcmd < draw_list->CmdBuffer.end(); elem_offset += pcmd->ElemCount, pcmd++)
				{
					if (pcmd->UserCallback)
					{
						ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
						continue;
					}
					ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
					bool pcmd_node_open = ImGui::TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()), "Draw %-4d %s vtx, tex = %p, clip_rect = (%.0f,%.0f)..(%.0f,%.0f)", pcmd->ElemCount, draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed", pcmd->TextureId, pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);
					if (show_clip_rects && ImGui::IsItemHovered())
					{
						ImRect clip_rect = pcmd->ClipRect;
						ImRect vtxs_rect;
						for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount; i++)
							vtxs_rect.Add(draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
						clip_rect.Floor(); overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max, IM_COL32(255, 255, 0, 255));
						vtxs_rect.Floor(); overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max, IM_COL32(255, 0, 255, 255));
					}
					if (!pcmd_node_open)
						continue;
					ImGuiListClipper clipper(pcmd->ElemCount / 3);                     while (clipper.Step())
						for (int prim = clipper.DisplayStart, vtx_i = elem_offset + clipper.DisplayStart * 3; prim < clipper.DisplayEnd; prim++)
						{
							char buf[300], *buf_p = buf;
							ImVec2 triangles_pos[3];
							for (int n = 0; n < 3; n++, vtx_i++)
							{
								ImDrawVert& v = draw_list->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
								triangles_pos[n] = v.pos;
								buf_p += sprintf(buf_p, "%s %04d { pos = (%8.2f,%8.2f), uv = (%.6f,%.6f), col = %08X }\n", (n == 0) ? "vtx" : "   ", vtx_i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
							}
							ImGui::Selectable(buf, false);
							if (ImGui::IsItemHovered())
								overlay_draw_list->AddPolyline(triangles_pos, 3, IM_COL32(255, 255, 0, 255), true, 1.0f, false);
						}
					ImGui::TreePop();
				}
				overlay_draw_list->PopClipRect();
				ImGui::TreePop();
			}

			static void NodeWindows(ImVector<ImGuiWindow*>& windows, const char* label)
			{
				if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size))
					return;
				for (int i = 0; i < windows.Size; i++)
					Funcs::NodeWindow(windows[i], "Window");
				ImGui::TreePop();
			}

			static void NodeWindow(ImGuiWindow* window, const char* label)
			{
				if (!ImGui::TreeNode(window, "%s '%s', %d @ 0x%p", label, window->Name, window->Active || window->WasActive, window))
					return;
				NodeDrawList(window->DrawList, "DrawList");
				ImGui::BulletText("Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)", window->Size.x, window->Size.y, window->SizeContents.x, window->SizeContents.y);
				ImGui::BulletText("Scroll: (%.2f,%.2f)", window->Scroll.x, window->Scroll.y);
				if (window->RootWindow != window) NodeWindow(window->RootWindow, "RootWindow");
				if (window->DC.ChildWindows.Size > 0) NodeWindows(window->DC.ChildWindows, "ChildWindows");
				ImGui::BulletText("Storage: %d bytes", window->StateStorage.Data.Size * (int)sizeof(ImGuiStorage::Pair));
				ImGui::TreePop();
			}
		};

		ImGuiContext& g = *GImGui;                        Funcs::NodeWindows(g.Windows, "Windows");
		if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)", g.RenderDrawLists[0].Size))
		{
			for (int i = 0; i < g.RenderDrawLists[0].Size; i++)
				Funcs::NodeDrawList(g.RenderDrawLists[0][i], "DrawList");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Popups", "Open Popups Stack (%d)", g.OpenPopupStack.Size))
		{
			for (int i = 0; i < g.OpenPopupStack.Size; i++)
			{
				ImGuiWindow* window = g.OpenPopupStack[i].Window;
				ImGui::BulletText("PopupID: %08x, Window: '%s'%s%s", g.OpenPopupStack[i].PopupId, window ? window->Name : "NULL", window && (window->Flags & ImGuiWindowFlags_ChildWindow) ? " ChildWindow" : "", window && (window->Flags & ImGuiWindowFlags_ChildMenu) ? " ChildMenu" : "");
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Basic state"))
		{
			ImGui::Text("FocusedWindow: '%s'", g.FocusedWindow ? g.FocusedWindow->Name : "NULL");
			ImGui::Text("HoveredWindow: '%s'", g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
			ImGui::Text("HoveredRootWindow: '%s'", g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
			ImGui::Text("HoveredID: 0x%08X/0x%08X", g.HoveredId, g.HoveredIdPreviousFrame);             ImGui::Text("ActiveID: 0x%08X/0x%08X", g.ActiveId, g.ActiveIdPreviousFrame);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}


#ifdef IMGUI_INCLUDE_IMGUI_USER_INL
#include "imgui_user.inl"
#endif


