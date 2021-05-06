#include "../../Include.hpp"

Backtrack* backtrack = new Backtrack();

void Backtrack::GetClosestEntity(int& RecordUserID)
{
	float CrossEntDist = FLT_MAX;
	std::map<int, Player>::iterator it;
	for (it = lagcomp->PlayerList.begin(); it != lagcomp->PlayerList.end(); it++)
	{
		Player player = it->second;
		if (it->second.Records.empty())
			continue;
		Vec Angle = aimbot->CalculateAngle(it->second.Records.back().HeadPos);
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
	for (Tick tick : lagcomp->PlayerList[RecordUserID].Records)
	{
		Vec Angle = aimbot->CalculateAngle(tick.HeadPos);
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossTickDist)
		{
			TargetTick = tick;
			BestTickCount = TimeToTicks(tick.SimulationTime + GetLerp());
			CrossTickDist = CrossDist;
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
	static Config::CState* RageAimbot = Config::GetState("rage-aim-enable");

	// If we are using rage aimbot, don't try to legit backtrack lol (rn forcing it cuz im dumb i think
	if (RageAimbot->Get()) return;

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

	//CapsuleOverlay2(lagcomp->PlayerList[RecordUserID].ptrEntity, Color(255, 125, 125, 255), 4.f, TargetTick.Matrix);

}

