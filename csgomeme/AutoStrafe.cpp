#include "InputSystem.h"
#include "Interfaces.h"
#include "Math.h"
#include "Features.h"

void CMisc::NormalAutoStrafe(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (GetAsyncKeyState(0x57)
		|| GetAsyncKeyState(0x41)
		|| GetAsyncKeyState(0x53)
		|| GetAsyncKeyState(0x44)
		|| !GetAsyncKeyState(0x20))
		return;
	if (LocalPlayer->GetFlags() & FL_ONGROUND)
		return;

	if (GetAsyncKeyState(VK_SPACE) && !(LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -400.f;
		}
		if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 400.f;
		}
	}
}

void CMisc::SilentAutoStrafe(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (GetAsyncKeyState(0x57) 
		|| GetAsyncKeyState(0x41) 
		|| GetAsyncKeyState(0x53) 
		|| GetAsyncKeyState(0x44) 
		|| !GetAsyncKeyState(0x20)) 
		return;

	if (LocalPlayer->GetFlags() & FL_ONGROUND)
		return;
	if (GetAsyncKeyState(VK_SPACE) && (LocalPlayer->GetFlags() & FL_ONGROUND) && LocalPlayer->GetVelocity().Length() <= 60.f)
	{
		pCmd->forwardmove = 450.f;
	}
	auto Delta = 0.f;
	if (LocalPlayer->GetVelocity().Length() > 60.f)
	{
		auto x = 30.f;
		auto z = x / LocalPlayer->GetVelocity().Length();
		z = fabsf(z);
		auto a = x * z;
		Delta = a;
	}
	if (GetAsyncKeyState(VK_SPACE) && !(LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (pCmd->mousedx < 0)
			pCmd->sidemove = -450;
		if (pCmd->mousedx > 0)
			pCmd->sidemove = 450;
		if (pCmd->tick_count % 2)
		{
			pCmd->sidemove = -450;
		}
		else
		{
			pCmd->viewangles.y += Delta;
			pCmd->sidemove = 450;
		}
	}
}

void CMisc::RageAutoStrafe(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (GetAsyncKeyState(0x57)
		|| GetAsyncKeyState(0x41) 
		|| GetAsyncKeyState(0x53) 
		|| GetAsyncKeyState(0x44) 
		|| !GetAsyncKeyState(0x20)) 
		return;

	if (LocalPlayer->GetFlags() & FL_ONGROUND)
		return;
	if (GetAsyncKeyState(VK_SPACE) && LocalPlayer->GetVelocity().Length() <= 30.f)
		pCmd->forwardmove = 450.f;
	auto Delta = 0.f;
	if (LocalPlayer->GetVelocity().Length() > 30.f)
	{
		auto x = 30.f;
		auto z = x / LocalPlayer->GetVelocity().Length();
		z = fabsf(z);
		auto a = x * z;
		Delta = a;
	}
	if (GetAsyncKeyState(VK_SPACE) && !(LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (pCmd->mousedx < 0)
			pCmd->sidemove = -450;
		if (pCmd->mousedx > 0)
			pCmd->sidemove = 450;
		if (pCmd->tick_count % 2)
		{
			pCmd->viewangles.y -= Delta;
			pCmd->sidemove = -450;
		}
		else
		{
			pCmd->viewangles.y += Delta;
			pCmd->sidemove = 450;
		}
	}
}