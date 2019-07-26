#pragma once

class CGlobalVarsBase
{
public:
	float realtime;
	int framecount;
	float absolute_frametime;
	float absolute_frame_start_time;
	float curtime;
	float frame_time;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int sim_ticks_this_frame;
	int network_protocol;
	void* p_save_data;
	bool is_client;
	bool is_remote_client;
	int timestamp_networking_base;
	int timestamp_randomize_window;

private:
    // 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
    int       nTimestampNetworkingBase;
    // 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to Set the networking basis, prevents
    //  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
    int       nTimestampRandomizeWindow;
};