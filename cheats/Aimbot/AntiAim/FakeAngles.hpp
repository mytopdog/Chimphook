#pragma once

#include "../../common_includes.hpp"

extern matrix3x4_t FakeMatrix[128];
extern CCSGOPlayerAnimState* FakeAnimState;

void DoFakeAngs(CUserCmd* cmd, bool& bSendPacket);
void ManageFakeAnimState();