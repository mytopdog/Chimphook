#include "Triggerbot.hpp"
#include "GetHitchance.hpp"

bool Settings::Misc::Triggerbot::Enabled = false;
int Settings::Misc::Triggerbot::Delay = 0;

void Triggerbot(CUserCmd* pCmd)
{
	static size_t tDelay = 0;

	if (!Settings::Misc::Triggerbot::Enabled || !Settings::Misc::Triggerbot::_enabled)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !g_LocalPlayer->m_hActiveWeapon() || g_LocalPlayer->m_hActiveWeapon()->IsKnife())
		return;

	Vector vStart, vEnd, vAngles;
	vStart = g_LocalPlayer->GetEyePos();

	ConVar* recoil_scale = g_CVar->FindVar("weapon_recoil_scale");

	QAngle angle = pCmd->viewangles;
	angle -= g_LocalPlayer->m_aimPunchAngle() * recoil_scale->GetFloat();

	Math::AngleVectors(angle, vAngles);

	vAngles *= g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->flRange;
	vEnd = vStart + vAngles;
	Ray_t ray;
	ray.Init(vStart, vEnd);
	trace_t tr;
	CTraceFilter pTraceFilter;
	pTraceFilter.pSkip = g_LocalPlayer;

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &pTraceFilter, &tr);

	C_BaseEntity* ent = (C_BaseEntity*)tr.hit_entity;

	if (!ent || !ent->IsPlayer())
		return;

	C_BasePlayer* pl = (C_BasePlayer*)tr.hit_entity;

	if (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return;

	if (!pl->IsAlive())
		return;

	if (!CompareHitchance(pCmd, pl, pl->GetHitboxPos(tr.hitgroup)))
		return;

	if (Settings::Misc::Triggerbot::Delay > 0)
	{
		if (GetTickCount() > tDelay)
		{
			if ((tr.hitgroup <= 7 && tr.hitgroup > 0))
				if (!g_LocalPlayer->m_hActiveWeapon()->IsAutomatic())
					if (g_LocalPlayer->m_hActiveWeapon()->CanFire())
						pCmd->buttons |= IN_ATTACK;
					else
						pCmd->buttons &= ~IN_ATTACK;
				else
					pCmd->buttons |= IN_ATTACK;

			tDelay = GetTickCount() + Settings::Misc::Triggerbot::Delay * 35;
		}
	}
	else
	{
		if ((tr.hitgroup <= 7 && tr.hitgroup > 0))
			if (!g_LocalPlayer->m_hActiveWeapon()->IsAutomatic())
				if (g_LocalPlayer->m_hActiveWeapon()->CanFire())
					pCmd->buttons |= IN_ATTACK;
				else
					pCmd->buttons &= ~IN_ATTACK;
			else
				pCmd->buttons |= IN_ATTACK;
	}
}