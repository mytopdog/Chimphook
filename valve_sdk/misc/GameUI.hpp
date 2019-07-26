#pragma once

#include <cstdint>

class ECommandMsgBoxSlot;

class IGameUI
{
public:
	void CreateCommandMsgBox(char const* title, char const* msg)
	{
		typedef void(__thiscall * fn)(void*, char const*, char const*, bool, bool, char const*, char const*, char const*, char const*);
		return CallVFunction<fn>(this, 20)(this, title, msg, true, false, "", "", "", "");
	}
};