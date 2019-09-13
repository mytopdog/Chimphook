#include "GrenadePrediction.hpp"

bool Settings::ESP::GrenadePrediction::Enabled = false;
bool Settings::ESP::GrenadePrediction::OnlyLocal = false;

float CSGO_Armor1(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;

	if (ArmorValue > 0)
	{
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;
		
		if (flArmor > static_cast<float>(ArmorValue))
		{
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}

	return flDamage;
}

grenadedata& getlocal()
{
	return grendat[g_LocalPlayer->EntIndex()];
}

grenadedata& getpl(C_BasePlayer* pl)
{
	return grendat[pl->EntIndex()];
}

void GrenadePrediction::View()
{
	if (!Settings::ESP::GrenadePrediction::Enabled)
		return;

	if (g_LocalPlayer && g_LocalPlayer->IsAlive())
	{
		C_BaseCombatWeapon* weapon = g_LocalPlayer->m_hActiveWeapon();
		if (weapon && weapon->IsGrenade())
		{
			QAngle Angles;
			g_EngineClient->GetViewAngles(&Angles);

			grendat[g_LocalPlayer->EntIndex()].grenadeType = weapon->m_iItemDefinitionIndex();
			Simulate(Angles, g_LocalPlayer);
		}
		else
		{
			grendat[g_LocalPlayer->EntIndex()].grenadeType = 0;
		}
	}

	if (Settings::ESP::GrenadePrediction::OnlyLocal)
		return;

	for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (pl && pl->IsAlive() && pl != g_LocalPlayer)
		{
			C_BaseCombatWeapon* weapon = pl->m_hActiveWeapon();

			if (weapon && weapon->IsGrenade())
			{
				QAngle angles = pl->GetAngles();

				grendat[pl->EntIndex()].grenadeType = weapon->m_iItemDefinitionIndex();
				Simulate(angles, pl);
			}
			else
			{
				grendat[pl->EntIndex()].grenadeType = 0;
			}
		}
	}
}

void GrenadePrediction::Paint()
{
	if (!Settings::ESP::GrenadePrediction::Enabled)
		return;

	if (!g_LocalPlayer)
		return;

	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	if ((grendat[g_LocalPlayer->EntIndex()].grenadeType) && grendat[g_LocalPlayer->EntIndex()].predictedPath.size() > 1 && weapon->IsGrenade())
	{
		Vector ab, cd;
		Vector prev = grendat[g_LocalPlayer->EntIndex()].predictedPath[0];
		for (auto it = grendat[g_LocalPlayer->EntIndex()].predictedPath.begin(), end = grendat[g_LocalPlayer->EntIndex()].predictedPath.end(); it != end; ++it)
		{
			if (Math::WorldToScreen(prev, ab) && Math::WorldToScreen(*it, cd))
			{
				Render::Get().RenderLine(ab[0], ab[1], cd[0], cd[1], TracerColor);
			}
			prev = *it;
		}

		for (auto it = grendat[g_LocalPlayer->EntIndex()].predictedOtherCollisions.begin(), end = grendat[g_LocalPlayer->EntIndex()].predictedOtherCollisions.end(); it != end; ++it)
		{
			Render::Get().Render3DCube(2.f, it->second, it->first, Color(0, 255, 0, 200));
		}

		Render::Get().Render3DCube(2.f, grendat[g_LocalPlayer->EntIndex()].predictedOtherCollisions.rbegin()->second, grendat[g_LocalPlayer->EntIndex()].predictedOtherCollisions.rbegin()->first, Color(255, 0, 0, 200));

		std::string EntName;
		auto bestdmg = 0;
		static Color redcol = { 255, 0, 0, 255 };
		static Color greencol = { 25, 255, 25, 255 };
		static Color yellowgreencol = { 177, 253, 2, 255 };
		static Color yellowcol = { 255, 255, 0, 255 };
		static Color orangecol = { 255, 128, 0, 255 };
		Color* BestColor = &redcol;

		Vector endpos = grendat[g_LocalPlayer->EntIndex()].predictedPath[grendat[g_LocalPlayer->EntIndex()].predictedPath.size() - 1];
		Vector absendpos = endpos;

		float totaladded = 0.0f;

		while (totaladded < 30.0f)
		{
			if (g_EngineTrace->GetPointContents(endpos) == CONTENTS_EMPTY)
				break;

			totaladded += 2.0f;
			endpos.z += 2.0f;
		}

		C_BaseCombatWeapon* pWeapon = g_LocalPlayer->m_hActiveWeapon().Get();
		int weap_id = pWeapon->m_iItemDefinitionIndex();

		if (pWeapon &&
			weap_id == WEAPON_HEGRENADE ||
			weap_id == WEAPON_MOLOTOV ||
			weap_id == WEAPON_INCGRENADE)
		{
			for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
			{
				C_BasePlayer* pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

				if (!pEntity || pEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
					continue;

				float dist = (pEntity->m_vecOrigin() - endpos).Length();

				if (dist < 350.0f)
				{
					CTraceFilter filter;
					filter.pSkip = g_LocalPlayer;
					Ray_t ray;
					Vector NadeScreen;
					Math::WorldToScreen(endpos, NadeScreen);

					Vector vPelvis = pEntity->GetBonePos(HITBOX_PELVIS);
					ray.Init(endpos, vPelvis);
					trace_t ptr;
					g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &ptr);

					if (ptr.hit_entity == pEntity)
					{
						Vector PelvisScreen;

						Math::WorldToScreen(vPelvis, PelvisScreen);

						static float a = 105.0f;
						static float b = 25.0f;
						static float c = 140.0f;

						float d = ((((pEntity->m_vecOrigin()) - prev).Length() - b) / c);
						float flDamage = a * exp(-d * d);
						int dmg = 0;

						if (CSGO_Armor1(flDamage, pEntity->m_ArmorValue()))
							dmg = CSGO_Armor1(flDamage, pEntity->m_ArmorValue());

						Color* destcolor = dmg >= 65 ? &redcol : dmg >= 40.0f ? &orangecol : dmg >= 20.0f ? &yellowgreencol : &greencol;

						if (dmg > bestdmg)
						{
							EntName = pEntity->GetPlayerInfo().szName;
							BestColor = destcolor;
							bestdmg = dmg;
						}
					}
				}
			}
		}

		if (bestdmg > 0.f)
		{
			if (weap_id != WEAPON_HEGRENADE)
			{
				if (Math::WorldToScreen(prev, cd))
					Render::Get().RenderText(firegrenade_didnt_hit ? "No collisions" : (EntName + " will be burnt.").c_str(), ImVec2(cd[0], cd[1]), 12.f, *BestColor, true, false, f_AndaleMono);
			}
			else
			{
				if (Math::WorldToScreen(*grendat[g_LocalPlayer->EntIndex()].predictedPath.begin(), cd))
					Render::Get().RenderText(("Most damage dealt to: " + EntName + " -" + std::to_string(bestdmg)).c_str(), ImVec2(cd[0], cd[1]), 12.f, *BestColor, true, false, f_AndaleMono);
			}
		}
	}

	if (Settings::ESP::GrenadePrediction::OnlyLocal)
		return;

	for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pl = C_BasePlayer::GetPlayerByIndex(i);

		if (!pl || !pl->IsAlive() || pl == g_LocalPlayer || pl->IsDormant())
			continue;

		C_BaseCombatWeapon* plwep = pl->m_hActiveWeapon();

		if (!plwep && grendat[i].grenadeType && grendat[i].predictedPath.size() > 1)
			continue;


		if ((grendat[i].grenadeType) && grendat[i].predictedPath.size() > 1 && plwep->IsGrenade())
		{
			Vector ab, cd;
			Vector prev = grendat[i].predictedPath[0];
			for (auto it = grendat[i].predictedPath.begin(), end = grendat[i].predictedPath.end(); it != end; ++it)
			{
				if (Math::WorldToScreen(prev, ab) && Math::WorldToScreen(*it, cd))
				{
					Render::Get().RenderLine(ab[0], ab[1], cd[0], cd[1], TracerColor);
				}
				prev = *it;
			}

			for (auto it = grendat[i].predictedOtherCollisions.begin(), end = grendat[i].predictedOtherCollisions.end(); it != end; ++it)
			{
				Render::Get().Render3DCube(2.f, it->second, it->first, Color(0, 255, 0, 200));
			}

			Render::Get().Render3DCube(2.f, grendat[i].predictedOtherCollisions.rbegin()->second, grendat[i].predictedOtherCollisions.rbegin()->first, Color(255, 0, 0, 200));

			std::string EntName;
			auto bestdmg = 0;
			static Color redcol = { 255, 0, 0, 255 };
			static Color greencol = { 25, 255, 25, 255 };
			static Color yellowgreencol = { 177, 253, 2, 255 };
			static Color yellowcol = { 255, 255, 0, 255 };
			static Color orangecol = { 255, 128, 0, 255 };
			Color* BestColor = &redcol;

			Vector endpos = grendat[i].predictedPath[grendat[i].predictedPath.size() - 1];
			Vector absendpos = endpos;

			float totaladded = 0.0f;

			while (totaladded < 30.0f)
			{
				if (g_EngineTrace->GetPointContents(endpos) == CONTENTS_EMPTY)
					break;

				totaladded += 2.0f;
				endpos.z += 2.0f;
			}
		}
	}
}

void GrenadePrediction::Setup(C_BasePlayer* pl, Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles)
{
	QAngle angThrow = angEyeAngles;
	float pitch = angThrow.pitch;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.pitch = a;

	float flVel = 750.0f * 0.9f;

	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float o = ((C_BaseCSGrenade*) & *pl->m_hActiveWeapon())->m_flThrowStrength();
	float b = o;
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	Math::AngleVectors(angThrow, vForward, vRight, vUp);
	vecSrc = pl->GetEyePos();
	float off = (o * 12.0f) - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	vecThrow = pl->m_vecVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void GrenadePrediction::Simulate(QAngle& Angles, C_BasePlayer* pLocal)
{
	Vector vecSrc, vecThrow;
	Setup(pLocal, vecSrc, vecThrow, Angles);

	float interval = g_GlobalVars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	grendat[pLocal->EntIndex()].predictedPath.clear();
	grendat[pLocal->EntIndex()].predictedOtherCollisions.clear();
	// TracerColor = Color(255, 255, 0, 255);
	TracerColor = Color(255, 255, 255, 255);
	for (unsigned int i = 0; i < grendat[pLocal->EntIndex()].predictedPath.max_size() - 1; ++i)
	{
		if (!logtimer)
			grendat[pLocal->EntIndex()].predictedPath.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval, pLocal);
		if ((s & 1) || vecThrow == Vector(0, 0, 0))
			break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}

	grendat[pLocal->EntIndex()].predictedPath.push_back(vecSrc);
}

int GrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval, C_BasePlayer* player)
{
	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval, player))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	if (tr.hit_entity && ((C_BasePlayer*)tr.hit_entity)->IsPlayer())
	{
		// TracerColor = Color(255, 0, 0, 255);
		TracerColor = Color(255, 255, 255, 255);
	}

	if ((result & 1) || vecThrow == Vector(0, 0, 0) || tr.fraction != 1.0f)
	{
		QAngle angles;
		Math::VectorAngles((tr.endpos - tr.startpos).Normalized(), angles);
		grendat[player->EntIndex()].predictedOtherCollisions.push_back(std::make_pair(tr.endpos, angles));
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}

bool GrenadePrediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval, C_BasePlayer* pl)
{
	firegrenade_didnt_hit = false;
	switch (grendat[pl->EntIndex()].grenadeType)
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;

	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		return static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));

	default:
		return false;
	}
}

void GrenadePrediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	// Setup grenade hull
	static const Vector hull[2] = { Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f) };

	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceFilterWorldAndPropsOnly filter;

	g_EngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void GrenadePrediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void GrenadePrediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void GrenadePrediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int GrenadePrediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}
