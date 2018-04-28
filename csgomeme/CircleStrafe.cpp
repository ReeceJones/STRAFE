#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"
#include "FindPattern.h"

void CMisc::CircleStrafe(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static int stage = 0;
	static int time = 0;
	static float CurrentMoveForward = pCmd->forwardmove;
	static float CurrentMoveSideWays = pCmd->sidemove;
	if (!(LocalPlayer->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(Settings.Misc.CircleStrafeKey))
	{
		pCmd->viewangles.y += 30.f;

		//switch (stage)
		//{
		//case 1:
		//	pCmd->forwardmove += 5;
		//	pCmd->sidemove += 5;
		//	break;

		//case 2:
		//	pCmd->forwardmove -= 5;
		//	pCmd->sidemove += 5;
		//	break;

		//case 3:
		//	pCmd->forwardmove -= 5;
		//	pCmd->sidemove -= 5;
		//	break;

		//case 4:
		//	pCmd->forwardmove += 5;
		//	pCmd->sidemove -= 5;
		//	break;
		//}

		//if (stage == 4 && time == 250)
		//{
		//	stage = 0;
		//	time = 0;
		//}
		//else
		//{
		//	time++;
		//	if (time % 50)
		//		stage++;
		//}
	}
}