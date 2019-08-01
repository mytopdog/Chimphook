#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../valve_sdk/sdk.hpp"
#include "../helpers/math.hpp"
#include "../singleton.hpp"
#include "../settings.hpp"
#include "../render.hpp"
#include "../hooks.hpp"
#include "../menu.hpp"
#include "grenadedata.hpp";

void BulletShots(IGameEvent* ev);
void WallbangCrosshair();

namespace Glow
{
	void Shutdown();
	void Run();
}

class Visuals : public Singleton<Visuals>
{
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	Visuals();
	~Visuals();
public:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_teammate;
			bool          is_localplayer;
			bool          is_visible;
			float         distance;
			float         font_size;
			ImVec2        cursor;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		}ctx;

		void Begin(C_BasePlayer* pl);
		void RenderBox(Color col);
		void RenderName(Color col);
		void RenderSkeleton(Color col);
		void RenderHealth(Color col);
		void RenderWeapon(Color col);
		void RenderSnapline(Color col);
		void RenderPosition(Color col);
		void RenderEyeLine(Color col);
	};
	//void RenderCrosshair();
	//void RenderWeapon(C_BaseCombatWeapon* ent);
	//void RenderDefuseKit(C_BaseEntity* ent);
	//void RenderPlantedC4(C_BaseEntity* ent);
	//void RenderItemEsp(C_BaseEntity* ent);
	//void ThirdPerson();
public:
	void AddToDrawList();
};

namespace Chams {
	void SceneEnd();
	void OnDME(
		IMatRenderContext* ctx,
		const DrawModelState_t& state,
		const ModelRenderInfo_t& info,
		matrix3x4_t* matrix);
	void Initialize();
	void Shutdown();
};

enum
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class grenade_prediction : public Singleton<grenade_prediction>
{
public:
	void Tick(int buttons);
	void View();
	void Paint();

	void Setup(C_BasePlayer* pl, Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles);
	void Simulate(QAngle& Angles, C_BasePlayer* pLocal);
	int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);
	void TraceHull(Vector& src, Vector& end, trace_t& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);

	int act = 0;
	int type = 0;
	std::vector<Vector> path;
	std::vector<std::pair<Vector, QAngle>> OtherCollisions;
	Color TracerColor = Color(255, 255, 0, 255);
	bool firegrenade_didnt_hit = false;
};

namespace SkinChanger
{
	void nSequence(const CRecvProxyData* pDta, void* pStruct, void* pOut);
	bool ApplyKnifeModel(C_BaseAttributableItem* weapoon, const char* model);
	bool ApplyKnifeSkin(C_BaseAttributableItem* weapon, int ItemDefinitionIndex, int paint_kit, int model_int, int entity_quality, float wear);
	void Run();
}

class SmokeHelper : public Singleton<SmokeHelper>
{
public:
	void Draw();
	void OnCreateMove(CUserCmd* cmd);

	GrenadeData selData;
	bool Found;
};

void DebugMode();