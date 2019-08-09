#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "cheats/misc.hpp"
#include "valve_sdk/sdk.hpp"
#include "singleton.hpp"
#include "helpers/utils.hpp"
#include "settings.hpp"

#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}

template <typename T = bool>
class Var
{
public:
	std::string name;
	T* value;
	int32_t size;
	Var(std::string name, T *v) : name(name)
	{
		value = v;
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T*() { return &*value; }
	operator T() const { return *value; }
};

class Options
{
public:
	OPTION(bool, miscmoonwalk, &Settings::Misc::MoonWalk);
	OPTION(bool, miscbunnyhop, &Settings::Misc::BunnyHop);
	OPTION(bool, miscautostrafer, &Settings::Misc::AutoStrafer);
	OPTION(bool, miscautocowboy, &Settings::Misc::AutoCowboy);
	OPTION(bool, miscreversecowboy, &Settings::Misc::ReverseCowboy);
	OPTION(bool, miscblockbot, &Settings::Misc::BlockBot);
	OPTION(bool, miscinfiniteduck, &Settings::Misc::InfiniteDuck);
	OPTION(bool, miscselfnade, &Settings::Misc::SelfNade);
	OPTION(bool, miscedgejumper, &Settings::Misc::EdgeJumper);

	OPTION(bool, misczeusrange, &Settings::Misc::ZeusRange::Enabled);
	OPTION(float, misczeusrangespeed, &Settings::Misc::ZeusRange::Speed);
	OPTION(int, misczeusrangethickness, &Settings::Misc::ZeusRange::Thickness);
	OPTION(int, misczeusrangestep, &Settings::Misc::ZeusRange::Step);

	OPTION(bool, miscslowwalk, &Settings::Misc::SlowWalk::Enabled);
	OPTION(int, miscslowwalkamount, &Settings::Misc::SlowWalk::Amount);

	OPTION(bool, miscautopistol, &Settings::Misc::AutoPistol);
	OPTION(bool, miscautoreload, &Settings::Misc::AutoReload);
	OPTION(bool, miscdeaglespinner, &Settings::Misc::DeagleSpinner);

	OPTION(bool, miscfakeduck, &Settings::Misc::FakeDuck::Enabled);

	OPTION(bool, miscfakelagenabled, &Settings::Misc::FakeLag::Enabled);
	OPTION(int, miscfakelagchoke, &Settings::Misc::FakeLag::Choke);

	OPTION(bool, miscremovefootsteps, &Settings::Misc::RemoveFootsteps);
	OPTION(bool, miscrankreveal, &Settings::Misc::RankReveal);

	OPTION(bool, miscrecoilcrosshair, &Settings::Misc::RecoilCrosshair);
	OPTION(bool, miscwallbangcrosshair, &Settings::Misc::WallbangCrosshair);
	OPTION(bool, miscradarhack, &Settings::Misc::RadarHack);
	OPTION(bool, miscinverseragdollgravity, &Settings::Misc::InverseRagdollGravity);
	OPTION(bool, miscchickentamer, &Settings::Misc::ChickenTamer);

	OPTION(bool, miscspectatorlist, &Settings::Misc::SpectatorList);
	OPTION(bool, miscsnipercrosshair, &Settings::Misc::SniperCrosshair);
	OPTION(bool, miscbulletimpacts, &Settings::Misc::BulletImpacts);
	OPTION(bool, miscnodecals, &Settings::Misc::NoDecals);

	OPTION(bool, misctranslatebot, &Settings::Misc::TranslateBot);

	OPTION(bool, miscautoaccept, &Settings::Misc::AutoAccept);

	OPTION(bool, misccamerafov, &Settings::Misc::CameraFOV::Enabled);
	OPTION(int, misccamerafovval, &Settings::Misc::CameraFOV::FOV);
	OPTION(bool, miscviewmodelfov, &Settings::Misc::ViewmodelFOV::Enabled);
	OPTION(int, miscviewmodelfovval, &Settings::Misc::ViewmodelFOV::FOV);

	OPTION(bool, visualsworldcolourenabled, &Settings::Visuals::WorldColour::Enabled);
	OPTION(Color, visualsworldcolourcolor, &Settings::Visuals::WorldColour::Colour);

	OPTION(bool, miscbrightness, &Settings::Misc::Brightness::Enabled);
	OPTION(int, miscbrightnessperc, &Settings::Misc::Brightness::Perc);

	OPTION(bool, miscremovalsflash, &Settings::Misc::Removals::Flash);
	OPTION(bool, miscremovalssmokeoverlay, &Settings::Misc::Removals::SmokeOverlay);
	OPTION(bool, miscremovalsscope, &Settings::Misc::Removals::Scope);
	OPTION(bool, miscremovalsscopeblur, &Settings::Misc::Removals::ScopeBlur);
	OPTION(bool, miscremovalssmoke, &Settings::Misc::Removals::Smoke);
	OPTION(bool, miscremovalspostprocessing, &Settings::Misc::Removals::PostProcessing);
	OPTION(int, miscremovalssmoketype, &Settings::Misc::Removals::SmokeType);
	OPTION(bool, miscremovalsrecoilshake, &Settings::Misc::Removals::RecoilShake);
	OPTION(bool, miscremovalsarms, &Settings::Misc::Removals::Arms);
	OPTION(bool, miscremovalssleeves, &Settings::Misc::Removals::Sleeves);
	OPTION(bool, miscremovalsweapon, &Settings::Misc::Removals::Weapon);
	OPTION(bool, miscremoavls3dsky, &Settings::Misc::Removals::Sky3D);
	OPTION(bool, miscremovalsshadows, &Settings::Misc::Removals::Shadows);

	OPTION(bool, miscFUCK, &Settings::Misc::FUCK);


	OPTION(bool, miscautodefuseenabled, &Settings::Misc::AutoDefuse::Enabled);

	OPTION(bool, miscsoundshitsoundenabled, &Settings::Misc::Sounds::HitSound::Enabled);
	OPTION(int, miscsoundshitsound, &Settings::Misc::Sounds::HitSound::Sound);

	OPTION(bool, miscthirdpersonenabled, &Settings::Misc::ThirdPerson::Enabled);
	OPTION(bool, miscthirdpersonignorescope, &Settings::Misc::ThirdPerson::IgnoreScope);
	OPTION(bool, miscthirdpersonignoreplayers, &Settings::Misc::ThirdPerson::IgnorePlayers);
	OPTION(bool, miscthirdpersonignorewalls, &Settings::Misc::ThirdPerson::IgnoreWalls);
	OPTION(bool, miscthirdpersonrealangles, &Settings::Misc::ThirdPerson::RealAngles);
	OPTION(int, miscthirdpersoncameraoffset, &Settings::Misc::ThirdPerson::CameraOffset);

	OPTION(bool, miscviewmodeloffset, &Settings::Misc::ViewmodelOffset::Enabled);
	OPTION(float, miscviewmodeloffsetx, &Settings::Misc::ViewmodelOffset::X);
	OPTION(float, miscviewmodeloffsety, &Settings::Misc::ViewmodelOffset::Y);
	OPTION(float, miscviewmodeloffsetz, &Settings::Misc::ViewmodelOffset::Z);

	OPTION(bool, miscfreelookenabled, &Settings::Misc::FreeLook::Enabled);

	OPTION(bool, miscchatmimic, &Settings::Misc::ChatMimic::Enabled);
	OPTION(bool, miscchatmimiccustommsg, &Settings::Misc::ChatMimic::CustomMsg);

	OPTION(bool, miscfakezoom, &Settings::Misc::FakeZoom::Enabled);

	OPTION(bool, espgrenadeprediction, &Settings::ESP::GrenadePrediction::Enabled);
	OPTION(bool, espgrenadepredictiononlylocal, &Settings::ESP::GrenadePrediction::OnlyLocal);

	OPTION(bool, miscsmokehelper, &Settings::Misc::SmokeHelper::Enabled);
	OPTION(int, miscsmokehelpertype, &Settings::Misc::SmokeHelper::TypeHelp);

	OPTION(bool, espplayermaster, &Settings::ESP::Players::Enabled);

	OPTION(bool, espenemiesenabled, &Settings::ESP::Players::Enemies::Enabled);
	OPTION(bool, espenemiesoccluded, &Settings::ESP::Players::Enemies::Occluded);
	OPTION(bool, espenemiesboxes, &Settings::ESP::Players::Enemies::Boxes);
	OPTION(bool, espenemiesskeletons, &Settings::ESP::Players::Enemies::Skeletons);
	OPTION(bool, espenemiesbacktrackskeletons, &Settings::ESP::Players::Enemies::BacktrackSkeletons);
	OPTION(bool, espenemiesnames, &Settings::ESP::Players::Enemies::Names);
	OPTION(bool, espenemieshealth, &Settings::ESP::Players::Enemies::Health);
	OPTION(bool, espenemiesweapons, &Settings::ESP::Players::Enemies::Weapons);
	OPTION(bool, espenemiessnaplines, &Settings::ESP::Players::Enemies::Snaplines);
	OPTION(bool, espenemiespositions, &Settings::ESP::Players::Enemies::Positions);
	OPTION(bool, espenemieseyelines, &Settings::ESP::Players::Enemies::EyeLines);

	OPTION(Color, espenemiescolourboxes, &Settings::ESP::Players::Enemies::Colours::Boxes);
	OPTION(Color, espenemiescolourskeletons, &Settings::ESP::Players::Enemies::Colours::Skeletons);
	OPTION(Color, espenemiescolourbacktrackskeletons, &Settings::ESP::Players::Enemies::Colours::BacktrackSkeletons);
	OPTION(Color, espenemiescolournames, &Settings::ESP::Players::Enemies::Colours::Names);
	OPTION(Color, espenemiescolourhealth, &Settings::ESP::Players::Enemies::Colours::Health);
	OPTION(Color, espenemiescolourweapons, &Settings::ESP::Players::Enemies::Colours::Weapons);
	OPTION(Color, espenemiescoloursnaplines, &Settings::ESP::Players::Enemies::Colours::Snaplines);
	OPTION(Color, espenemiescolourpositions, &Settings::ESP::Players::Enemies::Colours::Positions);
	OPTION(Color, espenemiescoloureyelines, &Settings::ESP::Players::Enemies::Colours::EyeLines);

	OPTION(bool, espteammatesenabled, &Settings::ESP::Players::Teammates::Enabled);
	OPTION(bool, espteammatesoccluded, &Settings::ESP::Players::Teammates::Occluded);
	OPTION(bool, espteammatesboxes, &Settings::ESP::Players::Teammates::Boxes);
	OPTION(bool, espteammatesskeletons, &Settings::ESP::Players::Teammates::Skeletons);
	OPTION(bool, espteammatesnames, &Settings::ESP::Players::Teammates::Names);
	OPTION(bool, espteammateshealth, &Settings::ESP::Players::Teammates::Health);
	OPTION(bool, espteammatesweapons, &Settings::ESP::Players::Teammates::Weapons);
	OPTION(bool, espteammatessnaplines, &Settings::ESP::Players::Teammates::Snaplines);
	OPTION(bool, espteammatespositions, &Settings::ESP::Players::Teammates::Positions);
	OPTION(bool, espteammateseyelines, &Settings::ESP::Players::Teammates::EyeLines);

	OPTION(Color, espteammatescolourboxes, &Settings::ESP::Players::Teammates::Colours::Boxes);
	OPTION(Color, espteammatescolourskeletons, &Settings::ESP::Players::Teammates::Colours::Skeletons);
	OPTION(Color, espteammatescolournames, &Settings::ESP::Players::Teammates::Colours::Names);
	OPTION(Color, espteammatescolourhealth, &Settings::ESP::Players::Teammates::Colours::Health);
	OPTION(Color, espteammatescolourweapons, &Settings::ESP::Players::Teammates::Colours::Weapons);
	OPTION(Color, espteammatescoloursnaplines, &Settings::ESP::Players::Teammates::Colours::Snaplines);
	OPTION(Color, espteammatescolourpositions, &Settings::ESP::Players::Teammates::Colours::Positions);
	OPTION(Color, espteammatescoloureyelines, &Settings::ESP::Players::Teammates::Colours::EyeLines);

	OPTION(bool, esplocalplayerenabled, &Settings::ESP::Players::Localplayer::Enabled);
	OPTION(bool, esplocalplayeroccluded, &Settings::ESP::Players::Localplayer::Occluded);
	OPTION(bool, esplocalplayerboxes, &Settings::ESP::Players::Localplayer::Boxes);
	OPTION(bool, esplocalplayerskeletons, &Settings::ESP::Players::Localplayer::Skeletons);
	OPTION(bool, esplocalplayernames, &Settings::ESP::Players::Localplayer::Names);
	OPTION(bool, esplocalplayerhealth, &Settings::ESP::Players::Localplayer::Health);
	OPTION(bool, esplocalplayerweapons, &Settings::ESP::Players::Localplayer::Weapons);
	OPTION(bool, esplocalplayerpositions, &Settings::ESP::Players::Localplayer::Positions);

	OPTION(Color, esplocalplayercolourboxes, &Settings::ESP::Players::Localplayer::Colours::Boxes);
	OPTION(Color, esplocalplayercolourskeletons, &Settings::ESP::Players::Localplayer::Colours::Skeletons);
	OPTION(Color, esplocalplayercolournames, &Settings::ESP::Players::Localplayer::Colours::Names);
	OPTION(Color, esplocalplayercolourhealth, &Settings::ESP::Players::Localplayer::Colours::Health);
	OPTION(Color, esplocalplayercolourweapons, &Settings::ESP::Players::Localplayer::Colours::Weapons);
	OPTION(Color, esplocalplayercolourpositions, &Settings::ESP::Players::Localplayer::Colours::Positions);

	OPTION(bool, glowteammatesenabled, &Settings::ESP::Glow::Players::Teammates::Enabled);
	OPTION(bool, glowteammatescover, &Settings::ESP::Glow::Players::Teammates::Cover);
	OPTION(int, glowteammatesstyle, &Settings::ESP::Glow::Players::Teammates::Style);
	OPTION(Color, glowteammatescolour, &Settings::ESP::Glow::Players::Teammates::Colour);

	OPTION(bool, glowenemiesenabled, &Settings::ESP::Glow::Players::Enemies::Enabled);
	OPTION(bool, glowenemiescover, &Settings::ESP::Glow::Players::Enemies::Cover);
	OPTION(int, glowenemiesstyle, &Settings::ESP::Glow::Players::Enemies::Style);
	OPTION(Color, glowenemiescolour, &Settings::ESP::Glow::Players::Enemies::Colour);

	OPTION(bool, espgrenades, &Settings::ESP::Others::Grenades::Enabled);
	OPTION(Color, espgrenadescolour, &Settings::ESP::Others::Grenades::Colour);

	OPTION(bool, espc4, &Settings::ESP::Others::C4::Enabled);
	OPTION(Color, espc4colour, &Settings::ESP::Others::C4::Colour);

	OPTION(bool, aimrcsenabled, &Settings::Aim::RCS::Enabled);
	OPTION(bool, aimrcssilent, &Settings::Aim::RCS::Silent);
	OPTION(float, aimrcsscalex, &Settings::Aim::RCS::ScaleX);
	OPTION(float, aimrcsscaley, &Settings::Aim::RCS::ScaleY);

	OPTION(int, clantagtype, &Settings::ClanTag::Type);

	OPTION(Color, systemtheme, &Settings::System::Theme);
	OPTION(Color, systemtexttheme, &Settings::System::TextTheme);
	OPTION(Color, systemwindowtheme, &Settings::System::WindowTheme);
	OPTION(bool, systemantiuntrust, &Settings::System::AntiUntrust);
	OPTION(bool,  systemantiobs, &Settings::System::AntiOBS);
	OPTION(bool, systemplayerlistshowlocalplayer, &Settings::System::PlayerList::ShowLocalplayer);
	OPTION(bool, systemplayerlistshowenemies, &Settings::System::PlayerList::ShowEnemies);
	OPTION(bool, systemplayerlistshowbots, &Settings::System::PlayerList::ShowBots);

	OPTION(bool, visualsbulletshots, &Settings::Visuals::BulletShots::Enabled);
	OPTION(Color, visualsbulletshotscolour, &Settings::Visuals::BulletShots::Colour);

	OPTION(bool, visualsnightmode, &Settings::Visuals::Nightmode::Enabled);
	OPTION(float, visualsnightmodeintensity, &Settings::Visuals::Nightmode::Intensity);

	OPTION(bool, chamsplayersmaster, &Settings::Visuals::Chams::Players::Enabled);

	OPTION(bool, chamsplayersenemies, &Settings::Visuals::Chams::Players::Enemies::Enabled);
	OPTION(bool, chamsplayersteammates, &Settings::Visuals::Chams::Players::Teammates::Enabled);
	OPTION(bool, chamsplayerslocalplayer, &Settings::Visuals::Chams::Players::Localplayer::Enabled);

	OPTION(bool, chamsplayersenemiesocc, &Settings::Visuals::Chams::Players::Enemies::DoOccluded);
	OPTION(bool, chamsplayersteammatesocc, &Settings::Visuals::Chams::Players::Teammates::DoOccluded);
	OPTION(bool, chamsplayerslocalplayerocc, &Settings::Visuals::Chams::Players::Localplayer::DoOccluded);

	OPTION(int, chamsplayersenemiesmat, &Settings::Visuals::Chams::Players::Enemies::Material);
	OPTION(int, chamsplayersteammatesmat, &Settings::Visuals::Chams::Players::Teammates::Material);
	OPTION(int, chamsplayerslocalplayermat, &Settings::Visuals::Chams::Players::Localplayer::Material);

	OPTION(Color, chamsplayerscolenemiesvisible, &Settings::Visuals::Chams::Players::Enemies::Visible);
	OPTION(Color, chamsplayerscolenemiesoccluded, &Settings::Visuals::Chams::Players::Enemies::Occluded);
	OPTION(Color, chamsplayerscolorteammatesvisible, &Settings::Visuals::Chams::Players::Teammates::Visible);
	OPTION(Color, chamsplayerscolenteammatesoccluded, &Settings::Visuals::Chams::Players::Teammates::Occluded);
	OPTION(Color, chamsplayerscolorlocalplayervisible, &Settings::Visuals::Chams::Players::Localplayer::Visible);
	OPTION(Color, chamsplayerscolenlocalplayeroccluded, &Settings::Visuals::Chams::Players::Localplayer::Occluded);

	OPTION(bool, chamsviewmodelarmsenabled, &Settings::Visuals::Chams::Viewmodel::Arms::Enabled);
	OPTION(bool, chamsviewmodelarmswireframe, &Settings::Visuals::Chams::Viewmodel::Arms::Wireframe);
	OPTION(int, chamsviewmodelarmsmaterial, &Settings::Visuals::Chams::Viewmodel::Arms::Material);
	OPTION(Color, chamsviewmodelarmscolour, &Settings::Visuals::Chams::Viewmodel::Arms::Colour);

	OPTION(bool, chamsviewmodelsleevesenabled, &Settings::Visuals::Chams::Viewmodel::Sleeves::Enabled);
	OPTION(bool, chamsviewmodelsleeveswireframe, &Settings::Visuals::Chams::Viewmodel::Sleeves::Wireframe);
	OPTION(int, chamsviewmodelsleevesmaterial, &Settings::Visuals::Chams::Viewmodel::Sleeves::Material);
	OPTION(Color, chamsviewmodelsleevescolour, &Settings::Visuals::Chams::Viewmodel::Sleeves::Colour);

	OPTION(bool, chamsviewmodelweaponsenabled, &Settings::Visuals::Chams::Viewmodel::Weapons::Enabled);
	OPTION(bool, chamsviewmodelweaponswireframe, &Settings::Visuals::Chams::Viewmodel::Weapons::Wireframe);
	OPTION(int, chamsviewmodelweaponsmaterial, &Settings::Visuals::Chams::Viewmodel::Weapons::Material);
	OPTION(Color, chamsviewmodelweaponscolour, &Settings::Visuals::Chams::Viewmodel::Weapons::Colour);

	OPTION(int, antiaimpitchtype, &Settings::AntiAim::Pitch::Type);
	OPTION(int, antiaimyawtype, &Settings::AntiAim::Yaw::Type);
	OPTION(int, antiaimyawangle, &Settings::AntiAim::Yaw::Angle);
	OPTION(int, antiaimyawjitter, &Settings::AntiAim::Yaw::Jitter);
	OPTION(int, antiaimyawlkey, &Settings::AntiAim::Yaw::LKey);
	OPTION(int, antiaimyawrkey, &Settings::AntiAim::Yaw::RKey);
	
	OPTION(bool, testaimbotenabled, &Settings::Aim::TestAimbot::Enabled);
	OPTION(bool, testaimbotonshoot, &Settings::Aim::TestAimbot::OnShoot);
	OPTION(bool, testaimbotautoshoot, &Settings::Aim::TestAimbot::AutoShoot);
	OPTION(bool, testaimbotsilent, &Settings::Aim::TestAimbot::Silent);
	OPTION(int, testaimbottargetpriority, &Settings::Aim::TestAimbot::TargetPriority);
	OPTION(bool, triggerbotenable, &Settings::Misc::Triggerbot::Enabled);

	OPTION(bool, testaimbotscoutautostop, &Settings::Aim::TestAimbot::Scout::AutoStop);
	OPTION(bool, testaimbotscoutautocrouch, &Settings::Aim::TestAimbot::Scout::AutoCrouch);
	OPTION(bool, testaimbotscoutautoscope, &Settings::Aim::TestAimbot::Scout::AutoScope);
	OPTION(int, testaimbotscoutmindamage, &Settings::Aim::TestAimbot::Scout::MinDamage);
	OPTION(float, testaimbotscouthitchance, &Settings::Aim::TestAimbot::Scout::HitChance);
	OPTION(bool, testaimbotscoutprioritise, &Settings::Aim::TestAimbot::Scout::Prioritise);
	OPTION(int, testaimbotscoutprioritised, &Settings::Aim::TestAimbot::Scout::Prioritised);

	OPTION(bool, testaimbotawpautostop, &Settings::Aim::TestAimbot::Awp::AutoStop);
	OPTION(bool, testaimbotawpautocrouch, &Settings::Aim::TestAimbot::Awp::AutoCrouch);
	OPTION(bool, testaimbotawpautoscope, &Settings::Aim::TestAimbot::Awp::AutoScope);
	OPTION(int, testaimbotawpmindamage, &Settings::Aim::TestAimbot::Awp::MinDamage);
	OPTION(float, testaimbotawphitchance, &Settings::Aim::TestAimbot::Awp::HitChance);
	OPTION(bool, testaimbotawpprioritise, &Settings::Aim::TestAimbot::Awp::Prioritise);
	OPTION(int, testaimbotawpprioritised, &Settings::Aim::TestAimbot::Awp::Prioritised);

	OPTION(bool, testaimbotautoautostop, &Settings::Aim::TestAimbot::Auto::AutoStop);
	OPTION(bool, testaimbotautoautocrouch, &Settings::Aim::TestAimbot::Auto::AutoCrouch);
	OPTION(bool, testaimbotautoautoscope, &Settings::Aim::TestAimbot::Auto::AutoScope);
	OPTION(int, testaimbotautomindamage, &Settings::Aim::TestAimbot::Auto::MinDamage);
	OPTION(float, testaimbotautohitchance, &Settings::Aim::TestAimbot::Auto::HitChance);
	OPTION(bool, testaimbotautoprioritise, &Settings::Aim::TestAimbot::Auto::Prioritise);
	OPTION(int, testaimbotautoprioritised, &Settings::Aim::TestAimbot::Auto::Prioritised);

	OPTION(bool, testaimbotautor8, &Settings::Misc::AutoRevolver);
	OPTION(bool, testaimbotdeagler8autostop, &Settings::Aim::TestAimbot::DeagleR8::AutoStop);
	OPTION(bool, testaimbotdeagler8autocrouch, &Settings::Aim::TestAimbot::DeagleR8::AutoCrouch);
	OPTION(int, testaimbotdeagler8mindamage, &Settings::Aim::TestAimbot::DeagleR8::MinDamage);
	OPTION(float, testaimbotdeagler8hitchance, &Settings::Aim::TestAimbot::DeagleR8::HitChance);
	OPTION(bool, testaimbotdeagler8prioritise, &Settings::Aim::TestAimbot::DeagleR8::Prioritise);
	OPTION(int, testaimbotdeagler8prioritised, &Settings::Aim::TestAimbot::DeagleR8::Prioritised);

	OPTION(bool, testaimbotrifleautostop, &Settings::Aim::TestAimbot::Rifle::AutoStop);
	OPTION(bool, testaimbotrifleautocrouch, &Settings::Aim::TestAimbot::Rifle::AutoCrouch);
	OPTION(int, testaimbotriflemindamage, &Settings::Aim::TestAimbot::Rifle::MinDamage);
	OPTION(float, testaimbotriflehitchance, &Settings::Aim::TestAimbot::Rifle::HitChance);
	OPTION(bool, testaimbotrifleprioritise, &Settings::Aim::TestAimbot::Rifle::Prioritise);
	OPTION(int, testaimbotrifleprioritised, &Settings::Aim::TestAimbot::Rifle::Prioritised);

	OPTION(bool, testaimbotpistolautostop, &Settings::Aim::TestAimbot::Pistol::AutoStop);
	OPTION(bool, testaimbotpistolautocrouch, &Settings::Aim::TestAimbot::Pistol::AutoCrouch);
	OPTION(int, testaimbotpistolmindamage, &Settings::Aim::TestAimbot::Pistol::MinDamage);
	OPTION(float, testaimbotpistolhitchance, &Settings::Aim::TestAimbot::Pistol::HitChance);
	OPTION(bool, testaimbotpistolprioritise, &Settings::Aim::TestAimbot::Pistol::Prioritise);
	OPTION(int, testaimbotpistolprioritised, &Settings::Aim::TestAimbot::Pistol::Prioritised);

	OPTION(bool, testaimbotshotgunautostop, &Settings::Aim::TestAimbot::Shotgun::AutoStop);
	OPTION(bool, testaimbotshotgunautocrouch, &Settings::Aim::TestAimbot::Shotgun::AutoCrouch);
	OPTION(int, testaimbotshotgunmindamage, &Settings::Aim::TestAimbot::Shotgun::MinDamage);
	OPTION(float, testaimbotshotgunhitchance, &Settings::Aim::TestAimbot::Shotgun::HitChance);
	OPTION(bool, testaimbotshotgunprioritise, &Settings::Aim::TestAimbot::Shotgun::Prioritise);
	OPTION(int, testaimbotshotgunprioritised, &Settings::Aim::TestAimbot::Shotgun::Prioritised);

	OPTION(bool, testaimbotsmgautostop, &Settings::Aim::TestAimbot::SMG::AutoStop);
	OPTION(bool, testaimbotsmgautocrouch, &Settings::Aim::TestAimbot::SMG::AutoCrouch);
	OPTION(int, testaimbotsmgmindamage, &Settings::Aim::TestAimbot::SMG::MinDamage);
	OPTION(float, testaimbotsmghitchance, &Settings::Aim::TestAimbot::SMG::HitChance);
	OPTION(bool, testaimbotsmgprioritise, &Settings::Aim::TestAimbot::SMG::Prioritise);
	OPTION(int, testaimbotsmgprioritised, &Settings::Aim::TestAimbot::SMG::Prioritised);

	OPTION(bool, testaimbototherautostop, &Settings::Aim::TestAimbot::Other::AutoStop);
	OPTION(bool, testaimbototherautocrouch, &Settings::Aim::TestAimbot::Other::AutoCrouch);
	OPTION(int, testaimbotothermindamage, &Settings::Aim::TestAimbot::Other::MinDamage);
	OPTION(float, testaimbototherhitchance, &Settings::Aim::TestAimbot::Other::HitChance);
	OPTION(bool, testaimbototherprioritise, &Settings::Aim::TestAimbot::Other::Prioritise);
	OPTION(int, testaimbototherprioritised, &Settings::Aim::TestAimbot::Other::Prioritised);

	OPTION(bool, backtrack, &Settings::Backtrack::Enabled);
	OPTION(int, backtrackms, &Settings::Backtrack::ms);
}options;

class Useropt
{
public:
	OPTION(int, thirdpersonkey, &Settings::KeyBinds::ThirdPerson::Key);
	OPTION(int, thirdpersontype, &Settings::KeyBinds::ThirdPerson::Type);
	OPTION(int, freelookkey, &Settings::KeyBinds::FreeLook::Key);
	OPTION(int, freelooktype, &Settings::KeyBinds::FreeLook::Type);
	OPTION(int, fakeduckkey, &Settings::KeyBinds::FakeDuck::Key);
	OPTION(int, fakeducktype, &Settings::KeyBinds::FakeDuck::Type);
	OPTION(int, autodefusekey, &Settings::KeyBinds::AutoDefuse::Key);
	OPTION(int, autodefusetype, &Settings::KeyBinds::AutoDefuse::Type);
	OPTION(int, triggerbottype, &Settings::KeyBinds::Triggerbot::Type);
	OPTION(int, triggerbotkey, &Settings::KeyBinds::Triggerbot::Key);
	OPTION(int, fakezoomkey, &Settings::KeyBinds::FakeZoom::Key);
	OPTION(int, fakezoomtype, &Settings::KeyBinds::FakeZoom::Type);
	OPTION(int, aimbotkey, &Settings::KeyBinds::Aimbot::Key);
	OPTION(int, aimbottype, &Settings::KeyBinds::Aimbot::Type);

	OPTION(float, spectatorposx, &Settings::System::SpectatorPosX);
	OPTION(float, spectatorposy, &Settings::System::SpectatorPosY);

	OPTION(bool, chimphooknotis, &Settings::System::ChimpHookNotifications);
}useropt;

std::string join(const std::vector<ClanTagMember>& lst, const std::string& delim)
{
	std::string ret;
	for (const auto& s : lst)
	{
		if (!ret.empty())
			ret += delim;
		ret += s.value;
	}
	return ret;
}

std::vector<ClanTagMember> split(std::string str, std::string token)
{
	std::vector<std::string> result;
	while (str.size())
	{
		int index = str.find(token);
		if (index != std::string::npos)
		{
			result.push_back(str.substr(0, index));
			str = str.substr(index + token.size());
			if (str.size() == 0)result.push_back(str);
		}
		else
		{
			result.push_back(str);
			str = "";
		}
	}

	std::vector<ClanTagMember> result2;
	for (auto i : result)
	{
		ClanTagMember n;
		strcpy(n.value, i.c_str());
		result2.push_back(n);
	}

	return result2;
}

inline bool exists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

class Config : public Singleton<Config>
{
public:
	void SaveUser()
	{
		std::stringstream filename;
		filename << "$CHIMPUSER.chimps";
		std::ofstream fout(filename.str().c_str(), std::ios::binary);
		const auto sz = sizeof(Useropt);
		const auto var_sz = sizeof(Var<bool>);
		const auto cnt = sz / var_sz;
		for (auto i = 0; i < cnt; i++)
		{
			const auto el = &(*(Var<int>*)(&useropt)) + i;
			auto name = el->name;
			auto val = el->value;
			auto sizeof_val = el->size;
			fout << name << "\t" << Utils::BytesToString((unsigned char*) * (int*)& val, sizeof_val) << std::endl;
		}

		fout << "chatspammerfilename" << "\t" << Settings::Misc::ChatSpammer::Filename << std::endl;

		fout.close();
	}

	void LoadUser()
	{
		if (!exists("$CHIMPUSER.chimps"))
			return;

		std::ifstream fin("$CHIMPUSER.chimps", std::ios::binary);
		std::stringstream ss;
		ss << fin.rdbuf();


		auto lines = Utils::Split(ss.str(), "\n");

		for (auto line : lines)
		{
			auto data = Utils::Split(line, "\t");
			if (data.size() < 2) continue;

			const size_t sz = sizeof(Useropt);
			const size_t var_sz = sizeof(Var<bool>);
			const auto cnt = sz / var_sz;

			if (data[0] == "chatspammerfilename")
			{
				Settings::Misc::ChatSpammer::Filename = data[1];
				continue;
			}

			for (auto i = 0; i < cnt; i++)
			{
				const auto& el = &(*(Var<bool>*)(&useropt)) + i;
				if (data[0] == el->name)
				{
					auto bytes = Utils::HexToBytes(data[1]);
					memcpy(*(void**)& el->value, bytes.data(), el->size);
				}
			}
		}
		
		if (exists(Settings::Misc::ChatSpammer::Filename)) {
			std::ifstream fin2(Settings::Misc::ChatSpammer::Filename.c_str(), std::ios::binary);
			std::stringstream ss;
			ss << fin2.rdbuf();

			ChatSpammer::Get().filedata = ss.str();

			fin2.close();
		}

		fin.close();
	}

	void Save(char name[32])
	{
		std::stringstream filename;
		filename << "$CHIMPHOOK_" << name << ".chimps";
		std::ofstream fout(filename.str().c_str(), std::ios::binary);
		const auto sz = sizeof(Options);
		const auto var_sz = sizeof(Var<bool>);
		const auto cnt = sz / var_sz;
		for (auto i = 0; i < cnt; i++)
		{
			const auto el = &(*(Var<int>*)(&options)) + i;
			auto name = el->name;
			auto val = el->value;
			auto sizeof_val = el->size;
			fout << name << "\t" << Utils::BytesToString((unsigned char*) * (int*)& val, sizeof_val) << std::endl;
		}
	}


	void Load(char name[32])
	{
		std::stringstream filename;
		filename << "$CHIMPHOOK_" << name << ".chimps";
		std::ifstream fin(filename.str().c_str(), std::ios::binary);
		std::stringstream ss;
		ss << fin.rdbuf();


		auto lines = Utils::Split(ss.str(), "\n");

		for (auto line : lines)
		{
			auto data = Utils::Split(line, "\t");
			if (data.size() < 2) continue;

			const size_t sz = sizeof(Options);
			const size_t var_sz = sizeof(Var<bool>);
			const auto cnt = sz / var_sz;

			for (auto i = 0; i < cnt; i++)
			{
				const auto& el = &(*(Var<bool>*)(&options)) + i;
				if (data[0] == el->name)
				{
					auto bytes = Utils::HexToBytes(data[1]);
					memcpy(*(void**)& el->value, bytes.data(), el->size);
				}
			}
		}

		fin.close();

		if (Settings::Visuals::Nightmode::Enabled)
			Nightmode::Get().Execute();

		if (Settings::Visuals::WorldColour::Enabled)
			WorldColour::Execute();
	}
};