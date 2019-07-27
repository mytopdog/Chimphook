#include "movement.hpp"
#include "../helpers/math.hpp"

bool Settings::Misc::BunnyHop = false;
bool Settings::Misc::AutoStrafer = false;

bool Settings::Misc::AutoCowboy = false;
bool Settings::Misc::ReverseCowboy = false;

bool Settings::Misc::BlockBot = false;

void BunnyHop(CUserCmd* cmd)
{
	if (!Settings::Misc::BunnyHop)
		return;

	static bool jumped_last_tick = false;
	static bool should_fake_jump = false;

	if (!jumped_last_tick && should_fake_jump)
	{
		should_fake_jump = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
		{
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			jumped_last_tick = false;
		}
	}
	else
	{
		jumped_last_tick = false;
		should_fake_jump = false;
	}
}

float get_delta(float hspeed, float maxspeed, float airaccelerate)
{
	auto term = (30.f - (airaccelerate * (float)maxspeed / 66.f)) / hspeed;

	if (term < 1.0f && term > -1.0f)
	{
		return acos(term);
	}

	return 0.f;
}

void AutoStrafer(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoStrafer)
		return;

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer->IsAlive())
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND)) {
		if (cmd->mousedx > 1 || cmd->mousedx < -1)
		{
			cmd->sidemove = cmd->mousedx < 0.f ? -400.f : 400.f;
		}
		else
		{
			cmd->forwardmove = 5850.f / g_LocalPlayer->m_vecVelocity().Length2D();
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -400.f : 400.f;
		}
	}
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

void AutoCowboy(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoCowboy)
		return;

	if (!g_LocalPlayer->IsAlive() || !g_LocalPlayer)
		return;

	C_BaseEntity* GroundEntity = g_LocalPlayer->m_hGroundEntity();
	Vector ownPos = g_LocalPlayer->m_vecOrigin();

	if (GroundEntity)
	{
		if (GroundEntity->IsPlayer())
		{
			C_BasePlayer* GroundPlayer = (C_BasePlayer*)(GroundEntity);
			float yaw = cmd->viewangles.yaw;
			Vector entPos = GroundPlayer->m_vecOrigin();
			Vector VecForward = ownPos - entPos;

			Vector translatedVelocity = Vector(
				(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
				(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
				VecForward.z
			);

			cmd->forwardmove = -translatedVelocity.x * 10.f;
			cmd->sidemove = translatedVelocity.y * 10.f;

			return;
		}
	}

	if (Settings::Misc::ReverseCowboy)
	{
		for (auto i = 1; i <= g_EngineClient->GetMaxClients(); ++i)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || !pl->IsPlayer() || pl == g_LocalPlayer)
				continue;

			Vector plPos = pl->m_vecOrigin();
			C_BaseEntity * GroundEntity = pl->m_hGroundEntity();

			if (GroundEntity)
			{
				if (GroundEntity->IsPlayer())
				{
					C_BasePlayer* GroundPlayer = (C_BasePlayer*)GroundEntity;

					if (GroundPlayer == g_LocalPlayer)
					{
						float yaw = cmd->viewangles.yaw;
						Vector entPos = pl->m_vecOrigin();
						Vector VecForward = ownPos - entPos;
						Vector translatedVelocity = Vector(
							(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
							(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
							VecForward.z
						);

						cmd->forwardmove = -translatedVelocity.x * 10.f;
						cmd->sidemove = translatedVelocity.y * 10.f;

						return;
					}
				}
			}
		}
	}
}

void BlockBot(CUserCmd* cmd)
{
	if (!Settings::Misc::BlockBot)
		return;

	constexpr auto degreesToRadians = [](float degrees) noexcept { return degrees * static_cast<float>(M_PI) / 180; };
	constexpr float doorRange{ 250.0f };
	Vector viewAngles{ cos(degreesToRadians(cmd->viewangles.yaw)) * cos(degreesToRadians(cmd->viewangles.pitch)) * doorRange,
					   cos(degreesToRadians(cmd->viewangles.yaw)) * sin(degreesToRadians(cmd->viewangles.pitch)) * doorRange,
					  -sin(degreesToRadians(cmd->viewangles.yaw)) * doorRange };

	Ray_t Ray;
	Ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + viewAngles);
	trace_t trace;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;

	g_EngineTrace->TraceRay(Ray, MASK_SHOT, &filter, &trace);

	if (!trace.hit_entity)
		return;

	C_BasePlayer* pl = (C_BasePlayer*)trace.hit_entity;

	if (!pl || !pl->IsPlayer() || !pl->IsAlive() || pl->IsDormant())
		return;

	Utils::ConsolePrint(pl->GetPlayerInfo().szName);

	QAngle angles = Math::CalcAngle(g_LocalPlayer->m_vecOrigin(), pl->m_vecOrigin());
	angles.yaw -= g_LocalPlayer->m_angEyeAngles().yaw;
	angles.Normalize();

	if (angles.yaw < 0.f)
		cmd->sidemove = 450.f;
	if (angles.yaw > 0.f)
		cmd->sidemove = -450.f;

	g_EngineClient->SetViewAngles(&angles);
}


// EdgeJumper, not perfect as it also jumps when you're not running towards where you want to jump off, prob trace a ray 
// and check if it hits the ground infront.
void EdgeJumper(CUserCmd* cmd)
{
	if (!Settings::Misc::EdgeJumper)
		return;

	float vel = g_LocalPlayer->m_vecVelocity().Length2D();

	if (vel > 135)
	{
		int flags = g_LocalPlayer->m_fFlags();

		if (flags & FL_PARTIALGROUND)
		{
			cmd->buttons |= IN_JUMP;
		}
	}
}