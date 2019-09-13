#include "Chams.hpp"

#include "../Aimbot/Backtracking.hpp"

bool Settings::Visuals::Chams::Players::Enabled;

bool Settings::Visuals::Chams::Players::Enemies::Enabled = false;
bool Settings::Visuals::Chams::Players::Teammates::Enabled = false;
bool Settings::Visuals::Chams::Players::Localplayer::Enabled = false;

bool Settings::Visuals::Chams::Players::Enemies::DoOccluded = false;
bool Settings::Visuals::Chams::Players::Teammates::DoOccluded = false;
bool Settings::Visuals::Chams::Players::Localplayer::DoOccluded = false;

int Settings::Visuals::Chams::Players::Enemies::Material = 0;
int Settings::Visuals::Chams::Players::Teammates::Material = 0;
int Settings::Visuals::Chams::Players::Localplayer::Material = 0;

Color Settings::Visuals::Chams::Players::Enemies::Visible = Color(200, 0, 0);
Color Settings::Visuals::Chams::Players::Enemies::Occluded = Color(100, 0, 0);
Color Settings::Visuals::Chams::Players::Teammates::Visible = Color(0, 0, 200);
Color Settings::Visuals::Chams::Players::Teammates::Occluded = Color(0, 0, 100);
Color Settings::Visuals::Chams::Players::Localplayer::Visible = Color(0, 200, 0);
Color Settings::Visuals::Chams::Players::Localplayer::Occluded = Color(0, 100, 0);

bool Settings::Visuals::Chams::Viewmodel::Arms::Enabled = false;
bool Settings::Visuals::Chams::Viewmodel::Sleeves::Enabled = false;
bool Settings::Visuals::Chams::Viewmodel::Weapons::Enabled = false;

bool Settings::Visuals::Chams::Viewmodel::Arms::Wireframe = false;
bool Settings::Visuals::Chams::Viewmodel::Sleeves::Wireframe = false;
bool Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe = false;

int Settings::Visuals::Chams::Viewmodel::Arms::Material = 0;
int Settings::Visuals::Chams::Viewmodel::Sleeves::Material = 0;
int Settings::Visuals::Chams::Viewmodel::Weapons::Material = 0;

Color Settings::Visuals::Chams::Viewmodel::Arms::Colour = Color(255, 255, 255);
Color Settings::Visuals::Chams::Viewmodel::Sleeves::Colour = Color(255, 255, 255);
Color Settings::Visuals::Chams::Viewmodel::Weapons::Colour = Color(255, 255, 255);

void Chams::Initialize()
{
	std::ofstream("csgo\\materials\\chimphook_regular.vmt") << R"#("VertexLitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez"      "0"
		"$envmap"       ""
		"$nofog"        "1"
		"$model"        "1"
		"$nocull"       "0"
		"$selfillum"    "1"
		"$halflambert"  "1"
		"$znearer"      "0"
		"$flat"         "1"
	})#";

	std::ofstream("csgo\\materials\\chimphook_flat.vmt") << R"#("UnlitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez"      "0"
		"$envmap"       ""
		"$nofog"        "1"
		"$model"        "1"
		"$nocull"       "0"
		"$selfillum"    "1"
		"$halflambert"  "1"
		"$znearer"      "0"
		"$flat"         "1"
	})#";

	std::ofstream("csgo\\materials\\chimphook_glossy.vmt") << R"#("VertexLitGeneric" {
		"$basetexture" "vgui/white_additive"
		"$ignorez" "0"
		"$envmap" "env_cubemap"
		"$normalmapalphaenvmapmask" "1"
		"$envmapcontrast"  "1"
		"$nofog" "1"
		"$model" "1"
		"$nocull" "0"
		"$selfillum" "1"
		"$halflambert" "1"
		"$znearer" "0"
		"$flat" "1" 
	})#";
}

void Chams::Shutdown()
{
	std::remove("csgo\\materials\\chimphook_regular.vmt");
	std::remove("csgo\\materials\\chimphook_flat.vmt");
	std::remove("csgo\\materials\\chimphook_glossy.vmt");
}

IMaterial* GetMaterial(int i)
{
	static IMaterial* reg = g_MatSystem->FindMaterial("chimphook_regular");
	static IMaterial* flat = g_MatSystem->FindMaterial("chimphook_flat");
	static IMaterial* glossy = g_MatSystem->FindMaterial("chimphook_glossy");

	switch (i)
	{
	case 0:
		return g_MatSystem->FindMaterial("chimphook_regular");
		break;
	case 1:
		return g_MatSystem->FindMaterial("chimphook_flat");
		break;
	case 2:
		return g_MatSystem->FindMaterial("chimphook_glossy");
		break;
	}
}

void Chams::SceneEnd()
{
	for (int i = 1; i < g_EntityList->GetMaxEntities(); ++i)
	{
		C_BaseEntity* ent = C_BaseEntity::GetEntityByIndex(i);

		if (ent && ent->IsPlayer() && !ent->IsDormant())
		{
			C_BasePlayer* pl = (C_BasePlayer*)ent;

			if (ent && pl->IsAlive() && !pl->IsDormant())
			{
				bool is_teammate = pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum();
				bool is_enemy = !is_teammate;
				bool is_localplayer = pl == g_LocalPlayer;

				if (is_localplayer)
				{
					if (Settings::Visuals::Chams::Players::Localplayer::Enabled)
					{
						IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Players::Localplayer::Material);

						if (Settings::Visuals::Chams::Players::Localplayer::DoOccluded)
						{
							Color col = Settings::Visuals::Chams::Players::Localplayer::Occluded;
							g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							mat->IncrementReferenceCount();
							g_MdlRender->ForcedMaterialOverride(mat);

							ent->DrawModel(1, 255);
						}

						Color col = Settings::Visuals::Chams::Players::Localplayer::Visible;
						g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->IncrementReferenceCount();
						g_MdlRender->ForcedMaterialOverride(mat);

						ent->DrawModel(1, 255);

						if (Settings::AntiAim::Yaw::Type == 3)
						{
							QAngle origAngle = g_LocalPlayer->GetAngles();

							g_LocalPlayer->SetAbsAngles(QAngle(0, CreateMove::FakeAngles.yaw, 0));
							mat->ColorModulate(0.f, 0.f, 0.f);
							g_MdlRender->ForcedMaterialOverride(mat);
							g_LocalPlayer->DrawModel(1, 255);
							g_MdlRender->ForcedMaterialOverride(nullptr);
							g_LocalPlayer->SetAbsAngles(QAngle(0, origAngle.yaw, 0));
						}
					}
				}
				else if (!is_localplayer && is_enemy)
				{
					if (Settings::Visuals::Chams::Players::Enemies::Enabled)
					{
						IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Players::Enemies::Material);

						if (Settings::Visuals::Chams::Players::Enemies::DoOccluded)
						{
							Color col = Settings::Visuals::Chams::Players::Enemies::Occluded;
							g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);

							if (Settings::Backtrack::Enabled && Backtrack::Get().selectedpl && Backtrack::Get().selectedr && Backtrack::Get().selectedpl == pl->EntIndex())
								g_RenderView->SetBlend(0.05f);
							else
								g_RenderView->SetBlend(1.f);

							mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							mat->IncrementReferenceCount();
							g_MdlRender->ForcedMaterialOverride(mat);

							ent->DrawModel(1, 255);

							if (Settings::Backtrack::Enabled)
							{
								QAngle origAng = pl->GetAngles();
								Vector origOrigin = pl->m_vecOrigin();
								Vector origVelocity = pl->m_vecVelocity();
								float origCycle = pl->m_flCycle();
								int origSequence = pl->m_nSequence();
								EntityFlags origFlags = (EntityFlags)pl->m_fFlags();
								float origLowerbodyyawtarget = pl->m_flLowerBodyYawTarget();

								for (int i = 0; i < Backtrack::Get().records[ent->EntIndex()].size(); i++)
								{
									backtrack_record_t record = Backtrack::Get().records[ent->EntIndex()][i];

									pl->SetAbsOrigin(record.origin);
									pl->SetAbsAngles(record.angs);
									pl->m_vecVelocity() = record.velocity;
									pl->m_flCycle() = record.cycle;
									pl->m_nSequence() = record.sequence;
									pl->m_fFlags() = record.flags;
									pl->m_flLowerBodyYawTarget() = record.lowerbodyyawtarget;

									if (pl->EntIndex() == Backtrack::Get().selectedpl && i == Backtrack::Get().selectedr)
										g_RenderView->SetBlend(1.f);
									else
										g_RenderView->SetBlend(0.05f);

									ent->DrawModel(0x1, 255);
								}

								g_RenderView->SetBlend(1.f);

								pl->SetAbsAngles(origAng);
								pl->SetAbsOrigin(origOrigin);
								pl->m_vecVelocity() = origVelocity;
								pl->m_flCycle() = origCycle;
								pl->m_nSequence() = origSequence;
								pl->m_fFlags() = origFlags;
								pl->m_flLowerBodyYawTarget() = origLowerbodyyawtarget;
							}
						}

						Color col = Settings::Visuals::Chams::Players::Enemies::Visible;
						g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);

						if (Settings::Backtrack::Enabled && Backtrack::Get().selectedpl && Backtrack::Get().selectedr && Backtrack::Get().selectedpl == pl->EntIndex())
						{
							g_RenderView->SetBlend(0.05f);
						}
						else
						{
							g_RenderView->SetBlend(1.f);
						}

						mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->IncrementReferenceCount();
						g_MdlRender->ForcedMaterialOverride(mat);

						ent->DrawModel(1, 255);

						if (Settings::Backtrack::Enabled)
						{
							QAngle origAng = pl->GetAngles();
							Vector origOrigin = pl->m_vecOrigin();
							Vector origVelocity = pl->m_vecVelocity();
							float origCycle = pl->m_flCycle();
							int origSequence = pl->m_nSequence();
							EntityFlags origFlags = (EntityFlags)pl->m_fFlags();
							float origLowerbodyyawtarget = pl->m_flLowerBodyYawTarget();

							for (int i = 0; i < Backtrack::Get().records[ent->EntIndex()].size(); i++)
							{
								backtrack_record_t record = Backtrack::Get().records[ent->EntIndex()][i];

								pl->SetAbsOrigin(record.origin);
								pl->SetAbsAngles(record.angs);
								pl->m_vecVelocity() = record.velocity;
								pl->m_flCycle() = record.cycle;
								pl->m_nSequence() = record.sequence;
								pl->m_fFlags() = record.flags;
								pl->m_flLowerBodyYawTarget() = record.lowerbodyyawtarget;

								if (pl->EntIndex() == Backtrack::Get().selectedpl && i == Backtrack::Get().selectedr)
									g_RenderView->SetBlend(1.f);
								else
									g_RenderView->SetBlend(0.05f);

								ent->DrawModel(0x1, 255);
							}

							g_RenderView->SetBlend(1.f);

							pl->SetAbsAngles(origAng);
							pl->SetAbsOrigin(origOrigin);
							pl->m_vecVelocity() = origVelocity;
							pl->m_flCycle() = origCycle;
							pl->m_nSequence() = origSequence;
							pl->m_fFlags() = origFlags;
							pl->m_flLowerBodyYawTarget() = origLowerbodyyawtarget;
						}
					}
				}
				else if (!is_localplayer && is_teammate)
				{
					if (Settings::Visuals::Chams::Players::Teammates::Enabled)
					{
						IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Players::Teammates::Material);

						if (Settings::Visuals::Chams::Players::Teammates::DoOccluded)
						{
							Color col = Settings::Visuals::Chams::Players::Teammates::Occluded;
							g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							mat->IncrementReferenceCount();
							g_MdlRender->ForcedMaterialOverride(mat);

							ent->DrawModel(1, 255);
						}

						Color col = Settings::Visuals::Chams::Players::Teammates::Visible;
						g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->IncrementReferenceCount();
						g_MdlRender->ForcedMaterialOverride(mat);

						ent->DrawModel(1, 255);
					}
				}

				g_MdlRender->ForcedMaterialOverride(nullptr);
			}
		}

		if (ent && ent->IsGrenade())
		{
			C_BaseGrenadeProjectile* gren = (C_BaseGrenadeProjectile*)ent;
			IMaterial* mat = GetMaterial(0);

			g_RenderView->SetColorModulation(1.f, 1.f, 1.f);
			mat->ColorModulate(1.f, 1.f, 1.f);
			mat->IncrementReferenceCount();
			mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
			g_MdlRender->ForcedMaterialOverride(mat);

			ent->DrawModel(1, 255);

			g_MdlRender->ForcedMaterialOverride(nullptr);
		}
	}
}

void Chams::OnDME(
	IMatRenderContext* ctx,
	const DrawModelState_t& state,
	const ModelRenderInfo_t& info,
	matrix3x4_t* matrix)
{
	static auto fnDME = Hooks::mdlrender_hook.get_original<decltype(&Hooks::hkDrawModelExecute)>(index::DrawModelExecute);

	const model_t* mdl = info.pModel;

	if (!g_EngineClient->IsInGame())
		return;

	bool is_arm = strstr(mdl->szName, "arms");
	bool is_sleeves = strstr(mdl->szName, "sleeve");
	bool is_weapon = strstr(mdl->szName, "weapons/v_");

	if (is_sleeves)
	{
		auto mat = g_MatSystem->FindMaterial(mdl->szName);
		if (!mat)
			return;

		if (Settings::Misc::Removals::Sleeves)
		{
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_MdlRender->ForcedMaterialOverride(mat);
		}
		else
		{
			if (Settings::Visuals::Chams::Viewmodel::Sleeves::Enabled)
			{
				IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Viewmodel::Sleeves::Material);
				Color col = Settings::Visuals::Chams::Viewmodel::Sleeves::Colour;

				mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
				mat->AlphaModulate(col.a() / 255.f);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Visuals::Chams::Viewmodel::Sleeves::Wireframe);

				g_MdlRender->ForcedMaterialOverride(mat);
			}
		}

		return;
	}
	else if (is_arm)
	{
		auto mat = g_MatSystem->FindMaterial(mdl->szName);
		if (!mat)
			return;

		if (Settings::Misc::Removals::Arms)
		{
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_MdlRender->ForcedMaterialOverride(mat);
		}
		else
		{
			if (Settings::Visuals::Chams::Viewmodel::Arms::Enabled)
			{
				IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Viewmodel::Arms::Material);
				Color col = Settings::Visuals::Chams::Viewmodel::Arms::Colour;

				mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
				mat->AlphaModulate(col.a() / 255.f);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Visuals::Chams::Viewmodel::Arms::Wireframe);

				g_MdlRender->ForcedMaterialOverride(mat);
			}
		}

		return;
	}
	else if (is_weapon)
	{
		auto mat = g_MatSystem->FindMaterial(mdl->szName);
		if (!mat)
			return;

		if (Settings::Misc::Removals::Weapon)
		{
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_MdlRender->ForcedMaterialOverride(mat);
		}
		else
		{
			if (Settings::Visuals::Chams::Viewmodel::Weapons::Enabled)
			{
				IMaterial* mat = GetMaterial(Settings::Visuals::Chams::Viewmodel::Weapons::Material);
				Color col = Settings::Visuals::Chams::Viewmodel::Weapons::Colour;

				mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
				mat->AlphaModulate(col.a() / 255.f);
				mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, true);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe);

				g_MdlRender->ForcedMaterialOverride(mat);
			}
		}

		return;
	}
}