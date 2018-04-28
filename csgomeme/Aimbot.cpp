#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"
#include "FindPattern.h"

QAngle m_oldangle;
float m_oldforward;
float m_oldsidemove;

void Autocock(CBaseEntity *local, CBaseCombatWeapon *weapon, CUserCmd *pCmd, bool shoot)
{
	if (!weapon || weapon == nullptr)
		return;
	if (*weapon->GetItemDefinitionIndex() != WEAPON_REVOLVER)
		return;

	float readytime = *(float*)((DWORD)weapon + offsets.m_flPostponeFireReadyTime);
	int time = TIME_TO_TICKS(Interfaces::GlobalVarsBase->curtime - readytime); //can shoot if > -1
	if (time == -4 && !shoot)
		pCmd->buttons &= ~IN_ATTACK;
	else
		pCmd->buttons |= IN_ATTACK;
}

void Start(CUserCmd* pCmd)
{
	m_oldangle = pCmd->viewangles;
	m_oldforward = pCmd->forwardmove;
	m_oldsidemove = pCmd->sidemove;
}

void End(CUserCmd* pCmd)
{//pitch fix plox
	float yaw_delta = pCmd->viewangles.y - m_oldangle.y;
	float f1;
	float f2;

	if (m_oldangle.y < 0.f)
		f1 = 360.0f + m_oldangle.y;
	else
		f1 = m_oldangle.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	pCmd->forwardmove = cos(DEG2RAD(yaw_delta)) * m_oldforward + cos(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
	pCmd->sidemove = sin(DEG2RAD(yaw_delta)) * m_oldforward + sin(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
}

void CAimbot::DoAimbot(CUserCmd* pCmd, bool &bSendPacket)
{
	bool isRCSinAimbot = false;
	this->isAimbotting = false;
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	CBaseCombatWeapon* LocalWeapon = LocalPlayer->GetWeapon();
	if (!LocalWeapon || LocalWeapon == nullptr)
		return;
	if (!LocalPlayer || LocalPlayer == nullptr)
		return;


	if (Settings.Aimbot.AutoRevolver)
		Autocock(LocalPlayer, LocalPlayer->GetWeapon(), pCmd, false);

	static CBaseCombatWeapon* OldWeapon;
	if (LocalWeapon->GetWeaponType() != WEAPONTYPE_C4
		&& LocalWeapon->GetWeaponType() != WEAPONTYPE_GRENADE
		&& LocalWeapon->GetWeaponType() != WEAPONTYPE_KNIFE
		&& LocalWeapon->GetWeaponType() != WEAPONTYPE_UNKNOWN)
	{
		Vector HitBox;
		this->m_iBestEnt = this->GetBestTarget(pCmd, HitBox);
		if (LocalPlayer->GetAlive()
			&& this->m_iBestEnt != -1)
		{
			CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(this->m_iBestEnt);
			Vector Pos;
			if ((Settings.Aimbot.AimbotMode == 0)
				|| (this->GetBestHitbox(Entity, Pos) && Settings.Aimbot.AimbotMode == 1))
			{
				if (Settings.Aimbot.AutoStop)
					this->Stop(pCmd);
				if (Settings.Aimbot.AutoCrouch)
					this->Crouch(pCmd);
				if (Settings.Aimbot.AutoScope && !LocalPlayer->GetScoped() && (Entity->GetWeapon()->GetWeaponType() == WEAPONTYPE_SNIPER_RIFLE || Entity->GetWeapon()->GetWeaponID() == WEAPON_AUG || 
					Entity->GetWeapon()->GetWeaponID() == WEAPON_SG556 || 
					Entity->GetWeapon()->GetWeaponID() == WEAPON_SCAR20 || 
					Entity->GetWeapon()->GetWeaponID() == WEAPON_G3SG1))
					pCmd->buttons |= IN_ATTACK2;
				if (Settings.Aimbot.AutoRevolver)
					Autocock(LocalPlayer, LocalPlayer->GetWeapon(), pCmd, true);
			}
		}
	}
	if (LocalPlayer->GetAlive())
		if (LocalWeapon->GetInnacuracy() * 1000 >= Settings.Aimbot.Hitchance && Settings.Aimbot.HitchanceEnabled)
			return;
	Start(pCmd);
	if (Settings.Aimbot.Enabled && LocalPlayer->GetHealth() > 0)
	{
		if (LocalWeapon->GetWeaponType() != WEAPONTYPE_C4
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_GRENADE
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_KNIFE
			&& LocalWeapon->GetWeaponType() != WEAPONTYPE_UNKNOWN)
		{
			if (Settings.Aimbot.EnginePrediction)
				g_pAimbot->EnginePrediction(pCmd);
			Vector HitBox;
			int BestTarget = this->GetBestTarget(pCmd, HitBox);
			this->m_vBestPos = HitBox;
			if (BestTarget != -1)
			{
				CBaseEntity* Entity = Interfaces::EntityList->GetClientEntity(BestTarget);
				if (Settings.Aimbot.LagCompensation)
					pCmd->tick_count = TIME_TO_TICKS(Entity->GetSimulationTime() + this->GetLagCompensation());
				if (Settings.Aimbot.AutoShoot
					&& (LocalWeapon == OldWeapon
						&& !(pCmd->buttons & IN_RELOAD))
					&& this->CanShoot())
				{
					if (LocalWeapon->GetWeaponID() != WEAPON_REVOLVER)
					{
						if (LocalWeapon->GetWeaponInfo()->bFullAuto)
							pCmd->buttons |= IN_ATTACK;
						else
						{
							static bool bSwitch = false;
							if (bSwitch)
								pCmd->buttons |= IN_ATTACK;
							else
								pCmd->buttons &= ~IN_ATTACK;
							bSwitch = !bSwitch;
						}
					}
				}
				if ((Settings.Aimbot.SilentAim && pCmd->buttons & IN_ATTACK) || !Settings.Aimbot.SilentAim)
				{
					if ((GetAsyncKeyState(Settings.Aimbot.Key) && Settings.Aimbot.Key != 0)
						|| (Settings.Aimbot.Key == 0 && !Settings.Aimbot.AutoShoot)
						|| (Settings.Aimbot.Key == 0 && Settings.Aimbot.AutoShoot && pCmd->buttons & IN_ATTACK))
					{
						this->isAimbotting = true;
						HitBox.z += Settings.Aimbot.AimHeight;
						QAngle Angle = g_Math.CalcAngle(LocalPlayer->GetEyePosition(), HitBox);
						Angle.Normalize();


						//pCmd->viewangles = Angle;

						if (Settings.Aimbot.NoRecoil != 0)
						{
							this->NoRecoil(pCmd, Angle);
							isRCSinAimbot = true;
						}

						QAngle Difference = Angle - pCmd->viewangles;
						Difference.Normalize();
						Difference.y /= Settings.Aimbot.SmoothY;
						Difference.x /= Settings.Aimbot.SmoothX;
						Difference.Normalize();
						Angle = pCmd->viewangles + Difference;
						Angle.Normalize();
						pCmd->viewangles = Angle;



						if (Settings.Aimbot.SilentFirstXShots && !Settings.Aimbot.SilentAim)
						{
							if (LocalPlayer->GetShots() > Settings.Aimbot.XShots)
								Interfaces::Engine->SetViewAngles(pCmd->viewangles);
						}
						else if (!Settings.Aimbot.SilentAim)
							Interfaces::Engine->SetViewAngles(pCmd->viewangles);

						//if (!Settings.Aimbot.SilentAim && (GetAsyncKeyState(Settings.Aimbot.Key) || Settings.Aimbot.Key == 0))
						//	Interfaces::Engine->SetViewAngles(Angle);
					}
				}
			}
		}
	}

	if (Settings.Aimbot.NoRecoil != 0 && !isRCSinAimbot)
		this->NoRecoil(pCmd);
	if (LocalPlayer->GetAlive())
		OldWeapon = LocalWeapon;
	End(pCmd);
	if (Settings.Aimbot.AntiUntrust)
		this->ClampAngles(pCmd);
}
CAimbot* g_pAimbot = new CAimbot;