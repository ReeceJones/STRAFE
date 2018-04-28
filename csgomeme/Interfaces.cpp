#include "Interfaces.h"
namespace Interfaces
{
	IBaseClientDll*				Client;
	IEngineClient*				Engine;
	IClientMode*				ClientMode;
	CInput*						Input;
	ISurface*					Surface;
	IVPanel*					Panel;
	IClientEntityList*			EntityList;
	ICVar*						Cvar;
	IEngineTrace*				EngineTrace;
	IGlobalVarsBase*			GlobalVarsBase;
	IPrediction*				Prediction;
	CGameMovement*				GameMovement;
	CMoveHelper*				MoveHelper;
	IMDLCache*					MDLCache;
	IPhysicsSurfaceProps*		Physics;
	IVModelInfo*				ModelInfo;
	IStudioRender*				StudioRender;
	IMaterialSystem*			MaterialSystem;
	IVModelRender*				ModelRender;
	IVRenderView*				RenderView;
	CBaseClientState*			ClientState;
	IVEffects*					Effects;
	ILocalize*					Local;
}