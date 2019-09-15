#pragma once

static bool kb_thirdperson_storage = false;
static bool kb_thirdpersonspec_storage = false;
static bool kb_freelook_storage = false;
static bool kb_fakeduck_storage = false;
static bool kb_autodefuse_storage = false;
static bool kb_fakezoom_storage = false;
static bool kb_triggerbot_storage = false;
static bool kb_blockbot_storage = false;
static bool kb_edgejumper_storage = false;
static bool kb_aimbot_storage = false;

namespace Hotkeys
{
	void Run(const char* name, int key, bool* option, bool toggle, bool* storage, bool alert);
	void Begin();
}