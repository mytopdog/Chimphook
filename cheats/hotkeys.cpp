#include <math.h>  
#include "hotkeys.hpp"
#include "../settings.hpp"
#include "../helpers/math.hpp"

#include "../valve_sdk/csgostructs.hpp"

bool Settings::System::ChimpHookNotifications = true;

bool Settings::Misc::FreeLook::_enabled = false;
bool Settings::Misc::Triggerbot::_enabled = false;
bool Settings::Misc::ThirdPerson::_enabled = false;
bool Settings::Misc::FakeDuck::_enabled = false;
bool Settings::Misc::AutoDefuse::_enabled = false;
bool Settings::Misc::FakeZoom::_enabled = false;
bool Settings::Aim::TestAimbot::_enabled = false;

bool Settings::KeyBinds::Aimbot::Enabled = false;
int Settings::KeyBinds::Aimbot::Key = 0;
int Settings::KeyBinds::Aimbot::Type = 0;

bool Settings::KeyBinds::ThirdPerson::Enabled = false;
int Settings::KeyBinds::ThirdPerson::Key = 0;
int Settings::KeyBinds::ThirdPerson::Type = 0;

int Settings::KeyBinds::FreeLook::Key = 0;
int Settings::KeyBinds::FreeLook::Type = 0;

int Settings::KeyBinds::FakeDuck::Key = 0;
int Settings::KeyBinds::FakeDuck::Type = 0;

int Settings::KeyBinds::AutoDefuse::Key = 0;
int Settings::KeyBinds::AutoDefuse::Type = 0;

int Settings::KeyBinds::Triggerbot::Key = 0;
int Settings::KeyBinds::Triggerbot::Type = 0;

int Settings::KeyBinds::FakeZoom::Key = 0;
int Settings::KeyBinds::FakeZoom::Type = 0;

void Hotkeys::Run(const char* name, int key, bool* option, bool toggle, bool* storage, bool alert)
{
	if (toggle)
	{
		if (GetAsyncKeyState(key))
		{
			if (!*storage)
			{
				*storage = true;

				if (alert && Settings::System::ChimpHookNotifications)
				{
					std::string str = "[CHIMPHOOK] ";

					if (!*option)
						str += "\x04";
					else
						str += "\x02";
					
					str += name;
					str += " is now ";

					if (!*option)
						str += "enabled.";
					else
						str += "disabled.";


					g_HUDChat->ChatPrintf(0, 0, str.c_str());
				}

				*option = !*(option);
			}
		}
		else
		{
			*storage = false;
		}
	}
	else
	{
		bool old = *option;

		if (GetAsyncKeyState(key))
		{
			*option = true;
		}
		else
		{
			*option = false;
		}

		if (old != *option && alert && Settings::System::ChimpHookNotifications) {
			std::string str = "[CHIMPHOOK] ";

			if (*option)
				str += "\x04";
			else
				str += "\x02";

			str += name;
			str += " is now ";

			if (*option)
				str += "enabled.";
			else
				str += "disabled.";

			g_HUDChat->ChatPrintf(0, 0, str.c_str());
		}
	}
}

void Hotkeys::Begin()
{
	if (Settings::Misc::ThirdPerson::Enabled && Settings::KeyBinds::ThirdPerson::Enabled)
		Hotkeys::Run("ThirdPerson", Settings::KeyBinds::ThirdPerson::Key, &Settings::Misc::ThirdPerson::_enabled, (bool)(Settings::KeyBinds::ThirdPerson::Type == 1), &kb_thirdperson_storage, false); 

	if (Settings::Misc::FreeLook::Enabled)
		Hotkeys::Run("FreeLook", Settings::KeyBinds::FreeLook::Key, &Settings::Misc::FreeLook::_enabled, (bool)(Settings::KeyBinds::FreeLook::Type == 1), &kb_freelook_storage, false);

	if (Settings::Misc::FakeDuck::Enabled)
		Hotkeys::Run("FakeDuck", Settings::KeyBinds::FakeDuck::Key, &Settings::Misc::FakeDuck::_enabled, (bool)(Settings::KeyBinds::FakeDuck::Type == 1), &kb_fakeduck_storage, false);

	if (Settings::Misc::AutoDefuse::Enabled)
		Hotkeys::Run("AutoDefuse", Settings::KeyBinds::AutoDefuse::Key, &Settings::Misc::AutoDefuse::_enabled, (bool)(Settings::KeyBinds::AutoDefuse::Type == 1), &kb_autodefuse_storage, true);

	if (Settings::Misc::Triggerbot::Enabled)
		Hotkeys::Run("Triggerbot", Settings::KeyBinds::Triggerbot::Key, &Settings::Misc::Triggerbot::_enabled, (bool)(Settings::KeyBinds::Triggerbot::Type == 1), &kb_triggerbot_storage, true);

	if (Settings::Misc::FakeZoom::Enabled)
		Hotkeys::Run("FakeZoom", Settings::KeyBinds::FakeZoom::Key, &Settings::Misc::FakeZoom::_enabled, (bool)(Settings::KeyBinds::FakeZoom::Type == 1), &kb_fakezoom_storage, false);
		
	if (Settings::Aim::TestAimbot::Enabled && Settings::KeyBinds::Aimbot::Enabled)
		Hotkeys::Run("Aimbot", Settings::KeyBinds::Aimbot::Key, &Settings::Aim::TestAimbot::_enabled, (bool)(Settings::KeyBinds::Aimbot::Type == 1), &kb_aimbot_storage, true);

	if (!Settings::KeyBinds::Aimbot::Enabled)
		Settings::Aim::TestAimbot::_enabled = true;

	if (!Settings::KeyBinds::ThirdPerson::Enabled)
		Settings::Misc::ThirdPerson::_enabled = true;

	if (GetAsyncKeyState(Settings::AntiAim::Yaw::LKey))
	{
		Settings::AntiAim::Yaw::__lkey = true;
	}

	if (GetAsyncKeyState(Settings::AntiAim::Yaw::RKey))
	{
		Settings::AntiAim::Yaw::__lkey = false;
	}
}