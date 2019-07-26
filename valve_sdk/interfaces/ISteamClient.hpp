#include <cstdint>
#include "ISteamFriends.hpp"
#include "ISteamUtils.hpp"

class ISteamUser;
class ISteamGameServer;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamHTTP;
class ISteamUnifiedMessages;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;

typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;
typedef uint64_t SteamAPICall_t;

typedef void(__cdecl* SteamAPIWarningMessageHook_t)(int, const char*);
typedef void(*SteamAPI_PostAPIResultInProcess_t)(SteamAPICall_t callHandle, void*, uint32_t unCallbackSize, int iCallbackNum);
typedef uint32_t(*SteamAPI_CheckCallbackRegistered_t)(int iCallbackNum);

class ISteamClient
{
public:
	virtual HSteamPipe CreateSteamPipe() = 0;
	virtual bool BReleaseSteamPipe(HSteamPipe hSteamPipe) = 0;
	virtual HSteamUser ConnectToGlobalUser(HSteamPipe hSteamPipe) = 0;
	virtual HSteamUser CreateLocalUser(HSteamPipe* phSteamPipe, EAccountType eAccountType) = 0;
	virtual void ReleaseUser(HSteamPipe hSteamPipe, HSteamUser hUser) = 0;
	virtual ISteamUser* GetISteamUser(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamGameServer* GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual void SetLocalIPBinding(uint32_t unIP, uint16_t usPort) = 0;
	virtual ISteamFriends* GetISteamFriends(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamUtils* GetISteamUtils(HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamMatchmaking* GetISteamMatchmaking(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamMatchmakingServers* GetISteamMatchmakingServers(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual void* GetISteamGenericInterface(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamUserStats* GetISteamUserStats(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamGameServerStats* GetISteamGameServerStats(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamApps* GetISteamApps(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamNetworking* GetISteamNetworking(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamRemoteStorage* GetISteamRemoteStorage(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamScreenshots* GetISteamScreenshots(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual void RunFrame() = 0;
	virtual uint32_t GetIPCCallCount() = 0;
	virtual void SetWarningMessageHook(SteamAPIWarningMessageHook_t pFunction) = 0;
	virtual bool ShutdownIfAllPipesClosed() = 0;
	virtual ISteamHTTP* GetISteamHTTP(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamUnifiedMessages* GetISteamUnifiedMessages(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamController* GetISteamController(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamUGC* GetISteamUGC(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamAppList* GetISteamAppList(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamMusic* GetISteamMusic(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamMusicRemote* GetISteamMusicRemote(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamHTMLSurface* GetISteamHTMLSurface(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual void Set_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Remove_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(SteamAPI_CheckCallbackRegistered_t func) = 0;
	virtual ISteamInventory* GetISteamInventory(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
	virtual ISteamVideo* GetISteamVideo(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
};