#include "IBaseClientDll.h"

ClientClass* IBaseClientDll::GetAllClasses()
{
	typedef ClientClass*(__thiscall* ClassFn)(PVOID);
	return g_pUtil->CallVFunc<ClassFn>(this, 8)(this);
} 