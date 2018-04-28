
#include "Setup.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "FindPattern.h"
#include "Features.h"
#include "Config.h"
#include "SkinChanger.h"

CSetup* Setup = new CSetup;
HFont ESPFont;

namespace HookManagers
{
	VTableHook* ClientMode;
	VTableHook*	Client;
	VTableHook*	D3D;
	VTableHook*	MDLCache;
	VTableHook*	StudioRender;
	VTableHook*	Panel;
	VTableHook*	ModelRender;
	VTableHook* Surface;
	VTableHook*	Present;
}

void CSetup::Initialize()
{
	//g_pUtil->CreateConsoleWindow("CSGOMeme V2 - Counter Strike : Global Offensive");		/*Create a Console Window (mainly used for debugging purposes)*/
	std::cout << "Starting..." << '\n';
	this->SetupInterfaces();															/*Get Interfaces*/
	std::cout << "Interfaces Setup!" << '\n';
	NetvarManager->Init();																/*Get Netvar Manager Ready*/
	std::cout << "NetVars Initialized!" << '\n';
	pOffsets->GetOffsets();																/*Grab Offsets*/
	std::cout << "Offsets Grabbed!" << '\n';
	this->SetupHookManagers();															/*Create Hook Managers*/
	std::cout << "HookManagers Created!" << '\n';
	this->SetupHooks();																	/*Start Hooks*/
	std::cout << "Hooks Setup!" << '\n';
	this->SetupFonts();																	/*Setup Fonts*/
	std::cout << "Fonts Setup!" << '\n';
	this->SetupTextures();																/*Setup Texutures (Materials)*/
	std::cout << "Textures Setup!" << '\n';										
	g_pSkinChanger->GetSkins(k_Skins, k_Gloves);										/*Grab Skins*/
	std::cout << "Got Skins!" << '\n';
//	FreeConsole();																		/*Allow user to close console*/
}
void CSetup::SetupInterfaces()
{
	Interfaces::Client = reinterpret_cast<IBaseClientDll*>(g_pUtil->GetInterface("client.dll", "VClient"));
	Interfaces::Engine = reinterpret_cast<IEngineClient*>(g_pUtil->GetInterface("engine.dll", "VEngineClient"));
	Interfaces::ClientMode = **reinterpret_cast<IClientMode***>((*reinterpret_cast<DWORD**>(Interfaces::Client))[10] + 0x5);
	DWORD* ClientVMT = *reinterpret_cast<DWORD**>(Interfaces::Client);
	Interfaces::Input = *reinterpret_cast<CInput**>(ClientVMT[15] + 0x1);
	Interfaces::Surface = reinterpret_cast<ISurface*>(g_pUtil->GetInterface("vguimatsurface.dll", "VGUI_Surface"));
	Interfaces::Panel = reinterpret_cast<IVPanel*>(g_pUtil->GetInterface("vgui2.dll", "VGUI_Panel"));
	Interfaces::EntityList = reinterpret_cast<IClientEntityList*>(g_pUtil->GetInterface("client.dll", "VClientEntityList"));
	Interfaces::Cvar = reinterpret_cast<ICVar*>(g_pUtil->GetInterface("vstdlib.dll", "VEngineCvar"));
	Interfaces::EngineTrace = reinterpret_cast<IEngineTrace*>(g_pUtil->GetInterface("engine.dll", "EngineTraceClient"));
	Interfaces::GlobalVarsBase = **reinterpret_cast<IGlobalVarsBase***>((*reinterpret_cast<DWORD**>(Interfaces::Client))[0] + 0x1B);
	Interfaces::Prediction = reinterpret_cast<IPrediction*>(g_pUtil->GetInterface("client.dll", "VClientPrediction"));
	Interfaces::GameMovement = reinterpret_cast<CGameMovement*>(g_pUtil->GetInterface("client.dll", "GameMovement"));
	Interfaces::MoveHelper = **reinterpret_cast<CMoveHelper***>(FindPattern->IDA("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2);
	Window = FindWindowA("Valve001", "Counter-Strike: Global Offensive");
	Interfaces::MDLCache = reinterpret_cast<IMDLCache*>(g_pUtil->GetInterface("datacache.dll", "MDLCache"));
	Interfaces::Physics = reinterpret_cast<IPhysicsSurfaceProps*>(g_pUtil->GetInterface("vphysics.dll", "VPhysicsSurfaceProps"));
	Interfaces::ModelInfo = reinterpret_cast<IVModelInfo*>(g_pUtil->GetInterface("engine.dll", "VModelInfoClient"));
	Interfaces::StudioRender = reinterpret_cast<IStudioRender*>(g_pUtil->GetInterface("studiorender.dll", "VStudioRender"));
	Interfaces::MaterialSystem = reinterpret_cast<IMaterialSystem*>(g_pUtil->GetInterface("materialsystem.dll", "VMaterialSystem"));
	Interfaces::ModelRender = reinterpret_cast<IVModelRender*>(g_pUtil->GetInterface("engine.dll", "VEngineModel"));
	Interfaces::RenderView = reinterpret_cast<IVRenderView*>(g_pUtil->GetInterface("engine.dll", "VEngineRenderView"));
	Interfaces::ClientState = **reinterpret_cast<CBaseClientState***>((*reinterpret_cast<DWORD**>(Interfaces::Engine))[12] + 0x10);
	Interfaces::Effects = reinterpret_cast<IVEffects*>(g_pUtil->GetInterface("engine.dll", "VEngineEffects"));
	Interfaces::Local = reinterpret_cast<ILocalize*>(g_pUtil->GetInterface("localize.dll", "Localize_"));
	printf("Locaize: 0x%X\n", Interfaces::Local);
}

void CSetup::SetupHookManagers()
{
	HookManagers::ClientMode = new VTableHook((DWORD**)Interfaces::ClientMode, true);
	HookManagers::Client = new VTableHook((DWORD**)Interfaces::Client, true);
	HookManagers::D3D = new VTableHook((DWORD**)offsets.D3DDevice, true);
	HookManagers::MDLCache = new VTableHook((DWORD**)Interfaces::MDLCache, true);
	HookManagers::StudioRender = new VTableHook((DWORD**)Interfaces::StudioRender, true);
	HookManagers::Panel = new VTableHook((DWORD**)Interfaces::Panel, true);
	HookManagers::ModelRender = new VTableHook((DWORD**)Interfaces::ModelRender, true);
	HookManagers::Surface = new VTableHook((DWORD**)Interfaces::Surface, true);
	HookManagers::Present = new VTableHook((DWORD**)offsets.dwPresent, true);
}

void CSetup::SetupHooks()
{
	WndProc_Original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Window, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc_Hooked)));
	CreateMove_Original = reinterpret_cast<CreateMoveFn>(HookManagers::ClientMode->Hook(Hooks::CreateMove_Hooked, 24));
	Present_Original = reinterpret_cast<PresentFn>(HookManagers::Present->Hook(Hooks::PresentScene_Hooked, 0));
	ResetScene_Original = reinterpret_cast<tReset>(HookManagers::D3D->Hook(Hooks::Reset_Hooked, 16));
	GetViewModelFOV_Original = reinterpret_cast<GetViewModelFOVFn>(HookManagers::ClientMode->Hook(Hooks::GetViewModelFOV_Hooked, 35));
	FrameStageNotify_Original = reinterpret_cast<FrameStageNotifyFn>(HookManagers::Client->Hook(Hooks::FrameStageNotify_Hooked, 36));
	FindMDL_Original = reinterpret_cast<FindMDLFn>(HookManagers::MDLCache->Hook(Hooks::FindMDL_Hooked, 10));
	BeginFrame_Original = reinterpret_cast<BeginFrameFn>(HookManagers::StudioRender->Hook(Hooks::BeginFrame_Hooked, 10));
	PaintTraverse_Original = reinterpret_cast<PaintTraverseFn>(HookManagers::Panel->Hook(Hooks::PaintTraverse_Hooked, 41));
	OverrideView_Original = reinterpret_cast<OverrideViewFn>(HookManagers::ClientMode->Hook(Hooks::OverrideView_Hooked, 18));
	DrawModelExecute_Original = reinterpret_cast<DrawModelExecuteFn>(HookManagers::ModelRender->Hook(Hooks::DrawModelExecute_Hooked, 21));
	PlaySound_Original = reinterpret_cast<PlaySoundFn>(HookManagers::Surface->Hook(Hooks::PlaySound_Hooked, 82));
}

void CSetup::SetupFonts()
{
	Interfaces::Surface->SetFontGlyphSet(ESPFont = Interfaces::Surface->Create_Font(), "tahoma", 12, 600, NULL, NULL, FONTFLAG_DROPSHADOW/* FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE*/);//"tahoma", 12, 0, NULL, NULL, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
}

void CSetup::SetupTextures()
{//ignorez, nofog, model, nocull, halflambert, wireframe
	Visible_Textured = Interfaces::MaterialSystem->CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true, false, false);
	Visible_Flat = Interfaces::MaterialSystem->CreateMaterial("UnlitGeneric", "VGUI/white_additive", false, true, true, true, true, false, false);
	Visible_WireFramed = Interfaces::MaterialSystem->CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true, true, false);
	Visible_Bubble = Interfaces::MaterialSystem->CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true, false, true);

	IgnoreZ_Textured = Interfaces::MaterialSystem->CreateMaterial("VertexLitGeneric", "VGUI/white_additive", true, true, true, true, true, false, false);
	IgnoreZ_Flat = Interfaces::MaterialSystem->CreateMaterial("UnlitGeneric", "VGUI/white_additive", true, true, true, true, true, false, false);
	IgnoreZ_WireFramed = Interfaces::MaterialSystem->CreateMaterial("VertexLitGeneric", "VGUI/white_additive", true, true, true, true, true, true, false);
	IgnoreZ_Bubble = Interfaces::MaterialSystem->CreateMaterial("VertexLixGeneric", "VGUI/white_additive", true, true, true, true, true, false, true);

	Meme = Interfaces::MaterialSystem->CreateMaterial("VertexLixGeneric", "VGUI/white_additive", false, false, false, false, false, false, false);
}