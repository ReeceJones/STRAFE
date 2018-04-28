#include "Features.h"
#include "Definitions.h"
#include "Entity.h"
#include "Interfaces.h"
#include "Settings.h"
#include "Math.h"

void CAimbot::EnginePrediction(CUserCmd* pCmd)
{
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto MoveData = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(Interfaces::GameMovement) + 0x8);
	if (!Interfaces::MoveHelper)return;
	if (!Interfaces::Engine->IsInGame() && !Interfaces::Engine->IsConnected()) return;
	if (!MoveData) return;

	auto oldFrame = Interfaces::GlobalVarsBase->frametime;
	Interfaces::GlobalVarsBase->frametime = Interfaces::GlobalVarsBase->interval_per_tick;
	Interfaces::MoveHelper->SetHost(LocalPlayer);
	Interfaces::Prediction->SetupMove(LocalPlayer, pCmd, nullptr, MoveData);
	Interfaces::GameMovement->ProcessMovement(LocalPlayer, MoveData);
	Interfaces::Prediction->FinishMove(LocalPlayer, pCmd, MoveData);
	Interfaces::MoveHelper->SetHost(nullptr);
	Interfaces::GlobalVarsBase->frametime = oldFrame;
}