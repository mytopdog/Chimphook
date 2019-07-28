#pragma once

#define NOMINMAX
#include <Windows.h>

#include "misc/Enums.hpp"
#include "misc/vfunc.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "misc/Studio.hpp"

#include "interfaces/IMatchFramework.hpp"
#include "interfaces/IAppSystem.hpp"
#include "interfaces/IBaseClientDll.hpp"
#include "interfaces/IClientEntity.hpp"
#include "interfaces/IClientEntityList.hpp"
#include "interfaces/IClientMode.hpp"
#include "interfaces/IConVar.hpp"
#include "interfaces/ICvar.hpp"
#include "interfaces/IEngineTrace.hpp"
#include "interfaces/IVEngineClient.hpp"
#include "interfaces/IVDebugOverlay.hpp"
#include "interfaces/ISurface.hpp"
#include "interfaces/CInput.hpp"
#include "interfaces/IVModelInfoClient.hpp"
#include "interfaces/IVModelRender.hpp"
#include "interfaces/IRenderView.hpp"
#include "interfaces/IGameEventmanager.hpp"
#include "interfaces/IMaterialSystem.hpp"
#include "interfaces/IMoveHelper.hpp"
#include "interfaces/IMDLCache.hpp"
#include "interfaces/IPrediction.hpp"
#include "interfaces/IPanel.hpp"
#include "interfaces/IEngineSound.hpp"
#include "interfaces/IViewRender.hpp"
#include "interfaces/CClientState.hpp"
#include "interfaces/IPhysics.hpp"
#include "interfaces/IInputSystem.hpp"
#include "interfaces/IRefCounted.hpp"
#include "interfaces/CSPlayerResources.hpp"
#include "interfaces/IAlertSystem.hpp"
#include "interfaces/IViewRenderBeams.hpp"
#include "interfaces/ISteamClient.hpp"
#include "interfaces/IKeyValuesSystem.hpp"
#include "interfaces/ILocalize.hpp"

#include "misc/GameUI.hpp"
#include "misc/Convar.hpp"
#include "misc/CUserCmd.hpp"
#include "misc/glow_outline_effect.hpp"
#include "misc/datamap.hpp"

#include "netvars.hpp"

template<class T>
T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD * *>(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
	return (T*)find_hud_element(pThis, name);
}

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

static const char* CSGOMMRanks[] = {
	"-",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",
	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I", 
	"Master Guardian II", 
	"Master Guardian Eltie",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

class player_record_t
{
public:
	player_record_t()
	{
		initialized = false;
	}

	player_record_t(long long lSteamID64, char szName[128], long date, std::string sNote)
	{
		steamID64 = lSteamID64;
		initDate = date;
		strcpy(name, szName);
		note = sNote;
		initialized = true;
	}

	void Init(long long lSteamID64, char szName[128], long date, std::string sNote)
	{
		steamID64 = lSteamID64;
		initDate = date;
		strcpy(name, szName);
		note = sNote;
		initialized = true;
	}

	bool initialized;
	char name[128];
	long initDate;
	long long steamID64;
	std::string note;
};

inline ILocalize*            g_Localize;
inline ISteamClient*         g_SteamClient;
inline ISteamFriends*        g_SteamFriends;
inline ISteamUtils*          g_SteamUtils;
inline IViewRenderBeams*     g_RenderBeams;
inline IAlertSystem*         g_AlertSystem;
inline IMatchFramework*      g_MatchFramework;
inline C_CSPlayerResource**  g_PlayerResource;
inline IVEngineClient*       g_EngineClient;
inline CHudChat*             g_HUDChat;
inline IGameUI*              g_GameUI;
inline IBaseClientDLL*       g_CHLClient;
inline IClientEntityList*    g_EntityList;
inline CGlobalVarsBase*      g_GlobalVars;
inline IEngineTrace*         g_EngineTrace;
inline ICvar*                g_CVar;
inline IPanel*               g_VGuiPanel;
inline IClientMode*          g_ClientMode;
inline IVDebugOverlay*       g_DebugOverlay;
inline ISurface*             g_VGuiSurface;
inline CInput*               g_Input;
inline IVModelInfoClient*    g_MdlInfo;
inline IVModelRender*        g_MdlRender;
inline IVRenderView*         g_RenderView;
inline IMaterialSystem*      g_MatSystem;
inline IGameEventManager2*   g_GameEvents;
inline IMoveHelper*          g_MoveHelper;
inline IMDLCache*            g_MdlCache;
inline IPrediction*          g_Prediction;
inline CGameMovement*        g_GameMovement;
inline IEngineSound*         g_EngineSound;
inline CGlowObjectManager*   g_GlowObjManager;
inline IViewRender*          g_ViewRender;
inline IDirect3DDevice9*     g_D3DDevice9;
inline CClientState*         g_ClientState;
inline IPhysicsSurfaceProps* g_PhysSurface;
inline IInputSystem*         g_InputSystem;

template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};

inline C_LocalPlayer g_LocalPlayer;