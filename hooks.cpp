#include "hooks.hpp"
#include "render.hpp"
#include <intrin.h>  

#define NOMINMAX
#include <Windows.h>

#include "menu.hpp"
#include "eventemitter.hpp"
#include "settings.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "cheats/hotkeys.hpp"
#include "cheats/movement.hpp"
#include "cheats/visuals.hpp"
#include "cheats/misc.hpp"
#include "cheats/aim.hpp"
#include "cheats/chattranslator.h"

#include "helpers/skinchangerparser.hpp"
#include "cheats/chamsPreview/test.cpp"

#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

#pragma intrinsic(_ReturnAddress)  

bool Settings::System::Unload = false;
bool Settings::System::AntiUntrust = true;
bool Settings::Misc::RemoveFootsteps = false;
bool Settings::Misc::AutoAccept = true;
bool Settings::Misc::TranslateBot = false;
bool Settings::Misc::ViewmodelFOV::Enabled = false;
int Settings::Misc::ViewmodelFOV::FOV = 68;
bool Settings::Misc::CameraFOV::Enabled = false;
int Settings::Misc::CameraFOV::FOV = 0;
bool Settings::Misc::WorldColour::Enabled = false;
Color Settings::Misc::WorldColour::Colour = Color(0, 0, 0, 255);
bool Settings::Misc::Removals::RecoilShake = false;
bool Settings::Misc::Removals::Scope = false;
bool Settings::Misc::Removals::ScopeBlur = false;
bool Settings::Misc::Removals::Sky3D = false;

int Settings::ClanTag::Type = 0;
ClanTagMember Settings::ClanTag::StaticTag;
ClanTagMember Settings::ClanTag::PulseTag;
ClanTagMember Settings::ClanTag::ScrollTag;
std::vector<ClanTagMember> Settings::ClanTag::CustomTagEditor;

EventListener* events = nullptr;

bool CreateMove::sendPacket = true;
QAngle CreateMove::lastTickViewAngles = QAngle(0, 0, 0);
int CreateMove::tick = 0;

QAngle CreateMove::RealAngles = QAngle(0, 0, 0);
QAngle CreateMove::FakeAngles = QAngle(0, 0, 0);

bool Settings::Misc::FUCK = false;

void FastLargeNormaliseMove(QAngle& angle)
{
	if (!std::isfinite(angle.yaw)) { angle.yaw = 0; }
	if (!std::isfinite(angle.pitch)) { angle.pitch = 0; }

	while (angle.pitch <= -1800000) angle.pitch += 3600000;
	while (angle.pitch > 1800000) angle.pitch -= 3600000;
	while (angle.yaw <= -1800000) angle.yaw += 3600000;
	while (angle.yaw > 1800000) angle.yaw -= 3600000;

	while (angle.pitch <= -18000) angle.pitch += 36000;
	while (angle.pitch > 18000) angle.pitch -= 36000;
	while (angle.yaw <= -18000) angle.yaw += 36000;
	while (angle.yaw > 18000) angle.yaw -= 36000;

	while (angle.pitch <= -180) angle.pitch += 360;
	while (angle.pitch > 180) angle.pitch -= 360;
	while (angle.yaw < -180) angle.yaw += 360;
	while (angle.yaw > 180) angle.yaw -= 360;

	if (angle.pitch < -180) angle.pitch = -179.999;
	if (angle.pitch > 180) angle.pitch = 179.999;

	angle.roll = 0;
}

namespace Hooks
{

	void Initialize()
	{
		engineclient_hook.setup(g_EngineClient);
		hlclient_hook.setup(g_CHLClient);
		direct3d_hook.setup(g_D3DDevice9);
		vguipanel_hook.setup(g_VGuiPanel);
		vguisurf_hook.setup(g_VGuiSurface);
		sound_hook.setup(g_EngineSound);
		mdlrender_hook.setup(g_MdlRender);
		materialsystem_hook.setup(g_MatSystem);
		clientmode_hook.setup(g_ClientMode);
		viewrender_hook.setup(g_ViewRender);
		renderview_hook.setup(g_RenderView);
		bsp_query_hook.setup(g_EngineClient->GetBSPTreeQuery());
		ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
		sv_cheats.setup(sv_cheats_con);

		direct3d_hook.hook_index(index::EndScene, hkEndScene);
		direct3d_hook.hook_index(index::Reset, hkReset);
		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);
		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);
		sound_hook.hook_index(index::EmitSound1, hkEmitSound1);
		vguisurf_hook.hook_index(index::LockCursor, hkLockCursor);
		vguisurf_hook.hook_index(index::DrawSetColor, hkDrawSetColor);
		viewrender_hook.hook_index(index::RenderSmokeOverlay, hkRenderSmokeOverlay);
		// viewrender_hook.hook_index(index::RenderView, hkRenderView);
		renderview_hook.hook_index(index::SceneEnd, hkSceneEnd);
		materialsystem_hook.hook_index(index::FindMaterial, hkFindMaterial);
		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);
		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);
		sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);
		bsp_query_hook.hook_index(index::ListLeavesInBox, hkListLeavesInBox);
		engineclient_hook.hook_index(index::ClientCmd, hkClientCmd);
		hlclient_hook.hook_index(index::DispatchUserMessage, hkDispatchUserMessage);

		events = new EventListener({ "player_hurt", "game_init",  "game_start", "round_start", "game_newmap", "bullet_impact", "player_death", "round_end" });

		/*RecvProp* seq = NetvarSys::Get().GetNetvarProp("CBaseViewModel", "m_nSequence");
		if (seq) {
			o_nSequence = (RecvVarProxyFn)seq->m_ProxyFn;
			seq->m_ProxyFn = (RecvVarProxyFn)SkinChanger::nSequence;
		}*/

		/*
		g_MatSystem->forceBeginRenderTargetAllocation();
		mirrorTexture = g_MatSystem->CreateNamedRenderTargetTextureEx(
			"mirrorcam_rt", 1, 1, RT_SIZE_FULL_FRAME_BUFFER, g_MatSystem->GetBackBufferFormat(),
			MATERIAL_RT_DEPTH_SHARED, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			CREATERENDERTARGETFLAGS_HDR
		);
		g_MatSystem->forceEndRenderTargetAllocation();

		std::ofstream("csgo\\materials\\mirrorcam.vmt") << R"#("UnlitGeneric" {
			"$basetexture" "mirrorcam_rt"
		})#";

		mirrorMaterial = g_MatSystem->FindMaterial("mirrorcam.vmt");*/
	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		hlclient_hook.unhook_all();
		direct3d_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		materialsystem_hook.unhook_all();
		viewrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		sound_hook.unhook_all();
		sv_cheats.unhook_all();
		bsp_query_hook.unhook_all();

		std::remove("csgo\\materials\\mirrorcam.vmt");
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkRenderView(IViewRender* this0, void* EDX, CViewSetup& view, CViewSetup& hudViewSetup, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw)
	{
		static auto oRenderView = viewrender_hook.get_original<decltype(&hkRenderView)>(index::RenderView);
		oRenderView(this0, EDX, view, hudViewSetup, nClearFlags, whatToDraw);

		CViewSetup mirrorView = view;
		mirrorView.x = mirrorView.x_old = 0;
		mirrorView.y = mirrorView.y_old = 0;
		mirrorView.width = mirrorView.width_old = 480;
		mirrorView.height = mirrorView.height_old = 300;
		mirrorView.angles = view.angles + QAngle(180.f, 0, 0);
		mirrorView.m_flAspectRatio = float(mirrorView.width) / float(mirrorView.height);

		RenderCustomView(mirrorView, mirrorTexture);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		static auto oEndScene = direct3d_hook.get_original<decltype(&hkEndScene)>(index::EndScene);

		static ConVar* Sky3D = g_CVar->FindVar("r_3dsky");/*
		static ConVar* malr = g_CVar->FindVar("mat_ambient_light_r");
		static ConVar* malg = g_CVar->FindVar("mat_ambient_light_g");
		static ConVar* malb = g_CVar->FindVar("mat_ambient_light_b");
		if (Settings::Misc::WorldColour::Enabled) {
			malr->SetValue(Settings::Misc::WorldColour::Colour.r() / 255.f);
			malg->SetValue(Settings::Misc::WorldColour::Colour.g() / 255.f);
			malb->SetValue(Settings::Misc::WorldColour::Colour.b() / 255.f);
		}
		else
		{
			malr->SetValue(0.f);
			malg->SetValue(0.f);
			malb->SetValue(0.f);
		}*/

		Sky3D->SetValue(!Settings::Misc::Removals::Sky3D);

		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImDrawList* esp_drawlist = nullptr;
		esp_drawlist = Render::Get().RenderScene();
		std::vector<ImTextureID> trelease = Menu::Get().Render(pDevice);
		ImGui::Render(esp_drawlist);
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

		for (ImTextureID i : trelease)
			((LPDIRECT3DTEXTURE9)i)->Release();

		return oEndScene(pDevice);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkSceneEnd(void* thisptr, void* edx)
	{
		static auto oSceneEnd = viewrender_hook.get_original<decltype(&hkSceneEnd)>(index::SceneEnd);

		Chams::SceneEnd();

		oSceneEnd(thisptr, edx);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(index::Reset);

		Menu::Get().OnDeviceLost();

		auto hr = oReset(device, pPresentationParameters);

		if (hr >= 0)
			Menu::Get().OnDeviceReset();

		return hr;
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
	{
		static auto oCreateMove = hlclient_hook.get_original<decltype(&hkCreateMove_Proxy)>(index::CreateMove);

		oCreateMove(g_CHLClient, 0, sequence_number, input_sample_frametime, active);

		auto cmd = g_Input->GetUserCmd(sequence_number);
		auto verified = g_Input->GetVerifiedCmd(sequence_number);
		
		bSendPacket = CreateMove::sendPacket;
		CreateMove::sendPacket = true;
		CreateMove::tick = cmd->tick_count;

		if (!cmd || !cmd->command_number)
			return;

		AutoDefuse(cmd);
		BunnyHop(cmd);
		AutoStrafer(cmd);
		AutoCowboy(cmd);
		BlockBot(cmd);
		InfiniteDuck(cmd);
		AutoPistol(cmd);
		AutoReload(cmd);
		SelfNade(cmd);
		SlowWalk(cmd);
		EdgeJumper(cmd);

		RadarHack();
		RecoilCrosshair();
		InverseRagdollGravity();
		RemoveFlash();
		BulletImpacts();
		NoDecals();
		RemovePostProcessing();
		AdjustBrightness();
		RemoveShadows();
		WorldColour();

		NameStealer::OnCreateMove();
		FreeLook::OnCreateMove(cmd);

		ChatSpammer::Get().OnCreateMove();
		ClantagScroller::Get().OnCreateMove();

		grenade_prediction::Get().Tick(cmd->buttons);

		FakeLag(cmd, bSendPacket);
		FakeDuck(cmd, bSendPacket);

		if (Menu::Get().IsVisible())
			cmd->buttons &= ~IN_ATTACK;

		if (!ChickenTamer(cmd)) {
			DoAim(cmd, bSendPacket);
		}

		if (Settings::System::AntiUntrust)
		{
			FastLargeNormaliseMove(cmd->viewangles);
			Math::ClampAngles(cmd->viewangles);
			cmd->viewangles.roll = 0.f;
		}

		if (CreateMove::sendPacket)
		{
			if (g_LocalPlayer->IsAlive() && g_EngineClient->IsInGame()) CreateMove::lastTickViewAngles = cmd->viewangles;
		}

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}
	//--------------------------------------------------------------------------------
	__declspec(naked) void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkDrawSetColor(ISurface* thisptr, void* edx, int r, int g, int b, int a)
	{
		static auto oDrawSetColor = vguisurf_hook.get_original<decltype(&hkDrawSetColor)>(index::DrawSetColor);
		const uintptr_t return_address = uintptr_t( _ReturnAddress() );

		static auto return_to_scope_arc = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?") + 5;
		static auto return_to_scope_lens = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "FF 50 3C 8B 4C 24 20") + 3;

		if (Settings::Misc::Removals::Scope) {
			if (return_address == (uintptr_t)return_to_scope_arc
				|| return_address == (uintptr_t)return_to_scope_lens)
			{
				return oDrawSetColor(thisptr, edx, r, g, b, 0);
			}
		}

		oDrawSetColor(thisptr, edx, r, g, b, a);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<decltype(&hkPaintTraverse)>(index::PaintTraverse);

		oPaintTraverse(g_VGuiPanel, edx, panel, forceRepaint, allowForce);

		Hotkeys::Begin();

		if (!panelId)
		{
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel"))
			{
				panelId = panel;
			}
		}
		else if (panelId == panel)
		{
			static bool bSkip = false;
			bSkip = !bSkip;

			if (bSkip)
				return;

			if (g_LocalPlayer && InputSys::Get().IsKeyDown(VK_TAB) && Settings::Misc::RankReveal)
				Utils::RankRevealAll();

			Render::Get().BeginScene();
		}

	/*	IMatRenderContext* renderCtx = g_MatSystem->GetRenderContext();

		renderCtx->DrawScreenSpaceRectangle(mirrorMaterial, 100, 100, 400, 300, 0, 0, float(400), float(300), mirrorTexture->GetActualWidth(), mirrorTexture->GetActualHeight(), nullptr, 1, 1);

		renderCtx->Release();*/
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkEmitSound1(void* _this, int edx, IRecipientFilter & filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector * pOrigin, const Vector * pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk)
	{
		static auto ofunc = sound_hook.get_original<decltype(&hkEmitSound1)>(index::EmitSound1);

		if (Settings::Misc::AutoAccept) {
			if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep"))
			{
				static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

				if (fnAccept)
				{

					fnAccept("");

					FLASHWINFO fi;
					fi.cbSize = sizeof(FLASHWINFO);
					fi.hwnd = InputSys::Get().GetMainWindow();
					fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
					fi.uCount = 0;
					fi.dwTimeout = 0;
					FlashWindowEx(&fi);
				}
			}
		}
		ofunc(g_EngineSound, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);

	}
	//--------------------------------------------------------------------------------
	int __fastcall hkDoPostScreenEffects(void* _this, int edx, int a1)
	{
		static auto oDoPostScreenEffects = clientmode_hook.get_original<decltype(&hkDoPostScreenEffects)>(index::DoPostScreenSpaceEffects);

		RemoveSmoke();

		return oDoPostScreenEffects(g_ClientMode, edx, a1);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkRenderSmokeOverlay(uintptr_t ecx, uintptr_t edx, bool a1)
	{
		static auto ofunc = viewrender_hook.get_original<decltype(&hkRenderSmokeOverlay)>(index::RenderSmokeOverlay);

		if (Settings::Misc::Removals::SmokeOverlay)
		{
			return;
		}

		ofunc(ecx, edx, a1);
	}
	//--------------------------------------------------------------------------------
	IMaterial* __fastcall hkFindMaterial(void* thisptr, void* edx, const char* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
	{
		static auto oFindMaterial = materialsystem_hook.get_original<IMaterial * (__thiscall*)(void*, const char*, const char*, bool, const char*)>(84);

		IMaterial* mat = oFindMaterial(thisptr, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
		
		if (Settings::Misc::Removals::ScopeBlur) {
			if (!strcmp(pMaterialName, "dev/scope_bluroverlay"))
			{
				static auto ClearAsShit = oFindMaterial(thisptr, "dev/clearalpha", nullptr, complain, pComplainPrefix);

				return ClearAsShit;
			}
		}
		
		return mat;
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage)
	{
		static auto ofunc = hlclient_hook.get_original<decltype(&hkFrameStageNotify)>(index::FrameStageNotify);
		
		QAngle vecAimPunch;
		QAngle vecViewPunch;

		QAngle* pAimPunch = nullptr;
		QAngle* pViewPunch = nullptr;

		ThirdPerson::FrameStageNotify(stage);
		
		if (g_EngineClient->IsInGame() && stage == FRAME_RENDER_START)
		{
			if (g_LocalPlayer && g_LocalPlayer->IsAlive())
			{
				pAimPunch = &g_LocalPlayer->m_aimPunchAngle();
				pViewPunch = &g_LocalPlayer->m_viewPunchAngle();

				vecAimPunch = *pAimPunch;
				vecViewPunch = *pViewPunch;

				if (Settings::Misc::Removals::RecoilShake) {
					*pAimPunch = QAngle(0, 0, 0);
					*pViewPunch = QAngle(0, 0, 0);
				}
			}
		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			SkinChanger::Run();
		}

		ofunc(g_CHLClient, edx, stage);

		if (pAimPunch != nullptr && pViewPunch != nullptr)
		{
			*pAimPunch = vecAimPunch;
			*pViewPunch = vecViewPunch;
		}
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkOverrideView(void* _this, int edx, CViewSetup* vsView)
	{
		static auto ofunc = clientmode_hook.get_original<decltype(&hkOverrideView)>(index::OverrideView);

		if (!g_LocalPlayer)
			return ofunc(g_ClientMode, edx, vsView);;

		C_BasePlayer* lcl = g_LocalPlayer->SelfOrObs();

		if (!lcl || !vsView)
			return ofunc(g_ClientMode, edx, vsView);

		grenade_prediction::Get().View();
		FreeLook::OverrideView(vsView);
		ThirdPerson::OverrideView();
		CameraFOV(vsView);
		ViewmodelFOV(vsView);

		ofunc(g_ClientMode, edx, vsView);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = vguisurf_hook.get_original<decltype(&hkLockCursor)>(index::LockCursor);

		if (Menu::Get().IsVisible())
		{
			g_VGuiSurface->UnlockCursor();
			g_InputSystem->ResetInputState();
			return;
		}

		ofunc(g_VGuiSurface);

	}
	//--------------------------------------------------------------------------------
	void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & pInfo, matrix3x4_t * pCustomBoneToWorld)
	{
		static auto ofunc = mdlrender_hook.get_original<decltype(&hkDrawModelExecute)>(index::DrawModelExecute);

		Chams::OnDME(ctx, state, pInfo, pCustomBoneToWorld);

		ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);

		const model_t* mdl = pInfo.pModel;

		if (!g_EngineClient->IsInGame())
			return;

		bool is_arm = strstr(mdl->szName, "arms");
		bool is_sleeves = strstr(mdl->szName, "sleeve");
		bool is_weapon = strstr(mdl->szName, "weapons/v_"); 
		
		if (is_arm || is_sleeves || is_weapon) g_MdlRender->ForcedMaterialOverride(nullptr);
	}
	bool __fastcall hkSvCheatsGetBool(void* pConVar, void* edx)
	{
		static auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "85 C0 75 30 38 86");
		static auto ofunc = sv_cheats.get_original<bool(__thiscall*)(PVOID)>(13);
		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;
		return ofunc(pConVar);
	}

	bool __fastcall hkDispatchUserMessage(void* ecx, void* edx, int type, unsigned int a3, unsigned int length, const void* msg_data)
	{
		static auto oDispatchUserMessage = hlclient_hook.get_original<tDispatchUserMessage>(index::DispatchUserMessage);

		if (type == CS_UM_SayText2 && Settings::Misc::ChatMimic::Enabled) {
			ChatMimic(msg_data);
		}

		if (type == CS_UM_SayText2)
		{
			bf_read read(reinterpret_cast<uintptr_t>(msg_data));
			read.SetOffset(1);
			int ent_index = read.ReadByte();
			read.Skip(3);
			std::string msg_name = read.ReadString();
			std::string player_name = read.ReadString();
			std::string message = read.ReadString();
			read.Skip(3);
			bool all_chat = read.ReadBool();

			if (chat_translator::Get().dispatch_user_message(ent_index, message, ecx, type, a3, length, msg_data))
			{
				return false;
			}
		}

		if (type == CS_UM_VoteStart && Settings::Misc::AntiKick)
		{
			AntiKick(msg_data, length);
		}

		return oDispatchUserMessage(ecx, type, a3, length, msg_data);
	}

	int __fastcall hkListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
	{
		typedef int(__thiscall * ListLeavesInBox)(void*, const Vector&, const Vector&, unsigned short*, int);
		static auto ofunc = bsp_query_hook.get_original< ListLeavesInBox >(index::ListLeavesInBox);

		if (*(uint32_t*)_ReturnAddress() != 0x14244489)
			return ofunc(bsp, mins, maxs, pList, listMax);

		auto info = *(RenderableInfo_t * *)((uintptr_t)_AddressOfReturnAddress() + 0x14);
		if (!info || !info->m_pRenderable)
			return ofunc(bsp, mins, maxs, pList, listMax);

		auto base_entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
		if (!base_entity || !base_entity->IsPlayer())
			return ofunc(bsp, mins, maxs, pList, listMax);

		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
		auto count = ofunc(bsp, map_min, map_max, pList, listMax);
		return count;
	}
	
	void __fastcall hkClientCmd(void* ecx, void* edx, const char* cmd)
	{
		static auto ofunc = engineclient_hook.get_original<decltype(&hkClientCmd)>(index::ClientCmd);

		std::string buf;
		EChatTranslatorResult result = chat_translator::Get().client_cmd(cmd, buf);

		if (result == ECTR_Translate)
			return ofunc(ecx, edx, buf.c_str());

		if (result == ECTR_DontSend)
			return;

		return ofunc(ecx, edx, cmd);
	}
}

DWORD WINAPI OnDllAttach(LPVOID base)
{
	// 
	// Wait at most 10s for the main game modules to be loaded.
	// 
	if (Utils::WaitForModules(10000, { L"client_panorama.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
	{
		// One or more modules were not loaded in time
		return FALSE;
	}

#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try
	{
		Utils::ConsolePrint("Initializing...\n");

		Interfaces::Initialize();
		Interfaces::Dump();

		NetvarSys::Get().Initialize();
		InputSys::Get().Initialize();
		Render::Get().Initialize();
		Menu::Get().Initialize();
		Chams::Initialize();
		Hooks::Initialize();

		KitParser.setup();

		Utils::ConsolePrint("Finished.\n");

		InputSys::Get().RegisterHotkey(VK_DELETE, [base]()
		{
			Settings::System::Unload = true;
		});

		InputSys::Get().RegisterHotkey(VK_INSERT, [base]()
		{
			Menu::Get().Toggle();
		});

		g_GameUI->CreateCommandMsgBox("ChimpHook", "ChimpHook has been hooked.");

		while (!Settings::System::Unload)
			Sleep(100);

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

	}
	catch (const std::exception & ex)
	{
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::ConsolePrint("Press any key to exit.\n");
		Utils::ConsoleReadKey();
		Utils::DetachConsole();

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
	}

	// unreachable
	//return TRUE;
}

BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif
	Chams::Shutdown();
	Hooks::Shutdown();

	events->~EventListener();

	Menu::Get().Shutdown();
	return TRUE;
}

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_  LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDll);
		CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
		return TRUE;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return OnDllDetach();
		return TRUE;
	default:
		return TRUE;
	}
}