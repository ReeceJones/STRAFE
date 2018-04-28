#pragma once

#include "Vector.h"
#include "Structs.h"
#include <iostream>
#include <Windows.h>
#include "Util.h"
#include "Enums.h"
#include "FindPattern.h"
//#include "Collidable.h"

class ICollideable;
typedef unsigned short ModelInstanceHandle_t;
class CBaseCombatWeapon;
class CWeaponData;
typedef unsigned long CBaseHandle;
class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle() = 0;
	virtual void GetRefEHandle() const = 0;
};

class IEntityEnumerator
{
public:
	virtual bool EnumEntity(IHandleEntity *pHandleEntity) = 0;
};

//define all
class CBaseEntity
{
public:
	int GetHealth()
	{
		return *reinterpret_cast<int*>((DWORD)this + offsets.m_iHealth);
	}
	int GetFlags()
	{
		return *(int*)((DWORD)this + offsets.m_fFlags);
	}
	QAngle GetAimPunch()
	{
		return *(QAngle*)((DWORD)this + offsets.m_aimPunchAngle);
	}
	bool GetDormant()
	{
		return *(bool*)((DWORD)this + offsets.m_bDormant);
	}
	int GetTeam()
	{
		return *(int*)((DWORD)this + offsets.m_iTeamNum);
	}
	ClientClass* GetClientClass()
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef ClientClass*(__thiscall* OriginalFn)(PVOID);
		return g_pUtil->CallVFunc<OriginalFn>(pNetworkable, 2)(pNetworkable);
	}
	Vector GetOrigin()
	{
		return *(Vector*)((DWORD)this + offsets.m_vecOrigin);
	}
	Vector GetEyePosition()
	{
		Vector origin = this->GetOrigin();
		Vector offset = *(Vector*)((DWORD)this + offsets.m_vecViewOffset);

		return(origin + offset);
	}
	CBaseCombatWeapon* GetWeapon();
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		__asm
		{
			mov edi, this
			lea ecx, DWORD ptr ds : [edi + 0x4]
			mov edx, DWORD ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call DWORD ptr ds : [edx + 0x34]
		}
	}
	Vector GetBonePosition(int iBone)
	{
		matrix3x4_t boneMatrixes[128];
		if (this->SetupBones(boneMatrixes, 128, 0x100, 0))
		{
			matrix3x4_t boneMatrix = boneMatrixes[iBone];
			return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
		}
		else return Vector(0, 0, 0);
	}
	bool SetupBonesB(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		*(int*)((uintptr_t) this + offsets.m_nWritableBones) = 0;
		*(int*)((uintptr_t) this + offsets.m_bDidCheckForOcclusion) = reinterpret_cast<int*>(offsets.m_dwOcclusionArray)[1];
		__asm
		{
			mov edi, this
			lea ecx, DWORD ptr ds : [edi + 0x4]
			mov edx, DWORD ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call DWORD ptr ds : [edx + 0x34]
		}
	}
	Vector GetBonePositionB(int iBone)
	{
		matrix3x4_t boneMatrixes[128];
		if (this->SetupBonesB(boneMatrixes, 128, 0x100, 0))
		{
			matrix3x4_t boneMatrix = boneMatrixes[iBone];
			return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
		}
		else return Vector(0, 0, 0);
	}
	bool GetGunGameImmunity()
	{
		return *(bool*)((DWORD)this + offsets.m_bGunGameImmunity);
	}
	int GetTickBase()
	{
		return *(int*)((DWORD)this + offsets.m_nTickBase);
	}
	bool GetAlive()
	{
		return (bool)(*(int*)((DWORD)this + offsets.m_lifeState) == 0);
	}
	int GetMoveType()
	{
		return *(int*)((DWORD)this + offsets.m_nMoveType);
	}
	float GetLowerBodyYawTarget()
	{
		return *(float*)((DWORD)this + offsets.m_flLowerBodyYawTarget);
	}
	bool HasHelmet()
	{
		return *(bool*)((DWORD)this + offsets.m_bHasHelmet);
	}
	int GetArmor()
	{
		return *(int*)((DWORD)this + offsets.m_ArmorValue);
	}
	model_t* GetModel()
	{
		return *(model_t**)((DWORD)this + 0x6c);
	}
	int GetIndex() 
	{
		return *(int*)((DWORD)this + 0x64);
	}
	player_info_t GetPlayerInfo();
	Vector GetVelocity()
	{
		return *reinterpret_cast<Vector*>((DWORD)this + offsets.m_vecVelocity);
	}
	QAngle* GetHeadRotation()
	{
		return (QAngle*)((DWORD)this + offsets.m_angRotation);
	}
	QAngle* GetEyeAngles()
	{
		return (QAngle*)((DWORD)this + offsets.m_angEyeAngles);
	}
	float GetSimulationTime()
	{
		return *(float*)((DWORD)this + offsets.m_flSimulationTime);
	}
	int GetShots()
	{
		return *(int*)((DWORD)this + offsets.m_iShotsFired);
	}
	CBaseEntity* GetObserverTarget();
	int GetFov()
	{
		return *(int*)((DWORD)this + offsets.m_iFov);
	}
	bool* Spotted()
	{
		return (bool*)((DWORD)this + offsets.m_bSpotted);
	}
	bool GetScoped()
	{
		return *(bool*)((DWORD)this + offsets.m_bScoped);
	}
	CBaseEntity* GetOwner();
	bool HasC4()
	{
		static DWORD dwGameResources = FindPattern->Alt("client.dll", (BYTE*)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx", NULL, NULL) + 0x2;
		int iBombIndex = *(int*)(**(DWORD**)(dwGameResources) + offsets.m_iPlayerC4);
		if (iBombIndex == this->GetIndex())
			return true;
		else
			return false;
	}
	bool GetDefusing()
	{
		return *(bool*)((DWORD)this + offsets.m_bIsDefusing);
	}
	int GetMoney()
	{
		return *(int*)((DWORD)this + offsets.m_iAccount);
	}
	ICollideable* GetCollideable();
	matrix3x4a_t& GetCoordinateFrame()
	{
		return *(matrix3x4a_t*)((DWORD)this + offsets.m_CollisionGroup - 0x30);
	}
	int GetRank()
	{
		//return *(int*)((DWORD)this + offsets.m_iCompetitiveRanking);

		//return *(int*)(DWORD() + offsets.m_iCompetitiveRanking + this->GetIndex() * 4);
		DWORD GameResources = **(DWORD**)offsets.dwGameResources;
		return *(int*)(**(DWORD**)(offsets.dwGameResources)+ offsets.m_iCompetitiveRanking + this->GetIndex() * 4);
		//return *(int*)(DWORD(GameResources) + offsets.m_iCompetitiveRanking + this->GetIndex() * 4);
	}
	bool GetDefuser()
	{
		return *(bool*)(DWORD(**(DWORD**)offsets.dwGameResources) + offsets.m_bHasDefuser + this->GetIndex() * 4);
	}
	int GetPing()
	{
		return *(int*)(DWORD(**(DWORD**)offsets.dwGameResources) + offsets.m_iPing + this->GetIndex() * 4);
	}
	CBaseHandle *GetWeapons()
	{
		return (CBaseHandle*)((DWORD)this + offsets.m_hMyWeapons);
	}
	CBaseHandle GetViewModel()
	{
		return *(CBaseHandle*)((DWORD)this + offsets.m_hViewModel);
	}
};

class CBaseViewModel
{
public:
	CBaseHandle GetOwner()
	{
		return *(CBaseHandle*)((DWORD)this + offsets.m_hOwner);
	}
	int GetWeapon()
	{
		return *(int*)((DWORD)this + offsets.m_hWeapon);
	}
	int* GetModelIndex()
	{
		return (int*)((DWORD)this + offsets.m_nModelIndex);
	}
};

class CBasePlayer
{
public:
	int* GetWeapons();
	QAngle GetAimPunchAngle();
	QAngle* GetViewPunchAngle();
	unsigned int GetTickBase();
	int GetShotsFired();
};
class CBasePlantedC4 : public CBaseEntity
{
public:
	bool IsBombTicking();
	float GetBombTime();
	bool IsBombDefused();
	int GetBombDefuser();
};
class CHudTexture
{
public:
	char type[64];
	char subtype[64];
	char unknowndata00[2];
	char charinFont;
	char unknowndata01[1];
};
class CSWeaponType { public: };
class WeaponInfo_t
{
public:
	BYTE		pad_0x0000[4];			//0x0000
	bool		bParsedScript;			//0x0004
	bool		bLoadedHudElements;		//0x0005
	char		szClassName[80];		//0x0006
	char		szPrintName[80];		//0x0056
	char		szViewModel[80];		//0x00A6
	char		szWorldModel[80];		//0x00F6
	char		szBulletType[32];		//0x0146
	char		szWorldDroppedModel[80];//0x0166
	char		szAnimationPrefix[16];	//0x01B6
	BYTE		pad_0x01C6[2];			//0x01C6
	int			iSlot;					//0x01C8
	int			iPosition;				//0x01CC
	int			iMaxClip1;				//0x01D0
	int			iMaxClip2;				//0x01D4
	int			iDefaultClip1;			//0x01D8
	int			iDefaultClip2;			//0x01DC
	int			iWeight;				//0x01E0
	int			iRumbleEffect;			//0x01E4
	bool		bAutoSwitchTo;			//0x01E8
	bool		bAutoSwitchFrom;		//0x01E9
	BYTE		pad_0x01EA[2];			//0x01EA
	int			iFlags;					//0x01EC
	char		szAmmo1[16];			//0x01F0
	char		szAmmo2[16];			//0x0200
	char		szAIAddOn[80];			//0x0210
	char		aShootSounds[17][80];	//0x0260
	char PAD_NEW[80];
	int			iAmmoType;
	int			iAmmo2Type;
	bool		bMeleeWeapon;
	bool		bBuiltRightHanded;
	bool		bAllowFlipping;
	BYTE		pad_0x07BB;
	int			iSpriteCount;
	CHudTexture* iconWeaponS;
	CHudTexture* m_pHudTexture_Weapon; //iconWeapon
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconWeaponSmall;
	bool		bShowUsageHint;
	BYTE		pad_0x07E5[3];
	CSWeaponType iWeaponType;
	int			iTeam;
	int			iWeaponID;
	bool		bFullAuto;
	BYTE		pad_0x07F5[3];
	float		flHeatPerShot;
	int			iWeaponPrice;
	float		flArmorRatio;
	float		flMaxPlayerSpeed;
	float		flMaxPlayerSpeedAlt;
	int			iCrosshairMinDistance;
	int			iCrosshairDeltaDistance;
	float		flPenetration;
	int			iDamage;
	float		flRange;
	float		flRangeModifier;
	int			iBullets;
	float		flCycleTime;
	float		flCycleTimeAlt;
	char		szHeatEffect[80];
	float		aflSmokeColor[3];
	char		szMuzzleFlashEffect_1stPerson[80];
	char		szMuzzleFlashEffect_3rdPerson[80];
	char		szEjectBrassEffect[80];
	char		szTracerEffect[80];
	int			iTracerFrequency;
	float		flSpread;
	float		flSpreadAlt;
	float		flInaccuracyCrouch;
	float		flInaccuracyCrouchAlt;
	float		flInaccuracyStand;
	float		flInaccuracyStandAlt;
	float		flInaccuracyJump;
	float		flInaccuracyJumpAlt;
	float		flInaccuracyLand;
	float		flInaccuracyLandAlt;
	float		flInaccuracyLadder;
	float		flInaccuracyLadderAlt;
	float		flInaccuracyFire;
	float		flInaccuracyFireAlt;
	float		flInaccuracyMove;
	float		flInaccuracyMoveAlt;
	float		flRecoveryTimeStand;
	float		flRecoveryTimeCrouch;
	float		flInaccuracyReload;
	float		flInaccuracyAltSwitch;
	float		flRecoilAngle;
	float		flRecoilAngleAlt;
	float		flRecoilAngleVariance;
	float		flRecoilAngleVarianceAlt;
	float		flRecoilMagnitude;
	float		flRecoilMagnitudeAlt;
	float		flRecoilMagnitudeVariance;
	float		flRecoilMagnitudeVarianceAlt;
	int			iRecoilSeed;
	float		flFlinchVelocityModifierLarge;
	float		flFlinchVelocityModifierSmall;
	float		flTimeToIdle;
	float		flIdleInterval;
	float		recoilTable[2][128];
	int			iZoomLevels;
	int			iZoomFOV[3];
	float		fZoomTime[3];
	bool		bHideViewModelZoomed;
	char		szZoomInSound[80];
	char		szZoomOutSound[80];
	BYTE		pad_0x0F11[3];
	float		flBotAudibleRange;
	bool		bCanUseWithShield;
	char		szWrongTeamMsg[32];
	char		szAnimPrefix[16];
	char		szShieldViewModel[64];
	char		szAddonModel[80];
	char		szAddonLocation[80];
	char		szSilencerModel[80];
	BYTE		pad_0x1079[3];
	float		flAddonScale;
	float		flThrowVelocity;
	int			iKillAward;
	BYTE		pad_0x1088[8];
};
class CBaseCombatWeapon
{
public:
	char			__pad[0x64];
	int				index;
	float& GetNextPrimaryAttack()
	{
		return *(float*)((DWORD)this + offsets.m_flNextPrimaryAttack);
	}
	int& GetWeaponID()
	{
		return *(int*)((DWORD)this + offsets.m_iWeaponID);
	}
	int GetWeaponType()
	{
		if (!this) return WEAPONTYPE_UNKNOWN;
		auto id = this->GetWeaponID();
		switch (id)
		{
		case WEAPON_DEAGLE:
		case WEAPON_ELITE:
		case WEAPON_FIVESEVEN:
		case WEAPON_HKP2000:
		case WEAPON_USP_SILENCER:
		case WEAPON_CZ75A:
		case WEAPON_TEC9:
		case WEAPON_REVOLVER:
		case WEAPON_GLOCK:
		case WEAPON_P250:
			return WEAPONTYPE_PISTOL;
			break;
		case WEAPON_AK47:
		case WEAPON_M4A1:
		case WEAPON_M4A1_SILENCER:
		case WEAPON_GALILAR:
		case WEAPON_AUG:
		case WEAPON_FAMAS:
		case WEAPON_SG556:
			return WEAPONTYPE_RIFLE;
			break;
		case WEAPON_P90:
		case WEAPON_BIZON:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_MAC10:
		case WEAPON_UMP45:
			return WEAPONTYPE_SUBMACHINEGUN;
			break;
		case WEAPON_AWP:
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
		case WEAPON_SSG08:
			return WEAPONTYPE_SNIPER_RIFLE;
			break;
		case WEAPON_NEGEV:
		case WEAPON_M249:
			return WEAPONTYPE_MACHINEGUN;
			break;
		case WEAPON_MAG7:
		case WEAPON_SAWEDOFF:
		case WEAPON_NOVA:
		case WEAPON_XM1014:
			return WEAPONTYPE_SHOTGUN;
			break;
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_PUSH:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_T:
			return WEAPONTYPE_KNIFE;
			break;
		case WEAPON_C4:
			return WEAPONTYPE_C4;
			break;
		case WEAPON_HEGRENADE:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_FLASHBANG:
		case WEAPON_MOLOTOV:
		case WEAPON_INCGRENADE:
		case WEAPON_DECOY:
			return WEAPONTYPE_GRENADE;
			break;

		default:
			return WEAPONTYPE_UNKNOWN;
		}
	}
	WeaponInfo_t* GetWeaponInfo()
	{
		if (!this) return nullptr;

		typedef WeaponInfo_t*(__thiscall* OriginalFn)(void*);
		return g_pUtil->CallVFunc<OriginalFn>(this, 456)(this);
	}
	float GetSpread()
	{
		typedef float(__thiscall* OriginalFn)(void*);
		return g_pUtil->CallVFunc<OriginalFn>(this, 484)(this);
	}	
	float GetInnacuracy()
	{
		typedef float(__thiscall* OriginalFn)(void*);
		return g_pUtil->CallVFunc<OriginalFn>(this, 483)(this);
	}
	float& CBaseCombatWeapon::GetAccuracyPenalty()
	{
		return *(float*)((DWORD)this + offsets.m_fAccuracyPenalty);
	}
	std::string CBaseCombatWeapon::GetWeaponName()
	{
		if (!this)
			return "";

		auto id = this->GetWeaponID();

		switch (id)
		{
		case WEAPON_DEAGLE:
			return "Desert Eagle";
		case WEAPON_ELITE:
			return "Dual Berettas";
		case WEAPON_FIVESEVEN:
			return "Five-SeveN";
		case WEAPON_GLOCK:
			return "Glock-18";
		case WEAPON_AK47:
			return "AK-47";
		case WEAPON_AUG:
			return "AUG";
		case WEAPON_AWP:
			return "AWP";
		case WEAPON_FAMAS:
			return "FAMAS";
		case WEAPON_G3SG1:
			return "G3SG1";
		case WEAPON_GALILAR:
			return "Galil";
		case WEAPON_M249:
			return "M249";
		case WEAPON_M4A1:
			return "M4A4";
		case WEAPON_MAC10:
			return "MAC-10";
		case WEAPON_P90:
			return "P90";
		case WEAPON_UMP45:
			return "UMP-45";
		case WEAPON_XM1014:
			return "XM1014";
		case WEAPON_BIZON:
			return "PP-Bizon";
		case WEAPON_MAG7:
			return "MAG-7";
		case WEAPON_NEGEV:
			return "Negev";
		case WEAPON_SAWEDOFF:
			return "Sawed-Off";
		case WEAPON_TEC9:
			return "Tec-9";
		case WEAPON_TASER:
			return "Taser";
		case WEAPON_HKP2000:
			return "P2000";
		case WEAPON_MP7:
			return "MP7";
		case WEAPON_MP9:
			return "MP9";
		case WEAPON_NOVA:
			return "Nova";
		case WEAPON_P250:
			return "P250";
		case WEAPON_SCAR20:
			return "SCAR-20";
		case WEAPON_SG556:
			return "SG 553";
		case WEAPON_SSG08:
			return "SSG 08";
		case WEAPON_KNIFE:
			return "Knife";
		case WEAPON_FLASHBANG:
			return "Flashbang";
		case WEAPON_HEGRENADE:
			return "HE Grenade";
		case WEAPON_SMOKEGRENADE:
			return "Smoke Grenade";
		case WEAPON_MOLOTOV:
			return "Molotov";
		case WEAPON_DECOY:
			return "Decoy";
		case WEAPON_INCGRENADE:
			return "Incendiary Grenade";
		case WEAPON_C4:
			return "C4";
		case WEAPON_KNIFE_T:
			return "Knife";
		case WEAPON_M4A1_SILENCER:
			return "M4A1-S";
		case WEAPON_USP_SILENCER:
			return "USP-S";
		case WEAPON_CZ75A:
			return "CZ75-Auto";
		case WEAPON_REVOLVER:
			return "R8 Revolver";
		default:
			return "Knife";
		}

		return "";
	}
	int GetCurrentAmmo()
	{
		return *(int*)((DWORD)this + offsets.m_iClip1);
	}
	int GetMaxAmmo()
	{
		if (!this)
			return 0;

		auto id = this->GetWeaponID();

		switch (id)
		{
		case WEAPON_DEAGLE:
			return 7;
		case WEAPON_ELITE:
			return 30;
		case WEAPON_FIVESEVEN:
			return 20;
		case WEAPON_GLOCK:
			return 20;
		case WEAPON_AK47:
			return 30;
		case WEAPON_AUG:
			return 30;
		case WEAPON_AWP:
			return 10;
		case WEAPON_FAMAS:
			return 25;
		case WEAPON_G3SG1:
			return 20;
		case WEAPON_GALILAR:
			return 35;
		case WEAPON_M249:
			return 100;
		case WEAPON_M4A1:
			return 30;
		case WEAPON_MAC10:
			return 30;
		case WEAPON_P90:
			return 50;
		case WEAPON_UMP45:
			return 25;
		case WEAPON_XM1014:
			return 7;
		case WEAPON_BIZON:
			return 64;
		case WEAPON_MAG7:
			return 5;
		case WEAPON_NEGEV:
			return 150;
		case WEAPON_SAWEDOFF:
			return 7;
		case WEAPON_TEC9:
			return 25;
		case WEAPON_TASER:
			return 1;
		case WEAPON_HKP2000:
			return 13;
		case WEAPON_MP7:
			return 30;
		case WEAPON_MP9:
			return 30;
		case WEAPON_NOVA:
			return 8;
		case WEAPON_P250:
			return 13;
		case WEAPON_SCAR20:
			return 20;
		case WEAPON_SG556:
			return 30;
		case WEAPON_SSG08:
			return 30;
		case WEAPON_KNIFE:
			return 0;
		case WEAPON_FLASHBANG:
			return 2;
		case WEAPON_HEGRENADE:
			return 1;
		case WEAPON_SMOKEGRENADE:
			return 1;
		case WEAPON_MOLOTOV:
			return 1;
		case WEAPON_DECOY:
			return 1;
		case WEAPON_INCGRENADE:
			return 1;
		case WEAPON_C4:
			return 1;
		case WEAPON_KNIFE_T:
			return 1;
		case WEAPON_M4A1_SILENCER:
			return 30;
		case WEAPON_USP_SILENCER:
			return 12;
		case WEAPON_CZ75A:
			return 12;
		case WEAPON_REVOLVER:
			return 8;
		default:
			return 0;
		}

		return 0;
	}
	CBaseEntity* GetOwner()
	{
		return (CBaseEntity*)((DWORD)this + offsets.m_hOwnerEntity);
	}
	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* UpdateAccuracyPenaltyFn)(void*);
		UpdateAccuracyPenaltyFn Update = (UpdateAccuracyPenaltyFn)((*(PDWORD_PTR*)this)[486]);
		return Update(this);
	}
	unsigned int* GetItemDefinitionIndex() 
	{
		return (unsigned int*)((DWORD)this + offsets.m_iItemDefinitionIndex);
	}
};
class IClientEntityList {
public:
	virtual void* GetClientNetworkable(int entindex) = 0;
	virtual void* GetClientNetworkableFromHandle(CBaseHandle entity_handle) = 0;
	virtual void* GetClientUnknownFromHandle(CBaseHandle entity_handle) = 0;
	virtual CBaseEntity* GetClientEntity(int entindex) = 0;
	virtual CBaseEntity* GetClientEntityFromHandle(CBaseHandle entity_handle) = 0;
	virtual int NumberOfEntities(bool include_non_networkable) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void SetMaxEntities(int maxents) = 0;
	virtual int GetMaxEntities() = 0;
};
class CWeaponData
{
public:
	char _0x0000[2048];
	int32_t ammo_type_maybe; //0x0800 
	char _0x0804[52];
	int32_t weapon_type; //0x0838 
	char _0x083C[12];
	float heat_ps; //0x0848 
	int32_t price; //0x084C 
	float armor_ratio; //0x0850 
	float player_speed; //0x0854 
	float player_speed_alt; //0x0858 
	int32_t crosshair_min_distance; //0x085C 
	int32_t crosshair_delta; //0x0860 
	float penetration; //0x0864 
	int32_t damage; //0x0868 
	float range; //0x086C 
	float range_modifier; //0x0870 
	__int32 bullets; //0x0874 
	float cycle_time; //0x0878 
	float cycle_time_alt; //0x087C 
};