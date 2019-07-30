#pragma once

#include "sdk.hpp"
#include <array>
#include "../helpers/utils.hpp"

#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }

#define NETPROP(name, table, netvar) static RecvProp* name() \
{ \
	static auto prop_ptr = NetvarSys::Get().GetNetvarProp(table,netvar); \
	return prop_ptr; \
}

struct datamap_t;
class AnimationLayer;
class CBasePlayerAnimState;
class CCSGOPlayerAnimState;
class C_BaseEntity;

template<class T>
const T& mathmin(const T& a, const T& b)
{
	return (b < a) ? b : a;
}

template<class T>
const T& mathmax(const T& a, const T& b)
{
	return (a < b) ? b : a;
}

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN

};

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
	char szShortName[64];    //0x0000
	char szTextureFile[64];  //0x0040
	bool bRenderUsingFont;   //0x0080
	bool bPrecached;         //0x0081
	int8_t cCharacterInFont; //0x0082
	uint8_t pad_0083[1];     //0x0083
	uint32_t hFont;          //0x0084
	int32_t iTextureId;      //0x0088
	float afTexCoords[4];    //0x008C
	uint8_t pad_009C[16];    //0x009C
};

class C_EconItemView
{
private:
	using str_32 = char[32];
public:
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
	NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(str_32, m_iCustomName, "DT_BaseAttributableItem", "m_szCustomName");
};

enum class Bone : int
{
	INVALID = -1,
	BONE_PELVIS = 0,
	LEAN_ROOT,
	CAM_DRIVER,
	BONE_HIP,
	BONE_LOWER_SPINAL_COLUMN,
	BONE_MIDDLE_SPINAL_COLUMN,
	BONE_UPPER_SPINAL_COLUMN,
	BONE_NECK,
	BONE_HEAD,
};

enum class Hitbox : int
{
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_SPINE,
	HITBOX_LEGS,
	HITBOX_ARMS,
};

class C_BaseEntity : public IClientEntity
{
public:
	datamap_t * GetDataDescMap() {
		typedef datamap_t*(__thiscall *o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>(this, 15)(this);
	}

	datamap_t *GetPredDescMap() {
		typedef datamap_t*(__thiscall *o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>(this, 17)(this);
	}
	static __forceinline C_BaseEntity* GetEntityByIndex(int index) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
	}
	static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
	}

	NETVAR(int32_t, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int32_t, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecAngles, "DT_BaseEntity", "m_vecAngles");
	NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(CHandle<C_BasePlayer>, m_hLeader, "DT_CChicken", "m_leader");
	NETVAR(QAngle, m_angRotation, "DT_BaseEntity", "m_angRotation");
	NETVAR(float_t, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");

	void SetAngle2(QAngle wantedang)
	{
		typedef void(__thiscall * SetAngleFn)(void*, const QAngle&);
		static auto SetAngle = (SetAngleFn)(Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		SetAngle(this, wantedang);
	}

	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
		return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}

	bool* m_bSpotted()
	{
		return (bool*)((uintptr_t)this + NetvarSys::Get().GetOffset("DT_BaseEntity", "m_bSpotted"));
	}

	bool IsPlayer();
	bool IsLoot();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
	bool IsGrenade();
	bool IsChicken();
	//bool isSpotted();
};

class C_BaseGrenade : public C_BaseEntity
{
public:
	NETVAR(bool, m_bIsLive, "DT_BaseGrenade", "m_bIsLive");
	NETVAR(CHandle<C_BasePlayer>, m_hThrower, "DT_BaseGrenade", "m_hThrower");
	NETVAR(float, m_flDamage, "DT_BaseGrenade", "m_flDamage");
};

class C_BaseGrenadeProjectile : public C_BaseGrenade
{
public:
	
};


class C_SmokeGrenadeProjectile : public C_BaseGrenadeProjectile
{
public:
	NETVAR(bool, m_bDidSmokeEffect, "DT_SmokeGrenadeProjectile", "m_bDidSmokeEffect");
	NETVAR(int, m_nSmokeEffectTickBegin, "DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
};

class C_DecoyGrenadeProjectile : public C_BaseGrenadeProjectile
{
public:
	NETVAR(float, m_fLastShotTime, "DT_DecoyGrenade", "m_fLastShotTime");
	NETVAR(float, m_flPostponeFireReadyTime, "DT_DecoyGrenade", "m_flPostponeFireReadyTime");
	NETVAR(bool, m_bLoopingSoundPlaying, "DT_DecoyGrenade", "m_bLoopingSoundPlaying");
};

class C_PlantedC4 : public C_BaseEntity
{
public:
	NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(float, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(int, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");

	NETVAR(C_EconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");

	C_EconItemView& m_Item()
	{
		// Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
		// A real fix for this requires a rewrite of the netvar manager
		return *(C_EconItemView*)this;
	}
	void SetGloveModelIndex(int modelIndex);

};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
	NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");


	CCSWeaponInfo* GetCSWeaponData();
	bool HasBullets();
	bool CanFire();
	bool IsGrenade();
	bool IsKnife();
	bool IsReloading();
	bool IsSMG();
	bool IsHeavy();
	bool IsShotgun();
	bool IsRifle();
	bool IsAuto();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();
	float GetInaccuracy();
	float GetSpread();
	void UpdateAccuracyPenalty();
	CUtlVector<IRefCounted*>& m_CustomMaterials();
	bool* m_bCustomMaterialInitialized();
};

class C_WeaponCSBase : public C_BaseCombatWeapon {};

class C_BaseCSGrenade : public C_WeaponCSBase
{
public:
	NETVAR(float, m_flThrowStrength, "DT_BaseCSGrenade", "m_flThrowStrength");
	NETVAR(float, m_fThrowStrength, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(bool, m_bLoopingSoundPlaying, "DT_BaseCSGrenade", "m_bLoopingSoundPlaying");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(bool, m_bRedraw, "DT_BaseCSGrenade", "m_bRedraw");
	NETVAR(bool, m_bIsBeingHeldByPlayer, "DT_BaseCSGrenade", "m_bIsBeingHeldByPlayer");
};

enum OBS_MODES
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_POI,
	OBS_MODE_ROAMING,

	NUM_OBSERVER_MODES
};

class C_BasePlayer : public C_BaseEntity
{
public:
	static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
	}
	static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
	}

	static __forceinline C_BasePlayer* GetLocalPlayerOrObserverPlayer(bool* obs)
	{
		C_BasePlayer* pl = nullptr;

		if (g_LocalPlayer->m_hObserverTarget())
		{
			pl = (C_BasePlayer*)C_BasePlayer::get_entity_from_handle(g_LocalPlayer->m_hObserverTarget());
			*obs = true;
		}
		else
		{
			pl = g_LocalPlayer;
			*obs = false;
		}

		return pl;
	}
	static __forceinline Bone GetBoneByName(C_BasePlayer* player, const char* boneName)
	{
		if (!player)
			return Bone::INVALID;

		if (!player->GetModel())
			return Bone::INVALID;

		studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(player->GetModel());
		if (!pStudioModel)
			return Bone::INVALID;

		matrix3x4_t pBoneToWorldOut[128];
		if (!player->SetupBones(pBoneToWorldOut, 128, 256, 0))
			return Bone::INVALID;

		for (int i = 0; i < pStudioModel->numbones; i++)
		{
			mstudiobone_t* pBone = pStudioModel->GetBone(i);
			if (!pBone)
				continue;

			if (pBone->pszName() && strcmp(pBone->pszName(), boneName) == 0)
				return (Bone)i;
		}

		return Bone::INVALID;
	}

	UINT* get_weapons()
	{
		return (UINT*)((uintptr_t)this + NetvarSys::Get().GetNetvarProp("DT_CSPlayer", "m_hMyWeapons"));
	}

	void SetAbsOrigin(const Vector& origin)
	{

		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector & origin);
		static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		SetAbsOrigin(this, origin);
	}

	void SetAbsAngles(QAngle angles)
	{
		using SetAbsAngles_t = void(__thiscall*)(C_BaseEntity*, const QAngle & angle);
		static SetAbsAngles_t SetAbsAnglesFn = (SetAbsAngles_t)Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
		SetAbsAnglesFn(this, angles);
	}

	NETVAR(bool, m_bHasDefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bInBombZone, "DT_CSPlayer", "m_bInBombZone");
	NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");;
	NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(int, m_iObserverMode, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(CHandle<C_BaseEntity>, m_hGroundEntity, "DT_BasePlayer", "m_hGroundEntity");
	NETVAR(float, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(int32_t, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(float, m_flNextAttack, "DT_BaseCombatCharacter", "m_flNextAttack");
	NETVAR(int32_t, m_iAccount, "DT_CSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(int, m_nSequence, "DT_BaseViewModel", "m_nSequence");
	PNETVAR(char, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	NETPROP(m_flLowerBodyYawTargetProp, "DT_CSPlayer", "m_flLowerBodyYawTarget");


	//NETVAR(int, m_iAccount, "DT_CSPlayer", "m_iAccount");


	NETVAR(QAngle, m_angAbsAngles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "DT_BaseEntity", "m_flDuckAmount");
	std::array<float, 24> m_flPoseParameter() const {
		static int _m_flPoseParameter = NetvarSys::Get().GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
	}

	PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");

	CUserCmd*& m_pCurrentCommand();

	/*gladiator v2*/
	void InvalidateBoneCache();
	int GetNumAnimOverlays();
	AnimationLayer *GetAnimOverlays();
	AnimationLayer *GetAnimOverlay(int i);
	int GetSequenceActivity(int sequence);
	CCSGOPlayerAnimState *GetPlayerAnimState();

	static void UpdateAnimationState(CCSGOPlayerAnimState *state, QAngle angle);
	static void ResetAnimationState(CCSGOPlayerAnimState *state);
	void CreateAnimationState(CCSGOPlayerAnimState *state);

	float_t &m_surfaceFriction()
	{
		static unsigned int _m_surfaceFriction = Utils::FindInDataMap(GetPredDescMap(), "m_surfaceFriction");
		return *(float_t*)((uintptr_t)this + _m_surfaceFriction);
	}
	Vector &m_vecBaseVelocity()
	{
		static unsigned int _m_vecBaseVelocity = Utils::FindInDataMap(GetPredDescMap(), "m_vecBaseVelocity");
		return *(Vector*)((uintptr_t)this + _m_vecBaseVelocity);
	}

	float_t &m_flMaxspeed()
	{
		static unsigned int _m_flMaxspeed = Utils::FindInDataMap(GetPredDescMap(), "m_flMaxspeed");
		return *(float_t*)((uintptr_t)this + _m_flMaxspeed);
	}

	float get_max_desync_delta()
	{

		uintptr_t animstate = (uintptr_t)(this->GetPlayerAnimState());

		float rate = 180;
		float duckammount = *(float*)(animstate + 0xA4);
		float speedfraction = mathmax(0.f, mathmin(*reinterpret_cast<float*>(animstate + 0xF8), 1.f));

		float speedfactor = mathmax(0.f, mathmin(1.f, *reinterpret_cast<float*>(animstate + 0xFC)));

		float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
		float unk2 = unk1 + 1.f;
		float unk3;

		if (duckammount > 0)
		{

			unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

		}

		unk3 = *(float*)(animstate + 0x334) * unk2;

		return rate;

	}

	Vector        GetEyePos();
	player_info_t GetPlayerInfo();
	bool          IsAlive();
	bool		  IsFlashed();
	bool          HasC4();
	Vector        GetHitboxPos(int hitbox_id);
	mstudiobbox_t * GetHitbox(int hitbox_id);
	bool          GetHitboxPos(int hitbox, Vector &output);
	Vector        GetBonePos(int bone);
	bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
	bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);
	void          UpdateClientSideAnimation();
	int           m_nMoveType();

	QAngle * GetVAngles();
	float_t m_flSpawnTime();

	C_BasePlayer* SelfOrObs(bool* isObs = (bool*)0);

	bool notfriend;
};

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(m_nSequence, "DT_BaseViewModel", "m_nSequence");
	void SendViewModelMatchingSequence(int sequence);
};

class AnimationLayer
{
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

class CCSGOPlayerAnimState
{
public:
	void* pThis;
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
}; //Size=0x344