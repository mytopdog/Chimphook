#include "backtracking.hpp"
#include "../helpers/math.hpp"

convars cvars;

bool Settings::Backtrack::Enabled = false;
int Settings::Backtrack::ms = 200;

float Backtrack::GetLerpTime()
{
	cvars.interp_ratio = g_CVar->FindVar("cl_interp_ratio");
	cvars.min_interp_ratio = g_CVar->FindVar("sv_client_min_interp_ratio");
	cvars.max_interp_ratio = g_CVar->FindVar("sv_client_max_interp_ratio");
	cvars.interp = g_CVar->FindVar("cl_interp");
	cvars.max_update_rate = g_CVar->FindVar("sv_maxupdaterate");
	cvars.update_rate = g_CVar->FindVar("cl_updaterate");
	cvars.max_unlag = g_CVar->FindVar("sv_maxunlag");

	const float ratio = std::clamp(cvars.interp_ratio->GetFloat(), cvars.min_interp_ratio->GetFloat(), cvars.max_interp_ratio->GetFloat());
	return std::max(cvars.interp->GetFloat(), cvars.max_update_rate ? cvars.max_update_rate->GetFloat() : cvars.update_rate->GetFloat());
}

bool Backtrack::IsValidTick(float simtime)
{
	cvars.interp_ratio = g_CVar->FindVar("cl_interp_ratio");
	cvars.min_interp_ratio = g_CVar->FindVar("sv_client_min_interp_ratio");
	cvars.max_interp_ratio = g_CVar->FindVar("sv_client_max_interp_ratio");
	cvars.interp = g_CVar->FindVar("cl_interp");
	cvars.max_update_rate = g_CVar->FindVar("sv_maxupdaterate");
	cvars.update_rate = g_CVar->FindVar("cl_updaterate");
	cvars.max_unlag = g_CVar->FindVar("sv_maxunlag");

	INetChannelInfo* network = g_EngineClient->GetNetChannelInfo();

	if (!network)
		return false;

	auto delta = std::clamp(network->GetLatency(0) + GetLerpTime(), 0.f, cvars.max_unlag->GetFloat()) - (g_GlobalVars->curtime - simtime);
}

void Backtrack::UpdateEntities()
{
	if (!Settings::Backtrack::Enabled || !g_LocalPlayer || !g_LocalPlayer->IsAlive()) {
		if (records->empty())
			records->clear();

		return;
	}

	for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || pl == g_LocalPlayer || !pl->IsAlive()) {
			records[i].clear();

			continue;
		}

		if (records[i].size() && (records[i].front().simtime == pl->m_flSimulationTime()))
			continue;

		uintptr_t var_map = (uintptr_t)pl + 0x24;
		int vars_count = *(int*)((uintptr_t)var_map + 0x14);

		for (int j = 0; j < vars_count; j++)
			*(uintptr_t*)(*(uintptr_t*)var_map + j * 0xC) = 0;

		backtrack_record_t record;
		record.angs = pl->GetAngles();
		record.origin = pl->m_vecOrigin();
		record.headpos = pl->GetHitboxPos(HITBOX_HEAD);
		record.velocity = pl->m_vecVelocity();
		record.cycle = pl->m_flCycle();
		record.sequence = pl->m_nSequence();
		record.flags = (EntityFlags)pl->m_fFlags();
		record.lowerbodyyawtarget = pl->m_flLowerBodyYawTarget();

		record.simtime = pl->m_flSimulationTime();

		pl->SetupBones(record.matrix, 128, 0x7FF00, g_GlobalVars->curtime);

		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > (size_t)TIME_TO_TICKS((float)Settings::Backtrack::ms / 1000.f))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const backtrack_record_t & rec) { return !Backtrack::Get().IsValidTick(rec.simtime); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

void Backtrack::Run(CUserCmd* cmd)
{
	if (!Settings::Backtrack::Enabled)
		return;

	if (!g_LocalPlayer)
		return;

	float best_fov = 255.f;
	C_BasePlayer* best_target;
	int best_target_index;
	Vector best_head_position;
	int best_record;
	bool found = false;

	for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || pl == g_LocalPlayer || pl->IsDormant() || !pl->IsAlive() || pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			continue;

		Vector headpos = pl->GetBonePos(HEAD_0);

		QAngle angle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), headpos);
		float fov = Math::GetFOV(cmd->viewangles, angle);

		if (fov < best_fov)
		{
			best_fov = fov;
			best_target = pl;
			best_target_index = i;
			best_head_position = headpos;
			found = true;
		}
	}

	if (found && best_target)
	{
		found = false;

		if (records[best_target_index].size() <= 3)
			return;

		best_fov = 255.f;

		for (size_t i = 0; i < records[best_target_index].size(); i++)
		{
			backtrack_record_t* record = &records[best_target_index][i];

			if (!record || !IsValidTick(record->simtime))
				continue;

			QAngle angle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), record->headpos);
			float fov = Math::GetFOV(cmd->viewangles, angle);

			if (fov < best_fov)
			{
				best_fov = fov;
				best_record = i;
				found = true;
			}
		}
	}

	if (found && best_record)
	{
		backtrack_record_t record = records[best_target_index][best_record];
		selectedpl = best_target_index;
		selectedr = best_record;

		if (cmd->buttons & IN_ATTACK)
			cmd->tick_count = TIME_TO_TICKS(record.simtime);
	}
	else
	{
		selectedpl = 0;
		selectedr = 0;
	}
}












































































