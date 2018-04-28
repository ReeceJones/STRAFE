#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"
#include "FindPattern.h"
#include "SpoofedConvar.h"

#include <xstring>
void CMisc::DoMisc_CreatemovePreAimbot(CUserCmd* pCmd, bool &bSendPacket)
{
	if (Settings.Misc.Bunnyhop)
		this->Bunnyhop(pCmd);
	if (Settings.Misc.AutoStrafe == 1)
		this->NormalAutoStrafe(pCmd);
	if (Settings.Misc.AutoStrafe == 2)
		this->SilentAutoStrafe(pCmd);
	if (Settings.Misc.AutoStrafe == 3)
		this->RageAutoStrafe(pCmd);
	if (Settings.Aimbot.CircleStrafe)
		this->CircleStrafer(pCmd);
}

void CMisc::DoMisc_CreatemovePostAimbot(CUserCmd* pCmd, bool &bSendPacket)
{
	if (Settings.Misc.FakeLag)
		this->FakeLag(pCmd, bSendPacket);
	if (Settings.Misc.RevealRank)
		this->RevealRank();
	if (Settings.Misc.AutoPistol)
		this->AutoPistol(pCmd);
	if (Settings.Misc.NoFlash)
		this->NoFlash();
	this->NoSmoke();
	if (Settings.Misc.CustomClanTag)
		this->ChangeClanTag(Settings.Misc.ClanTag);
	this->MeinKampf();
}

void CMisc::DoMisc_BeginFrame()
{
	if (Settings.Misc.NameSpam)
		this->SpamName("autism");
	if (Settings.Misc.NameStealer)
		this->NameStealer();
	if (Settings.Misc.Radar)
		this->Radar();
}

void CMisc::Radar()
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
	{
		CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (LocalPlayer)
		{
			for (int i = 0; i < Interfaces::EntityList->GetHighestEntityIndex(); i++)
			{
				CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
				if (!pEntity || pEntity == nullptr)
					continue;
				if (pEntity->GetTeam() == LocalPlayer->GetTeam())
					continue;
				if (!pEntity->GetAlive())
					continue;
				*pEntity->Spotted() = true;
			}
		}
	}
}

void CMisc::MeinKampf()
{
	static ConVar* mein = Interfaces::Cvar->FindVar("mat_showlowresimage");
	mein->nFlags &= ~FCVAR_CHEAT;
	static SpoofedConvar* kampf = new SpoofedConvar(mein);

	if (Settings.Misc.MeinKampfMode)
		kampf->SetBool(true);
	else
		kampf->SetBool(false);
	mein->bRegistered = true;
}

void CMisc::ChangeClanTag(std::string lmao)
{
	using SetClanTag = void(__fastcall*)(const char*, const char*);
	static SetClanTag oSetClanTag;
	static SetClanTag pSetClanTag = reinterpret_cast<SetClanTag>(FindPattern->IDA("engine.dll", "53 56 57 8B DA 8B F9 FF 15 ?? ?? ?? ??"));

	if (!Settings.Misc.AnimateClanTag)
		pSetClanTag(Settings.Misc.ClanTag, Settings.Misc.ClanTag);
	else
	{
		std::string temp = Settings.Misc.ClanTag;
		static int i = 0;
		static int shift = 0;
		if (i >= 30)
		{
			std::string newaagw = "";
			for (int i = 0; i < shift; i++)
				newaagw.push_back(temp[i]);
			pSetClanTag(newaagw.c_str(), Settings.Misc.ClanTag);
			if (shift >= temp.size())
				shift = 0;
			else
				shift += 1;
			i = 0;
		}
		else
			i += 1;
	}
}

void CMisc::RevealRank()
{
	static DWORD RankReveal = FindPattern->IDA("client.dll", "8B EC 8B 0D ? ? ? ? 68") - 1;
	static MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankReveal = (MsgFunc_ServerRankRevealAllFn)RankReveal;
	float fArray[3] = { 0.f, 0.f, 0.f };
	MsgFunc_ServerRankReveal(fArray);
}

void CMisc::NoFlash()
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!LocalPlayer || LocalPlayer == nullptr)
		return;
	float* alpha = (float*)((DWORD)LocalPlayer + offsets.m_flFlashMaxAlpha);
	*alpha = 0.f;
}

void CMisc::NoSmoke()
{
	static ConVar* r_drawparticles = Interfaces::Cvar->FindVar("r_drawparticles");
	static SpoofedConvar* s_r_drawparticles = new SpoofedConvar(r_drawparticles);

	if (Settings.Misc.NoSmoke)
		s_r_drawparticles->SetBool(false);
	else
		s_r_drawparticles->SetBool(true);
}

void CMisc::AutoPistol(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!LocalPlayer || LocalPlayer == nullptr)
		return;
	CBaseCombatWeapon* LocalWeapon = LocalPlayer->GetWeapon();
	if (!LocalWeapon || LocalWeapon == nullptr)
		return;

	if (pCmd->buttons & IN_ATTACK && LocalPlayer->GetWeapon()->GetWeaponType() == WEAPONTYPE_PISTOL && LocalPlayer->GetWeapon()->GetWeaponID() != WEAPON_CZ75A)
	{
		//if (g_pAimbot->CanShoot())
		//{
			static bool bSwitch = false;

			if (bSwitch && g_pAimbot->CanShoot())
				pCmd->buttons |= IN_ATTACK;
			else
				pCmd->buttons &= ~IN_ATTACK;

			bSwitch = !bSwitch;
		//}
	}
}


bool CMisc::CircleStrafer(CUserCmd* pCmd)
{
	CBaseEntity * Local = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	QAngle View(pCmd->viewangles);
	float dildo = 0;
	if (GetAsyncKeyState(Settings.Aimbot.CircleKey))
	{
		pCmd->forwardmove = 450.f;
		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = View.y;
		if (Local->GetVelocity().Length() > 50.f)
		{
			dildo += 0.00007;
			current_y += 3 - dildo;

		}
		else
		{
			dildo = 0;

		}
		View.y = current_y;
		if (random == random2)
			View.y += random;
		g_pAimbot->ClampAngles(pCmd);
		pCmd->viewangles = View;
	}
	else
	{
		float dildo = 0;
	}
	return false;
}

CMisc* g_pMisc = new CMisc;