#include "Features.h"
#include "Settings.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Util.h"
#include "Math.h"

void CVisuals::DrawBox(CBaseEntity* Entity)
{
	Color EntityColorTerrorist = Color(Settings.Visuals.BoxColorTerrorist[0] * 255, Settings.Visuals.BoxColorTerrorist[1] * 255, Settings.Visuals.BoxColorTerrorist[2] * 255);
	Color EntityColorCounterTerrorist = Color(Settings.Visuals.BoxColorCounterTerrorist[0] * 255, Settings.Visuals.BoxColorCounterTerrorist[1] * 255, Settings.Visuals.BoxColorCounterTerrorist[2] * 255);
	Color RenderColor;
	
	switch (Entity->GetTeam())
	{
	case Terrorists:
		RenderColor = EntityColorTerrorist;
		break;
	case CounterTerrorists:
		RenderColor = EntityColorCounterTerrorist;
		break;
	}/*
	Interfaces::Surface->DrawSetColor(RenderColor);
	Interfaces::Surface->DrawOutlinedRect(this->TopX, this->TopY, this->BottomX, this->BottomY);
	Interfaces::Surface->DrawSetColor(Color(0, 0, 0));
	Interfaces::Surface->DrawOutlinedRect(this->TopX - 1, this->TopY - 1, this->BottomX + 1, this->BottomY + 1);*/
	if (Settings.Visuals.BoxType == 0)
	{
		Drawing.DrawOutlinedRect(this->TopX, this->TopY, this->BottomX, this->BottomY, RenderColor);
		Drawing.DrawOutlinedRect(this->TopX - 1, this->TopY - 1, this->BottomX + 1, this->BottomY + 1, Color(0, 0, 0));
	}
	else if (Settings.Visuals.BoxType == 1)
	{
		int width = this->TopX - this->BottomX;
		int indent = width / 4;
		//8 outlines
		Drawing.DrawLine(this->BottomX - 1, this->BottomY - 1, this->BottomX + indent, this->BottomY - 1, Color(0, 0, 0)); //upper left
		Drawing.DrawLine(this->BottomX - 1, this->BottomY - 1, this->BottomX - 1, this->BottomY + indent, Color(0, 0, 0));

		Drawing.DrawLine(this->TopX + 1, this->BottomY - 1, this->TopX - indent, this->BottomY - 1, Color(0, 0, 0)); //upper right
		Drawing.DrawLine(this->TopX + 1, this->BottomY - 1, this->TopX + 1, this->BottomY + indent, Color(0, 0, 0));

		Drawing.DrawLine(this->BottomX - 1, this->TopY + 1, this->BottomX + indent, this->TopY + 1, Color(0, 0, 0)); //bottom left
		Drawing.DrawLine(this->BottomX - 1, this->TopY + 1, this->BottomX - 1, this->TopY - indent, Color(0, 0, 0));

		Drawing.DrawLine(this->TopX + 1, this->TopY + 1, this->TopX - indent, this->TopY + 1, Color(0, 0, 0)); //bottom right
		Drawing.DrawLine(this->TopX + 1, this->TopY + 1, this->TopX + 1, this->TopY - indent, Color(0, 0, 0));

		//8 lines
		Drawing.DrawLine(this->BottomX, this->BottomY, this->BottomX + indent, this->BottomY, RenderColor); //upper left
		Drawing.DrawLine(this->BottomX, this->BottomY, this->BottomX, this->BottomY + indent, RenderColor);

		Drawing.DrawLine(this->TopX, this->BottomY, this->TopX - indent, this->BottomY, RenderColor); //upper right
		Drawing.DrawLine(this->TopX, this->BottomY, this->TopX, this->BottomY + indent, RenderColor);

		Drawing.DrawLine(this->BottomX, this->TopY, this->BottomX + indent, this->TopY, RenderColor); //bottom left
		Drawing.DrawLine(this->BottomX, this->TopY, this->BottomX, this->TopY - indent, RenderColor);

		Drawing.DrawLine(this->TopX, this->TopY, this->TopX - indent, this->TopY, RenderColor); //bottom right
		Drawing.DrawLine(this->TopX, this->TopY, this->TopX, this->TopY - indent, RenderColor);
	}
	else if (Settings.Visuals.BoxType == 2)
	{



		Vector vecOrigin = Entity->GetOrigin();
		Vector vecViewOffset = Entity->GetEyePosition();
		int width = 14; 
		int additionalHeight = 6;

		Vector a, b;
		WorldToScreen(vecOrigin + Vector(width, width, 0), a);
		WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(-width, width, 0), a);
		WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(-width, -width, 0), a);
		WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(width, -width, 0), a);
		WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);
		///////////////////////
		WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight), a);
		WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecViewOffset + Vector(width, width, additionalHeight), a);
		WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight), a);
		WorldToScreen(vecViewOffset + Vector(-width, width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecViewOffset + Vector(width, -width, additionalHeight), a);
		WorldToScreen(vecViewOffset + Vector(-width, -width, additionalHeight), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);
		///////////////////////
		WorldToScreen(vecOrigin + Vector(width, width, 0), a);
		WorldToScreen(vecOrigin + Vector(width, -width, 0), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(width, width, 0), a);
		WorldToScreen(vecOrigin + Vector(-width, width, 0), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(-width, -width, 0), a);
		WorldToScreen(vecOrigin + Vector(-width, width, 0), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);

		WorldToScreen(vecOrigin + Vector(width, -width, 0), a);
		WorldToScreen(vecOrigin + Vector(-width, -width, 0), b);
		Drawing.DrawLine(a.x, a.y, b.x, b.y, RenderColor);
	}
}

void CVisuals::DrawHealthBar(CBaseEntity* Entity)
{
	int Health = Entity->GetHealth();
	int Height = this->TopY - this->BottomY;
	int FixedLineHeight = Height - ((Height * Health) / 100);
	if (Settings.Visuals.DrawHealthBar == 1)
	{
		Drawing.DrawLine(this->TopX - 5, this->TopY - FixedLineHeight, this->TopX - 5, this->BottomY, Color(0, 255, 0));
		Drawing.DrawOutlinedRect(this->TopX - 6, this->TopY - 1, this->TopX - 3, this->BottomY + 1, Color(0, 0, 0));
	}
	else
	{
		Drawing.DrawLine(this->BottomX + 4, this->TopY - FixedLineHeight, this->BottomX + 4, this->BottomY, Color(0, 255, 0));
		Drawing.DrawOutlinedRect(this->BottomX + 3, this->TopY - 1, this->BottomX + 6, this->BottomY + 1, Color(0, 0, 0));
	}
}

void CVisuals::DrawArmorBar(CBaseEntity* Entity)
{
	int Armor = Entity->GetArmor();
	int Height = this->TopY - this->BottomY;
	int FixedLineHeight = Height - ((Height * Armor) / 100);

	if (Settings.Visuals.DrawArmorBar == 1)
	{
		Drawing.DrawLine(this->TopX - 5, this->TopY - FixedLineHeight, this->TopX - 5, this->BottomY, Color(0, 100, 255));
		Drawing.DrawOutlinedRect(this->TopX - 6, this->TopY - 1, this->TopX - 3, this->BottomY + 1, Color(0, 0, 0));
	}
	else
	{
		Drawing.DrawLine(this->BottomX + 4, this->TopY - FixedLineHeight, this->BottomX + 4, this->BottomY, Color(0, 100, 255));
		Drawing.DrawOutlinedRect(this->BottomX + 3, this->TopY - 1, this->BottomX + 6, this->BottomY + 1, Color(0, 0, 0));
	}

	//Drawing.DrawLine(this->BottomX + 4, this->TopY - FixedLineHeight, this->BottomX + 4, this->BottomY, Color(0, 100, 255));
	//Drawing.DrawOutlinedRect(this->BottomX + 3, this->TopY - 1, this->BottomX + 6, this->BottomY + 1, Color(0, 0, 0));
}

void CVisuals::DrawDistance(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	int distance = (int)Entity->GetOrigin().Dist(LocalPlayer->GetOrigin());
	Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 46, Color(255, 255, 255), FONT_RIGHT, "Dist: %i", distance);
}

void CVisuals::DrawLocation(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	char* Location = reinterpret_cast<char*>(DWORD(Entity) + offsets.m_szLastPlaceName);
	Drawing.DrawString(ESPFont, this->TopX - 4, this->TopY + 56, Color(255, 255, 255), FONT_RIGHT, "%s", Location);
}

void CVisuals::DrawSnapLine(CBaseEntity* Entity)
{
	if (!Entity)
		return;

	Vector vOrigin = Entity->GetOrigin();
	Vector vScr;
	if (this->WorldToScreen(vOrigin, vScr))
	{
		Color EntityColorTerrorist = Color(Settings.Visuals.BoxColorTerrorist[0] * 255, Settings.Visuals.BoxColorTerrorist[1] * 255, Settings.Visuals.BoxColorTerrorist[2] * 255);
		Color EntityColorCounterTerrorist = Color(Settings.Visuals.BoxColorCounterTerrorist[0] * 255, Settings.Visuals.BoxColorCounterTerrorist[1] * 255, Settings.Visuals.BoxColorCounterTerrorist[2] * 255);
		Color RenderColor;

		switch (Entity->GetTeam())
		{
		case Terrorists:
			RenderColor = EntityColorTerrorist;
			break;
		case CounterTerrorists:
			RenderColor = EntityColorCounterTerrorist;
			break;
		}
		int width, height;
		Interfaces::Engine->GetScreenSize(width, height);
		Drawing.DrawLine(width / 2, height, vScr.x, vScr.y, RenderColor);
	}

}

void CVisuals::DrawBarrel(CBaseEntity* Entity)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Entity == LocalPlayer)
		return;

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	g_Math.AngleVectors(*Entity->GetEyeAngles(), forward);
	//g_Math.AngleVectors(Entity->GetEyeAngles(), &forward);
	filter.pSkip = Entity;
	src3D = Entity->GetBonePosition(8) - Vector(0, 0, 0);
	dst3D = src3D + (forward * 8124);

	ray.Init(src3D, dst3D);

	Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!WorldToScreen(src3D, src) || !WorldToScreen(tr.endpos, dst))
		return;

	Drawing.DrawLine(src.x, src.y, dst.x, dst.y, Color(255, 255, 255));
	Drawing.DrawRect(dst.x - 3, dst.y - 3, 6, 6, Color(255, 255, 255));
}

void CVisuals::Callout(CBaseEntity* Entity)
{
	if (!Entity)
		return;
	CBaseCombatWeapon* pWeapon = Entity->GetWeapon();
	if (!pWeapon)
		return;
	const char* Location = reinterpret_cast<char*>(DWORD(Entity) + offsets.m_szLastPlaceName);
	int Health = Entity->GetHealth();
	auto weaponName = Entity->GetWeapon()->GetWeaponName();
	player_info_t info;
	info = Entity->GetPlayerInfo();

	static int i = 0;
	i++;


	if (i == Settings.Visuals.Delay)
	{
		char pszBuffer[128];
		if (Settings.Visuals.Spam == 2)
			sprintf_s(pszBuffer, "say Player: %s | Health: %i HP | Location: %s | Weapon: %s", info.name, Health, Location, weaponName.c_str());
		else
			sprintf_s(pszBuffer, "say_team Player: %s | Health: %i HP | Location: %s | Weapon: %s", info.name, Health, Location, weaponName.c_str());

		Interfaces::Engine->ClientCmd(pszBuffer);
	}

	if (i >= Settings.Visuals.Delay)
		i = 0;
}