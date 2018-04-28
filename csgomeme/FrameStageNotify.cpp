#include "Hooks.h"
#include "Interfaces.h"
#include "Definitions.h"
#include "Settings.h"
#include "Features.h"
#include "NetvarManager.h"
#include "FindPattern.h"
#include "PaintKit.h"

FrameStageNotifyFn FrameStageNotify_Original;
void __stdcall Hooks::FrameStageNotify_Hooked(ClientFrameStage_t CurrentStage)
{
	QAngle aimold;
	QAngle viewold;
	QAngle* aim = NULL;
	QAngle* view = NULL;
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (CurrentStage == FRAME_RENDER_START)
		{
			CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			if (LocalPlayer)
			{

				bool ThirdPerson = *reinterpret_cast<bool*>(reinterpret_cast<DWORD>(Interfaces::Input) + 0xa5);
				if (ThirdPerson)
				{
					*reinterpret_cast<QAngle*>(reinterpret_cast<DWORD>(LocalPlayer) + offsets.deadflag + 0x4) = Settings.Visuals.VisualAngle;
				}

				bool *PostProcessingOverride = *reinterpret_cast<bool**>(offsets.s_bOverridePostProcessing);
				if (PostProcessingOverride)
				{
					if (Settings.Visuals.DisablePP)
					{

						*PostProcessingOverride = true;
						//printf("%i\n", *PostProcessingOverride);
					}
					else
					{
						*PostProcessingOverride = false;
					}
				}
				//Interfaces::MaterialSystem->FirstMaterial();

				if (Settings.Misc.ChangeWallColor)
				{
					for (unsigned short i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
					{
						IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);


						if (!pMaterial)
							continue;

						if (Settings.Misc.NoSky) //No Sky
						{
							if (strstr(pMaterial->GetName(), "skybox"))
							{
								pMaterial->ColorModulate(0.f, 0.f, 0.f);
							}
						}
						else
						{
							if (strstr(pMaterial->GetName(), "skybox"))
							{
								pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
							}
						}

						if (Settings.Misc.ChangeWallColor)
						{
							if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
							{
								if (!Settings.Misc.RainbowWalls)
								{
									pMaterial->ColorModulate(Settings.Misc.WallColor[0], Settings.Misc.WallColor[1], Settings.Misc.WallColor[2]);
								}
								else
								{
									static float r = 0.f;
									Color c = Color::FromHSB(r, 1.f, 1.f);
									pMaterial->ColorModulate(c.r() / 255.f, c.g() / 255.f, c.b() / 255.f);
									r += 0.00001f;
									if (r >= 1.f)
										r = 0.f;
								}
								pMaterial->AlphaModulate(Settings.Misc.WallAlpha);
							}
						}
						else
						{
							pMaterial->ColorModulate(1.f, 1.f, 1.f);
							pMaterial->AlphaModulate(1.f);
						}
					}
				}


				if (LocalPlayer->GetHealth() > 0)
				{
					if (Settings.Visuals.NoVisRecoil)
					{
						aim = reinterpret_cast<QAngle*>(DWORD(LocalPlayer) + offsets.m_aimPunchAngle);
						view = reinterpret_cast<QAngle*>(DWORD(LocalPlayer) + offsets.m_viewPunchAngle);
						aimold = *aim;
						viewold = *view;
						*aim = *aim * Settings.Visuals.AimPunchFactor;
						*view = QAngle(0, 0, 0);
					}
				}
			}
		}
	}
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (CurrentStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (Settings.Aimbot.Resolver)
			{
				for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
				{
					CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(i);
					CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (!Entity
						|| Entity == LocalPlayer
						|| Entity->GetDormant()
						|| !Entity->GetAlive()
						|| Entity->GetTeam() == LocalPlayer->GetTeam())
						continue;
					Entity->GetEyeAngles()->y = Entity->GetLowerBodyYawTarget(); //Entity->GetHeadRotation()->y
				}
			}
		}
	}

	FrameStageNotify_Original(CurrentStage);
	if (aim && view && Settings.Visuals.NoVisRecoil)
	{
		*aim = aimold;
		*view = viewold;
	}
}