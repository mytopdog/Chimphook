#pragma once

#include "hooks.hpp"

struct LagRecord
{
	Vector m_vecVelocity;
	Vector m_vAbsOrigin;
	QAngle m_vecAngles;
	Vector m_eyeAngles;
	int m_iTargetID;
	int m_nSequence;
	int m_iWantedTick;
	int m_iBacktrackedTick;
	float m_flSimulationTime;
	float m_flCycle;
	float m_flPoseParameter[24];
	float m_flLowerBodyYawTarget;
	unsigned char flags;
	bool m_bIsBreakingLagComp;
	bool m_bIsFixed;
	Vector headSpot;

	LagRecord()
	{
		m_iTargetID = -1;
		m_flSimulationTime = -1;
		m_iWantedTick = -1;
		m_bIsBreakingLagComp = false;
		m_bIsFixed = false;
		m_iBacktrackedTick = -1;
		m_vAbsOrigin = Vector(0, 0, 0);
		m_flCycle = -1;
		m_nSequence = -1;
		m_flLowerBodyYawTarget = 0;
		m_flPoseParameter[24] = 0;
		m_vecAngles = QAngle(0, 0, 0);
		flags = -1;
		m_vecVelocity = Vector(0, 0, 0);
		m_eyeAngles = Vector(0, 0, 0);
		headSpot = Vector(0, 0, 0);
	}

	LagRecord(const LagRecord& src)
	{
		m_flCycle = src.m_flCycle;
		m_nSequence = src.m_nSequence;
		m_iTargetID = src.m_iTargetID;
		m_vecAngles = src.m_vecAngles;
		m_flSimulationTime = src.m_flSimulationTime;
		m_iWantedTick = src.m_iWantedTick;
		m_bIsBreakingLagComp = src.m_bIsBreakingLagComp;
		m_bIsFixed = src.m_bIsFixed;
		m_vAbsOrigin = src.m_vAbsOrigin;
		m_iBacktrackedTick = src.m_iBacktrackedTick;
		flags = src.flags;
		m_flLowerBodyYawTarget = src.m_flLowerBodyYawTarget;
		m_flPoseParameter[24] = src.m_flPoseParameter[24];
		m_vecVelocity = src.m_vecVelocity;
		m_eyeAngles = src.m_eyeAngles;
		headSpot = src.headSpot;
	}
};

class LagCompensation
{
public:

	LagRecord m_LagRecord[64][11] = {};
	LagRecord m_PrevRecords[64] = {};

public:

	LagCompensation();

	void logEntity(C_BasePlayer* player);
	void logCurrentEnt(C_BasePlayer* player);
	void setEntity(C_BasePlayer* player, LagRecord record);
	void setCurrentEnt(C_BasePlayer* player);
	int fakeLagFix(C_BasePlayer* player, int historyIdx);
	void initLagRecord();
	int fixTickcount(C_BasePlayer* player);
	bool isValidTick(float simTime);
	void log(C_BasePlayer* player);
	bool lbyShallUpdate(IClientEntity* player);

private:

	template<class T, class U>
	T clamp(T in, U low, U high);
	float lerpTime();
};

extern LagCompensation* lagComp;

