#pragma once

#include "../settings.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../hooks.hpp"

extern C_BasePlayer* AimbotTarget;

template<typename T>
struct IntHash
{
	size_t operator()(const T& t) const noexcept
	{
		return std::hash<int>()((int)t);
	}
};

void RCS(CUserCmd* cmd);
void DoAim(CUserCmd* cmd, bool bSendPacket);
void AntiAim(CUserCmd* cmd, bool bSendPacket);

void Triggerbot(CUserCmd* pCmd);