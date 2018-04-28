#include "Interfaces.h"
#include "Features.h"
#include <algorithm>

float CAimbot::GetLagCompensation()
{
	float updaterate = Interfaces::Cvar->FindVar("cl_updaterate")->fValue;
	ConVar* minupdate = Interfaces::Cvar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = Interfaces::Cvar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->fValue;

	float ratio = Interfaces::Cvar->FindVar("cl_interp_ratio")->fValue;

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::Cvar->FindVar("cl_interp")->fValue;
	ConVar* cmin = Interfaces::Cvar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = Interfaces::Cvar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->fValue != 1) {
		if (ratio < cmin->fValue)
			ratio = cmin->fValue;
		else if (ratio > cmax->fValue)
			ratio = cmax->fValue;
	}
	return std::fmax(lerp, ratio / updaterate);
}