#include "../Include.hpp"

Aimbot* aimbot = new Aimbot();

//general help functions
Vec Aimbot::CalculateAngle(Vec Target)
{
	Vec EyePos = G::LocalPlayer->GetEyePos();//starting head position

	static float Distance;	//Total distance between target and starting head position
	Distance = sqrtf(pow(EyePos.x - Target.x, 2) + pow(EyePos.y - Target.y, 2) + pow(EyePos.z - Target.z, 2));

	static float Height;	//total difference in height between target and starting head position
	Height = EyePos.z - Target.z;

	static Vec Angle;
	Angle.x = RAD2DEG(asin(Height / Distance));									//yaw angle
	Angle.y = RAD2DEG(atan2(EyePos.y - Target.y, EyePos.x - Target.x)) - 180;	//pitch angle

	Angle.NormalizeAngle();	//making sure the angles are proper
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

	Angle.NormalizeAngle();	//making sure the angles are proper
	return Angle;
}

float Aimbot::CrosshairDist(Vec TargetAngle)
{
	static Vec cur;
	cur = G::StartAngle;
	cur.NormalizeAngle();
	cur += G::LocalPlayer->GetAimPunchAngle();
	cur.NormalizeAngle();

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
//void aimbot::legit()
//{
//	if (!g::localplayer) return;
//
//	if (!g::localplayeralive) return;
//
//	entity* activeweapon = g::localplayer->getactiveweapon();
//	if (!activeweapon) return;
//
//	//get closest entity
//	vec bestangle;
//	float bestcrossdist = flt_max; //crosshair distance
//	bool validtargetfound = false;
//
//	for (int i = 0; i < i::entitylist->gethighestentityindex(); i++)
//	{
//		player_info_t playerinfo;
//		if (!i::engine->getplayerinfo(i, &playerinfo)) //if not player
//			continue;
//
//		entity* ent = i::entitylist->getcliententity(i);
//		if (!ent)
//			continue;
//
//		if (ent->getteam() == g::localplayer->getteam()) //if teamate
//			continue;
//
//		if (ent->isdormant()) //if dormant
//			continue;
//
//		if (!(ent->gethealth() > 0)) //if dead
//			continue;
//
//		studiohdr_t* studiomodel = i::modelinfo->getstudiomodel(ent->getmodel());
//		if (!studiomodel) continue; //if cant get the model
//
//		if (backtrack->records[i].empty()) continue; //if the record is empty...
//
//		//general hitboxes
//		int hitboxes[] = { hitbox_head, hitbox_chest};
//		for (auto hitbox : hitboxes)
//		{
//			mstudiobbox_t* studiobox = studiomodel->gethitboxset(0)->gethitbox(hitbox);
//			if (!studiobox) continue;	//if cant get the hitbox...
//
//			vec min = studiobox->bbmin.transform(backtrack->records[i].front().matrix[studiobox->bone]);
//			vec max = studiobox->bbmax.transform(backtrack->records[i].front().matrix[studiobox->bone]);
//			vec loc = (max + min) / 2.f; //just getting middle of bone cuz not rage...
//
//			if (!autowall->isvisible(loc, ent)) //if not visible
//				continue;
//
//			vec angle = calculateangle(loc);
//			float crossdist = crosshairdist(angle);
//			if (crossdist > 30) //if not in fov
//				continue;
//
//			bool inpriority = hitbox == hitbox_head;
//			
//			if (crossdist < bestcrossdist && !inpriority)
//			{
//				bestcrossdist = crossdist;
//				bestangle = angle;
//				validtargetfound = true;
//			}
//			else if(inpriority)
//			{
//				bestcrossdist *= -1;
//				bestangle = angle;
//				validtargetfound = true;
//			}
//		}
//		
//	}
//
//	bestangle -= (g::localplayer->getaimpunchangle() * 2);
//	smooth(bestangle);
//
//	if (validtargetfound)
//	{
//		g::cmd->viewangles = bestangle;
//	}
//		
//} 

void Aimbot::Smooth(Vec& Angle)
{
	bool FastToSlow = false;
	bool Constant = true;
	bool SlowToFast = false;
	float SmoothYaw = 50.f / 100.f;
	float SmoothPitch = 50.f / 100.f;

	G::StartAngle.NormalizeAngle();
	Angle.NormalizeAngle();
	Vec Delta = G::StartAngle - Angle;

	Delta.NormalizeAngle();

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

	Angle = G::StartAngle + Delta;
	Angle.NormalizeAngle();
}

void Aimbot::Run()
{
	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

}

//rage functions
//void Aimbot::Rage()
//{
//	if (!G::Localplayer)
//		return;
//
//	if (!G::LocalPlayerAlive)
//		return;
//
//	if (!G::Localplayer->CanShoot())
//		return;
//
//	if (!G::Localplayer->GetAmmo())
//		return;
//
//
//	//get closest Entity
//	int BestIndex = -1;
//	float BestCrossDist = FLT_MAX;
//	for (int i = 1; i < 65; i++) {
//		Entity* Ent = I::entitylist->GetClientEntity(i);
//		if (!Ent) //if nullptr
//			continue;
//		
//		static player_info_t info;
//		if (!I::engine->GetPlayerInfo(i, &info)) //if not player
//			continue;
//		
//		if (G::Localplayer == Ent) //if localplayer
//			continue;
//		
//		if (i == I::engine->GetLocalPlayer()) //if localplayer
//			continue;
//		
//		if (Ent->GetHealth() <= 0) //if health isn't greater than 0
//			continue;
//		
//		if (Ent->GetTeam() == G::Localplayer->GetTeam()) //if on local players team
//			continue;
//		
//		if (Ent->IsDormant()) //if csgo servers have not sent us updated information about this entity
//			continue;
//
//		Vec		Location	= Ent->GetBone(HITBOX_HEAD);
//		QAngle	Angle		= aimbot->CalculateAngle(Location);
//		float	CrossDist	= aimbot->CrosshairDist(Angle);
//		if (CrossDist < BestCrossDist)
//		{
//			BestCrossDist = CrossDist;
//			BestIndex = i;
//		}
//	}
//
//	//if a valid target hasn't been found
//	if (BestIndex == -1)
//		return;
//
//	Entity* Ent = I::entitylist->GetClientEntity(BestIndex);
//	if (!Ent) return; //if proper entity
//	
//	//absolute unit of a shit code
//	Entity* Weapon = G::Localplayer->GetActiveWeapon();
//	WeaponData* WeapData = Weapon->GetWeaponData();
//
//	QAngle Angle;
//	bool ValidFound = false;
//
//	std::vector<Hitboxes> hitboxes = { HITBOX_HEAD };//HITBOX_PELVIS, HITBOX_STOMACH, HITBOX_LOWER_CHEST, HITBOX_CHEST, HITBOX_UPPER_CHEST, HITBOX_RIGHT_THIGH, HITBOX_LEFT_THIGH, HITBOX_HEAD };
//	int HitGroup = 0;
//
//	for (auto HBOX : hitboxes)
//	{
//		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
//		if (!StudioModel) continue; //if cant get the model
//
//		//if(!(tick.Value > 0)) continue; //if not onshot...
//
//			//GOIN FOR HEAD BB
//		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HBOX);
//		if (!StudioBox) continue;	//if cant get the hitbox...
//
//		if (HBOX == HITBOX_HEAD)
//			HitGroup = HITGROUP_HEAD;
//		else if (HBOX == HITBOX_PELVIS || HBOX == HITBOX_STOMACH)
//			HitGroup = HITGROUP_STOMACH;
//		else if (HBOX == HITBOX_LOWER_CHEST || HBOX == HITBOX_CHEST || HBOX == HITBOX_UPPER_CHEST)
//			HitGroup = HITGROUP_CHEST;
//		else if (HBOX == HITBOX_RIGHT_THIGH)
//			HitGroup = HITGROUP_RIGHTLEG;
//		else if (HBOX == HITBOX_LEFT_THIGH)
//			HitGroup = HITGROUP_LEFTLEG;
//
//		Vec loc = Ent->GetBone(HBOX);
//
//		if (!autowall->CanScanBacktrack(Ent, loc, WeapData, 1, true, HitGroup))
//			continue;
//
//		Angle = aimbot->CalculateAngle(loc);
//		ValidFound = true;
//	}
//	
//	if (!ValidFound) return;
//
//	//HIDE SHOTS
//	*G::pSendPacket = true;
//
//	//actual shit
//	Angle -= (G::Localplayer->GetAimPunchAngle() * 2);
//	G::cmd->viewangles = Angle;
//	G::cmd->buttons |= IN_ATTACK;
//}

