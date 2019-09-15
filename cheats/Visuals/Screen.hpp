#pragma once
#include "../common_includes.hpp"
#include "GrenadeData.hpp"

void DrawC4Damage(C_BaseEntity* c4ent);
void WallbangCrosshair();
void DebugMode();
void ZeusRange();

class SmokeHelper : public Singleton<SmokeHelper>
{
public:
	void Draw();
	void OnCreateMove(CUserCmd* cmd);

	GrenadeData selData;
	bool Found;
};