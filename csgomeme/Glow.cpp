#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "GlowManager.h"


void CVisuals::Glow()
{
	CGlowObjectManager* GlowManager = (CGlowObjectManager*)offsets.GlowManager;
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
	{
		CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		for (int i = 0; i < GlowManager->size; ++i)
		{
			CGlowObjectManager::GlowObjectDefinition_t* GlowEntity = &GlowManager->m_GlowObjectDefinitions[i];
			GlowEntity->m_flGlowAlpha = 1.f;
			CBaseEntity* Entity = GlowEntity->getEntity();
			if (Entity == LocalPlayer && Settings.Visuals.ThirdPerson == 0.f) //add trace fraction stuff like was suggested so that the third person doesnt clip through walls
				continue;
			if (!GlowEntity->IsEmpty() && Entity && !Entity->GetDormant() && Entity->GetClientClass()->m_ClassID == 35)
			{
				int PlayerTeam = Entity->GetTeam();
				if (Settings.Visuals.EnemyOnly && PlayerTeam == LocalPlayer->GetTeam())
					continue;

				switch (PlayerTeam)
				{
				case Terrorists:
					GlowEntity->set(Color(Settings.Visuals.GlowColorTerrorist[0] * 255, Settings.Visuals.GlowColorTerrorist[1] * 255, Settings.Visuals.GlowColorTerrorist[2] * 255));
					break;
				case CounterTerrorists:
					GlowEntity->set(Color(Settings.Visuals.GlowColorCounterTerrorist[0] * 255, Settings.Visuals.GlowColorCounterTerrorist[1] * 255, Settings.Visuals.GlowColorCounterTerrorist[2] * 255));
					break;
				}
			}
		}
	}
}
