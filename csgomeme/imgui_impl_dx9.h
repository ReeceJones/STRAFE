
struct IDirect3DDevice9;

IMGUI_API bool ImGui_ImplDX9_Init(void* hwnd, IDirect3DDevice9* device);
IMGUI_API void ImGui_ImplDX9_Shutdown();
IMGUI_API void ImGui_ImplDX9_NewFrame();

IMGUI_API void ImGui_ImplDX9_InvalidateDeviceObjects();
IMGUI_API bool ImGui_ImplDX9_CreateDeviceObjects();




