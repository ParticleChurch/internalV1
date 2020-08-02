#include "../Include.hpp"

Backtrack* backtrack = new Backtrack();

void Backtrack::update(int stage)
{
	Entity* Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!I::engine->IsInGame() || !Localplayer || !(Localplayer->GetHealth() > 0)) {
		for (auto a : records)
			a.clear();
		return;
	}

	if (stage != FRAME_RENDER_START)
		return;

	for (int i = 1; i < 65; i++) {
		static Entity* ent;
		ent = I::entitylist->GetClientEntity(i);
		if (!ent) //if not nullptr
		{
			records[i].clear();
			continue;
		}
		static player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info)) //if player
		{
			records[i].clear();
			continue;
		}
		if (Localplayer == ent) //if player not localplayer
		{
			records[i].clear();
			continue;
		}
		if (i == I::engine->GetLocalPlayer()) //if player localplayer
		{
			records[i].clear();
			continue;
		}
		if (!(ent->GetHealth() > 0)) //if health isn't greater than 0
		{
			records[i].clear();
			continue;
		}
		if (ent->GetTeam() == Localplayer->GetTeam())
		{
			records[i].clear();
			continue;
		}

		Tick tick;
		tick.TickCount = I::globalvars->m_tickCount;
		//could add ping to ticks to this value, to maybe be more accurate? try experimenting...
		ent->SetupBones(tick.Matrix, 128, 0x100, 0);

		if (records[i].empty() || records[i].back().TickCount != tick.TickCount) {
			records[i].push_back(tick);
		}

		if (records[i].size() > 12)
		{
			records[i].pop_front();
			records[i].resize(12);
		}
	}

}

void Backtrack::run()
{
	static Entity* localPlayer;
	localPlayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!I::engine->IsInGame() || !localPlayer || !(localPlayer->GetHealth() > 0)) {
		return;
	}

	//get closest entity
	float CrossEntDist = FLT_MAX;
	int RecordIndex = -1;
	for (int i = 0; i < 65; i++)
	{
		if (records[i].empty())
			continue;
		Vec Angle = aimbot->CalculateAngle(records[i].back().Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			RecordIndex = i;
			CrossEntDist = CrossDist;
		}
	}

	//if unable to find proper record
	if (RecordIndex == -1)
		return;

	//get closest tick
	float CrossTickDist = FLT_MAX;
	int BestTickCount = -1;
	for (Tick tick : records[RecordIndex])
	{
		Vec Angle = aimbot->CalculateAngle(tick.Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossTickDist)
		{
			BestTickCount = tick.TickCount;
			CrossTickDist = CrossDist;
		}
	}

	//if unable to find proper tick
	if (BestTickCount == -1)
		return;

	//set command number to proper number
	G::cmd->tick_count = BestTickCount;
}
