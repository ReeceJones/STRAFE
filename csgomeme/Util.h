#pragma once
#include "Vector.h"
#include <Windows.h>
#include <stdio.h>
#include <string>

typedef void(__thiscall* InitKeyValuesFn)(void* thisptr, const char* name);
extern InitKeyValuesFn InitKeyValuesEx;
typedef void(__thiscall* LoadFromBufferFn)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
extern LoadFromBufferFn LoadFromBufferEx;

class KeyValues;
class IMaterial;
class cUtil
{
public:
	bool CreateConsoleWindow(char*);
	inline void**& GetVTable(void* inst, size_t offset = 0)
	{
		return *reinterpret_cast<void***>(reinterpret_cast<size_t>(inst) + offset);
	}
	inline const void** GetVTable(const void* inst, size_t offset = 0)
	{
		return *reinterpret_cast<const void***>(reinterpret_cast<size_t>(inst) + offset);
	}
	template< typename Fn >
	inline Fn CallVFunc(const void* inst, size_t index, size_t offset = 0)
	{
		return reinterpret_cast<Fn>(GetVTable(inst, offset)[index]);
	}
	typedef void* (*CreateInterface_t)(const char*, int*);
	inline void* GetInterface(const char* chHandle, const char* chInterfaceName)
	{
		volatile auto handlegotten = (GetModuleHandleA(chHandle) != nullptr);
		while (!GetModuleHandleA(chHandle)) Sleep(100);
		void* fnFinal = nullptr;
		auto PossibleInterfaceName = new char[strlen(chInterfaceName) + 4];
		auto TestInterface = reinterpret_cast<CreateInterface_t>(GetProcAddress(GetModuleHandleA(chHandle), "CreateInterface"));
		for (auto i = 100; i > 0; i--)
		{

			sprintf(PossibleInterfaceName, "%s%03i", chInterfaceName, i);
			fnFinal = static_cast<void*>(TestInterface(PossibleInterfaceName, nullptr));

			if (fnFinal != nullptr)
				break;

		}
		delete PossibleInterfaceName;
		return fnFinal;
	}
	bool LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
	void InitKeyValues(KeyValues* pKeyValues, const char* name);
	void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = nullptr, void* pfnEvaluateSymbolProc = nullptr);
};
extern cUtil *g_pUtil;