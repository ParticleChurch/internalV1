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

void Resolver::LogShots(GameEvent* event)
{
	static int ImpactEntIndex;
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

		int iUser = event->GetInt("userid");
		if (I::engine->GetPlayerForUserID(iUser) != G::LocalPlayerIndex)
			return;

		for (int i = 0; i < 64; i++)
			OldShotsMissed[i] = ShotsMissed[i];
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
		int iUser = event->GetInt("userid");

		if (I::engine->GetPlayerForUserID(iUser) != G::LocalPlayerIndex)
			return;

		Vec Loc = Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

		trace_t tr;
		Ray_t ray(G::LocalPlayer->GetEyePos(), Loc);
		CTraceFilter traceFilter(G::LocalPlayer);
		I::enginetrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);
		Entity* ShouldHitEntity = tr.Entity;

		// No point in doing anything if we shouldn't have hit anything for now
		if (!ShouldHitEntity)
		{
			return;
		}
			

		int index = ShouldHitEntity->Index();

		// Not player entity
		if (index > 64)
			return;

		// Keep track of the entity we supossedly missed due to resolver
		ImpactEntIndex = index;

		// No need to increase if already increased once...
		if (ShotsMissed[index] > OldShotsMissed[index])
			return;

		// Log the shot up by 1
		ShotsMissed[index]++;
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
		int iUser = event->GetInt("userid");
		int iAttacker = event->GetInt("attacker");

		if (I::engine->GetPlayerForUserID(iUser) == G::LocalPlayerIndex)
			return;

		if (I::engine->GetPlayerForUserID(iAttacker) != G::LocalPlayerIndex)
			return;

		// if they are the same indexes --> HIT!
		if (ImpactEntIndex == I::engine->GetPlayerForUserID(iUser))
		{
			// No shots were missed, revert back to normal
			ShotsMissed[ImpactEntIndex]--;
				
		}
	}
	break;
	default:
		return;
	}	
}

void Resolver::Resolve(int stage)
{
	if (!I::engine->IsInGame()) return;

	Entity* localplayer = (Entity*)I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!localplayer) return;

	if (!(localplayer->GetHealth() > 0)) return;

	int team = localplayer->GetTeam();

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < I::engine->GetMaxClients(); ++i)
		{
			Entity* player = (Entity*)I::entitylist->GetClientEntity(i);

			if (!player
				|| player == localplayer
				|| player->IsDormant()
				|| !(player->GetHealth() > 0)
				|| team == player->GetTeam())
				continue;

			/*AnimationFix(player);*/
			BruteForce(player, i);
			AnimationFix(player);
		}
	}
}

void Resolver::AnimationFix(Entity* entity)
{
	AnimState* anim = entity->GetAnimstate();
	if (!anim)
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

	entity->ClientAnimations() = true;
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

	entity->ClientAnimations() = true;
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
	// Deal with OnShot (ish)
	AnimState* animstate = entity->GetAnimstate();
	if (!animstate)
		return;

	float lby = entity->GetLBY();

	if (I::globalvars->m_curTime - entity->GetLastShotTime() < I::globalvars->m_intervalPerTick)
	{
		entity->PGetEyeAngles()->y = entity->GetLBY();
		ResolverFlag[index] = "Onshot (ish)";
		return;
	}
	
	// If they aint on the ground, then their head is gonna be where the lby is
	if (!(entity->GetFlags() & FL_ONGROUND))
	{
		entity->PGetEyeAngles()->y = entity->GetLBY();
		ResolverFlag[index] = "Off Ground";
		return;
	}

	float Velocity = entity->GetVecVelocity().VecLength2D();
	float MAS = entity->MaxAccurateSpeed() / 3.f; // max accurate speed
	// If they are on the ground, and are going faster than max accurate speed...
	if (Velocity > MAS)
	{
		entity->PGetEyeAngles()->y = entity->GetLBY();
		ResolverFlag[index] = "Faster than Max Accurate Speed";
		return;
	}

	// If they are not standing still, but are slow-walking...
	if (Velocity < MAS && Velocity > 5.f)
	{
		ResolverFlag[index] = "Slow-walking: ";
		switch (ShotsMissed[index] % 5) {
		case 0:
			// do fucking nothing
			ResolverFlag[index] += "0";
			break;
		case 1:
			entity->PGetEyeAngles()->y = lby + -35.f;
			ResolverFlag[index] += "-35";
			break;
		case 2:
			entity->PGetEyeAngles()->y = lby + 35.f;
			ResolverFlag[index] += "35";
			break;
		case 3:
			entity->PGetEyeAngles()->y = lby + -12.5f;
			ResolverFlag[index] += "-12.5";
			break;
		case 4:
			entity->PGetEyeAngles()->y = lby + 12.5f;
			ResolverFlag[index] += "12.5";
			break;
		}
		return;
	}

	// If they are standing still...
	if (Velocity < 5.f)
	{
		ResolverFlag[index] = "Standing: ";
		switch (ShotsMissed[index] % 7) {
		case 0:
			// do fucking nothing
			ResolverFlag[index] += "0";
			break;
		case 1:
			ResolverFlag[index] += "20";
			entity->PGetEyeAngles()->y = lby + 20.f;
			break;
		case 2:
			ResolverFlag[index] += "-20";
			entity->PGetEyeAngles()->y = lby + -20.f;
			break;
		case 3:
			ResolverFlag[index] += "35";
			entity->PGetEyeAngles()->y = lby + 40.f;
			break;
		case 4:
			ResolverFlag[index] += "-35";
			entity->PGetEyeAngles()->y = lby + -40.f;
			break;
		case 5:
			ResolverFlag[index] += "60";
			entity->PGetEyeAngles()->y = lby + 65.f;
			break;
		case 6:
			ResolverFlag[index] += "-60";
			entity->PGetEyeAngles()->y = lby + -65.f;
			break;
		}
		return;
	}

}
