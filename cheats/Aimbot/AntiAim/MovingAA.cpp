#include "MovingAA.hpp"

int Settings::AntiAim::Moving::MinVelocity = 0;

int Settings::AntiAim::Moving::Pitch::Type = 0;

int Settings::AntiAim::Moving::Yaw::Type = 0;
int Settings::AntiAim::Moving::Yaw::Manual::LKey = 0;
int Settings::AntiAim::Moving::Yaw::Manual::RKey = 0;
int Settings::AntiAim::Moving::Yaw::Manual::BKey = 0;
int Settings::AntiAim::Moving::Yaw::Spin::Speed = 0;
int Settings::AntiAim::Moving::Yaw::Custom::Angle = 0;
bool Settings::AntiAim::Moving::Yaw::Custom::Static = false;

int Settings::AntiAim::Moving::YawAdd::Type = 0;
int Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange = 0;
bool Settings::AntiAim::Moving::YawAdd::Jitter::Switch = false;
int Settings::AntiAim::Moving::YawAdd::Custom::Angle = 0;

void DoMovingAA(CUserCmd* cmd, bool& bSendPacket)
{
	switch (Settings::AntiAim::Moving::Pitch::Type)
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

	switch (Settings::AntiAim::Moving::Yaw::Type)
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
		if (GetAsyncKeyState(Settings::AntiAim::Moving::Yaw::Manual::LKey))
			lastmanual = 0;
		else if (GetAsyncKeyState(Settings::AntiAim::Moving::Yaw::Manual::RKey))
			lastmanual = 1;
		else if (GetAsyncKeyState(Settings::AntiAim::Moving::Yaw::Manual::BKey))
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

		{
			QAngle LAng = curang + QAngle(0, 45, 0);
			Vector LVec;
			Math::AngleVectors(LAng, LVec);

			Ray_t ray;
			ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + LVec * 8192);
			CTraceFilterWorldAndPropsOnly filter;
			trace_t tr;

			g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

			LDist = tr.startpos.DistTo(tr.endpos);
		}

		{
			QAngle RAng = curang + QAngle(0, 45, 0);
			Vector RVec;
			Math::AngleVectors(RAng, RVec);

			Ray_t ray;
			ray.Init(g_LocalPlayer->GetEyePos(), g_LocalPlayer->GetEyePos() + RVec * 8192);
			CTraceFilterWorldAndPropsOnly filter;
			trace_t tr;

			g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

			RDist = tr.startpos.DistTo(tr.endpos);
		}

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
		spinang += Settings::AntiAim::Moving::Yaw::Spin::Speed;
		while (spinang > 360)
			spinang -= 360;

		while (spinang < 360)
			spinang += 360;

		cmd->viewangles.yaw = spinang;
		break;
	case 8:
		if (Settings::AntiAim::Moving::Yaw::Custom::Static)
			cmd->viewangles.yaw = Settings::AntiAim::Moving::Yaw::Custom::Angle;
		else
			cmd->viewangles.yaw += Settings::AntiAim::Moving::Yaw::Custom::Angle;

		break;
	}

	if (!Settings::AntiAim::Moving::Yaw::Type)
		return;

	switch (Settings::AntiAim::Moving::YawAdd::Type)
	{
	case 0:
		break;
	case 1:
		cmd->viewangles.yaw += 180.f;
		break;
	case 2:
		if (Settings::AntiAim::Moving::YawAdd::Jitter::Switch)
			cmd->viewangles.yaw += cmd->command_number % 2 ? Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange : -Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange;
		else
			if (Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange)
				cmd->viewangles.yaw += (rand() % (Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange * 2)) - Settings::AntiAim::Moving::YawAdd::Jitter::AngleRange;

		break;
	}
}