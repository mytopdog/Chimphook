#include <math.h>  
#include "hotkeys.hpp"
#include "../settings.hpp"
#include "../helpers/math.hpp"

#include "../valve_sdk/csgostructs.hpp"

bool Settings::System::ChimpHookNotifications = true;

bool Settings::Misc::ThirdPerson::_enabled = false;
bool Settings::Misc::ThirdPersonSpectate::_enabled = false;
bool Settings::Misc::FreeLook::_enabled = false;
bool Settings::Misc::FakeDuck::_enabled = false;
bool Settings::Misc::AutoDefuse::_enabled = false;
bool Settings::Misc::FakeZoom::_enabled = false;
bool Settings::Misc::Triggerbot::_enabled = false;
bool Settings::Misc::Blockbot::_enabled = false;
bool Settings::Misc::EdgeJumper::_enabled = false;
bool Settings::Aim::TestAimbot::_enabled = false;

int Settings::KeyBinds::ThirdPerson::Key = 0;
int Settings::KeyBinds::ThirdPerson::Type = 0;

int Settings::KeyBinds::ThirdPersonSpec::Key = 0;
int Settings::KeyBinds::ThirdPersonSpec::Type = 0;

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

int Settings::KeyBinds::Blockbot::Key = 0;
int Settings::KeyBinds::Blockbot::Type = 0;

int Settings::KeyBinds::EdgeJumper::Key = 0;
int Settings::KeyBinds::EdgeJumper::Type = 0;

int Settings::KeyBinds::Aimbot::Key = 0;
int Settings::KeyBinds::Aimbot::Type = 0;

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

		/*if (old != *option && alert && Settings::System::ChimpHookNotifications) {
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
		}*/
	}
}

void Hotkeys::Begin()
{
	if (Settings::Misc::ThirdPerson::Enabled && Settings::KeyBinds::ThirdPerson::Key)
		Hotkeys::Run("ThirdPerson", Settings::KeyBinds::ThirdPerson::Key, &Settings::Misc::ThirdPerson::_enabled, (bool)(Settings::KeyBinds::ThirdPerson::Type == 1), &kb_thirdperson_storage, false); 
	
	if (Settings::Misc::ThirdPersonSpectate::Enabled && Settings::KeyBinds::ThirdPersonSpec::Key)
		Hotkeys::Run("ThirdPerson Spectate", Settings::KeyBinds::ThirdPersonSpec::Key, &Settings::Misc::ThirdPersonSpectate::_enabled, (bool)(Settings::KeyBinds::ThirdPersonSpec::Type == 1), &kb_thirdpersonspec_storage, false);

	if (Settings::Misc::FreeLook::Enabled && Settings::KeyBinds::FreeLook::Key)
		Hotkeys::Run("FreeLook", Settings::KeyBinds::FreeLook::Key, &Settings::Misc::FreeLook::_enabled, (bool)(Settings::KeyBinds::FreeLook::Type == 1), &kb_freelook_storage, false);

	if (Settings::Misc::FakeDuck::Enabled && Settings::KeyBinds::FakeDuck::Key)
		Hotkeys::Run("FakeDuck", Settings::KeyBinds::FakeDuck::Key, &Settings::Misc::FakeDuck::_enabled, (bool)(Settings::KeyBinds::FakeDuck::Type == 1), &kb_fakeduck_storage, false);

	if (Settings::Misc::AutoDefuse::Enabled && Settings::KeyBinds::AutoDefuse::Key)
		Hotkeys::Run("AutoDefuse", Settings::KeyBinds::AutoDefuse::Key, &Settings::Misc::AutoDefuse::_enabled, (bool)(Settings::KeyBinds::AutoDefuse::Type == 1), &kb_autodefuse_storage, true);

	if (Settings::Misc::Triggerbot::Enabled && Settings::KeyBinds::Triggerbot::Key)
		Hotkeys::Run("Triggerbot", Settings::KeyBinds::Triggerbot::Key, &Settings::Misc::Triggerbot::_enabled, (bool)(Settings::KeyBinds::Triggerbot::Type == 1), &kb_triggerbot_storage, true);

	if (Settings::Misc::FakeZoom::Enabled && Settings::KeyBinds::FakeZoom::Key)
		Hotkeys::Run("FakeZoom", Settings::KeyBinds::FakeZoom::Key, &Settings::Misc::FakeZoom::_enabled, (bool)(Settings::KeyBinds::FakeZoom::Type == 1), &kb_fakezoom_storage, false);

	if (Settings::Misc::Blockbot::Enabled && Settings::KeyBinds::Blockbot::Key)
		Hotkeys::Run("Blockbot", Settings::KeyBinds::Blockbot::Key, &Settings::Misc::Blockbot::_enabled, (bool)(Settings::KeyBinds::Blockbot::Type == 1), &kb_blockbot_storage, true);
	
	if (Settings::Misc::EdgeJumper::Enabled && Settings::KeyBinds::EdgeJumper::Key)
		Hotkeys::Run("Edge Jumper", Settings::KeyBinds::EdgeJumper::Key, &Settings::Misc::EdgeJumper::_enabled, (bool)(Settings::KeyBinds::EdgeJumper::Type == 1), &kb_edgejumper_storage, true);

	if (Settings::Aim::TestAimbot::Enabled && Settings::KeyBinds::Aimbot::Key)
		Hotkeys::Run("Aimbot", Settings::KeyBinds::Aimbot::Key, &Settings::Aim::TestAimbot::_enabled, (bool)(Settings::KeyBinds::Aimbot::Type == 1), &kb_aimbot_storage, true);

	if (!Settings::KeyBinds::ThirdPerson::Key)
		Settings::Misc::ThirdPerson::_enabled = true;
		
	if (!Settings::KeyBinds::ThirdPersonSpec::Key)
		Settings::Misc::ThirdPersonSpectate::_enabled = true;

	if (!Settings::KeyBinds::FreeLook::Key)
		Settings::Misc::FreeLook::_enabled = true;

	if (!Settings::KeyBinds::FakeDuck::Key)
		Settings::Misc::FakeDuck::_enabled = true;

	if (!Settings::KeyBinds::AutoDefuse::Key)
		Settings::Misc::AutoDefuse::_enabled = true;

	if (!Settings::KeyBinds::Triggerbot::Key)
		Settings::Misc::Triggerbot::_enabled = true;

	if (!Settings::KeyBinds::FakeZoom::Key)
		Settings::Misc::FakeZoom::_enabled = true;

	if (!Settings::KeyBinds::Blockbot::Key)
		Settings::Misc::Blockbot::_enabled = true;

	if (!Settings::KeyBinds::EdgeJumper::Key)
		Settings::Misc::EdgeJumper::_enabled = true;

	if (!Settings::KeyBinds::Aimbot::Key)
		Settings::Aim::TestAimbot::_enabled = true;
}