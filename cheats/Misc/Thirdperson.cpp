#include "Thirdperson.hpp"

#include "Visual.hpp"

bool Settings::Misc::ThirdPersonSpectate::Enabled = false;
bool Settings::Misc::ThirdPerson::Enabled = false;
bool Settings::Misc::ThirdPerson::IgnoreScope = false;
bool Settings::Misc::ThirdPerson::IgnorePlayers = false;
bool Settings::Misc::ThirdPerson::IgnoreWalls = false;
bool Settings::Misc::ThirdPerson::RealAngles = false;
int Settings::Misc::ThirdPerson::CameraOffset = 120;

void ThirdPerson::OverrideView()
{
	if (!g_LocalPlayer)
		return;

	if (Settings::Misc::ThirdPerson::Enabled && Settings::Misc::ThirdPerson::_enabled && (!g_LocalPlayer->SelfOrObs()->m_bIsScoped() || Settings::Misc::ThirdPerson::IgnoreScope))
	{
		if (!g_LocalPlayer->m_hObserverTarget())
		{
			if (!g_Input->m_fCameraInThirdPerson)
			{
				g_Input->m_fCameraInThirdPerson = true;
			}

			QAngle view;// = LastState::Get().virtualViewangles;
			g_EngineClient->GetViewAngles(&view);

			auto GetCorrectDistance = [](float ideal_distance, QAngle angle) -> float
			{
				angle.pitch *= -1.f, angle.yaw += 180.f;

				Vector direction;
				Math::AngleVectors(angle, direction);

				if (Settings::Misc::ThirdPerson::IgnorePlayers)
				{
					CTraceFilterWorldAndPropsOnly filter;
					trace_t trace;
					Ray_t ray;

					ray.Init(g_LocalPlayer->SelfOrObs()->GetEyePos(), g_LocalPlayer->SelfOrObs()->GetEyePos() + (direction * ideal_distance));

					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

					return (ideal_distance * trace.fraction) - 10.f;
				}
				else
				{
					CTraceFilter filter;
					filter.pSkip = g_LocalPlayer;
					trace_t trace;
					Ray_t ray;

					ray.Init(g_LocalPlayer->SelfOrObs()->GetEyePos(), g_LocalPlayer->SelfOrObs()->GetEyePos() + (direction * ideal_distance));

					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

					return (ideal_distance * trace.fraction) - 10.f;
				}
			};

			g_Input->m_fCameraInThirdPerson = true;

			if (Settings::Misc::ThirdPerson::IgnoreWalls)
				g_Input->m_vecCameraOffset.z = Settings::Misc::ThirdPerson::CameraOffset - 10.f;
			else
				g_Input->m_vecCameraOffset.z = GetCorrectDistance(Settings::Misc::ThirdPerson::CameraOffset, view);
		}
		else
		{
			g_Input->m_fCameraInThirdPerson = false;

			DoThirdPersonSpectate();
		}
	}
	else
	{
		g_Input->m_fCameraInThirdPerson = false;

		if (g_LocalPlayer->m_hObserverTarget()) DoThirdPersonSpectate();
	}
}

void ThirdPerson::FrameStageNotify(ClientFrameStage_t stage)
{
	if (!g_EngineClient->IsInGame())
		return;

	if (stage != ClientFrameStage_t::FRAME_RENDER_START)
		return;

	if (Settings::Misc::ThirdPerson::Enabled && Settings::Misc::ThirdPerson::_enabled && (!g_LocalPlayer->m_bIsScoped() || Settings::Misc::ThirdPerson::IgnoreScope) && Settings::Misc::ThirdPerson::RealAngles)
		* g_LocalPlayer->GetVAngles() = CreateMove::lastTickViewAngles;
}