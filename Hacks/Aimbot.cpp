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
	using namespace Config;
	if (!legitbot.Enable) return;

	if (!legitbot.EnableAim) return;

	if (!G::Localplayer) return;

	if (!(G::Localplayer->GetHealth() > 0)) return;

	if (!GetAsyncKeyState(legitbot.AimKey)) return;

	//Config Stuff (for certain weapons)
	int ConfigIndex = -1;
	Entity* ActiveWeapon = G::Localplayer->GetActiveWeapon();
	if (!ActiveWeapon) return;
	WeaponId WeaponID = ActiveWeapon->GetWeaponId();
	int WeaponClass = GetWeaponClass(WeaponID);
	if (WeaponClass == 35) //pistol
		ConfigIndex = 0;
	else if (WeaponClass == 37)
		ConfigIndex = 1;
	else if (WeaponClass == 36)
		ConfigIndex = 2;
	else if (WeaponClass == 38)
	{
		if (GetWeaponIndex(WeaponID) <= 34 && GetWeaponIndex(WeaponID) >= 32)
			ConfigIndex = 4;
		else if (GetWeaponIndex(WeaponID) <= 31 && GetWeaponIndex(WeaponID) >= 24)
			ConfigIndex = 3;
	}
	if (ConfigIndex == -1) return; //if not holding a weapon...

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

		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
		if (!StudioModel) continue; //if cant get the model

		if (backtrack->Records[i].empty()) continue; //if the record is empty...

		//general hitboxes
		for (auto hitbox : legitbot.weapon[ConfigIndex].Hitboxes)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(hitbox);
			if (!StudioBox) continue;	//if cant get the hitbox...

			Vec min = StudioBox->bbmin.Transform(backtrack->Records[i].front().Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmax.Transform(backtrack->Records[i].front().Matrix[StudioBox->bone]);
			Vec loc = (max + min) / 2.f; //just getting middle of bone cuz not rage...

			if (!autowall->IsVisible(loc, Ent)) //if not visible
				continue;

			Vec Angle = CalculateAngle(loc);
			float CrossDist = CrosshairDist(Angle);
			if (CrossDist > legitbot.weapon[ConfigIndex].Fov) //if not in fov
				continue;

			if (CrossDist < BestCrossDist)
			{
				BestCrossDist = CrossDist;
				BestAngle = Angle;
				ValidTargetFound = true;
			}
		}
		
		//for priority
		/*
		for (auto hitbox : legitbot.weapon[ConfigIndex].Priority)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(hitbox);
			if (!StudioBox) continue;	//if cant get the hitbox...

			Vec min = StudioBox->bbmin.Transform(backtrack->Records[i].front().Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmax.Transform(backtrack->Records[i].front().Matrix[StudioBox->bone]);
			Vec loc = (max + min) / 2.f; //just getting middle of bone cuz not rage...

			if (!autowall->IsVisible(loc, Ent)) //if not visible
				return;

			Vec Angle = CalculateAngle(loc);
			float CrossDist = CrosshairDist(Angle);
			if (CrossDist < legitbot.weapon[ConfigIndex].Fov) //if not in fov
				continue;

			if (CrossDist < BestCrossDist)
			{
				BestCrossDist = CrossDist;
				BestAngle = Angle;
				ValidTargetFound = true;
			}
		}
		*/
	}

	if(G::Localplayer->GetShotsFired() > 1 &&		//shots 
		legitbot.weapon[ConfigIndex].RecoilControl)	//recoil enabled
		BestAngle -= (G::Localplayer->GetAimPunchAngle() * 2);

	Smooth(BestAngle, ConfigIndex);

	if (ValidTargetFound)
	{
		G::cmd->viewangles = BestAngle;
	}
		
} 

void Aimbot::Smooth(Vec& Angle, int ConfigIndex)
{
	bool FastToSlow = false;
	bool Constant = true;
	bool SlowToFast = false;
	float SmoothYaw = Config::legitbot.weapon[ConfigIndex].YawSpeed / 100.f;
	float SmoothPitch = Config::legitbot.weapon[ConfigIndex].PitchSpeed / 100.f;

	G::CM_StartAngle.Normalize();
	Angle.Normalize();
	Vec Delta = G::CM_StartAngle - Angle;

	Delta.Normalize();

	if (FastToSlow)
	{
		Delta.y *= SmoothYaw;
		Delta.x *= SmoothPitch;
	}

	float SpeedYaw = 10 * SmoothYaw;
	float SpeedPitch = 10 * SmoothPitch;
	if (Constant)
	{
		float CurSpeed = Delta.VecLength();
		if (CurSpeed > SpeedYaw)
		{
			Delta.y *= (SpeedYaw / CurSpeed);
		}
		if (CurSpeed > SpeedPitch)
		{
			Delta.x *= (SpeedPitch / CurSpeed);
		}
	}

	float Factor = 5;
	if (SlowToFast)
	{
		float CurSpeed = powf(Factor,2) / powf(Delta.VecLength(), 2);
		if (CurSpeed < SpeedPitch)
		{
			Delta.y *= CurSpeed;
			Delta.y *= SmoothYaw;
		}
		if (CurSpeed < SpeedYaw)
		{
			Delta.x *= CurSpeed;
			Delta.x *= SmoothPitch;
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
	float BestAngle = FLT_MAX;
	bool TargetShoot = false;
	for (auto Hitbox : Hitboxes)
	{
		for (Tick tick : backtrack->Records[RecordIndex])
		{
			if (!backtrack->Valid(tick.SimulationTime))
				continue;

			Entity* Ent = I::entitylist->GetClientEntity(tick.Index);
			if (!Ent) continue;
			studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
			if (!StudioModel) continue;

			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(Hitbox);
			if (!StudioBox) continue;

			float radius = StudioBox->m_flRadius * 0.5;
			Vec min = StudioBox->bbmin.Transform(tick.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmax.Transform(tick.Matrix[StudioBox->bone]);



			//Vec MinLeft = Ent->GetLeft(min, radius, Ent);
			//Vec MinRight = Ent->GetRight(min, radius, Ent);
			//Vec MaxLeft = Ent->GetLeft(max, radius, Ent);
			//Vec MaxRight = Ent->GetRight(max, radius, Ent);
			Vec Middle = (max + min) / 2.f;
			Middle.z += radius;
			if (!autowall->CanScanBacktrack(Ent, Middle, Weapondata, 20, true, HITGROUP_HEAD)) continue;

			if (BestAngle < tick.angle.x)
				continue;

			if (TargetShoot)
				continue;

			if (tick.Shooting)
				TargetShoot = true;		//ONLY ONSHOT_______________________

			BestAngle = tick.angle.x;
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

