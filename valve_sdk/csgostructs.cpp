#include "csgostructs.hpp"
#include "../Helpers/Math.hpp"
#include "../Helpers/Utils.hpp"

bool C_BaseEntity::IsPlayer()
{
	//ref: "effects/nightvision"

	// C_BaseEntity::IsPlayer -> i156
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 156)(this);
}

bool C_BaseEntity::IsLoot() {
	return GetClientClass()->m_ClassID == ClassId_CPhysPropAmmoBox ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropLootCrate ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropRadarJammer ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropWeaponUpgrade ||
		GetClientClass()->m_ClassID == ClassId_CDrone ||
		GetClientClass()->m_ClassID == ClassId_CDronegun ||
		GetClientClass()->m_ClassID == ClassId_CItem_Healthshot ||
		GetClientClass()->m_ClassID == ClassId_CItemCash || 
		GetClientClass()->m_ClassID == ClassId_CBumpMine;
}

bool C_BaseEntity::IsWeapon()
{
	// C_BaseEntity::IsWeapon -> i164
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 164)(this);
}

bool C_BaseEntity::IsGrenade()
{
	return GetClientClass()->m_ClassID == ClassId_CBaseCSGrenadeProjectile
		|| GetClientClass()->m_ClassID == ClassId_CBaseCSGrenade
		|| GetClientClass()->m_ClassID == ClassId_CBaseGrenade

		|| GetClientClass()->m_ClassID == ClassId_CMolotovProjectile
		|| GetClientClass()->m_ClassID == ClassId_CIncendiaryGrenade
		|| GetClientClass()->m_ClassID == ClassId_CSmokeGrenadeProjectile
		|| GetClientClass()->m_ClassID == ClassId_CDecoyProjectile

		|| GetClientClass()->m_ClassID == ClassId_CHEGrenade
		|| GetClientClass()->m_ClassID == ClassId_CDecoyGrenade
		|| GetClientClass()->m_ClassID == ClassId_CFlashbang
		|| GetClientClass()->m_ClassID == ClassId_CMolotovGrenade
		|| GetClientClass()->m_ClassID == ClassId_CIncendiaryGrenade;
}

bool C_BaseEntity::IsPlantedC4()
{
	return GetClientClass()->m_ClassID == ClassId_CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit()
{
	return GetClientClass()->m_ClassID == ClassId_CBaseAnimating;
}

bool C_BaseEntity::IsChicken()
{
	return GetClientClass()->m_ClassID == ClassId_CChicken;
}

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{
	// C_BaseCombatWeapon::GetCSWeaponData -> i455
	return CallVFunction<CCSWeaponInfo*(__thiscall*)(C_BaseCombatWeapon*)>(this, 455)(this);
}

bool C_BaseCombatWeapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
	auto owner = this->m_hOwnerEntity().Get();
	if (!owner)
		return false;

	if (IsReloading() || m_iClip1() <= 0)
		return false;

	auto flServerTime = owner->m_nTickBase() * g_GlobalVars->interval_per_tick;

	if (owner->m_flNextAttack() > flServerTime)
		return false;


	return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsGrenade()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsGun()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_C4:
		return false;
	case WEAPONTYPE_GRENADE:
		return false;
	case WEAPONTYPE_KNIFE:
		return false;
	case WEAPONTYPE_UNKNOWN:
		return false;
	default:
		return true;
	}
}

bool C_BaseCombatWeapon::IsTaser()
{
	return m_Item().m_iItemDefinitionIndex() == WEAPON_TASER;
}

bool C_BaseCombatWeapon::IsKnife()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsSMG()
{

	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_SUBMACHINEGUN;
}

bool C_BaseCombatWeapon::IsShotgun()
{

	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_SHOTGUN;
}

bool C_BaseCombatWeapon::IsHeavy()
{

	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_MACHINEGUN;
}

bool C_BaseCombatWeapon::IsDeagleR8()
{
	return m_Item().m_iItemDefinitionIndex() == WEAPON_DEAGLE || m_Item().m_iItemDefinitionIndex() == WEAPON_REVOLVER2 || m_Item().m_iItemDefinitionIndex() == WEAPON_REVOLVER;
}
 
bool C_BaseCombatWeapon::IsRifle()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_RIFLE;
}

bool C_BaseCombatWeapon::IsPistol()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_PISTOL;
}

bool C_BaseCombatWeapon::IsAuto()
{
	return m_iItemDefinitionIndex() == WEAPON_G3SG1 ||
		m_iItemDefinitionIndex() == WEAPON_SCAR20;
}

bool C_BaseCombatWeapon::IsSniper()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_SNIPER_RIFLE && !IsAuto();
}

bool C_BaseCombatWeapon::IsAutomatic() {
	return IsAuto() || IsRifle() || IsHeavy() || IsSMG();
}

bool C_BaseCombatWeapon::IsReloading()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
	return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 476)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 446)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 477)(this);
}

CUtlVector<IRefCounted*>& C_BaseCombatWeapon::m_CustomMaterials()
{	static auto customMaterials = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + customMaterials);
}

bool* C_BaseCombatWeapon::m_bCustomMaterialInitialized()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "C6 86 ? ? ? ? ? FF 50 04") + 2);
	return (bool*)((uintptr_t)this + currentCommand);
}

CUtlVector<IRefCounted*>& C_EconItemView::m_CustomMaterials()
{
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + 0x14);
}

CUtlVector<IRefCounted*>& C_EconItemView::m_VisualsDataProcessors()
{
	static auto visDataProc = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C 24 0C") + 2);
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + visDataProc);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

int C_BasePlayer::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x298C);
}

AnimationLayer *C_BasePlayer::GetAnimOverlays()
{
	return *(AnimationLayer**)((DWORD)this + 0x2980);
}

AnimationLayer *C_BasePlayer::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];
	return nullptr;
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = g_MdlInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::PatternScan(GetModuleHandle(L"client_panorama.dll"), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return get_sequence_activity(this, hdr, sequence);
}

CCSGOPlayerAnimState *C_BasePlayer::GetPlayerAnimState()
{
	return *(CCSGOPlayerAnimState**)((DWORD)this + 0x3900);
}

C_BasePlayer* C_BasePlayer::SelfOrObs(bool* isObs)
{
	if (isObs) *isObs = (bool)this->m_hObserverTarget();
	if (this->m_hObserverTarget()) return this->m_hObserverTarget();

	return this;
}

void C_BasePlayer::UpdateAnimationState(CCSGOPlayerAnimState *state, QAngle angle)
{
	static auto UpdateAnimState = Utils::PatternScan(
		GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

void C_BasePlayer::ResetAnimationState(CCSGOPlayerAnimState *state)
{
	typedef void(__thiscall* fnResetAnimationState)(CCSGOPlayerAnimState*);

	static auto ResetAnimState = (fnResetAnimationState)Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
	
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void C_BasePlayer::CreateAnimationState(CCSGOPlayerAnimState *state)
{
	typedef void(__thiscall* fnCreateAnimationState)(CCSGOPlayerAnimState*, C_BasePlayer*);

	static fnCreateAnimationState CreateAnimState = (fnCreateAnimationState)Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
	
	if (!CreateAnimState)
		return;

	CreateAnimState(state, this);
}

Vector C_BasePlayer::GetEyePos()
{
	return m_vecOrigin() + m_vecViewOffset();
}

player_info_t C_BasePlayer::GetPlayerInfo()
{
	player_info_t info;
	if (this == nullptr)
		return info;

	g_EngineClient->GetPlayerInfo(EntIndex(), &info);
	return info;
}

bool C_BasePlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::IsFlashed()
{
	static auto m_flFlashMaxAlpha = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	return *(float*)((uintptr_t)this + m_flFlashMaxAlpha - 0x8) > 200.0;
}

bool C_BasePlayer::HasC4()
{
	typedef bool(__thiscall* fnHasC4)(void*);

	static fnHasC4 hasC4 = (fnHasC4)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "56 8B F1 85 F6 74 31"));

	return hasC4(this);
}

Vector C_BasePlayer::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		studiohdr_t* studio_model = g_MdlInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

mstudiobbox_t* C_BasePlayer::GetHitbox(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				return hitbox;
			}
		}
	}
	return nullptr;
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector &output)
{
	if (hitbox >= HITBOX_MAX)
		return false;

	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}

Vector C_BasePlayer::GetBonePos(int bone)
{
	if (this == nullptr)
		return Vector{};

	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
		return boneMatrix[bone].at(3);
	}
	return Vector{};
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, int hitbox)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos(hitbox);

	ray.Init(GetEyePos(), endpos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(GetEyePos(), pos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

void C_BasePlayer::UpdateClientSideAnimation()
{
	typedef void(__thiscall* fnUpdateClientSideAnimation)(void*);

	static fnUpdateClientSideAnimation updatefn = (fnUpdateClientSideAnimation)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "56 8B F1 80 BE ? ? ? ? ? 74 29"));

	return updatefn(this);
}

void C_BasePlayer::InvalidateBoneCache()
{
	static DWORD addr = (DWORD)Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

	*(int*)((uintptr_t)this + 0xA30) = g_GlobalVars->framecount;
	*(int*)((uintptr_t)this + 0xA28) = 0;

	unsigned long g_iModelBoneCounter = **(unsigned long**)(addr + 10);
	*(unsigned int*)((DWORD)this + 0x2924) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)this + 0x2690) = (g_iModelBoneCounter - 1);
}

int C_BasePlayer::m_nMoveType()
{
	return *(int*)((uintptr_t)this + 0x25C);
}

QAngle* C_BasePlayer::GetVAngles()
{
	static auto deadflag = NetvarSys::Get().GetOffset("DT_BasePlayer", "deadflag");
	return (QAngle*)((uintptr_t)this + deadflag + 0x4);
}

void C_BaseAttributableItem::SetGloveModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

void C_BaseViewModel::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 244)(this, sequence);
}

float_t C_BasePlayer::m_flSpawnTime()
{
	return *(float_t*)((uintptr_t)this + 0xA360);
}