#include "Features.h"
#include "Structs.h"
#include "Enums.h"
#include "Interfaces.h"
#include "Util.h"
#include "Definitions.h"
#include "Math.h"
#include "Settings.h"

CAutowall* g_pAutowall = new CAutowall;
float CAutowall::GetHitgroupDamageMultiplier(int HitGroup)
{
	switch (HitGroup)
	{
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		return 1.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	default:
		return 1.0f;
	}
}
void CAutowall::ScaleDamage(int HitGroup, CBaseEntity* Entity, float ArmorRatio, float &CurrentDamage)
{
	CurrentDamage *= GetHitgroupDamageMultiplier(HitGroup);

	if (!strstr(Entity->GetClientClass()->m_pNetworkName, "Chicken"))
	{
		if (Entity->GetArmor() > 0)
			if (HitGroup == HITGROUP_HEAD)
			{
				if (Entity->HasHelmet())
					CurrentDamage *= ArmorRatio * 0.5f;
			}
			else
				CurrentDamage *= ArmorRatio * 0.5f;
	}
}
bool CAutowall::TraceToExit(Vector &End, trace_t *TraceEnter, Vector Start, Vector Direcion, trace_t *TraceExit)
{
	float Distance = 0.0f;
	while (Distance <= 90.0f)
	{
		Distance += 4.0f;
		End = Start + Direcion * Distance;
		auto point_contents = Interfaces::EngineTrace->GetPointContents(End, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);
		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;
		auto new_end = End - (Direcion * 4.0f);
		Ray_t ray;
		ray.Init(End, new_end);
		Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT, 0, TraceExit);
		if (TraceExit->startsolid && TraceExit->surface.flags & SURF_HITBOX)
		{
			ray.Init(End, Start);
			CTraceFilter filter;
			filter.pSkip = TraceExit->m_pEnt;
			Interfaces::EngineTrace->TraceRay(ray, 0x600400B, &filter, TraceExit);
			if ((TraceExit->fraction < 1.0f || TraceExit->allsolid) && !TraceExit->startsolid)
			{
				End = TraceExit->endpos;
				return true;
			}
			continue;
		}
		if (!(TraceExit->fraction < 1.0 || TraceExit->allsolid || TraceExit->startsolid) || TraceExit->startsolid)
		{
			if (TraceExit->m_pEnt)
			{
				if (TraceEnter->m_pEnt && TraceEnter->m_pEnt == Interfaces::EntityList->GetClientEntity(0))
					return true;
			}
			continue;
		}
		if (TraceExit->surface.flags >> 7 & 1 && !(TraceEnter->surface.flags >> 7 & 1))
			continue;
		if (TraceExit->plane.normal.Dot(Direcion) <= 1.0f)
		{
			auto fraction = TraceExit->fraction * 4.0f;
			End -= (Direcion * fraction);
			return true;
		}
	}
	return false;
}
bool CAutowall::HandleBulletPenetration(WeaponInfo_t* weaponInfo, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = Interfaces::Physics->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((uint8_t*)enter_surface_data + 76);
	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= powf(weaponInfo->flRangeModifier, data.trace_length * 0.002f);
	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;
	if (data.penetrate_count <= 0)
		return false;
	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;
	surfacedata_t *exit_surface_data = Interfaces::Physics->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = *(float*)((uint8_t*)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;
	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}
	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponInfo->flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)
		return false;
	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f)
		return false;
	data.src = trace_exit.endpos;
	data.penetrate_count--;
	return true;
}
void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;
	Interfaces::EngineTrace->TraceRay(ray, mask, &filter, ptr);
}
bool CAutowall::SimulateFireBullet(CBaseCombatWeapon* pWeapon, FireBulletData &data)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	WeaponInfo_t* WeaponInfo = pWeapon->GetWeaponInfo();
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = (float)WeaponInfo->iDamage;
	while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
	{
		data.trace_length_remaining = WeaponInfo->flRange - data.trace_length;
		Vector end = data.src + data.direction * data.trace_length_remaining;
		TraceLine(data.src, end, MASK_SHOT, LocalPlayer, &data.enter_trace);
		Ray_t ray;
		ray.Init(data.src, end + data.direction * 40.f);
		Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT, &data.filter, &data.enter_trace);
		TraceLine(data.src, end + data.direction * 40.f, MASK_SHOT, LocalPlayer, &data.enter_trace);
		if (data.enter_trace.fraction == 1.0f)
			break;
		if (data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG && data.enter_trace.hitgroup > HITGROUP_GENERIC)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= powf(WeaponInfo->flRangeModifier, data.trace_length * 0.002f);
			CBaseEntity* player = (CBaseEntity*)data.enter_trace.m_pEnt;
			if (player->GetTeam() == LocalPlayer->GetTeam())
				return false;
			ScaleDamage(data.enter_trace.hitgroup, player, WeaponInfo->flArmorRatio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(WeaponInfo, data))
			break;
	}
	return false;
}
float CAutowall::GetDamage(const Vector& point)
{
	float damage = 0.f;
	Vector dst = point;
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	FireBulletData data;
	data.src = LocalPlayer->GetEyePosition();
	data.filter.pSkip = LocalPlayer;
	QAngle angles = g_Math.CalcAngle(data.src, dst);
	g_Math.AngleVectors(angles, data.direction);
	data.direction.NormalizeInPlace();
	CBaseCombatWeapon* pWeapon = LocalPlayer->GetWeapon();
	if (!pWeapon)
		return -1.0f;
	if (SimulateFireBullet(pWeapon, data))
		damage = data.current_damage;
	return damage;
}