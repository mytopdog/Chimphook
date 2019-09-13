#include "Combat.hpp"

bool Settings::Misc::AutoPistol = false;

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

bool Settings::Misc::AutoReload = false;

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

bool Settings::Misc::AutoRevolver = false;

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
		else
		{
			RevolverDelay = 0;

			float flPostponeFireReadyTime = g_LocalPlayer->m_hActiveWeapon()->m_flPostponeFireReadyTime();

			if (flPostponeFireReadyTime > 0 && flPostponeFireReadyTime < g_GlobalVars->curtime)
				cmd->buttons &= ~IN_ATTACK;
		}
	}
}

bool Settings::Misc::SelfNade = false;

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

bool Settings::Misc::RadarHack = false;

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