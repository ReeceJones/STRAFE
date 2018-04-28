#include "Entity.h"
#include "Interfaces.h"
#include "Collidable.h"

CBaseCombatWeapon* CBaseEntity::GetWeapon()
{
	DWORD weaponData = *(DWORD*)((DWORD)this + offsets.m_hActiveWeapon);
	return (CBaseCombatWeapon*)Interfaces::EntityList->GetClientEntityFromHandle(weaponData);
}

player_info_t CBaseEntity::GetPlayerInfo()
{
	player_info_t PlayerInfo;
	Interfaces::Engine->GetPlayerInfo(this->GetIndex(), &PlayerInfo);
	return PlayerInfo;
}

CBaseEntity* CBaseEntity::GetObserverTarget()
{
	int index = *(int*)((DWORD)this + offsets.m_hObserverTarget);
	return Interfaces::EntityList->GetClientEntityFromHandle(index);
}

float CBasePlantedC4::GetBombTime()
{
	return *(float*)((DWORD)this + offsets.m_flC4Blow);
}

ICollideable* CBaseEntity::GetCollideable()
{
	return (ICollideable*)((DWORD)this + offsets.m_Collision);
}

CBaseEntity* CBaseEntity::GetOwner()
{
	//ULONG hOwnerEntity = *reinterpret_cast<PULONG>(this + offsets.m_hOwnerEntity);

	return Interfaces::EntityList->GetClientEntityFromHandle(*(ULONG*)((DWORD)this + offsets.m_hOwnerEntity));
}