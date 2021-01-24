#include "../../Include.hpp"

Resolver* resolver = new Resolver();
/*
ORDER GOES:
WEAPON_FIRE
BULLET_IMPACT
PLAYER_HURT

Logging on everything XD
*/

void Resolver::LogShots(GameEvent* event)
{
	static bool ShotEnd = false;
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

		ShotEnd = true;

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

		Loc = Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
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

		Hit[I::engine->GetPlayerForUserID(iUser)] = true;
	}
	break;
	default:
		return;
	}

	if (ShotEnd)
		ShotEnd = false;
	else
		return;

	trace_t tr;
	Ray_t ray(G::LocalPlayer->GetEyePos(), Loc);
	CTraceFilter traceFilter(G::LocalPlayer);
	I::enginetrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);
	Entity* ShouldHitEntity = tr.Entity;

	// No point in doing anything if we shouldn't have hit anything for now
	if (!ShouldHitEntity)
		return;

	int index = ShouldHitEntity->Index();
	if (index > 64)
		return;

	bool bad_resolve = false;
	// If we should have hit an enemy, and did--> Good Resolver
	if (Hit[index])
	{
		Hit[index] = false;
	}
	// If we should have hit an enemy, and didn't--> Bad Resolver
	else
	{
		ShotsMissed[index]++;
		bad_resolve = true;
	}

	if (!bad_resolve)
		return;
	
	// Log to some resolver console
}

void Resolver::Resolve(int stage)
{
	if (!I::engine->IsInGame())
		return;

	Entity* localplayer = (Entity*)I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!localplayer)
		return;

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

			AnimationFix(player);
			// we have decided to resolve!
			switch (ShotsMissed[i] % 3) {
			case 0:
				ABSROTATION(player);
				break;
			case 1:
				player->GetAnimstate()->m_flAbsRotation() = -60.f;
				break;
			case 2:
				player->GetAnimstate()->m_flAbsRotation() = 60.f;
				break;

			}
			AnimationFix(player);
		}
	}

	//BRUTEFORCE METHOD!
	/*
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START || stage == FRAME_RENDER_END)
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

			//player->GetEyeAngles()->y = *player->GetLowerBodyYawTarget();
			if (!player->PGetEyeAngles())
				continue;

			
			float MaxDesync = 90;
			float Split = MaxDesync / 4;
			player->PGetEyeAngles()->y = player->GetLBY();

			switch (ShotsMissed[i] % 9)
			{
			case 0:
				break;
			case 1:
				player->PGetEyeAngles()->y += 4 * Split;
				break;
			case 2:
				player->PGetEyeAngles()->y += 2 * Split;
				break;
			case 3:
				player->PGetEyeAngles()->y -= 2 * Split;
				break;
			case 4:
				player->PGetEyeAngles()->y -= 4 * Split;
				break;
			case 5:
				player->PGetEyeAngles()->y += 3 * Split;
				break;
			case 6:
				player->PGetEyeAngles()->y -= 3 * Split;
				break;
			case 7:
				player->PGetEyeAngles()->y += 1 * Split;
				break;
			case 8:
				player->PGetEyeAngles()->y -= 1 * Split;
				break;
			}
		}
	}
	*/
}

void Resolver::AnimationFix(Entity* entity)
{
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


	if (entity->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
		entity->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;

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



	if (entity->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
		entity->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;

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

void Resolver::ABSROTATION(Entity* entity)
{
	auto feet_yaw = entity->GetAnimOverlay(3)->m_flCycle > 0.9f && entity->GetAnimOverlay(3)->m_flWeight > 0.9f && entity->GetVecVelocity().VecLength2D() < 0.1f;
	auto body_max_rotation = 60.f;
	if (feet_yaw <= 60)
	{
		if (-60 > feet_yaw)
			(*entity->PGetEyeAngles()).y += body_max_rotation;
	}
	else
	{
		(*entity->PGetEyeAngles()).y = body_max_rotation - entity->GetEyeAngles().y;
	}
	if (entity->GetAnimOverlay(3)->m_flCycle > 0.9)
	{
		for (int resolve_delta = 60.f; resolve_delta < -60.f; resolve_delta = resolve_delta - 20.f)
		{
			(*entity->PGetEyeAngles()).y = resolve_delta;
		}
	}
}
