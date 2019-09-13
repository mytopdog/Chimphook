#pragma once

#include "../common_includes.hpp"

bool ChickenTamer(CUserCmd* cmd);
void AutoDefuse(CUserCmd* cmd);
void ChatMimic(const void* msg_data);

void InverseRagdollGravity();

class ChatSpammer : public Singleton<ChatSpammer>
{
public:
	void OnCreateMove();
	std::string filedata;
};

class ClantagScroller : public Singleton<ClantagScroller>
{
public:
	void OnCreateMove();
	std::string tag = "Chimphook ";
};

namespace NameStealer
{
	void OnCreateMove();
	void FireGameEvent(IGameEvent* ev);
}