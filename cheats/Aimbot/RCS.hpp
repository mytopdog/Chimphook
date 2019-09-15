#pragma once

#include "../common_includes.hpp"

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

static QAngle RCSLastPunch;

void RCS(CUserCmd* cmd);