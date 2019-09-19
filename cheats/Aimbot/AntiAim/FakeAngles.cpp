#include "FakeAngles.hpp"

float GetCurTime(CUserCmd* cmd)
{
	static int tick = 0;
	static CUserCmd* lastCmd = nullptr;

	if (!lastCmd || lastCmd->hasbeenpredicted)
	{
		tick = g_LocalPlayer->m_nTickBase();
	}
	else
	{
		++tick;
	}
	lastCmd = cmd;
	float curtime = tick * g_GlobalVars->interval_per_tick;
	return curtime;
}

bool ShouldBreakLBY(CUserCmd* cmd)
{
	static float next_lby_update_time = 0;
	float curtime = GetCurTime(cmd);

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		return false;

	if (g_LocalPlayer->m_vecVelocity().Length() > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

inline float NormaliseQuick(float yaw)
{
	while (yaw <= -18000) yaw += 36000;
	while (yaw > 18000) yaw -= 36000;

	while (yaw < -180) yaw += 360;
	while (yaw > 180) yaw -= 360;

	return yaw;
}

int Settings::AntiAim::Fake::Type = 0;
int Settings::AntiAim::Fake::Legit::Side = 0;
int Settings::AntiAim::Fake::Legit::SideKey = 0;

bool Settings::AntiAim::Fake::Rage::LBYBreak = 0;
int Settings::AntiAim::Fake::Rage::Lean = 0;
int Settings::AntiAim::Fake::Rage::LeanKey = 0;

void DoFakeAngs(CUserCmd* cmd, bool& bSendPacket)
{
	if (!Settings::AntiAim::Fake::Type)
		return;

	if (Settings::AntiAim::Fake::Type == 1)
	{
		// Legit AA;

		static bool prebf = false;

		if (GetAsyncKeyState(Settings::AntiAim::Fake::Legit::SideKey))
		{
			if (!prebf)
			{
				Settings::AntiAim::Fake::Legit::Side = 1 - Settings::AntiAim::Fake::Legit::Side;
			}

			prebf = true;
		}
		else
		{
			prebf = false;
		}

		CreateMove::CurrentLBY = g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw;

		if (ShouldBreakLBY(cmd))
		{
			if (!Settings::AntiAim::Fake::Legit::Side)
			{
				if (NormaliseQuick(cmd->viewangles.yaw - g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw) < 57.f)
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw - 125.f);
					cmd->viewangles.yaw -= 125.f;
				}
				else
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw - 180.f);
					cmd->viewangles.yaw -= 180.f;
				}
			}
			else
			{
				if (NormaliseQuick(cmd->viewangles.yaw - g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw) > -57.f)
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw + 125.f);
					cmd->viewangles.yaw += 125.f;
				}
				else
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw + 180.f);
					cmd->viewangles.yaw += 180.f;
				}
			}

			bSendPacket = false;

			return;
		}
		
		if (!bSendPacket)
		{
			cmd->viewangles.yaw += g_LocalPlayer->GetMaxDesyncDelta() * (Settings::AntiAim::Fake::Legit::Side ? -1 : 1);
		}
	}

	if (Settings::AntiAim::Fake::Type == 2)
	{

		// Rage AA;

		static bool prebf = false;

		if (GetAsyncKeyState(Settings::AntiAim::Fake::Rage::LeanKey))
		{
			if (!prebf)
			{
				Settings::AntiAim::Fake::Rage::Lean = 1 - Settings::AntiAim::Fake::Rage::Lean;
			}

			prebf = true;
		}
		else
		{
			prebf = false;
		}

		CreateMove::CurrentLBY = g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw;

		if (Settings::AntiAim::Fake::Rage::LBYBreak && ShouldBreakLBY(cmd))
		{
			if (!Settings::AntiAim::Fake::Rage::Lean)
			{
				if (NormaliseQuick(cmd->viewangles.yaw - g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw) < 57.f)
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw - 125.f);
					cmd->viewangles.yaw -= 125.f;
				}
				else
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw - 180.f);
					cmd->viewangles.yaw -= 180.f;
				}
			}
			else
			{
				if (NormaliseQuick(cmd->viewangles.yaw - g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw) > -57.f)
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw + 125.f);
					cmd->viewangles.yaw += 125.f;
				}
				else
				{
					CreateMove::GoalBreakDelta = NormaliseQuick(cmd->viewangles.yaw + 180.f);
					cmd->viewangles.yaw += 180.f;
				}
			}

			bSendPacket = false;

			return;
		}

		if (!Settings::AntiAim::Fake::Rage::LBYBreak)
		{
			if (g_LocalPlayer->m_fFlags() & FL_ONGROUND && cmd->sidemove < 3 && cmd->sidemove > -3)
			{
				static bool switchsm = false;
				cmd->sidemove = switchsm ? 2.f : -2.f;

				switchsm = !switchsm;
			}
		}
		
		if (!bSendPacket)
		{
			cmd->viewangles.yaw += g_LocalPlayer->GetMaxDesyncDelta() * (Settings::AntiAim::Fake::Rage::Lean ? -1 : 1);
		}
	}
}

matrix3x4_t FakeMatrix[128];
CCSGOPlayerAnimState* FakeAnimState = nullptr;

void ManageFakeAnimState()
{
	if (!Settings::Visuals::Chams::Players::Localplayer::DoFakeChams)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;
	
	static float spawn_time = g_LocalPlayer->m_flSpawnTime();
	static CBaseHandle* handle = const_cast<CBaseHandle*>(&g_LocalPlayer->GetRefEHandle());;

	bool allocate = (!FakeAnimState),
		change = (!allocate) && (&g_LocalPlayer->GetRefEHandle() != handle),
		reset = (!allocate && !change) && (g_LocalPlayer->m_flSpawnTime() != spawn_time);

	if (allocate)
		free(FakeAnimState);

	if (allocate || change)
	{
		CCSGOPlayerAnimState* state = reinterpret_cast<CCSGOPlayerAnimState*>(malloc(sizeof(CCSGOPlayerAnimState)));

		if (state != nullptr)
			g_LocalPlayer->CreateAnimationState(state);

		handle = const_cast<CBaseHandle*>(&g_LocalPlayer->GetRefEHandle());;
		spawn_time = g_LocalPlayer->m_flSpawnTime();

		FakeAnimState = state;
	}
	else if (g_LocalPlayer->m_flSpawnTime() != spawn_time)
	{
		C_BasePlayer::ResetAnimationState(FakeAnimState);

		spawn_time = g_LocalPlayer->m_flSpawnTime();
	}
	else
	{
		if (FakeAnimState)
		{
			AnimationLayer networked_layers[13];
			memcpy(networked_layers, g_LocalPlayer->GetAnimOverlays(), 13);

			QAngle backup_abs_angles = g_LocalPlayer->GetAngles();
			std::array<float, 24> backup_poses = g_LocalPlayer->m_flPoseParameter();

			g_LocalPlayer->UpdateAnimationState(FakeAnimState, QAngle(0, 0, 0));
			g_LocalPlayer->InvalidateBoneCache();
			g_LocalPlayer->SetupBones(FakeMatrix, 128, 0x7FF00, g_GlobalVars->curtime);

			memcpy(g_LocalPlayer->GetAnimOverlays(), networked_layers, 13);

			g_LocalPlayer->m_flPoseParameter() = backup_poses;
			g_LocalPlayer->GetAngles() = backup_abs_angles;
		}
	}
}