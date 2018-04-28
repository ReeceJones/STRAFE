#include "Features.h"
#include "Settings.h"
#include "Interfaces.h"

void CMisc::FakeLag(CUserCmd* pCmd, bool &bSendPacket)
{
	if ((Settings.Misc.FakeLagOnKey && GetAsyncKeyState(Settings.Misc.FakeLagKey)) || !Settings.Misc.FakeLagOnKey)
	{
		CBaseEntity *LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		static int ChokedPackets = 0;

		if (ChokedPackets < Settings.Misc.FakeLagAmount && LocalPlayer->GetAlive() && LocalPlayer && LocalPlayer != nullptr && LocalPlayer->GetHealth() > 0)
			bSendPacket = false;
		else
			bSendPacket = true;

		if (ChokedPackets > Settings.Misc.FakeLagAmount)
			ChokedPackets = 0;
		else
			ChokedPackets += 1;
	}
}
