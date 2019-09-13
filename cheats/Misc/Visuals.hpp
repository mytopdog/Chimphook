#pragma once

#include "../common_includes.hpp"

void CameraFOV(CViewSetup* vsView);
void ViewmodelFOV(CViewSetup* vsView);
bool FakeZoom(CViewSetup* vsView);
void DoThirdPersonSpectate();

class LastState : public Singleton<LastState>
{
public:
	QAngle viewangles = QAngle(0, 0, 0);
	QAngle virtualViewangles = QAngle(0, 0, 0);
};

namespace FreeLook
{
	void OnCreateMove(CUserCmd* cmd);
	void OverrideView(CViewSetup* vsView);
}

class Nightmode : public Singleton<Nightmode>
{
public:
	const char* currentSkyName;

	void Clear();
	void Execute();
};

namespace WorldColour
{
	void Clear();
	void Execute();
}