#include "Util.h"
#include <iostream>
#include <corecrt_wstdio.h>
#include <stdio.h>
#include "NetvarManager.h"
#include "FindPattern.h"
#include <string>
#include <sstream>

InitKeyValuesFn InitKeyValuesEx;
LoadFromBufferFn LoadFromBufferEx;

bool cUtil::CreateConsoleWindow(char* name)
{
	FILE* g_pFileBuf;
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
		if (!AllocConsole())
			return false;
	if (!SetConsoleTitleA(name))
		return false;
	errno_t Error = freopen_s(&g_pFileBuf, "CONOUT$", "w", stdout);
	if (Error != NULL)
		return false;
	return true;
}

void cUtil::InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	InitKeyValuesEx = reinterpret_cast<InitKeyValuesFn>(offsets.InitKeyValuesEx);
	InitKeyValuesEx(pKeyValues, name);
}

void cUtil::LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)
{
	LoadFromBufferEx = reinterpret_cast<LoadFromBufferFn>(offsets.LoadFromBufferEx);
	LoadFromBufferEx(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

bool cUtil::LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos)
{
	typedef bool(__cdecl* oLineGoesThroughSmoke) (Vector, Vector);
	static oLineGoesThroughSmoke LineGoesThroughSmoke = (oLineGoesThroughSmoke)(FindPattern->IDA("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));
	return LineGoesThroughSmoke(vStartPos, vEndPos);
}

cUtil *g_pUtil = new cUtil;
CNetvarManager* NetvarManager = new CNetvarManager;