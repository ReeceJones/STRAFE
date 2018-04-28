#include "Interfaces.h"
#include "Hooks.h"
#include "Features.h"
#include "Settings.h"
#include "Util.h"
#include <vector>
#include <unordered_map>
#include "Definitions.h"

static std::unordered_map<std::string, bool> specs;
const std::string waterMark = "CSGOMeme V2 for CSGO";

inline double rad2deg(double d)
{
	return 57.2958 * d;
}

inline double deg2rad(double d)
{
	return 0.0174533 * d;
}

inline float GetScreenFov() 
{
	int width, height;
	Interfaces::Engine->GetScreenSize(width, height);
	return rad2deg(atan(tan(deg2rad(Interfaces::Engine->GetScreenAspectRatio(width, height) * 0.5)) * 0.75)) * 2;
}

PaintTraverseFn PaintTraverse_Original;
void __fastcall Hooks::PaintTraverse_Hooked(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{

	auto cName = Interfaces::Panel->GetName(vguiPanel);
	//if (!strcmp(cName, "HudZoom")) //Fullscreen Root Panel
	if (!strcmp(cName, "Fullscreen Root Panel"))
	{
		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && !Interfaces::Engine->IsTakingScreenshot())
		{	
			CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			Interfaces::Surface->DrawSetColor(Color(0, 0, 0));
			if (LocalPlayer && LocalPlayer != nullptr && Settings.Visuals.Enabled)
				g_pVisuals->DoVisuals_PaintTraverse();

			if (Settings.Misc.WaterMark)
			{
				static float rain = 0.f;
				Drawing.DrawString(ESPFont, 50, 50, Color::FromHSB(rain, 1.f, 1.f), FONT_LEFT, waterMark.c_str());

				if (rain >= 1.f)
					rain = 0.f;
				else
					rain += 0.001f;
			}

			if (Settings.Visuals.SpreadCrosshair)
			{
				if (LocalPlayer != nullptr && LocalPlayer->GetAlive())
				{
					auto RendCol = Color(Settings.Visuals.SpreadCrosshairColor[0] * 255.f, Settings.Visuals.SpreadCrosshairColor[1] * 255.f, Settings.Visuals.SpreadCrosshairColor[2] * 255.f);
					auto Punch = LocalPlayer->GetAimPunch() * 9;
					Interfaces::Surface->DrawSetColor(RendCol);
					int width, height;
					Interfaces::Engine->GetScreenSize(width, height);
					auto spread = LocalPlayer->GetWeapon()->GetSpread();
					auto innaccuracy = LocalPlayer->GetWeapon()->GetInnacuracy();
					printf("spread: %f; accuracy: %f;\n", spread, innaccuracy);
					Interfaces::Surface->DrawOutlinedCircle(width / 2 - Punch.y, height / 2 + Punch.x, (spread + innaccuracy) * 500, 100);
				}
			}

			if (g_pAimbot->m_iBestEnt != -1 && Settings.Aimbot.DrawAimspot)
			{
				Vector vBestPos = g_pAimbot->m_vBestPos;
				Vector vBestPosScr;
				if (g_pVisuals->WorldToScreen(vBestPos, vBestPosScr))
				{
					Drawing.DrawRect(vBestPosScr.x - 1, vBestPosScr.y - 1, 3, 3, Color(255, 255, 255));
				}
			}

			if (Settings.Aimbot.DrawAimbotFov || Settings.Triggerbot.DrawTriggerMagnetFov)
			{
				int width, height;
				Interfaces::Engine->GetScreenSize(width, height);
				float aimbotRadius = tan(deg2rad(Settings.Aimbot.Fov * 0.5) * 2) / GetScreenFov() * (width * 0.5);
				float magnetRadius = tan(deg2rad(Settings.Triggerbot.Fov * 0.5) * 2) / GetScreenFov() * (width * 0.5);

				if (aimbotRadius == magnetRadius && (Settings.Aimbot.Enabled && Settings.Triggerbot.TriggerbotEnabled && Settings.Triggerbot.TriggerMagnet))
				{
					Drawing.DrawCircle(width / 2, height / 2, aimbotRadius, 200, Color(0, 255, 0));
				}
				else
				{
					if (Settings.Aimbot.Enabled)
						Drawing.DrawCircle(width / 2, height / 2, aimbotRadius, 200, Color(255, 0, 0));
					if (Settings.Triggerbot.TriggerbotEnabled && Settings.Triggerbot.TriggerMagnet)
						Drawing.DrawCircle(width / 2, height / 2, magnetRadius, 200, Color(0, 0, 255));
				}
			}

			if (Settings.Misc.ShowSpectators)
			{
				specs.clear();
				const int XSpec = 75;
				const int YSpec = 300;
				for (int i = 1; i < 64; i++)
				{
					CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
					if (!pEntity)
						continue;
					if (pEntity->GetAlive())
						continue;
					if (pEntity->GetHealth() > 0)
						continue;
					if (pEntity == LocalPlayer)
						continue;
					if (pEntity->GetTeam() == LocalPlayer->GetTeam() && Settings.Misc.ShowSpecTeamOnly)
						continue;

					std::string strObserverName = pEntity->GetPlayerInfo().name;
					if (pEntity->GetObserverTarget() == LocalPlayer)
						specs[strObserverName] = true;
					else
						specs[strObserverName] = false;

				}
				for (auto it = specs.begin(); it != specs.end(); it++)//: specs)
				{
					int i = std::distance(specs.begin(), it);

					if (it->second == false)
						Drawing.DrawString(ESPFont, XSpec, YSpec + (15 * i), Color(255, 255, 255), FONT_LEFT, it->first.c_str());
					else
						Drawing.DrawString(ESPFont, XSpec, YSpec + (15 * i), Color(255, 0, 0), FONT_LEFT, it->first.c_str());
				}
			}

			if (Settings.Visuals.RecoilCrosshairType != 0)
			{
				int width, height;
				Interfaces::Engine->GetScreenSize(width, height);

				QAngle Punch = LocalPlayer->GetAimPunch() * 9;//9?

				int wBase, hBase;
				if (Settings.Visuals.FollowRecoil)
				{
					wBase = width / 2 - Punch.y;
					hBase = height / 2 + Punch.x;
				}
				else
				{
					wBase = width / 2;
					hBase = height / 2;
				}

				if (Settings.Visuals.RecoilCrosshairType == 1)
				{
					Drawing.DrawCircle(wBase, hBase, 1.f, 15.f, Color(255, 255, 255));
					Drawing.DrawCircle(wBase, hBase, 2.f, 15.f, Color(0, 0, 0));
				}
				else if (Settings.Visuals.RecoilCrosshairType == 2)
				{
					Drawing.DrawLine(wBase - 3, hBase - 3, wBase + 3, hBase + 3, Color(255, 255, 255));
					Drawing.DrawLine(wBase + 3, hBase - 3, wBase - 3, hBase + 3, Color(255, 255, 255));
				}
				else if (Settings.Visuals.RecoilCrosshairType == 3)
				{
					Interfaces::Surface->DrawSetColor(Color(255, 255, 255));
					Interfaces::Surface->DrawOutlinedRect((wBase) - 2, (hBase) - 2, (wBase) + 2, (hBase) + 2);
					Interfaces::Surface->DrawSetColor(Color(0, 0, 0));
					Interfaces::Surface->DrawOutlinedRect((wBase) - 3, (hBase) - 3, (wBase) + 3, (hBase) + 3);
				}
				else if (Settings.Visuals.RecoilCrosshairType == 4)
				{
					Drawing.DrawString(ESPFont, wBase, hBase, Color(255, 255, 255), FONT_CENTER, "+");
				}
				else if (Settings.Visuals.RecoilCrosshairType == 5)
				{
					Drawing.DrawString(ESPFont, wBase, hBase, Color(255, 255, 255), FONT_CENTER, "^");
				}
				else if (Settings.Visuals.RecoilCrosshairType == 6)
				{
					static float r = 0.f;
					Drawing.DrawLine(wBase, hBase, wBase + 8, hBase, Color::FromHSB(r, 1.f, 1.f));
					Drawing.DrawLine(wBase, hBase, wBase - 8, hBase, Color::FromHSB(r, 1.f, 1.f));
					Drawing.DrawLine(wBase, hBase, wBase, hBase + 8, Color::FromHSB(r, 1.f, 1.f));
					Drawing.DrawLine(wBase, hBase, wBase, hBase - 8, Color::FromHSB(r, 1.f, 1.f));

					Drawing.DrawLine(wBase + 8, hBase, wBase + 8, hBase + 8, Color::FromHSB(r, 1.f, 1.f));
					Drawing.DrawLine(wBase - 8, hBase, wBase - 8, hBase - 8, Color::FromHSB(r, 1.f, 1.f));

					Drawing.DrawLine(wBase, hBase + 8, wBase - 8, hBase + 8, Color::FromHSB(r, 1.f, 1.f));
					Drawing.DrawLine(wBase, hBase - 8, wBase + 8, hBase - 8, Color::FromHSB(r, 1.f, 1.f));
					if (r >= 1.f)
						r = 0.f;
					else
						r += 0.001f;
				}
			}
			
			if (LocalPlayer->GetScoped() && Settings.Visuals.NoScope)
			{
				int width, height;
				Interfaces::Engine->GetScreenSize(width, height);
				Drawing.DrawLine(width / 2, 0, width / 2, height, Color(0, 0, 0));
				Drawing.DrawLine(0, height / 2, width, height / 2, Color(0, 0, 0));
			}

		}
	}

	else if (!strcmp("HudZoom", Interfaces::Panel->GetName(vguiPanel)) && Settings.Visuals.NoScope)
		return; //return so that original is not called and scope is not drawn...

	PaintTraverse_Original(ecx, edx, vguiPanel, forceRepaint, allowForce);
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && !Interfaces::Engine->IsTakingScreenshot())
	{
		CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		//*(bool*)((DWORD)LocalPlayer + offsets.m_bScoped) = false;

	}
}