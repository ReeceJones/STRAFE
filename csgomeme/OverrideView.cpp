#include "Hooks.h"
#include "Interfaces.h"
#include "Features.h"
#include "Settings.h"

OverrideViewFn OverrideView_Original;

void __fastcall Hooks::OverrideView_Hooked(void* ecx, void* edx, CViewSetup* setup)
{

	CBaseEntity *LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsInGame() && LocalPlayer)
	{
		if (Settings.Visuals.Enabled)
			g_pVisuals->DoVisuals_OverrideView();
		if (!LocalPlayer->GetScoped())
			setup->fov = Settings.Visuals.RenderFov;
	}

	OverrideView_Original(ecx, setup);
}