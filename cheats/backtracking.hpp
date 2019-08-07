#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include <deque>

ConVar* interp_ratio;
ConVar* min_interp_ratio;
ConVar* max_interp_ratio;
ConVar* interp;
ConVar* max_update_rate;
ConVar* update_rate;
ConVar* max_unlag;

#define TICK_INTERVAL			( g_GlobalVars->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

struct backtrack_record_t
{
	Vector headpos;
	float simtime;
	matrix3x4_t matrix[128];
};

class Backtrack : public Singleton<Backtrack>
{
public:
	std::deque<backtrack_record_t> records[65];

	bool IsValidTick(float simtime);
	void Run(CUserCmd* cmd);

	float GetLerpTime();
	void UpdateEntities();

	void Init()
	{
		interp_ratio = g_CVar->FindVar("cl_interp_ratio");
		min_interp_ratio = g_CVar->FindVar("sv_client_min_interp_ratio");
		max_interp_ratio = g_CVar->FindVar("sv_client_max_interp_ratio");
		interp = g_CVar->FindVar("cl_interp");
		max_update_rate = g_CVar->FindVar("sv_maxupdaterate");
		update_rate = g_CVar->FindVar("cl_updaterate");
		max_unlag = g_CVar->FindVar("sv_maxunlag");
	}
};

// OwO

//slave boy, code *cracks whip* 