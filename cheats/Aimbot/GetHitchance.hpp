#pragma once

#include "../common_includes.hpp"
#include "RCS.hpp"
#include "AimbotSettings.hpp"

#define M_PI   3.14159265358979323846
#define XM_2PI 6.283185307f

inline bool CompareHitchance(CUserCmd* cmd, C_BasePlayer* target, Vector pos)
{
	Vector forward, right, up;

	C_BasePlayer* local = g_LocalPlayer;
	C_BaseCombatWeapon* weapon = local->m_hActiveWeapon();

	constexpr auto max_traces = 250;

	Math::AngleVectors(Math::CalcAngle(g_LocalPlayer->GetEyePos(), pos), forward, right, up);

	int total_hits = 0;
	float needed_hitchance = GetWeaponHitChance(weapon);
	int needed_hits = (max_traces / 100) * needed_hitchance;

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

		float current_hitchance = ((float)total_hits / (float)max_traces) * 100.f;

		if (current_hitchance >= needed_hitchance)
			return true;
	}


	float current_hitchance = ((float)total_hits / (float)max_traces) * 100.f;

	if (current_hitchance >= needed_hitchance)
		return true;

	return false;
}
