#include "Visual.hpp"

bool Settings::Misc::Removals::Arms = false;
bool Settings::Misc::Removals::Sleeves = false;
bool Settings::Misc::Removals::Weapon = false;

bool Settings::Misc::FreeLook::Enabled = false;

void FreeLook::OnCreateMove(CUserCmd* cmd)
{
	if (!Settings::Misc::FreeLook::Enabled || !Settings::Misc::FreeLook::_enabled)
	{
		LastState::Get().viewangles = cmd->viewangles;
		LastState::Get().virtualViewangles.Init(0, 0, 0);
	}
	else
	{
		if (LastState::Get().virtualViewangles.IsZero())
		{
			LastState::Get().virtualViewangles = LastState::Get().viewangles;
		}

		LastState::Get().virtualViewangles += cmd->viewangles - LastState::Get().viewangles;

		LastState::Get().virtualViewangles.Normalize();
		if (LastState::Get().virtualViewangles.pitch >= 89.0f) LastState::Get().virtualViewangles.pitch = 89.0f;
		else if (LastState::Get().virtualViewangles.pitch <= -89.0f) LastState::Get().virtualViewangles.pitch = -89.0f;

		g_EngineClient->SetViewAngles(&LastState::Get().viewangles);
	}
}

void FreeLook::OverrideView(CViewSetup* vsView)
{
	if (Settings::Misc::FreeLook::Enabled && Settings::Misc::FreeLook::_enabled && !LastState::Get().virtualViewangles.IsZero() && !g_Input->m_fCameraInThirdPerson)
	{
		vsView->angles.yaw = LastState::Get().virtualViewangles.yaw;
		vsView->angles.pitch = LastState::Get().virtualViewangles.pitch;
	}
}

bool Settings::Misc::FakeZoom::Enabled = false;

bool FakeZoom(CViewSetup* vsView)
{
	if (Settings::Misc::FakeZoom::Enabled && Settings::Misc::FakeZoom::_enabled && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
	{
		vsView->fov -= 45;
	}

	return false;
}

void DoThirdPersonSpectate()
{
	if (!g_LocalPlayer)
		return;

	if (!g_LocalPlayer->m_hObserverTarget())
		return;

	if (Settings::Misc::ThirdPersonSpectate::Enabled && Settings::Misc::ThirdPersonSpectate::_enabled)
	{
		g_LocalPlayer->m_iObserverMode() = OBS_MODE_CHASE;
	}
	else
	{
		g_LocalPlayer->m_iObserverMode() = OBS_MODE_IN_EYE;
	}
}

void CameraFOV(CViewSetup* vsView)
{
	bool isobs = false;
	C_BasePlayer* player = g_LocalPlayer->SelfOrObs(&isobs);

	if (FakeZoom(vsView))
		return;

	if (isobs)
	{
		OBS_MODES obsmode = (OBS_MODES)g_LocalPlayer->m_iObserverMode();

		if (obsmode != OBS_MODE_IN_EYE && obsmode != OBS_MODE_DEATHCAM)
		{
			return;
		}
	}


	if (Settings::Misc::CameraFOV::Enabled && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
	{
		vsView->fov += Settings::Misc::CameraFOV::FOV;
	}
}

bool Settings::Misc::ViewmodelOffset::Enabled = false;
float Settings::Misc::ViewmodelOffset::X = 0.f;
float Settings::Misc::ViewmodelOffset::Y = 0.f;
float Settings::Misc::ViewmodelOffset::Z = 0.f;

void ViewmodelFOV(CViewSetup* vsView)
{
	static ConVar* viewmodel_fov = g_CVar->FindVar("viewmodel_fov");
	viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_x = g_CVar->FindVar("viewmodel_offset_x");
	viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_y = g_CVar->FindVar("viewmodel_offset_y");
	viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;

	static ConVar* viewmodel_offset_z = g_CVar->FindVar("viewmodel_offset_z");
	viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;

	if (Settings::Misc::ViewmodelFOV::Enabled)
		viewmodel_fov->SetValue(Settings::Misc::ViewmodelFOV::FOV);
	else
		viewmodel_fov->SetValue(60);

	if (Settings::Misc::ViewmodelOffset::Enabled)
	{
		viewmodel_offset_x->SetValue(Settings::Misc::ViewmodelOffset::X);
		viewmodel_offset_y->SetValue(Settings::Misc::ViewmodelOffset::Y);
		viewmodel_offset_z->SetValue(Settings::Misc::ViewmodelOffset::Z);
	}
	else
	{
		viewmodel_offset_x->SetValue(0);
		viewmodel_offset_y->SetValue(0);
		viewmodel_offset_z->SetValue(0);
	}
}

void WorldColour::Clear()
{
	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World"))
		{
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
		}
	}

	if (Settings::Visuals::Nightmode::Enabled)
		Nightmode::Get().Execute();
}

void WorldColour::Execute()
{
	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World"))
		{
			// pMaterial->AlphaModulate(0.5f);
			pMaterial->ColorModulate(Settings::Visuals::WorldColour::Colour.r() / 255.f, Settings::Visuals::WorldColour::Colour.g() / 255.f, Settings::Visuals::WorldColour::Colour.b() / 255.f);
		}
	}
}

void LoadSky(const char* skyname)
{
	static void(__fastcall* LoadNamedSky)(const char*) = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::PatternScan(GetModuleHandleW(L"engine.dll"), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	LoadNamedSky(skyname);
}

bool Settings::Visuals::Nightmode::Enabled = false;
float Settings::Visuals::Nightmode::Intensity = 0.f;

void Nightmode::Clear()
{
	LoadSky(currentSkyName);

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		static IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
		{
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, true);
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
		}
	}
}

void Nightmode::Execute()
{
	LoadSky("sky_csgo_night02");

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
		{
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
			pMaterial->ColorModulate(1.f - Settings::Visuals::Nightmode::Intensity, 1.f - Settings::Visuals::Nightmode::Intensity, 1.f - Settings::Visuals::Nightmode::Intensity);
		}
	}
}

bool Settings::Misc::Brightness::Enabled = false;
int Settings::Misc::Brightness::Perc = 0;

void AdjustBrightness()
{
	static ConVar* mat_force_tonemap_scale = g_CVar->FindVar("mat_force_tonemap_scale");

	if (Settings::Misc::Brightness::Enabled)
		mat_force_tonemap_scale->SetValue((float)Settings::Misc::Brightness::Perc / 100.f);
	else
		mat_force_tonemap_scale->SetValue(mat_force_tonemap_scale->m_pszDefaultValue);
}

bool Settings::Misc::Removals::Shadows = false;

void RemoveShadows()
{
	static ConVar* shadows = g_CVar->FindVar("cl_csm_enabled");
	shadows->SetValue(!Settings::Misc::Removals::Shadows);
}

bool Settings::Misc::Removals::Flash = false;

void RemoveFlash()
{
	if (!Settings::Misc::Removals::Flash)
		return;

	g_LocalPlayer->m_flFlashDuration() = 0.f;
}

bool Settings::Misc::Removals::Smoke = false;
bool Settings::Misc::Removals::SmokeOverlay = false;
int Settings::Misc::Removals::SmokeType = 0;

void RemoveSmoke()
{
	static constexpr const char* smokeMaterials[]{
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull",
	};

	for (const auto mat : smokeMaterials)
	{
		auto material = g_MatSystem->FindMaterial(mat);

		material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Misc::Removals::SmokeType == 0 && Settings::Misc::Removals::Smoke);
		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Settings::Misc::Removals::SmokeType == 1 && Settings::Misc::Removals::Smoke);
	}
}

bool Settings::Misc::BulletImpacts = false;

void BulletImpacts()
{
	ConVar* bulletImpacts = g_CVar->FindVar("sv_showimpacts");

	bulletImpacts->SetValue(Settings::Misc::BulletImpacts ? 1 : 0);
}

bool Settings::Misc::NoDecals = false;

void NoDecals()
{
	if (!Settings::Misc::NoDecals)
		return;

	g_EngineClient->ClientCmd("r_cleardecals");
}

bool Settings::Misc::RecoilCrosshair = false;

void RecoilCrosshair()
{
	ConVar* recoilCrosshair = g_CVar->FindVar("cl_crosshair_recoil");

	recoilCrosshair->SetValue(Settings::Misc::RecoilCrosshair ? 1 : 0);
}

bool Settings::Misc::Removals::PostProcessing = false;

void RemovePostProcessing()
{
	ConVar* postprocessing = g_CVar->FindVar("mat_postprocess_enable");

	postprocessing->SetValue(Settings::Misc::Removals::PostProcessing ? 0 : 1);
}

bool Settings::Misc::SniperCrosshair = false;

void SniperCrosshair()
{
	if (!Settings::Misc::SniperCrosshair)
		return;

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer)
		return;

	static ConVar* showSpread = g_CVar->FindVar("weapon_debug_spread_show");

	bool isobs = false;
	C_BasePlayer* player = g_LocalPlayer->SelfOrObs(&isobs);
	OBS_MODES obsmode = OBS_MODE_NONE;

	if (isobs)
	{
		OBS_MODES obsmode = (OBS_MODES)g_LocalPlayer->m_iObserverMode();
	}

	if (obsmode == OBS_MODE_IN_EYE || obsmode == OBS_MODE_DEATHCAM || !isobs)
		if (g_LocalPlayer->SelfOrObs() && !g_LocalPlayer->SelfOrObs()->m_bIsScoped())
			if (g_LocalPlayer->SelfOrObs()->m_hActiveWeapon())
				if (g_LocalPlayer->SelfOrObs()->m_hActiveWeapon()->IsSniper() || g_LocalPlayer->SelfOrObs()->m_hActiveWeapon()->IsAuto())
					showSpread->SetValue(3);
				else
					showSpread->SetValue(0);
			else
				showSpread->SetValue(0);
		else
			showSpread->SetValue(0);
	else
		showSpread->SetValue(0);
}
