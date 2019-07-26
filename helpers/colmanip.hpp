#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../imgui/imgui.h"
#include "math.hpp"

ImVec4 ColToVec4(float r, float g, float b, float a = 255.f)
{
	return ImVec4(r / 255, g / 255, b / 255, a / 255);
}

ImVec4 ColToVec4(Color col)
{
	return ImVec4(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f, col.a() / 255.f);
}

Color Darken(Color col)
{
	int R = col.r() - 16;
	int G = col.g() - 16;
	int B = col.b() - 16;
	int A = col.a();

	if (R < 0) R += 64;
	if (G < 0) G += 64;
	if (B < 0) B += 64;

	return Color(R, G, B, A);
}

Color Darken(Color col, int a)
{
	return Darken(Color(col.r(), col.g(), col.b(), a));
}

Color Lighten(Color col)
{
	int R = col.r() + 16;
	int G = col.g() + 16;
	int B = col.b() + 16;
	int A = col.a();

	if (R > 255) R -= 64;
	if (G > 255) G -= 64;
	if (B > 255) B -= 64;

	return Color(R, G, B, A);
}

Color Lighten(Color col, int a)
{
	return Lighten(Color(col.r(), col.g(), col.b(), a));
}