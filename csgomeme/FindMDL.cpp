#include "Hooks.h"
#include "Interfaces.h"
#include "Definitions.h"
#include "Settings.h"
#include "Features.h"

FindMDLFn FindMDL_Original;
MDLHandle_t __fastcall Hooks::FindMDL_Hooked(void* ecx, void* edx, const char* pMDLRelativePath)
{
	//if (strstr(pMDLRelativePath, "v_knife_default_ct.mdl")
	//	|| strstr(pMDLRelativePath, "v_knife_default_t.mdl"))
	//	return FindMDL_Original(ecx, "models/weapons/v_knife_butterfly.mdl");

	if (strstr(pMDLRelativePath, "models/player")
		&& !strstr(pMDLRelativePath, "w_") 
		&& Settings.Visuals.CustomPlayers)
		return FindMDL_Original(ecx, "models/player/custom_player/kuristaja/ak/batman/batmanv2.mdl");

	if (strstr(pMDLRelativePath, "arms")
		&& Settings.Visuals.CustomGloves)
		return FindMDL_Original(ecx, "models/player/custom_player/kuristaja/ak/batman/batman_arms.mdl");
	
	return FindMDL_Original(ecx, pMDLRelativePath);
}