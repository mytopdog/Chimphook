#include "Backtracking.hpp"
#include "../Aimbot/AntiAim.hpp"
#include "../Aimbot/Autowall.hpp"
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
	if (!Settings::Backtrack::Enabled)
	{
		for (int i = 0; i < 65; i++)
		{
			records[i].clear();
		}

		return;
	}

	for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || !pl->IsPlayer() || pl == g_LocalPlayer || !pl->IsAlive() || pl->IsDormant())
		{
			records[i].clear();

			continue;
		}

		if (records[i].size() && (records[i].front().simtime == pl->m_flSimulationTime()))
			continue;

		uintptr_t var_map = (uintptr_t)pl + 0x24;
		int vars_count = *(int*)((uintptr_t)var_map + 0x14);

		for (int j = 0; j < vars_count; j++)
			* (uintptr_t*)(*(uintptr_t*)var_map + j * 0xC) = 0;

		backtrack_record_t record;
		record.angs = pl->GetAngles();
		record.origin = pl->m_vecOrigin();
		record.headpos = pl->GetHitboxPos(HITBOX_HEAD);
		record.entity = pl;
		record.simtime = pl->m_flSimulationTime();

		pl->SetupBones(record.matrix, 128, 0x7FF00, g_GlobalVars->curtime);

		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > (size_t)TIME_TO_TICKS((float)Settings::Backtrack::ms / 1000.f))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const backtrack_record_t& rec) { return !Backtrack::Get().IsValidTick(rec.simtime); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

void Backtrack::Run(CUserCmd* cmd)
{
	if (!Settings::Backtrack::Enabled)
		return;

	if (!g_LocalPlayer)
		return;

	if (IsFiring(cmd))
	{
		for (int i = 0; i < 65; i++)
		{
			for (int j = 0; j < records[i].size(); j++)
			{
				backtrack_record_t record = records[i][j];

				unsigned long origBoneCount = *record.entity->mostRecentModelBoneCounter();
				matrix3x4_t* origBoneCached = *record.entity->cachedBoneData();

				*record.entity->mostRecentModelBoneCounter() = 128;
				*record.entity->cachedBoneData() = record.matrix;

				Vector dst, forward;

				Math::AngleVectors(cmd->viewangles, forward);
				dst = g_LocalPlayer->GetEyePos() + (forward * 8196.f);
				
				Ray_t ray;
				ray.Init(g_LocalPlayer->GetEyePos(), dst);
				trace_t tr;

				g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, record.entity, &tr);

				*record.entity->mostRecentModelBoneCounter() = origBoneCount;
				*record.entity->cachedBoneData() = origBoneCached;

				if (tr.hit_entity == record.entity)
				{
					cmd->tick_count = TIME_TO_TICKS(record.simtime);

					return;
				}
			}
		}
	}
}