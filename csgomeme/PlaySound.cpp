#include "Hooks.h"
#include "Settings.h"
#include "FindPattern.h"
#include "Interfaces.h"

PlaySoundFn PlaySound_Original;

void __stdcall Hooks::PlaySound_Hooked(const char* szFileName)
{
	static auto IsReady = reinterpret_cast<void(__cdecl*)()>(FindPattern->IDA("client.dll", "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B BE"));

	PlaySound_Original(Interfaces::Surface, szFileName);

	if (Settings.Misc.AutoAccept && !Interfaces::Engine->IsInGame())
	{
		if (!strcmp(szFileName, "weapons/hegrenade/beep.wav"))
		{
			IsReady();
			FLASHWINFO fi;
			fi.cbSize = sizeof(FLASHWINFO);
			fi.hwnd = Window;
			fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			fi.uCount = 0;
			fi.dwTimeout = 0;
			FlashWindowEx(&fi);
		}
	}
}
