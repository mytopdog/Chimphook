#include "Glow.hpp"

bool Settings::ESP::Glow::Players::Teammates::Enabled = false;
bool Settings::ESP::Glow::Players::Teammates::Cover = false;
int Settings::ESP::Glow::Players::Teammates::Style = 0;
Color Settings::ESP::Glow::Players::Teammates::Colour = Color(0, 0, 0);

bool Settings::ESP::Glow::Players::Enemies::Enabled = false;
bool Settings::ESP::Glow::Players::Enemies::Cover = false;
int Settings::ESP::Glow::Players::Enemies::Style = 0;
Color Settings::ESP::Glow::Players::Enemies::Colour = Color(0, 0, 0);

void Glow::Shutdown()
{
	for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
		GlowObjectDefinition_t& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		C_BasePlayer* pl = (C_BasePlayer*)glowObject.m_pEntity;

		if (glowObject.IsUnused())
			continue;

		if (!pl || pl->IsDormant())
			continue;

		glowObject.m_flAlpha = 0.f;
	}
}

void Glow::Run()
{
	for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
		GlowObjectDefinition_t& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		C_BasePlayer* pl = (C_BasePlayer*)glowObject.m_pEntity;

		if (glowObject.IsUnused())
			continue;

		if (!pl || pl->IsDormant())
			continue;

		ClassId classid = pl->GetClientClass()->m_ClassID;

		if (classid == ClassId_CCSPlayer)
		{
			if (pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			{
				Color col = Settings::ESP::Glow::Players::Teammates::Colour;
				//glowObject.m_bFullBloomRender = Settings::ESP::Glow::Players::Teammates::Cover;
				glowObject.m_nGlowStyle = Settings::ESP::Glow::Players::Teammates::Style;
				glowObject.m_bRenderWhenOccluded = Settings::ESP::Glow::Players::Teammates::Enabled;
				glowObject.m_flRed = col.r() / 255.f;
				glowObject.m_flGreen = col.g() / 255.f;
				glowObject.m_flBlue = col.b() / 255.f;
				glowObject.m_flAlpha = col.a() / 255.f;
			}
			else
			{
				Color col = Settings::ESP::Glow::Players::Enemies::Colour;
				//glowObject.m_bFullBloomRender = Settings::ESP::Glow::Players::Enemies::Cover;
				glowObject.m_nGlowStyle = Settings::ESP::Glow::Players::Enemies::Style;
				glowObject.m_bRenderWhenOccluded = Settings::ESP::Glow::Players::Enemies::Enabled;
				glowObject.m_flRed = col.r() / 255.f;
				glowObject.m_flGreen = col.g() / 255.f;
				glowObject.m_flBlue = col.b() / 255.f;
				glowObject.m_flAlpha = col.a() / 255.f;
			}
		}
	}
}
