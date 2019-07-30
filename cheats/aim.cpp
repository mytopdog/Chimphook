#include "aim.hpp"
#include "autowall.hpp"
#include "aimsett.h"
#include "misc.hpp"

#define M_PI   3.14159265358979323846
#define XM_2PI 6.283185307f

C_BasePlayer* AimbotTarget = nullptr;

bool Settings::Aim::RCS::Enabled = false;
bool Settings::Aim::RCS::Silent = false;
float Settings::Aim::RCS::ScaleX = 2;
float Settings::Aim::RCS::ScaleY = 2;

static QAngle RCSLastPunch;

inline void RandomSeed(int seed)
{
	static auto fn = (decltype(&RandomSeed))(GetProcAddress(GetModuleHandle(L"vstdlib.dll"), "RandomSeed"));

	return fn(seed);
}

inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle(L"vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

void RCS(CUserCmd* cmd)
{
	if (!Settings::Aim::RCS::Enabled)
		return;

	if (!g_LocalPlayer->IsAlive() || !g_LocalPlayer->m_hActiveWeapon())
		return;

	//if (!(cmd->buttons & IN_ATTACK && g_LocalPlayer->m_iShotsFired() > 0))
	//	return;

	QAngle CurrentPunch = g_LocalPlayer->m_aimPunchAngle();

	ConVar * recoil_scale = g_CVar->FindVar("weapon_recoil_scale");

	Vector vecForward, vecRight, vecUp, vecAntiDir;
	float flSpread, flInaccuracy, flSpreadX, flSpreadY;
	QAngle qAntiSpread;

	flSpread = g_LocalPlayer->m_hActiveWeapon()->GetSpread();
	flInaccuracy = g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy();

	RandomSeed((cmd->random_seed & 0xFF) + 1);

	float flRandPi_1 = RandomFloat(0.0f, 2.0f * M_PI);
	float flRandInaccuracy = RandomFloat(0.0f, flInaccuracy);
	float flRandPi_2 = RandomFloat(0.0f, 2.0f * M_PI);
	float flRandSpread = RandomFloat(0, flSpread);

	flSpreadX = cos(flRandPi_1) * flRandInaccuracy + cos(flRandPi_2) * flRandSpread;
	flSpreadY = sin(flRandPi_1) * flRandInaccuracy + sin(flRandPi_2) * flRandSpread;

	Math::AngleVectors(cmd->viewangles, vecForward, vecRight, vecUp);

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

			/*	Vector dir;
				Math::AngleVectors(cmd->viewangles, dir);

				vecAntiDir = dir + (vecRight * -flSpreadX) + (vecUp * -flSpreadY);
				vecAntiDir.NormalizeInPlace();

				Math::VectorAngles(vecAntiDir, qAntiSpread);

				cmd->viewangles = qAntiSpread;*/

				g_EngineClient->SetViewAngles(&cmd->viewangles);
			}
		}
		else
		{
			QAngle CurrentPunch = g_LocalPlayer->m_aimPunchAngle();

			cmd->viewangles.yaw -= CurrentPunch.yaw * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleX / 2);
			cmd->viewangles.pitch -= CurrentPunch.pitch * recoil_scale->GetFloat() * (Settings::Aim::RCS::ScaleY / 2);

		/*	Vector dir;
			Math::AngleVectors(cmd->viewangles, dir);

			vecAntiDir = dir + (vecRight * -flSpreadX) + (vecUp * -flSpreadY);
			vecAntiDir.NormalizeInPlace();

			Math::VectorAngles(vecAntiDir, qAntiSpread);

			cmd->viewangles = qAntiSpread;*/

		}
	}

	RCSLastPunch = CurrentPunch;

	RCSLastPunch = CurrentPunch;
}


int Settings::AntiAim::Pitch::Type = 0;

int Settings::AntiAim::Yaw::Type = 0;
int Settings::AntiAim::Yaw::Angle = 0;
int Settings::AntiAim::Yaw::Jitter = 0;

bool Settings::AntiAim::Yaw::__lkey = false;
int Settings::AntiAim::Yaw::LKey = 0;
int Settings::AntiAim::Yaw::RKey = 0;

bool CanFire(CUserCmd* cmd)
{
	float flServerTime = (float)(g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick);

	if (!g_LocalPlayer->m_hActiveWeapon())
		return false;

	bool IsShooting = cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2;

	if (g_LocalPlayer->m_hActiveWeapon()->IsGrenade() && IsShooting)
		return true;

	if (g_LocalPlayer->m_hActiveWeapon()->IsReloading() || g_LocalPlayer->m_hActiveWeapon()->m_iClip1() <= 0 || !g_LocalPlayer)
		return false;

	if (g_LocalPlayer->m_hActiveWeapon()->IsKnife() && IsShooting)
		return true;

	if (g_LocalPlayer->m_flNextAttack() < flServerTime)
	{
		if (cmd->buttons & IN_ATTACK)
			return (g_LocalPlayer->m_hActiveWeapon()->m_flNextPrimaryAttack() < flServerTime);
		else if (cmd->buttons & IN_ATTACK2)
			return (g_LocalPlayer->m_hActiveWeapon()->m_flNextSecondaryAttack() < flServerTime);
		else return false;
	}

	return false;
}

void AntiAim(CUserCmd* cmd, bool bSendPacket)
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

	static bool switch_ = false;
	static bool bSend = true;

	switch (Settings::AntiAim::Pitch::Type)
	{
	case 0:
		break;
	case 1:
		ang.pitch = 89.f;
		break;
	case 2:
		ang.pitch = -89.f;
		break;
	case 3:
		ang.pitch = (cmd->command_number % 2 == 0 ? -89.f : 89.f);
		break;
	}
	switch (Settings::AntiAim::Yaw::Type)
	{
	case 0:
		break;
	case 1:
		ang.yaw += 180.f;
		break;
	case 2:
		if (Settings::AntiAim::Yaw::__lkey)
			ang.yaw += 90.f;
		else
			ang.yaw += -90.f;
		break;
	case 3:
		bSend = !bSend;

		if (!bSend)
			ang.yaw += g_LocalPlayer->get_max_desync_delta();
		else
			ang.yaw += 0.f;

		bSendPacket = bSend;

		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND && cmd->sidemove < 3 && cmd->sidemove > -3)
		{
			if (switch_)
				cmd->sidemove = 2;
			else
				cmd->sidemove = -2;

			switch_ = !switch_;
		}

		if (bSend)
			CreateMove::RealAngles = ang;
		else
			CreateMove::FakeAngles = ang;
		break;
	case 4:
		ang.yaw += Settings::AntiAim::Yaw::Angle;
		break;
	}

	if (Settings::AntiAim::Yaw::Type > 0)
		ang.yaw += (cmd->command_number % 2 == 0 ? Settings::AntiAim::Yaw::Jitter : -Settings::AntiAim::Yaw::Jitter);

	cmd->viewangles = ang;

	cmd->viewangles.Normalize();

	Math::CorrectMovement(oldAng, cmd, oldFwd, oldSde);
}

std::unordered_map<Hitbox, std::vector<const char*>, IntHash<Hitbox>> hitboxes = {
		{ Hitbox::HITBOX_HEAD, { "head_0" } },
		{ Hitbox::HITBOX_NECK, { "neck_0" } },
		{ Hitbox::HITBOX_PELVIS, { "pelvis" } },
		{ Hitbox::HITBOX_SPINE, { "spine_0", "spine_1", "spine_2", "spine_3" } },
		{ Hitbox::HITBOX_LEGS, { "leg_upper_L", "leg_upper_R", "leg_lower_L", "leg_lower_R", "ankle_L", "ankle_R" } },
		{ Hitbox::HITBOX_ARMS, { "hand_L", "hand_R", "arm_upper_L", "arm_lower_L", "arm_upper_R", "arm_lower_R" } },
};

/*std::unordered_map<ItemDefinitionIndex, AimbotWeapon_t, Util::IntHash<ItemDefinitionIndex>> Settings::Aimbot::weapons = {
		{ ItemDefinitionIndex::INVALID, { false, false, false, Bone::BONE_HEAD, ButtonCode_t::MOUSE_MIDDLE, false, false, 1.0f, SmoothType::SLOW_END, false, 0.0f, false, 0.0f, true, 180.0f, false, 25.0f, false, false, 2.0f, 2.0f, false, false, false, false, false, false, false, false, 10.0f, false, false, false, 5.0f } },
};*/

static const char* targets[] = { "pelvis", "", "", "spine_0", "spine_1", "spine_2", "spine_3", "neck_0", "head_0" };

float velocityhitchance()
{
	float velocity;
	float Imbored = (1 / g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy());

	if (!g_LocalPlayer->m_hActiveWeapon()) {return FLT_MAX; }

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
	{
		if (Imbored >= 50)
		{
			velocity = (GetWeaponHitChance(g_LocalPlayer->m_hActiveWeapon()) - 1);
		}
		else
		{
			velocity = FLT_MAX;
		}
	}
	else
	{
		velocity = g_LocalPlayer->m_vecVelocity().Length2D();
	}

	return velocity;
}

/*
float hit_chance(Vector dest) {
	int totalhits = 0;

	for (int i = 0; i < 25; i++)
	{
		if(!g_LocalPlayer->m_hActiveWeapon())
			continue;

		float a = (float)M_PI * 2.0f * ((float)(rand() % 1000) / 1000.0f);
		float c = (float)M_PI * 2.0f * ((float)(rand() % 1000) / 1000.0f);
		float b = g_LocalPlayer->m_hActiveWeapon()->GetSpread() * ((float)(rand() % 1000) / 1000.0f) * 90.0f;
		float d = g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy() * ((float)(rand() % 1000) / 1000.0f) * 90.0f;

		Vector  dir, src;
		trace_t tr;
		Ray_t   ray;
		CTraceFilter filter;

		src = g_LocalPlayer->GetEyePos();
		QAngle angles = Math::CalcAngle(src, dest);
		angles.yaw += (b) + (d);
		angles.pitch += (b) + (d);
		Math::AngleVectors(angles, dir);
		dest = src + (dir * 8192);

		ray.Init(src, dest);
		filter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		C_BasePlayer* hitent = (C_BasePlayer*) tr.hit_entity;

		if(!hitent)
			continue;

		if ( hitent != g_LocalPlayer &&
			hitent->IsPlayer() &&
			!hitent->IsDormant() &&
			hitent->IsAlive() &&
			hitent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() )
			totalhits++;
	}

	float percent = (totalhits / 25) * 100;

	return totalhits;
}*/

bool hit_chance(CUserCmd* cmd, C_BasePlayer* target)
{
	/*Vector forward, right, up;

	C_BasePlayer* local = g_LocalPlayer;
	C_BaseCombatWeapon* weapon = local->m_hActiveWeapon();

	constexpr auto max_traces = 100;

	Math::AngleVectors(cmd->viewangles, forward, right, up);

	int total_hits = 0;
	int needed_hits = static_cast<int>(max_traces * (GetWeaponHitChance(weapon) / 100.f));

	weapon->UpdateAccuracyPenalty();

	auto eyes = local->GetEyePos();
	auto flRange = weapon->GetCSWeaponData()->flRange;

	for (int i = 0; i < max_traces; i++)
	{
		RandomSeed(i + 1);

		float fRand1 = RandomFloat(0.f, 1.f);
		float fRandPi1 = RandomFloat(0.f, XM_2PI);
		float fRand2 = RandomFloat(0.f, 1.f);
		float fRandPi2 = RandomFloat(0.f, XM_2PI);

		float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
		float fRandSpread = fRand2 * weapon->GetSpread();

		float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
		float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

		auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

		QAngle viewAnglesSpread;
		Math::VectorAngles(viewSpreadForward, viewAnglesSpread);
		viewAnglesSpread.Normalize();

		Vector viewForward;
		Math::AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = eyes + (viewForward * flRange);

		trace_t tr;
		Ray_t ray;
		ray.Init(eyes, viewForward);

		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);

		if (tr.hit_entity == target)
			total_hits++;

		//if (total_hits >= needed_hits)
		//	return true;

		//if ((max_traces - i + total_hits) < needed_hits)
		//	return false;
	}

	if (total_hits >= needed_hits)
		return true;

	return false;*/

	if (GetWeaponHitChance(g_LocalPlayer->m_hActiveWeapon()) == 0)
		return false;
		
	float hitchance = 75.f + (GetWeaponHitChance(g_LocalPlayer->m_hActiveWeapon()) / 4);
	return ((0.998f - g_LocalPlayer->m_hActiveWeapon()->GetInaccuracy()) * 100.f < hitchance);
}

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

	switch (Settings::Aim::TestAimbot::TargetPriority) {
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
				if (pl->CanSeePlayer(g_LocalPlayer, g_LocalPlayer->GetHitboxPos((int)(HITBOX_HEAD))));
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
				if (threatLvl > maxThreat) {
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
		
	if (hit_chance(cmd, target))
		return false;

	return true;
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

	if (!CanFire(cmd))
		return false;

	if (activeWeapon->IsKnife())
		return false;

	if (activeWeapon->IsGrenade())
		return false;

	return true;
}

void DoAim(CUserCmd* cmd, bool bSendPacket)
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !g_EngineClient->IsInGame() || !g_LocalPlayer->m_hActiveWeapon())
		return;

	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	QAngle angle;
	g_EngineClient->GetViewAngles(&angle);

	ConVar* recoil_scale = g_CVar->FindVar("weapon_recoil_scale");

	if (cmd->buttons & IN_USE)
		return;

	if (Settings::AntiAim::Pitch::Type != 0 || Settings::AntiAim::Yaw::Type != 0)
	{
		if (!Settings::Aim::TestAimbot::Enabled || !Settings::Aim::TestAimbot::_enabled) {
			if (!CanFire(cmd))
			{
				return AntiAim(cmd, bSendPacket);
			}
		}
		
		if (!Settings::Aim::TestAimbot::AutoShoot)
		{
			if (!CanFire(cmd))
			{
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

		if (found && IsViable(cmd, pl, found, dmg, bone)) {
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
					LastState::Get().virtualViewangles = cmd->viewangles;
					g_EngineClient->GetViewAngles(&LastState::Get().viewangles);

					LastState::Get().virtualViewangles.Normalize();
					Math::ClampAngles(LastState::Get().virtualViewangles);

					g_EngineClient->SetViewAngles(&LastState::Get().viewangles);
				}

				Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
			}

			if (Settings::Aim::TestAimbot::AutoShoot)
			{
				if (activeWeapon->CanFire()) {
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
			if (Settings::AntiAim::Pitch::Type != 0 || Settings::AntiAim::Yaw::Type != 0)
			{
				if (!CanFire(cmd))
				{
					return AntiAim(cmd, bSendPacket);
				}
			}

			RCS(cmd);
		}
	}
	else
	{
		if (Settings::AntiAim::Pitch::Type != 0 || Settings::AntiAim::Yaw::Type != 0)
		{
			if (!CanFire(cmd))
			{
				return AntiAim(cmd, bSendPacket);
			}
		}

		RCS(cmd);
	}
}