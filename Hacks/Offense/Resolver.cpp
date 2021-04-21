

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
		if (PlayerInfo.find(ImpactEndUserID) != PlayerInfo.end())
			PlayerInfo[ImpactEndUserID].ShotsMissed--;
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
	static Config2::CState* RageEnable = Config2::GetState("rage-aim-enable");
	static Config2::CState* LegitEnable = Config2::GetState("legit-aim-enable");

	if (!G::LocalPlayerAlive) return;
	if (!RageEnable->Get() && !LegitEnable->Get()) return;

	static int tick_count = I::globalvars->m_tickCount;
	if (I::globalvars->m_tickCount == tick_count)
		return;
	tick_count = I::globalvars->m_tickCount;

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
			continue;

		int UserID = info.userid;

		// If it aint in there... add it
		if (PlayerInfo.find(UserID) == PlayerInfo.end())
		{
			PlayerRes NewPlayer;
			PlayerInfo.insert(std::pair(UserID, NewPlayer));
		}

		ResolveEnt(ent, i);

	}
	L::Verbose("Resolver::Resolve - done");
}

void Resolver::ResolveEnt(Entity* entity, int Index)
{
	player_info_t info;
	if (!I::engine->GetPlayerInfo(Index, &info))
		return;
	int UserID = info.userid;
	auto& record = PlayerInfo[UserID];

	if (entity->GetSimulationTime() != record.OldSimTime)
	{
		if (entity->GetSimulationTime() - record.OldSimTime > I::globalvars->m_intervalPerTick) //Desync check
			record.IsDesyncing = true;
		else
			record.IsDesyncing = false;
		record.OldSimTime = entity->GetSimulationTime();
	}

	// If Desyncing... RESOLVE!
	if (record.IsDesyncing)
	{
		PlayerInfo[UserID].ResolverFlag = std::to_string(PlayerInfo[UserID].ShotsMissed) + "|";

		AnimState* animstate = entity->GetAnimstate();
		if (!animstate) return;

		float EyeDelta = entity->GetEyeAngles().y - entity->GetLBY();
		bool LowDelta = EyeDelta <= 30.f;
		int Side = (EyeDelta > 0.f) ? -1 : 1;
		float desync_delta = LowDelta ? entity->GetMaxDesyncAngle() / 2 : entity->GetMaxDesyncAngle();

		// The order is assuming everyone has low delta
		switch (PlayerInfo[UserID].ShotsMissed % 5)
		{
		case 0:
			PlayerInfo[UserID].ResolverFlag += "0";
			break;
		case 1:
			PlayerInfo[UserID].ResolverFlag += "+Side/2";
			animstate->m_flGoalFeetYaw = entity->GetEyeAngles().y + desync_delta * Side * 0.5;
			break;
		case 2:
			PlayerInfo[UserID].ResolverFlag += "-Side/2";
			animstate->m_flGoalFeetYaw = entity->GetEyeAngles().y + desync_delta * -Side * 0.5;
			break;
		case 3:
			PlayerInfo[UserID].ResolverFlag += "+Side";
			animstate->m_flGoalFeetYaw = entity->GetEyeAngles().y + desync_delta * Side;
			break;
		case 4:
			PlayerInfo[UserID].ResolverFlag += "-Side";
			animstate->m_flGoalFeetYaw = entity->GetEyeAngles().y + desync_delta * -Side;
			break;
		}

	}
	else
	{
		PlayerInfo[UserID].ResolverFlag = "Not Desyncing";
	}

}

void Resolver::UpdateEnt(Entity* entity, int UserID)
{
	auto& record = PlayerInfo[UserID];
	Entity* pWeapon = entity->GetActiveWeapon();

	//Hitting OnSHOT
	if (pWeapon)
	{
		if (record.ShotTime != pWeapon->GetLastShotTime())
		{
			record.Shot = true;
			record.ShotTime = pWeapon->GetLastShotTime();
		}
		else
			record.Shot = false;
	}
	else
	{
		record.Shot = false;
		record.ShotTime = 0.f;
	}


}
