#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"

bool CTriggerbot::ValidTargetT(CBaseEntity* Entity)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!Entity || Entity == nullptr)
		return false;

	if (Entity->GetDormant())
		return false;
	if (Settings.Triggerbot.AimAtTeam)
	{
		if (Entity->GetTeam() == Hostage)
			return false;
	}
	else if (!Settings.Triggerbot.AimAtTeam)
	{
		if (Entity->GetTeam() == Hostage || Entity->GetTeam() == LocalPlayer->GetTeam())
			return false;
	}
	if (Entity->GetClientClass()->m_ClassID != 35)
		return false;
	if (Entity->GetHealth() <= 0)
		return false;
	if (Entity->GetGunGameImmunity())
		return false;

	return true;
}

int CTriggerbot::GetBestTargetT(CUserCmd* pCmd, Vector &vPosition)
{
	float Fov = Settings.Triggerbot.Fov;
	int iTempTarget = -1;
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	for (auto i = 0; i <= Interfaces::EntityList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(i);
		if (this->ValidTargetT(Entity))
		{
			QAngle qTempTarget = g_Math.CalcAngle(LocalPlayer->GetEyePosition(), Entity->GetEyePosition());
			Vector Position;
			bool Autowall;
			if (Settings.Triggerbot.ScanType == 0)
			{
				Position = Entity->GetBonePositionB(Settings.Triggerbot.Bone);
			}
			else
			{
				if (!g_pAimbot->GetBestHitboxT(Entity, Position))
					return -1;
				Autowall = true;
			}
			if (g_pAutowall->GetDamage(Position) >= Settings.Triggerbot.AutowallDamage)
					Autowall = true;
			QAngle LocalAngles;
			Interfaces::Engine->GetViewAngles(LocalAngles);
			float fTempFov = g_Math.GetFov(LocalAngles, qTempTarget);
			if (fTempFov < Fov && ((g_pAutowall->GetDamage(Position) >= Settings.Triggerbot.AutowallDamage && Autowall && Settings.Triggerbot.EntityChecks == AUTOWALL)
				|| this->ShouldShootT(Entity) && Settings.Triggerbot.EntityChecks == VISIBLE))
			{
				Fov = fTempFov;
				iTempTarget = i;
				vPosition = Position;
			}
		}
	}
	return iTempTarget;
}

bool CTriggerbot::ShouldShootT(CBaseEntity* Entity)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Ray_t ray;
	trace_t tr;
	ray.Init(LocalPlayer->GetEyePosition(), Entity->GetBonePositionB(Settings.Triggerbot.Bone));
	CTraceFilter filter;
	filter.pSkip = LocalPlayer;

	Interfaces::EngineTrace->TraceRay(ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, &filter, &tr);

	if (tr.m_pEnt == Entity && g_pAimbot->CanShoot() && ((Settings.Triggerbot.SmokeCheck && !g_pUtil->LineGoesThroughSmoke(LocalPlayer->GetEyePosition(), Entity->GetBonePositionB(Settings.Triggerbot.Bone))) || !Settings.Triggerbot.SmokeCheck))
		return true;

	return false;
}