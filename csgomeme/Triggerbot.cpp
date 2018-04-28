#include "Features.h"
#include "Interfaces.h"
#include "Entity.h"

#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"
#include "FindPattern.h"
#include <ctime>

static bool bShouldShoot = false;
static bool bFakeShot = false;
static CBaseEntity* oldEnt = nullptr;

void CTriggerbot::DoTriggerbot(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	CBaseCombatWeapon* LocalWeapon = LocalPlayer->GetWeapon();
	if (!LocalWeapon || LocalWeapon == nullptr)
		return;
	static CBaseCombatWeapon* OldWeapon;
	if (LocalPlayer->GetAlive())
		if (LocalWeapon->GetSpread() * 1000 >= Settings.Triggerbot.Hitchance && Settings.Triggerbot.HitchanceEnabled)
			return;

	static clock_t timeWhenShoot = clock();
	clock_t currentTime = clock();

	if (bFakeShot && g_pAimbot->CanShoot())
	{
		static int shotCounter = 0;
		if (LocalWeapon->GetWeaponInfo()->bFullAuto)
		{
			pCmd->buttons |= IN_ATTACK;
		}
		else
		{
			static bool bSwitch = false;

			if (bSwitch)
				pCmd->buttons |= IN_ATTACK;
			else
				pCmd->buttons &= ~IN_ATTACK;

			bSwitch = !bSwitch;
		}

		if (shotCounter <= Settings.Triggerbot.AfterBurst)
			shotCounter++;
		else
		{
			shotCounter = 0;
			bFakeShot = false;
		}
	}
	else if (Settings.Triggerbot.TriggerbotEnabled && LocalPlayer->GetHealth() > 0)
	{
		if (LocalWeapon->GetWeaponType() != WEAPONTYPE_C4
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_GRENADE
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_KNIFE
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_UNKNOWN)
		{
			CBaseEntity* tEnt = nullptr;
			if (Settings.Triggerbot.TriggerMagnet && ((Settings.Triggerbot.TriggerMode == 0) || (Settings.Triggerbot.TriggerMode == 1 && GetAsyncKeyState(Settings.Triggerbot.Key))))
			{
				Vector HitBox;
				int BestTarget = this->GetBestTargetT(pCmd, HitBox);
				if (BestTarget != -1)
				{
					CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(BestTarget);
					if ((GetAsyncKeyState(Settings.Triggerbot.Key) && Settings.Triggerbot.Key != 0)
						|| (Settings.Triggerbot.Key == 0))
					{
						HitBox.z += Settings.Triggerbot.AimHeight;
						QAngle Angle = g_Math.CalcAngle(LocalPlayer->GetEyePosition(), HitBox);
						Angle.Normalize();
						QAngle Difference = Angle - pCmd->viewangles;
						Difference.Normalize();
						Difference.y /= Settings.Aimbot.SmoothY;
						Difference.x /= Settings.Aimbot.SmoothX;
						Difference.Normalize();
						Angle = pCmd->viewangles + Difference;
						Angle.Normalize();

						pCmd->viewangles = Angle;
						if (!Settings.Triggerbot.SilentAim)
							Interfaces::Engine->SetViewAngles(Angle);
						tEnt = Entity;
					}
				}
			}
			if ((Settings.Triggerbot.TriggerMode == 0) || (Settings.Triggerbot.TriggerMode == 1 && GetAsyncKeyState(Settings.Triggerbot.Key)))
			{
				CBaseEntity* pEntity = nullptr;

				Vector vEnd;
				QAngle view = pCmd->viewangles;

				trace_t trace;
				Ray_t ray;

				CTraceFilter filter;
				filter.pSkip = LocalPlayer;

				g_Math.AngleVectors(view, vEnd);

				vEnd = vEnd * 8192.0f + LocalPlayer->GetEyePosition();

				ray.Init(LocalPlayer->GetEyePosition(), vEnd);
				Interfaces::EngineTrace->TraceRay(ray, CONTENTS_SOLID | CONTENTS_GRATE | CONTENTS_HITBOX, &filter, &trace);

				if (!trace.m_pEnt)
				{
					bShouldShoot = false;
					return;
				}
				if (trace.m_pEnt->GetTeam() == LocalPlayer->GetTeam() && !Settings.Triggerbot.AimAtTeam)
				{
					bShouldShoot = false;
					return;
				}
				if (trace.hitgroup <= 7 && trace.hitgroup > 0)
					pEntity = trace.m_pEnt;
				else
				{
					bShouldShoot = false;
					return;
				}

				//if (Settings.Triggerbot.TriggerMagnet && (tEnt == nullptr || pEntity != tEnt))
				//{
				//	bShouldShoot = false;
				//	return;
				//}


				Vector vecToTarget = trace.endpos - LocalPlayer->GetEyePosition();
				float maxdist = LocalPlayer->GetWeapon()->GetWeaponInfo()->flRange;
				bool bShoot = false;

				if (trace.hitgroup == HITGROUP_HEAD && Settings.Triggerbot.Head)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_CHEST && Settings.Triggerbot.Chest)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_STOMACH && Settings.Triggerbot.Stomach)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_RIGHTARM && Settings.Triggerbot.RightArm)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_LEFTARM && Settings.Triggerbot.LeftArm)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_RIGHTLEG && Settings.Triggerbot.RightLeg)
					bShoot = true;
				if (trace.hitgroup == HITGROUP_RIGHTLEG && Settings.Triggerbot.LeftLeg)
					bShoot = true;

				if (LocalPlayer->GetWeapon()->GetNextPrimaryAttack() < Interfaces::GlobalVarsBase->curtime)
				{
					if (g_pAimbot->CanShoot())
					{
						//pCmd->buttons |= IN_ATTACK;
						if (!bShouldShoot)
							timeWhenShoot = currentTime;

						bShouldShoot = true;
					}
					else
					{
						//pCmd->buttons &= ~IN_ATTACK;
						bShouldShoot = false;
					}
				}

				if (bShouldShoot)
				{
					if (currentTime - timeWhenShoot > Settings.Triggerbot.Delay)
					{
						pCmd->buttons |= IN_ATTACK;
						bShouldShoot = false;
						timeWhenShoot = currentTime;
						bFakeShot = true;
					}
				}
				else
				{
					pCmd->buttons &= ~IN_ATTACK;
				}

				oldEnt = pEntity;

			}
			if (!bShouldShoot)
				timeWhenShoot = currentTime;
		}
	}
	if (Settings.Aimbot.AntiUntrust)
		g_pAimbot->ClampAngles(pCmd);
}
CTriggerbot* g_pTriggerbot = new CTriggerbot;