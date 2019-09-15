#include "Screen.hpp"

#include "../Aimbot/Autowall.hpp"

bool Settings::Misc::WallbangCrosshair = false;

bool Settings::Misc::ZeusRange::Enabled = false;
float Settings::Misc::ZeusRange::Speed = 0.f;
int Settings::Misc::ZeusRange::Thickness = 1;
int Settings::Misc::ZeusRange::Step = 1;

bool Settings::Misc::SmokeHelper::Enabled = false;
int Settings::Misc::SmokeHelper::TypeHelp = 0;
bool Settings::Misc::DebugMode = false;

float CSGO_Armor2(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;

	if (ArmorValue > 0)
	{
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue))
		{
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}

	return flDamage;
}

void DrawC4Damage(C_BaseEntity* c4ent)
{
	if (!Settings::ESP::Others::C4::Enabled)
		return;

	C_PlantedC4* c4 = (C_PlantedC4*)c4ent;

	float flDistance = g_LocalPlayer->m_vecOrigin().DistTo(c4->m_vecOrigin());
	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ((flDistance - b) / c);
	float flDamage = a * exp(-d * d);

	int c4Damage = std::ceil(CSGO_Armor2(flDamage, g_LocalPlayer->m_ArmorValue()));
	std::string dmgs;
	dmgs += "-";
	dmgs += std::to_string(c4Damage);

	int SWidth, SHeight;
	g_EngineClient->GetScreenSize(SWidth, SHeight);

	if (c4Damage > 0.f)
	{
		Render::Get().RenderText(dmgs, ImVec2(20, SHeight / 2), 32.f, Color(255, 255, 255, 255), false, true, f_Verdana);
		ImVec2 sz = f_Verdana->CalcTextSizeA(32.f, FLT_MAX, 0.f, dmgs.c_str());

		if (c4Damage >= g_LocalPlayer->m_iHealth())
		{
			Render::Get().RenderText("FATAL", ImVec2(20, SHeight / 2 + sz.y + 2), 32.f, Color(255, 0, 0, 255), false, true, f_Verdana);
		}
	}

	float bombTimer = c4->m_flC4Blow() - g_GlobalVars->curtime;

	if (bombTimer > 0.f && c4->m_bBombTicking())
	{
		std::string bombsite = "Bombsite ";
		if (c4->m_nBombSite())
			bombsite += "B";
		else
			bombsite += "A";

		int SWIDTH, SHEIGHT;
		g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

		ImVec2 startPos = ImVec2(SWIDTH / 8 * 3, SHEIGHT / 48 * 8);
		ImVec2 endPos = ImVec2(SWIDTH / 8 * 5, SHEIGHT / 48 * 8 + 10);
		int width = endPos.x - startPos.x - 6;
		float perc = bombTimer / 40 * 100;
		std::string timer = std::to_string(bombTimer);

		Render::Get().RenderText(bombsite, ImVec2(SWIDTH / 2, SHEIGHT / 48 * 7.5), 10.f, Color(255, 255, 255, 255), true, false, f_Verdana);
		Render::Get().RenderBoxFilled(startPos.x, startPos.y, endPos.x, endPos.y, Color(35, 35, 35, 200), 1, 12.f);
		Render::Get().RenderBoxFilled(startPos.x + 3, startPos.y + 2, startPos.x + (width / 100 * perc), endPos.y - 2, Color(255, 140, 0, 230), 1, 12.f);
		Render::Get().RenderText(timer, ImVec2(SWIDTH / 2, SHEIGHT / 48 * 8.5), 10.f, Color(255, 255, 255, 255), true, false, f_Verdana);

		startPos = startPos = ImVec2(SWIDTH / 8 * 3, SHEIGHT / 48 * 9);
		endPos = ImVec2(SWIDTH / 8 * 5, SHEIGHT / 48 * 9 + 10);
		width = endPos.x - startPos.x - 6;
		C_BasePlayer* defuser = c4->m_hBombDefuser();

		if (defuser)
		{
			float countdown = c4->m_flDefuseCountDown() - g_GlobalVars->curtime;
			perc = countdown / c4->m_flDefuseLength() * 100;

			if (countdown > 0.f)
			{
				std::string count = std::to_string(countdown);

				Render::Get().RenderBoxFilled(startPos.x, startPos.y, endPos.x, endPos.y, Color(35, 35, 35, 200), 1, 12.f);
				Render::Get().RenderBoxFilled(startPos.x + 3, startPos.y + 2, startPos.x + (width / 100 * perc), endPos.y - 2, Color(173, 216, 230), 1, 12.f);
				Render::Get().RenderText(count, ImVec2(SWIDTH / 2, SHEIGHT / 48 * 9.5), 10.f, Color(255, 255, 255, 255), true, false, f_Verdana);
			}
		}
	}
}

bool CanWallbang(float& dmg)
{
	if (!g_LocalPlayer)
		return false;

	Autowall::FireBulletData data;
	data.src = g_LocalPlayer->SelfOrObs()->GetEyePos();
	data.filter = CTraceFilter();
	data.filter.pSkip = g_LocalPlayer->SelfOrObs();

	QAngle EyeAng;
	g_EngineClient->GetViewAngles(&EyeAng);

	Vector dst, forward;

	Math::AngleVectors(EyeAng, forward);
	dst = data.src + (forward * 8196.f);

	QAngle angles = Math::CalcAngle(data.src, dst);
	Math::AngleVectors(angles, data.direction);
	data.direction.Normalized();

	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)C_BaseEntity::get_entity_from_handle(g_LocalPlayer->SelfOrObs()->m_hActiveWeapon());

	if (!weapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CCSWeaponInfo* weaponData = weapon->GetCSWeaponData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->iDamage;

	data.trace_length_remaining = weaponData->flRange - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	Ray_t ray;
	ray.Init(data.src, end);

	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace);

	if (data.enter_trace.fraction == 1.0f)
		return false;

	if (Autowall::HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}

void WallbangCrosshair()
{
	if (!Settings::Misc::WallbangCrosshair)
		return;

	if (!g_LocalPlayer)
		return;

	float damage = 0.f;

	INT SWIDTH, SHEIGHT;
	g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

	if (CanWallbang(damage))
	{
		Render::Get().RenderBox(SWIDTH / 2 - 2.5, SHEIGHT / 2 - 2.5, SWIDTH / 2 + 4.5, SHEIGHT / 2 + 4.5, Color(0, 255, 0), 1.f, 0.f);
	}
	else
	{
		Render::Get().RenderBox(SWIDTH / 2 - 2.5, SHEIGHT / 2 - 2.5, SWIDTH / 2 + 4.5, SHEIGHT / 2 + 4.5, Color(255, 0, 0), 1.f, 0.f);
	}
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

void SmokeHelper::Draw()
{
	Found = false;

	if (!Settings::Misc::SmokeHelper::Enabled)
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !g_LocalPlayer->m_hActiveWeapon())
		return;

	const char* levelname = g_ClientState->szLevelName;
	std::vector<GrenadeData> spots;

	switch (str2int(levelname))
	{
	case str2int("\\de_cbble.bsp"):
		spots = Data_cbble;
		break;
	case str2int("\\de_mirage.bsp"):
		spots = Data_mirage;
		break;
	case str2int("\\de_dust2.bsp"):
		spots = Data_dust2;
		break;
	case str2int("\\de_inferno.bsp"):
		spots = Data_inferno;
		break;
	case str2int("\\de_nuke.bsp"):
		spots = Data_nuke;
		break;
	case str2int("\\de_overpass.bsp"):
		spots = Data_overpass;
		break;
	case str2int("\\de_train.bsp"):
		spots = Data_train;
		break;
	case str2int("\\de_cache.bsp"):
		spots = Data_cache;
		break;
	}

	if (!g_LocalPlayer->m_hActiveWeapon()->IsGrenade())
		return;

	ClassId id = g_LocalPlayer->m_hActiveWeapon()->GetClientClass()->m_ClassID;

	for (GrenadeData spot : spots)
	{
		if ((id == ClassId_CSmokeGrenade && spot.Weapon == "Smoke")
			|| (id == ClassId_CFlashbang && spot.Weapon == "Flashbang")
			|| (id == ClassId_CMolotovGrenade && spot.Weapon == "Molotov")
			|| (id == ClassId_CIncendiaryGrenade && spot.Weapon == "Molotov"))
		{
			Vector pos = Vector(spot.XPos, spot.YPos, spot.ZPos);
			Vector ph;

			/*Ray_t ray;
			ray.Init(g_LocalPlayer->GetEyePos(), pos);
			CTraceFilter filter;
			filter.pSkip = g_LocalPlayer;
			trace_t tr;
			g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);*/
			// todo, don't show ones that you can't see. Trace line from eye to pos to see if you can actually see it or not.

			Render::Get().RenderCircle3D(pos, 36, 10, Color(255, 255, 255, 255), 2.f);

			if (pos.DistTo(g_LocalPlayer->m_vecOrigin()) < 10)
			{
				selData = spot;
				Found = true;

				QAngle dir = QAngle(spot.XAng, spot.YAng, 0);
				Vector va;

				Math::AngleVectors(dir, va);

				Vector po = g_LocalPlayer->GetEyePos() + va * 5;
				Vector scre;

				if (!Math::WorldToScreen(po, scre))
					continue;

				Render::Get().RenderCircle(scre.x, scre.y, 5, 36, Color(255, 0, 0, 255), 1.f);

				float cr = scre.y;
				Render::Get().RenderText(spot.Name, ImVec2(scre.x + 10, cr), 14.f, Color(255, 0, 0, 255), false, true, f_AndaleMono);
				ImVec2 sz = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.f, spot.Name.c_str());
				cr += sz.y + 5;

				Render::Get().RenderText(spot.Desc, ImVec2(scre.x + 10, cr), 14.f, Color(255, 0, 0, 255), false, true, f_AndaleMono);
				sz = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.f, spot.Desc.c_str());
				cr += sz.y + 5;

				Render::Get().RenderText(spot.Weapon, ImVec2(scre.x + 10, cr), 14.f, Color(255, 0, 0, 255), false, true, f_AndaleMono);
				sz = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.f, spot.Weapon.c_str());
				cr += sz.y + 5;
			}
		}
	}
}

void SmokeHelper::OnCreateMove(CUserCmd* cmd)
{
	if (!Settings::Misc::SmokeHelper::Enabled)
		return;

	if (!Found)
		return;

	if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT || cmd->buttons & IN_JUMP)
		return;

	switch (Settings::Misc::SmokeHelper::TypeHelp)
	{
	case 1: {
		float yaw = cmd->viewangles.yaw;
		Vector VecForward = g_LocalPlayer->m_vecOrigin() - Vector(selData.XPos, selData.YPos, selData.ZPos);

		Vector translatedVelocity = Vector(
			(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
			(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
			VecForward.z
		);

		cmd->forwardmove = -translatedVelocity.x * 10.f;
		cmd->sidemove = translatedVelocity.y * 10.f;

		if (Math::GetFOV(cmd->viewangles, QAngle(selData.XAng, selData.YAng, 0)) < 1.5)
		{
			cmd->viewangles -= (cmd->viewangles - QAngle(selData.XAng, selData.YAng, 0)) / 3.5;
			g_EngineClient->SetViewAngles(&cmd->viewangles);
		}
		break;
	}
	case 2: {
		float yaw = cmd->viewangles.yaw;
		Vector VecForward = g_LocalPlayer->m_vecOrigin() - Vector(selData.XPos, selData.YPos, selData.ZPos);

		Vector translatedVelocity = Vector(
			(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
			(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
			VecForward.z
		);

		cmd->forwardmove = -translatedVelocity.x * 10.f;
		cmd->sidemove = translatedVelocity.y * 10.f;

		cmd->viewangles = QAngle(selData.XAng, selData.YAng, 0);
		g_EngineClient->SetViewAngles(&cmd->viewangles);
		break;
	}
	case 3: {
		float yaw = cmd->viewangles.yaw;
		Vector VecForward = g_LocalPlayer->m_vecOrigin() - Vector(selData.XPos, selData.YPos, selData.ZPos);

		Vector translatedVelocity = Vector(
			(float)(VecForward.x * cos(yaw / 180 * PI_F) + VecForward.y * sin(yaw / 180 * PI_F)),
			(float)(VecForward.y * cos(yaw / 180 * PI_F) - VecForward.x * sin(yaw / 180 * PI_F)),
			VecForward.z
		);

		cmd->forwardmove = -translatedVelocity.x * 10.f;
		cmd->sidemove = translatedVelocity.y * 10.f;

		cmd->viewangles = QAngle(selData.XAng, selData.YAng, 0);
		g_EngineClient->SetViewAngles(&cmd->viewangles);

		if (g_LocalPlayer->m_vecVelocity().Length() <= 0.05f && Math::GetFOV(cmd->viewangles, QAngle(selData.XAng, selData.YAng, 0)) < .05f)
		{
			if (selData.Desc == "Throw")
			{
				const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(g_LocalPlayer->m_hActiveWeapon());

				if (grenade->m_bPinPulled() && grenade->m_flThrowStrength() >= 1.00000f)
				{
					cmd->buttons &= ~IN_ATTACK;
					cmd->buttons &= ~IN_ATTACK2;
				}
				else
				{
					cmd->buttons |= IN_ATTACK;
					cmd->buttons &= ~IN_ATTACK2;
				}
			}
			else if (selData.Desc == "Jump / Throw")
			{
				const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(g_LocalPlayer->m_hActiveWeapon());

				if (grenade->m_bPinPulled() && grenade->m_flThrowStrength() >= 1.00000f)
				{
					cmd->buttons |= IN_JUMP;

					if (g_LocalPlayer->m_vecVelocity().z > 240)
					{
						cmd->buttons &= ~IN_ATTACK;
						cmd->buttons &= ~IN_ATTACK2;
					}
				}
				else
				{
					cmd->buttons |= IN_ATTACK;
					cmd->buttons &= ~IN_ATTACK2;
				}
			}
			else if (selData.Desc == "Right Click")
			{
				const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(g_LocalPlayer->m_hActiveWeapon());

				if (grenade->m_bPinPulled() && grenade->m_flThrowStrength() <= 0.00000f)
				{

					cmd->buttons &= ~IN_ATTACK;
					cmd->buttons &= ~IN_ATTACK2;
				}
				else
				{
					cmd->buttons &= ~IN_ATTACK;
					cmd->buttons |= IN_ATTACK2;
				}
			}
		}

		break;
	}
	}
}

void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

void ZeusRange()
{
	if (!Settings::Misc::ZeusRange::Enabled)
		return;

	Vector prev_scr_pos, scr_pos;
	C_BaseCombatWeapon* local_weapon = g_LocalPlayer->SelfOrObs()->m_hActiveWeapon();

	if (!local_weapon || local_weapon->GetCSWeaponData()->iWeaponType != WEAPONTYPE_KNIFE || !g_Input->m_fCameraInThirdPerson)
		return;

	float step = Settings::Misc::ZeusRange::Step;
	float dist = 0.f;

	if (local_weapon->IsKnife())
	{
		dist = 64.f;
	}
	else if (local_weapon->m_iItemDefinitionIndex() == WEAPON_TASER)
	{
		dist = 180.f;
	}
	else
	{
		return;
	}

	Vector origin = g_LocalPlayer->SelfOrObs()->GetEyePos();
	static float hue_offset = 0;
	for (float rotation = 0; rotation < 360.f + step; rotation += step)
	{
		Vector pos(dist * cos(DEG2RAD(rotation)) + origin.x, dist * sin(DEG2RAD(rotation)) + origin.y, origin.z);

		Ray_t ray;
		trace_t trace;
		CTraceFilter filter;

		filter.pSkip = g_LocalPlayer->SelfOrObs();
		ray.Init(origin, pos);

		g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace);

		if (Math::WorldToScreen(trace.endpos, scr_pos))
		{
			if (prev_scr_pos.IsValid())
			{
				int hue = rotation + hue_offset;

				float r, g, b;
				ColorConvertHSVtoRGB(hue / 360.f, 1, 1, r, g, b);
				Color color = Color(r, g, b);

				Render::Get().RenderLine(prev_scr_pos.x, prev_scr_pos.y, scr_pos.x, scr_pos.y, color, Settings::Misc::ZeusRange::Thickness);
			}
			prev_scr_pos = scr_pos;
		}
	}
	hue_offset += Settings::Misc::ZeusRange::Speed;
}

void DebugMode()
{
	if (!Settings::Misc::DebugMode)
		return;

	if (!g_LocalPlayer)
		return;

	INT SWIDTH, SHEIGHT;
	g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

	ImVec2 cr = ImVec2(SWIDTH / 1.75, SHEIGHT / 2);

	std::string s;
	s += "LOCALPLAYER 0x";
	s += std::to_string(g_LocalPlayer);
	ImVec2 sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	s = "";
	s += "ORIGIN V(";
	s += std::to_string(g_LocalPlayer->m_vecOrigin().x);
	s += ", ";
	s += std::to_string(g_LocalPlayer->m_vecOrigin().y);
	s += ", ";
	s += std::to_string(g_LocalPlayer->m_vecOrigin().z);
	s += ")";
	sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	s = "";
	s += "VELOCITY V(";
	s += std::to_string(g_LocalPlayer->m_vecVelocity().x);
	s += ", ";
	s += std::to_string(g_LocalPlayer->m_vecVelocity().y);
	s += ", ";
	s += std::to_string(g_LocalPlayer->m_vecVelocity().z);
	s += ")";
	sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	s = "";
	s += "FLVELOCITY ";
	s += std::to_string(g_LocalPlayer->m_vecVelocity().Length());
	s += "f";
	sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	s = "";
	s += "EYEPOS V(";
	s += std::to_string(g_LocalPlayer->GetEyePos().x);
	s += ", ";
	s += std::to_string(g_LocalPlayer->GetEyePos().y);
	s += ", ";
	s += std::to_string(g_LocalPlayer->GetEyePos().z);
	s += ")";
	sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	QAngle angles;
	g_EngineClient->GetViewAngles(&angles);
	s = "";
	s += "EYEANGLES Q(";
	s += std::to_string(angles.yaw);
	s += ", ";
	s += std::to_string(angles.pitch);
	s += ", ";
	s += std::to_string(angles.roll);
	s += ")";
	sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
	Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
	cr.y += sz.y + 2;

	const auto weapon = g_LocalPlayer->m_hActiveWeapon();
	if (weapon && weapon->IsGrenade())
	{
		s = "";
		const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(weapon);
		s += "THROWSTRENGTH ";
		s += std::to_string(grenade->m_flThrowStrength());
		sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
		Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
		cr.y += sz.y + 2;
	}
}