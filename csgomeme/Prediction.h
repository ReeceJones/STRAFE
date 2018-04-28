#pragma once
#include "Entity.h"
#include "InputSystem.h"
#include "Util.h"

class IPrediction
{
public:
	void SetupMove(CBaseEntity* player, CUserCmd* ucmd, PVOID empty, PVOID moveData)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity* player, CUserCmd* ucmd, PVOID empty, PVOID moveData);
		g_pUtil->CallVFunc<OriginalFn>(this, 20)(this, player, ucmd, nullptr, moveData);
	}

	void FinishMove(CBaseEntity* player, CUserCmd* ucmd, PVOID moveData)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity* player, CUserCmd* ucmd, PVOID moveData);
		g_pUtil->CallVFunc<OriginalFn>(this, 21)(this, player, ucmd, moveData);
	}
};