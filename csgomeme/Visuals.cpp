#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "GlowManager.h"
#include "Offsets.h"
#include "FindPattern.h"
#include <algorithm>
#include "Collidable.h"

CVisuals* g_pVisuals = new CVisuals;

float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void CVisuals::DoVisuals_OverrideView()
{
	if (Settings.Visuals.ThirdPerson > 0.f)
		this->ThirdPerson();
	else
	{
		static QAngle vecAngles;
		Interfaces::Engine->GetViewAngles(vecAngles);
		Interfaces::Input->m_fCameraInThirdPerson = false;
		Interfaces::Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
	}
}

void CVisuals::DoVisuals_Createmove()
{
	if (Settings.Visuals.Glow)
		this->Glow();
}

void DrawDlight(CBaseEntity* Entity)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	Color EntityColorTerrorist = Color(Settings.Visuals.DLightColorTerrorist[0] * 255, Settings.Visuals.DLightColorTerrorist[1] * 255, Settings.Visuals.DLightColorTerrorist[2] * 255);
	Color EntityColorCounterTerrorist = Color(Settings.Visuals.DLightColorCounterTerrorist[0] * 255, Settings.Visuals.DLightColorCounterTerrorist[1] * 255, Settings.Visuals.DLightColorCounterTerrorist[2] * 255);
	Color RenderColor;

	switch (Entity->GetTeam())
	{
	case Terrorists:
		RenderColor = EntityColorTerrorist;
		break;
	case CounterTerrorists:
		RenderColor = EntityColorCounterTerrorist;
		break;
	}


	dlight_t* pDlight = Interfaces::Effects->CL_AllocDlight(Entity->GetIndex());
	pDlight->origin = Entity->GetOrigin();
	pDlight->radius = 100.f * Settings.Visuals.Radius;
	pDlight->color = RenderColor;
	pDlight->die = Interfaces::GlobalVarsBase->curtime + 0.05f;
	pDlight->decay = pDlight->radius / 5.0f;
	pDlight->key = Entity->GetIndex();
	dlight_t* pElight = Interfaces::Effects->CL_AllocElight(Entity->GetIndex());
	pElight->origin = Entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
	pElight->radius = 100.0f;
	pElight->color = RenderColor;
	pElight->die = Interfaces::GlobalVarsBase->curtime + 0.05f;
	pElight->decay = pElight->radius / 5.0f;
	pElight->key = Entity->GetIndex();
}

void CVisuals::DoVisuals_PaintTraverse()
{
	static int cancer = 0;
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	for (int i = 0; i < Interfaces::EntityList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(i);
		if (Entity == nullptr || !Entity->GetAlive() || Entity->GetDormant() || Entity->GetHealth() == 0)
			continue;
		if (Entity == LocalPlayer && Settings.Visuals.ThirdPerson == false)
			continue;
		if (Entity->GetTeam() == LocalPlayer->GetTeam() && Settings.Visuals.EnemyOnly)
			continue;

		this->TopX = 0;
		this->TopY = 0;
		this->BottomX = 0;
		this->BottomY = 0;
		Vector vHead = Entity->GetBonePositionB(8);
		Vector vOrigin = Entity->GetOrigin();
		vHead.x = vOrigin.x;
		Vector vHeadScreen, vOriginScreen;
		if (this->WorldToScreen(vOrigin, vOriginScreen) && this->WorldToScreen(vHead, vHeadScreen))
		{
			int EntityHeight = vOriginScreen.y - vHeadScreen.y;
			int EntityWidth = EntityHeight / Settings.Visuals.BoxWidth;
			this->TopX = vHeadScreen.x - (EntityWidth / 2);
			this->TopY = vHeadScreen.y - ((EntityHeight / 2.2) / 3);
			this->BottomX = vOriginScreen.x + (EntityWidth / 2);
			this->BottomY = vOriginScreen.y + 2;

			if (Settings.Visuals.DrawPlayerBoxes)
				this->DrawBox(Entity);
			if (Settings.Visuals.DrawName)
				this->DrawName(Entity);
			if (Settings.Visuals.DrawWeapon)
				this->DrawWeapon(Entity);
			if (Settings.Visuals.DrawHealthBar != 0)
				this->DrawHealthBar(Entity);
			if (Settings.Visuals.DrawArmorBar != 0)
				this->DrawArmorBar(Entity);
			if (Settings.Visuals.DrawRank)
				this->DrawRank(Entity);
			if (Settings.Visuals.ShowDefuseKit)
				this->DrawDefuser(Entity);
			if (Settings.Visuals.DrawPing)
				this->DrawPing(Entity);
			if (Settings.Visuals.ShowBomb)
				this->DrawHasBomb(Entity);
			if (Settings.Visuals.ShowDefusing)
				this->DrawIsDefusing(Entity);
			if (Settings.Visuals.DrawMoney)
				this->DrawMoney(Entity);
			if (Settings.Visuals.DrawWins)
				this->DrawWins(Entity);
			if (Settings.Visuals.Dlight)
				DrawDlight(Entity);
			if (Settings.Visuals.Sonar && (cancer % 50) == 0)
				Beep(2000 - Entity->GetOrigin().Dist(LocalPlayer->GetOrigin()), 1);
			if (Settings.Visuals.Barrel)
				this->DrawBarrel(Entity);
			if (Settings.Visuals.Distance)
				this->DrawDistance(Entity);
			if (Settings.Visuals.Location)
				this->DrawLocation(Entity);
			if (Settings.Visuals.Spam != 0)
				this->Callout(Entity);
			if (Settings.Visuals.Snaplines)
				this->DrawSnapLine(Entity);
			if (Settings.Visuals.DrawHelmet)
				this->DrawHelmet(Entity);
			if (Settings.Visuals.DrawHealth)
				this->DrawHealth(Entity);
			if (Settings.Visuals.DrawArmor)
				this->DrawArmor(Entity);
		}
	}
	this->DrawEntities();
	cancer++;
}

void CVisuals::DrawEntities()
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	for (int i = 0; i < Interfaces::EntityList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(i);
		if (!Entity || Entity == nullptr)
			continue;
		ICollideable* collision = Entity->GetCollideable();
		if (!collision)
			continue;

		Vector min = collision->OBBMins();
		Vector max = collision->OBBMaxs();

		Vector points[] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z) };

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++)
		{
			g_Math.VectorTransform(points[i], Entity->GetCoordinateFrame(), pointsTransformed[i]);
		}

		Vector pos = Entity->GetOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;


		if (!this->WorldToScreen(pointsTransformed[3], flb) || !this->WorldToScreen(pointsTransformed[5], brt)
			|| !this->WorldToScreen(pointsTransformed[0], blb) || !this->WorldToScreen(pointsTransformed[4], frt)
			|| !this->WorldToScreen(pointsTransformed[2], frb) || !this->WorldToScreen(pointsTransformed[1], brb)
			|| !this->WorldToScreen(pointsTransformed[6], blt) || !this->WorldToScreen(pointsTransformed[7], flt))
			continue;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		int left = (int)flb.x;        // left
		int top = (int)flb.y;        // top
		int right = (int)flb.x;    // right
		int bottom = (int)flb.y;    // bottom

		for (int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (top < arr[i].y)
				top = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}

		const char* className = Entity->GetClientClass()->m_pNetworkName;
		Vector vOrigin = Entity->GetOrigin();
		if (strstr(Entity->GetClientClass()->m_pNetworkName, "PlantedC4") && Settings.Visuals.ShowBombDetonation)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{

					CBasePlantedC4* pC4Entity = (CBasePlantedC4*)Entity;
					float flTime = pC4Entity->GetBombTime() - Interfaces::GlobalVarsBase->curtime;
					Color RendColor;
					if (flTime < 10 && flTime > 5)
						RendColor = Color(255, 255, 0);
					else if (flTime < 5)
						RendColor = Color(255, 0, 0);
					else
						RendColor = Color(255, 255, 255);


					Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
					Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(left, bottom, left, top, Color(0, 255, 255)); //actual box
					Drawing.DrawLine(left, top, right, top, Color(0, 255, 255));
					Drawing.DrawLine(right, top, right, bottom, Color(0, 255, 255));
					Drawing.DrawLine(right, bottom, left, bottom, Color(0, 255, 255));

					Drawing.DrawString(ESPFont, right + 2, bottom, RendColor, FONT_LEFT, "Bomb: %.1f", flTime); //bomb timer

					float flDistance = LocalPlayer->GetEyePosition().Dist(pC4Entity->GetOrigin());

					float a = 450.7f;
					float b = 75.68f;
					float c = 789.2f;
					float d = ((flDistance - b) / c);
					float flDamage = a*exp(-d * d);
#undef max //need to undefine max so we can use std::max
					int damage = std::max((int)ceilf(CSGO_Armor(flDamage, LocalPlayer->GetArmor())), 0);

					Drawing.DrawString(ESPFont, right + 2, bottom + 10, Color(255, 255, 255), FONT_LEFT, "Health: %i", LocalPlayer->GetHealth() - damage); //remaining hp after bomb explosion
					//bobm time = bomb->Gettime - globals->curtime

					Drawing.DrawString(ESPFont, right + 2, bottom + 20, Color(255, 0, 102), FONT_LEFT, "Bomb Planted"); //bomb planted notification
				}
			}
		}
		else if ((strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon") || strstr(Entity->GetClientClass()->m_pNetworkName, "CAK47")) && Settings.Visuals.ShowWeapons)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Entity;
					Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
					Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(left, bottom, left, top, Color(255, 255, 255)); //actual box
					Drawing.DrawLine(left, top, right, top, Color(255, 255, 255));
					Drawing.DrawLine(right, top, right, bottom, Color(255, 255, 255));
					Drawing.DrawLine(right, bottom, left, bottom, Color(255, 255, 255));
					Drawing.DrawString(0xa1, left - 2, bottom, Color(255, 255, 255), FONT_RIGHT, "%c", pWeapon->GetWeaponInfo()->m_pHudTexture_Weapon->charinFont);
					Drawing.DrawString(ESPFont, right + 3, bottom, Color(255, 255, 255), FONT_LEFT, pWeapon->GetWeaponName().c_str());
					Drawing.DrawString(ESPFont, right + 3, bottom + 10, Color(255, 255, 255), FONT_LEFT, "[ %i / %i ]", pWeapon->GetCurrentAmmo(), pWeapon->GetMaxAmmo());
				}
			}//	CChicken
			//	CFish
		}
		else if (strstr(Entity->GetClientClass()->m_pNetworkName, "Projectile") && Settings.Visuals.ShowGrenades)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					model_t* pModel = Entity->GetModel();
					if (!pModel)
						continue;
					studiohdr_t*  pHDR = Interfaces::ModelInfo->GetStudioModel(pModel);
					if (!pHDR)
						continue;

					const char* GrenadeName = pHDR->name;
					if (strstr(GrenadeName, "dropped") || strstr(GrenadeName, "thrown"))
					{
						const char* name = "";

						if (strstr(GrenadeName, "flashbang"))
							name = "Flashbang";
						else if (strstr(GrenadeName, "smoke"))
							name = "Smoke Grenade";
						else if (strstr(GrenadeName, "frag"))
							name = "HE Grenade";
						else if (strstr(GrenadeName, "decoy"))
							name = "Decoy";
						else if (strstr(GrenadeName, "molotov"))
							name = "Molotov";
						else if (strstr(GrenadeName, "inc"))
							name = "Incindiary Grenade";

						Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
						Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
						Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
						Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
						Drawing.DrawLine(left, bottom, left, top, Color(255, 255, 255)); //actual box
						Drawing.DrawLine(left, top, right, top, Color(255, 255, 255));
						Drawing.DrawLine(right, top, right, bottom, Color(255, 255, 255));
						Drawing.DrawLine(right, bottom, left, bottom, Color(255, 255, 255));

						Drawing.DrawString(ESPFont, right + 2, bottom, Color(255, 255, 255), FONT_LEFT , name);
					}
					//const char* GrenadeName = "NULL";
					//if (strstr(Entity->GetClientClass()->m_pNetworkName, "CFlashbang"))
					//	GrenadeName = "Flashbang";
					//else if (strstr(Entity->GetClientClass()->m_pNetworkName, "Smoke"))
					//	GrenadeName = "Smoke";
					//else if (strstr(Entity->GetClientClass()->m_pNetworkName, "Decoy"))
					//	GrenadeName = "Decoy";
					//else if (strstr(Entity->GetClientClass()->m_pNetworkName, "CHEGrenade"))
					//	GrenadeName = "HE Grenade";
					//else if (strstr(Entity->GetClientClass()->m_pNetworkName, "Molotov"))
					//	GrenadeName = "Molotov";
					//else if (strstr(Entity->GetClientClass()->m_pNetworkName, "Inc"))
					//	GrenadeName = "Incendiary Grenade";

				}
			}
		}
		else if (strstr(Entity->GetClientClass()->m_pNetworkName, "CChicken") && Settings.Visuals.ShowChickens)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
					Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(left, bottom, left, top, Color(0, 204, 102)); //actual box
					Drawing.DrawLine(left, top, right, top, Color(0, 204, 102));
					Drawing.DrawLine(right, top, right, bottom, Color(0, 204, 102));
					Drawing.DrawLine(right, bottom, left, bottom, Color(0, 204, 102));
					Drawing.DrawString(ESPFont, right + 2, bottom, Color(0, 204, 102), FONT_LEFT, "Chicken");
				}
			}
		}
		else if (strstr(Entity->GetClientClass()->m_pNetworkName, "CFish") && Settings.Visuals.ShowFish)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					Drawing.DrawString(ESPFont, vOriginScr.x, vOriginScr.y, Color(255, 51, 153), FONT_LEFT, "Fish"); //fish has no vec mins/max :(
				}
			}
		}
		else if (strstr(Entity->GetClientClass()->m_pNetworkName, "CBaseAnimating") && Settings.Visuals.ShowKits)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
					Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(left, bottom, left, top, Color(51, 102, 255)); //actual box
					Drawing.DrawLine(left, top, right, top, Color(51, 102, 255));
					Drawing.DrawLine(right, top, right, bottom, Color(51, 102, 255));
					Drawing.DrawLine(right, bottom, left, bottom, Color(51, 102, 255));
					Drawing.DrawString(ESPFont, right + 2, bottom, Color(51, 102, 255), FONT_LEFT, "Defuse Kit");
				}
			}
		}
		else if (strstr(Entity->GetClientClass()->m_pNetworkName, "C4") && Settings.Visuals.ShowBombDetonation)
		{
			Vector vOrigin = Entity->GetOrigin();
			if (vOrigin.x != 0 && vOrigin.y != 0 && vOrigin.z != 0)
			{
				Vector vOriginScr;
				if (WorldToScreen(vOrigin, vOriginScr))
				{
					Drawing.DrawLine(left - 1, bottom - 1, left - 1, top + 1, Color(0, 0, 0)); //outline
					Drawing.DrawLine(left - 1, top + 1, right + 1, top + 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, top + 1, right + 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(right + 1, bottom - 1, left - 1, bottom - 1, Color(0, 0, 0));
					Drawing.DrawLine(left, bottom, left, top, Color(255, 255, 0)); //actual box
					Drawing.DrawLine(left, top, right, top, Color(255, 255, 0));
					Drawing.DrawLine(right, top, right, bottom, Color(255, 255, 0));
					Drawing.DrawLine(right, bottom, left, bottom, Color(255, 255, 0));
					Drawing.DrawString(ESPFont, right + 2, bottom, Color(255, 255, 0), FONT_LEFT, "Bomb");
				}
			}
		}



	}
}