#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"
#include "FindPattern.h"
/*FUCK WE BROKE ANTI AIM*/
/*ON THE FUCKING GEAR*/

QAngle m_oldangleB;
float m_oldforwardB;
float m_oldsidemoveB;

void StartB(CUserCmd* pCmd)
{
	m_oldangleB = pCmd->viewangles;
	m_oldforwardB = pCmd->forwardmove;
	m_oldsidemoveB = pCmd->sidemove;
}

void EndB(CUserCmd* pCmd)
{//pitch fix plox
	float yaw_delta = pCmd->viewangles.y - m_oldangleB.y;
	float f1;
	float f2;
	if (m_oldangleB.y < 0.f)
		f1 = 360.0f + m_oldangleB.y;
	else
		f1 = m_oldangleB.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	pCmd->forwardmove = cos(DEG2RAD(yaw_delta)) * m_oldforwardB + cos(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemoveB;
	pCmd->sidemove = sin(DEG2RAD(yaw_delta)) * m_oldforwardB + sin(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemoveB;
}

void Yaw_Spin(CUserCmd* pCmd, float Amount)
{
	static float Spin = 0.f;

	pCmd->viewangles.y = Spin;

	if (Spin >= 180.f)
		Spin = -180.f;
	else
		Spin += Amount;
}

void Yaw_Backwards(CUserCmd* pCmd)
{
	pCmd->viewangles.y += 180.f;
}

void Yaw_Jitter(CUserCmd* pCmd)
{
	static int state = 0;

	switch (state)
	{
	case 0:
		pCmd->viewangles.y += 180.f + 45.f;
	case 1:
		pCmd->viewangles.y += 180.f;
	case 2:
		pCmd->viewangles.y += 180.f - 45.f;
	}

	if (state == 2)
		state = 0;
	else
		state++;
}

void Yaw_DynamicJitter(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static float OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
	static bool bSwitch = false;

	if (LocalPlayer->GetLowerBodyYawTarget() != OldLowerBodyYaw)
		bSwitch = !bSwitch;
	if (bSwitch)
		pCmd->viewangles.y += 90.f;
	else if (!bSwitch)
		pCmd->viewangles.y -= 90.f;

	OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
}

void Yaw_Right(CUserCmd* pCmd)
{
	pCmd->viewangles.y -= 90.f;
}

void Yaw_Left(CUserCmd* pCmd)
{
	pCmd->viewangles.y += 90.f;
}

void Yaw_FakeLeft(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Left(pCmd);
		bSendPacket = false;
	}
	else
		Yaw_Right(pCmd);
}

void Yaw_FakeRight1(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Right(pCmd);
		bSendPacket = false;
	}
	else
		Yaw_Left(pCmd);
}

void Yaw_FakeRight2(CUserCmd* pCmd, bool &bSendPacket)
{
	static int State = 0;
	static float Yaw_fix = 0;
	Yaw_fix = -90.f;
	if (pCmd->tick_count % 2)
	{
		Yaw_Right(pCmd);
		bSendPacket = false;
	}
	else
	{
		if (State > 5)
		{
			pCmd->viewangles.y = Yaw_fix + (State * 10.f);
		}
		else
		{
			pCmd->viewangles.y = Yaw_fix + ((10 - State) * 10.f);
		}
	}
	if (State < 10)
		State++;
	else
		State = 0;
}

void Yaw_FakeForward(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
		bSendPacket = false;
	else
		Yaw_Backwards(pCmd);
}

void Yaw_FakeJitter1(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Jitter(pCmd);
		bSendPacket = false;
	}
	else
		Yaw_Backwards(pCmd);
}

void Yaw_FakeJitter2(CUserCmd* pCmd, bool &bSendPacket)
{
	static bool bSwitch = false;
	if (pCmd->tick_count % 20)
		bSwitch = !bSwitch;
	if (bSwitch)
	{
		if (pCmd->tick_count % 2)
		{
			bSendPacket = false;
			Yaw_Jitter(pCmd);
		}
		else
			pCmd->viewangles.y = 0.f;
	}
	if (!bSwitch)
	{
		if (pCmd->tick_count % 2)
		{
			bSendPacket = false;
			Yaw_Jitter(pCmd);
		}
		else
			pCmd->viewangles.y = 180.f;
	}
}

void Yaw_FakeDynamicJitter(CUserCmd* pCmd, bool &bSendPacket)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static float OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
	static bool bSwitch = false;
	static bool OldbSwitch = false;

	if (LocalPlayer->GetLowerBodyYawTarget() != OldLowerBodyYaw)
		bSwitch = !bSwitch;

	if (OldbSwitch != bSwitch || pCmd->tick_count % 2)
	{
		if (bSwitch)
			pCmd->viewangles.y += 90.f;
		else if (!bSwitch)
			pCmd->viewangles.y -= 90.f;
		bSendPacket = false;
	}
	else
		Yaw_Backwards(pCmd);

	OldbSwitch = bSwitch;
	OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
}

void Yaw_FakeBackwards1(CUserCmd* pCmd, bool &bSendPacket)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static float OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
	static bool bSwitch = false;
	static bool OldbSwitch = false;

	if (LocalPlayer->GetLowerBodyYawTarget() != OldLowerBodyYaw)
		bSwitch = !bSwitch;

	if (OldbSwitch != bSwitch || bSwitch)
	{
		if (bSwitch)
			pCmd->viewangles.y += 90.f;
		else if (!bSwitch)
			pCmd->viewangles.y -= 90.f;
		true;
	}
	else if (pCmd->tick_count % 2 && !bSwitch)
		Yaw_Backwards(pCmd);

	OldbSwitch = bSwitch;
	OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
}

void Yaw_FakeBackwards2(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Backwards(pCmd);
		bSendPacket = false;
	}
	else
		Yaw_Jitter(pCmd);
}

void Yaw_FakeBackwards3(CUserCmd* pCmd, bool &bSendPacket)
{
	static bool bSwitch = false;
	if (pCmd->tick_count % 20)
		bSwitch = !bSwitch;
	if (bSwitch)
	{
		if (pCmd->tick_count % 2)
		{
			bSendPacket = false;
			Yaw_Backwards(pCmd);
		}
		else
			pCmd->viewangles.y = 0.f;
	}
	if (!bSwitch)
	{
		if (pCmd->tick_count % 2)
		{
			bSendPacket = false;
			Yaw_Backwards(pCmd);
		}
		else
			pCmd->viewangles.y = 180.f;
	}
}

void Yaw_FakeStatic(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		pCmd->viewangles.y = -124.3624f;
		bSendPacket = false;
	}
	else
		Yaw_Spin(pCmd, 10.f);
}

void Yaw_Random(CUserCmd* pCmd)
{
	pCmd->viewangles.y = (rand() % 360) - 180;
}

void Yaw_RealSpin1(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Spin(pCmd, 10.f);
		bSendPacket = false;
	}
	else
		pCmd->viewangles.y = -124.3624f;
}

void Yaw_RealSpin2(CUserCmd* pCmd, bool &bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		Yaw_Spin(pCmd, 10.f);
		bSendPacket = false;
	}
	else
		Yaw_Backwards(pCmd);
}

void Pitch_Down(CUserCmd* pCmd)
{
	pCmd->viewangles.x = 89.f;
}

void Pitch_Up(CUserCmd* pCmd)
{
	pCmd->viewangles.x = -89.f;
}

void Pitch_HalfDown(CUserCmd* pCmd)
{
	pCmd->viewangles.x = 44.45f;
}

void Pitch_HalfUp(CUserCmd* pCmd)
{
	pCmd->viewangles.x = -44.45;
}

void Pitch_Jitter(CUserCmd* pCmd)
{
	static int state = 0;

	switch (state)
	{
	case 0:
		pCmd->viewangles.x = 89.f;
		break;
	case 1:
		pCmd->viewangles.x = 0.f;
		break;
	case 2:
		pCmd->viewangles.x = -89.f;
		break;
	}

	if (state == 2)
		state = 0;
	else
		state++;
}

void MoonWalk(CUserCmd* pCmd, bool& bSendPacket)
{
	if (pCmd->tick_count % 2)
	{
		bSendPacket = false;
		pCmd->viewangles.y -= 180.f;
	}
	else
	{

		bSendPacket = true;
	}
}

void Pitch_DynamicJitter(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static float OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
	static bool Switch = false;
	if (LocalPlayer->GetLowerBodyYawTarget() != OldLowerBodyYaw)
		Switch = !Switch;
	if (Switch)
		pCmd->viewangles.x = 89.f;
	else
		pCmd->viewangles.x = -89.f;

	OldLowerBodyYaw = LocalPlayer->GetLowerBodyYawTarget();
}

void Pitch_SlowJitter(CUserCmd* pCmd)
{
	static int iChange = 0;
	static bool bSwitch = false;
	if (bSwitch)
		pCmd->viewangles.x = 89.f - 10.f * iChange;
	else if (!bSwitch)
		pCmd->viewangles.x = -89.f + 10.f * iChange;
	if (iChange >= 18)
		bSwitch = !bSwitch;

	if (iChange >= 18)
		iChange = 0;
	else
		iChange += 1;
}

void CAimbot::AntiAim(CUserCmd* pCmd, bool &bSendPacket)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static bool PrevTickHasGrenade = false;
	if (LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;
	if (pCmd->buttons & IN_USE)
		return;
	if (LocalPlayer->GetWeapon()->GetWeaponType() == WEAPONTYPE_GRENADE)
		return;
	if (pCmd->buttons & IN_ATTACK && this->CanShoot())
		return;

	StartB(pCmd);
	switch (Settings.Aimbot.AntiAimYaw)
	{
	case YAW_BACKWARDS:
		Yaw_Backwards(pCmd);
		break;
	case YAW_SPIN:
		Yaw_Spin(pCmd, 60.f);
		break;
	case YAW_JITTER:
		Yaw_Jitter(pCmd);
		break;
	case YAW_DYNAMICJITTER:
		Yaw_DynamicJitter(pCmd);
		break;
	case YAW_RIGHT:
		Yaw_Right(pCmd);
		break;
	case YAW_LEFT:
		Yaw_Left(pCmd);
		break;
	case YAW_FAKELEFT:
		Yaw_FakeLeft(pCmd, bSendPacket);
		break;
	case YAW_FAKERIGHT1:
		Yaw_FakeRight1(pCmd, bSendPacket);
		break;
	case YAW_FAKERIGHT2:
		Yaw_FakeRight2(pCmd, bSendPacket);
		break;
	case YAW_FAKEFORWARD:
		Yaw_FakeForward(pCmd, bSendPacket);
		break;
	case YAW_FAKEJITTER1:
		Yaw_FakeJitter1(pCmd, bSendPacket);
		break;
	case YAW_FAKEJITTER2:
		Yaw_FakeJitter2(pCmd, bSendPacket);
		break;
	case YAW_FAKEDYNAMICJITTER:
		Yaw_FakeDynamicJitter(pCmd, bSendPacket);
		break;
	case YAW_FAKEBACKWARDS1:
		Yaw_FakeBackwards1(pCmd, bSendPacket);
		break;
	case YAW_FAKEBACKWARDS2:
		Yaw_FakeBackwards2(pCmd, bSendPacket);
		break;
	case YAW_FAKEBACKWARDS3:
		Yaw_FakeBackwards3(pCmd, bSendPacket);
		break;
	case YAW_FAKESTATIC:
		Yaw_FakeStatic(pCmd, bSendPacket);
		break;
	case 18:
		MoonWalk(pCmd, bSendPacket);
		break;
	case 19:
		Yaw_Spin(pCmd, 90.f);
		break;
	case 20:
		Yaw_Spin(pCmd, 0.5f);
		break;
	case 21:
		Yaw_Random(pCmd);
		break;
	case 22:
		Yaw_RealSpin1(pCmd, bSendPacket);
		break;
	case 23:
		Yaw_RealSpin2(pCmd, bSendPacket);
		break;
	}
	switch (Settings.Aimbot.AntiAimPitch)
	{
	case PITCH_DOWN:
		Pitch_Down(pCmd);
		break;
	case PITCH_UP:
		Pitch_Up(pCmd);
		break;
	case PITCH_HALFDOWN:
		Pitch_HalfDown(pCmd);
		break;
	case PITCH_HALFUP:
		Pitch_HalfUp(pCmd);
		break;
	case PITCH_jITTER:
		Pitch_Jitter(pCmd);
		break;
	case PITCH_DYNAMICJITTER:
		Pitch_DynamicJitter(pCmd);
		break;
	case PITCH_SLOWJITTER:
		Pitch_SlowJitter(pCmd);
		break;
	}

	EndB(pCmd);
	this->ClampAngles(pCmd);
}