#pragma once
#include "Entity.h"
#include "Util.h"

class CMoveHelper
{
public:
	void SetHost(CBaseEntity* pPlayer)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity* pPlayer);
		g_pUtil->CallVFunc<OriginalFn>(this, 1)(this, pPlayer);
	}
};

class CGameMovement
{
public:
	void ProcessMovement(CBaseEntity* pPlayer, PVOID moveData)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, CBaseEntity* pPlayer, PVOID moveData);
		g_pUtil->CallVFunc<OriginalFn>(this, 1)(this, pPlayer, moveData);
	}
};
