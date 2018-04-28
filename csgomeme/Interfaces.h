#pragma once

#include "IBaseClientDll.h"
#include "Engine.h"
#include "IClientMode.h"
#include "InputSystem.h"
#include "Entity.h"
#include "Convar.h"
#include "GlobalVarsBase.h"
#include "Prediction.h"
#include "Movement.h"
#include "MDLCache.h"
#include "Physics.h"
#include "ModelInfo.h"
#include "StudioRender.h"
#include "MaterialSystem.h"
#include "ModelRender.h"
#include "RenderView.h"
#include "Effects.h"
#include "Localize.h"

namespace Interfaces
{
	extern IBaseClientDll*			Client;
	extern IEngineClient*			Engine;
	extern IClientMode*				ClientMode;
	extern CInput*					Input;
	extern ISurface*				Surface;
	extern IVPanel*					Panel;
	extern IClientEntityList*		EntityList;
	extern ICVar*					Cvar;
	extern IEngineTrace*			EngineTrace;
	extern IGlobalVarsBase*			GlobalVarsBase;
	extern IPrediction*				Prediction;
	extern CGameMovement*			GameMovement;
	extern CMoveHelper*				MoveHelper;
	extern IMDLCache*				MDLCache;
	extern IPhysicsSurfaceProps*	Physics;
	extern IVModelInfo*				ModelInfo;
	extern IStudioRender*			StudioRender;
	extern IMaterialSystem*			MaterialSystem;
	extern IVModelRender*			ModelRender;
	extern IVRenderView*			RenderView;
	extern CBaseClientState*		ClientState;
	extern IVEffects*				Effects;
	extern ILocalize*				Local;
}