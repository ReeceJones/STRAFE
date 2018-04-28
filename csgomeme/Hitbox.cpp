#include "Features.h"
#include "Interfaces.h"
#include "Structs.h"
#include "Enums.h"
#include "Definitions.h"
#include "Math.h"
#include "ModelInfo.h"
#include "Settings.h"
#include <vector>

Vector CAimbot::GetHitboxPosition(CBaseEntity* Entity, int Hitbox)
{
	matrix3x4a_t matrix[128];
	if (!Entity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
		return Vector(0, 0, 0);
	mstudiobbox_t* hitbox = Interfaces::ModelInfo->GetStudioModel(Entity->GetModel())->GetHitboxSet(0)->GetHitbox(Hitbox);
	if (!hitbox)
		return Vector(0, 0, 0);
	Vector vMin, vMax, vCenter;
	g_Math.VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	g_Math.VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
	vCenter = (vMin + vMax) / 2;
	return vCenter;
}
void SetupHitboxs(std::vector<int> &Hitboxes)
{
	Hitboxes.push_back(HITBOX_HEAD);
	Hitboxes.push_back(HITBOX_NECK);
	Hitboxes.push_back(HITBOX_LOWER_NECK);
	Hitboxes.push_back(HITBOX_PELVIS);
	Hitboxes.push_back(HITBOX_BODY);
	Hitboxes.push_back(HITBOX_THORAX);
	Hitboxes.push_back(HITBOX_CHEST);
	Hitboxes.push_back(HITBOX_UPPER_CHEST);
	Hitboxes.push_back(HITBOX_RIGHT_THIGH);
	Hitboxes.push_back(HITBOX_LEFT_THIGH);
	Hitboxes.push_back(HITBOX_RIGHT_CALF);
	Hitboxes.push_back(HITBOX_LEFT_CALF);
	Hitboxes.push_back(HITBOX_RIGHT_FOOT);
	Hitboxes.push_back(HITBOX_LEFT_FOOT);
	Hitboxes.push_back(HITBOX_RIGHT_HAND);
	Hitboxes.push_back(HITBOX_LEFT_HAND);
	Hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
	Hitboxes.push_back(HITBOX_RIGHT_FOREARM);
	Hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
	Hitboxes.push_back(HITBOX_LEFT_FOREARM);
}
bool CAimbot::GetBestHitbox(CBaseEntity* Entity, Vector& Hitbox)
{
	if (Settings.Aimbot.AimbotMode == 1)
	{
		std::vector<int> Hitboxes;
		Hitboxes.clear();
		SetupHitboxs(Hitboxes);
		for (int i = 0; i < Hitboxes.size(); i++)
		{
			Vector HitboxPos = this->GetHitboxPosition(Entity, i);
			if (HitboxPos == Vector(0, 0, 0))
				return false;
			if (g_pAutowall->GetDamage(HitboxPos) >= Settings.Aimbot.AutowallDamage)
			{
				Hitbox = HitboxPos;
				return true;
			}
		}
		return false;
	}
	else
	{
		std::vector<int> Hitboxes;
		Hitboxes.clear();
		SetupHitboxs(Hitboxes);
		float fov = Settings.Aimbot.Fov;
		Vector BestHitbox = Vector(0, 0, 0);
		CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int i = 0; i < Hitboxes.size(); i++)
		{
			Vector HitboxPos = this->GetHitboxPosition(Entity, i);
			if (HitboxPos == Vector(0, 0, 0))
				continue;

			QAngle LocalAngle; QAngle angToEnt;
			Interfaces::Engine->GetViewAngles(LocalAngle);
			angToEnt = g_Math.CalcAngle(LocalPlayer->GetEyePosition(), HitboxPos);

			float tempFov = g_Math.GetFov(LocalAngle, angToEnt);

			if (tempFov < fov)
			{
				fov = tempFov;
				BestHitbox = HitboxPos;
			}

		}
		if (BestHitbox != Vector(0, 0, 0))
		{
			Hitbox = BestHitbox;
			return true;
		}

		return false;
	}
	return false;
}

bool CAimbot::GetBestHitboxT(CBaseEntity* Entity, Vector& Hitbox)
{
	std::vector<int> pHitboxes;
	pHitboxes.clear();
	SetupHitboxs(pHitboxes);
	float fov = Settings.Triggerbot.Fov;
	Vector BestHitbox = Vector(0, 0, 0);
	CBaseEntity* LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	for (int i = 0; i < pHitboxes.size(); i++)
	{
		Vector HitboxPos = this->GetHitboxPosition(Entity, pHitboxes[i]);
		if (HitboxPos == Vector(0, 0, 0))
			continue;

		QAngle LocalAngle; QAngle angToEnt;
		Interfaces::Engine->GetViewAngles(LocalAngle);
		angToEnt = g_Math.CalcAngle(LocalPlayer->GetEyePosition(), HitboxPos);

		float tempFov = g_Math.GetFov(LocalAngle, angToEnt);

		if (tempFov < fov)
		{
			fov = tempFov;
			BestHitbox = HitboxPos;
		}

		}
	if (BestHitbox != Vector(0, 0, 0))
	{
			Hitbox = BestHitbox;
		return true;
	}
	return false;
}