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
	{
		ImpactEndUserID = info.userid;
		ImpactHitgroup = tr.Hitgroup;
	}
	else {
		ImpactEndUserID = -1;
		ImpactHitgroup = -1;
	}


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
	int hitgroup = event->GetInt("hitgroup");

	// if the localplayer gets hurt, return
	if (I::engine->GetPlayerForUserID(UserID) == G::LocalPlayerIndex)
		return;

	// if the localplayer isn't shootin, return
	if (I::engine->GetPlayerForUserID(iAttacker) != G::LocalPlayerIndex)
		return;

	player_info_t info;
	I::engine->GetPlayerInfo(I::engine->GetPlayerForUserID(UserID), &info);

	// if they are the same indexes + same hitgroup --> HIT!
	if (ImpactEndUserID == UserID && ImpactHitgroup == hitgroup)
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
	static Config2::CState* RageEnable = Config2::GetState("rage-aim-enable");
	static Config2::CState* LegitEnable = Config2::GetState("legit-aim-enable");

	if (!G::LocalPlayerAlive) return;
	if (!RageEnable->Get() && !LegitEnable->Get()) return;

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

		//L::Verbose("Resolver::Resolve - BF");
		//BruteForce(ent, i);

		L::Verbose("Resolver::Resolve - BF");
		//Example(ent, i);
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
	L::Verbose("Resolver::BruteForce - begin");
	ResolverFlag[index] = "";

	L::Verbose("Resolver::BruteForce - GetPlayerInfo");
	player_info_t info;
	if (!I::engine->GetPlayerInfo(index, &info))
		return;
	int UserID = info.userid;

	L::Verbose("Resolver::BruteForce - GetAnimstate");
	AnimState* animstate = entity->GetAnimstate();
	if (!animstate)
		return;

	L::Verbose("Resolver::BruteForce - PGetEyeAngles");
	Vec* EyeAngles = entity->PGetEyeAngles();
	L::Verbose("Resolver::BruteForce - GetLBY");
	float lby = entity->GetLBY();
	L::Verbose("Resolver::BruteForce - GetVecVelocity");
	float velocity = entity->GetVecVelocity().VecLength2D();
	L::Verbose("Resolver::BruteForce - MaxAccurateSpeed");
	float maxAccurateSpeed = entity->MaxAccurateSpeed() / 3.f;
	L::Verbose("Resolver::BruteForce - EyeAngles->y");
	//EyeAngles->y = lby;
	//L::Verbose("Resolver::BruteForce - animstate->goal_feet_yaw");
	//animstate->m_flGoalFeetYaw = lby;

	if (I::globalvars->m_curTime - entity->GetLastShotTime() <= I::globalvars->m_intervalPerTick) // if they shot within the last tick
	{
		Priority[UserID] = 4;
		ResolverFlag[index] = "Onshot";
	}
	else if (!(entity->GetFlags() & FL_ONGROUND)) 
	{
		Priority[UserID] = 3;
		ResolverFlag[index] = "Off Ground";
	}
	else if (velocity > maxAccurateSpeed)
	{
		Priority[UserID] = 2;
		ResolverFlag[index] = "Inaccurate Aim";
	}
	else if (velocity < maxAccurateSpeed && velocity >= 3.f)
	{
		Priority[UserID] = 1;
		ResolverFlag[index] = "Resolved Slow-Walk: ";
		switch (ShotsMissed[UserID] % 5) {
		case 0:
			ResolverFlag[index] += "0";
			break;
		case 1:
			EyeAngles->y += -35.f;
			animstate->m_flGoalFeetYaw += -35.f;
			ResolverFlag[index] += "-35";
			break;
		case 2:
			EyeAngles->y += 35.f;
			animstate->m_flGoalFeetYaw += 35.f;
			ResolverFlag[index] += "35";
			break;
		case 3:
			EyeAngles->y = lby + -12.5f;
			animstate->m_flGoalFeetYaw += -12.5f;
			ResolverFlag[index] += "-12.5";
			break;
		case 4:
			EyeAngles->y = lby + 12.5f;
			animstate->m_flGoalFeetYaw += 12.5f;
			ResolverFlag[index] += "12.5";
			break;
		}
	}
	else // not shooting, on ground, not moving
	{
		ResolverFlag[index] = "Resolved Stand-Still: ";
		switch (ShotsMissed[UserID] % 7) {
		case 0:
			// do fucking nothing
			ResolverFlag[index] += "0";
			break;
		case 1:
			ResolverFlag[index] += "20";
			EyeAngles->y += 20.f;
			animstate->m_flGoalFeetYaw += 20.f;
			break;
		case 2:
			ResolverFlag[index] += "-20";
			EyeAngles->y = lby + -20.f;
			animstate->m_flGoalFeetYaw += -20.f;
			break;
		case 3:
			ResolverFlag[index] += "35";
			EyeAngles->y = lby + 40.f;
			animstate->m_flGoalFeetYaw += 40.f;
			break;
		case 4:
			ResolverFlag[index] += "-35";
			EyeAngles->y = lby + -40.f;
			animstate->m_flGoalFeetYaw += -40.f;
			break;
		case 5:
			ResolverFlag[index] += "65";
			EyeAngles->y += 65.f;
			animstate->m_flGoalFeetYaw += 65.f;
			break;
		case 6:
			ResolverFlag[index] += "-65";
			EyeAngles->y = lby + -65.f;
			animstate->m_flGoalFeetYaw += -65.f;
			break;
		}
	}
}

void Resolver::PreResolver(int stage)
{
	//FRAME_NET_UPDATE_POSTDATAUPDATE_START - 2
	//FRAME_RENDER_START - 5

	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	static Config2::CState* RageEnable = Config2::GetState("rage-aim-enable");
	static Config2::CState* LegitEnable = Config2::GetState("legit-aim-enable");

	if (!G::LocalPlayerAlive) return;
	if (!RageEnable->Get() && !LegitEnable->Get()) return;

	L::Verbose("Resolver::PreResolver - begin");
	Entity* ent;
	for (int i = 1; i < I::engine->GetMaxClients(); ++i)
	{
		L::Verbose("Resolver::PreResolver - ent ", "");  L::Verbose(std::to_string(i).c_str());
		if (!(ent = I::entitylist->GetClientEntity(i))
			|| ent == G::LocalPlayer
			|| ent->GetTeam() == G::LocalPlayerTeam
			|| ent->IsDormant()
			|| ent->GetHealth() <= 0
			)
			continue;

		AnimationFix(ent);

		player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info))
			return;
		int UserID = info.userid;

		L::Verbose("Resolver::PreResolver - GetAnimstate");
		AnimState* animstate = ent->GetAnimstate();
		if (!animstate)
			return;

		float yaw = animstate->m_flGoalFeetYaw;

		// If it isn't found in OrigYaw, ADD IT!
		// Backing up old yaw
		if (OrigYaw.find(UserID) == OrigYaw.end())
			OrigYaw.insert(std::pair(UserID, yaw));
		else
			OrigYaw[UserID] = yaw;


		// Resolving
		int missedshots = ShotsMissed[UserID];
		ResolverFlag[i] = std::to_string(missedshots) + "|";

		float EyeDelta = ent->GetEyeAngles().y - animstate->m_flGoalFeetYaw;
		bool LowDelta = EyeDelta <= 30.f;
		int Side = (EyeDelta > 0.f) ? -1 : 1;
		float desync_delta = LowDelta ? ent->GetMaxDesyncAngle() / 2 : ent->GetMaxDesyncAngle();

		switch (missedshots % 3)
		{
		case 0: 
			ResolverFlag[i] += std::to_string(desync_delta * Side);
			animstate->m_flGoalFeetYaw = ent->GetEyeAngles().y + desync_delta * Side; 
			break;
		case 1: 
			ResolverFlag[i] += std::to_string(desync_delta * -Side);
			animstate->m_flGoalFeetYaw = ent->GetEyeAngles().y + desync_delta * -Side; 
			break;
		case 2: 
			ResolverFlag[i] += "0";
			break;// animstate->m_flGoalFeetYaw = 0;
		}

	}
	L::Verbose("Resolver::Resolve - done");
}

void Resolver::PostResolver(int stage)
{
	//FRAME_RENDER_START
	
	if (stage != FRAME_NET_UPDATE_START)
		return;

	static Config2::CState* RageEnable = Config2::GetState("rage-aim-enable");
	static Config2::CState* LegitEnable = Config2::GetState("legit-aim-enable");

	if (!G::LocalPlayerAlive) return;
	if (!RageEnable->Get() && !LegitEnable->Get()) return;

	L::Verbose("Resolver::PostResolver - begin");
	Entity* ent;
	for (int i = 1; i < I::engine->GetMaxClients(); ++i)
	{
		L::Verbose("Resolver::PostResolver - ent ", "");  L::Verbose(std::to_string(i).c_str());
		if (!(ent = I::entitylist->GetClientEntity(i))
			|| ent == G::LocalPlayer
			|| ent->GetTeam() == G::LocalPlayerTeam
			|| ent->IsDormant()
			|| ent->GetHealth() <= 0
			)
			continue;

		player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info))
			return;
		int UserID = info.userid;

		L::Verbose("Resolver::PostResolver - GetAnimstate");
		AnimState* animstate = ent->GetAnimstate();
		if (!animstate)
			return;

		// If it isn't found in OrigYaw, return
		if (OrigYaw.find(UserID) == OrigYaw.end())
			return;
		else
			animstate->m_flGoalFeetYaw = OrigYaw[UserID];
	}
	L::Verbose("Resolver::Resolve - done");
}

