#include "../../Include.hpp"
#include <bitset>

Aimbot* aimbot = new Aimbot();

//general help functions
Vec Aimbot::CalculateAngle(Vec Target)
{
	Vec EyePos = G::LocalPlayer->GetEyePos();//starting head position
	return CalculateAngle(EyePos, Target);
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
	cur = G::cmd->viewangles;
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

void Aimbot::Smooth(Vec& Angle)
{
	int State = smooth_method;
	if (State == 0)
		return;

	bool FastToSlow = (State == 2);
	bool Constant = (State == 3);
	bool SlowToFast = (State == 1);
	float SmoothYaw = smooth_amount;
	float SmoothPitch = smooth_amount;

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

int Aimbot::GetHitGroup(int Hitbox)
{
	switch (Hitbox)
	{
	case HITBOX_CHEST:
	case HITBOX_UPPER_CHEST:
	case HITBOX_LOWER_CHEST:
		return HITGROUP_CHEST;
	case HITBOX_LEFT_THIGH:
	case HITBOX_LEFT_CALF:
	case HITBOX_LEFT_FOOT:
		return HITGROUP_LEFTLEG;
	case HITBOX_RIGHT_THIGH:
	case HITBOX_RIGHT_CALF:
	case HITBOX_RIGHT_FOOT:
		return HITGROUP_RIGHTLEG;
	case HITBOX_RIGHT_HAND:
	case HITBOX_RIGHT_FOREARM:
	case HITBOX_RIGHT_UPPER_ARM:
		return HITGROUP_RIGHTARM;
	case HITBOX_LEFT_HAND:
	case HITBOX_LEFT_UPPER_ARM:
	case HITBOX_LEFT_FOREARM:
		return HITGROUP_LEFTARM;
	case HITBOX_HEAD:
	case HITBOX_NECK:
		return HITGROUP_HEAD;
	case HITBOX_STOMACH:
		return HITGROUP_STOMACH;
	case HITBOX_PELVIS:
		return HITGROUP_GENERIC;
	}
	return 0;
}

void Aimbot::GetPriorityHitbox(int gun)
{
	int selection = 0;
	switch (gun)
	{
	case 1:
		selection = Config::GetState("aimbot-pistol-hitbox-priority");
		break;
	case 2:
		selection = Config::GetState("aimbot-smg-hitbox-priority");
		break;
	case 3:
		selection = Config::GetState("aimbot-heavy-hitbox-priority");
		break;
	case 4:
		selection = Config::GetState("aimbot-scout-hitbox-priority");
		break;
	case 5:
		selection = Config::GetState("aimbot-awp-hitbox-priority");
		break;
	case 6:
		selection = Config::GetState("aimbot-auto-hitbox-priority");
		break;
	case 7:
		selection = Config::GetState("aimbot-rifle-hitbox-priority");
		break;
	default:
		break;
	}

	priority.clear();
	priority.resize(0);
	switch (selection)
	{
	case 0:
		priority.push_back(HITBOX_HEAD);
		break;
	case 1:
		priority.push_back(HITBOX_NECK);
		break;
	case 2:
		priority.push_back(HITBOX_UPPER_CHEST);
		break;
	case 3:
		priority.push_back(HITBOX_LOWER_CHEST);
		break;
	case 4:
		priority.push_back(HITBOX_STOMACH);
		break;
	case 5:
		priority.push_back(HITBOX_PELVIS);
		break;
	case 6:
		priority.push_back(HITBOX_RIGHT_UPPER_ARM);
		priority.push_back(HITBOX_LEFT_UPPER_ARM);
		break;
	case 7:
		priority.push_back(HITBOX_RIGHT_FOREARM);
		priority.push_back(HITBOX_LEFT_FOREARM);
		priority.push_back(HITBOX_RIGHT_HAND);
		priority.push_back(HITBOX_LEFT_HAND);
		break;
	case 8:
		priority.push_back(HITBOX_RIGHT_THIGH);
		priority.push_back(HITBOX_LEFT_THIGH);
		break;
	case 9:
		priority.push_back(HITBOX_RIGHT_CALF);
		priority.push_back(HITBOX_LEFT_CALF);
		break;
	case 10:
		priority.push_back(HITBOX_RIGHT_FOOT);
		priority.push_back(HITBOX_LEFT_FOOT);
		break;
	default:
		break;
	}
	
}

void Aimbot::GetHitboxes(int gun)
{
	uint16_t selection = 0;
	switch (gun)
	{
	case 1:
		selection = Config::GetSelections("aimbot-pistol-hitbox");
		break;
	case 2: 
		selection = Config::GetSelections("aimbot-smg-hitbox");
		break;
	case 3: 
		selection = Config::GetSelections("aimbot-heavy-hitbox");
		break;
	case 4:
		selection = Config::GetSelections("aimbot-scout-hitbox");
		break;
	case 5:
		selection = Config::GetSelections("aimbot-awp-hitbox");
		break;
	case 6:
		selection = Config::GetSelections("aimbot-auto-hitbox");
		break;
	case 7:
		selection = Config::GetSelections("aimbot-rifle-hitbox");
		break;
	default:
		break;
	}
	hboxes.clear();
	hboxes.resize(0);
	if (selection & (1 << 0))			// head
		hboxes.push_back(HITBOX_HEAD);
	if (selection & (1 << 1))			// neck
		hboxes.push_back(HITBOX_NECK);
	if (selection & (1 << 2))			// upper chest
		hboxes.push_back(HITBOX_UPPER_CHEST);
	if (selection & (1 << 3))			// lower chest
		hboxes.push_back(HITBOX_LOWER_CHEST);
	if (selection & (1 << 4))			// stomach
		hboxes.push_back(HITBOX_STOMACH);
	if (selection & (1 << 5))			// pelvis
		hboxes.push_back(HITBOX_PELVIS);
	if (selection & (1 << 6))			// Upper arms
	{
		hboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
		hboxes.push_back(HITBOX_LEFT_UPPER_ARM);
	}
	if (selection & (1 << 7))			// Lower arms
	{
		hboxes.push_back(HITBOX_RIGHT_FOREARM);
		hboxes.push_back(HITBOX_LEFT_FOREARM);
		hboxes.push_back(HITBOX_RIGHT_HAND);
		hboxes.push_back(HITBOX_LEFT_HAND);
	}
	if (selection & (1 << 8))			// Upper legs
	{
		hboxes.push_back(HITBOX_RIGHT_THIGH);
		hboxes.push_back(HITBOX_LEFT_THIGH);
	}
	if (selection & (1 << 9))			// Lower legs
	{
		hboxes.push_back(HITBOX_RIGHT_CALF);
		hboxes.push_back(HITBOX_LEFT_CALF);
	}
	if (selection & (1 << 10))			// TOES
	{
		hboxes.push_back(HITBOX_RIGHT_FOOT);
		hboxes.push_back(HITBOX_LEFT_FOOT);
	}
		
}

void Aimbot::VisibleScan(int& BestIndex, int& BestDamage, Vec& BestAimPoint)
{
	// Check visible points
	for (auto a : G::EntList)
	{
		if (a.index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(a.entity)) // entity DOES NOT exist
			continue;

		if (!a.player) // entity is NOT player
			continue;

		if (!(a.health > 0)) // entity is NOT alive
			continue;

		if (a.team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (a.dormant)	// Entity is dormant
			continue;

		if (!backtrack->Valid(a.lastSimTime)) // if not valid simtime
			continue;

		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(a.model);
		if (!StudioModel) continue; //if cant get the model

		// Left Hitboxes
		for (auto HITBOX : this->hboxes)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
			if (!StudioBox) continue;	//if cant get the hitbox...
			int HitGroup = GetHitGroup(HITBOX);

			Vec min = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec mid = (max + min) / 2;

			float radius = StudioBox->m_flRadius * hitchance;
			Vec left = a.entity->GetLeft(mid, radius, G::LocalPlayer);

			// no need to "visible autowall" if not visible
			if (!autowall->IsVisible(left, a.entity))
				continue;

			// no need to "visible autowall" if not visible
			bool visible = true;
			float dam = autowall->GetDamage(a.entity, left, true, visible);
			
			// if autowall found it hidden, continue
			if (!visible)
				continue;

			if (dam > mindamage_visible && dam > BestDamage)
			{
				BestDamage = dam;
				BestIndex = a.index;
				BestAimPoint = left;
			}
		}
		// Right Hitboxes
		for (auto HITBOX : this->hboxes)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
			if (!StudioBox) continue;	//if cant get the hitbox...
			int HitGroup = GetHitGroup(HITBOX);

			Vec min = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec mid = (max + min) / 2;

			float radius = StudioBox->m_flRadius * hitchance;
			Vec right = a.entity->GetRight(mid, radius, G::LocalPlayer);

			// no need to "visible autowall" if not visible
			if (!autowall->IsVisible(right, a.entity))
				continue;

			// no need to "visible autowall" if not visible
			bool visible = true;
			float dam = autowall->GetDamage(a.entity, right, true, visible);

			// if autowall found it hidden, continue
			if (!visible)
				continue;

			if (dam > mindamage_visible && dam > BestDamage)
			{
				BestDamage = dam;
				BestIndex = a.index;
				BestAimPoint = right;
			}
		}
	}
}

void Aimbot::AutowallScan(int& BestIndex, int& BestDamage, Vec& BestAimPoint)
{
	// Check hidden points
	for (auto a : G::EntList)
	{
		if (a.index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(a.entity)) // entity DOES NOT exist
			continue;

		if (!a.player) // entity is NOT player
			continue;

		if (!(a.health > 0)) // entity is NOT alive
			continue;

		if (a.team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (a.dormant)	// Entity is dormant
			continue;

		if (!backtrack->Valid(a.lastSimTime)) // if not valid simtime
			continue;

		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(a.model);
		if (!StudioModel) continue; //if cant get the model

		// Left Hitboxes
		for (auto HITBOX : this->hboxes)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
			if (!StudioBox) continue;	//if cant get the hitbox...
			int HitGroup = GetHitGroup(HITBOX);

			Vec min = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec mid = (max + min) / 2;

			float radius = StudioBox->m_flRadius * hitchance;
			Vec left = a.entity->GetLeft(mid, radius, G::LocalPlayer);

			// no need to  "autowall" if visible
			if (autowall->IsVisible(left, a.entity))
				continue;

			// no need to "visible autowall" if not visible
			bool visible = false;
			float dam = autowall->GetDamage(a.entity, left, true, visible);

			// if autowall visible, continue
			if (visible)
				continue;

			if (dam > mindamage_hidden && dam > BestDamage)
			{
				BestDamage = dam;
				BestIndex = a.index;
				BestAimPoint = left;
			}
		}
		// Right Hitboxes
		for (auto HITBOX : this->hboxes)
		{
			mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
			if (!StudioBox) continue;	//if cant get the hitbox...
			int HitGroup = GetHitGroup(HITBOX);

			Vec min = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec max = StudioBox->bbmin.Transform(a.Matrix[StudioBox->bone]);
			Vec mid = (max + min) / 2;

			float radius = StudioBox->m_flRadius * hitchance;
			Vec right = a.entity->GetRight(mid, radius, G::LocalPlayer);

			// no need to  "autowall" if visible
			if (autowall->IsVisible(right, a.entity))
				continue;

			// no need to "visible autowall" if not visible
			bool visible = false;
			float dam = autowall->GetDamage(a.entity, right, true, visible);

			// if autowall visible, continue
			if (visible)
				continue;

			if (dam > mindamage_hidden && dam > BestDamage)
			{
				BestDamage = dam;
				BestIndex = a.index;
				BestAimPoint = right;
			}
		}
	}
}

void Aimbot::Run()
{
	if (!Config::GetBool("enable-aimbot")) return;

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (!G::LocalPlayer->CanShoot()) return;

	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return;

	int weaponID = (int)weapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);

	if (WeaponClass == 35)		//pistol
	{
		smooth_method		= Config::GetState("aimbot-pistol-smoothing-method");
		smooth_amount		= Config::GetFloat("aimbot-pistol-smoothing-amount");
		fov					= Config::GetFloat("aimbot-pistol-fov");
		priority_hitbox		= Config::GetState("aimbot-pistol-hitbox-priority");
		mindamage_visible	= Config::GetFloat("aimbot-pistol-mindamage-visible");
		mindamage_hidden	= Config::GetFloat("aimbot-pistol-mindamage-hidden");
		hitchance			= Config::GetFloat("aimbot-pistol-hitchance");
		GetHitboxes(1);
		GetPriorityHitbox(1);
	}
	else if(WeaponClass == 37)	//smg
	{
		smooth_method		= Config::GetState("aimbot-smg-smoothing-method");
		smooth_amount		= Config::GetFloat("aimbot-smg-smoothing-amount");
		fov					= Config::GetFloat("aimbot-smg-fov");
		priority_hitbox		= Config::GetState("aimbot-smg-hitbox-priority");
		mindamage_visible	= Config::GetFloat("aimbot-smg-mindamage-visible");
		mindamage_hidden	= Config::GetFloat("aimbot-smg-mindamage-hidden");
		hitchance			= Config::GetFloat("aimbot-smg-hitchance");
		GetHitboxes(2);
		GetPriorityHitbox(2);
	}
	else if(WeaponClass == 36)	//heavy
	{
		smooth_method		= Config::GetState("aimbot-heavy-smoothing-method");
		smooth_amount		= Config::GetFloat("aimbot-heavy-smoothing-amount");
		fov					= Config::GetFloat("aimbot-heavy-fov");
		priority_hitbox		= Config::GetState("aimbot-heavy-hitbox-priority");
		mindamage_visible	= Config::GetFloat("aimbot-heavy-mindamage-visible");
		mindamage_hidden	= Config::GetFloat("aimbot-heavy-mindamage-hidden");
		hitchance			= Config::GetFloat("aimbot-heavy-hitchance");
		GetHitboxes(3);
		GetPriorityHitbox(3);
	}
	else if (WeaponClass == 38)	//rifles (includes scouts, awps, autos)
	{
		if (weaponID == 40) //scout
		{
			smooth_method		= Config::GetState("aimbot-scout-smoothing-method");
			smooth_amount		= Config::GetFloat("aimbot-scout-smoothing-amount");
			fov					= Config::GetFloat("aimbot-scout-fov");
			priority_hitbox		= Config::GetState("aimbot-scout-hitbox-priority");
			mindamage_visible	= Config::GetFloat("aimbot-scout-mindamage-visible");
			mindamage_hidden	= Config::GetFloat("aimbot-scout-mindamage-hidden");
			hitchance			= Config::GetFloat("aimbot-scout-hitchance");
			GetHitboxes(4);
			GetPriorityHitbox(4);
		}
		else if (weaponID == 9) //awp
		{
			smooth_method		= Config::GetState("aimbot-awp-smoothing-method");
			smooth_amount		= Config::GetFloat("aimbot-awp-smoothing-amount");
			fov					= Config::GetFloat("aimbot-awp-fov");
			priority_hitbox		= Config::GetState("aimbot-awp-hitbox-priority");
			mindamage_visible	= Config::GetFloat("aimbot-awp-mindamage-visible");
			mindamage_hidden	= Config::GetFloat("aimbot-awp-mindamage-hidden");
			hitchance			= Config::GetFloat("aimbot-awp-hitchance");
			GetHitboxes(5);
			GetPriorityHitbox(5);
		}
		else if (weaponID == 11 || weaponID == 38) //autos
		{
			smooth_method		= Config::GetState("aimbot-auto-smoothing-method");
			smooth_amount		= Config::GetFloat("aimbot-auto-smoothing-amount");
			fov					= Config::GetFloat("aimbot-auto-fov");
			priority_hitbox		= Config::GetState("aimbot-auto-hitbox-priority");
			mindamage_visible	= Config::GetFloat("aimbot-auto-mindamage-visible");
			mindamage_hidden	= Config::GetFloat("aimbot-auto-mindamage-hidden");
			hitchance			= Config::GetFloat("aimbot-auto-hitchance");
			GetHitboxes(6);
			GetPriorityHitbox(6);
		}
		else //rifles
		{
			smooth_method		= Config::GetState("aimbot-rifle-smoothing-method");
			smooth_amount		= Config::GetFloat("aimbot-rifle-smoothing-amount");
			fov					= Config::GetFloat("aimbot-rifle-fov");
			priority_hitbox		= Config::GetState("aimbot-rifle-hitbox-priority");
			mindamage_visible	= Config::GetFloat("aimbot-rifle-mindamage-visible");
			mindamage_hidden	= Config::GetFloat("aimbot-rifle-mindamage-hidden");
			hitchance			= Config::GetFloat("aimbot-rifle-hitchance");
			GetHitboxes(7);
			GetPriorityHitbox(7);
		}
	}
	else //grenades tasers and knifes
		return;

	//Set up percentages to proper vals
	hitchance /= 100.f;
	hitchance = 1 - hitchance;

	// Get Visible Entity with best damage
	int BestVisIndex = -1;
	int BestVisDamage = -1;
	Vec BestVisAimpoint;

	VisibleScan(BestVisIndex, BestVisDamage, BestVisAimpoint);

	// Get Hidden Entity with best damage
	int BestHiddenIndex = -1;
	int BestHiddenDamage = -1;
	Vec BestHiddenAimpoint;

	AutowallScan(BestHiddenIndex, BestHiddenDamage, BestHiddenAimpoint);

	if ((BestVisDamage <= 0) && (BestHiddenDamage <= 0)) return;

	// Calculate angle to point
	QAngle Angle = CalculateAngle(BestHiddenAimpoint);
	if(BestVisDamage > BestHiddenDamage)
		Angle = CalculateAngle(BestVisAimpoint);
	Angle -= (G::LocalPlayer->GetAimPunchAngle() * 2);



	
	// Smooth angle if needed
	QAngle ang = Angle;
	Smooth(Angle);

	// NOT Silent aim
	if (!Config::GetBool("enable-silentaim"))
		I::engine->SetViewAngles(Angle);
	G::cmd->viewangles = Angle;

	// Check if close enough to shoot
	if (ang.VecLength2D() - Angle.VecLength2D() > 1.f)
		return;

	// shoot
	if(Config::GetBool("aimbot-autoshoot"))
		G::cmd->buttons |= IN_ATTACK;
}

/*
* smoothing method
* smoothing amount
* FOV
*/