#include "Features.h"
#include "Entity.h"
#include "Definitions.h"
#include "Interfaces.h"

void CMisc::Bunnyhop(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!(LocalPlayer->GetFlags() & FL_ONGROUND) && pCmd->buttons & IN_JUMP)
		pCmd->buttons &= ~IN_JUMP;
}