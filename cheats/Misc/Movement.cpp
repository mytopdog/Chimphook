#include "Movement.hpp"


bool Settings::Misc::AutoCowboy = false;
bool Settings::Misc::ReverseCowboy = false;

void AutoCowboy(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoCowboy)
		return;

	if (!g_LocalPlayer->IsAlive() || !g_LocalPlayer)
		return;

	if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		return;

	C_BaseEntity* GroundEntity = g_LocalPlayer->m_hGroundEntity();

	Vector ownPos = g_LocalPlayer->m_vecOrigin();

	if (GroundEntity)
	{
		if (GroundEntity->IsPlayer())
		{
			C_BasePlayer* GroundPlayer = (C_BasePlayer*)(GroundEntity);
			Vector entPos;

			entPos = GroundPlayer->m_vecOrigin();

			float yaw = cmd->viewangles.yaw;
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
			C_BaseEntity* GroundEntity = pl->m_hGroundEntity();

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

bool Settings::Misc::AutoStrafer = false;

void AutoStrafer(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoStrafer)
		return;

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer->IsAlive() || g_LocalPlayer->m_nMoveType() & MOVETYPE_NOCLIP)
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
	{
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

bool Settings::Misc::BlockBot = false;

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

bool Settings::Misc::BunnyHop = false;

void Bunnyhop(CUserCmd* cmd)
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

bool Settings::Misc::MoonWalk = false;

void MoonWalk(CUserCmd* cmd)
{
	if (Settings::Misc::MoonWalk)
	{
		g_EngineClient->ExecuteClientCmd("bind w +back; bind s +forward; bind a +moveright; bind d +moveleft");

		if (cmd->buttons & IN_BACK)
			cmd->forwardmove = 450;

		if (cmd->buttons & IN_FORWARD)
			cmd->forwardmove = -450;

		if (cmd->buttons & IN_MOVELEFT)
			cmd->sidemove = 450;

		if (cmd->buttons & IN_MOVERIGHT)
			cmd->sidemove = -450;
	}
	else
	{
		g_EngineClient->ExecuteClientCmd("bind w +forward; bind s +back; bind a +moveleft; bind d +moveright");
	}
}

bool Settings::Misc::SlowWalk::Enabled = false;
int Settings::Misc::SlowWalk::Amount = 100;

void SlowWalk(CUserCmd* cmd)
{
	if (!Settings::Misc::SlowWalk::Enabled || !GetAsyncKeyState(VK_SHIFT))
		return;

	C_BaseCombatWeapon* weapon_handle = g_LocalPlayer->m_hActiveWeapon().Get();

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

bool Settings::Misc::InfiniteDuck = false;

void InfiniteDuck(CUserCmd* cmd)
{
	if (!Settings::Misc::InfiniteDuck)
		return;

	cmd->buttons |= IN_BULLRUSH;
}

bool Settings::Misc::FakeLag::Enabled = false;
int Settings::Misc::FakeLag::Choke = 0;

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

bool Settings::Misc::FakeDuck::Enabled = false;

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

bool Settings::Misc::EdgeJumper = false;