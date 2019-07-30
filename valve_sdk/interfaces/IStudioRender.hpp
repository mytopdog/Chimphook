
#ifndef ISTUDIORENDER_H
#define ISTUDIORENDER_H
#ifdef _WIN32
#pragma once
#endif

#include "../math/Vector4D.hpp"
#include "../misc/UtlVector.hpp"
#include "../misc/UtlBuffer.hpp"
#include "IMaterialSystem.hpp"
#include "IAppSystem.hpp"


//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct studiohdr_t;
struct studiomeshdata_t;
class Vector;
struct LightDesc_t;
class IMaterial;
struct studiohwdata_t;
struct Ray_t;
class Vector4D;
class IMaterialSystem;
struct matrix3x4_t;
class IMesh;
struct vertexFileHeader_t;
struct FlashlightState_t;
class VMatrix;
class StudioRenderConfig_t;

enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_NO_SKIN = 0x400,

	STUDIORENDER_SKIP_DECALS = 0x800,
};

abstract_class IStudioDataCache : public IAppSystem
{
public:
	virtual bool VerifyHeaders(studiohdr_t * pStudioHdr) = 0;
	virtual vertexFileHeader_t* CacheVertexData(studiohdr_t* pStudioHdr) = 0;
};

enum
{
	ADDDECAL_TO_ALL_LODS = -1
};

struct GetTriangles_Output_t
{
};

enum
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};


//-----------------------------------------------------------------------------
// Studio render interface
//-----------------------------------------------------------------------------
abstract_class IStudioRender : public IAppSystem
{
public:
	// FIXME: For backward compatibility
	virtual bool Init(CreateInterfaceFn materialSystemFactory, CreateInterfaceFn materialSystemHWConfigFactory,
		CreateInterfaceFn convarFactory, CreateInterfaceFn studioDataCacheFactory) = 0;

	virtual void BeginFrame(void) = 0;
	virtual void EndFrame(void) = 0;

	// Updates the rendering configuration 
	virtual void UpdateConfig(const StudioRenderConfig_t& config) = 0;
	virtual int GetRootLOD_Obsolete() = 0;

	virtual bool LoadModel(studiohdr_t* pStudioHdr, void* pVtxData, studiohwdata_t* pHardwareData) = 0;

	// since studiomeshes are allocated inside of the lib, they need to be freed there as well.
	virtual void UnloadModel(studiohwdata_t* pHardwareData) = 0;

	// This is needed to do eyeglint and calculate the correct texcoords for the eyes.
	virtual void SetEyeViewTarget(const Vector& worldPosition) = 0;

	virtual int GetNumAmbientLightSamples() = 0;

	virtual const Vector* GetAmbientLightDirections() = 0;

	// assumes that the arraysize is the same as returned from GetNumAmbientLightSamples
	virtual void SetAmbientLightColors(const Vector* pAmbientOnlyColors) = 0;

	virtual void SetLocalLights(int numLights, const LightDesc_t* pLights) = 0;

	virtual void SetViewState(
		const Vector& viewOrigin,
		const Vector& viewRight,
		const Vector& viewUp,
		const Vector& viewPlaneNormal) = 0;

	virtual void SetFlexWeights(int numWeights, const float* pWeights) = 0;
	virtual void SetFlexWeights(int numWeights, const float* pWeights, const float* pDelayedWeights) = 0;

	// fixme: these interfaces sucks. . use 'em to get this stuff working with the client dll
	// and then interate
	virtual matrix3x4_t* GetPoseToWorld(int i) = 0; // this will be hidden enntually (computed internally)
	virtual matrix3x4_t* GetBoneToWorld(int i) = 0;

	// NOTE: this array must have space for MAXSTUDIOBONES.
	virtual matrix3x4_t* GetBoneToWorldArray() = 0;

	// LOD stuff
	virtual int GetNumLODs(const studiohwdata_t& hardwareData) const = 0;
	virtual float GetLODSwitchValue(const studiohwdata_t& hardwareData, int lod) const = 0;
	virtual void SetLODSwitchValue(studiohwdata_t& hardwareData, int lod, float switchValue) = 0;

	// Sets the color modulation
	virtual void SetColorModulation(float const* pColor) = 0;
	virtual void SetAlphaModulation(float alpha) = 0;

	// returns the number of triangles rendered.
	virtual int DrawModel(DrawModelInfo_t& info, const Vector& modelOrigin,
		int* pLodUsed, float* pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;

	// Causes a material to be used instead of the materials the model was compiled with
	virtual void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;

	// Create, destroy list of decals for a particular model
	virtual StudioDecalHandle_t CreateDecalList(studiohwdata_t* pHardwareData) = 0;
	virtual void DestroyDecalList(StudioDecalHandle_t handle) = 0;

	// Add decals to a decal list by doing a planar projection along the ray
	// The BoneToWorld matrices must be set before this is called
	virtual void AddDecal(StudioDecalHandle_t handle, studiohdr_t* pStudioHdr, const Ray_t& ray,
		const Vector& decalUp, IMaterial* pDecalMaterial, float radius, int body, bool noPokethru = false, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;

	// Remove all the decals on a model
	virtual void RemoveAllDecals(StudioDecalHandle_t handle, studiohdr_t* pStudioHdr) = 0;

	// Compute the lighting at a point and normal
	virtual void ComputeLighting(const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting) = 0;

	// Refresh the studiohdr since it was lost...
	virtual void RefreshStudioHdr(studiohdr_t* pStudioHdr, studiohwdata_t* pHardwareData) = 0;

	// Used for the mat_stub console command.
	virtual void Mat_Stub(IMaterialSystem* pMatSys) = 0;

	// Shadow state (affects the models as they are rendered)
	virtual void AddShadow(IMaterial* pMaterial, void* pProxyData, FlashlightState_t* m_pFlashlightState = NULL, VMatrix* pWorldToTexture = NULL) = 0;
	virtual void ClearAllShadows() = 0;

	// Gets the model LOD; pass in the screen size in pixels of a sphere 
	// of radius 1 that has the same origin as the model to get the LOD out...
	virtual int ComputeModelLod(studiohwdata_t* pHardwareData, float unitSphereSize, float* pMetric = NULL) = 0;

	// Return a number that is usable for budgets, etc.
	// Things that we care about:
	// 1) effective triangle count (factors in batch sizes, state changes, etc)
	// 2) texture memory usage
	virtual void GetPerfStats(DrawModelInfo_t& info, CUtlBuffer* pSpewBuf = NULL) const = 0;
#ifndef _XBOX
	virtual void GetTriangles(DrawModelInfo_t& info, GetTriangles_Output_t& out) = 0;
#endif

	// Compute the lighting at a point, constant directional component is passed
	// as flDirectionalAmount
	virtual void ComputeLightingConstDirectional(const Vector* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vector& pt, const Vector& normal, Vector& lighting, float flDirectionalAmount) = 0;

	virtual int GetMaterialList(studiohdr_t* pStudioHdr, int count, IMaterial** ppMaterials) = 0;

	// returns the number of triangles rendered.
	virtual int DrawModelStaticProp(DrawModelInfo_t& info, const Vector& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;

	virtual void AddShadowEx(IMaterial* pMaterial, void* pProxyData, FlashlightState_t* m_pFlashlightState = NULL, VMatrix* pWorldToTexture = NULL, ITexture* pFlashlightDepthTexture = NULL) = 0;

	// Gets the current config (fills in the structure)
	virtual void GetCurrentConfig(StudioRenderConfig_t& config) = 0;
};

extern IStudioRender* g_pStudioRender;

#endif 
