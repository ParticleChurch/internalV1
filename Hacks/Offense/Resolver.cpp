#include "../../Include.hpp"

Resolver* resolver = new Resolver();

void Resolver::LogShots(GameEvent* event)
{
	/*ShowInfo();*/
	static bool ShotEnd = true;
	switch (StrHash::HashRuntime(event->GetName())) {
		// Called before bullet impact
	case StrHash::Hash("weapon_fire"):
	{
		int iUser = event->GetInt("userid");
		if (I::engine->GetPlayerForUserID(iUser) != G::LocalPlayerIndex)
			return;

		H::console.push_back("weapon_fire\n");
		dequeBulletImpacts.clear();
	}
	break;
	case StrHash::Hash("player_hurt"):
	{
		int iUser = event->GetInt("userid");
		int iAttacker = event->GetInt("attacker");

		if (I::engine->GetPlayerForUserID(iUser) == G::LocalPlayerIndex)
			return;

		if (I::engine->GetPlayerForUserID(iAttacker) != G::LocalPlayerIndex)
			return;

		H::console.push_back("player_hurt");
		bPlayerHurt[I::engine->GetPlayerForUserID(iUser)] = true;
	}
	break;
	case StrHash::Hash("bullet_impact"):
	{
		int iUser = event->GetInt("userid");

		if (I::engine->GetPlayerForUserID(iUser) != G::LocalPlayerIndex)
			return;

		if (dequeBulletImpacts.empty())
		{
			H::console.push_back("bullet_impact");
			dequeBulletImpacts.push_back(Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z")));
			bBulletImpact[TargetIndex] = true;
		}
	}
	break;
	default:
		return;
	}

	for (int i = 0; i < 64; i++)
	{
		if (bBulletImpact[i] && !dequeBulletImpacts.empty())
		{

			trace_t tr;
			Ray_t ray(G::LocalPlayer->GetEyePos(), dequeBulletImpacts.back());
			CTraceFilter traceFilter(G::LocalPlayer);
			//I::enginetrace->TraceRay(ray, 0x4600400B, &traceFilter, &tr);
			I::enginetrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

			auto hitEnt = tr.Entity;
			if (hitEnt && hitEnt->Index() == i && !bPlayerHurt[i])
			{
				H::console.push_back("Missed due to prediction @ " + std::to_string(i));
				//missed due to shit resolver
				bShotsMissed[i]++;

				bBulletImpact[i] = false;
				bPlayerHurt[i] = false;
				dequeBulletImpacts.clear();
			}
			else if (bPlayerHurt[i])
			{
				H::console.push_back("Hit Shot @ " + std::to_string(i));

				//shot real
				bBulletImpact[i] = false;
				bPlayerHurt[i] = false;
				dequeBulletImpacts.clear();
			}
			else
			{
				//missed due to spread
				H::console.push_back("Missed due to spread @ " + std::to_string(i));
				bBulletImpact[i] = false;
				bPlayerHurt[i] = false;
				dequeBulletImpacts.clear();
			}
		}
	}
	
	if (H::console.size() > 50 || GetAsyncKeyState(VK_LMENU))
	{
		H::console.clear();
		H::console.resize(0);
	}
	
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

			float MaxDesync = fabsf(player->GetMaxDesyncAngle());
			float Split = MaxDesync / 4;
			player->PGetEyeAngles()->y = player->GetLBY();

			switch (bShotsMissed[i] % 9)
			{
			case 0:
				break;
			case 1:
				player->PGetEyeAngles()->y += Split;
				break;
			case 2:
				player->PGetEyeAngles()->y += 2 * Split;
				break;
			case 3:
				player->PGetEyeAngles()->y += 3 * Split;
				break;
			case 4:
				player->PGetEyeAngles()->y += 4 * Split;
				break;
			case 5:
				player->PGetEyeAngles()->y -= Split;
				break;
			case 6:
				player->PGetEyeAngles()->y -= 2 * Split;
				break;
			case 7:
				player->PGetEyeAngles()->y -= 3 * Split;
				break;
			case 8:
				player->PGetEyeAngles()->y -= 4 * Split;
				break;
			}
		}
	}
}

void Resolver::ShowInfo()
{
	H::console.clear();
	H::console.resize(0);

	Entity* localplayer = (Entity*)I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!localplayer)
		return;

	int team = localplayer->GetTeam();

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

		float MaxDesync = fabsf(player->GetMaxDesyncAngle());
		float Split = MaxDesync / 4;
		player->PGetEyeAngles()->y = player->GetLBY();

		if (bShotsMissed[i] > 0)
		{
			switch (bShotsMissed[i] % 9)
			{
			case 0:
				break;
			case 1:
				H::console.push_back(std::to_string(Split));
				break;
			case 2:
				H::console.push_back(std::to_string(2 * Split));
				break;
			case 3:
				H::console.push_back(std::to_string(3 * Split));
				break;
			case 4:
				H::console.push_back(std::to_string(4 * Split));
				break;
			case 5:
				H::console.push_back(std::to_string(-Split));
				break;
			case 6:
				H::console.push_back(std::to_string(-2 * Split));
				break;
			case 7:
				H::console.push_back(std::to_string(-3 * Split));
				break;
			case 8:
				H::console.push_back(std::to_string(-4 * Split));
				break;
			}
		}

	}
}
