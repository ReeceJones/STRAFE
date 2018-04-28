#include "Hooks.h"
#include "Interfaces.h"
#include "Enums.h"
#include "Settings.h"

DrawModelExecuteFn DrawModelExecute_Original;

IMaterial* Visible_Textured;
IMaterial* Visible_Flat;
IMaterial* Visible_WireFramed;
IMaterial* Visible_Bubble;

IMaterial* IgnoreZ_Textured;
IMaterial* IgnoreZ_Flat;
IMaterial* IgnoreZ_WireFramed;
IMaterial* IgnoreZ_Bubble;

IMaterial* Meme;

void ForceMaterial(IMaterial* material, Color color) {
	if (material != nullptr) {
		Interfaces::RenderView->SetColorModulation(color.Base());
		Interfaces::ModelRender->ForcedMaterialOverride(material);
	}
}

IMaterial* GetBestMaterial(bool IgnoreZ)
{
	if (IgnoreZ) {
		if (Settings.Visuals.IgnoreZStyle == CHAMSTYLE_TEXTURED)
			return IgnoreZ_Textured;
		else if (Settings.Visuals.IgnoreZStyle == CHAMSTYLE_FLAT)
			return IgnoreZ_Flat;
		else if (Settings.Visuals.IgnoreZStyle == CHAMSTYLE_WIREFRAME)
			return IgnoreZ_WireFramed;
		else if (Settings.Visuals.IgnoreZStyle == CHAMSTYLE_BUBBLE)
			return IgnoreZ_Bubble;
	}
	else {
		if (Settings.Visuals.VisualStyle == CHAMSTYLE_TEXTURED)
			return Visible_Textured;
		else if (Settings.Visuals.VisualStyle == CHAMSTYLE_FLAT)
			return Visible_Flat;
		else if (Settings.Visuals.VisualStyle == CHAMSTYLE_WIREFRAME)
			return Visible_WireFramed;
		else if (Settings.Visuals.IgnoreZStyle == CHAMSTYLE_BUBBLE)
			return Visible_Bubble;
	}
}

void __stdcall Hooks::DrawModelExecute_Hooked(void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld) {
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	HookManagers::ModelRender->UnHook();
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && LocalPlayer) {
		if (info.pModel) {
			std::string modelName = Interfaces::ModelInfo->GetModelName(info.pModel);

			if (modelName.find("models/player") != std::string::npos && Settings.Visuals.Chams && Settings.Visuals.Enabled) {
				CBaseEntity* ModelEntity = static_cast<CBaseEntity*>(Interfaces::EntityList->GetClientEntity(info.entity_index));
				if (ModelEntity && ModelEntity->GetAlive() && !ModelEntity->GetDormant() && (!Settings.Visuals.EnemyOnly  || 
					(Settings.Visuals.EnemyOnly && ModelEntity->GetTeam() != LocalPlayer->GetTeam()))) {
					Color RenderColorVisible;
					Color RenderColorIgnoreZ;

					switch (ModelEntity->GetTeam())
					{
					case Terrorists:
						RenderColorVisible = Color(Settings.Visuals.ChamsColorTerroristVisual[0] * 255,
							Settings.Visuals.ChamsColorTerroristVisual[1] * 255,
							Settings.Visuals.ChamsColorTerroristVisual[2] * 255);
						RenderColorIgnoreZ = Color(Settings.Visuals.ChamsColorTerroristIgnoreZ[0] * 255,
							Settings.Visuals.ChamsColorTerroristIgnoreZ[1] * 255,
							Settings.Visuals.ChamsColorTerroristIgnoreZ[2] * 255);
						break;
					case CounterTerrorists:
						RenderColorVisible = Color(Settings.Visuals.ChamsColorCounterTerroristVisual[0] * 255,
							Settings.Visuals.ChamsColorCounterTerroristVisual[1] * 255,
							Settings.Visuals.ChamsColorCounterTerroristVisual[2] * 255);
						RenderColorIgnoreZ = Color(Settings.Visuals.ChamsColorCounterTerroristIgnoreZ[0] * 255,
							Settings.Visuals.ChamsColorCounterTerroristIgnoreZ[1] * 255,
							Settings.Visuals.ChamsColorCounterTerroristIgnoreZ[2] * 255);
						break;
					}
					ForceMaterial(Visible_WireFramed, Color(255, 255, 255));

					if (Settings.Visuals.ChamsOutline)
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);


					ForceMaterial(GetBestMaterial(true), RenderColorIgnoreZ);
					Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
					
					ForceMaterial(GetBestMaterial(false), RenderColorVisible);
					Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
				}
			}
			else
			{
				Color VisibleRenderColorHands = Color(Settings.Visuals.HandsColorVisible[0] * 255.f, Settings.Visuals.HandsColorVisible[1] * 255.f, Settings.Visuals.HandsColorVisible[2] * 255.f);
				Color IgnoreZRenderColorHands = Color(Settings.Visuals.HandsColorIgnoreZ[0] * 255.f, Settings.Visuals.HandsColorIgnoreZ[1] * 255.f, Settings.Visuals.HandsColorIgnoreZ[2] * 255.f);

				Color VisibleRenderColorWeapon = Color(Settings.Visuals.WeaponColorVisible[0] * 255.f, Settings.Visuals.WeaponColorVisible[1] * 255.f, Settings.Visuals.WeaponColorVisible[2] * 255.f);

				float Basecolor[3] = { 1.f,1.f,1.f };


				if (modelName.find("arms") != std::string::npos && Settings.Visuals.ChamsHands)
				{
					switch (Settings.Visuals.HandsType)
					{
					case 1: //textured
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_Textured, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_Textured, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 2: //flat
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_Flat, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_Flat, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 3: //wireframe
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_WireFramed, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_WireFramed, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 4: //buble
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_Bubble, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_Bubble, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 5: //color
						if (Settings.Visuals.HandsIgnoreZ)
						{
							Interfaces::RenderView->SetColorModulation(IgnoreZRenderColorHands.Base());
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						Interfaces::RenderView->SetColorModulation(VisibleRenderColorHands.Base());
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 6:
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_Flat, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
							ForceMaterial(IgnoreZ_WireFramed, Color(255, 255, 255));
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_Flat, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						ForceMaterial(Visible_WireFramed, Color(0, 0, 0));
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 7:
						if (Settings.Visuals.HandsIgnoreZ)
						{
							ForceMaterial(IgnoreZ_Textured, IgnoreZRenderColorHands);
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
							ForceMaterial(IgnoreZ_WireFramed, Color(255, 255, 255));
							Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						}
						ForceMaterial(Visible_Textured, VisibleRenderColorHands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						ForceMaterial(Visible_WireFramed, Color(0, 0, 0));
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					case 8:
						Color Hands = { 255,255,255 };
						Meme->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
						ForceMaterial(Meme, Hands);
						Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
						break;
					}
				}
				else if (modelName.find("weapon") != std::string::npos && Settings.Visuals.ChamsWeapons)
				{
					if (!(modelName.find("arms") != std::string::npos))
					{
						if (!(modelName.find("scope") != std::string::npos))
						{
							switch (Settings.Visuals.WeaponsType)
							{
							default:
								Interfaces::RenderView->SetColorModulation(Basecolor);
								break;
							case 1:
								ForceMaterial(Visible_Textured, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 2:
								ForceMaterial(Visible_Flat, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 3:
								ForceMaterial(Visible_WireFramed, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 4:
								ForceMaterial(Visible_Bubble, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 5:
								Interfaces::RenderView->SetColorModulation(VisibleRenderColorWeapon.Base());
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 6:
								ForceMaterial(Visible_Flat, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								ForceMaterial(Visible_WireFramed, Color(0, 0, 0));
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 7:
								ForceMaterial(Visible_Textured, VisibleRenderColorWeapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								ForceMaterial(Visible_WireFramed, Color(0, 0, 0));
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							case 8:

								Color Weapon = { 255,255,255 };
								Meme->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
								ForceMaterial(Meme, Weapon);
								Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
								break;
							}

						}
					}
					else if (modelName.find("arms") != std::string::npos)
					{
						if (!Settings.Visuals.ChamsHands || Settings.Visuals.HandsType == 0)
							Interfaces::RenderView->SetColorModulation(Basecolor);
					}

				}

			}
		}
	}
	Interfaces::ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
	HookManagers::ModelRender->ReHook();
}