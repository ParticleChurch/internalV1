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

			/*float MaxDesync = fabsf(player->GetMaxDesyncAngle());*/
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
}