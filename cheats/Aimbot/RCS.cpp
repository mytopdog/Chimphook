#include "RCS.hpp"

#include "../Misc/Visual.hpp"

bool Settings::Aim::RCS::Enabled = false;
bool Settings::Aim::RCS::Silent = false;
float Settings::Aim::RCS::ScaleX = 2;
float Settings::Aim::RCS::ScaleY = 2;

void RCS(CUserCmd* cmd)
{
	if (!Settings::Aim::RCS::Enabled)
		return;

	if (!g_LocalPlayer->IsAlive() || !g_LocalPlayer->m_hActiveWeapon())
		return;

	QAngle CurrentPunch = g_LocalPlayer->m_aimPunchAngle();

	ConVar* recoil_scale = g_CVar->FindVar("weapon_recoil_scale");

	if (g_LocalPlayer->m_iShotsFired() > 1)
	{
		if (!Settings::Aim::RCS::Silent)
		{
			QAngle NewPunch = { CurrentPunch.pitch - RCSLastPunch.pitch, CurrentPunch.yaw - RCSLastPunch.yaw, 0 };

			if (Settings::Misc::FreeLook::_enabled && Settings::Misc::FreeLook::Enabled && !LastState::Get().virtualViewangles.IsZero() && !g_Input->m_fCameraInThirdPerson)
			{
				LastState::Get().viewangles.yaw -= NewPunch.yaw * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleX / 2);
				LastState::Get().viewangles.pitch -= NewPunch.pitch * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleY / 2);

				LastState::Get().virtualViewangles.yaw -= NewPunch.yaw * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleX / 2);
				LastState::Get().virtualViewangles.pitch -= NewPunch.pitch * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleY / 2);
			}
			else
			{
				cmd->viewangles.yaw -= NewPunch.yaw * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleX / 2);
				cmd->viewangles.pitch -= NewPunch.pitch * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleY / 2);

				g_EngineClient->SetViewAngles(&cmd->viewangles);
			}
		}
		else
		{
			QAngle CurrentPunch = g_LocalPlayer->m_aimPunchAngle();

			cmd->viewangles.yaw -= CurrentPunch.yaw * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleX / 2);
			cmd->viewangles.pitch -= CurrentPunch.pitch * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleY / 2);
		}
	}

	RCSLastPunch = CurrentPunch;
}