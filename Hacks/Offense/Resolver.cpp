

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
			// GOOD shot
			ConsoleColorMsg(Color(0, 255, 0), "Shot ");
			ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
			ConsoleColorMsg(Color(255, 255, 255), " for ");
			ConsoleColorMsg(Color(255, 0, 0), "[%d]", dmg);
			ConsoleColorMsg(Color(255, 255, 255), " in the ");
			ConsoleColorMsg(Color(255, 69, 0), "[%s]\n", HitGroupStr(hitgroup).c_str());
			PlayerInfo[ImpactEndUserID].ShotsMissed--;
		}	
	}
	else if(LogEnable->Get())
	{
		// LUCKY shot (wasn't going t hit but did)
		PlayerInfo[ImpactEndUserID].ShotsMissed++;
		ConsoleColorMsg(Color(255, 0, 0), "Shot ");
		ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
		ConsoleColorMsg(Color(255, 255, 255), " for ");
		ConsoleColorMsg(Color(255, 0, 0), "[%d]", dmg);
		ConsoleColorMsg(Color(255, 255, 255), " in the ");
		ConsoleColorMsg(Color(255, 69, 0), "[%s]\n", HitGroupStr(hitgroup).c_str());

		LogShot = false; // dont log missed to spread (cuz luckily hit entity)
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
	static Config::CState* LogEnable = Config::GetState("misc-other-logs");

	if (!G::LocalPlayerAlive) return;

	static int tick_count = I::globalvars->m_tickCount;
	if (I::globalvars->m_tickCount == tick_count)
		return;
	tick_count = I::globalvars->m_tickCount;

	L::Verbose("Resolver::PreResolver - begin");
	Entity* ent;

	if (LogShot && LogEnable->Get() && !BacktrackShot)
	{
		player_info_t info;
		if (!I::engine->GetPlayerInfo(UserID, &info))
		{
			ConsoleColorMsg(Color(255, 255, 255), "Missed shot due to");
			ConsoleColorMsg(Color(255, 0, 0), " spread\n");
		}
		else
		{
			ConsoleColorMsg(Color(255, 255, 255), "Missed shot on");
			ConsoleColorMsg(Color(255, 0, 0), "[%s]", info.name);
			ConsoleColorMsg(Color(255, 255, 255), "  due to ");
			ConsoleColorMsg(Color(255, 0, 0), " spread\n");
		}
	}
	else if(LogEnable->Get() && BacktrackShot)
	{
		ConsoleColorMsg(Color(123, 123, 123), "Backtrack Shot --> Unknown");
		BacktrackShot = false;
	}
	LogShot = false;

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


		// If it's different and we haven't logged the shot
		if (LogEnable->Get() && PlayerInfo[UserID].ShotsMissed != PlayerInfo[UserID].OldShotsMissed && !PlayerInfo[UserID].LogShot)
		{
			PlayerInfo[UserID].LogShot = true;
			ConsoleColorMsg(Color(255, 69, 0), "Missed shot at ");
			ConsoleColorMsg(Color(0, 255, 0), "[%s]", info.name);
			ConsoleColorMsg(Color(255, 69, 0), " due to prediction error\n");
		}

		ResolveEnt(ent, i);

	}
	L::Verbose("Resolver::Resolve - done");
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

	// If Desyncing... RESOLVE!
	if (record.IsDesyncing)
	{
		PlayerInfo[UserID].ResolverFlag = std::to_string(PlayerInfo[UserID].ShotsMissed) + "|";

		// Applies pitch if shooting (I think)
		FindShot(entity, UserID);
		record.PrevAng = entity->GetEyeAngles();

		// Get Side of desync
		int side = 0;
		DetectSide(entity, &side);
		
		// Get Animstate
		AnimState* animstate = entity->GetAnimstate();
		if (!animstate) return;

		// Get EyeYaw
		float EyeYaw = animstate->m_flEyeYaw;

		// Get MaxDesyncDelta
		float MaxDesyncAng = entity->GetMaxDesyncAngle();

		// Check if extending
		AnimationLayer* layer = entity->GetAnimOverlay(3);
		bool m_extending = false;
		if (layer)
			m_extending = layer->m_flCycle == 0.f && layer->m_flWeight == 0.f;

		// set up brute ang
		static float brute = 0.f;

		// if they are roughly facing forward...
		bool forward = fabsf(NormalizeYaw(NormalizeYaw(entity->GetEyeAngles().y) - NormalizeYaw(GetBackwardYaw(entity) + 180))) < 90.f;

		// resolve shooting players separately.
		if (record.Shot)
		{
			PlayerInfo[UserID].ResolverFlag += "Shot!";
			float flPseudoFireYaw = NormalizeYaw(aimbot->CalculateAngle(entity->GetVecOrigin(), G::LocalPlayer->GetVecOrigin()).y);

			if (m_extending) {
				float flLeftFireYawDelta = fabsf(NormalizeYaw(flPseudoFireYaw - (entity->GetEyeAngles().y + 58.f)));
				float flRightFireYawDelta = fabsf(NormalizeYaw(flPseudoFireYaw - (entity->GetEyeAngles().y - 58.f)));

				//g_notify.add( tfm::format( XOR( "found shot record on %s: [ yaw: %i ]" ), game::GetPlayerName( record->m_player->index( ) ), int( flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f ) ) );

				brute = flLeftFireYawDelta > flRightFireYawDelta ? -58.f : 58.f;
			}
			else {
				float flLeftFireYawDelta = fabsf(NormalizeYaw(flPseudoFireYaw - (entity->GetEyeAngles().y + 29.f)));
				float flRightFireYawDelta = fabsf(NormalizeYaw(flPseudoFireYaw - (entity->GetEyeAngles().y - 29.f)));

				//g_notify.add( tfm::format( XOR( "found shot record on %s: [ yaw: %i ]" ), game::GetPlayerName( record->m_player->index( ) ), int( flLeftFireYawDelta > flRightFireYawDelta ? -29.f : 29.f ) ) );

				brute = flLeftFireYawDelta > flRightFireYawDelta ? -29.f : 29.f;
			}
		}
		// Otherwise BRUTEFORCE
		else
		{
			float resolve_yaw = MaxDesyncAng;
			// Because we know they are choking packets --> assume desync
			switch (record.ShotsMissed % 3) {
			case 0:
				PlayerInfo[UserID].ResolverFlag += "LEFT";
				brute = resolve_yaw * (forward ? side : -side);
				break;
			case 1:
				PlayerInfo[UserID].ResolverFlag += "RIGHT";
				brute = resolve_yaw * (forward ? -side : side);
				break;
			case 2:
				PlayerInfo[UserID].ResolverFlag += "0";
				brute = 0;
				break;
			}

			animstate->m_flGoalFeetYaw = EyeYaw + brute;

			// normalize the eye angles, doesn't really matter but its clean.
			animstate->m_flGoalFeetYaw = NormalizeYaw(animstate->m_flGoalFeetYaw);
			//entity->PGetEyeAngles()->y = NormalizeYaw(entity->PGetEyeAngles()->y);
		}
	}
	else
	{
		PlayerInfo[UserID].ResolverFlag = "Not Desyncing";
	}
}

void Resolver::FindShot(Entity* entity, int UserID)
{
	auto& record = PlayerInfo[UserID];
	Entity* pWeapon = entity->GetActiveWeapon();

	float shoot_time = -1.f;
	if (pWeapon) {
		// with logging this time was always one tick behind.
		// so add one tick to the last shoot time.
		shoot_time = pWeapon->GetLastShotTime() + I::globalvars->m_intervalPerTick;
	}

	// this record has a shot on it.
	if (TimeToTicks(shoot_time) == TimeToTicks(entity->GetSimulationTime())) {
		if (record.LagTime <= 2)
			record.Shot = true;

		// more then 1 choke, cant hit pitch, apply prev pitch.
		else
			entity->PGetEyeAngles()->x = record.PrevAng.x;
	}
	else
		record.Shot = false;
}

float Resolver::GetBackwardYaw(Entity* entity)
{
	return aimbot->CalculateAngle(G::LocalPlayer->GetVecOrigin(), entity->GetVecOrigin()).y;
}

void Resolver::DetectSide(Entity* entity, int* side)
{
	Vec src3D, dst3D, forward, right, up, src, dst;
	float back_two, right_two, left_two;
	trace_t tr;
	CTraceFilter filter(entity);

	AngleVectors(QAngle(0, GetBackwardYaw(entity), 0), &forward, &right, &up);

	src3D = entity->GetEyePos();
	dst3D = src3D + (forward * 384); //Might want to experiment with other numbers, incase you don't know what the number does, its how far the trace will go. Lower = shorter.

	I::enginetrace->TraceRay(Ray_t(src3D, dst3D), MASK_SHOT, &filter, &tr);
	back_two = (tr.Endpos - tr.Startpos).VecLength();

	I::enginetrace->TraceRay(Ray_t(src3D + right * 35, dst3D + right * 35), MASK_SHOT, &filter, &tr);
	right_two = (tr.Endpos - tr.Startpos).VecLength();

	I::enginetrace->TraceRay(Ray_t(src3D - right * 35, dst3D - right * 35), MASK_SHOT, &filter, &tr);
	left_two = (tr.Endpos - tr.Startpos).VecLength();

	if (left_two > right_two) {
		*side = -1;
	}
	else if (right_two > left_two) {
		*side = 1;
	}
	else
		*side = 0;
}
