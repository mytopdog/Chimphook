#include "ESP.hpp"

#include "Screen.hpp"
#include "../Aimbot/Backtracking.hpp"

bool Settings::ESP::Players::Enabled = false;

bool Settings::ESP::Players::Enemies::Enabled = false;
bool Settings::ESP::Players::Teammates::Enabled = false;
bool Settings::ESP::Players::Localplayer::Enabled = false;

bool Settings::ESP::Players::Enemies::Occluded = false;
bool Settings::ESP::Players::Teammates::Occluded = false;
bool Settings::ESP::Players::Localplayer::Occluded = false;

//* Enemies *//
bool Settings::ESP::Players::Enemies::Boxes = false;
bool Settings::ESP::Players::Enemies::Names = false;
bool Settings::ESP::Players::Enemies::Skeletons = false;
bool Settings::ESP::Players::Enemies::BacktrackSkeletons = false;
bool Settings::ESP::Players::Enemies::Health = false;
bool Settings::ESP::Players::Enemies::Weapons = false;
bool Settings::ESP::Players::Enemies::Snaplines = false;
bool Settings::ESP::Players::Enemies::Positions = false;
bool Settings::ESP::Players::Enemies::EyeLines = false;

Color Settings::ESP::Players::Enemies::Colours::Boxes = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Names = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Skeletons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::BacktrackSkeletons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Health = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Weapons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Snaplines = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Positions = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::EyeLines = Color(255, 255, 255, 255);

//* Teammates *//
bool Settings::ESP::Players::Teammates::Boxes = false;
bool Settings::ESP::Players::Teammates::Names = false;
bool Settings::ESP::Players::Teammates::Skeletons = false;
bool Settings::ESP::Players::Teammates::Health = false;
bool Settings::ESP::Players::Teammates::Weapons = false;
bool Settings::ESP::Players::Teammates::Snaplines = false;
bool Settings::ESP::Players::Teammates::Positions = false;
bool Settings::ESP::Players::Teammates::EyeLines = false;

Color Settings::ESP::Players::Teammates::Colours::Boxes = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Names = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Skeletons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Health = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Weapons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Snaplines = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::Positions = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Teammates::Colours::EyeLines = Color(255, 255, 255, 255);

//* Localplayer *//
bool Settings::ESP::Players::Localplayer::Box = false;
bool Settings::ESP::Players::Localplayer::Name = false;
bool Settings::ESP::Players::Localplayer::Skeleton = false;
bool Settings::ESP::Players::Localplayer::Health = false;
bool Settings::ESP::Players::Localplayer::Weapon = false;
bool Settings::ESP::Players::Localplayer::Position = false;

Color Settings::ESP::Players::Localplayer::Colours::Box = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Name = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Skeleton = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Health = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Weapon = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Position = Color(255, 255, 255, 255);

bool Settings::ESP::Others::Grenades::Enabled = false;
Color Settings::ESP::Others::Grenades::Colour = Color(255, 0, 0);

bool Settings::ESP::Others::C4::Enabled = false;
Color Settings::ESP::Others::C4::Colour = Color(255, 0, 0);

bool Settings::Visuals::BulletShots::Enabled = false;
Color Settings::Visuals::BulletShots::Colour = Color(255, 255, 255);

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++)
	{
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++)
	{
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++)
	{
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

void Visuals::Player::Begin(C_BasePlayer* pl)
{
	if (pl->IsDormant() || !pl->IsAlive())
		return;

	ctx.pl = pl;
	ctx.is_localplayer = pl == g_LocalPlayer;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_teammate = g_LocalPlayer->m_iTeamNum() == pl->m_iTeamNum() && !ctx.is_localplayer;
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (ctx.is_enemy && !Settings::ESP::Players::Enemies::Enabled)
	{
		return;
	}
	else if (ctx.is_teammate && !Settings::ESP::Players::Teammates::Enabled)
	{
		return;
	}
	else if (ctx.is_localplayer && !Settings::ESP::Players::Localplayer::Enabled)
	{
		return;
	}

	if (ctx.is_enemy && !ctx.is_visible && !Settings::ESP::Players::Enemies::Occluded)
		return;

	if (ctx.is_teammate && !ctx.is_visible && !Settings::ESP::Players::Teammates::Occluded)
		return;

	if (ctx.is_localplayer && !ctx.is_visible && !Settings::ESP::Players::Localplayer::Occluded)
		return;

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();
	head.z += 15;

	Math::WorldToScreen(head, ctx.head_pos);
	Math::WorldToScreen(origin, ctx.feet_pos);

	auto h = fabs(ctx.head_pos.y - ctx.feet_pos.y);
	auto w = h / 1.65f;

	ctx.bbox.left = ctx.feet_pos.x - w * 0.5f;
	ctx.bbox.right = ctx.bbox.left + w;
	ctx.bbox.bottom = ctx.feet_pos.y;
	ctx.bbox.top = ctx.head_pos.y;

	ImVec2 sz = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.f, "Sample");

	ctx.cursor.x = ctx.bbox.right + 5;
	ctx.cursor.y = ctx.bbox.top + sz.y;

	ctx.distance = Math::VectorDistance(origin, g_LocalPlayer->m_vecOrigin());

	ctx.font_size = 12.f;

	if (ctx.is_enemy && Settings::ESP::Players::Enemies::Snaplines)
		RenderSnapline(Settings::ESP::Players::Enemies::Colours::Snaplines);

	if (ctx.is_teammate && Settings::ESP::Players::Teammates::Snaplines)
		RenderSnapline(Settings::ESP::Players::Teammates::Colours::Snaplines);

	if (!Math::WorldToScreen(head, ctx.head_pos) ||
		!Math::WorldToScreen(origin, ctx.feet_pos))
		return;

	if (ctx.is_enemy)
	{
		if (Settings::ESP::Players::Enemies::Boxes)
			RenderBox(Settings::ESP::Players::Enemies::Colours::Boxes);

		if (Settings::ESP::Players::Enemies::Skeletons)
			RenderSkeleton(Settings::ESP::Players::Enemies::Colours::Skeletons);

		if (Settings::ESP::Players::Enemies::BacktrackSkeletons)
			RenderBacktrackSkeleton(Settings::ESP::Players::Enemies::Colours::BacktrackSkeletons);

		if (Settings::ESP::Players::Enemies::Names)
			RenderName(Settings::ESP::Players::Enemies::Colours::Names);

		if (Settings::ESP::Players::Enemies::Health)
			RenderHealth(Settings::ESP::Players::Enemies::Colours::Health);

		if (Settings::ESP::Players::Enemies::Weapons)
			RenderWeapon(Settings::ESP::Players::Enemies::Colours::Weapons);

		if (Settings::ESP::Players::Enemies::Positions)
			RenderPosition(Settings::ESP::Players::Enemies::Colours::Positions);

		if (Settings::ESP::Players::Enemies::EyeLines)
			RenderEyeLine(Settings::ESP::Players::Enemies::Colours::EyeLines);
	}
	else if (ctx.is_teammate)
	{
		if (Settings::ESP::Players::Teammates::Boxes)
			RenderBox(Settings::ESP::Players::Teammates::Colours::Boxes);

		if (Settings::ESP::Players::Teammates::Skeletons)
			RenderSkeleton(Settings::ESP::Players::Teammates::Colours::Skeletons);

		if (Settings::ESP::Players::Teammates::Names)
			RenderName(Settings::ESP::Players::Teammates::Colours::Names);

		if (Settings::ESP::Players::Teammates::Health)
			RenderHealth(Settings::ESP::Players::Teammates::Colours::Health);

		if (Settings::ESP::Players::Teammates::Weapons)
			RenderWeapon(Settings::ESP::Players::Teammates::Colours::Weapons);

		if (Settings::ESP::Players::Teammates::Positions)
			RenderPosition(Settings::ESP::Players::Teammates::Colours::Positions);

		if (Settings::ESP::Players::Teammates::EyeLines)
			RenderEyeLine(Settings::ESP::Players::Teammates::Colours::EyeLines);
	}
	else if (ctx.is_localplayer)
	{
		if (Settings::ESP::Players::Localplayer::Box)
			RenderBox(Settings::ESP::Players::Localplayer::Colours::Box);

		if (Settings::ESP::Players::Localplayer::Skeleton)
			RenderSkeleton(Settings::ESP::Players::Localplayer::Colours::Skeleton);

		if (Settings::ESP::Players::Localplayer::Name)
			RenderName(Settings::ESP::Players::Localplayer::Colours::Name);

		if (Settings::ESP::Players::Localplayer::Health)
			RenderHealth(Settings::ESP::Players::Localplayer::Colours::Health);

		if (Settings::ESP::Players::Localplayer::Weapon)
			RenderWeapon(Settings::ESP::Players::Localplayer::Colours::Weapon);

		if (Settings::ESP::Players::Localplayer::Position)
			RenderPosition(Settings::ESP::Players::Localplayer::Colours::Position);
	}
}

void Visuals::Player::RenderSkeleton(Color col)
{
	const model_t* itemModel = ctx.pl->GetModel();
	if (!itemModel)
		return;

	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(itemModel);

	if (!hdr)
		return;

	Vector v_parent, v_child, s_parent, s_child;

	for (int i = 0; i < hdr->numbones; i++)
	{
		mstudiobone_t* bone = hdr->GetBone(i);

		if (!bone)
			return;

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1))
		{
			v_child = ctx.pl->GetBonePos(i);
			v_parent = ctx.pl->GetBonePos(bone->parent);

			if (Math::WorldToScreen(v_parent, s_parent) && Math::WorldToScreen(v_child, s_child))
			{
				Render::Get().RenderLine(s_parent[0], s_parent[1], s_child[0], s_child[1], col, 1);
			}
		}
	}
}

void Visuals::Player::RenderBacktrackSkeleton(Color col)
{
	if (!Settings::Backtrack::Enabled)
		return;

	const model_t* itemModel = ctx.pl->GetModel();
	if (!itemModel)
		return;

	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(itemModel);

	if (!hdr)
		return;

	for (int i = 0; i < Backtrack::Get().records[ctx.pl->EntIndex()].size(); i++)
	{
		Vector v_parent, v_child, s_parent, s_child;

		backtrack_record_t record = Backtrack::Get().records[ctx.pl->EntIndex()][i];

		for (int j = 0; j < hdr->numbones; j++)
		{
			mstudiobone_t* bone = hdr->GetBone(j);

			if (!bone)
				continue;

			if ((bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1))
			{
				v_child = Vector(record.matrix[j][0][3], record.matrix[j][1][3], record.matrix[j][2][3]);
				v_parent = Vector(record.matrix[bone->parent][0][3], record.matrix[bone->parent][1][3], record.matrix[bone->parent][2][3]);

				if (Math::WorldToScreen(v_parent, s_parent) && Math::WorldToScreen(v_child, s_child))
				{
					Render::Get().RenderLine(s_parent[0], s_parent[1], s_child[0], s_child[1], col, 1);
				}
			}
		}
	}
}

void Visuals::Player::RenderBox(Color col)
{
	Render::Get().RenderBoxByType(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, col, 1);
	Render::Get().RenderBoxByType(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, Color(0, 0, 0, 255), 1);
	Render::Get().RenderBoxByType(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, col, 1);
}

float fs = 14.f;

void Visuals::Player::RenderName(Color col)
{
	player_info_t info = ctx.pl->GetPlayerInfo();
	ImVec2 sz = f_AndaleMono->CalcTextSizeA(ctx.font_size, FLT_MAX, 0.0f, info.szName);

	std::string name;
	if (info.fakeplayer)
		name += "BOT ";

	name += info.szName;

	if (!ctx.pl->notfriend)
		name += "*";

	Render::Get().RenderText(name, ImVec2(ctx.head_pos.x, ctx.bbox.top - 5), ctx.font_size, col, true, true, f_AndaleMono);
}

void Visuals::Player::RenderHealth(Color col)
{
	int hp = ctx.pl->m_iHealth();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	float off = 8;

	int height = (box_h * hp) / 100;

	int green = int(hp * 2.55f);
	int red = 255 - green;

	int x = ctx.bbox.left - off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y, x + w, y + h, Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y + h - 2, x + w - 1, y + h - height + 2, Color(red, green, 0, 255), 1.f, true);


	std::string textHP = std::to_string(ctx.pl->m_iHealth());
	textHP += "%";

	ImVec2 sz = f_AndaleMono->CalcTextSizeA(ctx.font_size, FLT_MAX, 0.0f, textHP.c_str());

	Render::Get().RenderText(textHP.c_str(), ImVec2(ctx.cursor.x, ctx.cursor.y), ctx.font_size, col, false, true, f_AndaleMono);

	ctx.cursor.y += sz.y + 2;
}

void Visuals::Player::RenderWeapon(Color col)
{
	if (!ctx.pl->m_hActiveWeapon())
		return;

	const char* weapon = ctx.pl->m_hActiveWeapon()->GetCSWeaponData()->szWeaponName;

	ImVec2 sz = f_AndaleMono->CalcTextSizeA(ctx.font_size, FLT_MAX, 0.0f, weapon);

	Render::Get().RenderText(weapon, ImVec2(ctx.cursor.x, ctx.cursor.y), ctx.font_size, col, false, true, f_AndaleMono);

	ctx.cursor.y += sz.y + 2;
}

void Visuals::Player::RenderPosition(Color col)
{
	if (!ctx.pl->m_szLastPlaceName())
		return;

	const char* position = ctx.pl->m_szLastPlaceName();

	ImVec2 sz = f_AndaleMono->CalcTextSizeA(ctx.font_size, FLT_MAX, 0.0f, position);

	Render::Get().RenderText(position, ImVec2(ctx.cursor.x, ctx.cursor.y), ctx.font_size, col, false, true, f_AndaleMono);

	ctx.cursor.y += sz.y + 2;
}

void Visuals::Player::RenderSnapline(Color col)
{
	int SWIDTH, SHEIGHT;
	g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

	Vector pos;
	Math::WorldToScreen(ctx.pl->GetHitboxPos(HITBOX_PELVIS), pos);

	Render::Get().RenderLine((float)SWIDTH / 2.f, (float)SHEIGHT / 2.f, pos.x, pos.y, col, 1);
}

void Visuals::Player::RenderEyeLine(Color col)
{
	if (ctx.pl == g_LocalPlayer)
		return;

	C_BasePlayer* pl = ctx.pl;
	QAngle viewangles = pl->m_angEyeAngles();
	Vector viewvec;

	Math::AngleVectors(viewangles, viewvec);

	Ray_t ray;
	ray.Init(ctx.head_pos, ctx.head_pos + viewvec * 8192);
	CTraceFilter filter;
	filter.pSkip = ctx.pl;
	trace_t trace;

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	Vector startPos;
	Vector endPos;

	Math::WorldToScreen(ctx.head_pos, startPos);
	Math::WorldToScreen(trace.endpos, endPos);

	Render::Get().RenderLine(startPos.x, startPos.y, endPos.x, endPos.y, Color(col.r(), col.g(), col.b(), 100));
}

char* GetGrenadeName(C_BaseEntity* grenade)
{
	const model_t* nadeModel = grenade->GetModel();

	if (!nadeModel)
		return NULL;

	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return NULL;

	if (!strstr(hdr->szName, "thrown") && !strstr(hdr->szName, "dropped"))
		return NULL;

	char* nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	g_MdlInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE Grenade";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}

	return nadeName;
}

void DoGrenadeESP(C_BaseEntity* grenade)
{
	RECT BBox = GetBBox(grenade);
	char* name = GetGrenadeName(grenade);

	Vector mid_;

	if (!Math::WorldToScreen(grenade->m_vecOrigin(), mid_))
		return;

	if (name)
	{
		Render::Get().RenderBox(BBox, Settings::ESP::Others::Grenades::Colour);

		Render::Get().RenderText(name, ImVec2(BBox.right + 2, BBox.top), 14.f, Settings::ESP::Others::Grenades::Colour, false, true, f_AndaleMono);

		if (strstr(name, "Smoke"))
		{
			C_SmokeGrenadeProjectile* gren = (C_SmokeGrenadeProjectile*)grenade;

			if (gren->m_nSmokeEffectTickBegin())
			{
				float time = g_GlobalVars->interval_per_tick * (g_GlobalVars->tickcount - gren->m_nSmokeEffectTickBegin());

				if (time < 17.5)
				{
					RECT nbox;
					nbox.left = BBox.left - 30;
					nbox.right = BBox.right + 30;

					float w = nbox.right - nbox.left;
					float perc = (17.5 - time) / 17.5 * 100;

					Render::Get().RenderBoxFilled(nbox.left, BBox.bottom - 6, nbox.right, BBox.bottom - 2, Color(35, 35, 35, 255), 1, 4.f);
					Render::Get().RenderBoxFilled((float)nbox.left, (float)BBox.bottom - 6, nbox.left + (w / 100 * perc), (float)BBox.bottom - 2, Color(255, 140, 0, 230), 1, 4.f);
				}
			}
		}
		if (strstr(name, "Decoy"))
		{
			C_DecoyGrenadeProjectile* gren = (C_DecoyGrenadeProjectile*)grenade;
		}
	}
}

void DoC4ESP(C_BaseEntity* c4ent)
{
	C_PlantedC4* c4 = (C_PlantedC4*)c4ent;

	if (c4->m_bBombTicking())
	{
		RECT BBox = GetBBox(c4);

		Vector mid_;

		if (!Math::WorldToScreen(c4->m_vecOrigin(), mid_))
			return;

		Render::Get().RenderBox(BBox, Settings::ESP::Others::Grenades::Colour);

		float bombTimer = c4->m_flC4Blow() - g_GlobalVars->curtime;
		std::string timer = bombTimer > 0 ? std::to_string(bombTimer) : "0.0";

		ImVec2 sz1 = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.0f, "Bomb");
		ImVec2 sz2 = f_AndaleMono->CalcTextSizeA(14.f, FLT_MAX, 0.0f, timer.c_str());
		int w = BBox.right - BBox.left;

		Render::Get().RenderText("Bomb", ImVec2(BBox.right + 2, BBox.bottom), 14.f, Settings::ESP::Others::C4::Colour, false, true, f_AndaleMono);
		Render::Get().RenderText(timer, ImVec2(BBox.right + 2, BBox.bottom + sz1.y + 2), 14.f, Settings::ESP::Others::C4::Colour, false, true, f_AndaleMono);
	}
}

void Visuals::AddToDrawList()
{
	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity == g_LocalPlayer && !g_Input->m_fCameraInThirdPerson)
			continue;

		if (entity->IsPlayer() && Settings::ESP::Players::Enabled)
		{
			Player player = Player();
			player.Begin((C_BasePlayer*)entity);
		}
		else if (entity->IsGrenade())
		{
			if (Settings::ESP::Others::Grenades::Enabled) DoGrenadeESP(entity);
		}
		else if (entity->IsPlantedC4())
		{
			if (Settings::ESP::Others::C4::Enabled) DoC4ESP(entity);
			DrawC4Damage(entity);
		}
	}
}

void BulletShots(IGameEvent* ev)
{
	if (!Settings::Visuals::BulletShots::Enabled)
		return;

	if (!strstr(ev->GetName(), "bullet_impact"))
		return;

	C_BasePlayer* pl = C_BasePlayer::GetPlayerByUserId(ev->GetInt("userid"));

	if (!pl || pl->IsDormant())
		return;

	BeamInfo_t beamInfo;

	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = "sprites/physbeam.vmt";
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 2.5f;
	beamInfo.m_flWidth = 1.f;
	beamInfo.m_flEndWidth = 1.5f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = 255.f;
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = Settings::Visuals::BulletShots::Colour.r();
	beamInfo.m_flGreen = Settings::Visuals::BulletShots::Colour.g();
	beamInfo.m_flBlue = Settings::Visuals::BulletShots::Colour.b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

	beamInfo.m_vecStart = pl->GetEyePos();
	beamInfo.m_vecEnd = Vector(ev->GetFloat("x"), ev->GetFloat("y"), ev->GetFloat("z"));

	Beam_t* beam = g_RenderBeams->CreateBeamPoints(beamInfo);

	if (beam)
		g_RenderBeams->DrawBeam(beam);
}