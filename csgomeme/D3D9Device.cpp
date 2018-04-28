
#include "SkinChanger.h"
#include "Hooks.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx9.h"
#include "Settings.h"
#include "Convar.h"
#include "Interfaces.h"
#include "Config.h"
#include "Gui.h"
#include <ctime>
#include "Features.h"

#define D3DFVF_TL (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
PresentFn Present_Original;
tReset ResetScene_Original;
WNDPROC WndProc_Original;
HWND Window;
bool CurKey[256] = {};
void GUI_Init(IDirect3DDevice9* pDevice);
static LPDIRECT3DTEXTURE9       g_FontTexture = NULL;
static bool Initialized = false;
CSettings Settings;
ConVar* CVar = new ConVar;
static bool AimTab = true;
static bool VisualsTab = false;
static bool MiscTab = false;
static bool TriggerTab = false;
ImFont* Tahoma;

const char* Keys[] =
{
	"No HotKey",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};

const char* Skyboxes[] =
{
"cs_baggage_skybox_",
"cs_tibet",
"embassy",
"italy",
"jungle",
"nukeblank",
"office",
"sky_cs15_daylight01_hdr",
"sky_cs15_daylight02_hdr",
"sky_cs15_daylight03_hdr",
"sky_cs15_daylight04_hdr",
"sky_csgo_cloudy01",
"sky_csgo_night02",
"sky_csgo_night02b",
"sky_dust",
"sky_venice",
"vertigo",
"vietnam"
};

LRESULT __stdcall Hooks::WndProc_Hooked(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto& io = ImGui::GetIO();
	switch (uMsg)
	{

	case WM_LBUTTONDOWN:
		CurKey[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		CurKey[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		CurKey[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		CurKey[VK_RBUTTON] = false;
		break;
	case WM_KEYDOWN:
		CurKey[wParam] = true;
		break;
	case WM_KEYUP:
		CurKey[wParam] = false;
		break;
	case WM_MOUSEMOVE:
		io.MousePos.x = static_cast<signed short>(lParam);
		io.MousePos.y = static_cast<signed short>(lParam >> 16);
		break;
	default: break;
	}
	{
		static auto isDown = false;
		static auto isClicked = false;
		if (CurKey[VK_INSERT])
		{
			isClicked = false;
			isDown = true;
		}
		else if (!CurKey[VK_INSERT] && isDown)
		{
			isClicked = true;
			isDown = false;
		}
		else
		{
			isClicked = false;
			isDown = false;
		}
		if (isClicked)
		{
			Settings.Menu.MenuOpened = !Settings.Menu.MenuOpened;

			static auto cl_mouseenable = Interfaces::Cvar->FindVar("cl_mouseenable");

			cl_mouseenable->SetValue(!Settings.Menu.MenuOpened);
		}

	}
	if (Initialized && Settings.Menu.MenuOpened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	return CallWindowProc(WndProc_Original, hWnd, uMsg, wParam, lParam);
}


const char* Months[]
{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

long __stdcall Hooks::PresentScene_Hooked(IDirect3DDevice9* pDevice, const RECT* pSourceRec, const RECT* pDestRect, HWND hWnd, const RGNDATA* pDirtyRegion)
{
	if (!Initialized)
	{
		GUI_Init(pDevice);
	}
	else
	{
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); //causes skins to become darker
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //morphes skins together
		DWORD oSRCBLEND, oDESTBLEND;
		pDevice->GetRenderState(D3DRS_SRCBLEND, &oSRCBLEND);
		pDevice->GetRenderState(D3DRS_DESTBLEND, &oDESTBLEND);


			if (!Interfaces::Engine->IsInGame() && !Interfaces::Engine->IsConnected())
				ImGui::GetIO().MouseDrawCursor = true;
			else
				ImGui::GetIO().MouseDrawCursor = Settings.Menu.MenuOpened;
		ImGui_ImplDX9_NewFrame();

			if (Settings.Menu.MenuOpened)
			{
				ImGui::PushFont(Tahoma);
				ImGui::Begin("CSGOMeme V2", &Settings.Menu.MenuOpened, ImVec2(1231, 420), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
				{
					time_t t = time(0);   // get time now
					struct tm * now = localtime( & t );
					bool isAm = true;
					if (now->tm_hour > 12)
					{
						now->tm_hour -= 12;
						isAm = false;
					}
					if (isAm)
						ImGui::Text("																																																																														%i, %s %i | %iam:%im:%is", now->tm_year + 1900, Months[now->tm_mon], now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
					else
						ImGui::Text("																																																																														%i, %s %i | %ipm:%im:%is", now->tm_year + 1900, Months[now->tm_mon], now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
					
					if (ImGui::Button("Aimbot", ImVec2(300.f, 20.f)))
					{
						TriggerTab = false;
						AimTab = true;
						VisualsTab = false;
						MiscTab = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Triggerbot", ImVec2(300.f, 20.f)))
					{
						AimTab = false;
						VisualsTab = false;
						MiscTab = false;
						TriggerTab = true;
					}
					ImGui::SameLine();
					if (ImGui::Button("Visuals", ImVec2(300.f, 20.f)))
					{
						AimTab = false;
						TriggerTab = false;
						VisualsTab = true;
						MiscTab = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Misc", ImVec2(300.f, 20.f)))
					{
						AimTab = false;
						VisualsTab = false;
						TriggerTab = false;
						MiscTab = true;
					}
					float w = ImGui::GetWindowWidth();
					if (AimTab)
					{
						ImGui::Text("Aimbot");
						//ImGui::Text("%f", ImGui::GetWindowWidth());
						ImGui::BeginChild("cAimbot Settings", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Text("Aimbot Settings");
							ImGui::Checkbox("Enabled", &Settings.Aimbot.Enabled);
							ImGui::Combo("Aimbot Mode", &Settings.Aimbot.AimbotMode, "Aim At Bone\0Hitscan\0Closest to Crosshair\0");
							if (Settings.Aimbot.AimbotMode == 0)
								ImGui::Combo("Bone", &Settings.Aimbot.Bone, "Pelvis\0unknown bone1\0unknown bone2\0Spine1\0Spine2\0Spine3\0Neck\0Head\0");
							ImGui::Combo("Entity Checks", &Settings.Aimbot.EntityChecks, "Visible\0Autowall\0");
							if (Settings.Aimbot.EntityChecks == AUTOWALL)
								ImGui::SliderFloat("Autowall Damage", &Settings.Aimbot.AutowallDamage, 0.1f, 100.f);
							ImGui::Combo("Key", &Settings.Aimbot.Key, Keys, ARRAYSIZE(Keys));
							ImGui::SliderFloat("Fov", &Settings.Aimbot.Fov, 0.f, 180.f);
							if (!Settings.Aimbot.AntiAim)
							{
								ImGui::SliderFloat("Smoothing-Y", &Settings.Aimbot.SmoothY, 1.f, 30.f);
								ImGui::SliderFloat("Smoothing-X", &Settings.Aimbot.SmoothX, 1.f, 30.f);
							}
							ImGui::Checkbox("p$ilent First Shots", &Settings.Aimbot.SilentFirstXShots);
							ImGui::SliderInt("Shots", &Settings.Aimbot.XShots, 0, 10);
							ImGui::Checkbox("Aim at team", &Settings.Aimbot.AimAtTeam);
							ImGui::Checkbox("Draw Aimbot Fov", &Settings.Aimbot.DrawAimbotFov);
							ImGui::SliderFloat("Aimheight", &Settings.Aimbot.AimHeight, 0.f, 6.f);
							ImGui::EndChild();
						}
						ImGui::SameLine();
						ImGui::BeginChild("cAimbot Misc", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Text("Aimbot Misc");
							ImGui::Checkbox("Hitchance", &Settings.Aimbot.HitchanceEnabled);
							if (Settings.Aimbot.HitchanceEnabled)
								ImGui::SliderInt("Hitchance", &Settings.Aimbot.Hitchance, 1, 100);
							ImGui::Checkbox("Prediction", &Settings.Aimbot.EnginePrediction);
							ImGui::Checkbox("AutoShoot", &Settings.Aimbot.AutoShoot);
							ImGui::Checkbox("p$ilent", &Settings.Aimbot.SilentAim);
							ImGui::Combo("Recoil", &Settings.Aimbot.NoRecoil, "None\0RCS\0No Recoil");
							ImGui::Checkbox("Anti-Untrusted", &Settings.Aimbot.AntiUntrust); //chill its a meme
							ImGui::Checkbox("AutoStop", &Settings.Aimbot.AutoStop);
							ImGui::Checkbox("AutoCrouch", &Settings.Aimbot.AutoCrouch);
							ImGui::Checkbox("AutoScope", &Settings.Aimbot.AutoScope);
							ImGui::Checkbox("Auto Cock Revolver", &Settings.Aimbot.AutoRevolver);
							ImGui::Checkbox("AntiAim", &Settings.Aimbot.AntiAim);
							if (Settings.Aimbot.AntiAim)
							{
								Settings.Aimbot.SmoothY = 1.f;
								Settings.Aimbot.SmoothX = 1.f;
								ImGui::Combo("AntiAim-Yaw", &Settings.Aimbot.AntiAimYaw, "None\0Backwards\0Spin\0Jitter\0Dynamic Jitter\0Right\0Left\0Real Left\0Real Right 1\0Real Right 2\0Real Forwards\0Real Jitter 1\0Real Jitter 2\0Real Dynamic Jitter\0Real Backwards 1\0Real Backwards 2\0Real Backwards 3\0Real Static\0Moon Walk\0Fast Spin\0Ultra Slow Spin\0Random\0Real Spin 1\0Real Spin 2\0");
								ImGui::Combo("AntiAim-Pitch", &Settings.Aimbot.AntiAimPitch, "None\0Down\0Up\0Half Down\0Half Up\0Jitter\0Dynamic Jitter\0Slow Jitter\0");
							}
							ImGui::Checkbox("Resolver", &Settings.Aimbot.Resolver);
							ImGui::Checkbox("Lag Compensation", &Settings.Aimbot.LagCompensation);
							ImGui::Checkbox("No Spread", &Settings.Aimbot.NoSpread);
							ImGui::Checkbox("Circle Strafer", &Settings.Aimbot.CircleStrafe);
							ImGui::Combo("Circle Strafer Key", &Settings.Aimbot.CircleKey, Keys, IM_ARRAYSIZE(Keys));
							ImGui::EndChild();
						}
					}
					if (TriggerTab)
					{
						//Settings.Triggerbot.TriggerbotEnabled
						ImGui::Text("Triggerbot");
						ImGui::BeginChild("cTriggerbot Settings", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Checkbox("Enabled ##triggerbot", &Settings.Triggerbot.TriggerbotEnabled);
							ImGui::Combo("Triggerbot Mode ##triggerbot", &Settings.Triggerbot.TriggerMode, "Always On\0Hotkey\0");
							if (Settings.Triggerbot.TriggerMode == 1)
								ImGui::Combo("Hotkey ##triggerbot", &Settings.Triggerbot.Key, Keys, ARRAYSIZE(Keys));
							ImGui::Combo("Scan Type", &Settings.Triggerbot.ScanType, "Bone\0Closest To Crosshair\0");
							if (Settings.Triggerbot.ScanType == 0)
								ImGui::Combo("Bone", &Settings.Triggerbot.Bone, "Pelvis\0unknown bone1\0unknown bone2\0Spine1\0Spine2\0Spine3\0Neck\0Head\0");
							ImGui::Combo("Entity Checks ##triggerbot", &Settings.Triggerbot.EntityChecks, "Visible\0Autowall\0");
							if (Settings.Triggerbot.EntityChecks == 1)
								ImGui::SliderFloat("Autowall Damage ##triggerbot", &Settings.Triggerbot.AutowallDamage, 0.f, 100.f);
							ImGui::Checkbox("TriggerMagnet", &Settings.Triggerbot.TriggerMagnet);
							ImGui::SliderFloat("Fov ##triggerbot", &Settings.Triggerbot.Fov, 0.f, 180.f);
							ImGui::SliderFloat("Aimheight ##triggercancer", &Settings.Triggerbot.AimHeight, 0.f, 6.f);
							ImGui::Checkbox("p$ilent ##triggerbot", &Settings.Triggerbot.SilentAim);
							ImGui::Checkbox("Hitchance ##triggerbot", &Settings.Triggerbot.HitchanceEnabled);
							if (Settings.Triggerbot.HitchanceEnabled)
								ImGui::SliderInt("Hitchance ##triggerbot", &Settings.Triggerbot.Hitchance, 0, 100);

							ImGui::Checkbox("Smoke Check", &Settings.Triggerbot.SmokeCheck);
							ImGui::SliderInt("Delay (ms)", &Settings.Triggerbot.Delay, 0, 1000);
							ImGui::SliderInt("After Burst", &Settings.Triggerbot.AfterBurst, 0, 10);

							ImGui::Checkbox("Draw Trigger Magnet Fov", &Settings.Triggerbot.DrawTriggerMagnetFov);
							ImGui::Checkbox("Shoot Teammates", &Settings.Triggerbot.AimAtTeam);
							ImGui::EndChild();
						}
						ImGui::SameLine();
						ImGui::BeginChild("cTriggerbot Filters", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Selectable("Head", &Settings.Triggerbot.Head);
							ImGui::Selectable("Chest", &Settings.Triggerbot.Chest);
							ImGui::Selectable("Stomach", &Settings.Triggerbot.Stomach);
							ImGui::Selectable("Right Arm", &Settings.Triggerbot.RightArm);
							ImGui::Selectable("Left Arm", &Settings.Triggerbot.LeftArm);
							ImGui::Selectable("Right Leg", &Settings.Triggerbot.RightLeg);
							ImGui::Selectable("Left Leg", &Settings.Triggerbot.LeftLeg);
							ImGui::EndChild();
						}
					}
					if (VisualsTab)
					{
						ImGui::Text("Visuals");
						ImGui::BeginChild("cVisual ESP", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Text("ESP");
							ImGui::Checkbox("Enbaled", &Settings.Visuals.Enabled);
							ImGui::Checkbox("Player Boxes", &Settings.Visuals.DrawPlayerBoxes);
							ImGui::SameLine();
							ImGui::ColorEdit3("Box Terrorist", Settings.Visuals.BoxColorTerrorist, ImGuiColorEditFlags_NoSliders);
							ImGui::SameLine();
							ImGui::ColorEdit3("Box Counter-Terrorist", Settings.Visuals.BoxColorCounterTerrorist, ImGuiColorEditFlags_NoSliders);
							ImGui::Combo("Box Type", &Settings.Visuals.BoxType, "Box\0Corner\0Three-Dimensional\0");
							ImGui::Checkbox("Player Name", &Settings.Visuals.DrawName);
							ImGui::Checkbox("Player Weapon", &Settings.Visuals.DrawWeapon);
							ImGui::Combo("Health Bar", &Settings.Visuals.DrawHealthBar, "None\0Left\0Right\0");
							ImGui::Combo("Armor Bar", &Settings.Visuals.DrawArmorBar, "None\0Left\0Right\0");
							ImGui::SliderFloat("Box Width", &Settings.Visuals.BoxWidth, 0.1f, 10.f);
							ImGui::Checkbox("Glow", &Settings.Visuals.Glow);
							ImGui::SameLine();
							ImGui::ColorEdit3("Glow Terrorist", Settings.Visuals.GlowColorTerrorist, ImGuiColorEditFlags_NoSliders);
							ImGui::SameLine();
							ImGui::ColorEdit3("Glow Counter-Terrorist", Settings.Visuals.GlowColorCounterTerrorist, ImGuiColorEditFlags_NoSliders);
							ImGui::Text("Chams");
							ImGui::Checkbox("Chams", &Settings.Visuals.Chams);
							if (Settings.Visuals.Chams) {
								ImGui::Combo("Visual Style", &Settings.Visuals.VisualStyle, "Textured\0Flat\0WireFramed\0Bubblegum\0");
								ImGui::ColorEdit3("Chams Terrorist Visual", Settings.Visuals.ChamsColorTerroristVisual, ImGuiColorEditFlags_NoSliders);
								ImGui::ColorEdit3("Chams Counter-Terrorist Visual", Settings.Visuals.ChamsColorCounterTerroristVisual, ImGuiColorEditFlags_NoSliders);
								//ImGui::Checkbox("Ignore Z Buffer", &Settings.Visuals.IgnoreZ);
								//if (Settings.Visuals.IgnoreZ) 
								//{
									ImGui::Combo("Ignore Z Style", &Settings.Visuals.IgnoreZStyle, "Textured\0Flat\0WireFramed\0Bubblegum\0");
									ImGui::ColorEdit3("Chams Terrorist Ignore Z", Settings.Visuals.ChamsColorTerroristIgnoreZ, ImGuiColorEditFlags_NoSliders);
									ImGui::ColorEdit3("Chams Counter-Terrorist Ignore Z", Settings.Visuals.ChamsColorCounterTerroristIgnoreZ, ImGuiColorEditFlags_NoSliders);
								//}
								ImGui::Checkbox("Outline Chams", &Settings.Visuals.ChamsOutline);
							}
							ImGui::Checkbox("Hands Chams", &Settings.Visuals.ChamsHands);
							if (Settings.Visuals.ChamsHands)
							{
								ImGui::Combo("Hands Type", &Settings.Visuals.HandsType, "None\0Textured\0Flat\0Wireframe\0Bubble\0Color\0Flat Wireframed\0Textured Wireframed\0No Hands\0");
								ImGui::ColorEdit3("Chams Hands Visual", Settings.Visuals.HandsColorVisible, ImGuiColorEditFlags_NoSliders);
								ImGui::ColorEdit3("Chams Hands Ignore Z", Settings.Visuals.HandsColorIgnoreZ, ImGuiColorEditFlags_NoSliders);
								ImGui::Checkbox("Hands Ignore Z", &Settings.Visuals.HandsIgnoreZ);
							}
							ImGui::Checkbox("Weapons Chams", &Settings.Visuals.ChamsWeapons);
							if (Settings.Visuals.ChamsWeapons)
							{
								ImGui::Combo("Weapons Type", &Settings.Visuals.WeaponsType, "None\0Textured\0Flat\0Wireframe\0Bubble\0Color\0Flat Wireframed\0Textured Wireframed\0No Hands\0");
								ImGui::ColorEdit3("Weapon Visual Visual", Settings.Visuals.WeaponColorVisible, ImGuiColorEditFlags_NoSliders);
							}
							ImGui::EndChild();
						}
						ImGui::SameLine();
						ImGui::BeginChild("cVisual Viewmodel", ImVec2(ImGui::GetWindowWidth() / 2 - 12, 340), true);
						{
							ImGui::Text("Viewmodel");
							ImGui::SliderFloat("Viewmodel Fov", &Settings.Visuals.ViewModelFov, -180.f, 180.f);
							ImGui::SliderFloat("Render Fov", &Settings.Visuals.RenderFov, 0.f, 180.f);
							ImGui::Checkbox("No Visual Recoil", &Settings.Visuals.NoVisRecoil);
							if (Settings.Visuals.NoVisRecoil)
								ImGui::SliderFloat("AimPunch Factor", &Settings.Visuals.AimPunchFactor, 0.f, 100.f);
							ImGui::Checkbox("Custom Players (Check before ingame)", &Settings.Visuals.CustomPlayers);
							ImGui::Checkbox("Custom Gloves (Check before ingame)", &Settings.Visuals.CustomGloves);
							ImGui::SliderFloat("Thirdperson", &Settings.Visuals.ThirdPerson, 0.f, 1000.f);
							ImGui::Checkbox("Spread Crosshair", &Settings.Visuals.SpreadCrosshair);
							ImGui::SameLine();
							ImGui::ColorEdit3("Spread Crosshair Color", Settings.Visuals.SpreadCrosshairColor, ImGuiColorEditFlags_NoSliders);
							ImGui::Checkbox("Enemy Only", &Settings.Visuals.EnemyOnly);
							ImGui::Checkbox("Draw Rank", &Settings.Visuals.DrawRank);
							ImGui::Checkbox("Draw Wins", &Settings.Visuals.DrawWins);
							ImGui::Checkbox("Draw Defuse Kit", &Settings.Visuals.ShowDefuseKit);
							ImGui::Checkbox("Draw Ping", &Settings.Visuals.DrawPing);
							ImGui::Checkbox("Draw Has Bomb", &Settings.Visuals.ShowBomb);
							ImGui::Checkbox("Draw Is Defusing", &Settings.Visuals.ShowDefusing);
							ImGui::Checkbox("Draw Money", &Settings.Visuals.DrawMoney);

							ImGui::Checkbox("Draw Bomb Info", &Settings.Visuals.ShowBombDetonation);
							ImGui::Checkbox("Draw Weapons", &Settings.Visuals.ShowWeapons);
							ImGui::Checkbox("Draw Grenades", &Settings.Visuals.ShowGrenades);
							ImGui::Checkbox("Draw Defuse Kits", &Settings.Visuals.ShowKits);
							ImGui::Checkbox("Draw Chickens", &Settings.Visuals.ShowChickens);
							ImGui::Checkbox("Draw Fish", &Settings.Visuals.ShowFish);

							ImGui::Checkbox("Draw Aimspot", &Settings.Aimbot.DrawAimspot);

							ImGui::Checkbox("NoScope", &Settings.Visuals.NoScope);
							ImGui::Checkbox("Disable Post Processing", &Settings.Visuals.DisablePP);
							ImGui::Checkbox("DLights", &Settings.Visuals.Dlight);
							ImGui::SliderFloat("DLight Radius", &Settings.Visuals.Radius, 0.f, 100.f);
							ImGui::ColorEdit3("DLight Color Terrorist", Settings.Visuals.DLightColorTerrorist, ImGuiColorEditFlags_NoSliders);
							ImGui::ColorEdit3("DLight Color Counter-Terrorist", Settings.Visuals.DLightColorCounterTerrorist, ImGuiColorEditFlags_NoSliders);

							ImGui::Combo("Crosshair", &Settings.Visuals.RecoilCrosshairType, "None\0Circle\0Cross\0Square\0Plus\0Chevron\0Nazi\0");
							ImGui::Checkbox("Follow Recoil", &Settings.Visuals.FollowRecoil);

							ImGui::Checkbox("Sonar ESP", &Settings.Visuals.Sonar);

							ImGui::Checkbox("Snap Lines", &Settings.Visuals.Snaplines);
							ImGui::Checkbox("Barrel ESP", &Settings.Visuals.Barrel);
							ImGui::Checkbox("Location", &Settings.Visuals.Location);
							ImGui::Combo("Callout Spam", &Settings.Visuals.Spam, "None\0Team\0All Chat\0");
							ImGui::SliderInt("Callout Delay", &Settings.Visuals.Delay, 100, 2000);
							ImGui::Checkbox("Distance", &Settings.Visuals.Distance);

							ImGui::Checkbox("Draw Helmet", &Settings.Visuals.DrawHelmet);
							ImGui::Checkbox("Draw Health", &Settings.Visuals.DrawHealth);
							ImGui::Checkbox("Draw Armor", &Settings.Visuals.DrawArmor);
							ImGui::EndChild();
						}
					}
					if (MiscTab)
					{
						ImGui::Text("Misc"); ImGui::BeginChild("cMisc Main", ImVec2(ImGui::GetWindowWidth() / 3 - 12, 340), true);
						{
							ImGui::Checkbox("Bunnyhop", &Settings.Misc.Bunnyhop);
							ImGui::Combo("Autostrafe Mode", &Settings.Misc.AutoStrafe, "None\0Normal\0Silent\0Rage\0");
							ImGui::Checkbox("NameSpammer", &Settings.Misc.NameSpam);
							ImGui::Checkbox("NameStealer", &Settings.Misc.NameStealer);
							ImGui::Checkbox("No flash", &Settings.Misc.NoFlash);
							ImGui::Checkbox("No smoke", &Settings.Misc.NoSmoke);
							ImGui::Checkbox("Auto Pistol", &Settings.Misc.AutoPistol);
							ImGui::Checkbox("Custom Clan Tag", &Settings.Misc.CustomClanTag);
							ImGui::Checkbox("Animate Clan Tag", &Settings.Misc.AnimateClanTag);
							static char tempTag[15] = "";
							ImGui::InputText("ClanTag ##autism", tempTag, 15);
							if (ImGui::Button("Apply Tag"))
								Settings.Misc.ClanTag = tempTag;
							ImGui::EndChild();
						}
						ImGui::SameLine();
						ImGui::BeginChild("cMisc Alt", ImVec2(ImGui::GetWindowWidth() / 3 - 12, 340), true);
						{
							ImGui::Checkbox("FakeLag", &Settings.Misc.FakeLag);
							if (Settings.Misc.FakeLag)
								ImGui::SliderInt("FakeLag Amount", &Settings.Misc.FakeLagAmount, 0, 15);
							ImGui::Checkbox("FakeLag On Key", &Settings.Misc.FakeLagOnKey);
							ImGui::Combo("FakeLag Key", &Settings.Misc.FakeLagKey, Keys, IM_ARRAYSIZE(Keys));
							ImGui::Checkbox("Spectators List", &Settings.Misc.ShowSpectators);
							ImGui::Checkbox("Spectators Show Team Only", &Settings.Misc.ShowSpecTeamOnly);
							ImGui::Checkbox("Radar", &Settings.Misc.Radar);
							ImGui::Checkbox("Reveal Ranks", &Settings.Misc.RevealRank);
							ImGui::Checkbox("Auto Accept", &Settings.Misc.AutoAccept);
							ImGui::Checkbox("Water Mark", &Settings.Misc.WaterMark);
							if (ImGui::Button("Change ##Skybox"))
							{
								static auto LoadNamedSky = reinterpret_cast<void(__fastcall*)(const char*)>(FindPattern->IDA("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
								LoadNamedSky(Skyboxes[Settings.Misc.Skybox]);
							}
							ImGui::SameLine();
							ImGui::Combo("Custom Skybox", &Settings.Misc.Skybox, Skyboxes, IM_ARRAYSIZE(Skyboxes));
							ImGui::Checkbox("NoSky", &Settings.Misc.NoSky);
							ImGui::Checkbox("Change Wall Color", &Settings.Misc.ChangeWallColor);
							ImGui::ColorEdit3("Wall Color", Settings.Misc.WallColor, ImGuiColorEditFlags_NoSliders);
							ImGui::Checkbox("Rainbow Walls", &Settings.Misc.RainbowWalls);
							ImGui::SliderFloat("Asus", &Settings.Misc.WallAlpha, 0.f, 1.f);
							ImGui::Checkbox("Mein Kampf Mode", &Settings.Misc.MeinKampfMode);
							if (ImGui::Button("No Name"))
								g_pMisc->ChangeName("\n");
							ImGui::EndChild();
						}
						ImGui::SameLine();
						ImGui::BeginChild("cMisc Config", ImVec2(ImGui::GetWindowWidth() / 3 - 12, 340), true);
						{
							std::vector<std::string> FilesInDir = g_Config.GetAllConfigs("C:\\GooseHook");
							for (int i = 0; i < FilesInDir.size(); i++)
								ImGui::Text(FilesInDir[i].c_str());

							ImGui::InputText("Config Name", Settings.Misc.ConfigBuffer, MAX_PATH);
							if (ImGui::Button("Save Config"))
								g_Config.CreateConfig("C:\\GooseHook", Settings.Misc.ConfigBuffer);
							ImGui::SameLine();
							if (ImGui::Button("Load Config"))
								g_Config.LoadConfig(Settings.Misc.ConfigBuffer);
							ImGui::SameLine();
							if (ImGui::Button("Delete Config"))
								g_Config.DeleteConfig(Settings.Misc.ConfigBuffer);
							//ImGui::Checkbox("Skinchanger Enabled", &Settings.SkinChanger.Enabled);
							ImGui::EndChild();
						}
						
					}
				}
				ImGui::End();

				//if (Settings.SkinChanger.Enabled)
				//{
					//ImGui::Begin("Skin Changer", &Settings.Menu.MenuOpened, ImVec2(620, 320), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
					//{					
					//	ImGui::Checkbox("Enabled", &Settings.SkinChanger.Enabled);
					//	ImGui::Combo("Weapon", &Settings.SkinChanger.SelectedWeapon, [](void* data, int idx, const char** out_text)
					//	{
					//		*out_text = k_WeaponNames[idx].szName;
					//		return true;
					//	}, nullptr, k_WeaponNames.size(), 5);

					//	Skin_t *skinerino = &SkinMap[k_WeaponNames[Settings.SkinChanger.SelectedWeapon].iDefinitionIndex];
					//	ImGui::Text("id: %i", k_WeaponNames[Settings.SkinChanger.SelectedWeapon].iDefinitionIndex);

					//	ImGui::Combo("Paint Kit", &skinerino->PaintKit, [](void* data, int idx, const char** out_text)
					//	{
					//		*out_text = k_Skins[idx].strName.c_str();
					//		return true;
					//	}, nullptr, k_Skins.size(), 10);
					//	ImGui::InputInt("Seed", &skinerino->Seed);
					//	ImGui::InputInt("StatTrak", &skinerino->Stattrak);
					//	ImGui::SliderFloat("Wear", &skinerino->Wear, 0.0000001f, 1.f);
					//}
					//ImGui::End();
				//}
			}
			ImGui::Render();
			pDevice->SetRenderState(D3DRS_SRCBLEND, oSRCBLEND);
			pDevice->SetRenderState(D3DRS_DESTBLEND, oDESTBLEND);
		}

	ImGui_ImplDX9_InvalidateDeviceObjects(); //Invalidate GUI resources
	ImGui_ImplDX9_CreateDeviceObjects(); //Recreate GUI resources
	return Present_Original(pDevice, pSourceRec, pDestRect, hWnd, pDirtyRegion);
}

HRESULT __stdcall Hooks::Reset_Hooked(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!!Initialized)
		return ResetScene_Original(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects(); //Invalidate GUI resources

	auto hr = ResetScene_Original(pDevice, pPresParam);

	ImGui_ImplDX9_CreateDeviceObjects(); //Recreate GUI resources
	return hr;
}	

void GUI_Init(IDirect3DDevice9* pDevice)
{
	ImGui_ImplDX9_Init(Window, pDevice);
	auto& style = ImGui::GetStyle();

	int width2, height2, bytes_per_pixel;
	unsigned char* pixels;
	//ImGuiStyle * style = &ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	/*comicsans = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 12);
	comicsansbig = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 24);*/
	Tahoma = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 13);
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width2, &height2, &bytes_per_pixel);
	//tahoma_compressed_data_base85
	g_FontTexture = NULL;

	pDevice->CreateTexture(width2, height2, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_FontTexture, NULL);
	D3DLOCKED_RECT tex_locked_rect;
	g_FontTexture->LockRect(0, &tex_locked_rect, NULL, 0);

	for (int y = 0; y < height2; y++)
		memcpy((unsigned char *)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width2 * bytes_per_pixel) * y, (width2 * bytes_per_pixel));
	g_FontTexture->UnlockRect(0);

	// Store our identifier
	io.Fonts->TexID = (void *)g_FontTexture;
	style.WindowMinSize = ImVec2(115, 20);
	style.FramePadding = ImVec2(4, 0);
	style.ItemSpacing = ImVec2(5, 2);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.Alpha = 1.f;
	style.WindowRounding = 3.0f;
	style.WindowPadding = ImVec2(9, 8);
	style.FrameRounding = 2.f;
	style.IndentSpacing = 5.f;
	style.ItemInnerSpacing = ImVec2(2, 4);
	style.WindowTitleAlign = ImGuiAlign_Center;
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;
	style.ScrollbarSize = 5.0f;
	style.ScrollbarRounding = 1.f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.20f, 0.20f, 0.2f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.0f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.0f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.08f, 0.69f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.76f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.f, 0.77f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.55f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.71f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.0f, 0.0f, 0.2f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.f, 0.f, 0.f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.05f, 0.64f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.04f, 0.70f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.02f, 0.57f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);

	Initialized = true;
}
