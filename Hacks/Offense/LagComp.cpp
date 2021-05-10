#include "../../Include.hpp"

LagComp* lagcomp = new LagComp();

bool LagComp::ValidSimulationTime(float SimTime)
{
	if (!I::engine->GetNetChannelInfo())
		return false;

	const auto correct = std::clamp(I::engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING)
		+ I::engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING)
		+ GetLerp(), 0.f, 0.2f);
	return fabsf(correct - (I::globalvars->m_curTime - SimTime)) < 0.2f && correct < 1.f;
}

bool LagComp::ValidRecord(Player player)
{
	if (!player.Valid)
		return false;
	if (player.Index == G::LocalPlayerIndex)
		return false;
	if (!(player.Health > 0))
		return false;
	if (player.Team == G::LocalPlayerTeam)
		return false;
	if (player.Dormant)
		return false;
	return true;
}

// Happens in FRAME_NET_UPDATE_END
void LagComp::Update()
{
	// Set every Player to unupdated value
	std::map<int, Player>::iterator it;
	for (it = this->PlayerList.begin(); it != this->PlayerList.end(); it++)
	{
		it->second.Valid = false;
	}

	// Deal with everything else
	player_info_t TempInfo;
	for (int i = 1; i < 65; i++)
	{
		// Localplayer Check
		if (i == G::LocalPlayerIndex) continue;

		// Entity Existance Check
		Entity* ent = I::entitylist->GetClientEntity(i);
		if (!ent) continue;

		// If Not Player
		if (!I::engine->GetPlayerInfo(i, &TempInfo)) continue;

		int UserId = TempInfo.userid;

		// If not in the list... ADD IT BOY
		if (PlayerList.find(UserId) == PlayerList.end())
		{
			Player NewPlayer;
			PlayerList.insert(std::pair<int, Player>(UserId, NewPlayer));
		}

		L::Verbose("Update Player Crap");
		// Update Player Crap
		PlayerList[UserId].ptrEntity = ent;
		ent->SetupBones(PlayerList[UserId].Matrix, 256, BONE_USED_BY_ANYTHING, 0.f);
		PlayerList[UserId].ptrWeap = ent->GetActiveWeapon();
		// Weird model stuff
		Model* model = ent->GetModel();
		if (model)
		{
			if (!PlayerList[UserId].ptrModel)
			{
				PlayerList[UserId].ptrModel = new Model();
				PlayerList[UserId].ptrModel = model;
			}
			else
			{
				PlayerList[UserId].ptrModel = model;
			}
		}
		else
		{
			free(PlayerList[UserId].ptrModel);
		}
		L::Verbose("Continue update");
		// O.W. Continue
		PlayerList[UserId].Index = i;
		PlayerList[UserId].Health = ent->GetHealth();
		PlayerList[UserId].SimulationTime = ent->GetSimulationTime();
		PlayerList[UserId].Team = ent->GetTeam();
		PlayerList[UserId].info = TempInfo;
		PlayerList[UserId].Valid = true;
		PlayerList[UserId].EyePos = ent->GetEyePos();
		PlayerList[UserId].Origin = ent->GetVecOrigin();
		PlayerList[UserId].AbsOrigin = ent->GetAbsOrigin();
		PlayerList[UserId].obb_mins = ent->GetMins();
		PlayerList[UserId].obb_maxs = ent->GetMaxs();
		PlayerList[UserId].Dormant = ent->IsDormant();

		L::Verbose("Clear records if bad");
		// Clear records if dormant/dead
		if (!ValidRecord(PlayerList[UserId]))
		{
			PlayerList[UserId].Records.clear();
			continue;
		}

		L::Verbose("Update record?");
		// Determine if we even need to update
		// (if last sim time is this sim time AND the records aren't empty
		if (!PlayerList[UserId].Records.empty() && PlayerList[UserId].SimulationTime == PlayerList[UserId].Records.front().SimulationTime)
			continue;

		// If we need to add the current tick for a record
		if (ValidSimulationTime(PlayerList[UserId].SimulationTime))
		{
			L::Verbose("Adding Tick");
			// Create Tick :D
			Tick tick;
			// Matrix
			L::Verbose("Matrix");
			if(PlayerList[UserId].Matrix)
				std::memcpy(tick.Matrix, PlayerList[UserId].Matrix, 256 * sizeof(Matrix3x4));
			// Dormant
			L::Verbose("Dormant");
			tick.Dormant = PlayerList[UserId].Dormant;
			// Velocity
			L::Verbose("Velocity");
			tick.Velocity = ent->GetVecVelocity();
			// Origin
			L::Verbose("Origin");
			tick.Origin = PlayerList[UserId].Origin;
			// Abs Origin
			L::Verbose("AbsOrigin");
			tick.AbsOrigin = PlayerList[UserId].AbsOrigin;
			// obb_mins / obb_maxs
			L::Verbose("obb_mins/maxs");
			tick.obb_mins = PlayerList[UserId].obb_mins;
			tick.obb_maxs = PlayerList[UserId].obb_maxs;
			// HeadPos
			L::Verbose("HeadPos");
			if(tick.Matrix)
				tick.HeadPos = Vec(tick.Matrix[8][0][3], tick.Matrix[8][1][3], tick.Matrix[8][2][3]);
			// EyeAng
			L::Verbose("EyeAng");
			tick.EyeAng = ent->GetEyeAngles();
			// Sim Time
			L::Verbose("SimulationTime");
			tick.SimulationTime = PlayerList[UserId].SimulationTime;
			// TickCount
			L::Verbose("TickCount");
			tick.TickCount = G::cmd->tick_count;
			// Duck
			L::Verbose("GetDuckAmount");
			tick.Duck = ent->GetDuckAmount();
			// LBY
			L::Verbose("GetLBY");
			tick.LBY = ent->GetLBY();
			// LastShotTIme
			L::Verbose("LastShotTIme");
			tick.LastShotTIme = PlayerList[UserId].ptrWeap ? PlayerList[UserId].ptrWeap->GetLastShotTime() : 0.f;
			// SpawnTime
			L::Verbose("SpawnTime");
			tick.SpawnTime = ent->m_flSpawnTime();
			// Flags
			L::Verbose("Flags");
			tick.Flags = ent->GetFlags();
			// Choked
			L::Verbose("Choked");
			tick.Choked = 0.f;//will figure out later :D
			// Shot
			L::Verbose("Shot");
			tick.Shot = false; // will aslo figure out later

			// Push tick forward
			PlayerList[UserId].Records.push_front(tick);
		}

		L::Verbose("Deal with too many records");
		// Deal with too many records (anything above 200ms just forget about it :D)
		unsigned int Ticks = TimeToTicks(.2f);
		while (PlayerList[UserId].Records.size() > 3 && PlayerList[UserId].Records.size() > Ticks) {
			PlayerList[UserId].Records.pop_back();
		}

		// Deal with Bad Sim Times (:D)
		L::Verbose("Deal with Bad Sim Times");
		for (size_t j = 0; j < PlayerList[UserId].Records.size(); j++)
		{
			if (!ValidSimTime(PlayerList[UserId].Records[j].SimulationTime))
				PlayerList[UserId].Records.erase(PlayerList[UserId].Records.begin() + j);
		}
	}
}

void LagComp::ClearRecords()
{
	for (auto& a : PlayerList)
	{
		a.second.Records.clear();
	}
}

void LagComp::ClearPlayerList()
{
	ClearRecords();
	PlayerList.clear();
}
