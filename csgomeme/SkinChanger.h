#pragma once
#include "Enums.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <codecvt>
#include "Interfaces.h"

inline bool IsKnife(int i) { return i >= 500 || i == 59 || i == 42; }

typedef struct Item_s
{
	constexpr Item_s(const char* szDisplayName, const char* szEntityName, const char* szModel, const char* szIcon = nullptr) :
		szDisplayName(szDisplayName),
		szEntityName(szEntityName),
		szModel(szModel),
		szIcon(szIcon)
	{}

	const char* szDisplayName = nullptr;
	const char* szEntityName = nullptr;
	const char* szModel = nullptr;
	const char* szIcon = nullptr;
} Item_t;

typedef struct WeaponName_s
{
	constexpr WeaponName_s(int iDefinitionIndex, const char* szName) :
		iDefinitionIndex(iDefinitionIndex),
		szName(szName)
	{}

	int iDefinitionIndex = 0;
	const char* szName = nullptr;
} WeaponName_t;

typedef struct QualityName_s
{
	constexpr QualityName_s(int iIndex, const char* szName) :
		iIndex(iIndex),
		szName(szName)
	{}

	int iIndex = 0;
	const char* szName = nullptr;
} QualityName_t;

struct SkinInfo_t
{
	int iIndex;
	std::string strShortName;
	std::string strName;

	bool operator < (const SkinInfo_t& other) const { return (strName < other.strName); }
};

typedef std::vector<SkinInfo_t> vecSkinInfo;

extern vecSkinInfo k_Skins;
extern vecSkinInfo k_Gloves;

static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

const std::map<size_t, Item_t> k_WeaponInfo =
{
	{ 1,{ "Desert Eagle", "WEAPON_DEAGLE", "models/weapons/v_pist_deagle.mdl", "deagle" } },
	{ 2,{ "Dual Berettas", "WEAPON_ELITE", "models/weapons/v_pist_elite.mdl", "elite" } },
	{ 3,{ "Five-SeveN", "WEAPON_FIVESEVEN", "models/weapons/v_pist_fiveseven.mdl", "fiveseven" } },
	{ 4,{ "Glock-18", "WEAPON_GLOCK", "models/weapons/v_pist_glock18.mdl", "glock" } },
	{ 7,{ "AK-47", "WEAPON_AK47", "models/weapons/v_rif_ak47.mdl", "ak47" } },
	{ 8,{ "AUG", "WEAPON_AUG", "models/weapons/v_rif_aug.mdl", "aug" } },
	{ 9,{ "AWP", "WEAPON_AWP", "models/weapons/v_snip_awp.mdl", "awp" } },
	{ 10,{ "FAMAS", "WEAPON_FAMAS", "models/weapons/v_rif_famas.mdl", "famas" } },
	{ 11,{ "G3SG1", "WEAPON_G3SG1", "models/weapons/v_snip_g3sg1.mdl", "g3sg1" } },
	{ 13,{ "Galil AR", "WEAPON_GALILAR", "models/weapons/v_rif_galilar.mdl", "galilar" } },
	{ 14,{ "M249", "WEAPON_M249", "models/weapons/v_mach_m249para.mdl", "m249" } },
	{ 16,{ "M4A4", "WEAPON_M4A1", "models/weapons/v_rif_m4a1.mdl", "m4a1" } },
	{ 17,{ "MAC-10", "WEAPON_MAC10", "models/weapons/v_smg_mac10.mdl", "mac10" } },
	{ 19,{ "P90", "WEAPON_P90", "models/weapons/v_smg_p90.mdl", "p90" } },
	{ 24,{ "UMP-45", "WEAPON_UMP45", "models/weapons/v_smg_ump45.mdl", "ump45" } },
	{ 25,{ "XM1014", "WEAPON_XM1014", "models/weapons/v_shot_xm1014.mdl", "xm1014" } },
	{ 26,{ "PP-Bizon", "WEAPON_BIZON", "models/weapons/v_smg_bizon.mdl", "bizon" } },
	{ 27,{ "MAG-7", "WEAPON_MAG7", "models/weapons/v_shot_mag7.mdl", "mag7" } },
	{ 28,{ "Negev", "WEAPON_NEGEV", "models/weapons/v_mach_negev.mdl", "negev" } },
	{ 29,{ "Sawed-Off", "WEAPON_SAWEDOFF", "models/weapons/v_shot_sawedoff.mdl", "sawedoff" } },
	{ 30,{ "Tec-9", "WEAPON_TEC9", "models/weapons/v_pist_tec9.mdl", "tec9" } },
	{ 31,{ "Zeus x27", "WEAPON_TASER", "models/weapons/v_eq_taser.mdl", "taser" } },
	{ 32,{ "P2000", "WEAPON_HKP2000", "models/weapons/v_pist_hkp2000.mdl", "hkp2000" } },
	{ 33,{ "MP7", "WEAPON_MP7", "models/weapons/v_smg_mp7.mdl", "mp7" } },
	{ 34,{ "MP9", "WEAPON_MP9", "models/weapons/v_smg_mp9.mdl", "mp9" } },
	{ 35,{ "Nova", "WEAPON_NOVA", "models/weapons/v_shot_nova.mdl", "nova" } },
	{ 36,{ "P250", "WEAPON_P250", "models/weapons/v_pist_p250.mdl", "p250" } },
	{ 38,{ "SCAR-20", "WEAPON_SCAR20", "models/weapons/v_snip_scar20.mdl", "scar20" } },
	{ 39,{ "SG 553", "WEAPON_SG556", "models/weapons/v_rif_sg556.mdl", "sg556" } },
	{ 40,{ "SSG 08", "WEAPON_SSG08", "models/weapons/v_snip_ssg08.mdl", "ssg08" } },
	{ 42,{ "Knife (Counter-Terrorists)", "WEAPON_KNIFE", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ 43,{ "Flashbang", "WEAPON_FLASHBANG", "models/weapons/v_eq_flashbang.mdl", "flashbang" } },
	{ 44,{ "HE Grenade", "WEAPON_HEGRENADE", "models/weapons/v_eq_fraggrenade.mdl", "hegrenade" } },
	{ 45,{ "Smoke Grenade", "WEAPON_SMOKEGRENADE", "models/weapons/v_eq_smokegrenade.mdl", "smokegrenade" } },
	{ 46,{ "Molotov", "WEAPON_MOLOTOV", "models/weapons/v_eq_molotov.mdl", "inferno" } },
	{ 47,{ "Decoy Grenade", "WEAPON_DECOY", "models/weapons/v_eq_decoy.mdl", "decoy" } },
	{ 48,{ "Incendiary Grenade", "WEAPON_INCGRENADE", "models/weapons/v_eq_incendiarygrenade.mdl", "inferno" } },
	{ 49,{ "C4 Explosive", "WEAPON_C4", "models/weapons/v_ied.mdl" } },
	{ 59,{ "Knife (Terrorists)", "WEAPON_KNIFE_T", "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ 60,{ "M4A1-S", "WEAPON_M4A1_SILENCER", "models/weapons/v_rif_m4a1_s.mdl", "m4a1_silencer" } },
	{ 61,{ "USP-S", "WEAPON_USP_SILENCER", "models/weapons/v_pist_223.mdl", "usp_silencer" } },
	{ 63,{ "CZ75 Auto", "WEAPON_CZ75A", "models/weapons/v_pist_cz_75.mdl", "cz75a" } },
	{ 64,{ "R8 Revolver", "WEAPON_REVOLVER", "models/weapons/v_pist_revolver.mdl", "revolver" } },
	{ 500,{ "Bayonet", "WEAPON_KNIFE_BAYONET", "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ 505,{ "Flip Knife", "WEAPON_KNIFE_FLIP", "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ 506,{ "Gut Knife", "WEAPON_KNIFE_GUT", "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ 507,{ "Karambit", "WEAPON_KNIFE_KARAMBIT", "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ 508,{ "M9 Bayonet", "WEAPON_KNIFE_M9_BAYONET", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ 509,{ "Huntsman Knife", "WEAPON_KNIFE_TACTICAL", "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ 512,{ "Falchion Knife", "WEAPON_KNIFE_FALCHION", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ 514,{ "Bowie Knife", "WEAPON_KNIFE_SURVIVAL_BOWIE", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ 515,{ "Butterfly Knife", "WEAPON_KNIFE_BUTTERFLY", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ 516,{ "Shadow Daggers", "WEAPON_KNIFE_PUSH", "models/weapons/v_knife_push.mdl", "knife_push" } }
};

const std::vector<WeaponName_s> k_KniveNames =
{
	{ 0, "Default" },
	{ 500, "Bayonet" },
	{ 505, "Flip Knife" },
	{ 506, "Gut Knife" },
	{ 507, "Karambit" },
	{ 508, "M9 Bayonet" },
	{ 509, "Huntsman Knife" },
	{ 512, "Falchion Knife" },
	{ 514, "Bowie Knife" },
	{ 515, "Butterfly Knife" },
	{ 516, "Shadow Daggers" }
};

const std::vector<WeaponName_t> k_WeaponNames =
{
	{ 42, "Knife" },
	{ 7,  "AK-47" },
	{ 8,  "AUG" },
	{ 9,  "AWP" },
	{ 63, "CZ75 Auto" },
	{ 1,  "Desert Eagle" },
	{ 2,  "Dual Berettas" },
	{ 10, "FAMAS" },
	{ 3,  "Five-SeveN" },
	{ 11, "G3SG1" },
	{ 13, "Galil AR" },
	{ 4,  "Glock-18" },
	{ 14, "M249" },
	{ 60, "M4A1-S" },
	{ 16, "M4A4" },
	{ 17, "MAC-10" },
	{ 27, "MAG-7" },
	{ 33, "MP7" },
	{ 34, "MP9" },
	{ 28, "Negev" },
	{ 35, "Nova" },
	{ 32, "P2000" },
	{ 36, "P250" },
	{ 19, "P90" },
	{ 26, "PP-Bizon" },
	{ 64, "R8 Revolver" },
	{ 29, "Sawed-Off" },
	{ 38, "SCAR-20" },
	{ 40, "SSG 08" },
	{ 39, "SG 553" },
	{ 30, "Tec-9" },
	{ 24, "UMP-45" },
	{ 61, "USP-S" },
	{ 25, "XM1014" },
};

const std::vector<QualityName_t> k_QualityNames =
{
	{ 0, "Default" },
	{ 1, "Genuine" },
	{ 2, "Vintage" },
	{ 3, "Unusual" },
	{ 5, "Community" },
	{ 6, "Developer" },
	{ 7, "Self-Made" },
	{ 8, "Customized" },
	{ 9, "Strange" },
	{ 10, "Completed" },
	{ 12, "Tournament" }
};

struct Skin_t
{
	int ItemDefinitionID;
	int PaintKit;
	int Seed;
	int Stattrak;
	float Wear;
};

static std::map<int, Skin_t> SkinMap;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;
//struct CPaintKit;
//struct Node_t;
//struct Head_t;


struct String_t
{
	char* szBuffer;	//0x0000 
	int nCapacity;	//0x0004 
	int _unknown;	//0x0008 
	int nLength;	//0x000C 
}; //Size=0x0010

struct C_PaintKit
{
	int iIndex;				//0x0000
	String_t Name;			//0x0004
	String_t NiceName;		//0x0014
	String_t Tag;			//0x0024
	String_t ShortName;		//0x0034
	String_t _unknown;		//0x0044
	char pad_0x0054[0x8C];	//0x0054
}; //Size=0x00E0

struct Node_t
{
	int nPreviousId;		//0x0000
	int nNextId;			//0x0004
	void* _unknown_ptr;		//0x0008
	int _unknown;			//0x000C
	int iPaintKitId;		//0x0010
	C_PaintKit* pPaintKit;	//0x0014
}; //Size=0x0018

struct Head_t
{
	Node_t* pMemory;		//0x0000
	int nAllocationCount;	//0x0004
	int nGrowSize;			//0x0008
	int nStartElement;		//0x000C
	int nNextAvailable;		//0x0010
	int _unknown;			//0x0014
	int nLastElement;		//0x0018
}; //Size=0x001C


//std::vector<PaintKit> GetPaintKits2()
//{
//	// Search the relative calls
//
//	// call    ItemSystem
//	// push    dword ptr [esi+0Ch]
//	// lea     ecx, [eax+4]
//	// call    CEconItemSchema::GetPaintKitDefinition
//
//	auto dwSigAddress = FindPattern->IDA("client.dll", "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
//
//	// Skip the opcode, read rel32 address
//	auto iItemSystemOffset = *reinterpret_cast<int32_t*>(dwSigAddress + 1);
//	// Add the offset to the end of the instruction
//	auto pItemSystem = reinterpret_cast<CCStrike15ItemSystem* (*)()>(dwSigAddress + 5 + iItemSystemOffset);
//
//	// Skip the instructions between, skip the opcode, read rel32 address
//	auto iGetPaintKitDefinitionOffset = *reinterpret_cast<int32_t*>(dwSigAddress + 11 + 1);
//	// Add the offset to the end of the instruction
//	auto pGetPaintKitDefinition = reinterpret_cast<CPaintKit* (__thiscall *)(int id)>(dwSigAddress + 11 + 5 + iGetPaintKitDefinitionOffset);
//
//	// The last offset is nHeadElement, we need that
//
//	// push    ebp
//	// mov     ebp, esp
//	// push    ebx
//	// push    esi
//	// push    edi
//	// mov     edi, ecx
//	// mov     eax, [edi + 298h]
//
//	// Skip instructions, skip opcode, read offset
//	auto dwStartElementOffset = *reinterpret_cast<uint32_t*>(uintptr_t(pGetPaintKitDefinition) + 8 + 2);
//
//	// Calculate head base from nStartElement's offset
//	auto dwHeadOffset = dwStartElementOffset - 12;
//
//	// Skip VTable, first member variable of ItemSystem is ItemSchema
//	auto pItemSchema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(pItemSystem()) + sizeof(void*));
//
//	auto pHead = reinterpret_cast<Head_t*>(uintptr_t(pItemSchema) + dwHeadOffset);
//
//	std::vector<PaintKit> vecKits;
//
//	for (int i = 0; i <= pHead->nLastElement; ++i)
//	{
//		auto pKit = pHead->pMemory[i].pPaintKit;
//
//		if (pKit->iIndex == 9001)
//			continue;
//
//		const wchar_t* wstrName = Interfaces::Local->Find(pKit->Tag.szBuffer + 1);
//
//		vecKits.push_back(PaintKit{ pKit->iIndex, wstrName });
//	}
//
//	return vecKits;
//}


//using vecSkinInfo = std::vector<SkinInfo_t>;

class CSkinChanger
{
public:
	//void InitializeSkins();
	void FSN();
	inline void GetSkins(vecSkinInfo& vecSkins, vecSkinInfo& vecGloves)
	{
		// Search the relative calls

		// call    ItemSystem
		// push    dword ptr [esi+0Ch]
		// lea     ecx, [eax+4]
		// call    CEconItemSchema::GetPaintKitDefinition

		auto dwSigAddress = FindPattern->IDA("client.dll", "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");

		// Skip the opcode, read rel32 address
		auto iItemSystemOffset = *reinterpret_cast<int32_t*>(dwSigAddress + 1);
		// Add the offset to the end of the instruction
		auto pItemSystem = reinterpret_cast<CCStrike15ItemSystem* (*)()>(dwSigAddress + 5 + iItemSystemOffset);

		// Skip the instructions between, skip the opcode, read rel32 address
		auto iGetPaintKitDefinitionOffset = *reinterpret_cast<int32_t*>(dwSigAddress + 11 + 1);
		// Add the offset to the end of the instruction
		auto pGetPaintKitDefinition = reinterpret_cast<C_PaintKit* (__thiscall *)(int id)>(dwSigAddress + 11 + 5 + iGetPaintKitDefinitionOffset);

		// The last offset is nHeadElement, we need that

		// push    ebp
		// mov     ebp, esp
		// push    ebx
		// push    esi
		// push    edi
		// mov     edi, ecx
		// mov     eax, [edi + 298h]

		// Skip instructions, skip opcode, read offset
		auto dwStartElementOffset = *reinterpret_cast<uint32_t*>(uintptr_t(pGetPaintKitDefinition) + 8 + 2);

		// Calculate head base from nStartElement's offset
		auto dwHeadOffset = dwStartElementOffset - 12;

		// Skip VTable, first member variable of ItemSystem is ItemSchema
		auto pItemSchema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(pItemSystem()) + sizeof(void*));

		auto pHead = reinterpret_cast<Head_t*>(uintptr_t(pItemSchema) + dwHeadOffset);

		//std::vector<PaintKit> vecKits;

		for (int i = 0; i <= pHead->nLastElement; ++i)
		{
			C_PaintKit* pKit = new C_PaintKit;
			pKit = pHead->pMemory[i].pPaintKit;

			if (pKit->iIndex == 9001)
				continue;

			//const wchar_t* wstrName = Interfaces::Local->Find(pKit->Tag.szBuffer + 1);

			//vecKits.push_back(PaintKit{ pKit->iIndex, wstrName });

			if (pKit->iIndex < 10000)
				vecSkins.push_back({ pKit->iIndex, pKit->ShortName.szBuffer, pKit->ShortName.szBuffer });
			else
				vecGloves.push_back({ pKit->iIndex, pKit->ShortName.szBuffer, pKit->ShortName.szBuffer });
		}

	}
	//std::unordered_map<std::string, int> uSkins;
}; extern CSkinChanger* g_pSkinChanger;
