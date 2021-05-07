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
		if (ent->GetAnimOverlays())
			std::memcpy(PlayerList[UserId].layers, ent->GetAnimOverlays(), 15 * sizeof(AnimationLayer));
		if (ent->poseParameter())
			PlayerList[UserId].poses = ent->m_flPoseParameter();
		if (ent->GetAnimstate())
			std::memcpy(&PlayerList[UserId].animstate, ent->GetAnimstate(), sizeof(AnimState));

		// Clear records if dormant/dead
		if (!ValidRecord(PlayerList[UserId]))
		{
			PlayerList[UserId].Records.clear();
			continue;
		}

		// Determine if we even need to update
		// (if last sim time is this sim time AND the records aren't empty
		if (!PlayerList[UserId].Records.empty() && PlayerList[UserId].SimulationTime == PlayerList[UserId].Records.front().SimulationTime)
			continue;

		// If we need to add the current tick for a record
		if (ValidSimulationTime(PlayerList[UserId].SimulationTime))
		{
			// Create Tick :D
			Tick tick;
			// Matrix
			std::memcpy(tick.Matrix, PlayerList[UserId].Matrix, 256 * sizeof(Matrix3x4));
			// Dormant
			tick.Dormant = PlayerList[UserId].Dormant;
			// Velocity
			tick.Velocity = ent->GetVecVelocity();
			// Origin
			tick.Origin = PlayerList[UserId].Origin;
			// Abs Origin
			tick.AbsOrigin = PlayerList[UserId].AbsOrigin;
			// obb_mins / obb_maxs
			tick.obb_mins = PlayerList[UserId].obb_mins;
			tick.obb_maxs = PlayerList[UserId].obb_maxs;
			// HeadPos
			tick.HeadPos = Vec(tick.Matrix[8][0][3], tick.Matrix[8][1][3], tick.Matrix[8][2][3]);
			// EyeAng
			tick.EyeAng = ent->GetEyeAngles();
			// Sim Time
			tick.SimulationTime = PlayerList[UserId].SimulationTime;
			// TickCount
			tick.TickCount = G::cmd->tick_count;
			// Duck
			tick.Duck = ent->GetDuckAmount();
			// LBY
			tick.LBY = ent->GetLBY();
			// LastShotTIme
			tick.LastShotTIme = PlayerList[UserId].ptrWeap ? PlayerList[UserId].ptrWeap->GetLastShotTime() : 0.f;
			// SpawnTime
			tick.SpawnTime = ent->m_flSpawnTime();
			// Flags
			tick.Flags = ent->GetFlags();
			// Choked
			tick.Choked = 0.f;//will figure out later :D
			// Shot
			tick.Shot = false; // will aslo figure out later


			// Animstate crap
			std::memcpy(tick.layers, PlayerList[UserId].layers, 15 * sizeof(AnimationLayer));
			if(ent->poseParameter())
				tick.poses = ent->m_flPoseParameter();
			if(ent->GetAnimstate())
				std::memcpy(&tick.animstate, ent->GetAnimstate(), sizeof(AnimState));

			// Push tick forward
			PlayerList[UserId].Records.push_front(tick);
		}

		// Deal with too many records (anything above 200ms just forget about it :D)
		unsigned int Ticks = TimeToTicks(.2f);
		while (PlayerList[UserId].Records.size() > 3 && PlayerList[UserId].Records.size() > Ticks) {
			PlayerList[UserId].Records.pop_back();
		}

		// Deal with Bad Sim Times (:D)
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
