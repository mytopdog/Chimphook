#pragma once

#include "../common_includes.hpp"

bool IsFiring(CUserCmd* cmd);
void AntiAim(CUserCmd* cmd, bool& bSendPacket);