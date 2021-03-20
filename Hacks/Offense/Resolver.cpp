#include "../../Include.hpp"

Resolver* resolver = new Resolver();
/*
ORDER GOES:
WEAPON_FIRE
BULLET_IMPACT
PLAYER_HURT

Logging on everything XD
*/

template<typename ...Args>
static void ConsoleColorMsg(const Color& color, const char* fmt, Args ...args)
{
	using ConColorMsg = void(*)(const Color&, const char*, ...);
	static ConColorMsg con_color_msg = nullptr;
	if (!con_color_msg) {
		con_color_msg = reinterpret_cast<ConColorMsg>(GetProcAddress(
			GetModuleHandleA("tier0.dll"),
			"?ConColorMsg@@YAXABVColor@@PBDZZ")
			);
	}

	con_color_msg(color, fmt, args...);
}

void Resolver::LogWeaponFire(GameEvent* event)
{
	// Get UserID
	int UserID = event->GetInt("userid");
	// If it's the localplayer, return
	if (I::engine->GetPlayerForUserID(UserID) != G::LocalPlayerIndex)
		return;

	// If it isnt in the list... add it!
	if (OldShotsMissed.find(UserID) == OldShotsMissed.end()) {
		// not found
		OldShotsMissed.insert(std::pair(UserID, 0));
		return;
	}

	// Otherwise it's already in the list, update everything!
	for (auto& a : ShotsMissed)
	{
		OldShotsMissed[a.first] = a.second;
	}
}

void Resolver::LogBulletImpact(GameEvent* event)
{
	int UserID = event->GetInt("userid");

	if (I::engine->GetPlayerForUserID(UserID) != G::LocalPlayerIndex)
		return;

	Vec Loc = Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

	trace_t tr;
	Ray_t ray(G::LocalPlayer->GetEyePos(), Loc);
	CTraceFilter traceFilter(G::LocalPlayer);
	I::enginetrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);
	Entity* ShouldHitEntity = tr.Entity;

	// No point in doing anything if we shouldn't have hit anything for now
	if (!ShouldHitEntity)
		return;

	// Keep track of the entity we supossedly missed due to resolver
	player_info_t info;
	if (I::engine->GetPlayerInfo(tr.Entity->Index(), &info))
		ImpactEndUserID = info.userid;
	else
		ImpactEndUserID = -1;

	// If it isn't found in newshots, ADD IT!
	if (ShotsMissed.find(info.userid) == ShotsMissed.end())
	{
		ShotsMissed.insert(std::pair(info.userid, 1));
		return;
	}

	// Otherwise...
	// If it isn't found in oldshots, return... as waiting for weaponfire is better
	if (OldShotsMissed.find(info.userid) == OldShotsMissed.end())
	{
		return;
	}

	// No need to increase if already increased once...
	if (ShotsMissed[info.userid] > OldShotsMissed[info.userid])
		return;

	// Log the shot up by 1
	ShotsMissed[info.userid]++;
}

void Resolver::LogPlayerHurt(GameEvent* event)
{
	int UserID = event->GetInt("userid");
	int iAttacker = event->GetInt("attacker");

	// if the localplayer gets hurt, return
	if (I::engine->GetPlayerForUserID(UserID) == G::LocalPlayerIndex)
		return;

	// if the localplayer isn't shootin, return
	if (I::engine->GetPlayerForUserID(iAttacker) != G::LocalPlayerIndex)
		return;

	player_info_t info;
	I::engine->GetPlayerInfo(I::engine->GetPlayerForUserID(UserID), &info);

	// if they are the same indexes --> HIT!
	if (ImpactEndUserID == UserID)
	{
		// No shots were missed, revert back to normal
		ShotsMissed[ImpactEndUserID]--;
	}
}

void Resolver::LogShots(GameEvent* event)
{
	switch (StrHash::HashRuntime(event->GetName())) {
		// Called before bullet impact
	case StrHash::Hash("weapon_fire"): //0
	{
		/*
		weapon_fire
		Note: Every time a client fires their weapon

		Name:	weapon_fire
		Structure:	
		short	userid	
		string	weapon	weapon name used
		bool	silenced	is weapon silenced
		*/
		LogWeaponFire(event);
	}
	break;
	case StrHash::Hash("bullet_impact"): //0
	{
		/*
		bullet_impact
		Note: Every time a bullet hits something

		Name:	bullet_impact
		Structure:	
		short	userid	
		float	x	
		float	y	
		float	z	
		*/
		LogBulletImpact(event);
	}
	break;
	case StrHash::Hash("player_hurt"): //1
	{
		/*
		player_hurt
		Name:	player_hurt
		Structure:	
		short	userid	user ID of who was hurt
		short	attacker	user ID of who attacked
		byte	health	remaining health points
		byte	armor	remaining armor points
		string	weapon	weapon name attacker used, if not the world
		short	dmg_health	damage done to health
		byte	dmg_armor	damage done to armor
		byte	hitgroup	hitgroup that was damaged
		*/
		LogPlayerHurt(event);
	}
	break;
	default:
		return;
	}	
}

void Resolver::Resolve()
{
	if (!G::LocalPlayerAlive) return;

	L::Verbose("Resolver::Resolve - begin");
	Entity* ent;
	for (int i = 1; i < I::engine->GetMaxClients(); ++i)
	{
		L::Verbose("Resolver::Resolve - ent ", "");  L::Verbose(std::to_string(i).c_str());
		if (!(ent = I::entitylist->GetClientEntity(i))
			|| ent == G::LocalPlayer
			|| ent->GetTeam() == G::LocalPlayerTeam
			|| ent->IsDormant()
			|| ent->GetHealth() <= 0
		)
			continue;

		L::Verbose("Resolver::Resolve - BF");
		BruteForce(ent, i);
		L::Verbose("Resolver::Resolve - AF");
		AnimationFix(ent);
	}
	L::Verbose("Resolver::Resolve - done");
}

void Resolver::AnimationFix(Entity* entity)
{
	AnimState* anim = entity->GetAnimstate();
	if (!anim)
		return;

	bool* ClientAnims = entity->ClientAnimations();
	if (!ClientAnims)
		return;

	float m_flRealtime = I::globalvars->m_realTime;
	float m_flCurtime = I::globalvars->m_curTime;
	float m_flFrametime = I::globalvars->m_frameTime;
	float m_flAbsFrametime = I::globalvars->m_absFrameTime;
	float m_flInterpolation = I::globalvars->m_interpAmount;
	float m_nFrames = I::globalvars->m_frameCount;
	float m_nTicks = I::globalvars->m_tickCount;

	int m_iNextSimulationTick = G::LocalPlayer->GetSimulationTime() / I::globalvars->m_intervalPerTick + 1;

	I::globalvars->m_realTime = G::LocalPlayer->GetSimulationTime();
	I::globalvars->m_curTime = G::LocalPlayer->GetSimulationTime();
	I::globalvars->m_frameTime = I::globalvars->m_intervalPerTick;
	I::globalvars->m_absFrameTime = I::globalvars->m_intervalPerTick;
	I::globalvars->m_frameCount = m_iNextSimulationTick;
	I::globalvars->m_tickCount = m_iNextSimulationTick;
	I::globalvars->m_interpAmount = 0.f;


	if (anim->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
		anim->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;


	*ClientAnims = true;
	entity->UpdateClientSideAnimation();


	I::globalvars->m_realTime = m_flRealtime;
	I::globalvars->m_curTime = m_flCurtime;
	I::globalvars->m_frameTime = m_flFrametime;
	I::globalvars->m_absFrameTime = m_flAbsFrametime;
	I::globalvars->m_interpAmount = m_flInterpolation;
	I::globalvars->m_frameTime = m_nFrames;
	I::globalvars->m_tickCount = m_nTicks;

	I::globalvars->m_realTime = G::LocalPlayer->GetSimulationTime();
	I::globalvars->m_curTime = G::LocalPlayer->GetSimulationTime();
	I::globalvars->m_frameTime = I::globalvars->m_intervalPerTick;
	I::globalvars->m_absFrameTime = I::globalvars->m_intervalPerTick;
	I::globalvars->m_frameCount = m_iNextSimulationTick;
	I::globalvars->m_tickCount = m_iNextSimulationTick;
	I::globalvars->m_interpAmount = 0.f;



	if (anim->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
		anim->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;

	*ClientAnims = true;
	entity->UpdateClientSideAnimation();



	I::globalvars->m_realTime = m_flRealtime;
	I::globalvars->m_curTime = m_flCurtime;
	I::globalvars->m_frameTime = m_flFrametime;
	I::globalvars->m_absFrameTime = m_flAbsFrametime;
	I::globalvars->m_interpAmount = m_flInterpolation;
	I::globalvars->m_frameCount = m_nFrames;
	I::globalvars->m_tickCount = m_nTicks;
}

void Resolver::BruteForce(Entity* entity, int index)
{
	ResolverFlag[index] = "";
	AnimState* animstate = entity->GetAnimstate();
	if (!animstate)
		return;

	L::Verbose("Resolver::BruteForce - GetLBY");
	float lby = entity->GetLBY();

	// If we can't get stuff, something is wrong with player,
	// should have been fix by now but still adding the check
	L::Verbose("Resolver::BruteForce - GetPlayerInfo");
	player_info_t info;
	if (!I::engine->GetPlayerInfo(index, &info))
		return;

	L::Verbose("Resolver::BruteForce - Priority");
	int UserID = info.userid;
	Priority[UserID] = 0;

	L::Verbose("Resolver::BruteForce - OnShot");
	// Deal with OnShot (ish)
	if (I::globalvars->m_curTime - entity->GetLastShotTime() <= I::globalvars->m_intervalPerTick)
	{
		Priority[UserID] = 4; // always try for os for now
		entity->PGetEyeAngles()->y = entity->GetLBY();
		entity->GetAnimstate()->m_flAbsRotation() = entity->GetLBY();
		ResolverFlag[index] = "Onshot (ish)";
		return;
	}

	L::Verbose("Resolver::BruteForce - FL_ONGROUND");
	// If they aint on the ground, then their head is gonna be where the lby is
	if (!(entity->GetFlags() & FL_ONGROUND))
	{
		Priority[UserID] = 3;
		entity->PGetEyeAngles()->y = entity->GetLBY();
		entity->GetAnimstate()->m_flAbsRotation() = entity->GetLBY();
		ResolverFlag[index] = "Off Ground";
		return;
	}

	L::Verbose("Resolver::BruteForce - Velocity");
	float Velocity = entity->GetVecVelocity().VecLength2D();
	float MAS = entity->MaxAccurateSpeed() / 3.f; // max accurate speed
	// If they are on the ground, and are going faster than max accurate speed...
	if (Velocity > MAS)
	{
		Priority[UserID] = 2;
		entity->PGetEyeAngles()->y = entity->GetLBY();
		entity->GetAnimstate()->m_flAbsRotation() = entity->GetLBY();
		ResolverFlag[index] = "Faster than Max Accurate Speed";
		return;
	}

	L::Verbose("Resolver::BruteForce - Slow-walking");
	// If they are not standing still, but are slow-walking...
	if (Velocity < MAS && Velocity > 5.f)
	{
		Priority[UserID] = 1;
		ResolverFlag[index] = "Slow-walking: ";
		switch (ShotsMissed[UserID] % 5) {
		case 0:
			// do fucking nothing
			ResolverFlag[index] += "0";
			break;
		case 1:
			entity->PGetEyeAngles()->y = lby + -35.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + -35.f;
			ResolverFlag[index] += "-35";
			break;
		case 2:
			entity->PGetEyeAngles()->y = lby + 35.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + 35.f;
			ResolverFlag[index] += "35";
			break;
		case 3:
			entity->PGetEyeAngles()->y = lby + -12.5f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + -12.5f;
			ResolverFlag[index] += "-12.5";
			break;
		case 4:
			entity->PGetEyeAngles()->y = lby + 12.5f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + 12.5f;
			ResolverFlag[index] += "12.5";
			break;
		}
		return;
	}

	L::Verbose("Resolver::BruteForce - standing still");
	// If they are standing still...
	if (Velocity < 5.f)
	{
		ResolverFlag[index] = "Standing: ";
		switch (ShotsMissed[UserID] % 7) {
		case 0:
			// do fucking nothing
			ResolverFlag[index] += "0";
			break;
		case 1:
			ResolverFlag[index] += "20";
			entity->PGetEyeAngles()->y = lby + 20.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + 20.f;
			break;
		case 2:
			ResolverFlag[index] += "-20";
			entity->PGetEyeAngles()->y = lby + -20.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + -20.f;
			break;
		case 3:
			ResolverFlag[index] += "35";
			entity->PGetEyeAngles()->y = lby + 40.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + 40.f;
			break;
		case 4:
			ResolverFlag[index] += "-35";
			entity->PGetEyeAngles()->y = lby + -40.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + -40.f;
			break;
		case 5:
			ResolverFlag[index] += "65";
			entity->PGetEyeAngles()->y = lby + 65.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + 65.f;
			break;
		case 6:
			ResolverFlag[index] += "-65";
			entity->PGetEyeAngles()->y = lby + -65.f;
			entity->GetAnimstate()->m_flAbsRotation() = lby + -65.f;
			break;
		}
		return;
	}



}
