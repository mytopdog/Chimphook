#include "render.hpp"
#include "cheats/visuals.hpp"
#include "cheats/misc.hpp"
#include "fonts/andalemono.h"
#include "fonts/verdana.h"

ImFont* f_Courier;
ImFont* f_Verdana;
ImFont* f_AndaleMono;

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();


	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	InitFonts();
}

void Render::InitFonts()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0x2005, 0x28FF,
		0,
	};

	f_Verdana = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(verdana_compressed_data,
		verdana_compressed_size,
		12.f,
		nullptr,
		ranges);

	f_AndaleMono = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(andalemono_compressed_data,
		andalemono_compressed_size,
		12.f,
		nullptr,
		ranges);

	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(verdana_compressed_data,
		verdana_compressed_size,
		12.f,
		nullptr,
		ranges);
}

void Render::ClearDrawList()
{
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}

void Render::BeginScene()
{
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();


	Render::Get().RenderText("ChimpHook", ImVec2(10, 5), 12.f, Color(255, 255, 255, 255), false, false, f_Verdana);

	SniperCrosshair();
	WallbangCrosshair();
	DebugMode();
	SmokeHelper::Get().Draw();

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
		Visuals::Get().AddToDrawList();

	if (g_EngineClient->IsInGame() && g_LocalPlayer->IsAlive() && g_LocalPlayer->m_hActiveWeapon()->IsGrenade()) {
		grenade_prediction::Get().Paint();
	}		


	render_mutex.lock();
	*draw_list_act = *draw_list;
	render_mutex.unlock();
}

ImDrawList* Render::RenderScene()
{

	if (render_mutex.try_lock())
	{
		*draw_list_rendering = *draw_list_act;
		render_mutex.unlock();
	}

	return draw_list_rendering;
}


float Render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center)
		pos.x -= textSize.x / 2.0f;

	if (outline)
	{
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
	}

	draw_list->AddText(pFont, size, pos, GetU32(color), text.c_str());

	draw_list->PopTextureID();

	return pos.y + textSize.y;
}

void Render::RenderCircle3D(Vector position, float points, float radius, Color color, int thickness)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color, thickness);
	}
}

void Render::Render3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline)
{
	Vector forward, right, up;
	Math::AngleVectors(angles, forward, right, up);

	Vector points[8];
	points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar);
	points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar);
	points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar);
	points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar);

	points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar);
	points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar);
	points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar);
	points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar);

	Vector points_screen[8];
	for (int i = 0; i < 8; i++)
		if (!Math::WorldToScreen(points[i], points_screen[i]))
			return;

	RenderLine(points_screen[0].x, points_screen[0].y, points_screen[1].x, points_screen[1].y, outline);
	RenderLine(points_screen[0].x, points_screen[0].y, points_screen[2].x, points_screen[2].y, outline);
	RenderLine(points_screen[3].x, points_screen[3].y, points_screen[1].x, points_screen[1].y, outline);
	RenderLine(points_screen[3].x, points_screen[3].y, points_screen[2].x, points_screen[2].y, outline);

	RenderLine(points_screen[0].x, points_screen[0].y, points_screen[4].x, points_screen[4].y, outline);
	RenderLine(points_screen[1].x, points_screen[1].y, points_screen[5].x, points_screen[5].y, outline);
	RenderLine(points_screen[2].x, points_screen[2].y, points_screen[6].x, points_screen[6].y, outline);
	RenderLine(points_screen[3].x, points_screen[3].y, points_screen[7].x, points_screen[7].y, outline);

	RenderLine(points_screen[4].x, points_screen[4].y, points_screen[5].x, points_screen[5].y, outline);
	RenderLine(points_screen[4].x, points_screen[4].y, points_screen[6].x, points_screen[6].y, outline);
	RenderLine(points_screen[7].x, points_screen[7].y, points_screen[5].x, points_screen[5].y, outline);
	RenderLine(points_screen[7].x, points_screen[7].y, points_screen[6].x, points_screen[6].y, outline);
}