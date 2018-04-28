#pragma once
#include "Drawing.h"
#include "Util.h"



class ConVar
{
public:
	void SetValue(const char *value)
	{
		typedef void(__thiscall* OriginalFn)(void*, const char*);
		return  g_pUtil->CallVFunc<OriginalFn>(this, 14)(this, value);
	}
	void SetValue(float value)
	{
		typedef void(__thiscall* OriginalFn)(void*, float);
		return g_pUtil->CallVFunc<OriginalFn>(this, 15)(this, value);
	}
	void SetValue(int value)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 16)(this, value);
	}
	void SetValue(Color value)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return g_pUtil->CallVFunc<OriginalFn>(this, 17)(this, value);
	}
	void InternalSetString(const char* str);
	char* GetName()
	{
		typedef char*(__thiscall* OriginalFn)(void*);
		return g_pUtil->CallVFunc<OriginalFn>(this, 5)(this);
	}
	char* GetDefault()
	{
		return pszDefaultValue;
	}
	char pad_0x0000[0x4];
	ConVar* pNext;
	__int32 bRegistered;
	char* pszName;
	char* pszHelpString;
	__int32 nFlags;
	char pad_0x0018[0x4];
	ConVar* pParent;
	char* pszDefaultValue;
	char* strString;
	__int32 StringLength;
	float fValue;
	__int32 nValue;
	__int32 bHasMin;
	float fMinVal;
	__int32 bHasMax;
	float fMaxVal;
	void* fnChangeCallback;
};
class IAppSystem
{
public:
	virtual ~IAppSystem()
	{
	}

	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

class ICVar : public IAppSystem
{
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			func13() = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	virtual void			func15() = 0;
	virtual void			func16() = 0;
	virtual void			func17() = 0;
	virtual void			func18() = 0;
	virtual void			func19() = 0;
	virtual void			func20() = 0;
};