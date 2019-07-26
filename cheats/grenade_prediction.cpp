#include "grenade_prediction.h"

GrenadePrediction::GrenadePrediction() :
	tickcount(0),
	curtime(0.f),
	m_nNextThinkTick(0),
	m_nWaterType(0),
	m_CollisionGroup(COLLISION_GROUP_PROJECTILE),
	m_flDetonateTime(0),
	m_nBounces(0),
	m_CollisionEntity(nullptr),
	m_Detonated(false),
	m_WeaponID(0),
	m_LastUpdateTick(0)
{
}

void GrenadePrediction::Init(CUserCmd* cmd)
{
	m_CurrentPath.Old = true;

	bool IsObserver;
	C_BasePlayer* Player = g_LocalPlayer->SelfOrObs(&IsObserver);
	if (!Player)
		return;

	C_BaseCombatWeapon* Weapon = Player->m_hActiveWeapon();
	if (!Weapon)
		return;

	auto Origin = Player->GetEyePos();

	if (Weapon->IsGrenade())
	{
		auto Grenade = reinterpret_cast<C_BaseCSGrenade*>(Weapon);
		if (Grenade->m_bPinPulled() || Grenade->m_fThrowTime())
		{
			m_WeaponID = Weapon->m_iItemDefinitionIndex();
			Predict(Grenade, Origin, IsObserver ? Player->m_angEyeAngles() : cmd->viewangles);
		}
	}

	if (m_CurrentPath.Old)
	{
		if (m_OldPaths.empty() && !m_CurrentPath.Points.empty() || !m_OldPaths.empty() && m_OldPaths.back().EraseTime != m_CurrentPath.EraseTime)
		{
			m_OldPaths.emplace_back(m_CurrentPath);
			m_CurrentPath.Points.clear();
		}
	}
}

void GrenadePrediction::PopulateGeometry()
{
	static auto DrawPath = [](Path& Path)
	{
		if (Path.Points.empty())
			return;

		auto Curtime = g_GlobalVars->curtime;
		if (Curtime - Path.EraseTime > 0.f)
		{
			const auto EraseTime = 1.f / 500.f;

			if (Curtime - Path.LastEraseTime > EraseTime)
			{
				Path.Points.pop_front();

				Path.LastEraseTime = Curtime + EraseTime;
			}
		}

		if (Path.Points.empty())
			return;

		//G.SetAntialias(true);

		//G.DrawSphere(Path.Points.back().Location, 100.f, Color::Green(100)); // TO DO: Endpoint visualization based on m_WeaponID

		auto LastOnScreen = false;
		Point LastLocation;
		for (const auto& Point : Path.Points)
		{
			Drawing::Point Location;
			auto OnScreen = WorldToScreen(Point, Location);

			if (LastOnScreen && OnScreen)
				Render::Get().RenderLine(LastLocation.x, LastLocation.y, Location.x, Location.y, Color(255, 0, 0));

			if (OnScreen && Point.Bounced)
				Render::Get().Render3DCube(2.f, Point.Location, )
				G.Draw3DBox(Point.Location, Math::QAngle::Clear(), Math::Vector(-2.f, -2.f, -2.f), Math::Vector(2.f, 2.f, 2.f), Color::Blue(100)); // TO DO: Maybe draw these last? I think a line above looks weird

			LastOnScreen = OnScreen;
			LastLocation = Location;
		}
	};

	for (auto it = m_OldPaths.begin(); it != m_OldPaths.end(); )
	{
		DrawPath(*it);

		if (it->Points.empty())
			it = m_OldPaths.erase(it);
		else
			++it;
	}

	DrawPath(m_CurrentPath);
}

void GrenadePrediction::OnNewMap(const char* Map) const // TO DO: Maybe handle more events
{
	struct EventData
	{
		EventData(const std::string& Targetname, const std::vector<std::string>& OnBreak) :
			Targetname(Targetname),
			OnBreak(OnBreak)
		{
		}

		std::string Targetname;
		std::vector<std::string> OnBreak;
	};
	std::vector<EventData> Events;
	Breakables.clear();

	static auto FindEntitiesByTargetname = [](const std::vector<EventData> & Events, const std::string & Targetname) // See https://github.com/LestaD/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/server/entitylist.cpp#L620 as well
	{
		std::vector<Breakable*> Entities;

		for (size_t i = 0; i < Events.size(); ++i)
		{
			auto it1 = Events[i].Targetname.begin();
			auto it2 = Targetname.begin();

			while (it1 != Events[i].Targetname.end() && it2 != Targetname.end())
			{
				if (*it1 != *it2 && tolower(*it1) != tolower(*it2))
					break;

				++it1;
				++it2;
			}

			if (it2 == Targetname.end())
			{
				if (it1 == Events[i].Targetname.end())
					Entities.emplace_back(&Breakables[i]);
			}
			else if (*it2 == '*')
				Entities.emplace_back(&Breakables[i]);
		}

		return Entities;
	};

	auto Entities = GetMapEntities(Map); // It's up to you to parse the BSP file
	for (const auto& Entity : Entities)
	{
		auto Classname = Entity.GetValue(XorStringC("classname"));

		BreakableType Type;
		if (Classname == XorStringC("func_breakable"))
			Type = BreakableType::func_breakable;
		else if (Classname == XorStringC("prop_dynamic") || Classname == XorStringC("prop_dynamic_override"))
			Type = BreakableType::prop_dynamic;
		else if (Classname == XorStringC("func_breakable_surf"))
			Type = BreakableType::func_breakable_surf;
		/*else if ( Classname == XorStringC( "prop_physics_multiplayer" ) ) // This never seems to break like I want it to so let's just ignore it, works fine in all official maps 10/4/17
			Type = BreakableType::prop_physics_multiplayer;*/
		else
			continue;

		if (Type == BreakableType::prop_dynamic && atoi(Entity.GetValue(XorStringC("solid")).c_str()) == SOLID_NONE) // TO DO: Figure out how to filter out bs better to save some iterations inside IsBreakableEntity
			continue;

		Breakable Breakable;
		Breakable.Model = Entity.GetValue(XorStringC("model"));
		Breakable.Health = atoi(Entity.GetValue(XorStringC("health")).c_str());
		Breakable.Type = Type;
		Breakables.emplace_back(Breakable);

		Events.emplace_back(Entity.GetValue(XorStringC("targetname")), Entity.GetValues(XorStringC("OnBreak")));
	}

	for (size_t i = 0; i < Events.size(); ++i)
	{
		for (const auto& OnBreak : Events[i].OnBreak)
		{
			auto Event = ParseEvent(OnBreak); // Just split the string with '\x1B'
			if (Event.TargetInput != XorStringC("Break"))
				continue;

			auto Found = FindEntitiesByTargetname(Events, Event.Target); // TO DO: Maybe use the delay, keeping in mind that it is completely useless in the official maps
			if (Found.empty())
				continue; // Maybe error?

			Breakables[i].Breakables.insert(Breakables[i].Breakables.end(), Found.begin(), Found.end());
		}
	}
}

void GrenadePrediction::Predict(C_BaseCSGrenade * Grenade, const Math::Vector & Origin, const Math::QAngle & Angle) // Running this w/ low sv_gravity w/ smoke/decoy can kill your fps
{
	const auto SamplesPerSecond = 30.f;

	ThrowGrenade(Grenade, Origin, Angle);

	const auto SampleTick = static_cast<int>(1.f / SamplesPerSecond / gpGlobals->interval_per_tick);
	m_LastUpdateTick = -SampleTick;

	m_CurrentPath.Old = false;
	m_CurrentPath.Points.clear();
	for (tickcount = 0; ; ++tickcount)
	{
		if (curtime > 60.f) // This is here because of (see the comment above)
		{
			Warning(XorStringC("The grenade lives more than a minute. Too much time spent predicting!\n"));

			break;
		}

		if (m_LastUpdateTick + SampleTick <= tickcount)
			UpdatePath(false);

		PhysicsSimulate();

		if (m_Detonated)
			break;

		curtime += gpGlobals->interval_per_tick;
	}

	// TO DO: Check if we actually threw the nade and we didn't just switch weapons
	m_CurrentPath.LastEraseTime = 0.f;
	m_CurrentPath.EraseTime = Prediction::Curtime() + curtime + 2.f; // Start erasing the path 2 seconds after the nade popped
}

void GrenadePrediction::ThrowGrenade(C_BaseCSGrenade * Grenade, Math::Vector vecSrc, Math::QAngle angThrow)
{
	auto pPlayer = C_BasePlayer::GetLocalPlayerOrObserverPlayer();

	if (angThrow.x() < -90)
		angThrow.x() += 360.f;
	else if (angThrow.x() > 90.f)
		angThrow.x() -= 360.f;

	angThrow.x() -= (90.f - abs(angThrow.x())) * 10.f / 90.f;

	auto ThrowStrength = std::clamp(Grenade->m_flThrowStrength(), 0.f, 1.f);

	auto flVel = std::clamp(Grenade->GetWeaponData()->ThrowVelocity * 0.9f, 15.f, 750.f);
	flVel *= ThrowStrength * 0.7f + 0.3f;

	Math::Vector vForward;
	AngleVectors(angThrow, &vForward);

	vecSrc.z() += ThrowStrength * 12.f - 12.f;

	trace_t trace;
	Math::Vector mins(-2.f, -2.f, -2.f);
	Math::Vector maxs(2.f, 2.f, 2.f);
	UTIL_TraceHull(vecSrc, vecSrc + vForward * 22.f, mins, maxs, MASK_SOLID | CONTENTS_CSGO_CUSTOM, pPlayer, COLLISION_GROUP_NONE, &trace);
	vecSrc = trace.endpos - vForward * 6.f;

	auto vecThrow = vForward * flVel + pPlayer->GetAbsVelocity() * 1.25f;

	EmitGrenade(vecSrc, vecThrow);
}

void GrenadePrediction::EmitGrenade(const Math::Vector & vecSrc, const Math::Vector & vecVel)
{
	curtime = 0.f;
	m_vecAbsVelocity = vecVel;
	m_vecAbsOrigin = vecSrc;
	m_CollisionGroup = COLLISION_GROUP_PROJECTILE;
	m_nBounces = 0;
	m_CollisionEntity = nullptr; // This is a handle in game code. I don't know how Valve named it...
	m_Detonated = false;
	UpdateWaterState();
	ClearBrokenEntities();

	switch (m_WeaponID)
	{
	case WEAPON_SMOKEGRENADE:
		SetNextThink(1.5f);

		break;
	case WEAPON_DECOY:
		SetNextThink(2.f);

		break;
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		m_flDetonateTime = 1.5f;

		SetNextThink(0.02f); // I really don't fucking know... I pulled this out of my ass because of some 3 tick diff

		break;
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		m_flDetonateTime = molotov_throw_detonate_time->GetFloat();

		SetNextThink(0.02f); // I really don't fucking know... I pulled this out of my ass because of some 3 tick diff

		break;
	default:
		break;
	}
}

void GrenadePrediction::PhysicsSimulate() // PhysicsToss
{
	trace_t trace;
	Math::Vector move;

	PhysicsRunThink();

	if (m_Detonated)
		return;

	PhysicsAddGravityMove(move);
	PhysicsPushEntity(move, &trace);

	// if ( grenade + 0x1C ) return;

	if (m_Detonated)
		return;

	if (trace.fraction != 1.f)
	{
		UpdatePath(true);

		PerformFlyCollisionResolution(trace);
	}

	PhysicsCheckWaterTransition();
}

void GrenadePrediction::NormalizedToWorldSpace(const Math::Vector & in, Math::Vector * pResult) const
{
	Math::Vector m_vecMins(-2.f, -2.f, -2.f);
	Math::Vector m_vecMaxs(2.f, 2.f, 2.f);

	*pResult = Lerp(m_vecMins, m_vecMaxs, in) + m_vecAbsOrigin;
}

float GrenadePrediction::GetActualGravity()
{
	auto ent_gravity = 0.4f;/* pEnt->GetGravity();
	if ( !ent_gravity )
		ent_gravity = 1.f;*/

	return ent_gravity * sv_gravity->GetFloat();
}

void GrenadePrediction::PhysicsAddGravityMove(Math::Vector & move)
{
	move.x() = m_vecAbsVelocity.x() * gpGlobals->interval_per_tick;
	move.y() = m_vecAbsVelocity.y() * gpGlobals->interval_per_tick;

	auto newZVelocity = m_vecAbsVelocity.z() - GetActualGravity() * gpGlobals->interval_per_tick;
	move.z() = (m_vecAbsVelocity.z() + newZVelocity) / 2.f * gpGlobals->interval_per_tick;
	m_vecAbsVelocity.z() = newZVelocity;
}

void GrenadePrediction::Physics_TraceEntity(const Math::Vector & vecAbsStart, const Math::Vector & vecAbsEnd, unsigned int mask, trace_t * ptr) const
{
	UTIL_TraceEntity(vecAbsStart, vecAbsEnd, mask, ptr);

	if (ptr->startsolid && ptr->contents & CONTENTS_CSGO_CUSTOM)
	{
		UTIL_ClearTrace(*ptr);
		UTIL_TraceEntity(vecAbsStart, vecAbsEnd, mask & ~CONTENTS_CSGO_CUSTOM, ptr);
	}

	if (ptr->fraction < 1.f || ptr->allsolid || ptr->startsolid)
	{
		if (ptr->m_pEnt && ptr->m_pEnt->IsPlayer())
		{
			UTIL_ClearTrace(*ptr);

			UTIL_TraceLine(vecAbsStart, vecAbsEnd, mask, C_BasePlayer::GetLocalPlayerOrObserverPlayer(), m_CollisionGroup, ptr);
		}
	}
}

void GrenadePrediction::PhysicsCheckSweep(const Math::Vector & vecAbsStart, const Math::Vector & vecAbsDelta, trace_t * pTrace) const // TO DO: Figure out why the vents on cache don't go through my filter and directly end up in this trace
{
	// PhysicsSolidMaskForEntity
	unsigned int mask;
	if (m_CollisionGroup == COLLISION_GROUP_DEBRIS)
		mask = (MASK_SOLID | CONTENTS_CSGO_CUSTOM) & ~CONTENTS_MONSTER;
	else
		mask = MASK_SOLID | CONTENTS_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE | CONTENTS_CSGO_CUSTOM | CONTENTS_HITBOX;

	// I doubt !pEntity->IsSolid() || pEntity->IsSolidFlagSet( FSOLID_VOLUME_CONTENTS ) ever gets evaluated to true

	Physics_TraceEntity(vecAbsStart, vecAbsStart + vecAbsDelta, mask, pTrace);
}

void GrenadePrediction::PhysicsPushEntity(const Math::Vector & push, trace_t * pTrace)	// TO DO: Debug. See if this gets called from ResolveFlyCollisionCustom and the other places I call it from
{
	PhysicsCheckSweep(m_vecAbsOrigin, push, pTrace);

	if (pTrace->startsolid) // I'm pretty sure this is what caused https://www.youtube.com/watch?v=nf4aWS6rM5Y
	{
		m_CollisionGroup = COLLISION_GROUP_INTERACTIVE_DEBRIS;

		UTIL_TraceLine(m_vecAbsOrigin - push, m_vecAbsOrigin + push, (MASK_SOLID | CONTENTS_CSGO_CUSTOM) & ~CONTENTS_MONSTER, C_BasePlayer::GetLocalPlayerOrObserverPlayer(), m_CollisionGroup, pTrace);
	}

	if (pTrace->fraction)
		m_vecAbsOrigin = pTrace->endpos;

	if (pTrace->m_pEnt)
	{
		//auto result = m_fFlags | pTrace->m_pEnt->m_fFlags();
		//if ( !(result & FL_ONFIRE ) )
		PhysicsImpact(pTrace->m_pEnt, *pTrace);
	}
}

void GrenadePrediction::PerformFlyCollisionResolution(trace_t & trace)	// Actually CBaseCSGrenadeProjectile::ResolveFlyCollisionCustom
{
	auto flSurfaceElasticity = 1.f;

	if (trace.m_pEnt)
	{
		if (!IsEntityBroken(trace.m_pEnt))
		{
			if (entity_cast<CChicken>(trace.m_pEnt))
			{
				MarkEntityAsBroken(trace.m_pEnt);

				return;
			}

			if (auto Breakable = IsBreakableEntity(trace.m_pEnt)) // Not really what I want
			{
				if (Breakable->Health - 10 <= 0) // TO DO: Figure out how to check for damage taken from bullets or other shit since it doesn't get networked ALSO scale that 10
				{
					MarkEntityAsBroken(*Breakable, trace.m_pEnt);

					m_vecAbsVelocity *= 0.4f;

					return;
				}
			}
		}

		if (trace.m_pEnt->IsPlayer())
			flSurfaceElasticity = 0.3f;

		if (!trace.DidHitWorld())
		{
			if (m_CollisionEntity == trace.m_pEnt)
			{
				if (trace.m_pEnt->IsPlayer() /*|| IsHostage || IsGrenadeProjectile*/)
				{
					m_CollisionGroup = COLLISION_GROUP_DEBRIS;

					//	if ( IsGrenadeProjectile )
					//		trace.m_pEnt->m_CollisionGroup = COLLISION_GROUP_DEBRIS;

					return;
				}
			}

			m_CollisionEntity = trace.m_pEnt;
		}
	}

	auto flTotalElasticity = GetElasticity() * flSurfaceElasticity;
	flTotalElasticity = std::clamp(flTotalElasticity, 0.f, 0.9f);

	Math::Vector vecAbsVelocity;
	PhysicsClipVelocity(m_vecAbsVelocity, trace.plane.normal, vecAbsVelocity, 2.f);
	vecAbsVelocity *= flTotalElasticity;

	if (trace.plane.normal.z() > 0.7f)
	{
		auto flSpeedSqr = vecAbsVelocity.Dot(vecAbsVelocity);
		if (flSpeedSqr > 96000.f)
		{
			auto l = vecAbsVelocity.Normalized().Dot(trace.plane.normal);
			if (l > 0.5f)
				vecAbsVelocity *= 1.f - l + 0.5f;
		}

		if (flSpeedSqr < 20.f * 20.f)
			m_vecAbsVelocity = Math::Vector::Clear();
		else
		{
			m_vecAbsVelocity = vecAbsVelocity;

			PhysicsPushEntity(vecAbsVelocity * ((1.f - trace.fraction) * gpGlobals->interval_per_tick), &trace);
		}
	}
	else
	{
		m_vecAbsVelocity = vecAbsVelocity;

		PhysicsPushEntity(vecAbsVelocity * ((1.f - trace.fraction) * gpGlobals->interval_per_tick), &trace);
	}

	if (m_nBounces > 20)
		Detonate(); // not really this but whatever
	else
		++m_nBounces;
}

void GrenadePrediction::PhysicsClipVelocity(const Math::Vector & in, const Math::Vector & normal, Math::Vector & out, float overbounce)
{
	const auto STOP_EPSILON = 0.1f;

	auto backoff = in.Dot(normal) * overbounce;
	for (auto i = 0; i < 3; ++i)
	{
		auto change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0.f;
	}
}

void GrenadePrediction::UpdateWaterState()
{
	Math::Vector point;
	NormalizedToWorldSpace(Math::Vector(0.5f, 0.5f, 0.f), &point);

	m_nWaterType = 0;

	auto cont = enginetrace->GetPointContents_WorldOnly(point, MASK_WATER);
	if (!(cont & MASK_WATER))
		return;

	m_nWaterType = cont;
}

void GrenadePrediction::PhysicsCheckWaterTransition()
{
	auto oldcont = m_nWaterType;
	UpdateWaterState();

	if (m_nWaterType && !oldcont)
	{
		UpdatePath(false);

		m_vecAbsVelocity.z() *= 0.5f;
	}
}

void GrenadePrediction::PhysicsRunThink()
{
	if (m_nNextThinkTick > tickcount)
		return;

	Think();
}

void GrenadePrediction::Think()
{
	switch (m_WeaponID)
	{
	case WEAPON_SMOKEGRENADE:
		if (m_vecAbsVelocity.LengthSqr() <= 0.1f * 0.1f)
			Detonate();

		break;
	case WEAPON_DECOY:
		if (m_vecAbsVelocity.LengthSqr() <= 0.2f * 0.2f)
			Detonate();

		break;
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		if (curtime > m_flDetonateTime)
			Detonate();

		if (m_nWaterType) // GetWaterLevel
			m_vecAbsVelocity *= 0.5f;

		break;
	default:
		break;
	}

	SetNextThink(curtime + 0.2f);
}

void GrenadePrediction::Touch(C_BaseEntity * pOther, trace_t & g_TouchTrace)
{
	if (m_WeaponID == WEAPON_MOLOTOV || m_WeaponID == WEAPON_INCGRENADE)
	{
		if (!pOther->IsPlayer() && g_TouchTrace.plane.normal.z() >= Math::cosd(weapon_molotov_maxdetonateslope->GetFloat()))
			Detonate(true);
	}
	else if (m_WeaponID == WEAPON_TAGRENADE)
	{
		if (!pOther->IsPlayer())
			Detonate(true);
	}
}

void GrenadePrediction::PhysicsImpact(C_BaseEntity * other, trace_t & trace)
{
	// TO DO: maybe more checks from PhysicsMarkEntityAsTouched
	// Pretty sure molotov checks FSOLID_TRIGGER | FSOLID_VOLUME_CONTENTS

	Touch(other, trace);
}

void GrenadePrediction::Detonate(bool Bounced /*= false*/)
{
	m_Detonated = true;

	UpdatePath(Bounced);
}

void GrenadePrediction::UpdatePath(bool Bounced)
{
	m_CurrentPath.Points.emplace_back(m_vecAbsOrigin, Bounced);
	m_LastUpdateTick = tickcount;
}

void GrenadePrediction::SetNextThink(float thinkTime)
{
	m_nNextThinkTick = static_cast<int>(thinkTime / gpGlobals->interval_per_tick + 0.5f);
}

void GrenadePrediction::UTIL_ClearTrace(trace_t & trace)
{
	memset(&trace, 0, sizeof trace);
	trace.fraction = 1.f;
	trace.surface.name = XorStringC("**empty**");
}

void GrenadePrediction::UTIL_TraceEntity(const Math::Vector & vecAbsStart, const Math::Vector & vecAbsEnd, unsigned int mask, trace_t * ptr) const
{
	Math::Vector mins(-2.f, -2.f, -2.f);
	Math::Vector maxs(2.f, 2.f, 2.f);
	UTIL_TraceHull(vecAbsStart, vecAbsEnd, mins, maxs, mask, C_BasePlayer::GetLocalPlayerOrObserverPlayer(), m_CollisionGroup, ptr);
}

void GrenadePrediction::UTIL_TraceHull(const Math::Vector & vecAbsStart, const Math::Vector & vecAbsEnd, const Math::Vector & hullMin, const Math::Vector & hullMax, unsigned int mask, const IHandleEntity * ignore, int collisionGroup, trace_t * ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);
	CTraceFilterEntity traceFilter(ignore, collisionGroup);

	enginetrace->TraceRay(ray, mask, &traceFilter, ptr);
}

void GrenadePrediction::UTIL_TraceLine(const Math::Vector & vecAbsStart, const Math::Vector & vecAbsEnd, unsigned int mask, const IHandleEntity * ignore, int collisionGroup, trace_t * ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilterEntity traceFilter(ignore, collisionGroup);

	enginetrace->TraceRay(ray, mask, &traceFilter, ptr);
}

std::vector<GrenadePrediction::Breakable> GrenadePrediction::Breakables;
GrenadePrediction::Breakable* GrenadePrediction::IsBreakableEntity(C_BaseEntity * Entity) const
{
	auto Model = Entity->GetModel();
	if (!Model)
		return nullptr;

	auto ModelName = modelinfo->GetModelName(Model);
	for (auto& Breakable : Breakables)
	{
		if (Breakable.Model == ModelName)
		{
			if (Breakable.Type == BreakableType::prop_dynamic || Breakable.Type == BreakableType::prop_physics_multiplayer)
			{
				auto Breakables = 0;
				if (auto Collide = modelinfo->GetVCollide(Model))
				{
					auto Parser = physcollision->VPhysicsKeyParserCreate(Collide);
					while (!Parser->Finished())
					{
						if (!_strcmpi(Parser->GetCurrentBlockName(), XorStringC("break")))
							++Breakables;

						Parser->SkipBlock();
					}
					physcollision->VPhysicsKeyParserDestroy(Parser);
				}

				if (!Breakables)
					return nullptr;

				// TO DO: Properly look for "health" https://developer.valvesoftware.com/wiki/Prop_data, https://github.com/LestaD/SourceEngine2007/blob/master/se2007/game/shared/props_shared.cpp
				if (auto KeyValueText = modelinfo->GetModelKeyValueText(Model))
				{
					if (auto HealthLocation = strstr(KeyValueText, XorStringC("health")))
					{
						Breakable.Health = atoi(HealthLocation + 9);

						return &Breakable;
					}

					if (!strstr(KeyValueText, XorStringC("Window")))
						return nullptr;
				}
				else
					return nullptr;
			}

			return &Breakable;
		}
	}

	return nullptr;
}

// Keep track of "broken" entities
std::vector<IClientUnknown*> BrokenEntities;
void GrenadePrediction::ClearBrokenEntities()
{
	BrokenEntities.clear();
}

void GrenadePrediction::MarkEntityAsBroken(const Breakable & Breakable, IClientUnknown * Entity)
{
	static auto FindEntityByModel = [](const char* Model) -> C_BaseEntity *
	{
		auto Hash = HashString(Model);
		for (auto i = gpGlobals->maxClients + 1; i <= entitylist->GetHighestEntityIndex(); ++i)
		{
			auto Entity = entitylist->GetClientEntity(i);
			if (!Entity)
				continue;

			auto CurrentModel = modelinfo->GetModelName(Entity->GetModel());
			if (HashString(CurrentModel) == Hash)
				return Entity;
		}

		return nullptr;
	};

	if (find(BrokenEntities.begin(), BrokenEntities.end(), Entity) != BrokenEntities.end())
		return; // Should ONLY fix infinite loops caused by "co-breaking" entities and not cause any bugs

	BrokenEntities.emplace_back(Entity);

	for (const auto& Broken : Breakable.Breakables)
	{
		if (auto BrokenEntity = FindEntityByModel(Broken->Model.c_str()))
			MarkEntityAsBroken(*Broken, BrokenEntity);
	}
}

void GrenadePrediction::MarkEntityAsBroken(IClientUnknown * Entity)
{
	BrokenEntities.emplace_back(Entity);
}

bool GrenadePrediction::IsEntityBroken(IClientUnknown * Entity)
{
	return find(BrokenEntities.begin(), BrokenEntities.end(), Entity) != BrokenEntities.end();
}

bool GrenadePrediction::CTraceFilterEntity::ShouldHitEntity(IClientUnknown * pHandleEntity, int contentsMask)
{
	if (auto Entity = pHandleEntity->GetBaseEntity())
	{
		// Entities have a "Disable Bone Followers" thingy
		if (entity_cast<CBoneFollower>(Entity))	// Really stupid fix that I don't know why it's needed... Here, inside ShouldHitEntity it's a CBoneFollower but the trace contains a prop_dynamic
		{											// If I search for GetModelKeyValueText I find some bone following shit but that still doesn't explain the discrepancy between this function and the trace
			if (auto Owner = Entity->GetOwnerEntity())
				pHandleEntity = Owner;
		}
		else if (HashString(Entity->GetClientClass()->m_pNetworkName + strlen(Entity->GetClientClass()->m_pNetworkName) - 10, 10) == HashStringC("Projectile")) // IsProjectile function maybe
			return false;
	}

	if (IsEntityBroken(pHandleEntity))
		return false;

	return BaseClass::ShouldHitEntity(pHandleEntity, contentsMask);
}