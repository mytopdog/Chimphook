#pragma once
#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

#include "valve_sdk/sdk.hpp"
#include "valve_sdk/misc/clientleafsystem.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>
#include <cstdint>

namespace CreateMove
{
	extern bool sendPacket;
	extern QAngle lastTickViewAngles;
	extern int lastbuttons;
	extern int tick;

	extern QAngle FakeAngles;
	extern QAngle RealAngles;
	extern float CurrentLBY;
	extern float GoalBreakDelta;
}

namespace index
{
	constexpr int EmitSound1 = 5;
	constexpr int EndScene = 42;
	constexpr int SceneEnd = 9;
	constexpr int DrawSetColor = 15;
	constexpr int Reset = 16;
	constexpr int PaintTraverse = 41;
	constexpr int CreateMove = 22;
	constexpr int RenderView = 6;
	constexpr int WriteUsercmdDeltaToBuffer = 23;
	constexpr int PlaySound = 82;
	constexpr int DispatchUserMessage = 38;
	constexpr int FrameStageNotify = 37;
	constexpr int DrawModelExecute = 21;
	constexpr int RenderSmokeOverlay = 41;
	constexpr int DoPostScreenSpaceEffects = 44;
	constexpr int FindMaterial = 84;
	constexpr int SvCheatsGetBool = 13;
	constexpr int OverrideView = 18;
	constexpr int LockCursor = 67;
	constexpr int ListLeavesInBox = 6;
	constexpr int ClientCmd = 7;
	constexpr int Present = 17;
}

static int tickBaseShift = 0;
static bool inSendMove = false, firstSendMovePack = false;

static ITexture* mirrorTexture;
static IMaterial* mirrorMaterial;

namespace Hooks
{
	void Initialize();
	void Shutdown();

	inline RecvVarProxyFn o_nSequence;

	inline vfunc_hook hlclient_hook;
	inline vfunc_hook direct3d_hook;
	inline vfunc_hook vguipanel_hook;
	inline vfunc_hook vguisurf_hook;
	inline vfunc_hook mdlrender_hook;
	inline vfunc_hook viewrender_hook;
	inline vfunc_hook renderview_hook;
	inline vfunc_hook materialsystem_hook;
	inline vfunc_hook sound_hook;
	inline vfunc_hook clientmode_hook;
	inline vfunc_hook sv_cheats;
	inline vfunc_hook bsp_query_hook;
	inline vfunc_hook engineclient_hook;

	typedef bool(__thiscall* tDispatchUserMessage)(void*, int type, unsigned int a3, unsigned int length, const void* msg_data);

	long __stdcall hkEndScene(IDirect3DDevice9* device);
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	long __stdcall hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __fastcall hkRenderView(IViewRender* this0, void* EDX, CViewSetup& view, CViewSetup& hudViewSetup, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw);
	void __fastcall hkSceneEnd(void* thisptr, void* edx);
	void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active);
	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __fastcall hkEmitSound1(void* _this, int, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	void __fastcall hkDrawModelExecute(void* _this, int, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void __fastcall hkFrameStageNotify(void* _this, int, ClientFrameStage_t stage);
	void __fastcall hkOverrideView(void* _this, int, CViewSetup* vsView);
	void __fastcall hkRenderSmokeOverlay(uintptr_t ecx, uintptr_t edx, bool a1);
	void __fastcall hkLockCursor(void* _this);
	int  __fastcall hkDoPostScreenEffects(void* _this, int, int a1);
	bool __fastcall hkSvCheatsGetBool(void* pConVar, void* edx);
	bool __fastcall hkDispatchUserMessage(void* ecx, void* edx, int type, unsigned int a3, unsigned int length, const void* msg_data);
	IMaterial* __fastcall hkFindMaterial(void* pThis, void* edx, const char* pMaterialName, const char* pTextureGroup, bool complain, const char* pComplainPrefix);
	int __fastcall hkListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);
	void __fastcall hkClientCmd(void* ecx, void* edx, const char* cmd);
	void __fastcall hkDrawSetColor(ISurface* thisptr, void* edx, int r, int g, int b, int a);
}