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

Vec Aimbot::CalculateAngle(Vec Source, Vec Target)
{
	static float Distance;	//Total distance between target and starting head position
	Distance = sqrtf(pow(Source.x - Target.x, 2) + pow(Source.y - Target.y, 2) + pow(Source.z - Target.z, 2));

	static float Height;	//total difference in height between target and starting head position
	Height = Source.z - Target.z;

	static Vec Angle;
	Angle.x = RAD2DEG(asin(Height / Distance));									//yaw angle
	Angle.y = RAD2DEG(atan2(Source.y - Target.y, Source.x - Target.x)) - 180;	//pitch angle

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

	Entity* ActiveWeapon = G::Localplayer->GetActiveWeapon();
	if (!ActiveWeapon) return;

	if (!((bool)GetAsyncKeyState(legitbot.AimKey))) return;

	//Config Stuff (for certain weapons)
	int ConfigIndex = -1;
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

			bool InPriority = false;
			for (auto a : legitbot.weapon[ConfigIndex].Priority)
			{
				if (a == hitbox) {
					InPriority = true;

				}
			}
			
			if (CrossDist < BestCrossDist && !InPriority)
			{
				BestCrossDist = CrossDist;
				BestAngle = Angle;
				ValidTargetFound = true;
			}
			else if(InPriority)
			{
				BestCrossDist *= -1;
				BestAngle = Angle;
				ValidTargetFound = true;
			}
		}
		
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


	//get closest Entity
	int BestIndex = -1;
	float BestCrossDist = FLT_MAX;
	for (int i = 1; i < 65; i++) {
		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent) //if not nullptr
			continue;
		
		static player_info_t info;
		if (!I::engine->GetPlayerInfo(i, &info)) //if player
			continue;
		
		if (G::Localplayer == Ent) //if player not localplayer
			continue;
		
		if (i == I::engine->GetLocalPlayer()) //if player localplayer
			continue;
		
		if (!(Ent->GetHealth() > 0)) //if health isn't greater than 0
			continue;
		
		if (Ent->GetTeam() == G::Localplayer->GetTeam()) //if on local players team
			continue;
		
		if (Ent->IsDormant()) //if out of range... (def cant hit)
			continue;

		Vec		Location	= Ent->GetBone(HITBOX_HEAD);
		QAngle	Angle		= aimbot->CalculateAngle(Location);
		float	CrossDist	= aimbot->CrosshairDist(Angle);
		if (CrossDist < BestCrossDist)
		{
			BestCrossDist = CrossDist;
			BestIndex = i;
		}
	}

	//if a valid target hasn't been found
	if (BestIndex == -1)
		return;

	Entity* Ent = I::entitylist->GetClientEntity(BestIndex);
	if (!Ent) return; //if proper entity
	
	//absolute unit of a shit code
	Entity* Weapon = G::Localplayer->GetActiveWeapon();
	WeaponData* WeapData = Weapon->GetWeaponData();

	QAngle Angle;
	int tick_count;
	bool ValidFound = false;

	std::vector<Hitboxes> hitboxes = { HITBOX_PELVIS, HITBOX_STOMACH, HITBOX_LOWER_CHEST, HITBOX_CHEST, HITBOX_UPPER_CHEST, HITBOX_RIGHT_THIGH, HITBOX_LEFT_THIGH, HITBOX_HEAD };
	int HitGroup = 0;

	for (auto HBOX : hitboxes)
	{
		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
		if (!StudioModel) continue; //if cant get the model

		//if(!(tick.Value > 0)) continue; //if not onshot...

			//GOIN FOR HEAD BB
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...

		if (HBOX == HITBOX_HEAD)
			HitGroup = HITGROUP_HEAD;
		else if (HBOX == HITBOX_PELVIS || HBOX == HITBOX_STOMACH)
			HitGroup = HITGROUP_STOMACH;
		else if (HBOX == HITBOX_LOWER_CHEST || HBOX == HITBOX_CHEST || HBOX == HITBOX_UPPER_CHEST)
			HitGroup = HITGROUP_CHEST;
		else if (HBOX == HITBOX_RIGHT_THIGH)
			HitGroup = HITGROUP_RIGHTLEG;
		else if (HBOX == HITBOX_LEFT_THIGH)
			HitGroup = HITGROUP_LEFTLEG;

		Vec loc = Ent->GetBone(HBOX);

		if (!autowall->CanScanBacktrack(Ent, loc, WeapData, 1, true, HitGroup))
			continue;

		Angle = aimbot->CalculateAngle(loc);
		ValidFound = true;
	}
	
	if (!ValidFound) return;

	//HIDE SHOTS
	*G::pSendPacket = true;

	//actual shit
	Angle -= (G::Localplayer->GetAimPunchAngle() * 2);
	G::cmd->viewangles = Angle;
	G::cmd->buttons |= IN_ATTACK;
}

