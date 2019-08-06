#include "../settings.hpp"

bool Settings::Aim::TestAimbot::Enabled = false;
bool Settings::Aim::TestAimbot::TargetTeammates = false;
bool Settings::Aim::TestAimbot::OnShoot = false;
bool Settings::Aim::TestAimbot::AutoShoot = false;
bool Settings::Aim::TestAimbot::Silent = false;
bool Settings::Aim::TestAimbot::WeaponMove = false;
int Settings::Aim::TestAimbot::TargetPriority = 0;

bool Settings::Aim::TestAimbot::Scout::AutoStop = false;
bool Settings::Aim::TestAimbot::Scout::AutoCrouch = false;
bool Settings::Aim::TestAimbot::Scout::AutoScope = false;
int Settings::Aim::TestAimbot::Scout::MinDamage = 0;
float Settings::Aim::TestAimbot::Scout::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Scout::Prioritise = false;
int Settings::Aim::TestAimbot::Scout::Prioritised = 0;

bool Settings::Aim::TestAimbot::Awp::AutoStop = false;
bool Settings::Aim::TestAimbot::Awp::AutoCrouch = false;
bool Settings::Aim::TestAimbot::Awp::AutoScope = false;
int Settings::Aim::TestAimbot::Awp::MinDamage = 0;
float Settings::Aim::TestAimbot::Awp::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Awp::Prioritise = false;
int Settings::Aim::TestAimbot::Awp::Prioritised = 0;

bool Settings::Aim::TestAimbot::Auto::AutoStop = false;
bool Settings::Aim::TestAimbot::Auto::AutoCrouch = false;
bool Settings::Aim::TestAimbot::Auto::AutoScope = false;
int Settings::Aim::TestAimbot::Auto::MinDamage = 0;
float Settings::Aim::TestAimbot::Auto::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Auto::Prioritise = false;
int Settings::Aim::TestAimbot::Auto::Prioritised = 0;

bool Settings::Aim::TestAimbot::DeagleR8::AutoStop = false;
bool Settings::Aim::TestAimbot::DeagleR8::AutoCrouch = false;
int Settings::Aim::TestAimbot::DeagleR8::MinDamage = 0;
float Settings::Aim::TestAimbot::DeagleR8::HitChance = 0.f;
bool Settings::Aim::TestAimbot::DeagleR8::Prioritise = false;
int Settings::Aim::TestAimbot::DeagleR8::Prioritised = 0;

bool Settings::Aim::TestAimbot::Rifle::AutoStop = false;
bool Settings::Aim::TestAimbot::Rifle::AutoCrouch = false;
int Settings::Aim::TestAimbot::Rifle::MinDamage = 0;
float Settings::Aim::TestAimbot::Rifle::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Rifle::Prioritise = false;
int Settings::Aim::TestAimbot::Rifle::Prioritised = 0;

bool Settings::Aim::TestAimbot::Pistol::AutoStop = false;
bool Settings::Aim::TestAimbot::Pistol::AutoCrouch = false;
int Settings::Aim::TestAimbot::Pistol::MinDamage = 0;
float Settings::Aim::TestAimbot::Pistol::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Pistol::Prioritise = false;
int Settings::Aim::TestAimbot::Pistol::Prioritised = 0;

bool Settings::Aim::TestAimbot::Shotgun::AutoStop = false;
bool Settings::Aim::TestAimbot::Shotgun::AutoCrouch = false;
int Settings::Aim::TestAimbot::Shotgun::MinDamage = 0;
float Settings::Aim::TestAimbot::Shotgun::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Shotgun::Prioritise = false;
int Settings::Aim::TestAimbot::Shotgun::Prioritised = 0;

bool Settings::Aim::TestAimbot::SMG::AutoStop = false;
bool Settings::Aim::TestAimbot::SMG::AutoCrouch = false;
int Settings::Aim::TestAimbot::SMG::MinDamage = 0;
float Settings::Aim::TestAimbot::SMG::HitChance = 0.f;
bool Settings::Aim::TestAimbot::SMG::Prioritise = false;
int Settings::Aim::TestAimbot::SMG::Prioritised = 0;

bool Settings::Aim::TestAimbot::Other::AutoStop = false;
bool Settings::Aim::TestAimbot::Other::AutoCrouch = false;
int Settings::Aim::TestAimbot::Other::MinDamage = 0;
float Settings::Aim::TestAimbot::Other::HitChance = 0.f;
bool Settings::Aim::TestAimbot::Other::Prioritise = false;
int Settings::Aim::TestAimbot::Other::Prioritised = 0;


float GetWeaponHitChance(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::HitChance;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::HitChance;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::HitChance;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::HitChance;

	else if (weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::DeagleR8::HitChance;

	else if (weapon->IsRifle())
		return Settings::Aim::TestAimbot::Rifle::HitChance;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::Pistol::HitChance;

	else if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::HitChance;

	else
		return Settings::Aim::TestAimbot::Other::HitChance;
}

int GetWeaponMinDamage(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::MinDamage;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::MinDamage;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::MinDamage;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::MinDamage;

	else if (weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::DeagleR8::MinDamage;

	else if (weapon->IsRifle())
		return Settings::Aim::TestAimbot::Rifle::MinDamage;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::Pistol::MinDamage;

	else if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::MinDamage;

	else
		return Settings::Aim::TestAimbot::Other::MinDamage;
}

bool GetWeaponAutoStop(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::AutoStop;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::AutoStop;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::AutoStop;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::AutoStop;

	else if (weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::DeagleR8::AutoStop;

	else if (weapon->IsRifle())
		return Settings::Aim::TestAimbot::Rifle::AutoStop;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::Pistol::AutoStop;

	else if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::AutoStop;

	else
		return Settings::Aim::TestAimbot::Other::AutoStop;
}

bool GetWeaponAutoCrouch(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::AutoCrouch;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::AutoCrouch;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::AutoCrouch;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::AutoCrouch;

	else if (weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::DeagleR8::AutoCrouch;

	else if (weapon->IsRifle())
		return Settings::Aim::TestAimbot::Rifle::AutoCrouch;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::Pistol::AutoCrouch;

	else if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::AutoCrouch;

	else
		return Settings::Aim::TestAimbot::Other::AutoCrouch;
}

bool GetWeaponAutoScope(C_BaseCombatWeapon* weapon)
{
	if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::AutoScope;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::AutoScope;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::AutoScope;

	else 
		return false;
}

bool GetWeaponPrioritiseHitbox(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::Prioritise;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return Settings::Aim::TestAimbot::Scout::Prioritise;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return Settings::Aim::TestAimbot::Awp::Prioritise;

	else if (weapon->IsAuto())
		return Settings::Aim::TestAimbot::Auto::Prioritise;

	else if (weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::DeagleR8::Prioritise;

	else if (weapon->IsRifle())
		return Settings::Aim::TestAimbot::Rifle::Prioritise;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return Settings::Aim::TestAimbot::Pistol::Prioritise;

	else if (weapon->IsSMG())
		return Settings::Aim::TestAimbot::SMG::Prioritise;

	else
		return Settings::Aim::TestAimbot::Other::Prioritise;

}
Hitbox GetWeaponPrioritisedHitbox(C_BaseCombatWeapon* weapon)
{
	if (weapon->IsSMG())
		return (Hitbox)Settings::Aim::TestAimbot::SMG::Prioritised;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_SSG08)
		return (Hitbox)Settings::Aim::TestAimbot::Scout::Prioritised;

	else if (weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
		return (Hitbox)Settings::Aim::TestAimbot::Awp::Prioritised;

	else if (weapon->IsAuto())
		return (Hitbox)Settings::Aim::TestAimbot::Auto::Prioritised;

	else if (weapon->IsDeagleR8())
		return (Hitbox)Settings::Aim::TestAimbot::DeagleR8::Prioritised;

	else if (weapon->IsRifle())
		return (Hitbox)Settings::Aim::TestAimbot::Rifle::Prioritised;

	else if (weapon->IsPistol() && !weapon->IsDeagleR8())
		return (Hitbox)Settings::Aim::TestAimbot::Pistol::Prioritised;

	else if (weapon->IsSMG())
		return (Hitbox)Settings::Aim::TestAimbot::SMG::Prioritised;

	else
		return (Hitbox)Settings::Aim::TestAimbot::Other::Prioritised;
}