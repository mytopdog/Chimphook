#include "Fun.hpp"
#include <random>

#include "../../helpers/protobuff.hpp"

bool Settings::Misc::ClantagScroller::Enabled = false;
std::string Settings::Misc::ClantagScroller::Clantag = "Chimphook";
int Settings::Misc::ClantagScroller::Delay = 250;

void ClantagScroller::OnCreateMove()
{
	if (!Settings::Misc::ClantagScroller::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < Settings::Misc::ClantagScroller::Delay)
		return;

	timeStamp = currentTime_ms;

	if (tag.size() == 0)
		return;

	std::rotate(std::begin(tag), std::next(std::begin(tag)), std::end(tag));
	Utils::SetClantag(tag.substr(0, 16).c_str());
}

inline bool exists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool Settings::Misc::ChatSpammer::Enabled = false;
std::string Settings::Misc::ChatSpammer::Filename = "";

void ChatSpammer::OnCreateMove()
{
	if (!Settings::Misc::ChatSpammer::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < 200)
		return;

	std::vector<std::string> lines = Utils::Split(filedata, "\n");

	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, lines.size() - 1);
	int rnum = dist(engine);

	if (rnum < lines.size())
	{
		std::string line = lines[rnum];

		std::string cmd = "say ";
		cmd += line.c_str();

		g_EngineClient->ClientCmd_Unrestricted(cmd.c_str());

		timeStamp = currentTime_ms;
	}
}

bool Settings::System::PlayerList::NameStealer::Enabled = false;
bool Settings::System::PlayerList::NameStealer::Enemies = false;
int Settings::System::PlayerList::NameStealer::Delay = 200;

static int CurrentPl = 0;

void NameStealer::OnCreateMove()
{
	if (!Settings::System::PlayerList::NameStealer::Enabled)
		return;

	long currentTime_ms = Utils::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < Settings::System::PlayerList::NameStealer::Delay)
		return;

	if (CurrentPl > 64)
		CurrentPl = 0;

	if (CurrentPl == -1)
	{
		Utils::SetName("\n\xAD\xAD\xAD");
		CurrentPl++;
		return;
	}

	while (CurrentPl < g_EngineClient->GetMaxClients())
	{
		CurrentPl++;

		if (CurrentPl >= g_EngineClient->GetMaxClients())
			CurrentPl = 1;

		if (CurrentPl == 0)
		{
			Utils::SetName("\n\xAD\xAD\xAD");

			timeStamp = currentTime_ms;
			break;
		}

		C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(CurrentPl);

		if (!player)
			continue;

		if (!player->IsPlayer())
			continue;

		if (!Settings::System::PlayerList::NameStealer::Enemies && g_LocalPlayer->m_iTeamNum() != player->m_iTeamNum())
			continue;

		player_info_t info = player->GetPlayerInfo();

		if (!info.szName || info.fakeplayer)
			continue;

		std::string set;

		set += info.szName;
		set += " ";
		char* setn = &set[0];

		Utils::SetName(setn);

		timeStamp = currentTime_ms;

		break;
	}
}

void NameStealer::FireGameEvent(IGameEvent* ev)
{
	if (!ev)
		return;

	if (!strcmp(ev->GetName(), "game_start") || !strcmp(ev->GetName(), "round_start") || !strcmp(ev->GetName(), "game_newmap"))
	{
		CurrentPl = -1;
	}
}

bool Settings::Misc::ChickenTamer = false;

float Distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool ChickenTamer(CUserCmd* cmd)
{
	if (!Settings::Misc::ChickenTamer)
		return false;

	Vector ownPos = g_LocalPlayer->GetEyePos();

	if (cmd->buttons & IN_ATTACK)
		return false;

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity->IsChicken())
		{
			Vector pos = entity->m_vecOrigin();
			pos.z += 7.5;

			float dist = pos.DistTo(ownPos);
			C_BasePlayer* playerLeader = entity->m_hLeader();

			if (!playerLeader || playerLeader != g_LocalPlayer)
			{
				if (dist < 80)
				{

					QAngle angle = Math::CalcAngle(ownPos, pos);

					QAngle oldAngle = cmd->viewangles;
					float oldForward = cmd->forwardmove;
					float oldSidemove = cmd->sidemove;

					cmd->viewangles = angle;
					cmd->buttons |= IN_USE;

					Math::CorrectMovement(oldAngle, cmd, oldForward, oldSidemove);

					return true;
				}
			}
		}
	}

	return false;
}

bool Settings::Misc::ChatMimic::Enabled = false;
bool Settings::Misc::ChatMimic::CustomMsg = false;
char Settings::Misc::ChatMimic::Msg[256] = "";

void ChatMimic(const void* msg_data)
{
	bf_read read(reinterpret_cast<uintptr_t>(msg_data));
	read.SetOffset(1);
	int ent_index = read.ReadByte();
	read.Skip(3);
	std::string msg_name = read.ReadString();
	std::string player_name = read.ReadString();
	std::string message = read.ReadString();
	read.Skip(3);
	bool all_chat = read.ReadBool();

	std::string chat;
	chat += "say ";

	C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(ent_index);

	if (!pl->notfriend || pl == g_LocalPlayer)
		return;

	if (!Settings::Misc::ChatMimic::CustomMsg)
	{
		chat += message;
	}
	else
	{
		chat += Settings::Misc::ChatMimic::Msg;
	}

	g_EngineClient->ClientCmd(chat.c_str());
}

bool Settings::Misc::InverseRagdollGravity = false;

void InverseRagdollGravity()
{
	ConVar* ragdollGrav = g_CVar->FindVar("cl_ragdoll_gravity");
	ragdollGrav->SetValue(Settings::Misc::InverseRagdollGravity ? -600 : 600);
}

bool Settings::Misc::AutoDefuse::Enabled = false;

void AutoDefuse(CUserCmd* cmd)
{
	if (!Settings::Misc::AutoDefuse::Enabled)
		return;

	if (!Settings::Misc::AutoDefuse::_enabled)
		return;

	C_BaseEntity* c4e = nullptr;

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity->IsPlantedC4())
		{
			c4e = entity;

			break;
		}
	}

	if (c4e)
	{
		C_PlantedC4* c4 = (C_PlantedC4*)c4e;
		if (g_LocalPlayer->IsAlive() && c4->m_bBombTicking() && (!c4->m_hBombDefuser() || c4->m_hBombDefuser() == g_LocalPlayer))
		{
			float bombTimer = c4->m_flC4Blow() - g_GlobalVars->curtime;
			bool shouldDefuse = g_LocalPlayer->m_bHasDefuser() ? (bombTimer < 5.2) : (bombTimer < 10.2);

			if (shouldDefuse)
			{

				cmd->buttons |= IN_USE;
			}
			else
			{
				cmd->buttons &= ~IN_USE;
			}
		}
	}
}