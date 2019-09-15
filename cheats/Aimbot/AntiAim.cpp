#include "AntiAim.hpp"

#include "AntiAim/AirAA.hpp"
#include "AntiAim/MovingAA.hpp"
#include "AntiAim/StandingAA.hpp"

bool Settings::AntiAim::Enabled = false;

bool IsFiring(CUserCmd* cmd)
{
	float flServerTime = (float)(g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick);

	C_BaseCombatWeapon* wep = g_LocalPlayer->m_hActiveWeapon();

	if (!wep)
		return false;

	bool IsShooting = cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2;

	if ((!wep->IsKnife() && !wep->IsGrenade()) && (wep->IsReloading() || g_LocalPlayer->m_hActiveWeapon()->m_iClip1() <= 0 || !g_LocalPlayer))
		return false;

	if (wep->IsGrenade())
	{
		float fThrowTime = wep->m_fThrowTime();

		if (fThrowTime > 0.f)
			return true;
	}

	if (g_LocalPlayer->m_flNextAttack() - g_GlobalVars->curtime <= 0.05f)
	{
		if (cmd->buttons & IN_ATTACK)
			return (wep->m_flNextPrimaryAttack() - g_GlobalVars->curtime <= 0.05f);
		else if (cmd->buttons & IN_ATTACK2)
			return (wep->m_flNextSecondaryAttack() - g_GlobalVars->curtime <= 0.05f);

		else return false;
	}

	return false;
}


void AntiAim(CUserCmd* cmd, bool& bSendPacket)
{
	if (!g_EngineClient->IsInGame() || !g_LocalPlayer->IsAlive())
		return;

	if (cmd->buttons & IN_USE)
		return;

	if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP)
		return;

	QAngle ang = cmd->viewangles;

	QAngle oldAng = cmd->viewangles;
	float oldFwd = cmd->forwardmove;
	float oldSde = cmd->sidemove;

	if (Settings::AntiAim::Enabled) {
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
		{
			if (g_LocalPlayer->m_vecVelocity().Length2D() > Settings::AntiAim::Moving::MinVelocity)
			{
				DoMovingAA(cmd, bSendPacket);
			}
			else
			{
				DoStandingAA(cmd, bSendPacket);
			}
		}
		else
		{
			DoAirAA(cmd, bSendPacket);
		}
	}

	Math::CorrectMovement(oldAng, cmd, oldFwd, oldSde);
}