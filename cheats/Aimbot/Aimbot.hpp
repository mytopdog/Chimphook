#pragma once

#include "../common_includes.hpp"

template<typename T>
struct IntHash
{
	size_t operator()(const T& t) const noexcept
	{
		return std::hash<int>()((int)t);
	}
};

static C_BasePlayer* AimbotTarget = nullptr;
static QAngle aimAngle = QAngle(0, 0, 0);
static QAngle oaimAngle = QAngle(0, 0, 0);

void DoAim(CUserCmd* cmd, bool& bSendPacket);