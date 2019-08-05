#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/pfd.h"
#include "menu.hpp"

#include "cheats/misc.hpp"
#include "config.hpp"
#include "convarc.hpp"
#include "helpers/hotkey.hpp"
#include <Windows.h>
#include <chrono>
#include "helpers/skinchangerparser.hpp"

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "settings.hpp" 

Color Settings::System::Theme = Color(100, 0, 0, 255);
Color Settings::System::TextTheme = Color(255, 255, 255, 255);
Color Settings::System::WindowTheme = Color(25, 25, 25, 255);

bool Settings::System::PlayerList::ShowBots = false;
bool Settings::System::PlayerList::ShowEnemies = false;
bool Settings::System::PlayerList::ShowLocalplayer = false;
float Settings::System::SpectatorPosX = 0.f;
float Settings::System::SpectatorPosY = 0.f;

bool CopyImageToTexture(UCHAR* pSrc, int xWidth, int yHeight, LPDIRECT3DTEXTURE9 pTex, int xPos, int yPos)
{
	D3DLOCKED_RECT d3drc;
	UCHAR r, g, b, a;
	UINT* pDest;
	int nRow, nPixel;

	// lock the texture

	if (FAILED(pTex->LockRect(0, &d3drc, NULL, 0)))
		return false;

	// adjust pitch from bytes to UINTs

	d3drc.Pitch >>= 2;

	// copy the image

	for (nRow = 0; nRow < yHeight; nRow++)
	{
		// set destination pointer for this row

		pDest = (UINT*)d3drc.pBits + (nRow + yPos) * d3drc.Pitch + xPos;

		// copy the row

		for (nPixel = 0; nPixel < xWidth; nPixel++)
		{
			// extract pixel data

			r = *pSrc++;
			g = *pSrc++;
			b = *pSrc++;
			a = *pSrc++;

			// write color word to texture

			(*pDest++) = b | (g << 8) | (r << 16) | (a << 24);
		}
	}

	// unlock texture

	pTex->UnlockRect(0);

	// return success

	return true;
}

void replaceAll(std::string& source, const std::string& from, const std::string& to)
{
	std::string newString;
	newString.reserve(source.length());

	std::string::size_type lastPos = 0;
	std::string::size_type findPos;

	while (std::string::npos != (findPos = source.find(from, lastPos)))
	{
		newString.append(source, lastPos, findPos - lastPos);
		newString += to;
		lastPos = findPos + from.length();
	}

	// Care for the rest after last occurrence
	newString += source.substr(lastPos);

	source.swap(newString);
}

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

void Menu::Initialize()
{
	CreateStyle();
	_visible = false;

	INT SWIDTH, SHEIGHT;
	g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

	Settings::System::SpectatorPosX = 0;
	Settings::System::SpectatorPosY = SHEIGHT / 2.5;

	Config::Get().LoadUser();
}

void Menu::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

int* allskins[] = {
	&Settings::SkinChanger::Paint::Knife,
	&Settings::SkinChanger::Paint::usp,
	&Settings::SkinChanger::Paint::p2000,
	&Settings::SkinChanger::Paint::glock,
	&Settings::SkinChanger::Paint::p250,
	&Settings::SkinChanger::Paint::fiveseven,
	&Settings::SkinChanger::Paint::tec,
	&Settings::SkinChanger::Paint::cz,
	&Settings::SkinChanger::Paint::duals,
	&Settings::SkinChanger::Paint::revolver,
	&Settings::SkinChanger::Paint::famas,
	&Settings::SkinChanger::Paint::galil,
	&Settings::SkinChanger::Paint::m4a4,
	&Settings::SkinChanger::Paint::m4a1,
	&Settings::SkinChanger::Paint::ak47,
	&Settings::SkinChanger::Paint::sg553,
	&Settings::SkinChanger::Paint::aug,
	&Settings::SkinChanger::Paint::ssg08,
	&Settings::SkinChanger::Paint::awp,
	&Settings::SkinChanger::Paint::scar,
	&Settings::SkinChanger::Paint::g3sg1,
	&Settings::SkinChanger::Paint::mac10,
	&Settings::SkinChanger::Paint::mp5sd,
	&Settings::SkinChanger::Paint::mp7,
	&Settings::SkinChanger::Paint::mp9,
	&Settings::SkinChanger::Paint::ppbizon,
	&Settings::SkinChanger::Paint::p90,
	&Settings::SkinChanger::Paint::ump45,
	&Settings::SkinChanger::Paint::mag7,
	&Settings::SkinChanger::Paint::nova,
	&Settings::SkinChanger::Paint::sawedoff,
	&Settings::SkinChanger::Paint::xm1014,
	&Settings::SkinChanger::Paint::m249,
	&Settings::SkinChanger::Paint::negev
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		std::string* str = (std::string*)data->UserData;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}

	return 0;
}

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		ImVec4 clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, show_alpha))
		{
			if (!show_alpha)
				clr.w = 1.f;

			v->SetColor(clr.x * 255.f, clr.y * 255.f, clr.z * 255.f, clr.w * 255.f);
			return true;
		}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
	inline bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = (ImGuiInputTextFlags)0, ImGuiInputTextCallback cb = (ImGuiInputTextCallback)0, void* user_data = (void*)0)
	{
		flags |= ImGuiInputTextFlags_CallbackResize;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, (void*)str);
	}
}

bool IsChecked(const char* name)
{
	for (auto i : checkedByVolvo)
	{
		if (!strcmp(i, name)) {
			return true;
		}
	}

	return false;
}

void RenderSpectatorList()
{
	ImGui::SetNextWindowPos(ImVec2(Settings::System::SpectatorPosX, Settings::System::SpectatorPosY), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, -ImGui::GetContentRegionAvail().y));

	bool spc = Settings::Misc::SpectatorList;

	ImGuiWindowFlags flags;
	
	flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing;

	if (spc)
	{
		if (ImGui::Begin("Spectator List", nullptr, flags))
		{
			static ImVec2 oldPos = ImGui::GetWindowPos();
			Settings::System::SpectatorPosX = ImGui::GetWindowPos().x;
			Settings::System::SpectatorPosY = ImGui::GetWindowPos().y;

			if (oldPos.x != Settings::System::SpectatorPosX || oldPos.y != Settings::System::SpectatorPosY) {
				oldPos.x = Settings::System::SpectatorPosX;
				oldPos.y = Settings::System::SpectatorPosY;

				Config::Get().SaveUser();
			}

			ImGui::Columns(2, nullptr, false);

			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Mode"); ImGui::NextColumn();

			if (!g_EngineClient->IsInGame())
				return ImGui::End();

			if (!g_LocalPlayer || !g_LocalPlayer->SelfOrObs() || !g_LocalPlayer->SelfOrObs()->IsAlive())
				return ImGui::End();

			for (int i = 0; i <= g_EngineClient->GetMaxClients(); i++)
			{
				C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(i);
				if (!player || !player->m_hObserverTarget() || player->IsAlive())
					continue;

				player_info_t info = player->GetPlayerInfo();
				C_BaseEntity * target = player->m_hObserverTarget();

				if (target == g_LocalPlayer->SelfOrObs())
				{
					ImGui::Text(info.szName); ImGui::NextColumn();
					ImGui::Text(spectypes[player->m_iObserverMode()]); ImGui::NextColumn();
				}
			}

			ImGui::Columns(1, nullptr, false);
		}
		ImGui::End();
	}
}

void RenderWeapon(int* selected, int i, const char* name)
{
	if (ImGui::Selectable(name, (bool)(*selected == i), ImGuiSelectableFlags_SpanAllColumns))
		*selected = i;

	ImGui::NextColumn();
	ImGui::Text(parser_skins[*allskins[i]].name.c_str());

	ImGui::NextColumn();
}

std::vector<ImTextureID> Menu::Render(IDirect3DDevice9* pDevice)
{
	std::vector<ImTextureID> texturesToRelease;

	CreateStyle();

	ImGui::GetIO().MouseDrawCursor = _visible;

	RenderSpectatorList();

	if (!_visible)
		return texturesToRelease;

	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 750, 850 }, ImGuiSetCond_Once);

	if (ImGui::Begin("Chimphook",
		&_visible,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNavFocus ))
	{
		static int tabSelected = 0;

		bool thisSel = tabSelected == 0;
		if (ImGui::ToggleButton("Misc", &thisSel, ImVec2(ImGui::GetWindowSize().x / 5 - 1, 20)))
		{
			tabSelected = 0;
		}
		thisSel = tabSelected == 1;
		ImGui::SameLine(0, 0);
		if (ImGui::ToggleButton("Visuals", &thisSel, ImVec2(ImGui::GetWindowSize().x / 5 - 1, 20)))
		{
			tabSelected = 1;
		}
		thisSel = tabSelected == 2;
		ImGui::SameLine(0, 0);
		if (ImGui::ToggleButton("Aim", &thisSel, ImVec2(ImGui::GetWindowSize().x / 5 - 1, 20)))
		{
			tabSelected = 2;
		}
		thisSel = tabSelected == 3;
		ImGui::SameLine(0, 0);
		if (ImGui::ToggleButton("AntiAim", &thisSel, ImVec2(ImGui::GetWindowSize().x / 5 - 1, 20)))
		{
			tabSelected = 3;
		}
		thisSel = tabSelected == 4;
		ImGui::SameLine(0, 0);
		if (ImGui::ToggleButton("Config", &thisSel, ImVec2(ImGui::GetWindowSize().x / 5 - 1, 20)))
		{
			tabSelected = 4;
		}
		ImGui::BeginGroupBox("MainBody", ImVec2(0, 0));
		{
			switch (tabSelected)
			{
			case 0:
			{
				static int vsmscselected = 0;

				ImGui::BeginGroup();
				{
					bool vsmsc = vsmscselected == 0;
					if (ImGui::ToggleButton("Page 1##MiscPages", &vsmsc, ImVec2(ImGui::GetWindowSize().x / 2, 20)))
					{
						vsmscselected = 0;
					}
					vsmsc = vsmscselected == 1;
					ImGui::SameLine(0, 0);
					if (ImGui::ToggleButton("Page 2##MiscPages", &vsmsc, ImVec2(ImGui::GetWindowSize().x / 2, 20)))
					{
						vsmscselected = 1;
					}
				}
				ImGui::EndGroup();
				ImGui::Separator();
				ImGui::Columns(2, nullptr, false);
				switch (vsmscselected)
				{
				case 0:
					ImGui::Checkbox("Anti Untrust", &Settings::System::AntiUntrust);
				/*	if (ImGui::Button("Queue Dust2"))
					{
						Utils::QueueDust2();
					}*/
					ImGui::Text("Movement");
					ImGui::BeginGroupBox("MiscMovement", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Auto Bunnyhop", &Settings::Misc::BunnyHop);
						ImGui::Checkbox("Autostrafer", &Settings::Misc::AutoStrafer);
						ImGui::PushItemWidth(100);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Auto Cowboy", &Settings::Misc::AutoCowboy); ImGui::NextColumn();
							ImGui::Checkbox("Reverse Cowboy", &Settings::Misc::ReverseCowboy); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Checkbox("Block Bot", &Settings::Misc::BlockBot);
						ImGui::Checkbox("Edge Jumper", &Settings::Misc::EdgeJumper);
					}
					ImGui::EndGroupBox();
					ImGui::Text("Third Person");
					ImGui::BeginGroupBox("MiscThirdPerson", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::PushItemWidth(100);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Enabled", &Settings::Misc::ThirdPerson::Enabled); ImGui::NextColumn();
							ImGui::SliderInt("Offset", &Settings::Misc::ThirdPerson::CameraOffset, 30, 250);
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						// ImGui::Checkbox("Ignore Players", &Settings::Misc::ThirdPerson::IgnorePlayers);
						ImGui::Checkbox("Ignore Walls", &Settings::Misc::ThirdPerson::IgnoreWalls);
						ImGui::Checkbox("Ignore Scope", &Settings::Misc::ThirdPerson::IgnoreScope);
						// ImGui::Checkbox("Ignore Spectating", &Settings::Misc::ThirdPerson::IgnoreSpectating);
						ImGui::Checkbox("Real Angles", &Settings::Misc::ThirdPerson::RealAngles);
					}
					ImGui::EndGroupBox();
					ImGui::Text("Server");
					ImGui::BeginGroupBox("ServerSettings", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						static char sname[64] = "";
						static char sclantag[32] = "";
						static char fk[80] = "Valve Announcement: CSGO maintenance at 3pm tomorrow. Press F1 to close.";
						static char fakemsg[128] = "";

						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							if (ImGui::Button("Patch Name"))
							{
								Utils::SetName("\n\xAD\xAD\xAD");
							} ImGui::NextColumn(); ImGui::NewLine(); ImGui::NextColumn();
							ImGui::InputText("Name##ServerSettings", sname, 64); ImGui::NextColumn();
							if (ImGui::Button("Set Name##ServerSettings"))
							{
								Utils::SetName(sname);
							}
							ImGui::NextColumn();
							ImGui::InputText("Clantag##ServerSettings", sclantag, 32); ImGui::NextColumn();
							if (ImGui::Button("Set Clantag##ServerSettings"))
							{
								Utils::SetClantag(sclantag);
							}
							ImGui::NextColumn();
							ImGui::InputText("Fake Kick", fk, 80); ImGui::NextColumn();
							if (ImGui::Button("Set Name##fkk"))
							{
								if (g_EngineClient->IsInGame() && g_LocalPlayer) {
									std::string fk_s = fk;
									replaceAll(fk_s, "\\n", "\n");
									std::string n = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
									n += fk_s;
									n += "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
									Utils::SetName(n.c_str());
								}
							}
							ImGui::NextColumn();
							// ImGui::InputText("Fake Message", fakemsg, 128); ImGui::NextColumn();
						/*	if (ImGui::Button("Set Name##fkn"))
							{
								if (g_EngineClient->IsInGame() && g_LocalPlayer)
								{
									std::string fk_s = fakemsg;
									/*
									&W  - White,
									&R  - Red,
									&P  - Purple,
									&G  - Green,
									&LG - Light Green,
									&T  - Turqouise,
									&LR - Light Red,
									&Y  - Yellow,
									&GR - Gray,
									&LB - Light Blue,
									&B  - Blue,
									&P  - Pink,
									&DO - Dark Orange,
									&O  - Orange
								
									replaceAll(fk_s, "&W", "\x01");
									replaceAll(fk_s, "&R", "\x02");
									replaceAll(fk_s, "&P", "\x03");
									replaceAll(fk_s, "&G", "\x04");
									replaceAll(fk_s, "&LG", "\x05");
									replaceAll(fk_s, "&T", "\x06");
									replaceAll(fk_s, "&LR", "\x07");
									replaceAll(fk_s, "&GR", "\x08");
									replaceAll(fk_s, "&Y", "\x09");
									replaceAll(fk_s, "&LB", "\x0B");
									replaceAll(fk_s, "&B", "\x0D");
									replaceAll(fk_s, "&P", "\x0E");
									replaceAll(fk_s, "&DO", "\x0F");
									replaceAll(fk_s, "&O", "\x10");
									std::string n;
									n += fk_s;
									n += "                             ";
									Utils::ConsolePrint("Set Name");
									Utils::SetName(n.c_str());
								}
							}*/
						}
						ImGui::EndGroup();
						ImGui::Columns(1, nullptr, false);
						ImGui::Text("(Make sure to patch name first. Max 80 chars.)");
						ImGui::Text("Fake message: attack a teammate after setting name for this to work.");
					}
					ImGui::EndGroupBox();
					ImGui::Text("Convar Editor");
					ImGui::BeginGroupBox("MiscConvarEditor", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						static char convarName[64];
						static char convarNameS[64];
						static char convarNVal[64];

						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::InputText("Name##ConvarEditorName", convarName, 64); ImGui::NextColumn();
							if (ImGui::Button("Find Convar"))
							{
								memset(convarNVal, 0, sizeof convarNVal);
								strcpy(convarNameS, convarName);
							}
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();

						ConVar* selc = g_CVar->FindVar(convarNameS);

						if (selc)
						{
							ImGui::BeginGroup();
							{
								ImGui::Columns(2, nullptr, false);
								ImGui::InputText("Value##ConvarEditorNewValue", convarNVal, 64); ImGui::NextColumn();
								if (ImGui::Button("Set Value"))
								{
									selc->SetValue(convarNVal);
									memset(convarNVal, 0, sizeof convarNVal);
								}
								ImGui::Columns(1, nullptr, false);
							}
							ImGui::EndGroup();

							std::stringstream selectedss;
							selectedss << "Name: '";
							selectedss << selc->GetName();
							selectedss << "'";
							ImGui::Text(selectedss.str().c_str());

							std::stringstream selectedcurrentvalue;
							selectedcurrentvalue << "Current Value: '";
							selectedcurrentvalue << selc->GetString();
							selectedcurrentvalue << "'";
							ImGui::Text(selectedcurrentvalue.str().c_str());

							std::stringstream selecteddefaultvalue;
							selecteddefaultvalue << "Default Value: '";
							selecteddefaultvalue << selc->GetDefault();
							selecteddefaultvalue << "'";
							ImGui::Text(selecteddefaultvalue.str().c_str());

							std::stringstream selectedischecked;
							selectedischecked << "Is Checked: '";
							if (IsChecked(selc->GetName()))
							{
								selectedischecked << "true";
							}
							else
							{
								selectedischecked << "false";
							}
							selectedischecked << "'";
							ImGui::Text(selectedischecked.str().c_str());
						}
						else
						{
							ImGui::BeginGroup();
							{
								ImGui::Columns(2, nullptr, false);
								ImGui::InputText("Value##ConvarEditorNewValue", convarNVal, 64); ImGui::NextColumn();
								if (ImGui::Button("Set Value"))
								{
									memset(convarNVal, 0, sizeof convarNVal);
								}
								ImGui::Columns(1, nullptr, false);
							}
							ImGui::EndGroup();

							ImGui::Text("Name: N/A");
							ImGui::Text("Current Value: N/A");
							ImGui::Text("Default Value: N/A");
							ImGui::Text("Is Safe: N/A");
						}
					}
					ImGui::EndGroupBox();
					ImGui::Text("Clantag Scroll");
					ImGui::BeginGroupBox("ClantagScroll", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Enabled", &Settings::Misc::ClantagScroller::Enabled); ImGui::NextColumn();
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGuiEx::InputText("Clantag", &Settings::Misc::ClantagScroller::Clantag); ImGui::NextColumn();
							if (ImGui::Button("Refresh"))
							{
								ClantagScroller::Get().tag = Settings::Misc::ClantagScroller::Clantag;
								ClantagScroller::Get().tag += " ";
							}
							ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::SliderInt("Delay", &Settings::Misc::ClantagScroller::Delay, 250, 1000);
					}
					ImGui::EndGroupBox();
					ImGui::NextColumn();
					ImGui::Text("Combat");
					ImGui::BeginGroupBox("MiscCombat", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Auto Pistol", &Settings::Misc::AutoPistol);
						ImGui::Checkbox("Auto Reload", &Settings::Misc::AutoReload);

						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Feedback Hitsound", &Settings::Misc::Sounds::HitSound::Enabled); ImGui::NextColumn();
							ImGui::Combo("Sound##Hitsound", &Settings::Misc::Sounds::HitSound::Sound, "Metal\0Arena Switch\0Timer Bell\0Glass"); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroupBox();
					ImGui::Text("Visuals");
					ImGui::BeginGroupBox("MiscVisuals", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Rank Reveal", &Settings::Misc::RankReveal);
						ImGui::Checkbox("Recoil Crosshair", &Settings::Misc::RecoilCrosshair);
						ImGui::Checkbox("Sniper Crosshair", &Settings::Misc::SniperCrosshair);
						ImGui::Checkbox("Spectator List", &Settings::Misc::SpectatorList);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);

							ImGui::Checkbox("Adjust Brightness", &Settings::Misc::Brightness::Enabled); ImGui::NextColumn();
							ImGui::SliderInt("%##Brightness", &Settings::Misc::Brightness::Perc, 1, 100); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Checkbox("No Decals", &Settings::Misc::NoDecals);
						ImGui::Checkbox("Free Look", &Settings::Misc::FreeLook::Enabled);
						ImGui::Checkbox("Fake Zoom", &Settings::Misc::FakeZoom::Enabled);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("World Colour", &Settings::Misc::WorldColour::Enabled); ImGui::NextColumn();
							ImGuiEx::ColorEdit4("Colour##World", &Settings::Misc::WorldColour::Colour); ImGui::NextColumn();

							ImGui::Checkbox("Camera FOV", &Settings::Misc::CameraFOV::Enabled); ImGui::NextColumn();
							ImGui::SliderInt("CameraFOVInt", &Settings::Misc::CameraFOV::FOV, 0, 70); ImGui::NextColumn();

							ImGui::Checkbox("Viewmodel FOV", &Settings::Misc::ViewmodelFOV::Enabled); ImGui::NextColumn();
							ImGui::SliderInt("FOV##ViewmodelFOVInt", &Settings::Misc::ViewmodelFOV::FOV, 60, 160); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroupBox();
					ImGui::Text("Others");
					ImGui::BeginGroupBox("MiscOthers", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Infinite Duck", &Settings::Misc::InfiniteDuck);
						ImGui::Checkbox("Auto Defuse", &Settings::Misc::AutoDefuse::Enabled);
						ImGui::Checkbox("Radar Hack", &Settings::Misc::RadarHack);
						ImGui::Checkbox("Bullet Impacts", &Settings::Misc::BulletImpacts);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Slow Walk", &Settings::Misc::SlowWalk::Enabled); ImGui::NextColumn();
							ImGui::SliderInt("Amount##SlowWalkAmount", &Settings::Misc::SlowWalk::Amount, 0, 100); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Checkbox("Auto Accept", &Settings::Misc::AutoAccept);
						ImGui::Checkbox("Anti Kick", &Settings::Misc::AntiKick);
						ImGui::Checkbox("Translate Bot", &Settings::Misc::TranslateBot);
						ImGui::Checkbox("Fake Duck", &Settings::Misc::FakeDuck::Enabled);
						ImGui::Checkbox("Self Nade helper", &Settings::Misc::SelfNade);
					}
					ImGui::EndGroupBox();
					ImGui::Text("Fake Lag");
					ImGui::BeginGroupBox("Fake Lag", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Columns(2, nullptr, false);
						ImGui::Checkbox("Enabled", &Settings::Misc::FakeLag::Enabled); ImGui::NextColumn();
						ImGui::SliderInt("Choke", &Settings::Misc::FakeLag::Choke, 0, 62); ImGui::NextColumn();
						ImGui::Columns(1, nullptr, false);
					}
					ImGui::EndGroupBox();
					break;
				case 1:
					ImGui::Text("Fun");
					ImGui::BeginGroupBox("MiscFun", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Inverse Ragdoll Gravity", &Settings::Misc::InverseRagdollGravity);
						ImGui::Checkbox("Chicken Tamer", &Settings::Misc::ChickenTamer);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Chat Mimic", &Settings::Misc::ChatMimic::Enabled); ImGui::NextColumn();
							ImGui::Checkbox("##CustomMessageEnabled", &Settings::Misc::ChatMimic::CustomMsg); ImGui::SameLine();
							ImGui::InputText("Message", Settings::Misc::ChatMimic::Msg, 256);
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Chat Spammer##CS", &Settings::Misc::ChatSpammer::Enabled); ImGui::NextColumn();
							static std::shared_ptr<pfd::open_file> open_file;
							if (ImGui::Button("Choose File"))
							{
								std::vector<std::string> filters = { "*.txt" };
								open_file = std::make_shared<pfd::open_file>("Select Chatspammer File", "", filters, false);
							}
							if (open_file && open_file->ready())
							{
								Settings::Misc::ChatSpammer::Filename = open_file->result()[0];

								std::ifstream fin(Settings::Misc::ChatSpammer::Filename.c_str(), std::ios::binary);
								std::stringstream ss;
								ss << fin.rdbuf();

								ChatSpammer::Get().filedata = ss.str();

								fin.close();

								Config::Get().SaveUser();
							}
							ImGui::Columns(1, nullptr, false);
							ImGui::Text("Selected: "); ImGui::SameLine();
							ImGui::Text(Settings::Misc::ChatSpammer::Filename.c_str());
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroupBox();
					break;
				}
				ImGui::Columns(1, nullptr, false);
				break;
			}
			case 1:
			{
				static int vstabselected = 0;

				ImGui::BeginGroup();
				{
					bool vssel = vstabselected == 0;
					if (ImGui::ToggleButton("Players##VisualsTab", &vssel, ImVec2(ImGui::GetWindowSize().x / 4, 20)))
					{
						vstabselected = 0;
					}
					vssel = vstabselected == 1;
					ImGui::SameLine(0, 0);
					if (ImGui::ToggleButton("Entities##VisualsTab", &vssel, ImVec2(ImGui::GetWindowSize().x / 4, 20)))
					{
						vstabselected = 1;
					}
					vssel = vstabselected == 2;
					ImGui::SameLine(0, 0);
					if (ImGui::ToggleButton("Viewmodel##VisualsTab", &vssel, ImVec2(ImGui::GetWindowSize().x / 4, 20)))
					{
						vstabselected = 2;
					}
					vssel = vstabselected == 3;
					ImGui::SameLine(0, 0);
					if (ImGui::ToggleButton("Others##VisualsTab", &vssel, ImVec2(ImGui::GetWindowSize().x / 4, 20)))
					{
						vstabselected = 3;
					}
				}
				ImGui::EndGroup();

				switch (vstabselected)
				{
				case 0:
					ImGui::Columns(2, nullptr, false);
					ImGui::Text("ESP");
					ImGui::BeginGroupBox("ESPPlayers", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Master", &Settings::ESP::Players::Enabled);
						static int vsespselected = 0;

						ImGui::BeginGroup();
						{
							bool vssel = vsespselected == 0;
							if (ImGui::ToggleButton("Enemy##VisualsESP", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 0;
							}
							vssel = vsespselected == 1;
							ImGui::SameLine(0, 0);
							if (ImGui::ToggleButton("Ally##VisualsESP", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 1;
							}
							vssel = vsespselected == 2;
							ImGui::SameLine(0, 0);
							if (ImGui::ToggleButton("Localplayer##VisualsESP", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 2;
							}
						}
						ImGui::EndGroup();
						ImGui::Separator();
						ImGui::BeginGroupBox("ESPAll", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							switch (vsespselected)
							{
							case 0:
								ImGui::Checkbox("Enabled##ESPEnemy", &Settings::ESP::Players::Enemies::Enabled);
								ImGui::Checkbox("Occluded##ESPEnemy", &Settings::ESP::Players::Enemies::Occluded);
								ImGui::Columns(2, nullptr, false);
								ImGui::Checkbox("Boxes##ESPEnemy", &Settings::ESP::Players::Enemies::Boxes);
								ImGui::Checkbox("Skeletons##ESPEnemy", &Settings::ESP::Players::Enemies::Skeletons);
								ImGui::Checkbox("Names##ESPEnemy", &Settings::ESP::Players::Enemies::Names);
								ImGui::Checkbox("Health##ESPEnemy", &Settings::ESP::Players::Enemies::Health);
								ImGui::Checkbox("Weapons##ESPEnemy", &Settings::ESP::Players::Enemies::Weapons);
								ImGui::Checkbox("Snaplines##ESPEnemy", &Settings::ESP::Players::Enemies::Snaplines);
								ImGui::Checkbox("Positions##ESPEnemy", &Settings::ESP::Players::Enemies::Positions);
								ImGui::Checkbox("Eye Lines##ESPEnemy", &Settings::ESP::Players::Enemies::EyeLines);

								ImGui::NextColumn();

								ImGuiEx::ColorEdit4("##ESPEnemyBoxesColour", &Settings::ESP::Players::Enemies::Colours::Boxes);
								ImGuiEx::ColorEdit4("##ESPEnemySkeletonsColour", &Settings::ESP::Players::Enemies::Colours::Skeletons);
								ImGuiEx::ColorEdit4("##ESPEnemyNamesColour", &Settings::ESP::Players::Enemies::Colours::Names);
								ImGuiEx::ColorEdit4("##ESPEnemyHealthColour", &Settings::ESP::Players::Enemies::Colours::Health);
								ImGuiEx::ColorEdit4("##ESPEnemyWeaponsColour", &Settings::ESP::Players::Enemies::Colours::Weapons);
								ImGuiEx::ColorEdit4("##ESPEnemySnaplinesColour", &Settings::ESP::Players::Enemies::Colours::Snaplines);
								ImGuiEx::ColorEdit4("##ESPEnemyPositionsColour", &Settings::ESP::Players::Enemies::Colours::Positions);
								ImGuiEx::ColorEdit4("##ESPEnemyEyeLinesColour", &Settings::ESP::Players::Enemies::Colours::EyeLines);
								ImGui::Columns(1, nullptr, false);
								break;
							case 1:
								ImGui::Checkbox("Enabled##ESPTeammate", &Settings::ESP::Players::Teammates::Enabled);
								ImGui::Checkbox("Occluded##ESPTeammate", &Settings::ESP::Players::Teammates::Occluded);
								ImGui::Columns(2, nullptr, false);
								ImGui::Checkbox("Boxes##ESPTeammate", &Settings::ESP::Players::Teammates::Boxes);
								ImGui::Checkbox("Skeletons##ESPTeammate", &Settings::ESP::Players::Teammates::Skeletons);
								ImGui::Checkbox("Names##ESPTeammates", &Settings::ESP::Players::Teammates::Names);
								ImGui::Checkbox("Health##ESPTeammate", &Settings::ESP::Players::Teammates::Health);
								ImGui::Checkbox("Weapons##ESPTeammate", &Settings::ESP::Players::Teammates::Weapons);
								ImGui::Checkbox("Snaplines##ESPTeammate", &Settings::ESP::Players::Teammates::Snaplines);
								ImGui::Checkbox("Positions##ESPTeammate", &Settings::ESP::Players::Teammates::Positions);
								ImGui::Checkbox("Eye Lines##ESPTeammate", &Settings::ESP::Players::Teammates::EyeLines);

								ImGui::NextColumn();

								ImGuiEx::ColorEdit4("##ESPTeammateBoxesColour", &Settings::ESP::Players::Teammates::Colours::Boxes);
								ImGuiEx::ColorEdit4("##ESPTeammateSkeletonsColour", &Settings::ESP::Players::Teammates::Colours::Skeletons);
								ImGuiEx::ColorEdit4("##ESPTeammateNamesColour", &Settings::ESP::Players::Teammates::Colours::Names);
								ImGuiEx::ColorEdit4("##ESPTeammateHealthColour", &Settings::ESP::Players::Teammates::Colours::Health);
								ImGuiEx::ColorEdit4("##ESPTeammateWeaponsColour", &Settings::ESP::Players::Teammates::Colours::Weapons);
								ImGuiEx::ColorEdit4("##ESPTeammateSnaplinesColour", &Settings::ESP::Players::Teammates::Colours::Snaplines);
								ImGuiEx::ColorEdit4("##ESPTeammatePositionsColour", &Settings::ESP::Players::Teammates::Colours::Positions);
								ImGuiEx::ColorEdit4("##ESPTeammatEyeLinesColour", &Settings::ESP::Players::Teammates::Colours::EyeLines);
								ImGui::Columns(1, nullptr, false);
								break;
							case 2:
								ImGui::Checkbox("Enabled##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Enabled);
								ImGui::Checkbox("Occluded##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Occluded);
								ImGui::Columns(2, nullptr, false);
								ImGui::Checkbox("Boxes##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Boxes);
								ImGui::Checkbox("Skeletons##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Skeletons);
								ImGui::Checkbox("Name##ESPLocalplayers", &Settings::ESP::Players::Localplayer::Names);
								ImGui::Checkbox("Health##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Health);
								ImGui::Checkbox("Weapons##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Weapons);
								ImGui::Checkbox("Positions##ESPLocalplayer", &Settings::ESP::Players::Localplayer::Positions);

								ImGui::NextColumn();

								ImGuiEx::ColorEdit4("##ESPLocalplayerBoxesColour", &Settings::ESP::Players::Localplayer::Colours::Boxes);
								ImGuiEx::ColorEdit4("##ESPLocalplayerSkeletonsColour", &Settings::ESP::Players::Localplayer::Colours::Skeletons);
								ImGuiEx::ColorEdit4("##ESPLocalplayerNamesColour", &Settings::ESP::Players::Localplayer::Colours::Names);
								ImGuiEx::ColorEdit4("##ESPLocalplayerHealthColour", &Settings::ESP::Players::Localplayer::Colours::Health);
								ImGuiEx::ColorEdit4("##ESPLocalplayerWeaponsColour", &Settings::ESP::Players::Localplayer::Colours::Weapons);
								ImGuiEx::ColorEdit4("##ESPLocalplayerPositionsColour", &Settings::ESP::Players::Localplayer::Colours::Positions);
								ImGui::Columns(1, nullptr, false);
								break;
							}
						}
						ImGui::EndGroupBox();
					}
					ImGui::EndGroupBox();
					ImGui::NextColumn();
					ImGui::Text("Chams");
					ImGui::BeginGroupBox("ChamsPlayers", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Master", &Settings::Visuals::Chams::Players::Enabled);
						static int vsespselected = 0;

						ImGui::BeginGroup();
						{
							bool vssel = vsespselected == 0;
							if (ImGui::ToggleButton("Enemy##VisualsChams", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 0;
							}
							vssel = vsespselected == 1;
							ImGui::SameLine(0, 0);
							if (ImGui::ToggleButton("Ally##VisualsChams", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 1;
							}
							vssel = vsespselected == 2;
							ImGui::SameLine(0, 0);
							if (ImGui::ToggleButton("Localplayer##VisualsChams", &vssel, ImVec2(ImGui::GetWindowSize().x / 3, 20)))
							{
								vsespselected = 2;
							}
						}
						ImGui::EndGroup();
						ImGui::BeginGroupBox("VisualsChamsAll", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							switch (vsespselected)
							{
							case 0:
								ImGui::Checkbox("Enabled", &Settings::Visuals::Chams::Players::Enemies::Enabled);
								ImGui::Checkbox("Occluded", &Settings::Visuals::Chams::Players::Enemies::DoOccluded);
								ImGui::Combo("Material", &Settings::Visuals::Chams::Players::Enemies::Material, "Regular\0Flat\0Glossy");
								ImGuiEx::ColorEdit3("Visible", &Settings::Visuals::Chams::Players::Enemies::Visible);
								ImGuiEx::ColorEdit3("Occluded", &Settings::Visuals::Chams::Players::Enemies::Occluded);
								break;
							case 1:
								ImGui::Checkbox("Enabled", &Settings::Visuals::Chams::Players::Teammates::Enabled);
								ImGui::Checkbox("Occluded", &Settings::Visuals::Chams::Players::Teammates::DoOccluded);
								ImGui::Combo("Material", &Settings::Visuals::Chams::Players::Teammates::Material, "Regular\0Flat\0Glossy");
								ImGuiEx::ColorEdit3("Visible", &Settings::Visuals::Chams::Players::Teammates::Visible);
								ImGuiEx::ColorEdit3("Occluded", &Settings::Visuals::Chams::Players::Teammates::Occluded);
								break;
							case 2:
								ImGui::Checkbox("Enabled", &Settings::Visuals::Chams::Players::Localplayer::Enabled);
								ImGui::Checkbox("Occluded", &Settings::Visuals::Chams::Players::Localplayer::DoOccluded);
								ImGui::Combo("Material", &Settings::Visuals::Chams::Players::Localplayer::Material, "Regular\0Flat\0Glossy");
								ImGuiEx::ColorEdit3("Visible", &Settings::Visuals::Chams::Players::Localplayer::Visible);
								ImGuiEx::ColorEdit3("Occluded", &Settings::Visuals::Chams::Players::Localplayer::Occluded);
								break;
							}
						}
						ImGui::EndGroupBox();
					}
					ImGui::EndGroupBox();
					ImGui::BeginGroupBox("GlowPlayers", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						static int vsglowselected = 0;

						ImGui::BeginGroup();
						{
							bool vssel = vsglowselected == 0;
							if (ImGui::ToggleButton("Enemy##VisualsGlow", &vssel, ImVec2(ImGui::GetWindowSize().x / 2, 20)))
							{
								vsglowselected = 0;
							}
							vssel = vsglowselected == 1;
							ImGui::SameLine(0, 0);
							if (ImGui::ToggleButton("Ally##VisualsGlow", &vssel, ImVec2(ImGui::GetWindowSize().x / 2, 20)))
							{
								vsglowselected = 1;
							}
						}
						ImGui::EndGroup();
						ImGui::BeginGroupBox("VisualsGlowAll", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							switch (vsglowselected)
							{
							case 0:
								ImGui::Checkbox("Enabled##VisualsGlowEnemies", &Settings::ESP::Glow::Players::Enemies::Enabled);
								//ImGui::Checkbox("Cover##VisualsGlowEnemies", &Settings::ESP::Glow::Players::Enemies::Cover);
								ImGui::Combo("Style##VisualsGlowEnemies", &Settings::ESP::Glow::Players::Enemies::Style, "Full Body\0Inline Flicker\0Inline Glow\0Flicker");
								ImGuiEx::ColorEdit3("Colour##VisualsGlowEnemies", &Settings::ESP::Glow::Players::Enemies::Colour);
								break;
							case 1:
								ImGui::Checkbox("Enabled##VisualsGlowTeammates", &Settings::ESP::Glow::Players::Teammates::Enabled);
								//ImGui::Checkbox("Cover##VisualsGlowTeammates", &Settings::ESP::Glow::Players::Teammates::Cover);
								ImGui::Combo("Style##VisualsGlowTeammates", &Settings::ESP::Glow::Players::Teammates::Style, "Full Body\0Inline Flicker\0Inline Glow\0Flicker");
								ImGuiEx::ColorEdit3("Colour##VisualsGlowTeammates", &Settings::ESP::Glow::Players::Teammates::Colour);
								break;
							}
						}
						ImGui::EndGroupBox();
					}
					ImGui::EndGroupBox();
					ImGui::Columns(1, nullptr, false);
					break;
				case 1:
					ImGui::Columns(2, nullptr, false);
					ImGui::Text("Other ESP");
					ImGui::BeginGroupBox("OthersESP", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Columns(2, nullptr, false);
						ImGui::Checkbox("Grenades", &Settings::ESP::Others::Grenades::Enabled); ImGui::NextColumn();
						ImGuiEx::ColorEdit3("Colour##Grenades", &Settings::ESP::Others::Grenades::Colour); ImGui::NextColumn();

						ImGui::Checkbox("C4", &Settings::ESP::Others::C4::Enabled); ImGui::NextColumn();
						ImGuiEx::ColorEdit3("Colour##C4", &Settings::ESP::Others::C4::Colour); ImGui::NextColumn();
						ImGui::Columns(1, nullptr, false);
					}
					ImGui::EndGroupBox();
					ImGui::Columns(1, nullptr, false);
					break;
				case 2:
					ImGui::BeginGroupBox("Skin Changer", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Enabled", &Settings::SkinChanger::Enabled);

						if (ImGui::Button("Force Update"))
						{
							shouldupdate = true;
						}

						static int wepsel = 0;

						ImGui::BeginGroupBox("Weapon Selector", ImVec2(0, 350));
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Text("Weapon");
							ImGui::NextColumn();
							ImGui::Text("Skin");
							ImGui::NextColumn();
							RenderWeapon(&wepsel, 0, "Knife");
							RenderWeapon(&wepsel, 1, "P200");
							RenderWeapon(&wepsel, 2, "USP-S");
							RenderWeapon(&wepsel, 3, "Glock");
							RenderWeapon(&wepsel, 4, "P250");
							RenderWeapon(&wepsel, 5, "FiveSeven");
							RenderWeapon(&wepsel, 6, "Tec9");
							RenderWeapon(&wepsel, 7, "CZ75A");
							RenderWeapon(&wepsel, 8, "Dualies");
							RenderWeapon(&wepsel, 9, "Deagle");
							RenderWeapon(&wepsel, 10, "Revolver");
							RenderWeapon(&wepsel, 11, "Famas");
							RenderWeapon(&wepsel, 12, "Galil");
							RenderWeapon(&wepsel, 13, "M4A4");
							RenderWeapon(&wepsel, 14, "AK47");
							RenderWeapon(&wepsel, 15, "SG553");
							RenderWeapon(&wepsel, 16, "AUG");
							RenderWeapon(&wepsel, 17, "SSG08");
							RenderWeapon(&wepsel, 18, "AWP");
							RenderWeapon(&wepsel, 19, "Scar20");
							RenderWeapon(&wepsel, 20, "G3SG1");
							RenderWeapon(&wepsel, 21, "MAC10");
							RenderWeapon(&wepsel, 22, "MP5-SD");
							RenderWeapon(&wepsel, 23, "MP7");
							RenderWeapon(&wepsel, 24, "MP9");
							RenderWeapon(&wepsel, 25, "PPBizon");
							RenderWeapon(&wepsel, 26, "P90");
							RenderWeapon(&wepsel, 27, "UMP45");
							RenderWeapon(&wepsel, 28, "MAG7");
							RenderWeapon(&wepsel, 29, "Nova");
							RenderWeapon(&wepsel, 30, "Sawed Off");
							RenderWeapon(&wepsel, 31, "XM1014");
							RenderWeapon(&wepsel, 32, "M249");
							RenderWeapon(&wepsel, 33, "Negev");
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroupBox();
						
						ImGui::BeginGroupBox("Skin Action", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							switch (wepsel)
							{
							case 0:
								ImGui::Combo("Knife Model", &Settings::SkinChanger::KnifeModel, "Default\0Bayonet\0M9\0Karambit\0Bowie\0Butterfly\0Falchion\0Flip\0Gut\0Huntsman\0Shaddow daggers\0Navaja\0Stiletto\0Talon\0Ursus");
								ImGui::Combo("Skin", allskins[0], [](void* data, int idx, const char** out_text)
									{
										*out_text = parser_skins[idx].name.c_str();
										return true;
									}, nullptr, parser_skins.size(), 10);
								ImGui::SliderFloat("Knife Wear", &Settings::SkinChanger::KnifeWear, 0.f, 1.f, "%.2f");
								break;
							default:
								ImGui::Combo("Skin", allskins[wepsel], [](void* data, int idx, const char** out_text)
								{
									*out_text = parser_skins[idx].name.c_str();
									return true;
								}, nullptr, parser_skins.size(), 10);

								break;
							}
						}
						ImGui::EndGroupBox();

						ImGui::Checkbox("Force deagle spin", &Settings::Misc::DeagleSpinner);
					}
					ImGui::EndGroupBox();
					ImGui::Columns(2, nullptr, false);
					ImGui::Checkbox("Arms##ArmsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Arms::Enabled); ImGui::SameLine();
					ImGui::Checkbox("Wireframe##ArmsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Arms::Wireframe); ImGui::NextColumn();
					ImGuiEx::ColorEdit3("Colour##ArmsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Arms::Colour); ImGui::SameLine();
					ImGui::Combo("Material##ArmsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Arms::Material, "Regular\0Flat\0Glossy"); ImGui::NextColumn();

					ImGui::Checkbox("Sleeves##SleevesVisualsChams", &Settings::Visuals::Chams::Viewmodel::Sleeves::Enabled); ImGui::SameLine();
					ImGui::Checkbox("Wireframe##SleevesVisualsChams", &Settings::Visuals::Chams::Viewmodel::Sleeves::Wireframe);  ImGui::NextColumn();
					ImGuiEx::ColorEdit3("Colour##SleevesVisualsChams", &Settings::Visuals::Chams::Viewmodel::Sleeves::Colour); ImGui::SameLine();
					ImGui::Combo("Material##SleevesVisualsChams", &Settings::Visuals::Chams::Viewmodel::Sleeves::Material, "Regular\0Flat\0Glossy"); ImGui::NextColumn();

					ImGui::Checkbox("Weapons##WeaponsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Weapons::Enabled); ImGui::SameLine();
					ImGui::Checkbox("Wireframe##WeaponsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe);  ImGui::NextColumn();
					ImGuiEx::ColorEdit3("Colour##WeaponsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Weapons::Colour); ImGui::SameLine();
					ImGui::Combo("Material##WeaponsVisualsChams", &Settings::Visuals::Chams::Viewmodel::Weapons::Material, "Regular\0Flat\0Glossy"); ImGui::NextColumn();
					ImGui::Columns(1, nullptr, false);
					break;
				case 3:
					ImGui::Columns(2, nullptr, false);
					ImGui::Text("Removals");
					ImGui::BeginGroupBox("MiscRemovals", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Flash", &Settings::Misc::Removals::Flash);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::PushItemWidth(100);
							ImGui::Checkbox("Smoke", &Settings::Misc::Removals::Smoke); ImGui::NextColumn();
							ImGui::Combo("Type", &Settings::Misc::Removals::SmokeType, "Wireframe\0Remove"); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Checkbox("Smoke Overlay", &Settings::Misc::Removals::SmokeOverlay);
						ImGui::Checkbox("Scope", &Settings::Misc::Removals::Scope);
						ImGui::Checkbox("Scope Blur", &Settings::Misc::Removals::ScopeBlur);
						ImGui::Checkbox("Post Processing", &Settings::Misc::Removals::PostProcessing);
						ImGui::Checkbox("Recoil Shake", &Settings::Misc::Removals::RecoilShake);
						ImGui::Checkbox("Arms", &Settings::Misc::Removals::Arms);
						ImGui::Checkbox("Sleeves", &Settings::Misc::Removals::Sleeves);
						ImGui::Checkbox("Weapon", &Settings::Misc::Removals::Weapon);
						ImGui::Checkbox("3D Sky", &Settings::Misc::Removals::Sky3D);
						ImGui::Checkbox("Shadows", &Settings::Misc::Removals::Shadows);
					}
					ImGui::EndGroupBox();
					ImGui::BeginGroupBox("GrenadePrediction", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Grenade Prediction", &Settings::ESP::GrenadePrediction);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Show Bullet Tracers", &Settings::Visuals::BulletShots::Enabled); ImGui::NextColumn();
							ImGuiEx::ColorEdit3("Colour", &Settings::Visuals::BulletShots::Colour);
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Checkbox("Wallbang Crosshair", &Settings::Misc::WallbangCrosshair);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Smoke Assist", &Settings::Misc::SmokeHelper::Enabled); ImGui::NextColumn();
							ImGui::Combo("Type", &Settings::Misc::SmokeHelper::TypeHelp, "Visuals\0Visuals + Assist\0Visuals + Aimbot\0Visuals + Smoke Bot");
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroupBox();
					ImGui::Columns(1, nullptr, false);
					break;
				}
				break;
			}
			case 2:
				ImGui::BeginGroup();
				{
					ImGui::Text("Legit Aim");
					ImGui::BeginGroupBox("LegitAim", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Text("RCS");
						ImGui::BeginGroupBox("LegitAimRCS", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							ImGui::Checkbox("Enabled##RCS", &Settings::Aim::RCS::Enabled);
							ImGui::Checkbox("Silent##RCS", &Settings::Aim::RCS::Silent);
							ImGui::SliderFloat("ScaleX", &Settings::Aim::RCS::ScaleX, 0, 2, "%.2f");
							ImGui::SliderFloat("ScaleY", &Settings::Aim::RCS::ScaleY, 0, 2, "%.2f");
						}
						ImGui::EndGroupBox();
						ImGui::Text("Triggerbot");
						ImGui::BeginGroupBox("LegitAimTriggerbot", ImVec2(0, -ImGui::GetContentRegionAvail().y));
						{
							ImGui::Checkbox("Enabled##Triggerbot", &Settings::Misc::Triggerbot::Enabled);
							ImGui::SliderInt("Delay##Triggerbot", &Settings::Misc::Triggerbot::Delay, 0, 64);
						}
						ImGui::EndGroupBox();
					}
					ImGui::EndGroupBox();
					ImGui::Text("Test Aimbot");
					ImGui::BeginGroupBox("TestAimbot", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						ImGui::Checkbox("Enabled##TestAimbot", &Settings::Aim::TestAimbot::Enabled);
						ImGui::Checkbox("OnShoot##TestAimbot", &Settings::Aim::TestAimbot::OnShoot);
						ImGui::Checkbox("AutoShoot##TestAimbot", &Settings::Aim::TestAimbot::AutoShoot);
						ImGui::BeginGroup();
						{
							ImGui::Columns(2, nullptr, false);
							ImGui::Checkbox("Silent##TestAimbot", &Settings::Aim::TestAimbot::Silent); ImGui::NextColumn();
							ImGui::Checkbox("Viewmodel Move##TestAimbot", &Settings::Aim::TestAimbot::WeaponMove);
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
						ImGui::Combo("Target Priority##TestAimbot", &Settings::Aim::TestAimbot::TargetPriority, "FOV\0Damage\0Closest\0Smart");
					}
					ImGui::EndGroupBox();
				}
				ImGui::EndGroup();
				ImGui::Text("Weapons");
				ImGui::BeginGroupBox("Weapons##Aimbot", ImVec2(0, -ImGui::GetContentRegionAvail().y));
				{
					static int weaponsselected = 0;

					ImGui::BeginGroup();
					{
						bool vssel = weaponsselected == 0;
						if (ImGui::ToggleButton("Scout##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 0;
						}
						vssel = weaponsselected == 1;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Awp##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 1;
						}
						vssel = weaponsselected == 2;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Auto##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 2;
						}
						vssel = weaponsselected == 3;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Rifle##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 3;
						}
						vssel = weaponsselected == 4;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Pistol##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 4;
						}
						vssel = weaponsselected == 5;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Shotgun##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 5;
						}
						vssel = weaponsselected == 6;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("SMG##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 6;
						}
						vssel = weaponsselected == 7;
						ImGui::SameLine(0, 0);
						if (ImGui::ToggleButton("Other##WeaponsAimbot", &vssel, ImVec2(ImGui::GetWindowSize().x / 8, 20)))
						{
							weaponsselected = 7;
						}
					}
					ImGui::EndGroup();
					ImGui::Separator();
					switch (weaponsselected)
					{
					case 0:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Scout::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Scout::AutoCrouch);
						ImGui::Checkbox("Auto AutoScope", &Settings::Aim::TestAimbot::Scout::AutoScope);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Scout::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Scout::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Scout::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Scout::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 1:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Awp::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Awp::AutoCrouch);
						ImGui::Checkbox("Auto AutoScope", &Settings::Aim::TestAimbot::Awp::AutoScope);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Awp::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Awp::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Awp::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Awp::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 2:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Auto::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Auto::AutoCrouch);
						ImGui::Checkbox("Auto AutoScope", &Settings::Aim::TestAimbot::Auto::AutoScope);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Auto::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Auto::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Scout::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Auto::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 3:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Rifle::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Rifle::AutoCrouch);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Rifle::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Rifle::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Rifle::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Rifle::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 4:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Pistol::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Pistol::AutoCrouch);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Pistol::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Pistol::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Pistol::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Pistol::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 5:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Shotgun::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Shotgun::AutoCrouch);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Shotgun::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Shotgun::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Shotgun::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Shotgun::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 6:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::SMG::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::SMG::AutoCrouch);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::SMG::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::SMG::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::SMG::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::SMG::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					case 7:
						ImGui::Checkbox("Auto Stop", &Settings::Aim::TestAimbot::Other::AutoStop);
						ImGui::Checkbox("Auto Crouch", &Settings::Aim::TestAimbot::Other::AutoCrouch);
						ImGui::SliderInt("Min Damage", &Settings::Aim::TestAimbot::Other::MinDamage, 0, 100);
						ImGui::SliderFloat("Hit Chance", &Settings::Aim::TestAimbot::Other::HitChance, 0, 99.99, "%.2f");
						ImGui::Checkbox("Prioritise Hitbox", &Settings::Aim::TestAimbot::Other::Prioritise);
						ImGui::Combo("Hitbox", &Settings::Aim::TestAimbot::Other::Prioritised, "Head\0Neck\0Pelvis\0Spine\0Legs\0Arms");
						break;
					}
				}
				ImGui::EndGroupBox();
				ImGui::Columns(1, nullptr, false);
				break;
			case 3:
				ImGui::Text("Pitch");
				ImGui::BeginGroupBox("AntiAimPitchSettings", ImVec2(0, -ImGui::GetContentRegionAvail().y));
				{
					ImGui::PushItemWidth(100);
					ImGui::Combo("Pitch", &Settings::AntiAim::Pitch::Type, "None\0Down\0Up\0Switch");
					ImGui::PopItemWidth();
				}
				ImGui::EndGroupBox();
				ImGui::Text("Yaw");
				ImGui::BeginGroupBox("AntiAimYawSettings", ImVec2(0, -ImGui::GetContentRegionAvail().y));
				{
					ImGui::PushItemWidth(100);
					ImGui::Combo("Yaw", &Settings::AntiAim::Yaw::Type, "None\0Backwards\0Manual\0Desync\0Custom");
					switch (Settings::AntiAim::Yaw::Type)
					{
					case 2:
						ImGui::Separator();
						ImGui::Text("Left Key: ");ImGui::SameLine();ImGui::Hotkey("Left Key", &Settings::AntiAim::Yaw::LKey);
						ImGui::Text("Right Key: "); ImGui::SameLine();ImGui::Hotkey("Right Key", &Settings::AntiAim::Yaw::RKey);
						break;
					case 4:
						ImGui::Separator();
						ImGui::SliderInt("Angle", &Settings::AntiAim::Yaw::Angle, -180, 180);
						break;
					}
					ImGui::SliderInt("Jitter", &Settings::AntiAim::Yaw::Jitter, 0, 45);
					ImGui::PopItemWidth();
				}
				ImGui::EndGroupBox();
				break;
			case 4:
				static char configname[32] = "default";
				ImGui::InputText("Config Name", configname, 32);
				if (ImGui::Button("Save"))
				{
					Config::Get().Save(configname);
				}
				ImGui::SameLine();
				if (ImGui::Button("Load"))
				{
					Config::Get().Load(configname);
				}
				ImGuiEx::ColorEdit3("System Theme", &Settings::System::Theme);
				ImGuiEx::ColorEdit3("Text Colour", &Settings::System::TextTheme);
				ImGuiEx::ColorEdit3("Window Colour", &Settings::System::WindowTheme);
				ImGui::Text("Keybinds");
				ImGui::BeginGroupBox("KeyBinds##KeyBindsGroupInit", ImVec2(0, -ImGui::GetContentRegionAvail().y));
				{
					if (ImGui::Checkbox("Enable ChimpHook chat notifications?", &Settings::System::ChimpHookNotifications))
						Config::Get().SaveUser();

					ImGui::Columns(2, nullptr, false);
					static int selectedKb;

					ImGui::BeginGroupBox("KeyBindsList", ImVec2(0, 200));
					{
						ImGui::BeginGroup();
						{
							ImGui::Columns(3, nullptr, false);
							ImGui::Text("Name"); ImGui::NextColumn();
							ImGui::Text("Key"); ImGui::NextColumn();
							ImGui::Text("Type"); ImGui::NextColumn();
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();

						ImGui::BeginGroup();
						{
							ImGui::Columns(3, nullptr, false);
							if (ImGui::Selectable("Thirdperson", (bool)(selectedKb == 0), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 0;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::ThirdPerson::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::ThirdPerson::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("FreeLook", (bool)(selectedKb == 1), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 1;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::FreeLook::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::FreeLook::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("FakeDuck", (bool)(selectedKb == 2), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 2;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::FakeDuck::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::FakeDuck::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("AutoDefuse", (bool)(selectedKb == 3), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 3;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::AutoDefuse::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::AutoDefuse::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("FakeZoom", (bool)(selectedKb == 4), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 4;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::FakeZoom::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::FakeZoom::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("Aimbot", (bool)(selectedKb == 5), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 5;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::Aimbot::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::Aimbot::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();

							if (ImGui::Selectable("Triggerbot", (bool)(selectedKb == 6), ImGuiSelectableFlags_SpanAllColumns))
								selectedKb = 6;
							ImGui::NextColumn();
							ImGui::Text(KeyNames[Settings::KeyBinds::Triggerbot::Key]);
							ImGui::NextColumn();
							ImGui::Text((bool)(Settings::KeyBinds::Triggerbot::Type == 1) ? "Toggle" : "Hold");
							ImGui::NextColumn();
							
							ImGui::Columns(1, nullptr, false);
						}
						ImGui::EndGroup();
					}
					ImGui::EndGroupBox();
					ImGui::NextColumn();
					ImGui::BeginGroupBox("KeyBindsControl", ImVec2(0, -ImGui::GetContentRegionAvail().y));
					{
						switch (selectedKb)
						{
						case 0:
							if (ImGui::Checkbox("Enabled##ThirdPerson", &Settings::KeyBinds::ThirdPerson::Enabled))
								Config::Get().SaveUser();

							if (ImGui::Combo("Type##ThirdPerson", &Settings::KeyBinds::ThirdPerson::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();

							if (ImGui::Hotkey("KeyInput##ThirdPerson", &Settings::KeyBinds::ThirdPerson::Key))
								Config::Get().SaveUser();
							break;
						case 1:
							if (ImGui::Combo("Type##FreeLook", &Settings::KeyBinds::FreeLook::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();

							if (ImGui::Hotkey("KeyInput##FreeLook", &Settings::KeyBinds::FreeLook::Key))
								Config::Get().SaveUser();
							break;
						case 2:
							if (ImGui::Combo("Type##FakeDuck", &Settings::KeyBinds::FakeDuck::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();

							if (ImGui::Hotkey("KeyInput##FakeDuck", &Settings::KeyBinds::FakeDuck::Key))
								Config::Get().SaveUser();
							break;
						case 3:
							if (ImGui::Combo("Type##AutoDefuse", &Settings::KeyBinds::AutoDefuse::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();
							
							if (ImGui::Hotkey("KeyInput##AutoDefuse", &Settings::KeyBinds::AutoDefuse::Key))
								Config::Get().SaveUser();
							break;
						case 4:
							if (ImGui::Combo("Type##FakeZoom", &Settings::KeyBinds::FakeZoom::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();
							
							if (ImGui::Hotkey("KeyInput##FakeZoom", &Settings::KeyBinds::FakeZoom::Key))
								Config::Get().SaveUser();
							break;
						case 5:
							if (ImGui::Checkbox("Enabled##Aimbot", &Settings::KeyBinds::Aimbot::Enabled))
								Config::Get().SaveUser();

							if (ImGui::Combo("Type##Aimbot", &Settings::KeyBinds::Aimbot::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();
							
							if (ImGui::Hotkey("KeyInput##Aimbot", &Settings::KeyBinds::Aimbot::Key))
								Config::Get().SaveUser();
							break;
						case 6:
							if (ImGui::Combo("Type##Triggerbot", &Settings::KeyBinds::Triggerbot::Type, "Hold\0Toggle"))
								Config::Get().SaveUser();

							ImGui::Text("Key"); ImGui::SameLine();

							if (ImGui::Hotkey("KeyInput##Triggerbot", &Settings::KeyBinds::Triggerbot::Key))
								Config::Get().SaveUser();
							break;
						}
					}
					ImGui::EndGroupBox();
					ImGui::Columns(1, nullptr, false);
				}
				ImGui::EndGroupBox();
				ImGui::Checkbox("FUCK", &Settings::Misc::FUCK);
				ImGui::Checkbox("Debug Mode", &Settings::Misc::DebugMode);

				if (ImGui::Button("Unload"))
					Settings::System::Unload = true;

				break;


			// i wont push to you directly I am also gay, just like you
			}
		}
		ImGui::EndGroupBox();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2{ 750, 0 }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 800, 600 }, ImGuiSetCond_Once);

	static std::string note;

	if (ImGui::Begin("Player List",
		&_visible,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		ImGui::BeginGroupBox("PlayerListBody", ImVec2(0, 0));
		{
			static int tabSelected = 0;

			bool thisSel = tabSelected == 0;
			if (ImGui::ToggleButton("List", &thisSel, ImVec2(ImGui::GetWindowSize().x / 2 - 1, 20)))
			{
				tabSelected = 0;
			}
			thisSel = tabSelected == 1;
			ImGui::SameLine(0, 0);
			if (ImGui::ToggleButton("Records", &thisSel, ImVec2(ImGui::GetWindowSize().x / 2 - 1, 20)))
			{
				tabSelected = 1;
			}

			switch (tabSelected)
			{
			case 0:
				ImGui::Checkbox("Localplayer", &Settings::System::PlayerList::ShowLocalplayer); ImGui::SameLine();
				ImGui::Checkbox("Enemies", &Settings::System::PlayerList::ShowEnemies); ImGui::SameLine();
				ImGui::Checkbox("Bots", &Settings::System::PlayerList::ShowBots);
				ImGui::Checkbox("Name Stealer", &Settings::System::PlayerList::NameStealer::Enabled); ImGui::SameLine();
				ImGui::Checkbox("Enemies##NS", &Settings::System::PlayerList::NameStealer::Enemies); ImGui::SameLine();
				ImGui::SliderInt("Delay", &Settings::System::PlayerList::NameStealer::Delay, 150, 600);

				static char searcher[128];
				static char term[128];

				static int oldSel;

				if (ImGui::InputText("Search##PlayerList", searcher, 128)) {
					strcpy(term, searcher);
				}
				ImGui::SameLine();
				if (ImGui::Button("Search"))
				{
					strcpy(term, searcher);
				}

				if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_PlayerResource) {
					selectedP = -1;
				}

				if (oldSel != selectedP) {
					note = "";
					oldSel = selectedP;
				}

				ImGui::BeginGroupBox("PlayerListArray", ImVec2(0, ImGui::GetWindowSize().y / 2.25));
				{
					ImGui::BeginGroup();
					{
						ImGui::Columns(4, nullptr, false);
						ImGui::Text("#entid"); ImGui::NextColumn();
						ImGui::Text("Name"); ImGui::NextColumn();
						ImGui::Text("SteamID"); ImGui::NextColumn();
						ImGui::Text("Flags"); ImGui::NextColumn();
						ImGui::Columns(1, nullptr, false);
					}
					ImGui::EndGroup();
					ImGui::Separator();
					ImGui::BeginGroup();
					{
						if (g_LocalPlayer && g_EngineClient->IsInGame()) 
						{
							for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
							{
								C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(i);

								if (!player)
									continue;

								player_info_t info = player->GetPlayerInfo();

								if (player->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() && !Settings::System::PlayerList::ShowEnemies)
									continue;

								if ((info.fakeplayer && !Settings::System::PlayerList::ShowBots) || (info.fakeplayer && strstr(info.szName, "GOTV")))
									continue;

								if (player == g_LocalPlayer && !Settings::System::PlayerList::ShowLocalplayer)
									continue;

								if (term[0]) {
									char szName[128];
									for (int i = 0; i < 128; i++)
										szName[i] = tolower(info.szName[i]);

									char szTerm[128];
									for (int i = 0; i < 128; i++)
										szTerm[i] = tolower(term[i]);

									if (!strstr(szName, szTerm))
									{
										continue;
									}
								}

								ImGui::BeginGroup();
								{
									ImGui::Columns(4, nullptr, false);
									std::string text;
									text += std::to_string(player->EntIndex());
									if (!player->notfriend && player != g_LocalPlayer) text += "*";

									if (ImGui::Selectable(text.c_str(), (bool)(selectedP == player->EntIndex()), ImGuiSelectableFlags_SpanAllColumns)) {
										selectedP = player->EntIndex();
									}

									if (ImGui::IsItemHovered() && ImGui::GetIO().MouseClicked[1])
										selectedP = player->EntIndex();

									ImGui::NextColumn();

									if (info.szName) ImGui::Text(info.szName);
									ImGui::NextColumn();
					
									if (info.iSteamID) ImGui::Text(std::to_string(info.iSteamID).c_str());
									ImGui::NextColumn();

									std::string flags;
									if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
										flags += "Teammate ";
									else
										flags += "Enemy ";

									if (info.fakeplayer)
										flags += "Bot ";

									if (player == g_LocalPlayer)
										flags += "Localplayer ";

									ImGui::Text(flags.c_str());

									ImGui::NextColumn();
									ImGui::Columns(1, nullptr, false);

									if (ImGui::BeginPopupContextWindow("ContextMenuPlayerlist", 1))
									{
										ImGui::BeginGroupBox("##ContextMenuNigger", ImVec2(200, 100));
										{
											if (player && selectedP == player->EntIndex())
											{
												ImGui::Columns(2, nullptr, false);
												player_info_t info = player->GetPlayerInfo();
												const char* clantag = (*g_PlayerResource)->GetClan(player->EntIndex());

												if (ImGui::Button("Steal Name"))
												{
													char* endl = " ";
													std::string f;
													f += info.szName;
													f += endl;
													Utils::SetName(f.c_str());
													ImGui::CloseCurrentPopup();
												}

												if (ImGui::Button("Steal Clan"))
												{
													const char* clantag = (const char*)((*g_PlayerResource)->GetClan(player->EntIndex()));

													Utils::SetClantag(clantag);
													ImGui::CloseCurrentPopup();
												}

												if (ImGui::Button("Open Profile"))
												{
													g_SteamFriends->ActivateGameOverlayToUser("steamid", (CSteamID)(uint64)info.steamID64);

													ImGui::CloseCurrentPopup();
													_visible = false;
												}

												if (ImGui::Button("Create Record"))
												{
													player_record_t rec;
													rec.Init(info.steamID64, info.szName, Utils::GetEpochTime(), "");

													AddRecord(rec);

													tabSelected = 1;
													selectedR = info.steamID64;
													ImGui::CloseCurrentPopup();
												}
												ImGui::NextColumn();
												int AvatarImage = g_SteamFriends->GetLargeFriendAvatar((CSteamID)(uint64)info.steamID64);

												if (AvatarImage > 0)
												{
													uint32 WIDTH = 0, HEIGHT = 0;
													g_SteamUtils->GetImageSize(AvatarImage, &WIDTH, &HEIGHT);

													if (WIDTH > 0 && HEIGHT > 0)
													{
														int sz = WIDTH * HEIGHT * 4;

														uint8* dest = (uint8*)malloc(sz);

														if (dest)
														{
															if (g_SteamUtils->GetImageRGBA(AvatarImage, dest, sz))
															{
																LPDIRECT3DTEXTURE9 texture;
																D3DXCreateTexture(pDevice, WIDTH, HEIGHT, 0, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture);

																if (texture)
																{
																	CopyImageToTexture(dest, WIDTH, HEIGHT, texture, 0, 0);

																	ImGui::Image((void*)(uintptr_t)texture, ImVec2(84, 84));

																	texturesToRelease.push_back((ImTextureID)(void*)(uintptr_t)texture);
																}
															}

															free(dest);
														}
													}
												}
												ImGui::Columns(1, nullptr, false);
											}
										}
										ImGui::EndGroupBox();
										ImGui::EndPopup();
									}
								}
								ImGui::EndGroup();
							}
						}
					}
					ImGui::EndGroup();
				}
				ImGui::EndGroupBox();
				ImGui::BeginGroupBox("PlayerListOptions", ImVec2(0, 0));
				{
					ImGui::Columns(2, nullptr, false);
					ImGui::BeginGroupBox("PlayerListOptionsInfo", ImVec2(0, 0));
					{
						if (selectedP > -1) {
							C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(selectedP);

							if (player)
							{
								player_info_t info = player->GetPlayerInfo();
								const char* clantag = (*g_PlayerResource)->GetClan(player->EntIndex());
								int* rank = (*g_PlayerResource)->GetCompetitiveRanking(player->EntIndex());
								int* wins = (*g_PlayerResource)->GetCompetitiveWins(player->EntIndex());
								int ping = (int)((*g_PlayerResource)->GetPing(player->EntIndex()));

								std::string temp = "";
								temp += "EntID: ";
								temp += std::to_string(player->EntIndex()).c_str();
								ImGui::Text(temp.c_str());

								temp = "";
								temp += "Ping: ";
								temp += std::to_string(ping);
								ImGui::Text(temp.c_str());

								temp = "";
								temp  += "Name: ";
								temp += info.szName;
								ImGui::Text(temp.c_str());

								temp = "";
								temp += "Clan: ";
								temp += clantag;
								ImGui::Text(temp.c_str());

								temp = "";
								temp += "Rank: ";
								temp += CSGOMMRanks[*rank];
								ImGui::Text(temp.c_str());

								temp = "";
								temp += "Wins: ";
								temp += std::to_string(*wins);
								ImGui::Text(temp.c_str());
							}
						}
					}
					ImGui::EndGroupBox();
					ImGui::NextColumn();

					ImGui::BeginGroupBox("PlayerListOptionsActions", ImVec2(0, 0));
					{
						ImGui::Columns(2, nullptr, false);
						if (selectedP > -1)
						{
							C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(selectedP);

							if (player)
							{
								player_info_t info = player->GetPlayerInfo();
								
								if (ImGui::Button("Steal Name"))
								{
									char* endl = " ";
									std::string f;
									f += info.szName;
									f += endl;
									Utils::SetName(f.c_str());
								}

								if (ImGui::Button("Steal Clan"))
								{
									const char* clantag = (const char*)((*g_PlayerResource)->GetClan(player->EntIndex()));

									Utils::SetClantag(clantag);
								}

								if (player != g_LocalPlayer)
								{
									if (ImGui::Button("Vote Kick"))
									{
										std::string votekick;
										votekick += "callvote kick ";
										votekick += std::to_string(info.userId);
										g_EngineClient->ClientCmd(votekick.c_str());
									}

									if (player->notfriend)
									{
										if (ImGui::Button("Add Friend"))
										{
											player->notfriend = false;
										}
									}
									else
									{
										if (ImGui::Button("Remove  Friend"))
										{
											player->notfriend = true;
										}
									}
								}

								if (ImGui::Button("Open Profile"))
								{
									g_SteamFriends->ActivateGameOverlayToUser("steamid", (CSteamID)(uint64)info.steamID64);

									_visible = false;
								}

								// test

								ImGuiEx::InputText("Note", &note);
								if (ImGui::Button("Create Record"))
								{
									player_record_t rec;
									rec.Init(info.steamID64, info.szName, Utils::GetEpochTime(), note);

									AddRecord(rec);

									tabSelected = 1;
									selectedR = info.steamID64;
								}

								ImGui::NextColumn();

								int AvatarImage = g_SteamFriends->GetLargeFriendAvatar((CSteamID)(uint64)info.steamID64);

								if (AvatarImage > 0)
								{
									uint32 WIDTH = 0, HEIGHT = 0;
									g_SteamUtils->GetImageSize(AvatarImage, &WIDTH, &HEIGHT);

									if (WIDTH > 0 && HEIGHT > 0) {
										int sz = WIDTH * HEIGHT * 4; 

										uint8* dest = (uint8*)malloc(sz);

										if (dest) {
											if (g_SteamUtils->GetImageRGBA(AvatarImage, dest, sz))
											{
												LPDIRECT3DTEXTURE9 texture;
												D3DXCreateTexture(pDevice, WIDTH, HEIGHT, 0, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture);

												if (texture) {
													CopyImageToTexture(dest, WIDTH, HEIGHT, texture, 0, 0);

													ImGui::Image((void*)(uintptr_t)texture, ImVec2(128, 128));

													texturesToRelease.push_back((ImTextureID)(void*)(uintptr_t)texture);
												}
											}

											free(dest);
										}
									}
								}
							}
						}
						ImGui::Columns(1, nullptr, false);
					}
					ImGui::EndGroupBox();
					ImGui::NextColumn();
					ImGui::Columns(1, nullptr, false);
				}
				ImGui::EndGroupBox();
				break;
			case 1:
				// records

				static char searchRec[128];
				static char termRec[128];

				if (ImGui::InputText("Search##PlayerRecords", searchRec, 128))
				{
					strcpy(termRec, searchRec);
				}
				ImGui::SameLine();
				if (ImGui::Button("Search"))
				{
					strcpy(termRec, searchRec);
				}

				if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_PlayerResource)
				{
					selectedR = 0;
				}

				int realSelected = 0;
				for (int i = 0; i < g_playerRecords.size(); i++)
				{
					if (g_playerRecords.at(i).steamID64 == selectedR)
						realSelected = i;
				}

				ImGui::BeginGroupBox("PlayerRecordsArray", ImVec2(0, ImGui::GetWindowSize().y / 2));
				{
					ImGui::BeginGroup();
					{
						ImGui::Columns(4, nullptr, false);
						ImGui::Text("#recordid"); ImGui::NextColumn();
						ImGui::Text("First known name"); ImGui::NextColumn();
						ImGui::Text("SteamID64"); ImGui::NextColumn();
						ImGui::Text("Date"); ImGui::NextColumn();
						ImGui::Columns(1, nullptr, false);
					}
					ImGui::EndGroup();
					ImGui::Separator();
					ImGui::Columns(4, nullptr, false);

					for (int i = 1; i < g_playerRecords.size(); i++)
					{
						if (!g_playerRecords.at(i).initialized)
							continue;

						if (term[0])
						{
							char szName[128];
							for (int i = 0; i < 128; i++)
								szName[i] = tolower(termRec[i]);

							char szTerm[128];
							for (int i = 0; i < 128; i++)
								szTerm[i] = tolower(termRec[i]);

							if (!strstr(szName, szTerm))
							{
								continue;
							}
						}

						if (ImGui::Selectable(std::to_string(i).c_str(), (bool)(realSelected = i), ImGuiSelectableFlags_SpanAllColumns))
						{
							selectedR = g_playerRecords.at(i).steamID64;
						}
						ImGui::NextColumn();

						if (g_playerRecords.at(i).name) ImGui::Text(g_playerRecords.at(i).name);
						ImGui::NextColumn();

						if (g_playerRecords.at(i).steamID64) ImGui::Text(std::to_string(g_playerRecords.at(i).steamID64).c_str());
						ImGui::NextColumn();

						long date = g_playerRecords.at(i).initDate;
						std::tm* local = std::localtime((time_t*)date);
						std::string time = std::asctime(local);
						if (g_playerRecords.at(i).initDate) ImGui::Text(time.c_str());
						ImGui::NextColumn();
					}
					ImGui::Columns(1, nullptr, false);
				}
				ImGui::EndGroupBox();
				break;
			}
		}
		ImGui::EndGroupBox();
	}
	ImGui::End();

	return texturesToRelease;
}

void Menu::Toggle()
{
	_visible = !_visible;
}

void Menu::CreateStyle()
{
	ImGui::StyleColorsDark();
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_AlphaBar| ImGuiColorEditFlags_HEX);
	_style.FrameRounding = 0.f;
	_style.WindowRounding = 0.f;
	_style.ChildRounding = 0.f;
	_style.Colors[ImGuiCol_Button] = ColToVec4( Settings::System::Theme );
	_style.Colors[ImGuiCol_SliderGrab] = ColToVec4( Settings::System::Theme );
	_style.Colors[ImGuiCol_SliderGrabActive] = ColToVec4( Darken(Settings::System::Theme) );
	_style.Colors[ImGuiCol_ButtonActive] = ColToVec4( Settings::System::Theme );
	_style.Colors[ImGuiCol_ButtonHovered] = ColToVec4( Darken(Settings::System::Theme) );
	_style.Colors[ImGuiCol_CheckMark] = ColToVec4( Settings::System::Theme );
	_style.Colors[ImGuiCol_Text] = ColToVec4( Settings::System::TextTheme );
	_style.Colors[ImGuiCol_FrameBg] = ColToVec4( Lighten(Settings::System::WindowTheme) );
	_style.Colors[ImGuiCol_WindowBg] = ColToVec4( Settings::System::WindowTheme );
	_style.Colors[ImGuiCol_PopupBg] = ColToVec4( 35.f, 35.f, 35.f, 255.f);
	ImGui::GetStyle() = _style;
}