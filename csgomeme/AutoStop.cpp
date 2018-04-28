#include "Interfaces.h"
#include "Features.h"
#include "Settings.h"
#include "Definitions.h"
#include "Enums.h"

void CAimbot::Crouch(CUserCmd* pCmd)
{
	pCmd->buttons |= IN_DUCK;
}

void CAimbot::Stop(CUserCmd* pCmd)
{
	pCmd->sidemove = 0;
	pCmd->forwardmove = 0;
}