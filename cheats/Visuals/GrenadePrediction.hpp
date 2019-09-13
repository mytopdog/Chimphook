#pragma once

#include "../common_includes.hpp"

struct grenadedata
{
	grenadedata()
	{
		grenadeType = 0;
		predictedPath = std::vector<Vector>();
		predictedOtherCollisions = std::vector<std::pair<Vector, QAngle>>();
	}
	int grenadeType;
	std::vector<Vector> predictedPath;
	std::vector<std::pair<Vector, QAngle>> predictedOtherCollisions;
};

static grenadedata grendat[65];

class GrenadePrediction : public Singleton<GrenadePrediction>
{
public:
	void View();
	void Paint();

	void Setup(C_BasePlayer* pl, Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles);
	void Simulate(QAngle& Angles, C_BasePlayer* pLocal);
	int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval, C_BasePlayer* player);
	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval, C_BasePlayer* pl);
	void TraceHull(Vector& src, Vector& end, trace_t& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);

	Color TracerColor = Color(255, 255, 0, 255);
	bool firegrenade_didnt_hit = false;
};