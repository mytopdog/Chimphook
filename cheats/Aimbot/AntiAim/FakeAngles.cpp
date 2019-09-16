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

	}
}

matrix3x4_t FakeMatrix[128];
CCSGOPlayerAnimState* FakeAnimState;

void ManageFakeAnimState()
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	float backup_pose = g_LocalPlayer->m_flPoseParameter()[11];
	QAngle backup_abs_angles = g_LocalPlayer->GetAngles();

	g_LocalPlayer->m_flPoseParameter()[11] = 0.5f;
	g_LocalPlayer->SetAbsAngles(QAngle(0.f, CreateMove::RealAngles.yaw, 0.f));

	g_LocalPlayer->SetupBones(FakeMatrix, 128, 0x7FF00, g_GlobalVars->curtime);

	g_LocalPlayer->m_flPoseParameter()[11] = backup_pose;
	g_LocalPlayer->SetAbsAngles(backup_abs_angles);

		/*
	static CBaseHandle handle = g_LocalPlayer->GetRefEHandle();
	static float spawn_time = g_LocalPlayer->m_flSpawnTime();

	bool allocate = (!FakeAnimState),
		change = (!allocate) && (g_LocalPlayer->GetRefEHandle() != handle),
		reset = (!allocate && !change) && (g_LocalPlayer->m_flSpawnTime() != spawn_time);

	if (change)
		free(FakeAnimState);

	if (reset)
	{
		g_LocalPlayer->ResetAnimationState(FakeAnimState);

		spawn_time = g_LocalPlayer->m_flSpawnTime();
	}

	static float oldSimTime = 0;

	if (allocate || change)
	{
		CCSGOPlayerAnimState* state = g_LocalPlayer->GetPlayerAnimState();

		if (state != nullptr)
			g_LocalPlayer->CreateAnimationState(state);

		handle = g_LocalPlayer->GetRefEHandle();
		spawn_time = g_LocalPlayer->m_flSpawnTime();

		FakeAnimState = state;
	}
	else if (g_LocalPlayer->m_flSimulationTime() != oldSimTime)
	{
		AnimationLayer networked_layers[13];
		memcpy(networked_layers, g_LocalPlayer->GetAnimOverlays(), 13);

		QAngle backup_abs_angles = g_LocalPlayer->GetAngles();
		std::array<float, 24> backup_poses = g_LocalPlayer->m_flPoseParameter();

		g_LocalPlayer->UpdateAnimationState(FakeAnimState, CreateMove::RealAngles);
		g_LocalPlayer->SetupBones(FakeMatrix, 128, 0x7FF00, g_GlobalVars->curtime);

		memcpy(g_LocalPlayer->GetAnimOverlays(), networked_layers, 13);

		g_LocalPlayer->m_flPoseParameter() = backup_poses;
		g_LocalPlayer->GetAngles() = backup_abs_angles;
	}

	oldSimTime = g_LocalPlayer->m_flSimulationTime();*/
}