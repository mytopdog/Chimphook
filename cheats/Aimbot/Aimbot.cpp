#include "Aimbot.hpp"
#include "AimbotSettings.hpp"
#include "GetHitchance.hpp"
#include "Autowall.hpp"
#include "AntiAim.hpp"
#include "RCS.hpp"

std::unordered_map<Hitbox, std::vector<const char*>, IntHash<Hitbox>> hitboxes = {
		{ Hitbox::HITBOX_HEAD, { "head_0" } },
		{ Hitbox::HITBOX_NECK, { "neck_0" } },
		{ Hitbox::HITBOX_PELVIS, { "pelvis" } },
		{ Hitbox::HITBOX_SPINE, { "spine_0", "spine_1", "spine_2", "spine_3" } },
		{ Hitbox::HITBOX_LEGS, { "leg_upper_L", "leg_upper_R", "leg_lower_L", "leg_lower_R", "ankle_L", "ankle_R" } },
		{ Hitbox::HITBOX_ARMS, { "hand_L", "hand_R", "arm_upper_L", "arm_lower_L", "arm_upper_R", "arm_lower_R" } },
};

static const char* targets[] = { "pelvis", "", "", "spine_0", "spine_1", "spine_2", "spine_3", "neck_0", "head_0" };


template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;

}

bool DoHitscan(C_BasePlayer* entity, float& bestDamage, Bone& bestBone, C_BasePlayer* plpers = g_LocalPlayer, bool doPrio = true)
{
	bool found = false;
	C_BaseCombatWeapon* wep = plpers->m_hActiveWeapon();
	bool getPrio = doPrio && GetWeaponPrioritiseHitbox(wep);
	Hitbox prio = GetWeaponPrioritisedHitbox(wep);

	for (std::unordered_map<Hitbox, std::vector<const char*>, Utils::IntHash<Hitbox>>::iterator it = hitboxes.begin(); it != hitboxes.end(); it++)
	{
		std::vector<const char*> hitboxList = hitboxes[it->first];
		for (std::vector<const char*>::iterator it2 = hitboxList.begin(); it2 != hitboxList.end(); it2++)
		{
			Bone bone = C_BasePlayer::GetBoneByName(entity, *it2);
			Vector vecBone = entity->GetBonePos((int)bone);

			Autowall::FireBulletData data;
			float damage = Autowall::GetDamage(plpers, vecBone, false, data) / 2;

			if (getPrio && it->first == prio && damage > GetWeaponMinDamage(wep))
			{
				bestDamage = damage;
				bestBone = bone;
				return true;
			}

			if (damage > bestDamage && damage > GetWeaponMinDamage(wep))
			{
				bestDamage = damage;
				bestBone = bone;
				found = true;
			}
		}
	}

	return found;
}

C_BasePlayer* FindPlayer(bool& found, float& bestDamage, Bone& bestBone)
{
	C_BasePlayer* player = nullptr;
	found = false;

	float maxFOV = 999;
	float maxDist = 999999;

	switch (Settings::Aim::TestAimbot::TargetPriority)
	{
	case 0:
		for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || pl->m_bGunGameImmunity() || pl->IsDormant() || !pl->IsPlayer() || !pl->notfriend || (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !Settings::Aim::TestAimbot::TargetTeammates))
				continue;

			Bone bone;
			float dmg = 0;

			if (DoHitscan(pl, dmg, bone))
			{
				QAngle vang;
				g_EngineClient->GetViewAngles(&vang);

				QAngle angtoplayer = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pl->GetBonePos((int)bone));
				float FOV = Math::GetFOV(vang, angtoplayer);

				if (FOV < maxFOV)
				{
					player = pl;
					bestBone = bone;
					bestDamage = dmg;
					maxFOV = FOV;
					found = true;
				}
			}
		}

		break;
	case 1:
		for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || pl->m_bGunGameImmunity() || pl->IsDormant() || !pl->IsPlayer() || !pl->notfriend || (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !Settings::Aim::TestAimbot::TargetTeammates))
				continue;

			Bone bone;
			float dmg = 0;

			if (DoHitscan(pl, dmg, bone))
			{
				if (dmg > bestDamage)
				{
					player = pl;
					bestBone = bone;
					bestDamage = dmg;
					found = true;
				}
			}
		}

		break;
	case 2:
		for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || pl->m_bGunGameImmunity() || pl->IsDormant() || !pl->IsPlayer() || !pl->notfriend || (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !Settings::Aim::TestAimbot::TargetTeammates))
				continue;

			Bone bone;
			float dmg = 0;

			if (DoHitscan(pl, dmg, bone))
			{
				Vector bonepos = pl->GetBonePos((int)bone);
				float dist = g_LocalPlayer->GetEyePos().DistTo(bonepos);

				if (dist < maxDist)
				{
					player = pl;
					bestBone = bone;
					bestDamage = dmg;
					maxDist = dist;
					found = true;
				}
			}
		}

		break;
	case 3:
		int maxThreat = 100;
		C_BasePlayer* bestPlayer = nullptr;
		float mostDmg = 0;
		bool foundg = false;

		for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || pl->m_bGunGameImmunity() || pl->IsDormant() || !pl->IsPlayer() || !pl->notfriend || (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !Settings::Aim::TestAimbot::TargetTeammates))
				continue;

			Bone bone;
			float dmg = 0;
			int threatLvl = 0;

			if (DoHitscan(g_LocalPlayer, dmg, bone, pl, false))
			{
				if (dmg > mostDmg)
				{
					bestPlayer = pl;
					mostDmg = dmg;
					foundg = true;
				}
			}
		}

		for (auto i = 0; i < g_EngineClient->GetMaxClients(); i++)
		{
			C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

			if (!pl || !pl->IsAlive() || pl->m_bGunGameImmunity() || pl->IsDormant() || !pl->IsPlayer() || !pl->notfriend || (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !Settings::Aim::TestAimbot::TargetTeammates))
				continue;

			Bone bone;
			float dmg = 0;
			int threatLvl = 0;

			if (DoHitscan(pl, dmg, bone))
			{
				if (pl->CanSeePlayer(g_LocalPlayer, g_LocalPlayer->GetHitboxPos((int)(HITBOX_HEAD))))
				{
					threatLvl++;
				}
				if (pl->m_iHealth() > g_LocalPlayer->m_iHealth())
				{
					threatLvl++;
				}
				if (pl->m_iShotsFired() > 0)
				{
					threatLvl++;
				}
				if (pl->m_bHasHelmet())
				{
					threatLvl++;
				}
				if (foundg && pl == bestPlayer)
				{
					threatLvl += 2;
				}
				if (pl->m_hActiveWeapon()->GetCSWeaponData()->iDamage > g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->iDamage)
				{
					threatLvl++;
				}
				if (threatLvl > maxThreat)
				{
					player = pl;
					bestBone = bone;
					bestDamage = dmg;
					maxThreat = threatLvl;
					found = true;
				}
			}
		}

		break;
	}


	return player;
}

bool IsViable(CUserCmd* cmd, C_BasePlayer* target, bool found, float dmg, Bone bone)
{

	if (!found)
		return false;

	if (dmg < GetWeaponMinDamage(g_LocalPlayer->m_hActiveWeapon()))
		return false;

	if (!g_LocalPlayer->m_hActiveWeapon()->CanFire())
		return false;

	return CompareHitchance(cmd, target, target->GetBonePos((int)bone));
}

bool CanTrigger(CUserCmd* cmd, QAngle viewAngles)
{
	Vector traceStart, traceEnd;
	trace_t tr;

	QAngle viewAngles_rcs = viewAngles + g_LocalPlayer->m_aimPunchAngle() * 2.0f;

	Math::AngleVectors(viewAngles_rcs, traceEnd);

	traceStart = g_LocalPlayer->GetEyePos();
	traceEnd = traceStart + (traceEnd * 8192.0f);

	Ray_t ray;
	ray.Init(traceStart, traceEnd);
	CTraceFilter traceFilter;
	traceFilter.pSkip = g_LocalPlayer;
	g_EngineTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);

	C_BasePlayer* player = (C_BasePlayer*)tr.hit_entity;
	if (!player)
		return false;

	if (player->GetClientClass()->m_ClassID != ClassId_CCSPlayer)
		return false;

	if (player == g_LocalPlayer
		|| player->IsDormant()
		|| !player->IsAlive()
		|| player->m_bGunGameImmunity())
		return false;

	if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return false;

	C_BaseCombatWeapon* activeWeapon = g_LocalPlayer->m_hActiveWeapon();
	CCSWeaponInfo* info = activeWeapon->GetCSWeaponData();

	if (!IsFiring(cmd))
		return false;

	if (activeWeapon->IsKnife())
		return false;

	if (activeWeapon->IsGrenade())
		return false;

	return true;
}

void DoAim(CUserCmd* cmd, bool& bSendPacket)
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !g_EngineClient->IsInGame() || !g_LocalPlayer->m_hActiveWeapon())
	{
		AimbotTarget = nullptr;
		return;
	}

	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	QAngle angle;
	g_EngineClient->GetViewAngles(&angle);

	ConVar* recoil_scale = g_CVar->FindVar("weapon_recoil_scale");

	if (cmd->buttons & IN_USE)
	{
		AimbotTarget = nullptr;
		return;
	}

	if (Settings::AntiAim::Enabled)
	{
		if (!Settings::Aim::TestAimbot::Enabled || !Settings::Aim::TestAimbot::_enabled)
		{
			if (!IsFiring(cmd))
			{
				AimbotTarget = nullptr;
				return AntiAim(cmd, bSendPacket);
			}
		}

		if (!Settings::Aim::TestAimbot::AutoShoot)
		{
			if (!IsFiring(cmd))
			{
				AimbotTarget = nullptr;
				return AntiAim(cmd, bSendPacket);
			}
		}
	}

	if (Settings::Aim::TestAimbot::Enabled && Settings::Aim::TestAimbot::_enabled)
	{

		bool found = false;
		float dmg = 0;
		Bone bone;
		C_BasePlayer* pl = FindPlayer(found, dmg, bone);

		C_BaseCombatWeapon* activeWeapon = g_LocalPlayer->m_hActiveWeapon();

		if (found)
		{
			if (GetWeaponAutoScope(activeWeapon) && (activeWeapon->IsSniper() || activeWeapon->IsAuto()) && !g_LocalPlayer->m_bIsScoped())
			{
				cmd->buttons |= IN_ATTACK2;
			}

			if (GetWeaponAutoCrouch(activeWeapon))
			{
				cmd->buttons |= IN_DUCK;
			}

			if (GetWeaponAutoStop(activeWeapon))
			{
				oldForward = 0;
				oldSideMove = 0;
				cmd->forwardmove = 0;
				cmd->sidemove = 0;
			}
		}

		if (found && IsViable(cmd, pl, found, dmg, bone))
		{
			angle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pl->GetBonePos((int)bone));
			QAngle CurrentPunch = g_LocalPlayer->m_aimPunchAngle();
			QAngle NewPunch = { CurrentPunch.pitch - RCSLastPunch.pitch, CurrentPunch.yaw - RCSLastPunch.yaw, 0 };

			angle -= CurrentPunch * recoil_scale->GetFloat();

			RCSLastPunch = CurrentPunch;

			cmd->viewangles = angle;
			AimbotTarget = pl;

			if (!Settings::Aim::TestAimbot::Silent)
			{
				g_EngineClient->SetViewAngles(&cmd->viewangles);
			}
			else
			{
				if (Settings::Aim::TestAimbot::WeaponMove && !g_Input->m_fCameraInThirdPerson)
				{
					aimAngle = angle;
					oaimAngle = angle;
				}

				Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
			}

			if (Settings::Aim::TestAimbot::AutoShoot)
			{
				if (activeWeapon->CanFire())
				{
					cmd->buttons |= IN_ATTACK;
				}
				else
				{
					cmd->buttons &= ~IN_ATTACK;
				}
			}
		}
		else
		{
			if (Settings::AntiAim::Enabled)
			{
				if (!IsFiring(cmd))
				{
					AimbotTarget = nullptr;
					return AntiAim(cmd, bSendPacket);
				}
			}

			AimbotTarget = nullptr;
			RCS(cmd);
		}
	}
	else
	{
		if (Settings::AntiAim::Enabled)
		{
			if (!IsFiring(cmd))
			{
				AimbotTarget = nullptr;
				return AntiAim(cmd, bSendPacket);
			}
		}

		AimbotTarget = nullptr;
		RCS(cmd);
	}
}