#pragma once

#include "../common_includes.hpp"

namespace Autowall
{
	struct FireBulletData
	{
		//public:
		Vector src;
		trace_t enter_trace;
		Vector direction;
		CTraceFilter filter;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;

		FireBulletData operator=(FireBulletData data)
		{
			src = data.src;
			enter_trace = data.enter_trace;
			direction = data.direction;
			filter = data.filter;
			trace_length = data.trace_length;
			trace_length_remaining = data.trace_length_remaining;
			current_damage = data.current_damage;
			penetrate_count = data.penetrate_count;
		}
	};

	float GetDamage(C_BasePlayer* fromPL, const Vector& vecPoint, bool teamCheck, FireBulletData& fData);
	bool HandleBulletPenetration(CCSWeaponInfo* weaponInfo, Autowall::FireBulletData& data);
}