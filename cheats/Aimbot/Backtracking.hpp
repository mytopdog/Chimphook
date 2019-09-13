#pragma once
#include "../common_includes.hpp"

struct convars
{
	ConVar* interp_ratio;
	ConVar* min_interp_ratio;
	ConVar* max_interp_ratio;
	ConVar* interp;
	ConVar* max_update_rate;
	ConVar* update_rate;
	ConVar* max_unlag;
};

extern convars cvars;

struct backtrack_record_t
{
	QAngle angs;
	Vector origin;
	Vector headpos;
	Vector velocity;
	float cycle;
	int sequence;
	EntityFlags flags;
	float lowerbodyyawtarget;

	matrix3x4_t matrix[128];
	float simtime;
};

class Backtrack : public Singleton<Backtrack>
{
public:
	int selectedpl;
	int selectedr;
	std::deque<backtrack_record_t> records[700];

	bool IsValidTick(float simtime);
	void Run(CUserCmd* cmd);

	float GetLerpTime();
	void UpdateEntities();
};

// OwO

//slave boy, code *cracks whip* 