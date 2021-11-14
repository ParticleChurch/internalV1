

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
	// If it's NOT the localplayer, return
	if (I::engine->GetPlayerForUserID(UserID) != G::LocalPlayerIndex)
		return;
	
	LogPredError = true;

	// If it isnt in the list... add it!
	if (PlayerInfo.find(UserID) == PlayerInfo.end())
	{
		// not found
		PlayerRes NewPlayer;
		PlayerInfo.insert(std::pair(UserID, NewPlayer));
		return;
	}

	// Otherwise it's already in the list, update everything!
	for (auto& a : PlayerInfo)
	{
		a.second.OldShotsMissed = a.second.ShotsMissed;
		a.second.LogShot = false;
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

	LogShot = false; // dont log missed to spread (cuz it should have hit entity)

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
	// If it isnt in the list... add it!
	if (PlayerInfo.find(UserID) == PlayerInfo.end())
	{
		// not found
		PlayerRes NewPlayer;
		NewPlayer.ShotsMissed = 1;
		PlayerInfo.insert(std::pair(UserID, NewPlayer));
		return;
	}

	// No need to increase if already increased once...
	if (PlayerInfo[info.userid].ShotsMissed > PlayerInfo[info.userid].OldShotsMissed)
		return;

	// Log the shot up by 1
	PlayerInfo[info.userid].ShotsMissed++;

}

void Resolver::LogPlayerHurt(GameEvent* event)
{
	static Config::CState* LogEnable = Config::GetState("misc-other-logs");
	int UserID = event->GetInt("userid");
	int iAttacker = event->GetInt("attacker");
	int hitgroup = event->GetInt("hitgroup");
	int dmg = event->GetInt("dmg_health");

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
		if (LogEnable->Get() && PlayerInfo.find(ImpactEndUserID) != PlayerInfo.end())
		{
			if (!BacktrackShot)
				PlayerInfo[ImpactEndUserID].ShotsMissed--;

			// GOOD shot
			ConsoleColorMsg(Color(0, 255, 0), "Shot ");
			ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
			ConsoleColorMsg(Color(255, 255, 255), " for ");
			ConsoleColorMsg(Color(255, 0, 0), "[%d]", dmg);
			ConsoleColorMsg(Color(255, 255, 255), " in the ");
			ConsoleColorMsg(Color(255, 69, 0), "[%s]", HitGroupStr(hitgroup).c_str());
			ConsoleColorMsg(Color(255, 255, 255), " at angle ");
			ConsoleColorMsg(Color(255, 255, 0), "[%d]", (int)PlayerInfo[info.userid].OffsetAngle);
			ConsoleColorMsg(Color(255, 255, 255), " | Shots Missed [%d]\n", (int)PlayerInfo[info.userid].ShotsMissed);
			

			

			LogPredError = false; // no prediction error cuz we hit them
			LogShot = false; // dont log anything more cuz we hit the shot
		}	
	}
	else if(LogEnable->Get() && !BacktrackShot)
	{
		// LUCKY shot (wasn't going t hit but did + not backtrack shot...)

		ConsoleColorMsg(Color(255, 69, 0), "Resolver Error: ");
		ConsoleColorMsg(Color(255, 0, 0), "Shot ");
		ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
		ConsoleColorMsg(Color(255, 255, 255), " for ");
		ConsoleColorMsg(Color(255, 0, 0), "[%d]", dmg);
		ConsoleColorMsg(Color(255, 255, 255), " in the ");
		ConsoleColorMsg(Color(255, 69, 0), "[%s]", HitGroupStr(hitgroup).c_str());
		ConsoleColorMsg(Color(255, 255, 255), " at angle ");
		ConsoleColorMsg(Color(255, 255, 0), "[%d]", (int)PlayerInfo[info.userid].OffsetAngle);
		ConsoleColorMsg(Color(255, 255, 255), " | Shots Missed [%d]\n", (int)PlayerInfo[info.userid].ShotsMissed);

		LogPredError = false;
		LogShot = false; // dont log missed to spread (cuz luckily hit entity)
	}
	else if (LogEnable->Get() && BacktrackShot)
	{
		// Backtrack shot - if it hits we assume resolver is good @ current state
		ConsoleColorMsg(Color(255, 192, 203), "Backtrack Shot ");
		ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
		ConsoleColorMsg(Color(255, 255, 255), " for ");
		ConsoleColorMsg(Color(255, 0, 0), "[%d]", dmg);
		ConsoleColorMsg(Color(255, 255, 255), " in the ");
		ConsoleColorMsg(Color(255, 69, 0), "[%s]", HitGroupStr(hitgroup).c_str());
		ConsoleColorMsg(Color(255, 255, 255), " at angle ");
		ConsoleColorMsg(Color(255, 255, 0), "[%d]", (int)PlayerInfo[info.userid].OffsetAngle);
		ConsoleColorMsg(Color(255, 255, 255), " | Shots Missed [%d]\n", (int)PlayerInfo[info.userid].ShotsMissed);
		//PlayerInfo[ImpactEndUserID].ShotsMissed--;

		LogPredError = false; // no prediction error cuz backtrack
		LogShot = false; // dont log anything more cuz we hit the shot
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

//FRAME_NET_UPDATE_POSTDATAUPDATE_START - 2
	//FRAME_RENDER_START - 5

void Resolver::Resolve()
{
	//
	static Config::CState* LogEnable = Config::GetState("misc-other-logs");

	if (!G::LocalPlayerAlive) return;

	static int tick_count = I::globalvars->m_tickCount;
	if (I::globalvars->m_tickCount == tick_count)
		return;
	tick_count = I::globalvars->m_tickCount;

	L::Debug("Resolver::PreResolver - begin");
	Entity* ent;

	for (int i = 1; i < I::engine->GetMaxClients(); ++i)
	{
		L::Debug("Resolver::PreResolver - ent ", "");  L::SameLine(std::to_string(i).c_str());
		if (!(ent = I::entitylist->GetClientEntity(i))
			|| ent == G::LocalPlayer
			|| ent->GetTeam() == G::LocalPlayerTeam
			|| ent->IsDormant()
			|| ent->GetHealth() <= 0
			)
			continue;

		player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info))
		{
			AnimationFix(ent);
			continue;
		}
			

		int UserID = info.userid;

		// If it aint in there... add it
		if (PlayerInfo.find(UserID) == PlayerInfo.end())
		{
			PlayerRes NewPlayer;
			PlayerInfo.insert(std::pair(UserID, NewPlayer));
		}


		// If it's different and we haven't logged the shot
		// and we haven't already logged prediction error...
		if (LogPredError && LogEnable->Get() && PlayerInfo[UserID].ShotsMissed != PlayerInfo[UserID].OldShotsMissed && !PlayerInfo[UserID].LogShot)
		{
			PlayerInfo[UserID].LogShot = false;
			ConsoleColorMsg(Color(255, 69, 0), "Resolver Error: ");
			ConsoleColorMsg(Color(255, 0, 0), "Missed shot at ");
			ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
			ConsoleColorMsg(Color(255, 0, 0), " at angle ");
			ConsoleColorMsg(Color(255, 255, 0), "[%d]", (int)PlayerInfo[info.userid].OffsetAngle);
			ConsoleColorMsg(Color(255, 255, 255), " | Shots Missed [%d]\n", (int)PlayerInfo[info.userid].ShotsMissed);
			LogPredError = false;
			LogShot = false; // dont wanna log spread error if it is a prediction error
		}

		ResolveEnt(ent, i);
		AnimationFix(ent);
		
	}

	// if we are aimboting, and have logs enabled...
	if (LogShot && LogEnable->Get() && !BacktrackShot)
	{
		player_info_t info;
		if (!I::engine->GetPlayerInfo(AimbotUserID, &info))
		{
			ConsoleColorMsg(Color(255, 255, 255), "Missed shot due to");
			ConsoleColorMsg(Color(255, 0, 0), " spread\n");
		}
		else
		{
			ConsoleColorMsg(Color(255, 255, 255), "Missed shot on");
			ConsoleColorMsg(Color(255, 0, 0), " [%s] ", info.name);
			ConsoleColorMsg(Color(255, 255, 255), "due to ");
			ConsoleColorMsg(Color(255, 0, 0), " spread");
			ConsoleColorMsg(Color(255, 0, 0), " at angle ");
			ConsoleColorMsg(Color(255, 255, 0), "[%d]", (int)PlayerInfo[info.userid].OffsetAngle);
			ConsoleColorMsg(Color(255, 255, 255), " | Shots Missed [%d]\n", (int)PlayerInfo[info.userid].ShotsMissed);
		}
	}
	else if(LogShot && LogEnable->Get() && BacktrackShot) // if we miss backtrack shot...
	{
		ConsoleColorMsg(Color(255, 255, 255), "Missed shot due to");
		ConsoleColorMsg(Color(0, 0, 255), " backtrack miscalculation\n");
	}
	LogShot = false;
	L::Debug("Resolver::Resolve - done");
}

void Resolver::ResolveEnt(Entity* entity, int Index)
{
	static Config::CState* ResolverEnabled = Config::GetState("resolver");

	// If resolver isn't enabled...
	if (!ResolverEnabled->Get()) return;

	player_info_t info;
	if (!I::engine->GetPlayerInfo(Index, &info))
		return;

	int UserID = info.userid;
	auto& record = PlayerInfo[UserID];

	record.LagTime = TimeToTicks(entity->GetSimulationTime() - record.OldSimTime);
	if (entity->GetSimulationTime() != record.OldSimTime)
	{
		if (entity->GetSimulationTime() - record.OldSimTime > I::globalvars->m_intervalPerTick) //Desync check
			record.IsDesyncing = true;
		else
			record.IsDesyncing = false;
		record.OldSimTime = entity->GetSimulationTime();
	}

	// Dont try to resolve if not desync lol
	/*if (!record.IsDesyncing)
		return;*/

	// Get Animstate
	AnimState* animstate = entity->GetAnimstate();
	if (!animstate) return;

	//animstate->m_flGoalFeetYaw = (entity->GetLBY() + entity->GetEyeAngles().y) / 2.f;

	float max_desync = entity->GetMaxDesyncAngle();

	record.ResolverFlag = std::to_string(record.ShotsMissed) + " | ";

	// we miss jitter resolve method hard so try straight back
	if (record.ResolveMethod == JITTER_METHOD && record.ShotsMissed - record.BeforeShotMissed > 2) {
		record.ResolveMethod = BACK_METHOD;
		record.BeforeShotMissed = record.ShotsMissed;
	}

	// we miss bruteforce method so try straight back
	if (record.ResolveMethod == BRUTE_FORCE && record.ShotsMissed - record.BeforeShotMissed > 6)
	{
		record.ResolveMethod = BACK_METHOD;
		record.BeforeShotMissed = record.ShotsMissed;
	}


	int side = 0;
	// if there is jitter to be had...
	if (DoesHaveJitter(entity, UserID, side))
	{
		if (record.ResolveMethod == BRUTE_FORCE && record.ShotsMissed - record.BeforeShotMissed > 6)
		{
			record.ResolveMethod = JITTER_METHOD;
			record.BeforeShotMissed = record.ShotsMissed;
		}
	}
		
	// we miss the back method as well --> RESET!
	if (record.ResolveMethod == BACK_METHOD
		&& record.ShotsMissed - record.BeforeShotMissed > 2) {
		record.ResolveMethod = BRUTE_FORCE;
		record.BeforeShotMissed = record.ShotsMissed;
	}


	if (record.ResolveMethod == BRUTE_FORCE)
	{
		switch (record.ShotsMissed % 7)
		{
		case 0:
			record.OffsetAngle = 0.f;
			break;
		case 1:
			record.OffsetAngle = max_desync;
			break;
		case 2:
			record.OffsetAngle = -max_desync;
			break;
		case 3:
			record.OffsetAngle = max_desync * 2 / 3;
			break;
		case 4:
			record.OffsetAngle = -max_desync * 2 / 3;
			break;
		case 5:
			record.OffsetAngle = max_desync * 1 / 3;
			break;
		case 6:
			record.OffsetAngle = -max_desync * 1 / 3;
			break;
		}
		animstate->m_flGoalFeetYaw += record.OffsetAngle;
		record.ResolverFlag += std::to_string(record.OffsetAngle);
		return;
	}
	else if (record.ResolveMethod == JITTER_METHOD)
	{
		record.ResolverFlag += "Jitter ";
		switch (record.ShotsMissed % 3)
		{
		case 0:
			record.OffsetAngle = 0.f;
			break;
		case 1:
			record.OffsetAngle = max_desync * side;
			break;
		case 2:
			record.OffsetAngle = max_desync * side;
			break;
		}
		animstate->m_flGoalFeetYaw += record.OffsetAngle;
		record.ResolverFlag += std::to_string(record.OffsetAngle);
		return;
	} 
	else if (record.ResolveMethod == BACK_METHOD)
	{
		record.ResolverFlag += "Backyaw ";
		float backyaw = GetBackwardsYaw(entity);
		switch (record.ShotsMissed % 3)
		{
		case 0:
			record.OffsetAngle = 0.f;
			break;
		case 1:
			record.OffsetAngle = max_desync;
			break;
		case 2:
			record.OffsetAngle = -max_desync;
			break;
		}
		animstate->m_flGoalFeetYaw = backyaw + record.OffsetAngle;
		record.ResolverFlag += std::to_string(record.OffsetAngle);
		return;
	}
}

bool Resolver::DoesHaveJitter(Entity* entity, int UserID, int &newside)
{
	float CurrentAngle = entity->GetEyeAngles().y;
	if (!IsNearEqual(CurrentAngle, PlayerInfo[UserID].LastAngle, 50.f)) {
		PlayerInfo[UserID].Switch = !PlayerInfo[UserID].Switch;
		PlayerInfo[UserID].LastAngle = CurrentAngle;
		newside = PlayerInfo[UserID].Switch ? 1 : -1;
		PlayerInfo[UserID].LastBrute = newside;
		PlayerInfo[UserID].LastUpdateTime = I::globalvars->m_curTime;
		return true;
	}
	else {
		if (fabsf(PlayerInfo[UserID].LastUpdateTime - I::globalvars->m_curTime >= TicksToTime(17))
			|| entity->GetSimulationTime() != entity->GetOldSimulationTime()) {
			PlayerInfo[UserID].LastAngle = CurrentAngle;
		}
		newside = PlayerInfo[UserID].LastBrute;
	}
	return false;
}

float Resolver::GetBackwardsYaw(Entity* entity)
{
	return aimbot->CalculateAngle(G::LocalPlayer->GetVecOrigin(), entity->GetVecOrigin()).y;
}

