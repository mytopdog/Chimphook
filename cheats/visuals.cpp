#include "visuals.hpp"
#include "autowall.hpp"
#include "../helpers/skinchangerparser.hpp"
bool Settings::ESP::GrenadePrediction = false;
bool Settings::Misc::WallbangCrosshair = false;

bool Settings::ESP::Players::Enabled = false;

bool Settings::ESP::Players::Enemies::Enabled = false;
bool Settings::ESP::Players::Teammates::Enabled = false;
bool Settings::ESP::Players::Localplayer::Enabled  = false;

bool Settings::ESP::Players::Enemies::Occluded = false;
bool Settings::ESP::Players::Teammates::Occluded = false;
bool Settings::ESP::Players::Localplayer::Occluded = false;

//* Enemies *//
bool Settings::ESP::Players::Enemies::Boxes = false;
bool Settings::ESP::Players::Enemies::Names = false;
bool Settings::ESP::Players::Enemies::Skeletons = false;
bool Settings::ESP::Players::Enemies::Health = false;
bool Settings::ESP::Players::Enemies::Weapons = false;
bool Settings::ESP::Players::Enemies::Snaplines = false;
bool Settings::ESP::Players::Enemies::Positions = false;
bool Settings::ESP::Players::Enemies::EyeLines = false;

Color Settings::ESP::Players::Enemies::Colours::Boxes = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Names = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Enemies::Colours::Skeletons = Color(255, 255, 255, 255);
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
bool Settings::ESP::Players::Localplayer::Boxes = false;
bool Settings::ESP::Players::Localplayer::Names = false;
bool Settings::ESP::Players::Localplayer::Skeletons = false;
bool Settings::ESP::Players::Localplayer::Health = false;
bool Settings::ESP::Players::Localplayer::Weapons = false;
bool Settings::ESP::Players::Localplayer::Positions = false;

Color Settings::ESP::Players::Localplayer::Colours::Boxes = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Names = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Skeletons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Health = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Weapons = Color(255, 255, 255, 255);
Color Settings::ESP::Players::Localplayer::Colours::Positions = Color(255, 255, 255, 255);

bool Settings::ESP::Others::Grenades::Enabled           = false;
Color Settings::ESP::Others::Grenades::Colour           = Color(255, 0, 0);

bool Settings::ESP::Others::C4::Enabled = false;
Color Settings::ESP::Others::C4::Colour = Color(255, 0, 0);

bool Settings::Visuals::BulletShots::Enabled            = false;
Color Settings::Visuals::BulletShots::Colour            = Color(255, 255, 255);

bool Settings::Visuals::Chams::Players::Enabled;

bool Settings::Visuals::Chams::Players::Enemies::Enabled         = false;
bool Settings::Visuals::Chams::Players::Teammates::Enabled       = false;
bool Settings::Visuals::Chams::Players::Localplayer::Enabled     = false;

bool Settings::Visuals::Chams::Players::Enemies::DoOccluded = false;
bool Settings::Visuals::Chams::Players::Teammates::DoOccluded = false;
bool Settings::Visuals::Chams::Players::Localplayer::DoOccluded = false;

int Settings::Visuals::Chams::Players::Enemies::Material         = 0;
int Settings::Visuals::Chams::Players::Teammates::Material       = 0;
int Settings::Visuals::Chams::Players::Localplayer::Material     = 0;

Color Settings::Visuals::Chams::Players::Enemies::Visible = Color(200, 0, 0);
Color Settings::Visuals::Chams::Players::Enemies::Occluded = Color(100, 0, 0);
Color Settings::Visuals::Chams::Players::Teammates::Visible = Color(0, 0, 200);
Color Settings::Visuals::Chams::Players::Teammates::Occluded = Color(0, 0, 100);
Color Settings::Visuals::Chams::Players::Localplayer::Visible = Color(0, 200, 0);
Color Settings::Visuals::Chams::Players::Localplayer::Occluded = Color(0, 100, 0);

bool Settings::Visuals::Chams::Viewmodel::Arms::Enabled         = false;
bool Settings::Visuals::Chams::Viewmodel::Sleeves::Enabled      = false;
bool Settings::Visuals::Chams::Viewmodel::Weapons::Enabled      = false;

bool Settings::Visuals::Chams::Viewmodel::Arms::Wireframe         = false;
bool Settings::Visuals::Chams::Viewmodel::Sleeves::Wireframe      = false;
bool Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe      = false;

int Settings::Visuals::Chams::Viewmodel::Arms::Material         = 0;
int Settings::Visuals::Chams::Viewmodel::Sleeves::Material      = 0;
int Settings::Visuals::Chams::Viewmodel::Weapons::Material      = 0;

Color Settings::Visuals::Chams::Viewmodel::Arms::Colour         = Color(255, 255, 255);
Color Settings::Visuals::Chams::Viewmodel::Sleeves::Colour      = Color(255, 255, 255);
Color Settings::Visuals::Chams::Viewmodel::Weapons::Colour      = Color(255, 255, 255);

bool Settings::SkinChanger::Enabled = false;

int Settings::SkinChanger::KnifeModel = 0;
float Settings::SkinChanger::KnifeWear = 0;
int Settings::SkinChanger::Paint::Knife = 0;
int Settings::SkinChanger::Paint::usp = 0;
int Settings::SkinChanger::Paint::p2000 = 0;
int Settings::SkinChanger::Paint::glock = 0;
int Settings::SkinChanger::Paint::p250 = 0;
int Settings::SkinChanger::Paint::fiveseven = 0;
int Settings::SkinChanger::Paint::tec = 0;
int Settings::SkinChanger::Paint::cz = 0;
int Settings::SkinChanger::Paint::duals = 0;
int Settings::SkinChanger::Paint::deagle = 0;
int Settings::SkinChanger::Paint::revolver = 0;
int Settings::SkinChanger::Paint::famas = 0;
int Settings::SkinChanger::Paint::galil = 0;
int Settings::SkinChanger::Paint::m4a4 = 0;
int Settings::SkinChanger::Paint::m4a1 = 0;
int Settings::SkinChanger::Paint::ak47 = 0;
int Settings::SkinChanger::Paint::sg553 = 0;
int Settings::SkinChanger::Paint::aug = 0;
int Settings::SkinChanger::Paint::ssg08 = 0;
int Settings::SkinChanger::Paint::awp = 0;
int Settings::SkinChanger::Paint::scar = 0;
int Settings::SkinChanger::Paint::g3sg1 = 0;
int Settings::SkinChanger::Paint::mac10 = 0;
int Settings::SkinChanger::Paint::mp5sd = 0;
int Settings::SkinChanger::Paint::mp7 = 0;
int Settings::SkinChanger::Paint::mp9 = 0;
int Settings::SkinChanger::Paint::ppbizon = 0;
int Settings::SkinChanger::Paint::p90 = 0;
int Settings::SkinChanger::Paint::ump45 = 0;
int Settings::SkinChanger::Paint::mag7 = 0;
int Settings::SkinChanger::Paint::nova = 0;
int Settings::SkinChanger::Paint::sawedoff = 0;
int Settings::SkinChanger::Paint::xm1014 = 0;
int Settings::SkinChanger::Paint::m249 = 0;
int Settings::SkinChanger::Paint::negev = 0;

bool Settings::Misc::DeagleSpinner = false;

bool Settings::ESP::Glow::Players::Teammates::Enabled = false;
bool Settings::ESP::Glow::Players::Teammates::Cover = false;
int Settings::ESP::Glow::Players::Teammates::Style = 0;
Color Settings::ESP::Glow::Players::Teammates::Colour = Color(0, 0, 0);

bool Settings::ESP::Glow::Players::Enemies::Enabled = false;
bool Settings::ESP::Glow::Players::Enemies::Cover = false;
int Settings::ESP::Glow::Players::Enemies::Style = 0;
Color Settings::ESP::Glow::Players::Enemies::Colour = Color(0, 0, 0);

bool Settings::Misc::SmokeHelper::Enabled = false;
int Settings::Misc::SmokeHelper::TypeHelp = 0;

bool Settings::Misc::DebugMode = false;

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

Visuals::Visuals()
{
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals()
{
	DeleteCriticalSection(&cs);
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
		if (Settings::ESP::Players::Localplayer::Boxes)
			RenderBox(Settings::ESP::Players::Localplayer::Colours::Boxes);

		if (Settings::ESP::Players::Localplayer::Skeletons)
			RenderSkeleton(Settings::ESP::Players::Localplayer::Colours::Skeletons);

		if (Settings::ESP::Players::Localplayer::Names)
			RenderName(Settings::ESP::Players::Localplayer::Colours::Names);

		if (Settings::ESP::Players::Localplayer::Health)
			RenderHealth(Settings::ESP::Players::Localplayer::Colours::Health);

		if (Settings::ESP::Players::Localplayer::Weapons)
			RenderWeapon(Settings::ESP::Players::Localplayer::Colours::Weapons);

		if (Settings::ESP::Players::Localplayer::Positions)
			RenderPosition(Settings::ESP::Players::Localplayer::Colours::Positions);
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

#define TICK_INTERVAL			( g_GlobalVars->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

void DoGrenadeESP(C_BaseEntity* grenade)
{
	RECT BBox = GetBBox(grenade);
	char* name = GetGrenadeName(grenade);

	Vector mid_;

	if (!Math::WorldToScreen(grenade->m_vecOrigin(), mid_))
		return;

	if (name) {
		Render::Get().RenderBox(BBox, Settings::ESP::Others::Grenades::Colour);

		Render::Get().RenderText(name, ImVec2(BBox.right + 2, BBox.top), 14.f, Settings::ESP::Others::Grenades::Colour, false, true, f_AndaleMono);

		if (strstr(name, "Smoke"))
		{
			C_SmokeGrenadeProjectile* gren = (C_SmokeGrenadeProjectile*)grenade;

			if (gren->m_nSmokeEffectTickBegin()) {
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

	if (c4->m_bBombTicking()) {
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

float CSGO_Armor(float flDamage, int ArmorValue)
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
	C_PlantedC4* c4 = (C_PlantedC4*)c4ent;

	float flDistance = g_LocalPlayer->m_vecOrigin().DistTo(c4->m_vecOrigin());
	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ((flDistance - b) / c);
	float flDamage = a * exp(-d * d);

	int c4Damage = std::ceil(CSGO_Armor(flDamage, g_LocalPlayer->m_ArmorValue()));
	std::string dmgs;
	dmgs += "-";
	dmgs += std::to_string(c4Damage);

	int SWidth, SHeight;
	g_EngineClient->GetScreenSize(SWidth, SHeight);

	if (c4Damage > 0) {
		Render::Get().RenderText(dmgs, ImVec2(20, SHeight / 2), 32.f, Color(255, 255, 255, 255), false, true, f_Verdana);
		ImVec2 sz = f_Verdana->CalcTextSizeA(32.f, FLT_MAX, 0.f, dmgs.c_str());

		if (c4Damage >= g_LocalPlayer->m_iHealth()) {
			Render::Get().RenderText("FATAL", ImVec2(20, SHeight / 2 + sz.y + 2), 32.f, Color(255, 0, 0, 255), false, true, f_Verdana);
		}
	}

	float bombTimer = c4->m_flC4Blow() - g_GlobalVars->curtime;

	if (bombTimer > 0.f && c4->m_bBombTicking())
	{
		int SWIDTH, SHEIGHT;
		g_EngineClient->GetScreenSize(SWIDTH, SHEIGHT);

		ImVec2 startPos = ImVec2(SWIDTH / 8 * 3, SHEIGHT / 48 * 8);
		ImVec2 endPos = ImVec2(SWIDTH / 8 * 5, SHEIGHT / 48 * 8 + 10);
		int width = endPos.x - startPos.x - 6;
		float perc = bombTimer / 40 * 100;
		std::string timer = std::to_string(bombTimer);

		Render::Get().RenderBoxFilled(startPos.x, startPos.y, endPos.x, endPos.y, Color(35, 35, 35, 200), 1, 12.f);
		Render::Get().RenderBoxFilled(startPos.x + 3, startPos.y + 2, startPos.x + (width / 100 * perc), endPos.y - 2, Color(255, 140, 0, 230), 1, 12.f);
		Render::Get().RenderText(timer, ImVec2(SWIDTH / 2, SHEIGHT / 48 * 8.5), 10.f, Color(255, 255, 255, 255), true, false, f_Verdana);
		
		startPos = startPos = ImVec2(SWIDTH / 8 * 3, SHEIGHT / 48 * 9);
		endPos = ImVec2(SWIDTH / 8 * 5, SHEIGHT / 48 * 9 + 10);
		width = endPos.x - startPos.x - 6;
		C_BasePlayer* defuser = c4->m_hBombDefuser();

		if (defuser) {
			float countdown = c4->m_flDefuseCountDown() - g_GlobalVars->curtime;
			perc = countdown / c4->m_flDefuseLength() * 100;

			if (countdown > 0.f) {
				std::string count = std::to_string(countdown);

				Render::Get().RenderBoxFilled(startPos.x, startPos.y, endPos.x, endPos.y, Color(35, 35, 35, 200), 1, 12.f);
				Render::Get().RenderBoxFilled(startPos.x + 3, startPos.y + 2, startPos.x + (width / 100 * perc), endPos.y - 2, Color(173, 216, 230), 1, 12.f);
				Render::Get().RenderText(count, ImVec2(SWIDTH / 2, SHEIGHT / 48 * 9.5), 10.f, Color(255, 255, 255, 255), true, false, f_Verdana);
			}
		}
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

		if (ent && ent->IsPlayer() && !ent->IsDormant()) {
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
						
						if (Settings::AntiAim::Yaw::Type == 3) {
							QAngle origAngle = g_LocalPlayer->m_angEyeAngles();

							g_LocalPlayer->SetAngle2(QAngle(0, CreateMove::FakeAngles.yaw, 0));
							g_RenderView->SetColorModulation(0.f, 0.f, 0.f);
							mat->ColorModulate(0.f, 0.f, 0.f);
							g_MdlRender->ForcedMaterialOverride(mat);
							g_LocalPlayer->DrawModel(1, 255);
							g_MdlRender->ForcedMaterialOverride(nullptr);
							g_LocalPlayer->SetAngle2(QAngle(0, origAngle.yaw, 0));
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
							mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
							mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
							mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							mat->IncrementReferenceCount();
							g_MdlRender->ForcedMaterialOverride(mat);

							ent->DrawModel(1, 255);
						}

						Color col = Settings::Visuals::Chams::Players::Enemies::Visible;
						g_RenderView->SetColorModulation(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->ColorModulate(col.r() / 255.f, col.g() / 255.f, col.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->IncrementReferenceCount();
						g_MdlRender->ForcedMaterialOverride(mat);

						ent->DrawModel(1, 255);
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

			g_RenderView->SetColorModulation(255, 255, 255);
			mat->ColorModulate(255, 255, 255);
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
				mat->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, true);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe);

				g_MdlRender->ForcedMaterialOverride(mat);
			}
		}

		return;
	}
}

void grenade_prediction::Tick(int buttons)
{
	if (!Settings::ESP::GrenadePrediction)
		return;

	bool in_attack = (buttons & IN_ATTACK);
	bool in_attack2 = (buttons & IN_ATTACK2);

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}

void grenade_prediction::View()
{
	if (!Settings::ESP::GrenadePrediction)
		return;

	if (g_LocalPlayer && g_LocalPlayer->IsAlive())
	{
		C_BaseCombatWeapon* weapon = g_LocalPlayer->m_hActiveWeapon();
		if (weapon && weapon->IsGrenade() && act != ACT_NONE)
		{
			QAngle Angles;
			g_EngineClient->GetViewAngles(&Angles);

			type = weapon->m_iItemDefinitionIndex();
			Simulate(Angles, g_LocalPlayer);
		}
		else
		{
			type = 0;
		}
	}
}

void grenade_prediction::Paint()
{
	if (!Settings::ESP::GrenadePrediction)
		return;

	if (!g_LocalPlayer)
		return;

	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	if ((type) && path.size() > 1 && act != ACT_NONE && weapon->IsGrenade())
	{
		Vector ab, cd;
		Vector prev = path[0];
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Math::WorldToScreen(prev, ab) && Math::WorldToScreen(*it, cd))
			{
				Render::Get().RenderLine(ab[0], ab[1], cd[0], cd[1], TracerColor);
			}
			prev = *it;
		}

		for (auto it = OtherCollisions.begin(), end = OtherCollisions.end(); it != end; ++it)
		{
			Render::Get().Render3DCube(2.f, it->second, it->first, Color(0, 255, 0, 200));
		}

		Render::Get().Render3DCube(2.f, OtherCollisions.rbegin()->second, OtherCollisions.rbegin()->first, Color(255, 0, 0, 200));

		std::string EntName;
		auto bestdmg = 0;
		static Color redcol = { 255, 0, 0, 255 };
		static Color greencol = { 25, 255, 25, 255 };
		static Color yellowgreencol = { 177, 253, 2, 255 };
		static Color yellowcol = { 255, 255, 0, 255 };
		static Color orangecol = { 255, 128, 0, 255 };
		Color* BestColor = &redcol;

		Vector endpos = path[path.size() - 1];
		Vector absendpos = endpos;

		float totaladded = 0.0f;

		while (totaladded < 30.0f)
		{
			if (g_EngineTrace->GetPointContents(endpos) == CONTENTS_EMPTY)
				break;

			totaladded += 2.0f;
			endpos.z += 2.0f;
		}

		C_BaseCombatWeapon* pWeapon = g_LocalPlayer->m_hActiveWeapon().Get();
		int weap_id = pWeapon->m_iItemDefinitionIndex();

		if (pWeapon &&
			weap_id == WEAPON_HEGRENADE ||
			weap_id == WEAPON_MOLOTOV ||
			weap_id == WEAPON_INCGRENADE)
		{
			for (int i = 1; i < 64; i++)
			{
				C_BasePlayer* pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

				if (!pEntity || pEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
					continue;

				float dist = (pEntity->m_vecOrigin() - endpos).Length();

				if (dist < 350.0f)
				{
					CTraceFilter filter;
					filter.pSkip = g_LocalPlayer;
					Ray_t ray;
					Vector NadeScreen;
					Math::WorldToScreen(endpos, NadeScreen);

					Vector vPelvis = pEntity->GetBonePos(HITBOX_PELVIS);
					ray.Init(endpos, vPelvis);
					trace_t ptr;
					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &ptr);

					if (ptr.hit_entity == pEntity)
					{
						Vector PelvisScreen;

						Math::WorldToScreen(vPelvis, PelvisScreen);

						static float a = 105.0f;
						static float b = 25.0f;
						static float c = 140.0f;

						float d = ((((pEntity->m_vecOrigin()) - prev).Length() - b) / c);
						float flDamage = a * exp(-d * d);
						int dmg = 0;
						
						if (CSGO_Armor(flDamage, pEntity->m_ArmorValue()))
							dmg = CSGO_Armor(flDamage, pEntity->m_ArmorValue());

						Color* destcolor = dmg >= 65 ? &redcol : dmg >= 40.0f ? &orangecol : dmg >= 20.0f ? &yellowgreencol : &greencol;

						if (dmg > bestdmg)
						{
							EntName = pEntity->GetPlayerInfo().szName;
							BestColor = destcolor;
							bestdmg = dmg;
						}
					}
				}
			}
		}

		if (bestdmg > 0.f)
		{
			if (weap_id != WEAPON_HEGRENADE)
			{
				if (Math::WorldToScreen(prev, cd))
					Render::Get().RenderText(firegrenade_didnt_hit ? "No collisions" : (EntName + " will be burnt.").c_str(), ImVec2(cd[0], cd[1]), 12.f, *BestColor, true, false, f_AndaleMono);
			}
			else
			{
				if (Math::WorldToScreen(*path.begin(), cd))
					Render::Get().RenderText(("Most damage dealt to: " + EntName + " -" + std::to_string(bestdmg)).c_str(), ImVec2(cd[0], cd[1]), 12.f, *BestColor, true, false, f_AndaleMono);
			}
		}
	}
}

void grenade_prediction::Setup(C_BasePlayer* pl, Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles)
{
	QAngle angThrow = angEyeAngles;
	float pitch = angThrow.pitch;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.pitch = a;

	float flVel = 750.0f * 0.9f;

	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	Math::AngleVectors(angThrow, vForward, vRight, vUp);
	vecSrc = g_LocalPlayer->GetEyePos();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	vecThrow = g_LocalPlayer->m_vecVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void grenade_prediction::Simulate(QAngle & Angles, C_BasePlayer* pLocal)
{
	Vector vecSrc, vecThrow;
	Setup(pLocal, vecSrc, vecThrow, Angles);

	float interval = g_GlobalVars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	path.clear();
	OtherCollisions.clear();
	// TracerColor = Color(255, 255, 0, 255);
	TracerColor = Color(255, 255, 255, 255);
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1) || vecThrow == Vector(0, 0, 0))
			break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int grenade_prediction::Step(Vector & vecSrc, Vector & vecThrow, int tick, float interval)
{
	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	if (tr.hit_entity && ((C_BasePlayer*)tr.hit_entity)->IsPlayer())
	{
		// TracerColor = Color(255, 0, 0, 255);
		TracerColor = Color(255, 255, 255, 255);
	}

	if ((result & 1) || vecThrow == Vector(0, 0, 0) || tr.fraction != 1.0f)
	{
		QAngle angles;
		Math::VectorAngles((tr.endpos - tr.startpos).Normalized(), angles);
		OtherCollisions.push_back(std::make_pair(tr.endpos, angles));
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}

bool grenade_prediction::CheckDetonate(const Vector & vecThrow, const trace_t & tr, int tick, float interval)
{
	firegrenade_didnt_hit = false;
	switch (type)
	{
	case ClassId_CSmokeGrenade:
	case ClassId_CDecoyGrenade:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

		/* TIMES AREN'T COMPLETELY RIGHT FROM WHAT I'VE SEEN ! ! ! */
	case ClassId_CMolotovGrenade:
	case ClassId_CIncendiaryGrenade:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		// OR we've been flying for too long

	case ClassId_CFlashbang:
	case ClassId_CHEGrenade: {
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		firegrenade_didnt_hit = static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));
		return firegrenade_didnt_hit;
	}
	default:
		return false;
	}
}

void grenade_prediction::TraceHull(Vector & src, Vector & end, trace_t & tr)
{
	// Setup grenade hull
	static const Vector hull[2] = { Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f) };

	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceFilterWorldAndPropsOnly filter;

	g_EngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void grenade_prediction::AddGravityMove(Vector & move, Vector & vel, float frametime, bool onground)
{
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void grenade_prediction::PushEntity(Vector & src, const Vector & move, trace_t & tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void grenade_prediction::ResolveFlyCollisionCustom(trace_t & tr, Vector & vecVelocity, float interval)
{
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int grenade_prediction::PhysicsClipVelocity(const Vector & in, const Vector & normal, Vector & out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
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


enum ESequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

inline int RandomSequence(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

// Map of animation fixes
// unfortunately can't be constexpr
const static std::unordered_map<std::string, int(*)(int)> animation_fix_map
{
	{ "models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	} },
	{ "models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	} },
	{ "models/weapons/v_knife_push.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	} },
	{ "models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	} },
	{ "models/weapons/v_knife_ursus.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	} },
	{ "models/weapons/v_knife_stiletto.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(12, 13);
		}
	} },
	{ "models/weapons/v_knife_widowmaker.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return RandomSequence(14, 15);
		}
	} }
};

void SkinChanger::nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return Hooks::o_nSequence(pData, pStruct, pOut);

	CRecvProxyData* proxydata = (CRecvProxyData*)pData;
	C_BaseViewModel* viewmodel = (C_BaseViewModel*)pStruct;

	if (viewmodel && viewmodel->m_hOwner() && viewmodel->m_hOwner().IsValid())
	{
		C_BasePlayer* owner = viewmodel->m_hOwner().Get();

		if (owner == g_LocalPlayer)
		{
			const model_t* knifemodel = g_MdlInfo->GetModel(viewmodel->m_nModelIndex());
			const char* modelname = g_MdlInfo->GetModelName(knifemodel);

			if (animation_fix_map.count(modelname))
				proxydata->m_Value.m_Int = animation_fix_map.at(modelname)(proxydata->m_Value.m_Int);
		}

		C_BaseCombatWeapon* activeWeapon;

		// Needs fixing because the animation cuts off towards the end due to csgo being gay, made a uc thread but no answers because they're all
		// retarded. https://www.unknowncheats.me/forum/counterstrike-global-offensive/347621-deagle-spinning-cut.html

		if (activeWeapon = g_LocalPlayer->m_hActiveWeapon())
			if (Settings::Misc::DeagleSpinner && activeWeapon->GetClientClass()->m_ClassID == ClassId::ClassId_CDEagle && proxydata->m_Value.m_Int == 7) {
				proxydata->m_Value.m_Int  = 8;
			}
	}

	viewmodel->SendViewModelMatchingSequence(proxydata->m_Value.m_Int);

	Hooks::o_nSequence(proxydata, pStruct, pOut);
}

bool SkinChanger::ApplyKnifeModel(C_BaseAttributableItem* weapon, const char* model)
{
	if (!g_LocalPlayer)
		return false;

	C_BaseViewModel* viewmodel = (C_BaseViewModel*)(g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hViewModel()));

	if(!viewmodel)
		return false;

	
	C_BaseAttributableItem* view_model_weapon = (C_BaseAttributableItem*)viewmodel->m_hWeapon();
	
	if (!view_model_weapon)
		return false;

	if (view_model_weapon != weapon)
		return false;

	viewmodel->m_nModelIndex() = g_MdlInfo->GetModelIndex(model);

	return true;
}

bool SkinChanger::ApplyKnifeSkin(C_BaseAttributableItem* weapon, int ItemDefinitionIndex, int paint_kit, int model_int, int entity_quality, float wear)
{
	weapon->m_iItemDefinitionIndex() = ItemDefinitionIndex;
	weapon->m_nFallbackPaintKit() = paint_kit;
	weapon->m_nModelIndex() = model_int;
	weapon->m_iEntityQuality() = entity_quality;
	weapon->m_flFallbackWear() = wear;

	return true;
}

void SkinChanger::Run()
{
	if (!g_EngineClient->IsConnected() && !g_EngineClient->IsInGame())
		return;

	if (!Settings::SkinChanger::Enabled)
		return;

	if (!g_LocalPlayer)
		return;

	C_BaseCombatWeapon* activeWeapon = g_LocalPlayer->m_hActiveWeapon();

	if (!activeWeapon)
		return;

	static const char* model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	static const char* model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	static const char* model_karambit = "models/weapons/v_knife_karam.mdl";
	static const char* model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	static const char* model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	static const char* model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	static const char* model_flip = "models/weapons/v_knife_flip.mdl";
	static const char* model_gut = "models/weapons/v_knife_gut.mdl";
	static const char* model_huntsman = "models/weapons/v_knife_tactical.mdl";
	static const char* model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	static const char* model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	static const char* model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	static const char* model_talon = "models/weapons/v_knife_widowmaker.mdl";
	static const char* model_ursus = "models/weapons/v_knife_ursus.mdl";

	static int index_bayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	static int index_m9 = g_MdlInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	static int index_karambit = g_MdlInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	static int index_bowie = g_MdlInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	static int index_butterfly = g_MdlInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	static int index_falchion = g_MdlInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	static int index_flip = g_MdlInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	static int index_gut = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	static int index_huntsman = g_MdlInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	static int index_shadow_daggers = g_MdlInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	static int index_navaja = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	static int index_stiletto = g_MdlInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	static int index_talon = g_MdlInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
	static int index_ursus = g_MdlInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");

	CHandle<C_BaseCombatWeapon>* myWeapons = g_LocalPlayer->m_hMyWeapons();

	for (int i = 0; myWeapons[i]; i++)
	{
		C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)(C_BaseEntity::get_entity_from_handle(myWeapons[i]));

		if (!weapon)
			continue;

		float wear = Settings::SkinChanger::KnifeWear;

		if (activeWeapon->GetClientClass()->m_ClassID == ClassId_CKnife)
		{
			switch (Settings::SkinChanger::KnifeModel)
			{
			case 0:
				break;
			case 1:
				ApplyKnifeModel(weapon, model_bayonet);
				break;
			case 2:
				ApplyKnifeModel(weapon, model_m9);
				break;
			case 3:
				ApplyKnifeModel(weapon, model_karambit);
				break;
			case 4:
				ApplyKnifeModel(weapon, model_bowie);
				break;
			case 5:
				ApplyKnifeModel(weapon, model_butterfly);
				break;
			case 6:
				ApplyKnifeModel(weapon, model_falchion);
				break;
			case 7:
				ApplyKnifeModel(weapon, model_flip);
				break;
			case 8:
				ApplyKnifeModel(weapon, model_gut);
				break;
			case 9:
				ApplyKnifeModel(weapon, model_huntsman);
				break;
			case 10:
				ApplyKnifeModel(weapon, model_shadow_daggers);
				break;
			case 11:
				ApplyKnifeModel(weapon, model_navaja);
				break;
			case 12:
				ApplyKnifeModel(weapon, model_stiletto);
				break;
			case 13:
				ApplyKnifeModel(weapon, model_talon);
				break;
			case 14:
				ApplyKnifeModel(weapon, model_ursus);
				break;
			}
		}

		if (weapon->GetClientClass()->m_ClassID == ClassId_CKnife)
		{
			switch (Settings::SkinChanger::KnifeModel)
			{
			case 0:
				break;
			case 1:
				ApplyKnifeSkin(weapon, WEAPON_BAYONET, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_bayonet, 3, wear);
				break;
			case 2:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_M9_BAYONET, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_m9, 3, wear);
				break;
			case 3:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_KARAMBIT, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_karambit, 3, wear);
				break;
			case 4:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_SURVIVAL_BOWIE, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_bowie, 3, wear);
				break;
			case 5:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_BUTTERFLY, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_butterfly, 3, wear);
				break;
			case 6:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_FALCHION, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_falchion, 3, wear);
				break;
			case 7:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_FLIP, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_flip, 3, wear);
				break;
			case 8:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_GUT, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_gut, 3, wear);
				break;
			case 9:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_TACTICAL, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_huntsman, 3, wear);
				break;
			case 10:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_PUSH, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_shadow_daggers, 3, wear);
				break;
			case 11:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_GYPSY_JACKKNIFE, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_navaja, 3, wear);
				break;
			case 12:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_STILETTO, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_stiletto, 3, wear);
				break;
			case 13:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_WIDOWMAKER, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_talon, 3, wear);
				break;
			case 14:
				ApplyKnifeSkin(weapon, WEAPON_KNIFE_URSUS, parser_skins[Settings::SkinChanger::Paint::Knife].id, index_ursus, 3, wear);
				break;
			}
		}

		switch (weapon->m_iItemDefinitionIndex())
		{
		case WEAPON_USP_SILENCER:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::usp].id;
			break;
		case WEAPON_HKP2000:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::p2000].id;
			break;
		case WEAPON_GLOCK:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::glock].id;
			break;
		case WEAPON_P250:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::p250].id;
			break;
		case WEAPON_FIVESEVEN:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::fiveseven].id;
			break;
		case WEAPON_TEC9:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::tec].id;
			break;
		case WEAPON_CZ75A:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::cz].id;
			break;
		case WEAPON_ELITE:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::duals].id;
			break;
		case WEAPON_DEAGLE:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::deagle].id;
			break;
		case WEAPON_REVOLVER:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::revolver].id;
			break;
		case WEAPON_FAMAS:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::famas].id;
			break;
		case WEAPON_GALILAR:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::galil].id;
			break;
		case WEAPON_M4A1:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::m4a4].id;
			break;
		case WEAPON_M4A1_SILENCER:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::m4a1].id;
			break;
		case WEAPON_AK47:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::ak47].id;
			break;
		case WEAPON_SG556:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::sg553].id;
			break;
		case WEAPON_AUG:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::aug].id;
			break;
		case WEAPON_SSG08:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::ssg08].id;
			break;
		case WEAPON_AWP:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::awp].id;
			break;
		case WEAPON_SCAR20:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::scar].id;
			break;
		case WEAPON_G3SG1:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::g3sg1].id;
			break;
		case WEAPON_MAC10:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::mac10].id;
			break;
		case WEAPON_MP5:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::mp5sd].id;
			break;
		case WEAPON_MP7:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::mp7].id;
			break;
		case WEAPON_MP9:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::mp9].id;
			break;
		case WEAPON_BIZON:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::ppbizon].id;
			break;
		case WEAPON_P90:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::p90].id;
			break;
		case WEAPON_UMP45:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::ump45].id;
			break;
		case WEAPON_MAG7:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::mag7].id;
			break;
		case WEAPON_NOVA:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::nova].id;
			break;
		case WEAPON_SAWEDOFF:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::sawedoff].id;
			break;
		case WEAPON_XM1014:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::xm1014].id;
			break;
		case WEAPON_M249:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::m249].id;
			break;
		case WEAPON_NEGEV:
			weapon->m_nFallbackPaintKit() = parser_skins[Settings::SkinChanger::Paint::negev].id;
			break;
		}

		weapon->m_OriginalOwnerXuidLow() = -1;
		weapon->m_OriginalOwnerXuid() = g_LocalPlayer->GetPlayerInfo().xuid_low;
		weapon->m_OriginalOwnerXuidHigh() = -1;
		weapon->m_nFallbackSeed() = 661;
		weapon->m_iItemIDHigh() = -1;
	}
}

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

	Utils::ConsolePrint(levelname);

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

			if (pos.DistTo(g_LocalPlayer->m_vecOrigin()) < 10) {
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

		cmd->viewangles =  QAngle(selData.XAng, selData.YAng, 0);
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
				}
			}
		}

		break;
	}
	}
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
	if (weapon && weapon->IsGrenade()) {
		s = "";
		const auto grenade = (C_BaseCSGrenade*)C_BaseEntity::get_entity_from_handle(weapon);
		s += "THROWSTRENGTH ";
		s += std::to_string(grenade->m_flThrowStrength());
		sz = f_AndaleMono->CalcTextSizeA(12.f, FLT_MAX, 0.f, s.c_str());
		Render::Get().RenderText(s, cr, 12.f, Color(255, 255, 0, 255), false, true, f_AndaleMono);
		cr.y += sz.y + 2;
	}
}