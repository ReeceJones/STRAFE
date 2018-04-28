#include "SkinChanger.h"
#include "Interfaces.h"
#include "Entity.h"
#include "Settings.h"
#include "Recv.h"
#include "NetvarManager.h"
#include <codecvt>
#include <string>
#include <xstring>
#include <comdef.h>

CSkinChanger* g_pSkinChanger = new CSkinChanger;

vecSkinInfo k_Skins;
vecSkinInfo k_Gloves;

CCStrike15ItemSystem* ItemSystem()
{
	static auto addr = reinterpret_cast<CCStrike15ItemSystem* (*)()>(FindPattern->IDA("client.dll", "A1 ? ? ? ? 85 C0 75 53"));
	printf("addr: 0x%X\n", addr);
	return addr();
}

//void CSkinChanger::GetSkins(vecSkinInfo& vecSkins, vecSkinInfo& vecGloves, std::string strModName)
//{
//	//std::ifstream items_game_file(strModName + "/scripts/items/items_game.txt");
//
//	//auto items_game = vdf::read(items_game_file);
//	//items_game_file.close();
//
//	//static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//
//	//for (auto& kit : items_game.childs["paint_kits"].childs)
//	//{
//	//	auto id = std::stoi(kit.second.name);
//
//	//	if (id == 9001)
//	//		continue;
//
//	//	auto desc = kit.second.attribs["description_tag"].substr(1);
//
//	//	const wchar_t* wname = Interfaces::Local->Find(desc.c_str());
//	//	auto name = converter.to_bytes(wname);
//
//	//	if (id < 10000)
//	//		vecSkins.push_back({ id, desc, name });
//	//	else
//	//		vecGloves.push_back({ id, desc, name });
//	//}
//
//	//std::sort(vecSkins.begin(), vecSkins.end());
//	//std::sort(vecGloves.begin(), vecGloves.end());
//
//
//	auto pItemSchema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(ItemSystem()) + sizeof(void*));
//	auto pHead = reinterpret_cast<Head_t*>(uintptr_t(pItemSchema) + 616);
//
//	std::vector<PaintKit> vecKits;
//	for (int i = 0; i <= pHead->nLastElement; i++)
//	{
//		printf("%i\n", i);
//		C_PaintKit* pKit = pHead->pMemory[i].pPaintKit;
//
//		if (pKit->iIndex == 9001)
//			continue;
//
//		const wchar_t* wstrName = Interfaces::Local->Find(pKit->Tag.szBuffer + 1); // Strip # at beginning
//
//		//const std::wstring w(wstrName);
//		//std::string(w.begin(), w.end());
//
//		//printf("%s\n", wstrName);
//
//		//sprintf(cstr, "%ws", wstrName);
//
//		//std::string str = cstr;
//
//		//vecKits.push_back(PaintKit{ pKit->iIndex, wstrName });
//
//		//printf("%S : %i\n", wstrName, pKit->iIndex);
//
//		//std::string shortName = pKit->ShortName;
//		printf("%i\n", pKit->iIndex);
//
//		if (pKit->iIndex < 10000)
//			vecSkins.push_back({ pKit->iIndex, pKit->ShortName.szBuffer, pKit->ShortName.szBuffer });
//		else
//			vecGloves.push_back({ pKit->iIndex, pKit->ShortName.szBuffer, pKit->ShortName.szBuffer });
//	}
//
//	std::sort(vecSkins.begin(), vecSkins.end());
//	std::sort(vecGloves.begin(), vecGloves.end());
//}


void CSkinChanger::FSN()
{
	//CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer()); //pointer to localplayer
	//if (!LocalPlayer || LocalPlayer == nullptr) //sanity checks
	//	return;

	//if (!LocalPlayer->GetAlive()) //some more sanity checks
	//	return;

	//player_info_t LocalInfo; //our players info, contains xuid
	//if (!Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalInfo)) //get our players info
	//	return;

	//CBaseHandle* LocalWeapons = reinterpret_cast<CBaseHandle*>((DWORD)LocalPlayer + 0x2DE8); //handle to the weapons our player has

	//for (auto i = 0; i < 64; ++i) //loop through weapon array, size is 64
	//{
	//	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList->GetClientEntityFromHandle(LocalWeapons[i]); //pointer the the weapon we are holding
	//	if (!pWeapon || pWeapon == nullptr) //sanity checks
	//		continue;

	//	unsigned int* iDefIndex = pWeapon->GetItemDefinitionIndex(); //the itemdefinitionindex of our weapon

	//	Skin_t *weaponSkin = &SkinMap[*iDefIndex]; //get the skin info for our weapon
	//	printf("ItemDefinitionIndex: %i; Weapon: %s; Paintkit: %i\n",  *iDefIndex,pWeapon->GetWeaponName().c_str(), weaponSkin->PaintKit);
	//}

}