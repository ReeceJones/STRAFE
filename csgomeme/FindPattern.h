#ifndef __MEMORY_FINDPATTERN_H__
#define __MEMORY_FINDPATTERN_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <Psapi.h>
#include <stdint.h>
#include <string>
#include "KeyValues.h"


class CFindPattern
{
public:
	inline DWORD Normal(DWORD dwBaseAddress, DWORD dwModuleSize, BYTE* pbMask, char* pszMask)
	{
		for (DWORD i = 0; i < dwModuleSize; i++)
		{
			if (this->DataCompare((BYTE*)(dwBaseAddress + i), pbMask, pszMask))
				return DWORD(dwBaseAddress + i);
		}

		return NULL;
	}
	inline uint64_t IDA(const char* szModule, const char* szSignature)
	{

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),((char)(0xe10+5042-0x2181)),((char)(0x90d+6492-0x2223))) ? ((x&(~0x20)) - ((char)(0x4e8+3447-0x121e)) + 0xa) : (INRANGE(x,((char)(0x736+2591-0x1125)),((char)(0xf40+5824-0x25c7))) ? x - ((char)(0x135d+3759-0x21dc)) : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}
	inline ULONG Alt(std::string szModuleName, PBYTE pbPattern, std::string szMask, ULONG uCodeBase, ULONG uSizeOfCode)
	{
		BOOL bPatternDidMatch = FALSE;
		HMODULE hModule = GetModuleHandleA(szModuleName.c_str());

		if (!hModule)
			return 0x0;

		PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
		PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

		if (uCodeBase == 0x0)
			uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

		if (uSizeOfCode == 0x0)
			uSizeOfCode = pOptionalHeader->SizeOfCode;

		ULONG uArraySize = szMask.length();

		if (!uCodeBase || !uSizeOfCode || !uArraySize)
			return 0x0;

		for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
		{
			for (size_t t = 0; t < uArraySize; t++)
			{
				if (*((PBYTE)i + t) == pbPattern[t] || szMask.c_str()[t] == ((char)(0x1cf + 4282 - 0x124a)))
					bPatternDidMatch = TRUE;

				else
				{
					bPatternDidMatch = FALSE;
					break;
				}
			}

			if (bPatternDidMatch)
				return i;
		}

		return 0x0;
	}
private:
	inline bool DataCompare(const BYTE* pData, const BYTE* pbMask, const char* pszMask)
	{
		for (; *pszMask; ++pszMask, ++pData, ++pbMask)
		{
			if (*pszMask == ((char)(0x20ed + 989 - 0x2452)) && *pData != *pbMask)
				return false;
		}
		return (*pszMask) == NULL;
	}
};
extern CFindPattern* FindPattern;
#endif