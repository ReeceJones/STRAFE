#include "Offsets.h"
#include "FindPattern.h"

sOffsets offsets;
void COffsets::GetOffsets()
{
	offsets.m_fFlags = NetvarManager->GetOffset("DT_CSPlayer", "m_fFlags");

	offsets.D3DDevice = **reinterpret_cast<DWORD**>(FindPattern->IDA("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 0x1); //needs to change
	printf("D3DDevice: 0x%X\n", offsets.D3DDevice);
	//Sleep(500);

	offsets.m_aimPunchAngle = NetvarManager->GetOffset("DT_BasePlayer", "m_aimPunchAngle");
	offsets.m_flFlashDuration = NetvarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
	offsets.m_iGlowIndex = offsets.m_flFlashDuration + 0x18;
	//0F 57 C0 C7 05 ? ? ? ? 00 00 00 00 0F 11 05 ? ? ? ? 68 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? 00 00 00 00 C7 05 ? ? ? ? 00 00 00 00) + 16]
	offsets.GlowManager = *(DWORD*)(FindPattern->IDA("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 0x3); //needs to change
	printf("GlowManager: 0x%X\n", offsets.GlowManager);
	//Sleep(500);

	offsets.m_bDormant = 0xe9;
	offsets.m_iTeamNum = NetvarManager->GetOffset("DT_CSPlayer", "m_iTeamNum");
	offsets.m_vecViewOffset = NetvarManager->GetOffset("DT_CSPlayer","m_vecViewOffset[0]");
	offsets.m_vecOrigin = NetvarManager->GetOffset("DT_BasePlayer", "m_vecOrigin");
	offsets.m_iHealth = NetvarManager->GetOffset("DT_CSPlayer", "m_iHealth");
	offsets.m_nWritableBones = NetvarManager->GetOffset("DT_CSPlayer", "m_nForceBone") + 0x20;

	offsets.m_dwOcclusionArray = *(DWORD*)(FindPattern->IDA("client.dll", "A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8") + 0x1);//needs to change if you want to not crash when using SetupBonesB
	printf("m_dwOcclusionArray: 0x%X\n", offsets.m_dwOcclusionArray);
	//Sleep(500);

	offsets.m_bDidCheckForOcclusion = *(DWORD*)(FindPattern->IDA("client.dll", "A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8") + 0x7);//needs to change
	printf("m_bDidCheckForOcclusion: 0x%X\n", offsets.m_bDidCheckForOcclusion);
	//Sleep(500);

	offsets.m_bGunGameImmunity = NetvarManager->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
	offsets.m_nTickBase = NetvarManager->GetOffset("DT_CSPlayer", "m_nTickBase");
	offsets.m_hActiveWeapon = NetvarManager->GetOffset("DT_CSPlayer", "m_hActiveWeapon");
	offsets.m_flNextPrimaryAttack = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	offsets.m_viewPunchAngle = NetvarManager->GetOffset("DT_BasePlayer", "m_viewPunchAngle");
	offsets.m_ArmorValue = NetvarManager->GetOffset("DT_CSPlayer", "m_ArmorValue");
	offsets.m_bHasHelmet = NetvarManager->GetOffset("DT_CSPlayer", "m_bHasHelmet");
	offsets.m_iWeaponID = NetvarManager->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty") + 0x30;
	offsets.m_lifeState = NetvarManager->GetOffset("DT_CSPlayer", "m_lifeState"); 
	offsets.m_nMoveType = 0x258;
	offsets.m_flLowerBodyYawTarget = NetvarManager->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
	offsets.m_hActiveWeapon = NetvarManager->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	offsets.m_vecVelocity = NetvarManager->GetOffset("DT_CSPlayer", "m_vecVelocity[0]");
	offsets.m_angRotation = NetvarManager->GetOffset("DT_CSPlayer", "m_angRotation"); //0x128
	offsets.m_angEyeAngles = NetvarManager->GetOffset("DT_CSPlayer", "m_angEyeAngles");

	offsets.CAM_Think = FindPattern->IDA("client.dll", "85 C0 75 30 38 86"); //needs to  change
	printf("CAM_Think: 0x%X\n", offsets.CAM_Think);
	//Sleep(500);

	offsets.deadflag = NetvarManager->GetOffset("DT_CSPlayer", "deadflag");
	offsets.m_bScoped = NetvarManager->GetOffset("DT_CSPlayer", "m_bIsScoped");
	offsets.m_iItemIDHigh = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iItemIDHigh");
	offsets.m_iItemIDLow = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iItemIDLow");
	offsets.m_iAccountID = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iAccountID");
	offsets.m_iEntityQuality = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iEntityQuality");
	offsets.m_OriginalOwnerXuidLow = NetvarManager->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	offsets.m_OriginalOwnerXuidHigh = NetvarManager->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	offsets.m_iItemDefinitionIndex = NetvarManager->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	offsets.m_nFallbackPaintKit = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_nFallbackPaintKit");
	offsets.m_nFallbackSeed = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_nFallbackSeed");
	offsets.m_flFallbackWear = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_flFallbackWear");
	offsets.m_nFallbackStatTrak = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_nFallbackStatTrak");
	offsets.m_nModelIndex = NetvarManager->GetOffset("DT_BasePlayer", "m_nModelIndex");
	offsets.m_iViewModelIndex = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
	offsets.m_iWorldModelIndex = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
	offsets.m_iClip1 = NetvarManager->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
	offsets.m_flSimulationTime = NetvarManager->GetOffset("DT_CSPlayer", "m_flSimulationTime");

	offsets.LoadFromBufferEx = FindPattern->IDA("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	printf("LoadFromBufferEx: 0x%X\n", offsets.LoadFromBufferEx);
	//Sleep(500);

	offsets.InitKeyValuesEx = FindPattern->IDA("client.dll", "55 8B EC 51 33 C0 C7 45"); 
	printf("InitKeyValuesEx: 0x%X\n", offsets.InitKeyValuesEx);
	//Sleep(500);

	offsets.m_iShotsFired = NetvarManager->GetOffset("DT_CSPlayer", "m_iShotsFired");
	offsets.m_hObserverTarget = NetvarManager->GetOffset("DT_CSPlayer", "m_hObserverTarget");
	offsets.m_iFov = NetvarManager->GetOffset("DT_BasePlayer", "m_iFOV");
	offsets.m_bSpotted = NetvarManager->GetOffset("DT_BaseEntity", "m_bSpotted");
	offsets.m_flFlashMaxAlpha = NetvarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	offsets.m_iCompetitiveRanking = NetvarManager->GetOffset("DT_CSPlayerResource", "m_iCompetitiveRanking");
	//printf("0x%X\n", offsets.m_iCompetitiveRanking);
	offsets.m_bHasDefuser = NetvarManager->GetOffset("DT_CSPlayerResource", "m_bHasDefuser");
	offsets.m_iPing = NetvarManager->GetOffset("DT_CSPlayerResource", "m_iPing");
	offsets.m_iPlayerC4 = NetvarManager->GetOffset("DT_CSPlayerResource", "m_iPlayerC4");
	offsets.m_hOwnerEntity = NetvarManager->GetOffset("DT_BaseEntity", "m_hOwnerEntity");
	offsets.m_bIsDefusing = NetvarManager->GetOffset("DT_CSPlayer", "m_bIsDefusing");
	/*
	---------[ DT_PlantedC4 ]---------
-> m_bBombTicking --> 0x2970
-> m_flC4Blow --> 0x297C
-> m_flTimerLength --> 0x2980
-> m_flDefuseLength --> 0x2990
-> m_flDefuseCountDown --> 0x2994
-> m_bBombDefused --> 0x2998
-> m_hBombDefuser --> 0x299C
	*/
	offsets.m_flC4Blow = NetvarManager->GetOffset("DT_PlantedC4", "m_flC4Blow");
	offsets.m_iCompetitiveWins = NetvarManager->GetOffset("DT_CSPlayerResource", "m_iCompetitiveWins");
	offsets.m_iAccount = NetvarManager->GetOffset("DT_CSPlayer", "m_iAccount");
	offsets.s_bOverridePostProcessing = FindPattern->IDA("client.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 0x2;
	printf("s_bOverridePostProcessing: 0x%X\n", offsets.s_bOverridePostProcessing);
	//Sleep(500);


	offsets.m_Collision = NetvarManager->GetOffset("DT_BaseEntity", "m_Collision");
	offsets.m_vecMins = NetvarManager->GetOffset("DT_BaseEntity", "m_vecMins");
	offsets.m_vecMaxs = NetvarManager->GetOffset("DT_BaseEntity", "m_vecMaxs");
	offsets.m_CollisionGroup = NetvarManager->GetOffset("DT_BaseEntity", "m_CollisionGroup");
	//offsets.dwGameResources = FindPattern->IDA("client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 0x2c;
	offsets.dwGameResources = FindPattern->Alt("client.dll", (BYTE*)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx", NULL, NULL) + 0x2;
	printf("dwGameResources: 0x%X\n", offsets.dwGameResources);
	//Sleep(500);
	offsets.m_hMyWeapons = NetvarManager->GetOffset("DT_CSPlayer", "m_hMyWeapons");
	printf("m_hMyWeapons: 0x%X\n", offsets.m_hMyWeapons);
	offsets.m_hViewModel = NetvarManager->GetOffset("DT_BasePlayer", "m_hViewModel[0]");

	offsets.m_hOwner = NetvarManager->GetOffset("DT_BaseViewModel", "m_hOwner");
	offsets.m_hWeapon = NetvarManager->GetOffset("DT_BaseViewModel", "m_hWeapon");
	offsets.m_szLastPlaceName = NetvarManager->GetOffset("DT_CSPlayer", "m_szLastPlaceName");
	offsets.m_flPostponeFireReadyTime = NetvarManager->GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
	offsets.m_fAccuracyPenalty = NetvarManager->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
	offsets.dwPresent = FindPattern->IDA("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;
	printf("dwPresent: 0x%X\n", offsets.dwPresent);
}
COffsets* pOffsets = new COffsets;
CFindPattern* FindPattern = new CFindPattern;