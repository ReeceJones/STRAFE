#include "Features.h"
#include "Settings.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Util.h"
#include "FindPattern.h"

void CVisuals::DrawName(CBaseEntity* Entity)
{
	player_info_t info = Entity->GetPlayerInfo();
	Drawing.DrawString(ESPFont, (this->TopX + this->BottomX) / 2, this->TopY - 7, Color(255, 255, 255), FONT_CENTER, info.name);
}

void CVisuals::DrawWeapon(CBaseEntity* Entity)
{
	auto Weapon = Entity->GetWeapon();
	if (!Weapon) 
		return;
	int Max = Entity->GetWeapon()->GetMaxAmmo();
	//int Current = Entity->GetWeapon()->GetCurrentAmmo();
	//Drawing.DrawString(0xa1, (this->TopX + this->BottomX) / 2, this->BottomY + 11, Color(255, 255, 255), FONT_CENTER, "%c", Weapon->GetWeaponInfo()->iconWeapon/*, Current, Max*/);

	char WeaponIcon = Entity->GetWeapon()->GetWeaponInfo()->m_pHudTexture_Weapon->charinFont;
	Drawing.DrawString(0xa1, ((this->TopX + this->BottomX) / 2) - 3, this->BottomY + 11, Color(255, 255, 255), FONT_RIGHT, "%c", WeaponIcon);
	Drawing.DrawString(ESPFont, ((this->TopX + this->BottomX) / 2) + 3, this->BottomY + 11, Color(255, 255, 255), FONT_LEFT, "[ %i / %i ]", Entity->GetWeapon()->GetCurrentAmmo(), Entity->GetWeapon()->GetMaxAmmo());

	//CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	//Vector LocalPos = LocalPlayer->GetOrigin();
	//Vector EntityPos = Entity->GetOrigin();
	//Vector Delta = LocalPos - EntityPos;
	//float Y = Delta.y * Delta.y;
	//float X = Delta.x * Delta.x;
	//float Dist = sqrt(X + Y);
	//float DistFinal = floorf(Dist);
	//Drawing.DrawString(ESPFont, this->BottomX, this->TopY, Color(255, 255, 255), FONT_LEFT, "%i", (int)DistFinal);
}

static std::string Ranks[] =
{
	"Unranked",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",
	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",
	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

void CVisuals::DrawRank(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	std::string rankStr = Ranks[Entity->GetRank()];

	Drawing.DrawString(ESPFont, this->BottomX + 4, this->TopY + 46, Color(255, 255, 255), FONT_LEFT, "Rank: %s", rankStr.c_str());
}

void CVisuals::DrawDefuser(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	bool hasDefuser = Entity->GetDefuser();

	if (hasDefuser)
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 6, Color(255, 255, 255), FONT_RIGHT, "Has Defuser");
	else
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 6, Color(255, 255, 255), FONT_RIGHT, "No Defuser");
}

void CVisuals::DrawPing(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	int ping = Entity->GetPing();

	Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 16, Color(255, 255, 255), FONT_RIGHT, "Ping: %i", ping);
}

void CVisuals::DrawHasBomb(CBaseEntity* Entity)
{
	bool hasBomb = Entity->HasC4();

	if (hasBomb)
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 26, Color(255, 255, 255), FONT_RIGHT, "Has Bomb");
	else
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 26, Color(255, 255, 255), FONT_RIGHT, "No Bomb");
}

void CVisuals::DrawIsDefusing(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	bool defusing = Entity->GetDefusing();

	if (defusing)
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 36, Color(0, 255, 0), FONT_RIGHT, "Is Defusing");
}

void CVisuals::DrawMoney(CBaseEntity* pEntity)
{
	if (!pEntity)
		return;
	int money = pEntity->GetMoney();

	Drawing.DrawString(ESPFont, this->BottomX + 4, this->TopY + 16, Color(255, 255, 255), FONT_LEFT, "Money: %i", money);
}

void CVisuals::DrawWins(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	int wins = *(int*)(**(DWORD**)(offsets.dwGameResources) + offsets.m_iCompetitiveWins + Entity->GetIndex() * 4);

	Drawing.DrawString(ESPFont, this->BottomX + 4, this->TopY + 6, Color(255, 255, 255), FONT_LEFT, "Wins: %i", wins);
}

void CVisuals::DrawHelmet(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	bool bHasHelmet = Entity->HasHelmet();
	if (bHasHelmet)
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 56, Color(255, 255, 255), FONT_RIGHT, "Has Helmet");
	else
		Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 56, Color(255, 255, 255), FONT_RIGHT, "No Helmet");
}

void CVisuals::DrawArmor(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	int Armor = Entity->GetArmor();

	Drawing.DrawString(ESPFont, this->BottomX + 4, this->TopY + 56, Color(255, 255, 255), FONT_LEFT, "Armor: %i", Armor);
}

void CVisuals::DrawHealth(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	int Health = Entity->GetHealth();

	Drawing.DrawString(ESPFont, this->BottomX + 4, this->TopY + 66, Color(255, 255, 255), FONT_LEFT, "Health: %i", Health);
}