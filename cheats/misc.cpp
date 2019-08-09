#include "../helpers/protoparse/ProtobuffMessages.h"
#include "../helpers/protoparse/ProtoParse.h"
#include "misc.hpp"
#include <random>

bool Settings::Misc::RankReveal = true;
bool Settings::Misc::AutoRevolver = false;
bool Settings::Misc::MoonWalk = false;
bool Settings::Misc::InfiniteDuck = false;
bool Settings::Misc::AutoPistol = false;
bool Settings::Misc::AutoReload = false;
bool Settings::Misc::RecoilCrosshair = false;
bool Settings::Misc::InverseRagdollGravity = false;
bool Settings::Misc::RadarHack = false;
bool Settings::Misc::ChickenTamer = false;
bool Settings::Misc::SelfNade = false;
bool Settings::Misc::EdgeJumper = false;

bool Settings::Misc::SlowWalk::Enabled = false;
int Settings::Misc::SlowWalk::Amount = 100;

bool Settings::Misc::FakeLag::Enabled = false;
int Settings::Misc::FakeLag::Choke = 0;

bool Settings::Misc::Removals::Flash = false;
bool Settings::Misc::Removals::Smoke = false;
bool Settings::Misc::Removals::SmokeOverlay = false;
bool Settings::Misc::Removals::PostProcessing = false;
bool Settings::Misc::Removals::Arms = false;
bool Settings::Misc::Removals::Sleeves = false;
bool Settings::Misc::Removals::Weapon = false;
bool Settings::Misc::Removals::Shadows = false;
int Settings::Misc::Removals::SmokeType = 0;

bool Settings::Misc::SniperCrosshair = false;
bool Settings::Misc::SpectatorList = false;
bool Settings::Misc::BulletImpacts = false;
bool Settings::Misc::NoDecals = false;

bool Settings::Misc::AutoDefuse::Enabled = false;
bool Settings::Misc::FakeZoom::Enabled = false;

bool Settings::Misc::Brightness::Enabled = false;
int Settings::Misc::Brightness::Perc = 0;

bool Settings::Misc::ChatMimic::Enabled = false;
bool Settings::Misc::ChatMimic::CustomMsg = false;
char Settings::Misc::ChatMimic::Msg[256] = "";
bool Settings::Misc::AntiKick = false;

bool Settings::Misc::ChatSpammer::Enabled = false;
std::string Settings::Misc::ChatSpammer::Filename = "";

bool Settings::Misc::ThirdPersonSpectate::Enabled = false;
bool Settings::Misc::ThirdPerson::Enabled = false;
bool Settings::Misc::ThirdPerson::IgnoreScope = false;
bool Settings::Misc::ThirdPerson::IgnorePlayers = false;
bool Settings::Misc::ThirdPerson::IgnoreWalls = false;
bool Settings::Misc::ThirdPerson::RealAngles = false;
int Settings::Misc::ThirdPerson::CameraOffset = 120;

bool Settings::Misc::FreeLook::Enabled = false;

bool Settings::Misc::Sounds::HitSound::Enabled = false;
int Settings::Misc::Sounds::HitSound::Sound = 0;

bool Settings::System::PlayerList::NameStealer::Enabled = false;
bool Settings::System::PlayerList::NameStealer::Enemies = false;
int Settings::System::PlayerList::NameStealer::Delay = 200;

bool Settings::Misc::FakeDuck::Enabled = false;

bool Settings::Misc::ClantagScroller::Enabled = false;
std::string Settings::Misc::ClantagScroller::Clantag = "Chimphook";
int Settings::Misc::ClantagScroller::Delay = 250;

bool Settings::Visuals::Nightmode::Enabled = false;
float Settings::Visuals::Nightmode::Intensity = 0.f;

bool Settings::Misc::ViewmodelOffset::Enabled = false;
float Settings::Misc::ViewmodelOffset::X = 0.f;
float Settings::Misc::ViewmodelOffset::Y = 0.f;
float Settings::Misc::ViewmodelOffset::Z = 0.f;

void InfiniteDuck(CUserCmd* cmd)
{
	if (!Settings::Misc::InfiniteDuck)
		return;

	cmd->buttons |= IN_BULLRUSH;
}

void AutoPistol(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoPistol)
		return;

	if (!g_LocalPlayer || !g_EngineClient->IsInGame())
		return;

	C_BaseCombatWeapon* activeWeapon = g_LocalPlayer->m_hActiveWeapon();

	if (!activeWeapon || !activeWeapon->IsPistol())
		return;

	if (cmd->buttons & IN_ATTACK && activeWeapon->CanFire())
	{
		cmd->buttons |= IN_ATTACK;
	}
	else if (cmd->buttons & IN_ATTACK && !activeWeapon->CanFire())
	{
		cmd->buttons &= ~IN_ATTACK;
	}
}

void AutoReload(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoReload)
		return;

	C_BaseCombatWeapon* activeWeapon = g_LocalPlayer->m_hActiveWeapon();

	if (!activeWeapon)
		return;

	if (!activeWeapon->m_iClip1())
	{
		cmd->buttons &= ~IN_ATTACK | IN_ATTACK2;
	}
}

void RemoveFootsteps()
{
	/*CUtlVector<SndInfo_t> sndList;
	sndList.RemoveAll();
	g_EngineSound->GetActiveSounds(sndList);

	for (int i = 0; i < sndList.Count(); i++)
	{
		if (!sndList[i].m_pOrigin)
			continue;

		Utils::ConsolePrint(std::to_string(sndList[i].m_nSoundSource).c_str());

		//C_BasePlayer* pTarget = C_BasePlayer::GetPlayer(sndList[i].m_nSoundSource);

		//if (!pTarget || pTarget->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		//	continue;

		g_EngineSound->StopSoundByGuid(sndList[i].m_nGuid, true);
	}*/
}

void FakeLag(CUserCmd* cmd, bool& bSendPacket)
{
	if (!Settings::Misc::FakeLag::Enabled || !g_EngineClient->IsInGame() || !g_LocalPlayer)
		return;

	if (cmd->buttons & IN_ATTACK) return;
	if (g_LocalPlayer->m_vecVelocity() == Vector(0, 0, 0)) return;

	int iChoke = Settings::Misc::FakeLag::Choke;

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}

void RecoilCrosshair()
{
	ConVar* recoilCrosshair = g_CVar->FindVar("cl_crosshair_recoil");

/*	static bool oldrc = recoilCrosshair->GetBool();

	if (oldrc != Settings::Misc::RecoilCrosshair)
	{
		recoilCrosshair->SetValue(Settings::Misc::RecoilCrosshair ? 1 : 0);
		oldrc = Settings::Misc::RecoilCrosshair;
	}
	if (recoilCrosshair->GetBool() != Settings::Misc::RecoilCrosshair)
	{
		Settings::Misc::RecoilCrosshair = recoilCrosshair->GetBool();
	}*/

	recoilCrosshair->SetValue(Settings::Misc::RecoilCrosshair ? 1 : 0);
}

void RemovePostProcessing()
{
	ConVar* postprocessing = g_CVar->FindVar("mat_postprocess_enable");


	postprocessing->SetValue(Settings::Misc::Removals::PostProcessing ? 0 : 1);
}

static QAngle oldv = QAngle(0, 90, 0);


void SniperCrosshair()
{
	if (!Settings::Misc::SniperCrosshair)
		return;

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer)
		return;

	static ConVar* showSpread = g_CVar->FindVar("weapon_debug_spread_show");

	bool isobs = false;
	C_BasePlayer* player = g_LocalPlayer->SelfOrObs(&isobs);
	OBS_MODES obsmode = OBS_MODE_NONE;

	if (isobs)
	{
		OBS_MODES obsmode = (OBS_MODES)g_LocalPlayer->m_iObserverMode();
	}

	if (obsmode == OBS_MODE_IN_EYE || obsmode == OBS_MODE_DEATHCAM || !isobs)
		if (g_LocalPlayer->SelfOrObs() && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
			if (g_LocalPlayer->SelfOrObs()->m_hActiveWeapon())
				if (g_LocalPlayer->SelfOrObs()->m_hActiveWeapon()->IsSniper() || g_LocalPlayer->SelfOrObs()->m_hActiveWeapon()->IsAuto())
					showSpread->SetValue(3);
				else
					showSpread->SetValue(0);
			else
				showSpread->SetValue(0);
		else
			showSpread->SetValue(0);
	else
		showSpread->SetValue(0);
}

void RadarHack()
{
	if (!Settings::Misc::RadarHack)
		return;

	for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || pl == g_LocalPlayer)
			continue;

		*pl->m_bSpotted() = true;
	}
}

void InverseRagdollGravity()
{
	ConVar* ragdollGrav = g_CVar->FindVar("cl_ragdoll_gravity");
	ragdollGrav->SetValue(Settings::Misc::InverseRagdollGravity ? -600 : 600);
}

void RemoveFlash()
{
	if (!Settings::Misc::Removals::Flash)
		return;

	g_LocalPlayer->m_flFlashDuration() = 0.f;
}

void RemoveSmoke()
{
	static constexpr const char* smokeMaterials[]{
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull",
	};

	for (const auto mat : smokeMaterials)
	{
		auto material = g_MatSystem->FindMaterial(mat);
		
		material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Misc::Removals::SmokeType == 0 && Settings::Misc::Removals::Smoke);
		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Settings::Misc::Removals::SmokeType == 1 && Settings::Misc::Removals::Smoke);
	}
}

static bool oldbi = Settings::Misc::BulletImpacts;

void BulletImpacts()
{
	ConVar* bulletImpacts = g_CVar->FindVar("sv_showimpacts");

	/*
	if (oldbi != Settings::Misc::BulletImpacts) {
		bulletImpacts->SetValue(Settings::Misc::BulletImpacts ? 1 : 0);
		oldbi = Settings::Misc::BulletImpacts;
	}
	if (bulletImpacts->GetBool() != Settings::Misc::BulletImpacts)
	{
		Settings::Misc::BulletImpacts = bulletImpacts->GetBool();
	}*/

	bulletImpacts->SetValue(Settings::Misc::BulletImpacts ? 1 : 0);
}

void NoDecals()
{
	if (!Settings::Misc::NoDecals)
		return;

	g_EngineClient->ClientCmd("r_cleardecals");
}

float Distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool ChickenTamer(CUserCmd* cmd)
{
	if (!Settings::Misc::ChickenTamer)
		return false;

	Vector ownPos = g_LocalPlayer->GetEyePos();

	if (cmd->buttons & IN_ATTACK)
		return false;

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity->IsChicken())
		{
			Vector pos = entity->m_vecOrigin();
			pos.z += 7.5;

			float dist = pos.DistTo(ownPos);
			C_BasePlayer* playerLeader = entity->m_hLeader();

			if (!playerLeader || playerLeader != g_LocalPlayer)
			{
				if (dist < 80)
				{

					QAngle angle = Math::CalcAngle(ownPos, pos);
				
					QAngle oldAngle = cmd->viewangles;
					float oldForward = cmd->forwardmove;
					float oldSidemove = cmd->sidemove;

					cmd->viewangles = angle;
					cmd->buttons |= IN_USE;

					Math::CorrectMovement(oldAngle, cmd, oldForward, oldSidemove);

					return true;
				}
			}
		}
	}

	return false;
}

void FreeLook::OnCreateMove(CUserCmd* cmd)
{
	if (!Settings::Misc::FreeLook::Enabled || !Settings::Misc::FreeLook::_enabled)
	{
		LastState::Get().viewangles = cmd->viewangles;
		LastState::Get().virtualViewangles.Init(0, 0, 0);
	}
	else
	{
		if (LastState::Get().virtualViewangles.IsZero())
		{
			LastState::Get().virtualViewangles = LastState::Get().viewangles;
		}

		LastState::Get().virtualViewangles += cmd->viewangles - LastState::Get().viewangles;

		LastState::Get().virtualViewangles.Normalize();
		if (LastState::Get().virtualViewangles.pitch >= 89.0f) LastState::Get().virtualViewangles.pitch = 89.0f;
		else if (LastState::Get().virtualViewangles.pitch <= -89.0f) LastState::Get().virtualViewangles.pitch = -89.0f;

		g_EngineClient->SetViewAngles(&LastState::Get().viewangles);
	}
}

void FreeLook::OverrideView(CViewSetup* vsView)
{
	if (Settings::Misc::FreeLook::Enabled && Settings::Misc::FreeLook::_enabled && !LastState::Get().virtualViewangles.IsZero() && !g_Input->m_fCameraInThirdPerson)
	{
		vsView->angles.yaw = LastState::Get().virtualViewangles.yaw;
		vsView->angles.pitch = LastState::Get().virtualViewangles.pitch;
	}
}

void DoThirdPersonSpectate()
{
	if (!g_LocalPlayer)
		return;

	if (!g_LocalPlayer->m_hObserverTarget())
		return;

	if (Settings::Misc::ThirdPersonSpectate::Enabled && Settings::Misc::ThirdPersonSpectate::_enabled)
	{
		g_LocalPlayer->m_iObserverMode() = OBS_MODE_CHASE;
	}
	else
	{
		g_LocalPlayer->m_iObserverMode() = OBS_MODE_IN_EYE;
	}
}

void ThirdPerson::OverrideView()
{
	if (!g_LocalPlayer)
		return;

	if (Settings::Misc::ThirdPerson::Enabled && Settings::Misc::ThirdPerson::_enabled && (!g_LocalPlayer->SelfOrObs()->m_bIsScoped() || Settings::Misc::ThirdPerson::IgnoreScope))
	{
		if (!g_LocalPlayer->m_hObserverTarget()) {
			if (!g_Input->m_fCameraInThirdPerson)
			{
				g_Input->m_fCameraInThirdPerson = true;
			}

			QAngle view;// = LastState::Get().virtualViewangles;
			g_EngineClient->GetViewAngles(&view);

			auto GetCorrectDistance = [](float ideal_distance, QAngle angle) -> float
			{
				angle.pitch *= -1.f, angle.yaw += 180.f;

				Vector direction;
				Math::AngleVectors(angle, direction);

				if (Settings::Misc::ThirdPerson::IgnorePlayers)
				{
					CTraceFilterWorldAndPropsOnly filter;
					trace_t trace;
					Ray_t ray;

					ray.Init(g_LocalPlayer->SelfOrObs()->GetEyePos(), g_LocalPlayer->SelfOrObs()->GetEyePos() + (direction * ideal_distance));

					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

					return (ideal_distance * trace.fraction) - 10.f;
				}
				else
				{
					CTraceFilter filter;
					filter.pSkip = g_LocalPlayer;
					trace_t trace;
					Ray_t ray;

					ray.Init(g_LocalPlayer->SelfOrObs()->GetEyePos(), g_LocalPlayer->SelfOrObs()->GetEyePos() + (direction * ideal_distance));

					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

					return (ideal_distance * trace.fraction) - 10.f;
				}
			};

			g_Input->m_fCameraInThirdPerson = true;

			if (Settings::Misc::ThirdPerson::IgnoreWalls)
				g_Input->m_vecCameraOffset.z = Settings::Misc::ThirdPerson::CameraOffset - 10.f;
			else
				g_Input->m_vecCameraOffset.z = GetCorrectDistance(Settings::Misc::ThirdPerson::CameraOffset, view);
		}
		else
		{
			g_Input->m_fCameraInThirdPerson = false;

			DoThirdPersonSpectate();
		}
	}
	else
	{
		g_Input->m_fCameraInThirdPerson = false;

		if (g_LocalPlayer->m_hObserverTarget()) DoThirdPersonSpectate();
	}
}

void ThirdPerson::FrameStageNotify(ClientFrameStage_t stage)
{
	if (!g_EngineClient->IsInGame())
		return;

	if (stage != ClientFrameStage_t::FRAME_RENDER_START)
		return;

	if (Settings::Misc::ThirdPerson::Enabled && Settings::Misc::ThirdPerson::_enabled && (!g_LocalPlayer->m_bIsScoped() || Settings::Misc::ThirdPerson::IgnoreScope) && Settings::Misc::ThirdPerson::RealAngles)
		*g_LocalPlayer->GetVAngles() = CreateMove::lastTickViewAngles;
}

void ChatMimic(const void* msg_data)
{
	bf_read read(reinterpret_cast<uintptr_t>(msg_data));
	read.SetOffset(1);
	int ent_index = read.ReadByte();
	read.Skip(3);
	std::string msg_name = read.ReadString();
	std::string player_name = read.ReadString();
	std::string message = read.ReadString();
	read.Skip(3);
	bool all_chat = read.ReadBool();

	std::string chat;
	chat += "say ";

	C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(ent_index);

	if (!pl->notfriend || pl == g_LocalPlayer)
		return;

	if (!Settings::Misc::ChatMimic::CustomMsg) {
		chat += message;
	}
	else
	{
		chat += Settings::Misc::ChatMimic::Msg;
	}

	g_EngineClient->ClientCmd(chat.c_str());
}

enum Team
{
	TEAM_NONE,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST
};

void AntiKick(const void* msg_data, int nBytes)
{
	/*CSUsrMsg_VoteStart msg((void*)msg_data, nBytes);

	auto team_idx = msg.get_team().get();
	if (team_idx == TEAM_COUNTER_TERRORIST || team_idx == TEAM_TERRORIST)
	{
		auto kicker_idx = msg.ent_idx().get();
		auto disp_str = msg.disp_str().get();
		auto kicked_nick = msg.details_str().get();
		auto kicker_entity = C_BasePlayer::GetPlayerByIndex(kicker_idx - 1);

		C_BasePlayer* local_entity = g_LocalPlayer;

		if (local_entity
			&& kicker_entity
			&& local_entity != kicker_entity
			&& local_entity->m_iTeamNum() == team_idx
			&& !disp_str.compare("#SFUI_vote_kick_player_other"))
		{
			auto local_nick = local_entity->GetPlayerInfo().szName;

			if (!kicked_nick.compare(local_nick) || kicked_nick.size() == 0)
			{
				g_EngineClient->ClientCmd_Unrestricted("callvote SwapTeams");
			}
		}
	}*/
}

static int CurrentPl = 0;

void NameStealer::OnCreateMove()
{
	if (!Settings::System::PlayerList::NameStealer::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < Settings::System::PlayerList::NameStealer::Delay)
		return;

	if (CurrentPl > 64)
		CurrentPl = 0;

	if (CurrentPl == -1) {
		Utils::SetName("\n\xAD\xAD\xAD");
		CurrentPl++;
		return;
	}

	while (CurrentPl < g_EngineClient->GetMaxClients())
	{
		CurrentPl++;

		if (CurrentPl >= g_EngineClient->GetMaxClients())
			CurrentPl = 1;

		if (CurrentPl == 0)
		{
			Utils::SetName("\n\xAD\xAD\xAD");

			timeStamp = currentTime_ms;
			break;
		}

		C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(CurrentPl);

		if (!player)
			continue;

		if (!player->IsPlayer())
			continue;

		if (!Settings::System::PlayerList::NameStealer::Enemies && g_LocalPlayer->m_iTeamNum() != player->m_iTeamNum())
			continue;

		player_info_t info = player->GetPlayerInfo();

		if (!info.szName || info.fakeplayer)
			continue;

		std::string set;

		set += info.szName;
		set += " ";
		char* setn = &set[0];

		Utils::SetName(setn);

		timeStamp = currentTime_ms;

		break;
	}
}

void NameStealer::FireGameEvent(IGameEvent* ev)
{
	if (!ev)
		return;

	if (!strcmp(ev->GetName(), "game_start") || !strcmp(ev->GetName(), "round_start") || !strcmp(ev->GetName(), "game_newmap"))
	{
		CurrentPl = -1;
	}
}

void FakeDuck(CUserCmd* cmd, bool& bSendPacket)
{
	if (!Settings::Misc::FakeDuck::Enabled)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	static bool counter = false;

	bool once = false;

	if (Settings::Misc::FakeDuck::_enabled)
	{
		static bool counter = false;
		static int counters = 0;
		if (counters == 9)
		{
			counters = 0;
			counter = !counter;
		}
		counters++;
		if (counter)
		{
			cmd->buttons |= IN_DUCK;
			bSendPacket = true;
		}
		else
			cmd->buttons &= ~IN_DUCK;
	}
}



void AutoDefuse(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoDefuse::Enabled)
		return;

	if (!Settings::Misc::AutoDefuse::_enabled)
		return;

	C_BaseEntity* c4e = nullptr;

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity->IsPlantedC4())
		{
			c4e = entity;

			break;
		}
	}

	if (c4e)
	{
		C_PlantedC4* c4 = (C_PlantedC4*)c4e;
		if (g_LocalPlayer->IsAlive() && c4->m_bBombTicking() && (!c4->m_hBombDefuser() || c4->m_hBombDefuser() == g_LocalPlayer))
		{
			float bombTimer = c4->m_flC4Blow() - g_GlobalVars->curtime;
			bool shouldDefuse = g_LocalPlayer->m_bHasDefuser() ? (bombTimer < 5.2) : (bombTimer < 10.2);

			if (shouldDefuse)
			{

				cmd->buttons |= IN_USE;
			}
			else
			{
				cmd->buttons &= ~IN_USE;
			}
		}
	}
}

void ClantagScroller::OnCreateMove()
{
	if (!Settings::Misc::ClantagScroller::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < Settings::Misc::ClantagScroller::Delay)
		return;

	timeStamp = currentTime_ms;

	if (tag.size() == 0)
		return;

	std::rotate(std::begin(tag), std::next(std::begin(tag)), std::end(tag));
	Utils::SetClantag(tag.substr(0, 16).c_str());
}

inline bool exists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void ChatSpammer::OnCreateMove()
{
	if (!Settings::Misc::ChatSpammer::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < 200)
		return;

	std::vector<std::string> lines = Utils::Split(filedata, "\n");

	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, lines.size() - 1);
	int rnum = dist(engine);

	if (rnum < lines.size()) {
		std::string line = lines[rnum];

		std::string cmd = "say ";
		cmd += line.c_str();

		g_EngineClient->ClientCmd_Unrestricted(cmd.c_str());

		timeStamp = currentTime_ms;
	}
}

bool FakeZoom(CViewSetup* vsView)
{
	if (Settings::Misc::FakeZoom::Enabled && Settings::Misc::FakeZoom::_enabled && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
	{
		vsView->fov -= 45;
	}

	return false;
}

void CameraFOV(CViewSetup* vsView)
{
	bool isobs = false;
	C_BasePlayer* player = g_LocalPlayer->SelfOrObs(&isobs);

	if (FakeZoom(vsView))
		return;

	if (isobs)
	{
		OBS_MODES obsmode = (OBS_MODES)g_LocalPlayer->m_iObserverMode();

		if (obsmode != OBS_MODE_IN_EYE && obsmode != OBS_MODE_DEATHCAM)
		{
			return;
		}
	}


	if (Settings::Misc::CameraFOV::Enabled && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
	{
		vsView->fov += Settings::Misc::CameraFOV::FOV;
	}
}

void ViewmodelFOV(CViewSetup* vsView)
{
	static ConVar* viewmodel_fov = g_CVar->FindVar("viewmodel_fov");
	viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_x = g_CVar->FindVar("viewmodel_offset_x");
	viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_y = g_CVar->FindVar("viewmodel_offset_y");
	viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_z = g_CVar->FindVar("viewmodel_offset_z");
	viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;

	if (Settings::Misc::ViewmodelFOV::Enabled)
		viewmodel_fov->SetValue(Settings::Misc::ViewmodelFOV::FOV);
	else
		viewmodel_fov->SetValue(60);

	if (Settings::Misc::ViewmodelOffset::Enabled)
	{
		viewmodel_offset_x->SetValue(Settings::Misc::ViewmodelOffset::X);
		viewmodel_offset_y->SetValue(Settings::Misc::ViewmodelOffset::Y);
		viewmodel_offset_z->SetValue(Settings::Misc::ViewmodelOffset::Z);
	}
	else
	{
		viewmodel_offset_x->SetValue(0);
		viewmodel_offset_y->SetValue(0);
		viewmodel_offset_z->SetValue(0);
	}
}

void AdjustBrightness()
{
	static ConVar* mat_force_tonemap_scale = g_CVar->FindVar("mat_force_tonemap_scale");
	
	if (Settings::Misc::Brightness::Enabled)
		mat_force_tonemap_scale->SetValue((float)Settings::Misc::Brightness::Perc / 100.f);
	else
		mat_force_tonemap_scale->SetValue(mat_force_tonemap_scale->m_pszDefaultValue);
}

void RemoveShadows()
{
	static ConVar* shadows = g_CVar->FindVar("cl_csm_enabled");
	shadows->SetValue(!Settings::Misc::Removals::Shadows);
}

void SelfNade(CUserCmd* cmd)
{
	if (!Settings::Misc::SelfNade)
		return;

	const auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (!weapon) return;
	if (!weapon->IsGrenade()) return;

	const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(weapon);

	if (grenade->m_flThrowStrength() >= 0.11f || grenade->m_flThrowStrength() <= 0.10f || cmd->viewangles.pitch > -88.0f)
		return;

	cmd->buttons &= ~IN_ATTACK;
	cmd->buttons &= ~IN_ATTACK2;
}

void SlowWalk(CUserCmd* cmd)
{
	if (!Settings::Misc::SlowWalk::Enabled || !GetAsyncKeyState(VK_SHIFT))
		return;

	C_BaseCombatWeapon * weapon_handle = g_LocalPlayer->m_hActiveWeapon().Get();

	if (!weapon_handle)
		return;

	float amount = 0.0034f * Settings::Misc::SlowWalk::Amount;

	Vector velocity = g_LocalPlayer->m_vecVelocity();
	QAngle direction;

	Math::VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.yaw = cmd->viewangles.yaw - direction.yaw;

	Vector forward;

	Math::AngleVectors(direction, forward);

	Vector source = forward * -speed;

	if (speed >= (weapon_handle->GetCSWeaponData()->max_speed * amount))
	{
		cmd->forwardmove = source.x;
		cmd->sidemove = source.y;
	}
}

void WorldColour::Clear()
{
	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World"))
		{
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
		}
	}

	if (Settings::Visuals::Nightmode::Enabled)
		Nightmode::Get().Execute();
}

void WorldColour::Execute()
{
	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World"))
		{
			// pMaterial->AlphaModulate(0.5f);
			pMaterial->ColorModulate(Settings::Visuals::WorldColour::Colour.r() / 255.f, Settings::Visuals::WorldColour::Colour.g() / 255.f, Settings::Visuals::WorldColour::Colour.b() / 255.f);
		}
	}
}

void AutoRevolver(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoRevolver)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->m_hActiveWeapon())
		return;

	if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_RELOAD)
		return;

	if (g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() == WEAPON_REVOLVER2 || g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		static int RevolverDelay = 0;
		RevolverDelay++;

		if (RevolverDelay <= 15)
			cmd->buttons |= IN_ATTACK;
		else {
			RevolverDelay = 0;

			float flPostponeFireReadyTime = g_LocalPlayer->m_hActiveWeapon()->m_flPostponeFireReadyTime();

			if (flPostponeFireReadyTime > 0 && flPostponeFireReadyTime < g_GlobalVars->curtime)
				cmd->buttons &= ~IN_ATTACK;
		}
	}

}

static ITexture* buffer;

void draw_test()
{
	/*
	CMatRenderContextPtr render_ctx(g_MatSystem);

	if (!buffer)
		buffer = g_MatSystem->CreateNamedRenderTargetTextureEx(
			"esp_preview", 1, 1, RT_SIZE_FULL_FRAME_BUFFER, g_MatSystem->GetBackBufferFormat(),
			MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			CREATERENDERTARGETFLAGS_HDR);

	int vp_x, vp_y, vp_w, vp_h;
	render_ctx->GetViewport(vp_x, vp_y, vp_w, vp_h);
	CViewSetup view;

	view.x = 50 + vp_x;
	view.y = 50 + vp_y;
	view.width = vp_w;
	view.height = vp_h;
	view.m_bOrtho = false;

	view.fov = 20;
	view.origin = Vector (-220, -4, 35);
	view.angles.Init();
	view.zNear = 8;
	view.zFar = 1000;
	view.m_flAspectRatio = 1.f;
	static ITexture* pCubemapTexture = g_MatSystem->FindTexture("editor/cubemap.hdr", NULL, true);
	//render_ctx->BindLocalCubemap(pCubemapTexture);
	//render_ctx->SetLightingOrigin(0, 0, 0);

	static Vector white[6] =
	{
		Vector(0.4, 0.4, 0.4),
		Vector(0.4, 0.4, 0.4),
		Vector(0.4, 0.4, 0.4),
		Vector(0.4, 0.4, 0.4),
		Vector(0.4, 0.4, 0.4),
		Vector(0.4, 0.4, 0.4),
	};

	static Vector4D whites[6] =
	{
		Vector4D(0.4, 0.4, 0.4, 1.f),
		Vector4D(0.4, 0.4, 0.4, 1.f),
		Vector4D(0.4, 0.4, 0.4, 1.f),
		Vector4D(0.4, 0.4, 0.4, 1.f),
		Vector4D(0.4, 0.4, 0.4, 1.f),
		Vector4D(0.4, 0.4, 0.4, 1.f),
	};

	render_ctx->SetAmbientLightCube(whites);
	g_StudioRender->SetAmbientLightColors(white);
	g_StudioRender->SetLocalLights(0, NULL);

	Frustum dummyFrustum;
	g_RenderView->Push3DView(view, 0, buffer, dummyFrustum);
	render_ctx->ClearColor4ub(0, 0, 0, 0);
	render_ctx->ClearBuffers(true, false, false);
	g_MdlRender->SuppressEngineLighting(true);

	float color[3] = { 1.f, 1.f, 1.f };
	g_RenderView->SetColorModulation(color);
	g_RenderView->SetBlend(1.f);
	g_LocalPlayer->SetAbsOrigin(Vector(0, 0, 0));
	g_LocalPlayer->SetAbsAngles(QAngle(0, -90, 0));
	g_LocalPlayer->DrawModel(1, 255);
	g_MdlRender->SuppressEngineLighting(false);
	g_RenderView->PopView(dummyFrustum);
	render_ctx->BindLocalCubemap(NULL);*/
}

void LoadSky(const char* skyname)
{
	static void (__fastcall* LoadNamedSky)(const char*) = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::PatternScan(GetModuleHandleW(L"engine.dll"), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	LoadNamedSky(skyname);
}

void Nightmode::Clear()
{
	LoadSky(currentSkyName);

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		static IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
		{
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, true);
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
		}
	}
}

void Nightmode::Execute()
{
	LoadSky("sky_csgo_night02");

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
		{
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
			pMaterial->ColorModulate(1.f - Settings::Visuals::Nightmode::Intensity, 1.f - Settings::Visuals::Nightmode::Intensity, 1.f - Settings::Visuals::Nightmode::Intensity);
		}
	}
}