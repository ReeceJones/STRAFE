// dllmain.cpp : Defines the entry point for the DLL application.
//#include <process.h>
//#include <iostream>
//#include "Setup.h"
//#include "Config.h"
//#include "Hooks.h"
//#pragma comment( lib, "wininet" )
//
//void StartThread(LPVOID lpArgs)
//{
//	Setup->Initialize();
//}
//
//BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartThread, NULL, NULL, NULL);
//	case DLL_THREAD_ATTACH:
//		break;
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		FreeConsole();
//		FreeLibraryAndExitThread(hModule, NULL);
//		break;
//	}
//	return TRUE;
//}