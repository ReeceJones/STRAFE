#include "Interfaces.h"
#include "Hooks.h"
#include "Entity.h"
#include "FindPattern.h"
#include "Features.h"

//i guess we have this for random stuff. forget why i hooked this :/
//oh well, if we need to check something every frame we have this 
BeginFrameFn BeginFrame_Original;
void __stdcall Hooks::BeginFrame_Hooked()
{
	g_pMisc->DoMisc_BeginFrame();
}