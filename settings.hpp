#pragma once

#include "valve_sdk/csgostructs.hpp"
#include "helpers/math.hpp"

struct ClanTagMember
{
	char value[64] = "";
};

namespace Settings
{
	namespace KeyBinds
	{
		namespace ThirdPerson
		{
			extern int Type;
			extern int Key;
		}

		namespace ThirdPersonSpec
		{
			extern int Type;
			extern int Key;
		}

		namespace FreeLook
		{
			extern int Type;
			extern int Key;
		}

		namespace FakeDuck
		{
			extern int Type;
			extern int Key;
		}

		namespace AutoDefuse
		{
			extern int Type;
			extern int Key;
		}

		namespace FakeZoom
		{
			extern int Type;
			extern int Key;
		}

		namespace Aimbot
		{
			extern int Type;
			extern int Key;
		}

		namespace Triggerbot
		{
			extern int Type;
			extern int Key;
		}

		namespace ThirdPersonSpectate
		{
			extern int Type;
			extern int Key;
		}
	}

	namespace Misc
	{
		extern bool BunnyHop;
		extern bool AutoStrafer;
		extern bool EdgeJumper;
		extern bool AutoCowboy;
		extern bool ReverseCowboy;
		extern bool BlockBot;
		extern bool InfiniteDuck;
		extern bool SelfNade;

		extern bool AutoRevolver;
		extern bool AutoPistol;
		extern bool AutoReload;
		extern bool DeagleSpinner;
		extern bool MoonWalk;

		extern bool RemoveFootsteps;
		extern bool RankReveal;

		extern bool RecoilCrosshair;
		extern bool WallbangCrosshair;
		extern bool RadarHack;
		extern bool InverseRagdollGravity;
		extern bool ChickenTamer;

		extern bool SniperCrosshair;
		extern bool BulletImpacts;
		extern bool NoDecals;

		extern bool AntiKick;
		extern bool AutoAccept;
		extern bool TranslateBot;
		extern bool SpectatorList;
		
		extern bool FUCK;
		extern bool DebugMode;

		namespace ThirdPersonSpectate
		{
			extern bool Enabled;
			extern bool _enabled;
		}
		
		namespace ZeusRange
		{
			extern bool Enabled;
			extern float Speed;
			extern int Thickness;
			extern int Step;
		}

		namespace SmokeHelper
		{
			extern bool Enabled;
			extern int TypeHelp;
		}

		namespace CameraFOV
		{
			extern bool Enabled;
			extern int FOV;
		}

		namespace ViewmodelFOV
		{
			extern bool Enabled;
			extern int FOV;
		}

		namespace Sounds
		{
			namespace HitSound
			{
				extern bool Enabled;
				extern int Sound;
			}
		}

		namespace FakeLag
		{
			extern bool Enabled;
			extern int Choke;
		}

		namespace ChatMimic
		{
			extern bool Enabled;
			extern bool CustomMsg;
			extern char Msg[256];
		}

		namespace ChatSpammer
		{
			extern bool Enabled;
			extern std::string Filename;
		}

		namespace Removals
		{
			extern bool Flash;
			extern bool SmokeOverlay;
			extern bool ScopeBlur;
			extern bool Scope;
			extern bool PostProcessing;
			extern bool Smoke;
			extern int  SmokeType;
			extern bool RecoilShake;
			extern bool Arms;
			extern bool Sleeves;
			extern bool Weapon;
			extern bool Sky3D;
			extern bool Shadows;
		}

		namespace ThirdPerson
		{
			extern bool Enabled;
			extern bool _enabled;
			extern bool IgnoreScope;
			extern bool IgnorePlayers;
			extern bool IgnoreWalls;
			extern bool RealAngles;
			extern int  CameraOffset;
		}

		namespace FakeDuck
		{
			extern bool Enabled;
			extern bool _enabled;
		}

		namespace AutoDefuse
		{
			extern bool Enabled;
			extern bool _enabled;
		}

		namespace Triggerbot
		{
			extern bool Enabled;
			extern int Delay;
			extern bool _enabled;
		}

		namespace FreeLook
		{
			extern bool Enabled;
			extern bool _enabled;
		}

		namespace ClantagScroller
		{
			extern bool Enabled;
			extern std::string Clantag;
			extern int Delay;
		}

		namespace FakeZoom
		{
			extern bool Enabled;
			extern bool _enabled;
		}

		namespace Brightness
		{
			extern bool Enabled;
			extern int Perc;
		}

		namespace SlowWalk
		{
			extern bool Enabled;
			extern int Amount;
		}

		namespace ViewmodelOffset
		{
			extern bool Enabled;
			extern float X;
			extern float Y;
			extern float Z;
		}
	}

	namespace Visuals
	{
		namespace Chams
		{
			namespace Players
			{
				extern bool Enabled;

				namespace Enemies
				{
					extern bool Enabled;
					extern bool DoOccluded;
					extern int Material;
					extern Color Visible;
					extern Color Occluded;
				}

				namespace Teammates
				{
					extern bool Enabled;
					extern bool DoOccluded;
					extern int Material;
					extern Color Visible;
					extern Color Occluded;
				}

				namespace Localplayer
				{
					extern bool Enabled;
					extern bool DoOccluded;
					extern int Material;
					extern Color Visible;
					extern Color Occluded;
				}
			}

			namespace Viewmodel
			{
				namespace Arms
				{
					extern bool Enabled;
					extern bool Wireframe;
					extern int Material;
					extern Color Colour;
				}
				namespace Sleeves
				{
					extern bool Enabled;
					extern bool Wireframe;
					extern int Material;
					extern Color Colour;
				}
				namespace Weapons
				{
					extern bool Enabled;
					extern bool Wireframe;
					extern int Material;
					extern Color Colour;
				}
			}
		}

		namespace BulletShots
		{
			extern bool Enabled;
			extern Color Colour;
		}

		namespace Nightmode
		{
			extern bool Enabled;
			extern float Intensity;
		}

		namespace WorldColour
		{
			extern bool Enabled;
			extern Color Colour;
		}
	}

	namespace ESP
	{
		namespace GrenadePrediction
		{
			extern bool Enabled;
			extern bool OnlyLocal;
		}

		namespace Glow
		{
			namespace Players
			{
				namespace Enemies
				{
					extern bool Enabled;
					extern bool Cover;
					extern int Style;
					extern Color Colour;
				}

				namespace Teammates
				{
					extern bool Enabled;
					extern bool Cover;
					extern int Style;
					extern Color Colour;
				}
			}
		}

		namespace Others
		{
			namespace Grenades
			{
				extern bool Enabled;
				extern Color Colour;
			}

			namespace C4
			{
				extern bool Enabled;
				extern Color Colour;
			}
		}

		namespace Players
		{
			extern bool Enabled;

			namespace Enemies
			{
				extern bool Enabled;
				extern bool Occluded;

				extern bool Boxes;
				extern bool Skeletons;
				extern bool BacktrackSkeletons;
				extern bool Names;
				extern bool Health;
				extern bool Weapons;
				extern bool Snaplines;
				extern bool Positions;
				extern bool EyeLines;

				namespace Colours
				{
					extern Color Boxes;
					extern Color Skeletons;
					extern Color BacktrackSkeletons;
					extern Color Names;
					extern Color Health;
					extern Color Weapons;
					extern Color Snaplines;
					extern Color Positions;
					extern Color EyeLines;
				}
			}

			namespace Teammates
			{
				extern bool Enabled;
				extern bool Occluded;

				extern bool Boxes;
				extern bool Skeletons;
				extern bool Names;
				extern bool Health;
				extern bool Weapons;
				extern bool Snaplines;
				extern bool Positions;
				extern bool EyeLines;

				namespace Colours
				{
					extern Color Boxes;
					extern Color Skeletons;
					extern Color Names;
					extern Color Health;
					extern Color Weapons;
					extern Color Snaplines;
					extern Color Positions;
					extern Color EyeLines;
				}
			}
			namespace Localplayer
			{
				extern bool Enabled;
				extern bool Occluded;

				extern bool Boxes;
				extern bool Skeletons;
				extern bool Names;
				extern bool Health;
				extern bool Weapons;
				extern bool Positions;

				namespace Colours
				{
					extern Color Boxes;
					extern Color Skeletons;
					extern Color Names;
					extern Color Health;
					extern Color Weapons;
					extern Color Positions;
				}
			}
		}
	}

	namespace System
	{
		extern Color Theme;
		extern Color TextTheme;
		extern Color WindowTheme;
		extern bool Unload;
		extern bool AntiUntrust;

		extern float SpectatorPosX;
		extern float SpectatorPosY;

		extern bool ChimpHookNotifications;

		namespace PlayerList
		{
			extern bool ShowLocalplayer;
			extern bool ShowEnemies;
			extern bool ShowBots;

			namespace NameStealer
			{
				extern bool Enabled;
				extern bool Enemies;
				extern int Delay;
			}
		}
	}

	namespace Aim
	{
		namespace RCS
		{
			extern bool Enabled;
			extern bool Silent;
			extern float ScaleX;
			extern float ScaleY;
		}

		namespace TestAimbot
		{
			extern bool Enabled;
			extern bool _enabled;
			extern bool TargetTeammates;
			extern bool OnShoot;
			extern bool AutoShoot;
			extern bool Silent;
			extern bool WeaponMove;
			extern int TargetPriority;

			namespace Scout
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern bool AutoScope;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Awp
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern bool AutoScope;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Auto
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern bool AutoScope;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace DeagleR8
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Rifle
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Pistol
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Shotgun
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace SMG
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}

			namespace Other
			{
				extern bool AutoStop;
				extern bool AutoCrouch;
				extern int MinDamage;
				extern float HitChance;
				extern bool Prioritise;
				extern int Prioritised;
			}
		}
	}

	namespace ClanTag
	{
		extern int Type;
		extern ClanTagMember StaticTag;
		extern ClanTagMember PulseTag;
		extern ClanTagMember ScrollTag;
		extern std::vector<ClanTagMember> CustomTagEditor;
	}

	namespace AntiAim
	{
		namespace Pitch
		{
			extern int Type;
		}

		namespace Yaw
		{
			extern int Type;
			extern int Angle;
			extern int Jitter;

			extern int LKey;
			extern int RKey;

			extern bool __lkey;
		}
	}

	namespace Backtrack
	{
		extern bool Enabled;
		extern int ms;
	}

	namespace SkinChanger
	{
		extern bool Enabled;

		extern float KnifeWear;
		extern int KnifeModel;

		namespace Paint
		{
			extern int Knife;
			extern int usp;
			extern int p2000;
			extern int glock;
			extern int p250;
			extern int fiveseven;
			extern int tec;
			extern int cz;
			extern int duals;
			extern int deagle;
			extern int revolver;
			extern int famas;
			extern int galil;
			extern int m4a4;
			extern int m4a1;
			extern int ak47;
			extern int sg553;
			extern int aug;
			extern int ssg08;
			extern int awp;
			extern int scar;
			extern int g3sg1;
			extern int mac10;
			extern int mp5sd;
			extern int mp7;
			extern int mp9;
			extern int ppbizon;
			extern int p90;
			extern int ump45;
			extern int mag7;
			extern int nova;
			extern int sawedoff;
			extern int xm1014;
			extern int m249;
			extern int negev;
		}
	}
};