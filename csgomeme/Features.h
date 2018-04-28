#pragma once
#include "InputSystem.h"
#include "Entity.h"
#include "Trace.h"
#include "Vector.h"
#include "Structs.h"
#include "Drawing.h"

extern bool SendPacket;
extern HFont ESPFont;

class CMisc
{
public:
	void DoMisc_CreatemovePreAimbot(CUserCmd* pCmd, bool &bSendPacket);
	void DoMisc_CreatemovePostAimbot(CUserCmd* pCmd, bool &bSendPacket);
	void ChangeName(const char* name);
	void DoMisc_BeginFrame();
private:
	void AutoPistol(CUserCmd* pCmd);
	void NoFlash();
	void NoSmoke();
	void RevealRank();
	void Radar();
	void NormalAutoStrafe(CUserCmd* pCmd);
	void SilentAutoStrafe(CUserCmd* pCmd);
	void RageAutoStrafe(CUserCmd* pCmd);
	void Bunnyhop(CUserCmd* pCmd);
	void SpamName(const char* name);
	void NameStealer();
	void FakeLag(CUserCmd* pCmd, bool &bSendPacket);
	bool CircleStrafer(CUserCmd* pCmd);
	void ChangeClanTag(std::string lmao);
	void MeinKampf();
}; extern CMisc* g_pMisc;

class CAimbot
{
public:
	void DoAimbot(CUserCmd* pCmd, bool &bSendPacket);
	void EnginePrediction(CUserCmd* pCmd);
	void ClampAngles(CUserCmd* pCmd);
	void AntiAim(CUserCmd* pCmd, bool &bSendPacket);
	bool CanShoot();
	void Stop(CUserCmd* pCmd);
	void Crouch(CUserCmd* pCmd);
	int m_iBestEnt;
	Vector m_vBestPos;
	bool GetBestHitboxT(CBaseEntity* Entity, Vector& Hitbox);
private:
	bool ValidTarget(CBaseEntity* Entity);
	int GetBestTarget(CUserCmd* pCmd, Vector &vPosition);
	void NoRecoil(CUserCmd* pCmd, QAngle &Angle = QAngle(NULL, NULL, NULL));
	bool ShouldShoot(CBaseEntity* Entity);
	Vector GetHitboxPosition(CBaseEntity* Entity, int Hitbox);
	bool GetBestHitbox(CBaseEntity* Entity, Vector& Hitbox);
	float GetLagCompensation();
	bool isAimbotting;
}; extern CAimbot* g_pAimbot;

class CVisuals
{
public:
	void DoVisuals_Createmove();
	void DoVisuals_PaintTraverse();
	void DoVisuals_OverrideView();
	bool WorldToScreen(Vector& vOrigin, Vector& vScreen);
private:
	void Glow();
	void DrawBox(CBaseEntity* Entity);
	void DrawName(CBaseEntity* Entity);
	void DrawWeapon(CBaseEntity* Entity);
	void DrawHealthBar(CBaseEntity* Entity);
	void DrawArmorBar(CBaseEntity* Entity);
	void DrawDefuser(CBaseEntity* Entity);
	void DrawPing(CBaseEntity* Entity);
	void DrawHasBomb(CBaseEntity* Entity);
	void DrawIsDefusing(CBaseEntity* Entity);
	void DrawMoney(CBaseEntity* Entity);
	void DrawWins(CBaseEntity* Entity);
	void DrawEntities();
	void ThirdPerson();
	void DrawRank(CBaseEntity* Entity);
	void DrawDistance(CBaseEntity* Entity);
	void DrawLocation(CBaseEntity* Entity);
	void DrawSnapLine(CBaseEntity* Entity);
	void DrawBarrel(CBaseEntity* Entity);
	void Callout(CBaseEntity* Entity);
	void DrawHelmet(CBaseEntity* Entity);
	void DrawArmor(CBaseEntity* Entity);
	void DrawHealth(CBaseEntity* Entity);

	int TopX, TopY, BottomX, BottomY;
}; extern CVisuals* g_pVisuals;
struct FireBulletData
{
	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class CAutowall
{
public:
	float GetDamage(const Vector& point);
private:
	float GetHitgroupDamageMultiplier(int HitGroup);
	void ScaleDamage(int HitGroup, CBaseEntity* Entity, float ArmorRatio, float &CurrentDamage);
	bool TraceToExit(Vector &End, trace_t *TraceEnter, Vector Start, Vector Direcion, trace_t *TraceExit);
	bool HandleBulletPenetration(WeaponInfo_t* WeaponInfo, FireBulletData &data);
	bool SimulateFireBullet(CBaseCombatWeapon* pWeapon, FireBulletData &data);
}; extern CAutowall* g_pAutowall;

class CDrawing
{
public:
	void DrawCircle(float x, float y, float r, float s, Color color);
	void DrawString(HFont font, int x, int y, Color color, DWORD alignment, const char* msg, ...);
	void DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...);
	void DrawRect(int x, int y, int w, int h, Color col);
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow);
	void DrawRectGradientHorizontal(int x, int y, int width, int height, Color color1, Color color2);
	void DrawPixel(int x, int y, Color col);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void DrawOutlinedCircle(int x, int y, int r, Color col);
	void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw);
	void DrawPolygon(int count, Vertex_t* Vertexs, Color color);
	void DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col);
	bool ScreenTransform(const Vector &point, Vector &screen);
	int GetStringWidth(HFont font, const char* msg, ...);
}; extern CDrawing Drawing;

class CTriggerbot
{
public:
	void DoTriggerbot(CUserCmd* pCmd);
private:
	bool ValidTargetT(CBaseEntity* Entity);
	int GetBestTargetT(CUserCmd* pCmd, Vector &vPosition);
	bool ShouldShootT(CBaseEntity* Entity);
}; extern CTriggerbot* g_pTriggerbot;