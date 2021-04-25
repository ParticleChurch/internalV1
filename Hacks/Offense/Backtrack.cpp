#include "../../Include.hpp"

Backtrack* backtrack = new Backtrack();

bool Backtrack::ValidPlayer(Player player)
{
	if (!player.Valid)
		return false;
	if (player.index == G::LocalPlayerIndex)
		return false;
	if (!(player.health > 0))
		return false;
	if (player.team == G::LocalPlayerTeam)
		return false;
	if (player.dormant)
		return false;
	return true;
}

void Backtrack::GetClosestEntity(int& RecordUserID)
{
	float CrossEntDist = FLT_MAX;
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		Player player = it->second;
		if (it->second.BacktrackRecords.empty())
			continue;
		Vec Angle = aimbot->CalculateAngle(it->second.BacktrackRecords.back().Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			RecordUserID = it->first;
			CrossEntDist = CrossDist;
		}
	}
}

void Backtrack::GetClosestTick(int RecordUserID, int& BestTickCount)
{
	float CrossTickDist = FLT_MAX;
	TragetTick = G::EntList[RecordUserID].BacktrackRecords.front();
	for (Player::Tick tick : G::EntList[RecordUserID].BacktrackRecords)
	{
		Vec Angle = aimbot->CalculateAngle(tick.Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossTickDist)
		{
			BestTickCount = TimeToTicks(tick.SimulationTime - GetLerp()) + 4;
			CrossTickDist = CrossDist;
			TragetTick = tick;
		}
	}
}

void Backtrack::ClearRecords()
{
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
		it->second.BacktrackRecords.clear();
}

void Backtrack::RunFSN()
{
	static Config2::CFloat* BacktrackTime = Config2::GetFloat("legitaim-backtrack-time");

	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		if (!ValidPlayer(it->second))
		{
			it->second.BacktrackRecords.clear();
			continue;
		}

		if (!it->second.BacktrackRecords.empty() && it->second.BacktrackRecords.front().SimulationTime == it->second.CurSimTime)
			continue;

		Player::Tick tick;
		tick.SimulationTime = it->second.CurSimTime;
		it->second.entity->SetupBones(tick.Matrix, 128, BONE_USED_BY_ANYTHING, 0.f);// I::globalvars->m_curTime);
		tick.priority = resolver->GetPriority(it->first);

		it->second.BacktrackRecords.push_front(tick);

		unsigned int Ticks = TimeToTicks(BacktrackTime->Get() / 1000.f);
		while (it->second.BacktrackRecords.size() > 3 && it->second.BacktrackRecords.size() > Ticks) {
			it->second.BacktrackRecords.pop_back();
		}

		for (size_t j = 0; j < it->second.BacktrackRecords.size(); j++)
		{
			if (!ValidSimTime(it->second.BacktrackRecords[j].SimulationTime))
				it->second.BacktrackRecords.erase(it->second.BacktrackRecords.begin() + j);
		}
	}
}

static void CapsuleOverlay2(Entity* pPlayer, Color col, float duration, Matrix3x4 pBoneToWorldOut[128])
{
	if (!pPlayer)
		return;

	studiohdr_t* pStudioModel = I::modelinfo->GetStudioModel(pPlayer->GetModel());
	if (!pStudioModel)
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	auto VectorTransform2 = [](const Vec in1, Matrix3x4 in2, Vec& out)
	{

		out.x = DotProduct(in1, Vec(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out.y = DotProduct(in1, Vec(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out.z = DotProduct(in1, Vec(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	};

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vec vMin, vMax;
		VectorTransform2(pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform2(pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->m_flRadius > -1)
		{
			I::debugoverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->m_flRadius, col.r(), col.g(), col.b(), 255, duration);
		}
	}
}

void Backtrack::run()
{
	static Config2::CState* RageAimbot = Config2::GetState("rage-aim-enable");

	if (RageAimbot->Get()) return;

	// If we are using rage aimbot, don't try to legit aimbot lol
	/*if (Config::GetBool("rage-aim-enable")) return;*/

	if (!I::engine->IsInGame() || !G::LocalPlayer || !G::LocalPlayerAlive) {
		return;
	}

	int RecordUserID = -1;
	GetClosestEntity(RecordUserID);

	//if unable to find proper record
	if (RecordUserID == -1)
		return;

	int BestTickCount = -1;
	GetClosestTick(RecordUserID, BestTickCount);

	//if unable to find proper tick
	if (BestTickCount == -1)
		return;

	// If ya can't shoot
	if (!G::LocalPlayer->CanShoot())
		return;

	// If you aren't trying to shoot
	if (!(G::cmd->buttons & IN_ATTACK))
		return;

	G::cmd->tick_count = BestTickCount;
}

