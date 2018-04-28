#include "Hooks.h"
#include "Interfaces.h"
#include "Definitions.h"
#include "Settings.h"
#include "Features.h"

GetViewModelFOVFn GetViewModelFOV_Original;

float __stdcall Hooks::GetViewModelFOV_Hooked()
{
	return Settings.Visuals.ViewModelFov;
}