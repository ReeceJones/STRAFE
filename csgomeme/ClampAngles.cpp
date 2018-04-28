#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"

void CAimbot::ClampAngles(CUserCmd* pCmd)
{
	pCmd->viewangles.Normalize();
	if (pCmd->forwardmove > 450.f)
		pCmd->forwardmove = 450.f;
	else if (pCmd->forwardmove < -450.f)
		pCmd->forwardmove = -450.f;

	if (pCmd->sidemove > 450.f)
		pCmd->sidemove = 450.f;
	else if (pCmd->sidemove < -450.f)
		pCmd->sidemove = -450.f;

	if (pCmd->upmove > 320.f)
		pCmd->upmove = 320.f;
	else if (pCmd->upmove < -320.f)
		pCmd->upmove = -320.f;

	pCmd->viewangles.Clamp();
}