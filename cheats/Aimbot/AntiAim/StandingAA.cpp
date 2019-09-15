#include "StandingAA.hpp"

int Settings::AntiAim::Standing::Pitch::Type = 0;

int Settings::AntiAim::Standing::Yaw::Type = 0;
int Settings::AntiAim::Standing::Yaw::Manual::LKey = 0;
int Settings::AntiAim::Standing::Yaw::Manual::RKey = 0;
int Settings::AntiAim::Standing::Yaw::Manual::BKey = 0;
int Settings::AntiAim::Standing::Yaw::Spin::Speed = 0;
int Settings::AntiAim::Standing::Yaw::Custom::Angle = 0;
bool Settings::AntiAim::Standing::Yaw::Custom::Static = false;

int Settings::AntiAim::Standing::YawAdd::Type = 0;
int Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange = 0;
bool Settings::AntiAim::Standing::YawAdd::Jitter::Switch = false;
int Settings::AntiAim::Standing::YawAdd::Custom::Angle = 0;

void DoStandingAA(CUserCmd* cmd, bool& bSendPacket)
{
	switch (Settings::AntiAim::Standing::Pitch::Type)
	{
	case 0:
		break;
	case 1:
		cmd->viewangles.pitch = 89.9f;
		break;
	case 2:
		cmd->viewangles.pitch = -89.f;
		break;
	case 3:
		cmd->viewangles.pitch = 0.f;
		break;
	case 4:
		cmd->viewangles.pitch = cmd->command_number % 2 ? 89.9f : -89.9f;
		break;
	case 5:
		cmd->viewangles.pitch = (float)(rand() % 180) - 89.f;
		break;
	}

	static int lastmanual = 0;
	static int spinang = 0;

	switch (Settings::AntiAim::Standing::Yaw::Type)
	{
	case 0:
		break;
	case 1:
		cmd->viewangles.yaw += 90.f;
		break;
	case 2:
		cmd->viewangles.yaw -= 90.f;
		break;
	case 3:
		cmd->viewangles.yaw += 180.f;
		break;
	case 4:
		if (GetAsyncKeyState(Settings::AntiAim::Standing::Yaw::Manual::LKey))
			lastmanual = 0;
		else if (GetAsyncKeyState(Settings::AntiAim::Standing::Yaw::Manual::RKey))
			lastmanual = 1;
		else if (GetAsyncKeyState(Settings::AntiAim::Standing::Yaw::Manual::BKey))
			lastmanual = 2;

		switch (lastmanual)
		{
		case 0:
			cmd->viewangles.yaw += 90.f;
			break;
		case 1:
			cmd->viewangles.yaw -= 90.f;
			break;
		case 2:
			cmd->viewangles.yaw += 180.f;
			break;
		}

		break;
	case 5:
	{
		QAngle curang = cmd->viewangles;
		float LDist;
		float RDist;

		QAngle LAng;
		g_EngineClient->GetViewAngles(&LAng);

		Vector L45;
		LAng.yaw -= 45.f;
		Math::AngleVectors(LAng, L45);

		Ray_t l_ray;
		l_ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + L45 * 512);
		CTraceFilter l_filter;
		l_filter.pSkip = g_LocalPlayer;
		trace_t l_tr;

		g_EngineTrace->TraceRay(l_ray, MASK_SHOT, &l_filter, &l_tr);

		LDist = l_tr.startpos.DistTo(l_tr.endpos);

		QAngle RAng;
		g_EngineClient->GetViewAngles(&RAng);

		Vector R45;
		RAng.yaw -= 45.f;
		Math::AngleVectors(RAng, R45);

		Ray_t r_ray;
		r_ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + R45 * 512);
		CTraceFilter r_filter;
		r_filter.pSkip = g_LocalPlayer;
		trace_t r_tr;

		g_EngineTrace->TraceRay(r_ray, MASK_SHOT, &r_filter, &r_tr);

		RDist = r_tr.startpos.DistTo(r_tr.endpos);

		if (RDist < LDist)
			cmd->viewangles.yaw -= 90.f;
		else
			cmd->viewangles.yaw += 90.f;

		break;
	}
	case 6:
		cmd->viewangles.yaw += (float)(rand() % 360) - 180.f;
		break;
	case 7:
		spinang += Settings::AntiAim::Standing::Yaw::Spin::Speed;
		while (spinang > 360)
			spinang -= 360;

		while (spinang < 360)
			spinang += 360;

		cmd->viewangles.yaw = spinang;
		break;
	case 8:
		if (Settings::AntiAim::Standing::Yaw::Custom::Static)
			cmd->viewangles.yaw = Settings::AntiAim::Standing::Yaw::Custom::Angle;
		else 
			cmd->viewangles.yaw += Settings::AntiAim::Standing::Yaw::Custom::Angle;

		break;
	}

	if (!Settings::AntiAim::Standing::Yaw::Type)
		return;

	switch (Settings::AntiAim::Standing::YawAdd::Type)
	{
	case 0:
		break;
	case 1:
		cmd->viewangles.yaw += 180.f;
		break;
	case 2:
		if (Settings::AntiAim::Standing::YawAdd::Jitter::Switch)
			cmd->viewangles.yaw += cmd->command_number % 2 ? Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange : -Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange;
		else
			if (Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange)
				cmd->viewangles.yaw += (rand() % (Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange * 2)) - Settings::AntiAim::Standing::YawAdd::Jitter::AngleRange;

		break;
	}
}