#include "Autowall.hpp"
#include "Aimbot.hpp"

static float GetHitgroupDamageMultiplier(HitGroups iHitGroup)
{
	switch (iHitGroup)
	{

	case HitGroups::HITGROUP_HEAD:
		return 4.0f;

	case HitGroups::HITGROUP_CHEST:
	case HitGroups::HITGROUP_LEFTARM:
	case HitGroups::HITGROUP_RIGHTARM:
		return 1.0f;

	case HitGroups::HITGROUP_STOMACH:
		return 1.25f;

	case HitGroups::HITGROUP_LEFTLEG:
	case HitGroups::HITGROUP_RIGHTLEG:
		return 0.75f;

	default:
		return 1.0f;

	}

}

static void ScaleDamage(HitGroups hitgroup, C_BasePlayer* enemy, float weapon_armor_ratio, float& current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);

	int armor = enemy->m_ArmorValue();

	if (armor > 0)
	{
		if (hitgroup == HitGroups::HITGROUP_HEAD)
		{
			if (enemy->m_bHasHelmet())
			{
				current_damage *= (weapon_armor_ratio);
			}
		}
		else
		{
			current_damage *= (weapon_armor_ratio);
		}
	}
}
// Old Scale Damage
/*current_damage *= GetHitgroupDamageMultiplier(hitgroup);
 float m_flArmor = (float)enemy->GetArmor();
 if (m_flArmor > 0.0f && hitgroup < HITGROUP_LEFTLEG)  //don't do gear, or legs
 {
 if (hitgroup == HITGROUP_HEAD && !enemy->HasHelmet())
 return;

 float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
 if ((current_damage - armorscaled) * 0.5f > m_flArmor)
 armorscaled = current_damage - (m_flArmor * 2.0f);
 current_damage = armorscaled;
 }*/

static bool TraceToExit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace)
{
	float distance = 0.0f;

	//if (!Aimbot::curTarget)
	//	return false;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = g_EngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		g_EngineTrace->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.Init(end, start);

			CTraceFilter filter;
			filter.pSkip = exit_trace->hit_entity;

			g_EngineTrace->TraceRay(ray, 0x600400B, &filter, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
		{
			if (exit_trace->hit_entity)
			{
				if (enter_trace->hit_entity && enter_trace->hit_entity == AimbotTarget)
					return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}

	return false;
}

bool Autowall::HandleBulletPenetration(CCSWeaponInfo* weaponInfo, Autowall::FireBulletData& data)
{
	surfacedata_t* enter_surface_data = g_PhysSurface->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= powf(weaponInfo->flRangeModifier, data.trace_length * 0.002f);

	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t* exit_surface_data = g_PhysSurface->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;

	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponInfo->flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

static void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	g_EngineTrace->TraceRay(ray, mask, &filter, ptr);
}

static bool SimulateFireBullet(C_BasePlayer* fromPL, C_BaseCombatWeapon* pWeapon, bool teamCheck, Autowall::FireBulletData& data)
{
	CCSWeaponInfo* weaponInfo = pWeapon->GetCSWeaponData();

	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = (float)weaponInfo->iDamage;

	while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
	{
		data.trace_length_remaining = weaponInfo->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		data.enter_trace;
		TraceLine(data.src, end, MASK_SHOT, fromPL, &data.enter_trace);

		Ray_t ray;
		ray.Init(data.src, end + data.direction * 40.f);

		g_EngineTrace->TraceRay(ray, MASK_SHOT, &data.filter, &data.enter_trace);

		TraceLine(data.src, end + data.direction * 40.f, MASK_SHOT, fromPL, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if (data.enter_trace.hitgroup <= (int)HitGroups::HITGROUP_RIGHTLEG && data.enter_trace.hitgroup > (int)HitGroups::HITGROUP_GENERIC)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= powf(weaponInfo->flRangeModifier, data.trace_length * 0.002f);

			C_BaseEntity* player = (C_BaseEntity*)data.enter_trace.hit_entity;
			if (teamCheck && player->m_iTeamNum() == fromPL->m_iTeamNum())
				return false;

			ScaleDamage((HitGroups)data.enter_trace.hitgroup, (C_BasePlayer*)player, weaponInfo->flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponInfo, data))
			break;
	}

	return false;
}
float Autowall::GetDamage(C_BasePlayer* fromPL, const Vector& point, bool teamCheck, FireBulletData& fData)
{
	float damage = 0.f;
	Vector dst = point;
	FireBulletData data;
	data.src = fromPL->GetEyePos();
	data.filter.pSkip = fromPL;

	QAngle angles = Math::CalcAngle(data.src, dst);
	Math::AngleVectors(angles, data.direction);

	data.direction.NormalizeInPlace();

	C_BaseCombatWeapon* activeWeapon = fromPL->m_hActiveWeapon();
	if (!activeWeapon)
		return -1.0f;

	if (SimulateFireBullet(fromPL, activeWeapon, teamCheck, data))
		damage = data.current_damage;

	fData.src = data.src;
	fData.enter_trace = data.enter_trace;
	fData.direction = data.direction;
	fData.filter = data.filter;
	fData.trace_length = data.trace_length;
	fData.trace_length_remaining = data.trace_length_remaining;
	fData.current_damage = data.current_damage;
	fData.penetrate_count = data.penetrate_count;

	return damage;
}