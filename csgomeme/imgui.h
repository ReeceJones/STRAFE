

#pragma once

#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h"       
#endif
#include <float.h>          
#include <stdarg.h>         
#include <stddef.h>         
#include <string.h>         

#define IMGUI_VERSION       "1.50 WIP"

#ifndef IMGUI_API
#define IMGUI_API
#endif

#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)    assert(_EXPR)
#endif

#if defined(__clang__) || defined(__GNUC__)
#define IM_PRINTFARGS(FMT) __attribute__((format(printf, FMT, (FMT+1))))
#else
#define IM_PRINTFARGS(FMT)
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif

struct ImDrawChannel;               struct ImDrawCmd;                   struct ImDrawData;                  struct ImDrawList;                  struct ImDrawVert;                  struct ImFont;                      struct ImFontAtlas;                 struct ImFontConfig;                struct ImColor;                     struct ImGuiIO;                     struct ImGuiOnceUponAFrame;         struct ImGuiStorage;                struct ImGuiStyle;                  struct ImGuiTextFilter;             struct ImGuiTextBuffer;             struct ImGuiTextEditCallbackData;   struct ImGuiSizeConstraintCallbackData; struct ImGuiListClipper;            struct ImGuiContext;
typedef unsigned int ImU32;         typedef unsigned int ImGuiID;       typedef unsigned short ImWchar;     typedef void* ImTextureID;          typedef int ImGuiCol;               typedef int ImGuiStyleVar;          typedef int ImGuiKey;               typedef int ImGuiAlign;             typedef int ImGuiColorEditFlags;    typedef int ImGuiMouseCursor;       typedef int ImGuiWindowFlags;       typedef int ImGuiSetCond;           typedef int ImGuiInputTextFlags;    typedef int ImGuiSelectableFlags;   typedef int ImGuiTreeNodeFlags;     typedef int(*ImGuiTextEditCallback)(ImGuiTextEditCallbackData *data);
typedef void(*ImGuiSizeConstraintCallback)(ImGuiSizeConstraintCallbackData* data);


struct ImVec2
{
	float x, y;
	ImVec2() { x = y = 0.0f; }
	ImVec2(float _x, float _y) { x = _x; y = _y; }
#ifdef IM_VEC2_CLASS_EXTRA          
	IM_VEC2_CLASS_EXTRA
#endif
};

struct ImVec4
{
	float x, y, z, w;
	ImVec4() { x = y = z = w = 0.0f; }
	ImVec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
#ifdef IM_VEC4_CLASS_EXTRA          
	IM_VEC4_CLASS_EXTRA
#endif
};

namespace ImGui
{
	IMGUI_API ImGuiIO&      GetIO();
	IMGUI_API ImGuiStyle&   GetStyle();
	IMGUI_API ImDrawData*   GetDrawData();                                  IMGUI_API void          NewFrame();                                     IMGUI_API void          Render();                                       IMGUI_API void          Shutdown();
	IMGUI_API void          ShowUserGuide();                                IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);        IMGUI_API void          ShowTestWindow(bool* p_open = NULL);            IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);
	IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);                                                       IMGUI_API bool          Begin(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha = -1.0f, ImGuiWindowFlags flags = 0);     IMGUI_API void          End();                                                                                                                          IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags extra_flags = 0);        IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags extra_flags = 0);                IMGUI_API void          EndChild();
	IMGUI_API ImVec2        GetContentRegionMax();                                                  IMGUI_API ImVec2        GetContentRegionAvail();                                                IMGUI_API float         GetContentRegionAvailWidth();                                           IMGUI_API ImVec2        GetWindowContentRegionMin();                                            IMGUI_API ImVec2        GetWindowContentRegionMax();                                            IMGUI_API float         GetWindowContentRegionWidth();                                          IMGUI_API ImDrawList*   GetWindowDrawList();                                                    IMGUI_API ImVec2        GetWindowPos();                                                         IMGUI_API ImVec2        GetWindowSize();                                                        IMGUI_API float         GetWindowWidth();
	IMGUI_API float         GetWindowHeight();
	IMGUI_API bool          IsWindowCollapsed();
	IMGUI_API void          SetWindowFontScale(float scale);
	IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiSetCond cond = 0);             IMGUI_API void          SetNextWindowPosCenter(ImGuiSetCond cond = 0);                          IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiSetCond cond = 0);           IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeConstraintCallback custom_callback = NULL, void* custom_callback_data = NULL);     IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);                           IMGUI_API void          SetNextWindowContentWidth(float width);                                 IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0);          IMGUI_API void          SetNextWindowFocus();                                                   IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiSetCond cond = 0);                 IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiSetCond cond = 0);               IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0);              IMGUI_API void          SetWindowFocus();                                                       IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiSetCond cond = 0);          IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiSetCond cond = 0);        IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiSetCond cond = 0);       IMGUI_API void          SetWindowFocus(const char* name);
	IMGUI_API float         GetScrollX();                                                           IMGUI_API float         GetScrollY();                                                           IMGUI_API float         GetScrollMaxX();                                                        IMGUI_API float         GetScrollMaxY();                                                        IMGUI_API void          SetScrollX(float scroll_x);                                             IMGUI_API void          SetScrollY(float scroll_y);                                             IMGUI_API void          SetScrollHere(float center_y_ratio = 0.5f);                             IMGUI_API void          SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f);            IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                                   IMGUI_API void          SetStateStorage(ImGuiStorage* tree);                                    IMGUI_API ImGuiStorage* GetStateStorage();

	IMGUI_API void          PushFont(ImFont* font);                                                 IMGUI_API void          PopFont();
	IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
	IMGUI_API void          PopStyleColor(int count = 1);
	IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);
	IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
	IMGUI_API void          PopStyleVar(int count = 1);
	IMGUI_API ImFont*       GetFont();                                                              IMGUI_API float         GetFontSize();                                                          IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                               IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);                      IMGUI_API ImU32         GetColorU32(const ImVec4& col);
	IMGUI_API void          PushItemWidth(float item_width);                                        IMGUI_API void          PopItemWidth();
	IMGUI_API float         CalcItemWidth();                                                        IMGUI_API void          PushTextWrapPos(float wrap_pos_x = 0.0f);                               IMGUI_API void          PopTextWrapPos();
	IMGUI_API void          PushAllowKeyboardFocus(bool v);                                         IMGUI_API void          PopAllowKeyboardFocus();
	IMGUI_API void          PushButtonRepeat(bool repeat);                                          IMGUI_API void          PopButtonRepeat();

	IMGUI_API void          Separator();                                                            IMGUI_API void          SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);                  IMGUI_API void          NewLine();                                                              IMGUI_API void          Spacing();                                                              IMGUI_API void          Dummy(const ImVec2& size);                                              IMGUI_API void          Indent(float indent_w = 0.0f);                                          IMGUI_API void          Unindent(float indent_w = 0.0f);                                        IMGUI_API void          BeginGroup();                                                           IMGUI_API void          EndGroup();
	IMGUI_API ImVec2        GetCursorPos();                                                         IMGUI_API float         GetCursorPosX();                                                        IMGUI_API float         GetCursorPosY();                                                        IMGUI_API void          SetCursorPos(const ImVec2& local_pos);                                  IMGUI_API void          SetCursorPosX(float x);                                                 IMGUI_API void          SetCursorPosY(float y);                                                 IMGUI_API ImVec2        GetCursorStartPos();                                                    IMGUI_API ImVec2        GetCursorScreenPos();                                                   IMGUI_API void          SetCursorScreenPos(const ImVec2& pos);                                  IMGUI_API void          AlignFirstTextHeightToWidgets();                                        IMGUI_API float         GetTextLineHeight();                                                    IMGUI_API float         GetTextLineHeightWithSpacing();                                         IMGUI_API float         GetItemsLineHeightWithSpacing();
	IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool border = true);      IMGUI_API void          NextColumn();                                                           IMGUI_API int           GetColumnIndex();                                                       IMGUI_API float         GetColumnOffset(int column_index = -1);                                 IMGUI_API void          SetColumnOffset(int column_index, float offset_x);                      IMGUI_API float         GetColumnWidth(int column_index = -1);                                  IMGUI_API int           GetColumnsCount();
	IMGUI_API void          PushID(const char* str_id);                                             IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);
	IMGUI_API void          PushID(const void* ptr_id);
	IMGUI_API void          PushID(int int_id);
	IMGUI_API void          PopID();
	IMGUI_API ImGuiID       GetID(const char* str_id);                                              IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
	IMGUI_API ImGuiID       GetID(const void* ptr_id);

	IMGUI_API void          Text(const char* fmt, ...) IM_PRINTFARGS(1);
	IMGUI_API void          TextV(const char* fmt, va_list args);
	IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...) IM_PRINTFARGS(2);      IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args);
	IMGUI_API void          TextDisabled(const char* fmt, ...) IM_PRINTFARGS(1);                        IMGUI_API void          TextDisabledV(const char* fmt, va_list args);
	IMGUI_API void          TextWrapped(const char* fmt, ...) IM_PRINTFARGS(1);                         IMGUI_API void          TextWrappedV(const char* fmt, va_list args);
	IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL);             IMGUI_API void          LabelText(const char* label, const char* fmt, ...) IM_PRINTFARGS(2);        IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args);
	IMGUI_API void          Bullet();                                                                   IMGUI_API void          BulletText(const char* fmt, ...) IM_PRINTFARGS(1);                          IMGUI_API void          BulletTextV(const char* fmt, va_list args);
	IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec2(0, 0));                IMGUI_API bool          SmallButton(const char* label);                                             IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& size);
	IMGUI_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	IMGUI_API bool          ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));        IMGUI_API bool          Checkbox(const char* label, bool* v);
	IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
	IMGUI_API bool          RadioButton(const char* label, bool active);
	IMGUI_API bool          RadioButton(const char* label, int* v, int v_button);
	IMGUI_API bool          Combo(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
	IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items = -1);          IMGUI_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
	IMGUI_API bool          ColorButton(const ImVec4& col, bool small_height = false, bool outline_border = true);
	IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);         IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0x01);      IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
	IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0x01);
	IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
	IMGUI_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
	IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
	IMGUI_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
	IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1, 0), const char* overlay = NULL);

	IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);         IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", const char* display_format_max = NULL, float power = 1.0f);
	IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");                                           IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
	IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
	IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
	IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f", const char* display_format_max = NULL);

	IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputFloat2(const char* label, float v[2], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputFloat3(const char* label, float v[3], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputFloat4(const char* label, float v[4], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags = 0);
	IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags = 0);

	IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);         IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f);
	IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format = "%.0f");
	IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* display_format = "%.0f");
	IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* display_format = "%.0f");
	IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* display_format = "%.0f");
	IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
	IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format = "%.0f");

	IMGUI_API bool          TreeNode(const char* label);                                                IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_PRINTFARGS(2);        IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_PRINTFARGS(2);        IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args);               IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args);               IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
	IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
	IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
	IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
	IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
	IMGUI_API void          TreePush(const char* str_id = NULL);                                        IMGUI_API void          TreePush(const void* ptr_id = NULL);                                        IMGUI_API void          TreePop();                                                                  IMGUI_API void          TreeAdvanceToLabelPos();                                                    IMGUI_API float         GetTreeNodeToLabelSpacing();                                                IMGUI_API void          SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond = 0);                   IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);          IMGUI_API bool          CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0);
	IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	IMGUI_API bool          ListBox(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
	IMGUI_API bool          ListBox(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
	IMGUI_API bool          ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0, 0));     IMGUI_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1);     IMGUI_API void          ListBoxFooter();
	IMGUI_API void          Value(const char* prefix, bool b);
	IMGUI_API void          Value(const char* prefix, int v);
	IMGUI_API void          Value(const char* prefix, unsigned int v);
	IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);
	IMGUI_API void          ValueColor(const char* prefix, const ImVec4& v);
	IMGUI_API void          ValueColor(const char* prefix, ImU32 v);

	IMGUI_API void          SetTooltip(const char* fmt, ...) IM_PRINTFARGS(1);                      IMGUI_API void          SetTooltipV(const char* fmt, va_list args);
	IMGUI_API void          BeginTooltip();                                                         IMGUI_API void          EndTooltip();

	IMGUI_API bool          BeginMainMenuBar();                                                     IMGUI_API void          EndMainMenuBar();
	IMGUI_API bool          BeginMenuBar();                                                         IMGUI_API void          EndMenuBar();
	IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                      IMGUI_API void          EndMenu();
	IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);      IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);
	IMGUI_API void          OpenPopup(const char* str_id);                                          IMGUI_API bool          BeginPopup(const char* str_id);                                         IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags extra_flags = 0);                   IMGUI_API bool          BeginPopupContextItem(const char* str_id, int mouse_button = 1);                                            IMGUI_API bool          BeginPopupContextWindow(bool also_over_items = true, const char* str_id = NULL, int mouse_button = 1);      IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, int mouse_button = 1);                                     IMGUI_API void          EndPopup();
	IMGUI_API void          CloseCurrentPopup();
	IMGUI_API void          LogToTTY(int max_depth = -1);                                           IMGUI_API void          LogToFile(int max_depth = -1, const char* filename = NULL);             IMGUI_API void          LogToClipboard(int max_depth = -1);                                     IMGUI_API void          LogFinish();                                                            IMGUI_API void          LogButtons();                                                           IMGUI_API void          LogText(const char* fmt, ...) IM_PRINTFARGS(1);
	IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
	IMGUI_API void          PopClipRect();

	IMGUI_API bool          IsItemHovered();                                                        IMGUI_API bool          IsItemHoveredRect();                                                    IMGUI_API bool          IsItemActive();                                                         IMGUI_API bool          IsItemClicked(int mouse_button = 0);                                    IMGUI_API bool          IsItemVisible();                                                        IMGUI_API bool          IsAnyItemHovered();
	IMGUI_API bool          IsAnyItemActive();
	IMGUI_API ImVec2        GetItemRectMin();                                                       IMGUI_API ImVec2        GetItemRectMax();                                                       IMGUI_API ImVec2        GetItemRectSize();                                                      IMGUI_API void          SetItemAllowOverlap();                                                  IMGUI_API bool          IsWindowHovered();                                                      IMGUI_API bool          IsWindowFocused();                                                      IMGUI_API bool          IsRootWindowFocused();                                                  IMGUI_API bool          IsRootWindowOrAnyChildFocused();                                        IMGUI_API bool          IsRootWindowOrAnyChildHovered();                                        IMGUI_API bool          IsRectVisible(const ImVec2& size);                                      IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);          IMGUI_API bool          IsPosHoveringAnyWindow(const ImVec2& pos);                              IMGUI_API float         GetTime();
	IMGUI_API int           GetFrameCount();
	IMGUI_API const char*   GetStyleColName(ImGuiCol idx);
	IMGUI_API ImVec2        CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = +0.0f);       IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
	IMGUI_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);
	IMGUI_API bool          BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags = 0);	    IMGUI_API void          EndChildFrame();

	IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
	IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
	IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
	IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

	IMGUI_API int           GetKeyIndex(ImGuiKey key);                                              IMGUI_API bool          IsKeyDown(int key_index);                                               IMGUI_API bool          IsKeyPressed(int key_index, bool repeat = true);                        IMGUI_API bool          IsKeyReleased(int key_index);                                           IMGUI_API bool          IsMouseDown(int button);                                                IMGUI_API bool          IsMouseClicked(int button, bool repeat = false);                        IMGUI_API bool          IsMouseDoubleClicked(int button);                                       IMGUI_API bool          IsMouseReleased(int button);                                            IMGUI_API bool          IsMouseHoveringWindow();                                                IMGUI_API bool          IsMouseHoveringAnyWindow();                                             IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);      IMGUI_API bool          IsMouseDragging(int button = 0, float lock_threshold = -1.0f);          IMGUI_API ImVec2        GetMousePos();                                                          IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                     IMGUI_API ImVec2        GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);        IMGUI_API void          ResetMouseDragDelta(int button = 0);                                    IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                    IMGUI_API void          SetMouseCursor(ImGuiMouseCursor type);                                  IMGUI_API void          CaptureKeyboardFromApp(bool capture = true);                            IMGUI_API void          CaptureMouseFromApp(bool capture = true);
	IMGUI_API void*         MemAlloc(size_t sz);
	IMGUI_API void          MemFree(void* ptr);
	IMGUI_API const char*   GetClipboardText();
	IMGUI_API void          SetClipboardText(const char* text);

	IMGUI_API const char*   GetVersion();
	IMGUI_API ImGuiContext* CreateContext(void* (*malloc_fn)(size_t) = NULL, void(*free_fn)(void*) = NULL);
	IMGUI_API void          DestroyContext(ImGuiContext* ctx);
	IMGUI_API ImGuiContext* GetCurrentContext();
	IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);


#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
	static inline bool      CollapsingHeader(const char* label, const char* str_id, bool framed = true, bool default_open = false) { (void)str_id; (void)framed; ImGuiTreeNodeFlags default_open_flags = 1 << 5; return CollapsingHeader(label, (default_open ? default_open_flags : 0)); }     static inline ImFont*   GetWindowFont() { return GetFont(); }                                  static inline float     GetWindowFontSize() { return GetFontSize(); }                          static inline void      SetScrollPosHere() { SetScrollHere(); }                                static inline bool      GetWindowCollapsed() { return ImGui::IsWindowCollapsed(); }            static inline bool      IsRectClipped(const ImVec2& size) { return !IsRectVisible(size); }
#endif

}
enum ImGuiWindowFlags_
{
	ImGuiWindowFlags_NoTitleBar = 1 << 0, ImGuiWindowFlags_NoResize = 1 << 1, ImGuiWindowFlags_NoMove = 1 << 2, ImGuiWindowFlags_NoScrollbar = 1 << 3, ImGuiWindowFlags_NoScrollWithMouse = 1 << 4, ImGuiWindowFlags_NoCollapse = 1 << 5, ImGuiWindowFlags_AlwaysAutoResize = 1 << 6, ImGuiWindowFlags_ShowBorders = 1 << 7, ImGuiWindowFlags_NoSavedSettings = 1 << 8, ImGuiWindowFlags_NoInputs = 1 << 9, ImGuiWindowFlags_MenuBar = 1 << 10, ImGuiWindowFlags_HorizontalScrollbar = 1 << 11, ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12, ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13, ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14, ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15, ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16, ImGuiWindowFlags_ChildWindow = 1 << 20, ImGuiWindowFlags_ChildWindowAutoFitX = 1 << 21, ImGuiWindowFlags_ChildWindowAutoFitY = 1 << 22, ImGuiWindowFlags_ComboBox = 1 << 23, ImGuiWindowFlags_Tooltip = 1 << 24, ImGuiWindowFlags_Popup = 1 << 25, ImGuiWindowFlags_Modal = 1 << 26, ImGuiWindowFlags_ChildMenu = 1 << 27
};

enum ImGuiInputTextFlags_
{
	ImGuiInputTextFlags_CharsDecimal = 1 << 0, ImGuiInputTextFlags_CharsHexadecimal = 1 << 1, ImGuiInputTextFlags_CharsUppercase = 1 << 2, ImGuiInputTextFlags_CharsNoBlank = 1 << 3, ImGuiInputTextFlags_AutoSelectAll = 1 << 4, ImGuiInputTextFlags_EnterReturnsTrue = 1 << 5, ImGuiInputTextFlags_CallbackCompletion = 1 << 6, ImGuiInputTextFlags_CallbackHistory = 1 << 7, ImGuiInputTextFlags_CallbackAlways = 1 << 8, ImGuiInputTextFlags_CallbackCharFilter = 1 << 9, ImGuiInputTextFlags_AllowTabInput = 1 << 10, ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11, ImGuiInputTextFlags_NoHorizontalScroll = 1 << 12, ImGuiInputTextFlags_AlwaysInsertMode = 1 << 13, ImGuiInputTextFlags_ReadOnly = 1 << 14, ImGuiInputTextFlags_Password = 1 << 15, ImGuiInputTextFlags_Multiline = 1 << 20
};

enum ImGuiTreeNodeFlags_
{
	ImGuiTreeNodeFlags_Selected = 1 << 0, ImGuiTreeNodeFlags_Framed = 1 << 1, ImGuiTreeNodeFlags_AllowOverlapMode = 1 << 2, ImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3, ImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4, ImGuiTreeNodeFlags_DefaultOpen = 1 << 5, ImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6, ImGuiTreeNodeFlags_OpenOnArrow = 1 << 7, ImGuiTreeNodeFlags_Leaf = 1 << 8, ImGuiTreeNodeFlags_Bullet = 1 << 9, ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};

enum ImGuiSelectableFlags_
{
	ImGuiSelectableFlags_DontClosePopups = 1 << 0, ImGuiSelectableFlags_SpanAllColumns = 1 << 1, ImGuiSelectableFlags_AllowDoubleClick = 1 << 2
};

enum ImGuiKey_
{
	ImGuiKey_Tab, ImGuiKey_LeftArrow, ImGuiKey_RightArrow, ImGuiKey_UpArrow, ImGuiKey_DownArrow, ImGuiKey_PageUp,
	ImGuiKey_PageDown,
	ImGuiKey_Home, ImGuiKey_End, ImGuiKey_Delete, ImGuiKey_Backspace, ImGuiKey_Enter, ImGuiKey_Escape, ImGuiKey_A, ImGuiKey_C, ImGuiKey_V, ImGuiKey_X, ImGuiKey_Y, ImGuiKey_Z, ImGuiKey_COUNT
};

enum ImGuiCol_
{
	ImGuiCol_Text,
	ImGuiCol_TextDisabled,
	ImGuiCol_WindowBg, ImGuiCol_ChildWindowBg, ImGuiCol_PopupBg, ImGuiCol_Border,
	ImGuiCol_BorderShadow,
	ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered,
	ImGuiCol_FrameBgActive,
	ImGuiCol_TitleBg,
	ImGuiCol_TitleBgCollapsed,
	ImGuiCol_TitleBgActive,
	ImGuiCol_MenuBarBg,
	ImGuiCol_ScrollbarBg,
	ImGuiCol_ScrollbarGrab,
	ImGuiCol_ScrollbarGrabHovered,
	ImGuiCol_ScrollbarGrabActive,
	ImGuiCol_ComboBg,
	ImGuiCol_CheckMark,
	ImGuiCol_SliderGrab,
	ImGuiCol_SliderGrabActive,
	ImGuiCol_Button,
	ImGuiCol_ButtonHovered,
	ImGuiCol_ButtonActive,
	ImGuiCol_Header,
	ImGuiCol_HeaderHovered,
	ImGuiCol_HeaderActive,
	ImGuiCol_Column,
	ImGuiCol_ColumnHovered,
	ImGuiCol_ColumnActive,
	ImGuiCol_ResizeGrip,
	ImGuiCol_ResizeGripHovered,
	ImGuiCol_ResizeGripActive,
	ImGuiCol_CloseButton,
	ImGuiCol_CloseButtonHovered,
	ImGuiCol_CloseButtonActive,
	ImGuiCol_PlotLines,
	ImGuiCol_PlotLinesHovered,
	ImGuiCol_PlotHistogram,
	ImGuiCol_PlotHistogramHovered,
	ImGuiCol_TextSelectedBg,
	ImGuiCol_ModalWindowDarkening, ImGuiCol_COUNT
};

enum ImGuiStyleVar_
{
	ImGuiStyleVar_Alpha, ImGuiStyleVar_WindowPadding, ImGuiStyleVar_WindowRounding, ImGuiStyleVar_WindowMinSize, ImGuiStyleVar_ChildWindowRounding, ImGuiStyleVar_FramePadding, ImGuiStyleVar_FrameRounding, ImGuiStyleVar_ItemSpacing, ImGuiStyleVar_ItemInnerSpacing, ImGuiStyleVar_IndentSpacing, ImGuiStyleVar_GrabMinSize
};

enum ImGuiAlign_
{
	ImGuiAlign_Left = 1 << 0,
	ImGuiAlign_Center = 1 << 1,
	ImGuiAlign_Right = 1 << 2,
	ImGuiAlign_Top = 1 << 3,
	ImGuiAlign_VCenter = 1 << 4,
	ImGuiAlign_Default = ImGuiAlign_Left | ImGuiAlign_Top
};

enum ImGuiColorEditFlags_
{
	ImGuiColorEditFlags_Alpha = 1 << 0, ImGuiColorEditFlags_RGB = 1 << 1, ImGuiColorEditFlags_HSV = 1 << 2,
	ImGuiColorEditFlags_HEX = 1 << 3,
	ImGuiColorEditFlags_NoPicker = 1 << 4, ImGuiColorEditFlags_NoOptions = 1 << 5, ImGuiColorEditFlags_NoColorSquare = 1 << 6, ImGuiColorEditFlags_NoSliders = 1 << 7, ImGuiColorEditFlags_ModeMask_ = ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX
};

enum ImGuiMouseCursor_
{
	ImGuiMouseCursor_Arrow = 0,
	ImGuiMouseCursor_TextInput, ImGuiMouseCursor_Move, ImGuiMouseCursor_ResizeNS, ImGuiMouseCursor_ResizeEW, ImGuiMouseCursor_ResizeNESW, ImGuiMouseCursor_ResizeNWSE, ImGuiMouseCursor_Count_
};

enum ImGuiSetCond_
{
	ImGuiSetCond_Always = 1 << 0, ImGuiSetCond_Once = 1 << 1, ImGuiSetCond_FirstUseEver = 1 << 2, ImGuiSetCond_Appearing = 1 << 3
};

struct ImGuiStyle
{
	float       Alpha;                          ImVec2      WindowPadding;                  ImVec2      WindowMinSize;                  float       WindowRounding;                 ImGuiAlign  WindowTitleAlign;               float       ChildWindowRounding;            ImVec2      FramePadding;                   float       FrameRounding;                  ImVec2      ItemSpacing;                    ImVec2      ItemInnerSpacing;               ImVec2      TouchExtraPadding;              float       IndentSpacing;                  float       ColumnsMinSpacing;              float       ScrollbarSize;                  float       ScrollbarRounding;              float       GrabMinSize;                    float       GrabRounding;                   ImVec2      DisplayWindowPadding;           ImVec2      DisplaySafeAreaPadding;         bool        AntiAliasedLines;               bool        AntiAliasedShapes;              float       CurveTessellationTol;           ImVec4      Colors[ImGuiCol_COUNT];

	IMGUI_API ImGuiStyle();
};

struct ImGuiIO
{

	ImVec2        DisplaySize;                  float         DeltaTime;                    float         IniSavingRate;                const char*   IniFilename;                  const char*   LogFilename;                  float         MouseDoubleClickTime;         float         MouseDoubleClickMaxDist;      float         MouseDragThreshold;           int           KeyMap[ImGuiKey_COUNT];       float         KeyRepeatDelay;               float         KeyRepeatRate;                void*         UserData;
	ImFontAtlas*  Fonts;                        float         FontGlobalScale;              bool          FontAllowUserScaling;         ImVec2        DisplayFramebufferScale;      ImVec2        DisplayVisibleMin;            ImVec2        DisplayVisibleMax;
	bool          OSXBehaviors;

	void(*RenderDrawListsFn)(ImDrawData* data);

	const char* (*GetClipboardTextFn)();
	void(*SetClipboardTextFn)(const char* text);

	void*       (*MemAllocFn)(size_t sz);
	void(*MemFreeFn)(void* ptr);

	void(*ImeSetInputScreenPosFn)(int x, int y);
	void*       ImeWindowHandle;

	ImVec2      MousePos;                       bool        MouseDown[5];                   float       MouseWheel;                     bool        MouseDrawCursor;                bool        KeyCtrl;                        bool        KeyShift;                       bool        KeyAlt;                         bool        KeySuper;                       bool        KeysDown[512];                  ImWchar     InputCharacters[16 + 1];
	IMGUI_API void AddInputCharacter(ImWchar c);                            IMGUI_API void AddInputCharactersUTF8(const char* utf8_chars);          inline void    ClearInputCharacters() { InputCharacters[0] = 0; }

	bool        WantCaptureMouse;               bool        WantCaptureKeyboard;            bool        WantTextInput;                  float       Framerate;                      int         MetricsAllocs;                  int         MetricsRenderVertices;          int         MetricsRenderIndices;           int         MetricsActiveWindows;

	ImVec2      MousePosPrev;                   ImVec2      MouseDelta;                     bool        MouseClicked[5];                ImVec2      MouseClickedPos[5];             float       MouseClickedTime[5];            bool        MouseDoubleClicked[5];          bool        MouseReleased[5];               bool        MouseDownOwned[5];              float       MouseDownDuration[5];           float       MouseDownDurationPrev[5];       float       MouseDragMaxDistanceSqr[5];     float       KeysDownDuration[512];          float       KeysDownDurationPrev[512];
	IMGUI_API   ImGuiIO();
};


template<typename T>
class ImVector
{
public:
	int                         Size;
	int                         Capacity;
	T*                          Data;

	typedef T                   value_type;
	typedef value_type*         iterator;
	typedef const value_type*   const_iterator;

	ImVector() { Size = Capacity = 0; Data = NULL; }
	~ImVector() { if (Data) ImGui::MemFree(Data); }

	inline bool                 empty() const { return Size == 0; }
	inline int                  size() const { return Size; }
	inline int                  capacity() const { return Capacity; }

	inline value_type&          operator[](int i) { IM_ASSERT(i < Size); return Data[i]; }
	inline const value_type&    operator[](int i) const { IM_ASSERT(i < Size); return Data[i]; }

	inline void                 clear() { if (Data) { Size = Capacity = 0; ImGui::MemFree(Data); Data = NULL; } }
	inline iterator             begin() { return Data; }
	inline const_iterator       begin() const { return Data; }
	inline iterator             end() { return Data + Size; }
	inline const_iterator       end() const { return Data + Size; }
	inline value_type&          front() { IM_ASSERT(Size > 0); return Data[0]; }
	inline const value_type&    front() const { IM_ASSERT(Size > 0); return Data[0]; }
	inline value_type&          back() { IM_ASSERT(Size > 0); return Data[Size - 1]; }
	inline const value_type&    back() const { IM_ASSERT(Size > 0); return Data[Size - 1]; }
	inline void                 swap(ImVector<T>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; value_type* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

	inline int                  _grow_capacity(int new_size) { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > new_size ? new_capacity : new_size; }

	inline void                 resize(int new_size) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
	inline void                 reserve(int new_capacity)
	{
		if (new_capacity <= Capacity) return;
		T* new_data = (value_type*)ImGui::MemAlloc((size_t)new_capacity * sizeof(value_type));
		if (Data)
			memcpy(new_data, Data, (size_t)Size * sizeof(value_type));
		ImGui::MemFree(Data);
		Data = new_data;
		Capacity = new_capacity;
	}

	inline void                 push_back(const value_type& v) { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); Data[Size++] = v; }
	inline void                 pop_back() { IM_ASSERT(Size > 0); Size--; }

	inline iterator             erase(const_iterator it) { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type)); Size--; return Data + off; }
	inline iterator             insert(const_iterator it, const value_type& v) { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(Capacity ? Capacity * 2 : 4); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type)); Data[off] = v; Size++; return Data + off; }
};

#define IMGUI_ONCE_UPON_A_FRAME    static ImGuiOnceUponAFrame imgui_oaf##__LINE__; if (imgui_oaf##__LINE__)
struct ImGuiOnceUponAFrame
{
	ImGuiOnceUponAFrame() { RefFrame = -1; }
	mutable int RefFrame;
	operator bool() const { int current_frame = ImGui::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};

struct ImGuiTextFilter
{
	struct TextRange
	{
		const char* b;
		const char* e;

		TextRange() { b = e = NULL; }
		TextRange(const char* _b, const char* _e) { b = _b; e = _e; }
		const char* begin() const { return b; }
		const char* end() const { return e; }
		bool empty() const { return b == e; }
		char front() const { return *b; }
		static bool is_blank(char c) { return c == ((char)(0x18f1 + 3564 - 0x26bd)) || c == '\t'; }
		void trim_blanks() { while (b < e && is_blank(*b)) b++; while (e > b && is_blank(*(e - 1))) e--; }
		IMGUI_API void split(char separator, ImVector<TextRange>& out);
	};

	char                InputBuf[256];
	ImVector<TextRange> Filters;
	int                 CountGrep;

	ImGuiTextFilter(const char* default_filter = "");
	~ImGuiTextFilter() {}
	void                Clear() { InputBuf[0] = 0; Build(); }
	bool                Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);        bool                PassFilter(const char* text, const char* text_end = NULL) const;
	bool                IsActive() const { return !Filters.empty(); }
	IMGUI_API void      Build();
};

struct ImGuiTextBuffer
{
	ImVector<char>      Buf;

	ImGuiTextBuffer() { Buf.push_back(0); }
	inline char         operator[](int i) { return Buf.Data[i]; }
	const char*         begin() const { return &Buf.front(); }
	const char*         end() const { return &Buf.back(); }          int                 size() const { return Buf.Size - 1; }
	bool                empty() { return Buf.Size <= 1; }
	void                clear() { Buf.clear(); Buf.push_back(0); }
	const char*         c_str() const { return Buf.Data; }
	IMGUI_API void      append(const char* fmt, ...) IM_PRINTFARGS(2);
	IMGUI_API void      appendv(const char* fmt, va_list args);
};

struct ImGuiStorage
{
	struct Pair
	{
		ImGuiID key;
		union { int val_i; float val_f; void* val_p; };
		Pair(ImGuiID _key, int _val_i) { key = _key; val_i = _val_i; }
		Pair(ImGuiID _key, float _val_f) { key = _key; val_f = _val_f; }
		Pair(ImGuiID _key, void* _val_p) { key = _key; val_p = _val_p; }
	};
	ImVector<Pair>      Data;

	IMGUI_API void      Clear();
	IMGUI_API int       GetInt(ImGuiID key, int default_val = 0) const;
	IMGUI_API void      SetInt(ImGuiID key, int val);
	IMGUI_API bool      GetBool(ImGuiID key, bool default_val = false) const;
	IMGUI_API void      SetBool(ImGuiID key, bool val);
	IMGUI_API float     GetFloat(ImGuiID key, float default_val = 0.0f) const;
	IMGUI_API void      SetFloat(ImGuiID key, float val);
	IMGUI_API void*     GetVoidPtr(ImGuiID key) const;     IMGUI_API void      SetVoidPtr(ImGuiID key, void* val);

	IMGUI_API int*      GetIntRef(ImGuiID key, int default_val = 0);
	IMGUI_API bool*     GetBoolRef(ImGuiID key, bool default_val = false);
	IMGUI_API float*    GetFloatRef(ImGuiID key, float default_val = 0.0f);
	IMGUI_API void**    GetVoidPtrRef(ImGuiID key, void* default_val = NULL);

	IMGUI_API void      SetAllInt(int val);
};

struct ImGuiTextEditCallbackData
{
	ImGuiInputTextFlags EventFlag;          ImGuiInputTextFlags Flags;              void*               UserData;           bool                ReadOnly;
	ImWchar             EventChar;
	ImGuiKey            EventKey;           char*               Buf;                int                 BufTextLen;         int                 BufSize;            bool                BufDirty;           int                 CursorPos;          int                 SelectionStart;     int                 SelectionEnd;
	void    DeleteChars(int pos, int bytes_count);
	void    InsertChars(int pos, const char* text, const char* text_end = NULL);
	bool    HasSelection() const { return SelectionStart != SelectionEnd; }
};

struct ImGuiSizeConstraintCallbackData
{
	void*   UserData;           ImVec2  Pos;                ImVec2  CurrentSize;        ImVec2  DesiredSize;
};

struct ImColor
{
	ImVec4              Value;

	ImColor() { Value.x = Value.y = Value.z = Value.w = 0.0f; }
	ImColor(int r, int g, int b, int a = 255) { float sc = 1.0f / 255.0f; Value.x = (float)r * sc; Value.y = (float)g * sc; Value.z = (float)b * sc; Value.w = (float)a * sc; }
	ImColor(ImU32 rgba) { float sc = 1.0f / 255.0f; Value.x = (float)(rgba & 0xff) * sc; Value.y = (float)((rgba >> 8) & 0xff) * sc; Value.z = (float)((rgba >> 16) & 0xff) * sc; Value.w = (float)(rgba >> 24) * sc; }
	ImColor(float r, float g, float b, float a = 1.0f) { Value.x = r; Value.y = g; Value.z = b; Value.w = a; }
	ImColor(const ImVec4& col) { Value = col; }
	inline operator ImU32() const { return ImGui::ColorConvertFloat4ToU32(Value); }
	inline operator ImVec4() const { return Value; }

	inline void    SetHSV(float h, float s, float v, float a = 1.0f) { ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }

	static ImColor HSV(float h, float s, float v, float a = 1.0f) { float r, g, b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b); return ImColor(r, g, b, a); }
};

struct ImGuiListClipper
{
	float   StartPosY;
	float   ItemsHeight;
	int     ItemsCount, StepNo, DisplayStart, DisplayEnd;

	ImGuiListClipper(int items_count = -1, float items_height = -1.0f) { Begin(items_count, items_height); }     ~ImGuiListClipper() { IM_ASSERT(ItemsCount == -1); }
	IMGUI_API bool Step();                                                  IMGUI_API void Begin(int items_count, float items_height = -1.0f);      IMGUI_API void End();
};


#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<24) | ((ImU32)(B)<<16) | ((ImU32)(G)<<8) | ((ImU32)(R)))
#define IM_COL32_WHITE       (0xffffffff)
#define IM_COL32_BLACK       (0xff000000)
#define IM_COL32_BLACK_TRANS (0x00000000)    

typedef void(*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);

struct ImDrawCmd
{
	unsigned int    ElemCount;                  ImVec4          ClipRect;                   ImTextureID     TextureId;                  ImDrawCallback  UserCallback;               void*           UserCallbackData;
	ImDrawCmd() { ElemCount = 0; ClipRect.x = ClipRect.y = -8192.0f; ClipRect.z = ClipRect.w = +8192.0f; TextureId = NULL; UserCallback = NULL; UserCallbackData = NULL; }
};

#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;
#endif

#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
	ImVec2  pos;
	ImVec2  uv;
	ImU32   col;
};
#else
IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

struct ImDrawChannel
{
	ImVector<ImDrawCmd>     CmdBuffer;
	ImVector<ImDrawIdx>     IdxBuffer;
};

struct ImDrawList
{
	ImVector<ImDrawCmd>     CmdBuffer;              ImVector<ImDrawIdx>     IdxBuffer;              ImVector<ImDrawVert>    VtxBuffer;
	const char*             _OwnerName;             unsigned int            _VtxCurrentIdx;         ImDrawVert*             _VtxWritePtr;           ImDrawIdx*              _IdxWritePtr;           ImVector<ImVec4>        _ClipRectStack;         ImVector<ImTextureID>   _TextureIdStack;        ImVector<ImVec2>        _Path;                  int                     _ChannelsCurrent;       int                     _ChannelsCount;         ImVector<ImDrawChannel> _Channels;
	ImDrawList() { _OwnerName = NULL; Clear(); }
	~ImDrawList() { ClearFreeMemory(); }
	IMGUI_API void  PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false);      IMGUI_API void  PushClipRectFullScreen();
	IMGUI_API void  PopClipRect();
	IMGUI_API void  PushTextureID(const ImTextureID& texture_id);
	IMGUI_API void  PopTextureID();

	IMGUI_API void  AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
	IMGUI_API void  AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners = 0x0f, float thickness = 1.0f);       IMGUI_API void  AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners = 0x0f);                         IMGUI_API void  AddRectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
	IMGUI_API void  AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
	IMGUI_API void  AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
	IMGUI_API void  AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
	IMGUI_API void  AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
	IMGUI_API void  AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
	IMGUI_API void  AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
	IMGUI_API void  AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
	IMGUI_API void  AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
	IMGUI_API void  AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), ImU32 col = 0xffffffff);
	IMGUI_API void  AddPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness, bool anti_aliased);
	IMGUI_API void  AddConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col, bool anti_aliased);
	IMGUI_API void  AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);

	inline    void  PathClear() { _Path.resize(0); }
	inline    void  PathLineTo(const ImVec2& pos) { _Path.push_back(pos); }
	inline    void  PathLineToMergeDuplicate(const ImVec2& pos) { if (_Path.Size == 0 || memcmp(&_Path[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
	inline    void  PathFill(ImU32 col) { AddConvexPolyFilled(_Path.Data, _Path.Size, col, true); PathClear(); }
	inline    void  PathStroke(ImU32 col, bool closed, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, closed, thickness, true); PathClear(); }
	IMGUI_API void  PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments = 10);
	IMGUI_API void  PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12);                     IMGUI_API void  PathBezierCurveTo(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, int num_segments = 0);
	IMGUI_API void  PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, int rounding_corners = 0x0f);

	IMGUI_API void  ChannelsSplit(int channels_count);
	IMGUI_API void  ChannelsMerge();
	IMGUI_API void  ChannelsSetCurrent(int channel_index);

	IMGUI_API void  AddCallback(ImDrawCallback callback, void* callback_data);      IMGUI_API void  AddDrawCmd();
	IMGUI_API void  Clear();
	IMGUI_API void  ClearFreeMemory();
	IMGUI_API void  PrimReserve(int idx_count, int vtx_count);
	IMGUI_API void  PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);          IMGUI_API void  PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
	IMGUI_API void  PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
	inline    void  PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col) { _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
	inline    void  PrimWriteIdx(ImDrawIdx idx) { *_IdxWritePtr = idx; _IdxWritePtr++; }
	inline    void  PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col) { PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); }
	IMGUI_API void  UpdateClipRect();
	IMGUI_API void  UpdateTextureID();
};

struct ImDrawData
{
	bool            Valid;                      ImDrawList**    CmdLists;
	int             CmdListsCount;
	int             TotalVtxCount;              int             TotalIdxCount;
	ImDrawData() { Valid = false; CmdLists = NULL; CmdListsCount = TotalVtxCount = TotalIdxCount = 0; }
	IMGUI_API void DeIndexAllBuffers();                   IMGUI_API void ScaleClipRects(const ImVec2& sc);
};

struct ImFontConfig
{
	void*           FontData;                       int             FontDataSize;                   bool            FontDataOwnedByAtlas;           int             FontNo;                         float           SizePixels;                     int             OversampleH, OversampleV;       bool            PixelSnapH;                     ImVec2          GlyphExtraSpacing;              const ImWchar*  GlyphRanges;                    bool            MergeMode;                      bool            MergeGlyphCenterV;
	char            Name[32];                                   ImFont*         DstFont;

	IMGUI_API ImFontConfig();
};

struct ImFontAtlas
{
	IMGUI_API ImFontAtlas();
	IMGUI_API ~ImFontAtlas();
	IMGUI_API ImFont*           AddFont(const ImFontConfig* font_cfg);
	IMGUI_API ImFont*           AddFontDefault(const ImFontConfig* font_cfg = NULL);
	IMGUI_API ImFont*           AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
	IMGUI_API ImFont*           AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);                                            IMGUI_API ImFont*           AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);      IMGUI_API ImFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);                  IMGUI_API void              ClearTexData();                 IMGUI_API void              ClearInputData();               IMGUI_API void              ClearFonts();                   IMGUI_API void              Clear();
	IMGUI_API void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);      IMGUI_API void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);      void                        SetTexID(void* id) { TexID = id; }

	IMGUI_API const ImWchar*    GetGlyphRangesDefault();        IMGUI_API const ImWchar*    GetGlyphRangesKorean();         IMGUI_API const ImWchar*    GetGlyphRangesJapanese();       IMGUI_API const ImWchar*    GetGlyphRangesChinese();        IMGUI_API const ImWchar*    GetGlyphRangesCyrillic();       IMGUI_API const ImWchar*    GetGlyphRangesThai();
	void*                       TexID;                  unsigned char*              TexPixelsAlpha8;        unsigned int*               TexPixelsRGBA32;        int                         TexWidth;               int                         TexHeight;              int                         TexDesiredWidth;        ImVec2                      TexUvWhitePixel;        ImVector<ImFont*>           Fonts;
	ImVector<ImFontConfig>      ConfigData;             IMGUI_API bool              Build();                IMGUI_API void              RenderCustomTexData(int pass, void* rects);
};

struct ImFont
{
	struct Glyph
	{
		ImWchar                 Codepoint;
		float                   XAdvance;
		float                   X0, Y0, X1, Y1;
		float                   U0, V0, U1, V1;
	};

	float                       FontSize;               float                       Scale;                  ImVec2                      DisplayOffset;          ImVector<Glyph>             Glyphs;                 ImVector<float>             IndexXAdvance;          ImVector<unsigned short>    IndexLookup;            const Glyph*                FallbackGlyph;          float                       FallbackXAdvance;       ImWchar                     FallbackChar;
	short                       ConfigDataCount;        ImFontConfig*               ConfigData;             ImFontAtlas*                ContainerAtlas;         float                       Ascent, Descent;
	IMGUI_API ImFont();
	IMGUI_API ~ImFont();
	IMGUI_API void              Clear();
	IMGUI_API void              BuildLookupTable();
	IMGUI_API const Glyph*      FindGlyph(ImWchar c) const;
	IMGUI_API void              SetFallbackChar(ImWchar c);
	float                       GetCharAdvance(ImWchar c) const { return ((int)c < IndexXAdvance.Size) ? IndexXAdvance[(int)c] : FallbackXAdvance; }
	bool                        IsLoaded() const { return ContainerAtlas != NULL; }

	IMGUI_API ImVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const;     IMGUI_API const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
	IMGUI_API void              RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const;
	IMGUI_API void              RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

	IMGUI_API void              GrowIndex(int new_size);
	IMGUI_API void              AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true);
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif

