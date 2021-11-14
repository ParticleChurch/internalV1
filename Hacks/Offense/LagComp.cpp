#include "../../Include.hpp"

LagComp* lagcomp = new LagComp();

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
			L::Debug("Add New Player");
			Player NewPlayer;
			NewPlayer.ptrEntity = ent;
			ent->SetupBones(NewPlayer.Matrix, 128, BONE_USED_BY_ANYTHING, 0.f);
			NewPlayer.ptrWeap = ent->GetActiveWeapon();
			// Weird model stuff
			Model* model = ent->GetModel();
			if (model)
			{
				if (!NewPlayer.ptrModel)
				{
					NewPlayer.ptrModel = new Model();
					NewPlayer.ptrModel = model;
				}
				else
				{
					NewPlayer.ptrModel = model;
				}
			}
			else
			{
				free(NewPlayer.ptrModel);
			}
			L::Debug("Continue update");
			// O.W. Continue
			NewPlayer.Index = i;
			NewPlayer.Health = ent->GetHealth();
			NewPlayer.SimulationTime = ent->GetSimulationTime();
			NewPlayer.Team = ent->GetTeam();
			NewPlayer.info = TempInfo;
			NewPlayer.Valid = true;
			NewPlayer.Origin = ent->GetVecOrigin();
			NewPlayer.EyePos = ent->GetEyePos();
			NewPlayer.Dormant = ent->IsDormant();
			PlayerList.insert(std::pair<int, Player>(UserId, NewPlayer));
		} 

		// otherwise we update the enity
		else
		{
			PlayerList[UserId].ptrEntity = ent;
			ent->SetupBones(PlayerList[UserId].Matrix, 128, BONE_USED_BY_HITBOX, 0.f);
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
			L::Debug("Continue update");
			// O.W. Continue
			PlayerList[UserId].Index = i;
			PlayerList[UserId].Health = ent->GetHealth();
			PlayerList[UserId].SimulationTime = ent->GetSimulationTime();
			PlayerList[UserId].Team = ent->GetTeam();
			PlayerList[UserId].info = TempInfo;
			PlayerList[UserId].Valid = true;
			PlayerList[UserId].Origin = ent->GetVecOrigin();
			PlayerList[UserId].EyePos = ent->GetEyePos();
			PlayerList[UserId].Dormant = ent->IsDormant();
		}

		// NOW DEAL WITH BACKTRACKING!_____________________________________________________

		// Dont bother to even attempt update if same sim time
		if (!PlayerList[UserId].Records.empty() && PlayerList[UserId].SimulationTime == PlayerList[UserId].Records.front().SimulationTime)
			continue;

		L::Debug("Clear records if bad");
		// Clear records if dormant/dead
		if (!ValidRecord(PlayerList[UserId]))
		{
			PlayerList[UserId].Records.clear();
			continue;
		}

		// Add the record
		L::Debug("Add to records");
		auto& p = PlayerList[UserId];
		Tick newTick;
		L::Debug("Matrix");
		std::memcpy(newTick.Matrix, p.Matrix, 128 * sizeof(Matrix3x4));
		L::Debug("Dormant");
		newTick.Dormant = p.Dormant;
		L::Debug("GetVecVelocity");
		newTick.Velocity = p.ptrEntity->GetVecVelocity();
		L::Debug("Origin");
		newTick.Origin = p.Origin;
		L::Debug("HeadPos");
		newTick.HeadPos = Vec(p.Matrix[8][0][3], p.Matrix[8][1][3], p.Matrix[8][2][3]);
		L::Debug("EyeAng");
		newTick.EyeAng = p.ptrEntity->GetEyeAngles();
		L::Debug("SimulationTime");
		newTick.SimulationTime = p.SimulationTime;
		L::Debug("Duck");
		newTick.Duck = p.ptrEntity->GetDuckAmount();
		L::Debug("LBY");
		newTick.LBY = p.ptrEntity->GetLBY();
		L::Debug("Flags");
		newTick.Flags = p.ptrEntity->GetFlags();
		L::Debug("Choked");
		newTick.Choked = 0;
		if (!PlayerList[UserId].Records.empty())
			newTick.Choked = TimeToTicks(PlayerList[UserId].SimulationTime - PlayerList[UserId].Records.front().SimulationTime);
		L::Debug("push_front");
		PlayerList[UserId].Records.push_front(newTick);
		
		L::Debug("Deal with too many records");
		// Deal with too many records (anything above 200ms just forget about it :D)
		unsigned int Ticks = TimeToTicks(0.2f);
		while (PlayerList[UserId].Records.size() > 3 && PlayerList[UserId].Records.size() > Ticks) {
			PlayerList[UserId].Records.pop_back();
		}

		// Deal with Bad Sim Times (:D)
		L::Debug("Deal with Bad Sim Times");
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
