#pragma once
#include "IClientMode.h"
#include "InputSystem.h"
#include "VTableHook.h"
#include <d3d9.h>
#include "Enums.h"
#include <Windows.h>

typedef unsigned short MDLHandle_t;

typedef bool(__thiscall* CreateMoveFn)(IClientMode*, float, CUserCmd*);
extern CreateMoveFn CreateMove_Original;

typedef long(__stdcall* tReset)(IDirect3DDevice9* dev, D3DPRESENT_PARAMETERS* pp);
extern tReset ResetScene_Original;

//typedef long(__stdcall* EndSceneFn)(IDirect3DDevice9* device);
//extern EndSceneFn EndScene_Original;

typedef float(__stdcall* GetViewModelFOVFn)();
extern GetViewModelFOVFn GetViewModelFOV_Original;

typedef void(__stdcall* FrameStageNotifyFn)(ClientFrameStage_t);
extern FrameStageNotifyFn FrameStageNotify_Original;

typedef MDLHandle_t(__thiscall* FindMDLFn)(void* ecx, const char* ptr);
extern FindMDLFn FindMDL_Original;

typedef void(__stdcall* BeginFrameFn)();
extern BeginFrameFn BeginFrame_Original;

typedef void(_fastcall* PaintTraverseFn)(void*, void*, unsigned int, bool, bool);
extern PaintTraverseFn PaintTraverse_Original;

typedef void(__thiscall *OverrideViewFn)(void* _this, CViewSetup* setup);
extern OverrideViewFn OverrideView_Original;

typedef void(__stdcall* DrawModelExecuteFn)(void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
extern DrawModelExecuteFn DrawModelExecute_Original;

typedef void(__thiscall* PlaySoundFn)(ISurface*, const char*);
extern PlaySoundFn PlaySound_Original;

typedef bool(_cdecl*MsgFunc_ServerRankRevealAllFn)(float*);
extern MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAllFn_Original;

typedef long(__stdcall* PresentFn)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
extern PresentFn Present_Original;

extern WNDPROC WndProc_Original;
extern HWND Window;
namespace HookManagers
{
	extern VTableHook* 	ClientMode;
	extern VTableHook*	Client;
	extern VTableHook*	D3D;
	extern VTableHook*	MDLCache;
	extern VTableHook*	StudioRender;
	extern VTableHook*	Panel;
	extern VTableHook*	ModelRender;
	extern VTableHook*	Surface;
	extern VTableHook*	Present;
}

namespace Hooks
{
	extern bool __stdcall CreateMove_Hooked(float sample_input_frametime, CUserCmd* pCmd);
	extern long __stdcall PresentScene_Hooked(IDirect3DDevice9* pDevice, const RECT* pSourceRec, const RECT* pDestRect, HWND hWnd, const RGNDATA* pDirtyRegion);
	extern HRESULT __stdcall Reset_Hooked(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam);
	extern LRESULT __stdcall WndProc_Hooked(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern float __stdcall GetViewModelFOV_Hooked();
	extern void __stdcall FrameStageNotify_Hooked(ClientFrameStage_t CurrentStage);
	extern MDLHandle_t __fastcall FindMDL_Hooked(void* ecx, void* edx, const char* pMDLRelativePath);
	extern void __stdcall BeginFrame_Hooked();
	extern void __fastcall PaintTraverse_Hooked(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	extern void __fastcall OverrideView_Hooked(void* ecx, void* edx, CViewSetup* setup);
	extern void __stdcall DrawModelExecute_Hooked(void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);
	extern void __stdcall PlaySound_Hooked(const char* szFileName);
}