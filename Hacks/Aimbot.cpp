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
	if (!G::Localplayer->CanShoot())
		return;

	//get closest entity
	float CrossEntDist = FLT_MAX;
	int RecordIndex = -1;
	for (int i = 0; i < 65; i++)
	{
		if (backtrack->Records[i].empty())
			continue;
		Vec Angle = aimbot->CalculateAngle(backtrack->Records[i].back().Bone(8));
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			RecordIndex = i;
			CrossEntDist = CrossDist;
		}
	}

	if (RecordIndex == -1)
		return;

	//searching for onshot tick
	int BestTickCount = -1;
	Vec BestAngle;
	Vec Target;
	for (Tick tick : backtrack->Records[RecordIndex])
	{
		if (tick.Shooting)
		{
			Entity* Ent = I::entitylist->GetClientEntity(tick.Index);
			if (autowall->CanScan(Ent, tick.Bone(8), G::Localplayer->GetActiveWeapon()->GetWeaponData(), 1, true))
			{
				BestTickCount = tick.TickCount;
				BestAngle = CalculateAngle(tick.Bone(8));
				Target = tick.Bone(8);
			}
		}
	}

	//if unable to find Onshot tick
	if (BestTickCount == -1)
	{
		Vec loc = backtrack->Records[RecordIndex].back().Bone(8);
		Entity* Ent = I::entitylist->GetClientEntity(backtrack->Records[RecordIndex].back().Index);
		if (!autowall->CanScan(Ent, loc, G::Localplayer->GetActiveWeapon()->GetWeaponData(), 1, true))
			return;
		BestAngle = CalculateAngle(loc);
	}
	else //if there is an onshot tick
	{
		G::cmd->tick_count = BestTickCount;
	}

	BestAngle -= (G::Localplayer->GetAimPunchAngle() * 2);

	G::cmd->viewangles = BestAngle;
	G::cmd->buttons |= IN_ATTACK;

	trace_t Trace;
	Trace.Startpos = G::Localplayer->GetEyePos();
	Trace.Endpos = Target;
	esp->traces.clear();
	esp->traces.resize(0);
	esp->traces.push_back(Trace);
}

