#pragma once

#include "../common_includes.hpp"

void AutoCowboy(CUserCmd* cmd);
void AutoStrafer(CUserCmd* cmd);
void BlockBot(CUserCmd* cmd);
void Bunnyhop(CUserCmd* cmd);
void EdgeJumper(CUserCmd* cmd);
void MoonWalk(CUserCmd* cmd);
void SlowWalk(CUserCmd* cmd);
void InfiniteDuck(CUserCmd* cmd);

void FakeLag(CUserCmd* cmd, bool& bSendPacket);
void FakeDuck(CUserCmd* cmd, bool& bSendPacket);