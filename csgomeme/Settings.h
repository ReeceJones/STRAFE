#pragma once
#include <Windows.h>
#include "Vector.h"
#include "SkinChanger.h"

class CSettings
{
public:
	struct
	{
		bool MenuOpened;
		bool FileOpened;
		struct
		{
			bool Save;
		} FileMenu;
	}Menu;

	struct
	{
		bool AntiUntrust = true;
		bool Enabled;
		int AimbotMode;
		int Bone;
		int EntityChecks;
		float AutowallDamage;
		bool AutoShoot;
		bool EnginePrediction;
		int Key;
		float Fov;
		bool SilentAim;
		int NoRecoil;
		bool AntiAim;
		int AntiAimYaw;
		int AntiAimPitch;
		bool AutoStop;
		bool AutoCrouch;
		bool HitchanceEnabled;
		int Hitchance;
		float SmoothX = 1.f;
		float SmoothY = 1.f;
		bool Resolver;
		bool LagCompensation;

		bool SilentFirstXShots;
		int XShots;
		bool DrawAimbotFov;

		bool AimAtTeam;
		bool AutoScope;
		bool NoSpread;
		bool DrawAimspot;

		bool CircleStrafe;
		int CircleKey;

		float AimHeight;
		bool AutoRevolver;
	}Aimbot;
	struct
	{
		bool Bunnyhop;
		bool NameSpam;
		int AutoStrafe;
		bool NameStealer;
		bool FakeLag;
		int FakeLagAmount;
		bool FakeLagOnKey;
		int FakeLagKey;
		char ConfigBuffer[MAX_PATH];
		bool ShowConfig;
		bool ShowSpectators;
		bool ShowSpecTeamOnly;
		bool Radar;
		bool RevealRank;
		bool AutoAccept;
		bool WaterMark = true;
		
		bool NoFlash;
		bool NoSmoke;
		bool AutoPistol;
		int Skybox;
		bool NoSky;
		bool ChangeWallColor;
		float WallColor[3] = { 1.f, 1.f, 1.f };
		float WallAlpha = 1.f;
		bool RainbowWalls;

		bool AnimateClanTag;
		bool CustomClanTag;
		char* ClanTag = "";
		bool MeinKampfMode;
	}Misc;
	struct
	{
		bool Enabled;
		bool Glow;
		float ViewModelFov = 68;
		bool NoVisRecoil;
		float AimPunchFactor;
		bool CustomPlayers;
		bool CustomGloves;
		bool DrawPlayerBoxes;
		float BoxWidth = 1.8f;
		bool DrawName;
		bool DrawWeapon;
		int DrawHealthBar;
		int DrawArmorBar;
		float GlowColorTerrorist[3] = { 1.f, 0.117f, 0.f };
		float GlowColorCounterTerrorist[3] = { 0.f, 0.615f, 1.f };
		float BoxColorTerrorist[3] = { 1.f, 0.117f, 0.f };
		float BoxColorCounterTerrorist[3] = { 0.f, 0.615f, 1.f };
		float ThirdPerson;
		QAngle VisualAngle;
		bool Chams;
		//bool IgnoreZ;
		int VisualStyle;
		int IgnoreZStyle;
		float ChamsColorTerroristVisual[3] = { 1.f, 0.117f, 0.f };;
		float ChamsColorCounterTerroristVisual[3] = { 0.f, 0.615f, 1.f };
		float ChamsColorTerroristIgnoreZ[3] = { 1.f, 0.117f, 0.f };;
		float ChamsColorCounterTerroristIgnoreZ[3] = { 0.f, 0.615f, 1.f };

		bool ChamsOutline;
		float ChamsOutlineColor[3] = { 1.f, 1.f, 1.f };
		bool SpreadCrosshair;
		float SpreadCrosshairColor[3] = { 1.f, 0.f, 0.f };

		bool EnemyOnly;
		bool ShowDefuseKit;
		bool ShowDefusing;
		bool ShowBomb;

		bool ShowBombDetonation;
		bool ShowWeapons;
		bool ShowGrenades;
		bool ShowKits;

		bool ShowChickens;
		bool ShowFish;

		bool DrawMoney;
		bool DrawWins;
		bool DrawRank;
		bool DrawPing;

		bool ChamsHands;
		bool HandsIgnoreZ;
		int HandsType;
		float HandsColorVisible[3] = { 1.f, 1.f, 1.f };
		float HandsColorIgnoreZ[3] = { 1.f, 1.f, 1.f };

		bool ChamsWeapons;
		int WeaponsType;
		float WeaponColorVisible[3] = { 1.f, 1.f, 1.f };

		bool NoScope;
		bool DisablePP;

		float RenderFov = 90.f; //default value set by game

		bool Dlight;
		float Radius;
		float DLightColorTerrorist[3] = { 1.f, 0.117f, 0.f };
		float DLightColorCounterTerrorist[3] = { 0.f, 0.615f, 1.f };

		int RecoilCrosshairType;
		bool FollowRecoil;

		bool Sonar;

		bool Distance;
		bool Location;
		int Spam;
		bool Snaplines;
		bool Barrel;
		int BoxType;

		bool DrawHelmet;
		bool DrawHealth;
		bool DrawArmor;

		int Delay;
	}Visuals;
	struct
	{
		bool TriggerbotEnabled;
		int TriggerMode;
		int EntityChecks;
		float AutowallDamage;
		bool TriggerMagnet;
		int Key;
		float Fov;
		bool SilentAim;
		bool HitchanceEnabled;
		int Hitchance;
		int Bone;
		int ScanType;

		bool Head;
		bool Chest;
		bool Stomach;
		bool RightArm;
		bool LeftArm;
		bool RightLeg;
		bool LeftLeg;

		int Delay;
		int AfterBurst;
		bool SmokeCheck;

		bool DrawTriggerMagnetFov;
		bool AimAtTeam;
		float AimHeight;
	}Triggerbot;
	struct
	{
		bool Enabled;
		int SelectedWeapon;
		int PaintKit;
		int Seed;
		float Wear;
		int Stattrak;
	}SkinChanger;
};
extern CSettings Settings;