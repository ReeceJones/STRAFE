#pragma once
#include "IMaterial.h"
#include "Util.h"

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial *mat)
	{
		typedef void(__thiscall *OrigFn)(void *, IMaterial *, int, int);
		g_pUtil->CallVFunc<OrigFn>(this, 1)(this, mat, 0, 0);
	}

	void DrawModelExecute(void* ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		typedef void(__thiscall* OriginalFn)(void*, void* ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
		return g_pUtil->CallVFunc<OriginalFn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
	}
};

extern IMaterial* Visible_Textured;
extern IMaterial* Visible_Flat;
extern IMaterial* Visible_WireFramed;
extern IMaterial* Visible_Bubble;

extern IMaterial* IgnoreZ_Textured;
extern IMaterial* IgnoreZ_Flat;
extern IMaterial* IgnoreZ_WireFramed;
extern IMaterial* IgnoreZ_Bubble;

extern IMaterial* Meme;