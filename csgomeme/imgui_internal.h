

#pragma once

#ifndef IMGUI_VERSION
#error Must include imgui.h before imgui_internal.h
#endif

#include <stdio.h>      
#include <math.h>       

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of struct 'xxx' // when IMGUI_API is set to__declspec(dllexport)
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"        // for stb_textedit.h
#pragma clang diagnostic ignored "-Wmissing-prototypes"     // for stb_textedit.h
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif


struct ImRect;
struct ImGuiColMod;
struct ImGuiStyleMod;
struct ImGuiGroupData;
struct ImGuiSimpleColumns;
struct ImGuiDrawContext;
struct ImGuiTextEditState;
struct ImGuiIniData;
struct ImGuiMouseCursorData;
struct ImGuiPopupRef;
struct ImGuiWindow;

typedef int ImGuiLayoutType;      typedef int ImGuiButtonFlags;     typedef int ImGuiTreeNodeFlags;   typedef int ImGuiSliderFlags;

namespace ImGuiStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiTextEditState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   -1.0f
#include "stb_textedit.h"

}

extern IMGUI_API ImGuiContext*  GImGui;

#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_PI                   3.14159265358979323846f

IMGUI_API int           ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end);      IMGUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);          IMGUI_API int           ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);   IMGUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                            IMGUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);
IMGUI_API ImU32         ImHash(const void* data, int data_size, ImU32 seed = 0);    IMGUI_API void*         ImLoadFileToMemory(const char* filename, const char* file_open_mode, int* out_file_size = NULL, int padding_bytes = 0);
IMGUI_API bool          ImIsPointInTriangle(const ImVec2& p, const ImVec2& a, const ImVec2& b, const ImVec2& c);
static inline bool      ImCharIsSpace(int c) { return c == ((char)(0x6a6 + 3474 - 0x1418)) || c == '\t' || c == 0x3000; }
static inline bool      ImIsPowerOfTwo(int v) { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }

IMGUI_API int           ImStricmp(const char* str1, const char* str2);
IMGUI_API int           ImStrnicmp(const char* str1, const char* str2, int count);
IMGUI_API char*         ImStrdup(const char* str);
IMGUI_API int           ImStrlenW(const ImWchar* str);
IMGUI_API const ImWchar*ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin); IMGUI_API const char*   ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
IMGUI_API int           ImFormatString(char* buf, int buf_size, const char* fmt, ...) IM_PRINTFARGS(3);
IMGUI_API int           ImFormatStringV(char* buf, int buf_size, const char* fmt, va_list args);

#ifdef IMGUI_DEFINE_MATH_OPERATORS
static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x*rhs, lhs.y*rhs); }
static inline ImVec2 operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x*rhs.x, lhs.y*rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
#endif

static inline int    ImMin(int lhs, int rhs) { return lhs < rhs ? lhs : rhs; }
static inline int    ImMax(int lhs, int rhs) { return lhs >= rhs ? lhs : rhs; }
static inline float  ImMin(float lhs, float rhs) { return lhs < rhs ? lhs : rhs; }
static inline float  ImMax(float lhs, float rhs) { return lhs >= rhs ? lhs : rhs; }
static inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(ImMin(lhs.x, rhs.x), ImMin(lhs.y, rhs.y)); }
static inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(ImMax(lhs.x, rhs.x), ImMax(lhs.y, rhs.y)); }
static inline int    ImClamp(int v, int mn, int mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
static inline float  ImClamp(float v, float mn, float mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
static inline ImVec2 ImClamp(const ImVec2& f, const ImVec2& mn, ImVec2 mx) { return ImVec2(ImClamp(f.x, mn.x, mx.x), ImClamp(f.y, mn.y, mx.y)); }
static inline float  ImSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLerp(float a, float b, float t) { return a + (b - a) * t; }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t) { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline float  ImLengthSqr(const ImVec2& lhs) { return lhs.x*lhs.x + lhs.y*lhs.y; }
static inline float  ImLengthSqr(const ImVec4& lhs) { return lhs.x*lhs.x + lhs.y*lhs.y + lhs.z*lhs.z + lhs.w*lhs.w; }
static inline float  ImInvLength(const ImVec2& lhs, float fail_value) { float d = lhs.x*lhs.x + lhs.y*lhs.y; if (d > 0.0f) return 1.0f / sqrtf(d); return fail_value; }
static inline float  ImFloor(float f) { return (float)(int)f; }
static inline ImVec2 ImFloor(ImVec2 v) { return ImVec2((float)(int)v.x, (float)(int)v.y); }

#ifdef IMGUI_DEFINE_PLACEMENT_NEW
struct ImPlacementNewDummy {};
inline void* operator new(size_t, ImPlacementNewDummy, void* ptr) { return ptr; }
inline void operator delete(void*, ImPlacementNewDummy, void*) {}
#define IM_PLACEMENT_NEW(_PTR)  new(ImPlacementNewDummy(), _PTR)
#endif


enum ImGuiButtonFlags_
{
	ImGuiButtonFlags_Repeat = 1 << 0, ImGuiButtonFlags_PressedOnClickRelease = 1 << 1, ImGuiButtonFlags_PressedOnClick = 1 << 2, ImGuiButtonFlags_PressedOnRelease = 1 << 3, ImGuiButtonFlags_PressedOnDoubleClick = 1 << 4, ImGuiButtonFlags_FlattenChilds = 1 << 5, ImGuiButtonFlags_DontClosePopups = 1 << 6, ImGuiButtonFlags_Disabled = 1 << 7, ImGuiButtonFlags_AlignTextBaseLine = 1 << 8, ImGuiButtonFlags_NoKeyModifiers = 1 << 9, ImGuiButtonFlags_AllowOverlapMode = 1 << 10
};

enum ImGuiSliderFlags_
{
	ImGuiSliderFlags_Vertical = 1 << 0
};

enum ImGuiSelectableFlagsPrivate_
{
	ImGuiSelectableFlags_Menu = 1 << 3,
	ImGuiSelectableFlags_MenuItem = 1 << 4,
	ImGuiSelectableFlags_Disabled = 1 << 5,
	ImGuiSelectableFlags_DrawFillAvailWidth = 1 << 6
};

enum ImGuiLayoutType_
{
	ImGuiLayoutType_Vertical,
	ImGuiLayoutType_Horizontal
};

enum ImGuiPlotType
{
	ImGuiPlotType_Lines,
	ImGuiPlotType_Histogram
};

enum ImGuiDataType
{
	ImGuiDataType_Int,
	ImGuiDataType_Float
};

struct IMGUI_API ImRect
{
	ImVec2      Min;        ImVec2      Max;
	ImRect() : Min(FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX) {}
	ImRect(const ImVec2& min, const ImVec2& max) : Min(min), Max(max) {}
	ImRect(const ImVec4& v) : Min(v.x, v.y), Max(v.z, v.w) {}
	ImRect(float x1, float y1, float x2, float y2) : Min(x1, y1), Max(x2, y2) {}

	ImVec2      GetCenter() const { return ImVec2((Min.x + Max.x)*0.5f, (Min.y + Max.y)*0.5f); }
	ImVec2      GetSize() const { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
	float       GetWidth() const { return Max.x - Min.x; }
	float       GetHeight() const { return Max.y - Min.y; }
	ImVec2      GetTL() const { return Min; }                       ImVec2      GetTR() const { return ImVec2(Max.x, Min.y); }      ImVec2      GetBL() const { return ImVec2(Min.x, Max.y); }      ImVec2      GetBR() const { return Max; }                       bool        Contains(const ImVec2& p) const { return p.x >= Min.x     && p.y >= Min.y     && p.x < Max.x     && p.y < Max.y; }
	bool        Contains(const ImRect& r) const { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x < Max.x && r.Max.y < Max.y; }
	bool        Overlaps(const ImRect& r) const { return r.Min.y < Max.y  && r.Max.y > Min.y  && r.Min.x < Max.x && r.Max.x > Min.x; }
	void        Add(const ImVec2& rhs) { if (Min.x > rhs.x)     Min.x = rhs.x;     if (Min.y > rhs.y) Min.y = rhs.y;         if (Max.x < rhs.x) Max.x = rhs.x;         if (Max.y < rhs.y) Max.y = rhs.y; }
	void        Add(const ImRect& rhs) { if (Min.x > rhs.Min.x) Min.x = rhs.Min.x; if (Min.y > rhs.Min.y) Min.y = rhs.Min.y; if (Max.x < rhs.Max.x) Max.x = rhs.Max.x; if (Max.y < rhs.Max.y) Max.y = rhs.Max.y; }
	void        Expand(const float amount) { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
	void        Expand(const ImVec2& amount) { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
	void        Reduce(const ImVec2& amount) { Min.x += amount.x; Min.y += amount.y; Max.x -= amount.x; Max.y -= amount.y; }
	void        Clip(const ImRect& clip) { if (Min.x < clip.Min.x) Min.x = clip.Min.x; if (Min.y < clip.Min.y) Min.y = clip.Min.y; if (Max.x > clip.Max.x) Max.x = clip.Max.x; if (Max.y > clip.Max.y) Max.y = clip.Max.y; }
	void        Floor() { Min.x = (float)(int)Min.x; Min.y = (float)(int)Min.y; Max.x = (float)(int)Max.x; Max.y = (float)(int)Max.y; }
	ImVec2      GetClosestPoint(ImVec2 p, bool on_edge) const
	{
		if (!on_edge && Contains(p))
			return p;
		if (p.x > Max.x) p.x = Max.x;
		else if (p.x < Min.x) p.x = Min.x;
		if (p.y > Max.y) p.y = Max.y;
		else if (p.y < Min.y) p.y = Min.y;
		return p;
	}
};

struct ImGuiColMod
{
	ImGuiCol    Col;
	ImVec4      PreviousValue;
};

struct ImGuiStyleMod
{
	ImGuiStyleVar   Var;
	ImVec2          PreviousValue;
};

struct ImGuiGroupData
{
	ImVec2      BackupCursorPos;
	ImVec2      BackupCursorMaxPos;
	float       BackupIndentX;
	float       BackupCurrentLineHeight;
	float       BackupCurrentLineTextBaseOffset;
	float       BackupLogLinePosY;
	bool        AdvanceCursor;
};

struct ImGuiColumnData
{
	float       OffsetNorm;
};

struct IMGUI_API ImGuiSimpleColumns
{
	int         Count;
	float       Spacing;
	float       Width, NextWidth;
	float       Pos[8], NextWidths[8];

	ImGuiSimpleColumns();
	void        Update(int count, float spacing, bool clear);
	float       DeclColumns(float w0, float w1, float w2);
	float       CalcExtraSpace(float avail_w);
};

struct IMGUI_API ImGuiTextEditState
{
	ImGuiID             Id;                             ImVector<ImWchar>   Text;                           ImVector<char>      InitialText;                    ImVector<char>      TempTextBuffer;
	int                 CurLenA, CurLenW;               int                 BufSizeA;                       float               ScrollX;
	ImGuiStb::STB_TexteditState   StbState;
	float               CursorAnim;
	bool                CursorFollow;
	bool                SelectedAllMouseLock;

	ImGuiTextEditState() { memset(this, 0, sizeof(*this)); }
	void                CursorAnimReset() { CursorAnim = -0.30f; }                                       void                CursorClamp() { StbState.cursor = ImMin(StbState.cursor, CurLenW); StbState.select_start = ImMin(StbState.select_start, CurLenW); StbState.select_end = ImMin(StbState.select_end, CurLenW); }
	bool                HasSelection() const { return StbState.select_start != StbState.select_end; }
	void                ClearSelection() { StbState.select_start = StbState.select_end = StbState.cursor; }
	void                SelectAll() { StbState.select_start = 0; StbState.select_end = CurLenW; StbState.cursor = StbState.select_end; StbState.has_preferred_x = false; }
	void                OnKeyPressed(int key);
};

struct ImGuiIniData
{
	char*       Name;
	ImGuiID     Id;
	ImVec2      Pos;
	ImVec2      Size;
	bool        Collapsed;
};

struct ImGuiMouseCursorData
{
	ImGuiMouseCursor    Type;
	ImVec2              HotOffset;
	ImVec2              Size;
	ImVec2              TexUvMin[2];
	ImVec2              TexUvMax[2];
};

struct ImGuiPopupRef
{
	ImGuiID         PopupId;            ImGuiWindow*    Window;             ImGuiWindow*    ParentWindow;       ImGuiID         ParentMenuSet;      ImVec2          MousePosOnOpen;
	ImGuiPopupRef(ImGuiID id, ImGuiWindow* parent_window, ImGuiID parent_menu_set, const ImVec2& mouse_pos) { PopupId = id; Window = NULL; ParentWindow = parent_window; ParentMenuSet = parent_menu_set; MousePosOnOpen = mouse_pos; }
};

struct ImGuiContext
{
	bool                    Initialized;
	ImGuiIO                 IO;
	ImGuiStyle              Style;
	ImFont*                 Font;                                   float                   FontSize;                               float                   FontBaseSize;                           ImVec2                  FontTexUvWhitePixel;
	float                   Time;
	int                     FrameCount;
	int                     FrameCountEnded;
	int                     FrameCountRendered;
	ImVector<ImGuiWindow*>  Windows;
	ImVector<ImGuiWindow*>  WindowsSortBuffer;
	ImGuiWindow*            CurrentWindow;                          ImVector<ImGuiWindow*>  CurrentWindowStack;
	ImGuiWindow*            FocusedWindow;                          ImGuiWindow*            HoveredWindow;                          ImGuiWindow*            HoveredRootWindow;                      ImGuiID                 HoveredId;                              bool                    HoveredIdAllowOverlap;
	ImGuiID                 HoveredIdPreviousFrame;
	ImGuiID                 ActiveId;                               ImGuiID                 ActiveIdPreviousFrame;
	bool                    ActiveIdIsAlive;
	bool                    ActiveIdIsJustActivated;                bool                    ActiveIdAllowOverlap;                   ImVec2                  ActiveIdClickOffset;                    ImGuiWindow*            ActiveIdWindow;
	ImGuiWindow*            MovedWindow;                            ImGuiID                 MovedWindowMoveId;                      ImVector<ImGuiIniData>  Settings;                               float                   SettingsDirtyTimer;                     ImVector<ImGuiColMod>   ColorModifiers;                         ImVector<ImGuiStyleMod> StyleModifiers;                         ImVector<ImFont*>       FontStack;                              ImVector<ImGuiPopupRef> OpenPopupStack;                         ImVector<ImGuiPopupRef> CurrentPopupStack;
	ImVec2                  SetNextWindowPosVal;
	ImVec2                  SetNextWindowSizeVal;
	ImVec2                  SetNextWindowContentSizeVal;
	bool                    SetNextWindowCollapsedVal;
	ImGuiSetCond            SetNextWindowPosCond;
	ImGuiSetCond            SetNextWindowSizeCond;
	ImGuiSetCond            SetNextWindowContentSizeCond;
	ImGuiSetCond            SetNextWindowCollapsedCond;
	ImRect                  SetNextWindowSizeConstraintRect;               ImGuiSizeConstraintCallback SetNextWindowSizeConstraintCallback;
	void*                       SetNextWindowSizeConstraintCallbackUserData;
	bool                    SetNextWindowSizeConstraint;
	bool                    SetNextWindowFocus;
	bool                    SetNextTreeNodeOpenVal;
	ImGuiSetCond            SetNextTreeNodeOpenCond;

	ImDrawData              RenderDrawData;                         ImVector<ImDrawList*>   RenderDrawLists[3];
	float                   ModalWindowDarkeningRatio;
	ImDrawList              OverlayDrawList;                        ImGuiMouseCursor        MouseCursor;
	ImGuiMouseCursorData    MouseCursorData[ImGuiMouseCursor_Count_];

	ImGuiTextEditState      InputTextState;
	ImFont                  InputTextPasswordFont;
	ImGuiID                 ScalarAsInputTextId;                    ImGuiStorage            ColorEditModeStorage;                   float                   DragCurrentValue;                       ImVec2                  DragLastMouseDelta;
	float                   DragSpeedDefaultRatio;                  float                   DragSpeedScaleSlow;
	float                   DragSpeedScaleFast;
	ImVec2                  ScrollbarClickDeltaToGrabCenter;        char                    Tooltip[1024];
	char*                   PrivateClipboard;                       ImVec2                  OsImePosRequest, OsImePosSet;
	bool                    LogEnabled;
	FILE*                   LogFile;                                ImGuiTextBuffer*        LogClipboard;                           int                     LogStartDepth;
	int                     LogAutoExpandMaxDepth;

	float                   FramerateSecPerFrame[120];              int                     FramerateSecPerFrameIdx;
	float                   FramerateSecPerFrameAccum;
	int                     CaptureMouseNextFrame;                  int                     CaptureKeyboardNextFrame;
	char                    TempBuffer[1024 * 3 + 1];
	ImGuiContext()
	{
		Initialized = false;
		Font = NULL;
		FontSize = FontBaseSize = 0.0f;
		FontTexUvWhitePixel = ImVec2(0.0f, 0.0f);

		Time = 0.0f;
		FrameCount = 0;
		FrameCountEnded = FrameCountRendered = -1;
		CurrentWindow = NULL;
		FocusedWindow = NULL;
		HoveredWindow = NULL;
		HoveredRootWindow = NULL;
		HoveredId = 0;
		HoveredIdAllowOverlap = false;
		HoveredIdPreviousFrame = 0;
		ActiveId = 0;
		ActiveIdPreviousFrame = 0;
		ActiveIdIsAlive = false;
		ActiveIdIsJustActivated = false;
		ActiveIdAllowOverlap = false;
		ActiveIdClickOffset = ImVec2(-1, -1);
		ActiveIdWindow = NULL;
		MovedWindow = NULL;
		MovedWindowMoveId = 0;
		SettingsDirtyTimer = 0.0f;

		SetNextWindowPosVal = ImVec2(0.0f, 0.0f);
		SetNextWindowSizeVal = ImVec2(0.0f, 0.0f);
		SetNextWindowCollapsedVal = false;
		SetNextWindowPosCond = 0;
		SetNextWindowSizeCond = 0;
		SetNextWindowContentSizeCond = 0;
		SetNextWindowCollapsedCond = 0;
		SetNextWindowFocus = false;
		SetNextWindowSizeConstraintCallback = NULL;
		SetNextWindowSizeConstraintCallbackUserData = NULL;
		SetNextTreeNodeOpenVal = false;
		SetNextTreeNodeOpenCond = 0;

		ScalarAsInputTextId = 0;
		DragCurrentValue = 0.0f;
		DragLastMouseDelta = ImVec2(0.0f, 0.0f);
		DragSpeedDefaultRatio = 1.0f / 100.0f;
		DragSpeedScaleSlow = 0.01f;
		DragSpeedScaleFast = 10.0f;
		ScrollbarClickDeltaToGrabCenter = ImVec2(0.0f, 0.0f);
		memset(Tooltip, 0, sizeof(Tooltip));
		PrivateClipboard = NULL;
		OsImePosRequest = OsImePosSet = ImVec2(-1.0f, -1.0f);

		ModalWindowDarkeningRatio = 0.0f;
		OverlayDrawList._OwnerName = "##Overlay";         MouseCursor = ImGuiMouseCursor_Arrow;
		memset(MouseCursorData, 0, sizeof(MouseCursorData));

		LogEnabled = false;
		LogFile = NULL;
		LogClipboard = NULL;
		LogStartDepth = 0;
		LogAutoExpandMaxDepth = 2;

		memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
		FramerateSecPerFrameIdx = 0;
		FramerateSecPerFrameAccum = 0.0f;
		CaptureMouseNextFrame = CaptureKeyboardNextFrame = -1;
		memset(TempBuffer, 0, sizeof(TempBuffer));
	}
};

struct IMGUI_API ImGuiDrawContext
{
	ImVec2                  CursorPos;
	ImVec2                  CursorPosPrevLine;
	ImVec2                  CursorStartPos;
	ImVec2                  CursorMaxPos;               float                   CurrentLineHeight;
	float                   CurrentLineTextBaseOffset;
	float                   PrevLineHeight;
	float                   PrevLineTextBaseOffset;
	float                   LogLinePosY;
	int                     TreeDepth;
	ImGuiID                 LastItemId;
	ImRect                  LastItemRect;
	bool                    LastItemHoveredAndUsable;      bool                    LastItemHoveredRect;           bool                    MenuBarAppending;
	float                   MenuBarOffsetX;
	ImVector<ImGuiWindow*>  ChildWindows;
	ImGuiStorage*           StateStorage;
	ImGuiLayoutType         LayoutType;

	float                   ItemWidth;                  float                   TextWrapPos;                bool                    AllowKeyboardFocus;         bool                    ButtonRepeat;               ImVector<float>         ItemWidthStack;
	ImVector<float>         TextWrapPosStack;
	ImVector<bool>          AllowKeyboardFocusStack;
	ImVector<bool>          ButtonRepeatStack;
	ImVector<ImGuiGroupData>GroupStack;
	int                     StackSizesBackup[6];
	float                   IndentX;                    float                   GroupOffsetX;
	float                   ColumnsOffsetX;             int                     ColumnsCurrent;
	int                     ColumnsCount;
	float                   ColumnsMinX;
	float                   ColumnsMaxX;
	float                   ColumnsStartPosY;
	float                   ColumnsCellMinY;
	float                   ColumnsCellMaxY;
	bool                    ColumnsShowBorders;
	ImGuiID                 ColumnsSetId;
	ImVector<ImGuiColumnData> ColumnsData;

	ImGuiDrawContext()
	{
		CursorPos = CursorPosPrevLine = CursorStartPos = CursorMaxPos = ImVec2(0.0f, 0.0f);
		CurrentLineHeight = PrevLineHeight = 0.0f;
		CurrentLineTextBaseOffset = PrevLineTextBaseOffset = 0.0f;
		LogLinePosY = -1.0f;
		TreeDepth = 0;
		LastItemId = 0;
		LastItemRect = ImRect(0.0f, 0.0f, 0.0f, 0.0f);
		LastItemHoveredAndUsable = LastItemHoveredRect = false;
		MenuBarAppending = false;
		MenuBarOffsetX = 0.0f;
		StateStorage = NULL;
		LayoutType = ImGuiLayoutType_Vertical;
		ItemWidth = 0.0f;
		ButtonRepeat = false;
		AllowKeyboardFocus = true;
		TextWrapPos = -1.0f;
		memset(StackSizesBackup, 0, sizeof(StackSizesBackup));

		IndentX = 0.0f;
		ColumnsOffsetX = 0.0f;
		ColumnsCurrent = 0;
		ColumnsCount = 1;
		ColumnsMinX = ColumnsMaxX = 0.0f;
		ColumnsStartPosY = 0.0f;
		ColumnsCellMinY = ColumnsCellMaxY = 0.0f;
		ColumnsShowBorders = true;
		ColumnsSetId = 0;
	}
};

struct IMGUI_API ImGuiWindow
{
	char*                   Name;
	ImGuiID                 ID;                                     ImGuiWindowFlags        Flags;                                  int                     IndexWithinParent;                      ImVec2                  PosFloat;
	ImVec2                  Pos;                                    ImVec2                  Size;                                   ImVec2                  SizeFull;                               ImVec2                  SizeContents;                           ImVec2                  SizeContentsExplicit;                   ImRect                  ContentsRegionRect;                     ImVec2                  WindowPadding;                          ImGuiID                 MoveId;                                 ImVec2                  Scroll;
	ImVec2                  ScrollTarget;                           ImVec2                  ScrollTargetCenterRatio;                bool                    ScrollbarX, ScrollbarY;
	ImVec2                  ScrollbarSizes;
	float                   BorderSize;
	bool                    Active;                                 bool                    WasActive;
	bool                    Accessed;                               bool                    Collapsed;                              bool                    SkipItems;                              int                     BeginCount;                             ImGuiID                 PopupId;                                int                     AutoFitFramesX, AutoFitFramesY;
	bool                    AutoFitOnlyGrows;
	int                     AutoPosLastDirection;
	int                     HiddenFrames;
	int                     SetWindowPosAllowFlags;                 int                     SetWindowSizeAllowFlags;                int                     SetWindowCollapsedAllowFlags;           bool                    SetWindowPosCenterWanted;

	ImGuiDrawContext        DC;                                     ImVector<ImGuiID>       IDStack;                                ImRect                  ClipRect;                               ImRect                  WindowRectClipped;                      int                     LastFrameActive;
	float                   ItemWidthDefault;
	ImGuiSimpleColumns      MenuColumns;                            ImGuiStorage            StateStorage;
	float                   FontWindowScale;                        ImDrawList*             DrawList;
	ImGuiWindow*            RootWindow;                             ImGuiWindow*            RootNonPopupWindow;                     ImGuiWindow*            ParentWindow;
	int                     FocusIdxAllCounter;                     int                     FocusIdxTabCounter;                     int                     FocusIdxAllRequestCurrent;              int                     FocusIdxTabRequestCurrent;              int                     FocusIdxAllRequestNext;                 int                     FocusIdxTabRequestNext;
public:
	ImGuiWindow(const char* name);
	~ImGuiWindow();

	ImGuiID     GetID(const char* str, const char* str_end = NULL);
	ImGuiID     GetID(const void* ptr);
	ImGuiID     GetIDNoKeepAlive(const char* str, const char* str_end = NULL);

	ImRect      Rect() const { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
	float       CalcFontSize() const { return GImGui->FontBaseSize * FontWindowScale; }
	float       TitleBarHeight() const { return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f; }
	ImRect      TitleBarRect() const { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
	float       MenuBarHeight() const { return (Flags & ImGuiWindowFlags_MenuBar) ? CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f : 0.0f; }
	ImRect      MenuBarRect() const { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};


namespace ImGui
{
	inline    ImGuiWindow*  GetCurrentWindowRead() { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
	inline    ImGuiWindow*  GetCurrentWindow() { ImGuiContext& g = *GImGui; g.CurrentWindow->Accessed = true; return g.CurrentWindow; }
	IMGUI_API ImGuiWindow*  GetParentWindow();
	IMGUI_API ImGuiWindow*  FindWindowByName(const char* name);
	IMGUI_API void          FocusWindow(ImGuiWindow* window);

	IMGUI_API void          EndFrame();
	IMGUI_API void          SetActiveID(ImGuiID id, ImGuiWindow* window);
	IMGUI_API void          SetHoveredID(ImGuiID id);
	IMGUI_API void          KeepAliveID(ImGuiID id);

	IMGUI_API void          ItemSize(const ImVec2& size, float text_offset_y = 0.0f);
	IMGUI_API void          ItemSize(const ImRect& bb, float text_offset_y = 0.0f);
	IMGUI_API bool          ItemAdd(const ImRect& bb, const ImGuiID* id);
	IMGUI_API bool          IsClippedEx(const ImRect& bb, const ImGuiID* id, bool clip_even_when_logged);
	IMGUI_API bool          IsHovered(const ImRect& bb, ImGuiID id, bool flatten_childs = false);
	IMGUI_API bool          FocusableItemRegister(ImGuiWindow* window, bool is_active, bool tab_stop = true);          IMGUI_API void          FocusableItemUnregister(ImGuiWindow* window);
	IMGUI_API ImVec2        CalcItemSize(ImVec2 size, float default_x, float default_y);
	IMGUI_API float         CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);

	IMGUI_API void          OpenPopupEx(const char* str_id, bool reopen_existing);

	IMGUI_API void          RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
	IMGUI_API void          RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
	IMGUI_API void          RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, ImGuiAlign align = ImGuiAlign_Default, const ImVec2* clip_min = NULL, const ImVec2* clip_max = NULL);
	IMGUI_API void          RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
	IMGUI_API void          RenderCollapseTriangle(ImVec2 pos, bool is_open, float scale = 1.0f, bool shadow = false);
	IMGUI_API void          RenderBullet(ImVec2 pos);
	IMGUI_API void          RenderCheckMark(ImVec2 pos, ImU32 col);
	IMGUI_API const char*   FindRenderedTextEnd(const char* text, const char* text_end = NULL);
	IMGUI_API bool          ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
	IMGUI_API bool          ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
	IMGUI_API bool          CloseButton(ImGuiID id, const ImVec2& pos, float radius);

	IMGUI_API bool          SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags = 0);
	IMGUI_API bool          SliderFloatN(const char* label, float* v, int components, float v_min, float v_max, const char* display_format, float power);
	IMGUI_API bool          SliderIntN(const char* label, int* v, int components, int v_min, int v_max, const char* display_format);

	IMGUI_API bool          DragBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_speed, float v_min, float v_max, int decimal_precision, float power);
	IMGUI_API bool          DragFloatN(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format, float power);
	IMGUI_API bool          DragIntN(const char* label, int* v, int components, float v_speed, int v_min, int v_max, const char* display_format);

	IMGUI_API bool          InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool          InputFloatN(const char* label, float* v, int components, int decimal_precision, ImGuiInputTextFlags extra_flags);
	IMGUI_API bool          InputIntN(const char* label, int* v, int components, ImGuiInputTextFlags extra_flags);
	IMGUI_API bool          InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags);
	IMGUI_API bool          InputScalarAsWidgetReplacement(const ImRect& aabb, const char* label, ImGuiDataType data_type, void* data_ptr, ImGuiID id, int decimal_precision);

	IMGUI_API bool          TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
	IMGUI_API bool          TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags = 0);                         IMGUI_API void          TreePushRawID(ImGuiID id);

	IMGUI_API void          PlotEx(ImGuiPlotType plot_type, const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size);

	IMGUI_API int           ParseFormatPrecision(const char* fmt, int default_value);
	IMGUI_API float         RoundScalar(float value, int decimal_precision);

}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

