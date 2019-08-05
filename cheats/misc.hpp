#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/protobuff.hpp"
#include "../helpers/math.hpp"
#include "../settings.hpp"
#include "../hooks.hpp"
#include "../render.hpp"
#include <Psapi.h>

void AutoDefuse(CUserCmd* cmd);
void Triggerbot(CUserCmd* cmd);
void InfiniteDuck(CUserCmd* cmd);
void AutoPistol(CUserCmd* cmd);
void AutoReload(CUserCmd* cmd);
bool ChickenTamer(CUserCmd* cmd);
void AutoReload(CUserCmd* cmd);
void SelfNade(CUserCmd* cmd);
void SlowWalk(CUserCmd* cmd);
void EdgeJumper(CUserCmd* cmd);

void RecoilCrosshair();
void RadarHack();
void InverseRagdollGravity();
void RemoveFlash();
void RemoveSmoke();
void SniperCrosshair();
void BulletImpacts();
void NoDecals();
void RemovePostProcessing();
void AdjustBrightness();
void RemoveShadows();
void WorldColour();

void CameraFOV(CViewSetup* vsView);
void ViewmodelFOV(CViewSetup* vsView);

void FakeLag(CUserCmd* cmd, bool& bSendPacket);
void FakeDuck(CUserCmd* cmd, bool& bSendPacket);
void ChatMimic(const void* msg_data);
void AntiKick(const void* msg_data, int nBytes);


class ChatSpammer : public Singleton<ChatSpammer>
{
public:
	void OnCreateMove();
	std::string filedata;
};

namespace FreeLook {
	void OnCreateMove(CUserCmd* cmd);
	void OverrideView(CViewSetup* vsView);
}

class ClantagScroller : public Singleton<ClantagScroller>
{
public:
	void OnCreateMove();
	std::string tag = "Chimphook ";
};

class LastState : public Singleton<LastState>
{
public:
	QAngle viewangles = QAngle(0, 0, 0);
	QAngle virtualViewangles = QAngle(0, 0, 0);
};

namespace ThirdPerson
{
	void OverrideView();
	void FrameStageNotify(ClientFrameStage_t stage);

}

namespace NameStealer
{
	void OnCreateMove();
	void FireGameEvent(IGameEvent* ev);
}

void draw_test();