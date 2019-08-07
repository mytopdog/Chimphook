#include "backtracking.hpp"
#include "../helpers/math.hpp"

float Backtrack::GetLerpTime()
{
	const float ratio = std::clamp(interp_ratio->GetFloat(), min_interp_ratio->GetFloat(), max_interp_ratio->GetFloat());
	return std::max(interp->GetFloat(), max_update_rate ? max_update_rate->GetFloat() : update_rate->GetFloat());
}

bool Backtrack::IsValidTick(float simtime)
{
	INetChannelInfo* network = g_EngineClient->GetNetChannelInfo();

	if (!network)
		return false;

	auto delta = std::clamp(network->GetLatency(0) + GetLerpTime(), 0.f, max_unlag->GetFloat()) - (g_GlobalVars->curtime - simtime);
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
		C_BasePlayer* pl= C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || pl == g_LocalPlayer || pl->IsDormant() || !pl->IsAlive() || pl->m_iTeamNum() == pl->m_iTeamNum()) {
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
		record.headpos = g_LocalPlayer->GetHitboxPos(HITBOX_HEAD);
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
	if (Settings::Backtrack::Enabled)
		return;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	if (!g_LocalPlayer)
		return;

	float best_fov = 360.f;
	C_BasePlayer* best_target;
	int best_target_index;
	Vector best_head_position;
	int best_record;

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
		}
	}

	if (best_target)
	{
		if (records[best_target_index].size() <= 3)
			return;

		best_fov = 360.f;

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
			}
		}
	}

	if (best_record)
	{
		backtrack_record_t record = records[best_target_index][best_record];

		cmd->tick_count = TIME_TO_TICKS(record.simtime);
	}
}












































































