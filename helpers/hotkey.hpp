#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <vector>

namespace ImGui
{
	bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0, 0));
}