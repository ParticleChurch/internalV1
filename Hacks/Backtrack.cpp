#include "../Include.hpp"

Backtrack* backtrack = new Backtrack();

void Backtrack::update(int stage)
{
	Entity* Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!I::engine->IsInGame() || !Localplayer || !(Localplayer->GetHealth() > 0)) {
		for (auto a : Records)
			a.clear();
		return;
	}

	if (stage != FRAME_RENDER_START)
		return;

	for (int i = 1; i < 65; i++) {
		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent) //if not nullptr
		{
			Records[i].clear();
			continue;
		}
		static player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info)) //if player
		{
			Records[i].clear();
			continue;
		}
		if (Localplayer == Ent) //if player not localplayer
		{
			Records[i].clear();
			continue;
		}
		if (i == I::engine->GetLocalPlayer()) //if player localplayer
		{
			Records[i].clear();
			continue;
		}
		if (!(Ent->GetHealth() > 0)) //if health isn't greater than 0
		{
			Records[i].clear();
			continue;
		}
		if (Ent->GetTeam() == Localplayer->GetTeam())
		{
			Records[i].clear();
			continue;
		}
		if (Ent->IsDormant())
		{
			Records[i].clear();
			continue;
		}

		Tick tick;
		tick.TickCount = I::globalvars->m_tickCount;
		tick.Index = i;
		//could add ping to ticks to this value, to maybe be more accurate? try experimenting...
		Ent->SetupBones(tick.Matrix, 128, 0x100, 0);

		if (Records[i].empty() || Records[i].back().TickCount != tick.TickCount) {
			Records[i].push_back(tick);
		}

		if (Records[i].size() > 12)
		{
			Records[i].pop_front();
			Records[i].resize(12);
		}
	}

}

void Backtrack::run()
{
	Entity* Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if (!I::engine->IsInGame() || !Localplayer || !(Localplayer->GetHealth() > 0)) {
		return;
	}

	//get closest entity
	float CrossEntDist = FLT_MAX;
	int RecordIndex = -1;
	for (int i = 0; i < 65; i++)
	{
		if (Records[i].empty())
			continue;
		Vec Angle = aimbot->CalculateAngle(Records[i].back().Bone(8));
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
	for (Tick tick : Records[RecordIndex])
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
