 #pragma once
#include "valve_sdk/csgostructs.hpp";
#include "cheats/misc.hpp"
#include "cheats/visuals.hpp"
#include "menu.hpp"

static constexpr const char* hitSounds[4]{
	"play physics/metal/metal_solid_impact_bullet2",
	"play buttons/arena_switch_press_02",
	"play training/timer_bell",
	"play physics/glass/glass_impact_bullet1"
};

class EventListener : public IGameEventListener2
{
public:
	EventListener(std::vector<const char*> events)
	{
		for (auto& it : events)
			g_GameEvents->AddListener(this, it, false);
	}

	~EventListener()
	{
		g_GameEvents->RemoveListener(this);
	}

	void FireGameEvent(IGameEvent * ev)
	{
		NameStealer::FireGameEvent(ev);
		BulletShots(ev);

		if (Settings::Misc::Sounds::HitSound::Enabled) {

			if (strstr(ev->GetName(), "player_hurt"))
			{
				C_BasePlayer* attacker = C_BasePlayer::GetPlayerByUserId(ev->GetInt("attacker"));

				if (attacker == g_LocalPlayer)
					g_EngineClient->ClientCmd_Unrestricted(hitSounds[Settings::Misc::Sounds::HitSound::Sound]);
			}
		}

		if (Settings::Misc::FUCK)
		{
			if (strstr(ev->GetName(), "player_death"))
			{
				C_BasePlayer* dead = C_BasePlayer::GetPlayerByUserId(ev->GetInt("userid"));
				
				if (dead == g_LocalPlayer)
					g_EngineClient->ClientCmd_Unrestricted("say FUCK");
			}

			if (strstr(ev->GetName(), "round_end"))
			{
				int team = ev->GetInt("winner");

				if (team != g_LocalPlayer->m_iTeamNum())
					g_EngineClient->ClientCmd_Unrestricted("say FUCK");
			}
		}

		if (strstr(ev->GetName(), "game_init") || strstr(ev->GetName(), "game_start") || strstr(ev->GetName(), "round_start") || strstr(ev->GetName(), "game_newmap")) {
			ConVar* skyname = g_CVar->FindVar("sv_skyname");
			Nightmode::Get().currentSkyName = skyname->GetString();
			
			if (Settings::Visuals::Nightmode::Enabled)
				Nightmode::Get().Execute();

			if (Settings::Visuals::WorldColour::Enabled)
				WorldColour::Execute();

			selectedP = -1;
		}
	}

	int GetEventDebugID() override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};