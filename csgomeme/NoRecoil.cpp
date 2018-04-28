#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"

void CAimbot::NoRecoil(CUserCmd* pCmd, QAngle &Angle)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Settings.Aimbot.NoRecoil == 2)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (Angle == QAngle(0, 0, 0))
			{
				pCmd->viewangles -= LocalPlayer->GetAimPunch() * 2.00f;
				pCmd->viewangles.Normalize();
			}
			else
			{
				Angle -= LocalPlayer->GetAimPunch() * 2.00f;
				Angle.Normalize();
			}
		}
	}
	else if (Settings.Aimbot.NoRecoil == 1)
	{
		QAngle Punch;
		static QAngle OldPunch;
		QAngle NewAngle;

		//Punch = LocalPlayer->GetAimPunch();
		//if (!this->isAimbotting)
		//	NewAngle = Angle - (Punch - OldPunch) * 2.f;
		//else
		//	NewAngle = Angle - Punch * 2.f;
		//Interfaces::Engine->SetViewAngles(NewAngle);
		if (Angle == QAngle(0, 0, 0))
		{
			QAngle q = pCmd->viewangles;
			Punch = LocalPlayer->GetAimPunch();
			if (!this->isAimbotting)
				NewAngle = q - (Punch - OldPunch) * 2.f;
			else
				NewAngle = q - Punch * 2.f;
			Interfaces::Engine->SetViewAngles(NewAngle);
		}
		else
		{
			Punch = LocalPlayer->GetAimPunch();
			if (!this->isAimbotting)
				NewAngle = Angle - (Punch - OldPunch) * 2.f;
			else
				NewAngle = Angle - (Punch * 2.f);
			Angle = NewAngle;
		}
		OldPunch = Punch;
	}
}