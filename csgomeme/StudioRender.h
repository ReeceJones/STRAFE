#pragma once
#include "Convar.h"
#include "Math.h"
#include "Structs.h"
#include "Enums.h"
#include "Vector.h"
#include "Trace.h"
#include "MDLCache.h"

#define ADDDECAL_TO_ALL_LODS -1

class CUtlBuffer;
class ITexture;
class IMaterialSystem;
class IMaterial;
struct StudioRenderConfig_t;
struct studiohdr_t;
struct studiohwdata_t;
struct LightDesc_t;
struct DrawModelResults_t;
struct DrawModelInfo_t;
struct StudioDecalHandle_t;
struct FlashlightState_t;
struct GetTriangles_Output_t;
struct model_array_instance_t;

class IStudioRender : public IAppSystem
{
public:
	virtual void BeginFrame(void) = 0;
	virtual void EndFrame(void) = 0;
	virtual void Mat_Stub(IMaterialSystem *pMatSys) = 0;
	virtual void UpdateConfig(const StudioRenderConfig_t& config) = 0;
	virtual void GetCurrentConfig(StudioRenderConfig_t& config) = 0;
	virtual bool LoadModel(studiohdr_t *pStudioHdr, void *pVtxData, studiohwdata_t	*pHardwareData) = 0;
	virtual void UnloadModel(studiohwdata_t *pHardwareData) = 0;
	virtual void RefreshStudioHdr(studiohdr_t* pStudioHdr, studiohwdata_t* pHardwareData) = 0;
	virtual void SetEyeViewTarget(const studiohdr_t *pStudioHdr, int nBodyIndex, const Vector& worldPosition) = 0;
	virtual int GetNumAmbientLightSamples() = 0;
	virtual const Vector *GetAmbientLightDirections() = 0;
	virtual void SetAmbientLightColors(const Vector *pAmbientOnlyColors) = 0;
	virtual void SetAmbientLightColors2(const Vector *pAmbientOnlyColors) = 0;
	virtual void SetLocalLights(int numLights, const LightDesc_t *pLights) = 0;
	virtual void SetViewState(const Vector& viewOrigin, const Vector& viewRight,
		const Vector& viewUp, const Vector& viewPlaneNormal) = 0;
	virtual void LockFlexWeights(int nWeightCount, float **ppFlexWeights, float **ppFlexDelayedWeights = NULL) = 0;
	virtual void UnlockFlexWeights() = 0;
	virtual matrix3x4_t* LockBoneMatrices(int nBoneCount) = 0;
	virtual void UnlockBoneMatrices() = 0;
	virtual int GetNumLODs(const studiohwdata_t &hardwareData) const = 0;
	virtual float GetLODSwitchValue(const studiohwdata_t &hardwareData, int lod) const = 0;
	virtual void SetLODSwitchValue(studiohwdata_t &hardwareData, int lod, float switchValue) = 0;
	virtual void SetColorModulation(float const* pColor) = 0;
	virtual void SetAlphaModulation(float flAlpha) = 0;

	// Draws the model
	virtual void DrawModel(DrawModelResults_t *pResults, const DrawModelInfo_t& info,
		matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;

	// Methods related to static prop rendering
	virtual void DrawModelStaticProp(const DrawModelInfo_t& drawInfo, const matrix3x4_t &modelToWorld, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void DrawStaticPropDecals(const DrawModelInfo_t &drawInfo, const matrix3x4_t &modelToWorld) = 0;
	virtual void DrawStaticPropShadows(const DrawModelInfo_t &drawInfo, const matrix3x4_t &modelToWorld, int flags) = 0;

	// Causes a material to be used instead of the materials the model was compiled with
	virtual void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;

	// Create, destroy list of decals for a particular model
	virtual StudioDecalHandle_t CreateDecalList(studiohwdata_t *pHardwareData) = 0;
	virtual void DestroyDecalList(StudioDecalHandle_t handle) = 0;

	// Add decals to a decal list by doing a planar projection along the ray
	// The BoneToWorld matrices must be set before this is called
	virtual void AddDecal(StudioDecalHandle_t handle, studiohdr_t *pStudioHdr, matrix3x4_t *pBoneToWorld,
		const Ray_t & ray, const Vector& decalUp, IMaterial* pDecalMaterial, float radius, int body, bool noPokethru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;

	// Compute the lighting at a point and normal
	virtual void ComputeLighting(const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting) = 0;

	// Compute the lighting at a point, constant directional component is passed
	// as flDirectionalAmount
	virtual void ComputeLightingConstDirectional(const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting, float flDirectionalAmount) = 0;

	// Shadow state (affects the models as they are rendered)
	virtual void AddShadow(IMaterial* pMaterial, void* pProxyData, FlashlightState_t *m_pFlashlightState = NULL, VMatrix *pWorldToTexture = NULL, ITexture *pFlashlightDepthTexture = NULL) = 0;
	virtual void ClearAllShadows() = 0;

	// Gets the model LOD; pass in the screen size in pixels of a sphere 
	// of radius 1 that has the same origin as the model to get the LOD out...
	virtual int ComputeModelLod(studiohwdata_t* pHardwareData, float unitSphereSize, float *pMetric = NULL) = 0;

	// Return a number that is usable for budgets, etc.
	// Things that we care about:
	// 1) effective triangle count (factors in batch sizes, state changes, etc)
	// 2) texture memory usage
	// Get Triangles returns the LOD used
	virtual void GetPerfStats(DrawModelResults_t *pResults, const DrawModelInfo_t &info, CUtlBuffer *pSpewBuf = NULL) const = 0;
	virtual void GetTriangles(const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, GetTriangles_Output_t &out) = 0;

	// Returns materials used by a particular model
	virtual int GetMaterialList(studiohdr_t *pStudioHdr, int count, IMaterial** ppMaterials) = 0;
	virtual int GetMaterialListFromBodyAndSkin(MDLHandle_t studio, int nSkin, int nBody, int nCountOutputMaterials, IMaterial** ppOutputMaterials) = 0;
	// draw an array of models with the same state
	virtual void DrawModelArray(const DrawModelInfo_t &drawInfo, int arrayCount, model_array_instance_t *pInstanceData, int instanceStride, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
};