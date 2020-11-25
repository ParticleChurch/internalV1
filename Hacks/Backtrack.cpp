#include "../Include.hpp"

Backtrack* backtrack = new Backtrack();

void Backtrack::Init()
{
	UpdateRate = I::cvar->FindVar("cl_updaterate");
	MaxUpdateRate = I::cvar->FindVar("sv_maxupdaterate");
	Interp = I::cvar->FindVar("cl_interp");
	InterpRatio = I::cvar->FindVar("cl_interp_ratio");
	MinInterpRatio = I::cvar->FindVar("sv_client_min_interp_ratio");
	MaxInterpRatio = I::cvar->FindVar("sv_client_max_interp_ratio");
	MaxUnlag = I::cvar->FindVar("sv_maxunlag");
}

void Backtrack::update(int CurStage)
{
	if (CurStage != FRAME_RENDER_START)
		return;

	if (!I::engine->IsInGame() || !G::LocalPlayer || !G::LocalPlayerAlive) {
		for (auto a : Records)
			a.clear();
		return;
	}

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
		if (G::LocalPlayer == Ent) //if player not localplayer
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
		if (Ent->GetTeam() == G::LocalPlayer->GetTeam())
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
		tick.Index = i;
		tick.SimulationTime = Ent->GetSimulationTime();
		Ent->SetupBones(tick.Matrix, 256, BONE_USED_BY_ANYTHING, I::globalvars->m_curTime);
		
		tick.Angle = Ent->GetEyeAngles();
		tick.EyePos = Ent->GetEyePos();
		tick.InAir = !(Ent->GetFlags() & FL_ONGROUND);

		Records[i].push_front(tick);

		unsigned int Ticks = TimeToTicks(Config::GetFloat("backtracking-time") / 1000.f);
		while (Records[i].size() > 3 && Records[i].size() > Ticks) {
			Records[i].pop_back();
		}
		
		for (unsigned int j = 0; j < Records[i].size(); j++)
		{
			if (!Valid(Records[i][j].SimulationTime))
				Records[i].erase(Records[i].begin() + j);
		}
	}
}

void Backtrack::run()
{
	if (!(G::cmd->buttons & IN_ATTACK))
		return;

	if (!I::engine->IsInGame() || !G::LocalPlayer || !G::LocalPlayerAlive) {
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
	Tick BestTick;
	for (Tick tick : Records[RecordIndex])
	{
		if (!Valid(tick.SimulationTime))
			continue;

		Vec Angle = aimbot->CalculateAngle(tick.Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossTickDist)
		{
			BestTickCount = backtrack->TimeToTicks(tick.SimulationTime - GetLerp());
			CrossTickDist = CrossDist;
			BestTick = tick;
		}
	}

	//if unable to find proper tick
	if (BestTickCount == -1)
		return;

	//set command number to proper number
	G::cmd->tick_count = BestTickCount;
}

