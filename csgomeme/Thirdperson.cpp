#include "Features.h"
#include "Settings.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Convar.h"
#include "SpoofedConvar.h"

void CVisuals::ThirdPerson()
{
	CBaseEntity *LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static QAngle vecAngles;
	if (LocalPlayer->GetAlive())
	{
		if (!Interfaces::Input->m_fCameraInThirdPerson)
		{
			Interfaces::Engine->GetViewAngles(vecAngles);
			Interfaces::Input->m_fCameraInThirdPerson = true;
			Interfaces::Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, Settings.Visuals.ThirdPerson);
		}
	}
}