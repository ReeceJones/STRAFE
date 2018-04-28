#include "Hooks.h"
#include "Interfaces.h"
#include "Definitions.h"
#include "Settings.h"
#include "Convar.h"
#include "Features.h"
#include "FindPattern.h"
#include "SpoofedConvar.h"
#include "Util.h"

bool SendPacket = true;
CreateMoveFn CreateMove_Original;
bool __stdcall Hooks::CreateMove_Hooked(float SampleTime, CUserCmd* UserCmd) {
	if (!UserCmd->command_number)
		return true;
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
	{
		CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (LocalPlayer)
		{
			CBaseCombatWeapon* LocalWeapon = LocalPlayer->GetWeapon();
			if (LocalWeapon)
			{
				SendPacket = true;

				g_pMisc->DoMisc_CreatemovePreAimbot(UserCmd, SendPacket);
				if (Settings.Aimbot.AntiAim && LocalPlayer->GetAlive())
					g_pAimbot->AntiAim(UserCmd, SendPacket);
				if (LocalPlayer->GetHealth() > 0 && & LocalPlayer != nullptr)
					g_pAimbot->DoAimbot(UserCmd, SendPacket);
				g_pMisc->DoMisc_CreatemovePostAimbot(UserCmd, SendPacket);
				if (Settings.Visuals.Enabled)
					g_pVisuals->DoVisuals_Createmove(); //move to isurface, just here for glow
				if (Settings.Triggerbot.TriggerbotEnabled)
					g_pTriggerbot->DoTriggerbot(UserCmd);

				if (LocalPlayer)
					if (LocalPlayer->GetAlive())
					{
						static ConVar* weapon_accuracy_nospread = Interfaces::Cvar->FindVar("weapon_accuracy_nospread");
						static SpoofedConvar* spoofed_weapon_accuracy_nospread = new SpoofedConvar(weapon_accuracy_nospread);
						if (Settings.Aimbot.NoSpread)
							spoofed_weapon_accuracy_nospread->SetBool(true);
						else
							spoofed_weapon_accuracy_nospread->SetBool(false);
					}

				g_pAimbot->ClampAngles(UserCmd);
				if (!LocalPlayer->GetAlive())
					SendPacket = true;
				PVOID pebp;
				__asm mov pebp, ebp;
				bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
				*pbSendPacket = SendPacket;
				if (SendPacket)
					Settings.Visuals.VisualAngle = UserCmd->viewangles;
			}
		}
	}

	return false;
}