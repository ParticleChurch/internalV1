#include "../Include.hpp"

Aimbot* aimbot = new Aimbot();

//general help functions
Vec Aimbot::CalculateAngle(Vec Target)
{
	Vec EyePos = G::Localplayer->GetEyePos();//starting head position

	static float Distance;	//Total distance between target and starting head position
	Distance = sqrtf(pow(EyePos.x - Target.x, 2) + pow(EyePos.y - Target.y, 2) + pow(EyePos.z - Target.z, 2));

	static float Height;	//total difference in height between target and starting head position
	Height = EyePos.z - Target.z;

	static Vec Angle;
	Angle.x = RAD2DEG(asin(Height / Distance));									//yaw angle
	Angle.y = RAD2DEG(atan2(EyePos.y - Target.y, EyePos.x - Target.x)) - 180;	//pitch angle

	Angle.Normalize();	//making sure the angles are proper
	return Angle;
}

float Aimbot::CrosshairDist(Vec TargetAngle)
{
	static Vec cur;
	cur = G::CM_StartAngle;
	cur.Normalize();
	cur += G::Localplayer->GetAimPunchAngle();
	cur.Normalize();

	//turn to 0-360 degrees
	TargetAngle.y += 180;
	cur.y += 180;

	//finds both angles and chooses shortest yaw angle delta
	float a = fabsf(TargetAngle.y - cur.y);
	float b = 360 - fabsf(TargetAngle.y - cur.y);

	//gets the pitch angle delta 
	float c = abs(TargetAngle.x - cur.x);

	//gets the total distance (in degrees)
	return sqrtf(powf(min(a, b), 2) + powf(c, 2));
}

//legit functions
void Aimbot::Legit()
{
	//get closest Entity
	Vec BestAngle;
	float BestCrossDist = FLT_MAX; //crosshair distance
	bool ValidTargetFound = false;

	for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
	{
		player_info_t PlayerInfo;
		if (!I::engine->GetPlayerInfo(i, &PlayerInfo)) //if not player
			continue;

		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent)
			continue;

		if (Ent->GetTeam() == G::Localplayer->GetTeam()) //if teamate
			continue;

		if (Ent->IsDormant()) //if dormant
			continue;

		if (!(Ent->GetHealth() > 0)) //if dead
			continue;

		Vec Head = Ent->GetBonePos(8);
		Vec Angle = CalculateAngle(Head);
		float CrossDist = CrosshairDist(Angle);
		if (CrossDist < BestCrossDist)
		{
			BestCrossDist = CrossDist;
			BestAngle = Angle;
			ValidTargetFound = true;
		}
	}

	if(G::Localplayer->GetShotsFired() > 1)
		BestAngle -= (G::Localplayer->GetAimPunchAngle() * 2);

	Smooth(BestAngle);

	if (ValidTargetFound)
		G::cmd->viewangles = BestAngle;
}

void Aimbot::Smooth(Vec& Angle)
{
	G::CM_StartAngle.Normalize();
	Angle.Normalize();
	Vec Delta = G::CM_StartAngle - Angle;

	Delta.Normalize();

	bool FastToSlow = false;
	float SmoothYaw = 0.1f;
	float SmoothPitch = 0.1f;
	if (FastToSlow)
	{
		Delta.y *= SmoothYaw;
		Delta.x *= SmoothPitch;
	}

	bool Constant = true;
	float Speed = 2;
	if (Constant)
	{
		float CurSpeed = Delta.VecLength();
		if (CurSpeed > Speed)
		{
			Delta.y *= (Speed / CurSpeed);
			Delta.x *= (Speed / CurSpeed);
		}
	}

	bool SlowToFast = false;
	float Factor = 5;
	if (SlowToFast)
	{
		float CurSpeed = powf(Factor,2) / powf(Delta.VecLength(), 2);
		if (CurSpeed < Speed)
		{
			Delta.y *= CurSpeed;
			Delta.x *= CurSpeed;
		}
	}

	Angle = G::CM_StartAngle + Delta;
	Angle.Normalize();
}

//rage functions
void Aimbot::Rage()
{
	//if (!Config::ragebot.Enable)
		//return;

	//if (!Config::ragebot.EnableAim)
		//return;

	if (!G::Localplayer->CanShoot())
		return;

	if (!G::Localplayer->GetAmmo())
		return;

	WeaponData* Weapondata = G::Localplayer->GetActiveWeapon()->GetWeaponData();
	if (!Weapondata) return;

	//get closest entity
	float CrossEntDist = FLT_MAX;
	int RecordIndex = -1;
	for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
	{
		player_info_t PlayerInfo;
		if (!I::engine->GetPlayerInfo(i, &PlayerInfo)) //if not player
			continue;

		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent)
			continue;

		if (Ent->GetTeam() == G::Localplayer->GetTeam()) //if teamate
			continue;

		if (Ent->IsDormant()) //if dormant
			continue;

		if (!(Ent->GetHealth() > 0)) //if dead
			continue;

		Vec Head = Ent->GetBonePos(8);
		Vec Angle = CalculateAngle(Head);
		float CrossDist = CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			CrossEntDist = CrossDist;
			RecordIndex = i;
		}
	}

	//if unable to find proper record
	if (RecordIndex == -1) return;

	

	Tick BestTick;
	Vec BestTarget;
	bool ValidFound = false;
	float SimTime = FLT_MAX;
	std::vector<int> Hitboxes = { HITBOX_HEAD }; 
	for (auto Hitbox : Hitboxes)
	{
		for (Tick tick : backtrack->Records[RecordIndex])
		{
			if (!backtrack->Valid(tick.SimulationTime))
				continue;

			if (!tick.Shooting) continue;		//ONLY ONSHOT_______________________

			Entity* Ent = I::entitylist->GetClientEntity(tick.Index);
			if (!Ent) continue;
			studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
			if (!StudioModel) continue;

			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(Hitbox);
			if (!StudioBox) continue;

			float radius = StudioBox->m_flRadius * 0.9;
			Vec min = StudioBox->bbmin.Transform(tick.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmax.Transform(tick.Matrix[StudioBox->bone]);


			//Vec MinLeft = Ent->GetLeft(min, radius, Ent);
			//Vec MinRight = Ent->GetRight(min, radius, Ent);
			//Vec MaxLeft = Ent->GetLeft(max, radius, Ent);
			//Vec MaxRight = Ent->GetRight(max, radius, Ent);
			Vec Middle = (max + min) / 2.f;
			if (!autowall->CanScanBacktrack(Ent, Middle, Weapondata, 1, true, HITGROUP_HEAD)) continue;

			SimTime = tick.SimulationTime;
			BestTarget = Middle;
			BestTick = tick;
			ValidFound = true;
		}
	}

	if (!ValidFound) return;
	
	Vec Angle = CalculateAngle(BestTarget);
	Angle -= (G::Localplayer->GetAimPunchAngle() * 2);

	G::cmd->viewangles = Angle;
	G::cmd->tick_count = backtrack->TimeToTicks(SimTime);
	G::cmd->buttons |= IN_ATTACK;
}

