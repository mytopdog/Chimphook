#include "SkinChanger.hpp"

#include "../../helpers/skinchangerparser.hpp"

int Settings::SkinChanger::Paint::xm1014 = 0;
int Settings::SkinChanger::Paint::usp = 0;
int Settings::SkinChanger::Paint::ump45 = 0;
int Settings::SkinChanger::Paint::tec  = 0;
int Settings::SkinChanger::Paint::ssg08 = 0;
int Settings::SkinChanger::Paint::sg553 = 0;
int Settings::SkinChanger::Paint::scar = 0;
int Settings::SkinChanger::Paint::sawedoff = 0;
int Settings::SkinChanger::Paint::revolver = 0;
int Settings::SkinChanger::Paint::ppbizon = 0;
int Settings::SkinChanger::Paint::p90 = 0;
int Settings::SkinChanger::Paint::p250 = 0;
int Settings::SkinChanger::Paint::p2000 = 0;
int Settings::SkinChanger::Paint::nova = 0;
int Settings::SkinChanger::Paint::negev = 0;
int Settings::SkinChanger::Paint::mp9 = 0;
int Settings::SkinChanger::Paint::mp7 = 0;
int Settings::SkinChanger::Paint::mp5sd = 0;
int Settings::SkinChanger::Paint::mag7 = 0;
int Settings::SkinChanger::Paint::mac10 = 0;
int Settings::SkinChanger::Paint::m4a4 = 0;
int Settings::SkinChanger::Paint::m4a1 = 0;
int Settings::SkinChanger::Paint::m249 = 0;
int Settings::SkinChanger::Paint::Knife = 0;
int Settings::SkinChanger::Paint::glock = 0;
int Settings::SkinChanger::Paint::galil = 0;
int Settings::SkinChanger::Paint::g3sg1 = 0;
int Settings::SkinChanger::Paint::fiveseven = 0;
int Settings::SkinChanger::Paint::famas = 0;
int Settings::SkinChanger::Paint::duals = 0;
int Settings::SkinChanger::Paint::deagle = 0;
int Settings::SkinChanger::Paint::cz = 0;
int Settings::SkinChanger::Paint::awp = 0;
int Settings::SkinChanger::Paint::aug = 0;
int Settings::SkinChanger::Paint::ak47 = 0;
int Settings::SkinChanger::KnifeModel = 0;
float Settings::SkinChanger::KnifeWear = 0;
bool Settings::SkinChanger::Enabled = 0;

bool Settings::Misc::DeagleSpinner = false;

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
			if (Settings::Misc::DeagleSpinner && activeWeapon->GetClientClass()->m_ClassID == ClassId::ClassId_CDEagle && proxydata->m_Value.m_Int == 7)
			{
				proxydata->m_Value.m_Int = 8;
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

	if (!viewmodel)
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