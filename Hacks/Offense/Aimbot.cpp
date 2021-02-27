#include "../../Include.hpp"

Aimbot* aimbot = new Aimbot();

float Aimbot::CalculateHitchance(QAngle vangles, const Vec& point, Entity* player, Hitboxes hbox)
{
	auto weapon = G::LocalPlayerWeapon;
	if (!weapon)
		return 0.f;

	//const auto hitchance_cfg = get_config(weapon)->hitchance->get<float>();

	Vec forward, right, up;
	Vec eyepos = G::LocalPlayer->GetEyePos();
	const auto spread = weapon->GetSpread();
	const auto inaccuracy = weapon->GetInaccuracy();
	const auto angles = vangles.Clamp();
	AngleVectors(angles, &forward, &right, &up);
	fast_vec_normalize(forward);
	fast_vec_normalize(right);
	fast_vec_normalize(up);
	auto endpoint = point;

	auto hits = 0;
	auto i = 0;

	while (i < 255)
	{
		const auto b = QuickRandom(0.f, 2.0f * 3.1415926535f); // tired of "warning C4305: 'argument': truncation from 'double' to 'float'"
		const auto c = QuickRandom(0.0f, 1.0f);
		const auto d = QuickRandom(0.f, 2.0f * 3.1415926535f); // so i swapped M_PI to this, lol

		const auto spread_val = c * spread;
		const auto inaccuracy_val = c * inaccuracy;

		const Vec v_spread((cos(b) * spread_val) + (cos(d) * inaccuracy_val), (sin(b) * spread_val) + (sin(d) * inaccuracy_val), 0);
		Vec dir;

		dir.x = forward.x + (right.x * v_spread.x) + (up.x * v_spread.y);
		dir.y = forward.y + (right.y * v_spread.x) + (up.y * v_spread.y);
		dir.z = forward.z + (right.z * v_spread.x) + (up.z * v_spread.y);

		fast_vec_normalize(dir);

		QAngle spread_view;
		Vec end;
		vector_angles(dir, spread_view);
		spread_view.NormalizeNoClamp();
		Vec a = angles;
		a -= (spread_view - angles);
		AngleVectors(a, &end);
		fast_vec_normalize(end);

		trace_t Trace;
		const auto trace_end = eyepos + (end * endpoint.Dist(eyepos));
		Ray_t Ray(eyepos, trace_end);
		I::enginetrace->ClipRayToEntity(Ray, MASK_SHOT_HULL | CONTENTS_HITBOX, player, &Trace);

		if (Trace.Entity == player && hbox == Trace.hitbox) // eg. in head
			hits++;

		i++;
	}

	return (hits / 255.f);
}

bool Aimbot::ValidPlayer(Player player)
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

void Aimbot::GetClosestEntity(int& RecordUserID, float& Distance)
{
	float CrossEntDist = FLT_MAX;
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		Player player = it->second;
		if (!ValidPlayer(player)) continue;
		Vec Angle = aimbot->CalculateAngle(it->second.EyePos);
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			Distance = CrossDist;
			RecordUserID = it->first;
			CrossEntDist = CrossDist;
		}
	}
}

void Aimbot::Legit()
{
	// Is legitbot enabled
	if (!Config::GetBool("legit-aim-enable")) return;

	// Make sure it's a different tick
	static int tickcount = G::cmd->tick_count;
	if (tickcount == G::cmd->tick_count) return;

	// If the player is pulling out a weapon...
	if (!G::LocalPlayer->CanShoot2()) return;

	// Is the current weapon alright/Get config Values
	if (!UpdateLegitVal()) return;

	// Get Closest Player
	int RecordUserID = -1;
	float Distance = FLT_MAX;
	GetClosestEntity(RecordUserID, Distance);

	// If there are no valid users...
	if (RecordUserID == -1) return;

	// If it isn't within FOV...
	if (Distance > legit.fov) return;

	// Get Closest Hitbox
	bool valid;
	QAngle Ang = GetClosestAng(RecordUserID, valid);
	if (!valid) return;

	// If more than a 1 tap... start doing aimpunch
	if(G::LocalPlayer->GetShotsFired() > 1)
		Ang -= (G::LocalPlayer->GetAimPunchAngle() * 2);

	// We have found a valid angle to target and go towards
	Smooth(Ang);

	I::engine->SetViewAngles(Ang);
}

bool Aimbot::UpdateLegitVal()
{
	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return false;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return false;

	int weaponID = (int)weapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);

	// I could use a switch case for this meh
	if (WeaponClass == 35)		//pistol
	{
		legit.fov = Config::GetFloat("legitaim-pistol-fov");
		legit.smoothing = Config::GetState("legitaim-pistol-smoothing");
		legit.smoothing_amount = Config::GetFloat("legitaim-pistol-smoothing-amount");
		GetLegitHitboxes(0);
		return true;
	}
	else if (WeaponClass == 37)	//smg
	{
		legit.fov = Config::GetFloat("legitaim-smg-fov");
		legit.smoothing = Config::GetState("legitaim-smg-smoothing");
		legit.smoothing_amount = Config::GetFloat("legitaim-smg-smoothing-amount");
		GetLegitHitboxes(1);
		return true;
	}
	else if (WeaponClass == 36)	//heavy
	{
		legit.fov = Config::GetFloat("legitaim-heavy-fov");
		legit.smoothing = Config::GetState("legitaim-heavy-smoothing");
		legit.smoothing_amount = Config::GetFloat("legitaim-heavy-smoothing-amount");
		GetLegitHitboxes(2);
		return true;
	}
	else if (WeaponClass == 38)	//rifles (includes scouts, awps, autos)
	{
		if (weaponID == 40 || weaponID == 9 || weaponID == 11 || weaponID == 38) //snipers
		{
			legit.fov = Config::GetFloat("legitaim-sniper-fov");
			legit.smoothing = Config::GetState("legitaim-sniper-smoothing");
			legit.smoothing_amount = Config::GetFloat("legitaim-sniper-smoothing-amount");
			GetLegitHitboxes(4);
			return true;
		}
		else //rifles
		{
			legit.fov = Config::GetFloat("legitaim-rifle-fov");
			legit.smoothing = Config::GetState("legitaim-rifle-smoothing");
			legit.smoothing_amount = Config::GetFloat("legitaim-rifle-smoothing-amount");
			GetLegitHitboxes(3);
			return true;
		}
	}
	else //grenades tasers and knifes
		return false;
}

void Aimbot::GetLegitHitboxes(int gun)
{
	uint16_t selection = 0;
	legit.hitboxes.clear();
	legit.hitboxes.resize(0);
	switch (gun)
	{
	case 0:
		selection = Config::GetSelections("legitaim-pistol-hitbox");
		break;
	case 1:
		selection = Config::GetSelections("legitaim-smg-hitbox");
		break;
	case 2:
		selection = Config::GetSelections("legitaim-heavy-hitbox");
		break;
	case 3:
		selection = Config::GetSelections("legitaim-rifle-hitbox");
		break;
	case 4:
		selection = Config::GetSelections("legitaim-sniper-hitbox");
		break;
	default:
		return;
		break;
	}
	if (selection & (1 << 0))			// head
		legit.hitboxes.push_back(HITBOX_HEAD);
	if (selection & (1 << 1))			// neck
		legit.hitboxes.push_back(HITBOX_NECK);
	if (selection & (1 << 2))			// upper chest
		legit.hitboxes.push_back(HITBOX_UPPER_CHEST);
	if (selection & (1 << 3))			// lower chest
		legit.hitboxes.push_back(HITBOX_LOWER_CHEST);
	if (selection & (1 << 4))			// stomach
		legit.hitboxes.push_back(HITBOX_STOMACH);
	if (selection & (1 << 5))			// pelvis
		legit.hitboxes.push_back(HITBOX_PELVIS);
	if (selection & (1 << 6))			// Upper arms
	{
		legit.hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
		legit.hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
	}
	if (selection & (1 << 7))			// Lower arms
	{
		legit.hitboxes.push_back(HITBOX_RIGHT_FOREARM);
		legit.hitboxes.push_back(HITBOX_LEFT_FOREARM);
		legit.hitboxes.push_back(HITBOX_RIGHT_HAND);
		legit.hitboxes.push_back(HITBOX_LEFT_HAND);
	}
	if (selection & (1 << 8))			// Upper legs
	{
		legit.hitboxes.push_back(HITBOX_RIGHT_THIGH);
		legit.hitboxes.push_back(HITBOX_LEFT_THIGH);
	}
	if (selection & (1 << 9))			// Lower legs
	{
		legit.hitboxes.push_back(HITBOX_RIGHT_CALF);
		legit.hitboxes.push_back(HITBOX_LEFT_CALF);
	}
	if (selection & (1 << 10))			// TOES
	{
		legit.hitboxes.push_back(HITBOX_RIGHT_FOOT);
		legit.hitboxes.push_back(HITBOX_LEFT_FOOT);
	}
}

QAngle Aimbot::GetClosestAng(int RecordUserID, bool& valid)
{
	float CrossEntDist = FLT_MAX;
	Vec BestAng;
	valid = false;
	
	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(G::EntList[RecordUserID].model);
	if (!StudioModel) return BestAng; //if cant get the model

	for (auto HITBOX : legit.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);

		Vec min = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);
		Vec mid = (max + min) / 2;

		// no need to "visible autowall" if not visible
		bool visible = autowall->IsVisible(mid, G::EntList[RecordUserID].entity);

		// if not visible, continue
		if (!visible)
			continue;

		float dam = autowall->GetDamage(G::EntList[RecordUserID].entity, mid, false);

		Vec Angle = aimbot->CalculateAngle(mid);
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist && dam > 0)
		{
			CrossEntDist = CrossDist;
			valid = true;
			BestAng = Angle;
		}
	}
	BestAng.Normalize();
	return BestAng;
}

void Aimbot::Smooth(QAngle& Ang)
{
	Ang.Normalize();
	G::StartAngle.Normalize();
	switch (legit.smoothing)
	{
	case 1:
		SmoothSlowToFast(Ang);
		break;
	case 2:
		SmoothFastToSlow(Ang);
		break;
	case 3:
		SmoothLinear(Ang);
		break;
	default:
		break;
	}
	return;
}

void Aimbot::SmoothLinear(QAngle& Ang)
{
	QAngle Delta = G::StartAngle - Ang;
	Delta.NormalizeAngle();	
	float MaxSpeed = (legit.smoothing_amount > 1 ? legit.hidden_max : 180.f) * (100.f - legit.smoothing_amount) / 100.f;
	float CurSpeed = Delta.VecLength();
	if (CurSpeed > MaxSpeed)
	{
		Delta *= (MaxSpeed / CurSpeed);
	}
	Ang = G::StartAngle + Delta;
}

void Aimbot::SmoothSlowToFast(QAngle& Ang)
{
	QAngle Delta = G::StartAngle - Ang;
	Delta.NormalizeAngle();
	float CurSpeed = Delta.VecLength();

	float coeff = 1 / CurSpeed;
	coeff = powf(coeff, 2.f) * (100.f - legit.smoothing_amount) / 10.f;
	coeff = min(0.9f, coeff);
	Delta *= coeff;

	Ang = G::StartAngle + Delta;
}

void Aimbot::SmoothFastToSlow(QAngle& Ang)
{
	QAngle Delta = G::StartAngle - Ang;
	Delta.NormalizeAngle();
	Delta *= (100.f - legit.smoothing_amount) / 100.f;
	Ang = G::StartAngle + Delta;
}

void Aimbot::Rage()
{
	// Make sure it's a different tick
	static int tickcount = G::cmd->tick_count;
	if (tickcount == G::cmd->tick_count) return;
	tickcount = G::cmd->tick_count;

	// Is ragebot enabled
	if (!Config::GetBool("rage-aim-enable")) return;

	// If the player can shoot a weapon... 
	if (!G::LocalPlayer->CanShoot()) return;

	// Is the current weapon alright/Get config Values
	if (!UpdateRageVal()) return;

	// If we are lagging on peak...
	if (fakelag->LaggingOnPeak) return;

	// If succeed with our onshot...
	if (TryOnShot()) return;

	// if force on shot, for testing ill be always doing this
	

	/*
	The IDEA is we get the order of entitys in closest to greatest from crosshair.
	We scan from there onward, so if someone is behind us itll be the last person
	we scan, but if we can hit them at least we will try. If we find someone closest
	to our crosshair that we can hit, then it will target that instead, as well as
	try and backtrack them if possible, as that will decrease the amount of autowall 
	parses we have to do. This (in the future) might be able to lead to half/full scans. 

	For now, we are checking the person closest to our crosshair (no backtrack!), then 
	checking everyone else. Not suuuper efficient, but it will do XD.
	*/

	/*
	Another idea is to parse the entites and immediately try and shoot once we find one 
	that fits the conditions to shoot. Sorting the enemies by distance/FOV would be an
	effective way of determining who we want to shoot
	*/

	int RecordID = -1;
	float Dist = FLT_MAX;
	GetClosestEntity(RecordID, Dist);

	// Aimpoint
	Vec AimPoint;
	// Clear up the playerscanlist
	PlayersScanned.clear(); 
	PlayersScanned.resize(0);
	PlayersScanned.push_back(RecordID);
	// if the recordID is proper, and the closest player is hittable
	// under the users standerds (hitchance etc...)
	if (RecordID != -1 && ScanPlayer(RecordID, AimPoint))
	{
		// Get Angle and do recoil
		QAngle Angle = CalculateAngle(AimPoint);
		Angle -= (G::LocalPlayer->GetAimPunchAngle() * 2);

		// If not silent aim, adjust angles, otherwise do silent
		if (!Config::GetBool("rage-aim-silent"))
			I::engine->SetViewAngles(Angle);
		G::cmd->viewangles = Angle;

		// If autoshoot.. FIRE!
		if (Config::GetBool("rage-aim-autoshoot"))
			G::cmd->buttons |= IN_ATTACK;

		if (G::pSendPacket)
			*G::pSendPacket = true;
		return;
	}
	// Otherwise do a less detailed scan of all of the other players
	else
	{
		H::console.clear();
		H::console.resize(0);
		int max = 2;
		for (auto &a : G::EntList)
		{	
			if (max <= 0) // only allow 2 additional scans (any more and lots of lag maen
				return;
			max--;
			
			H::console.push_back(std::to_string(a.first));
			Dist = FLT_MAX;
			RecordID = -1;
			GetClosestEntityNotScanned(RecordID, Dist);
			if (RecordID != -1 && ScanPlayerMin(RecordID, AimPoint))
			{
				// Get Angle and do recoil
				QAngle Angle = CalculateAngle(AimPoint);
				Angle -= (G::LocalPlayer->GetAimPunchAngle() * 2);

				// If not silent aim, adjust angles, otherwise do silent
				if (!Config::GetBool("rage-aim-silent"))
					I::engine->SetViewAngles(Angle);
				G::cmd->viewangles = Angle;

				// If autoshoot.. FIRE!
				if (Config::GetBool("rage-aim-autoshoot"))
					G::cmd->buttons |= IN_ATTACK;

				if (G::pSendPacket)
					*G::pSendPacket = true;
				return;
			}
		}
		return;
	}
}

bool Aimbot::UpdateRageVal()
{
	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return false;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return false;

	int weaponID = (int)weapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);

	// I could use a switch case for this meh
	if (WeaponClass == 35)		//pistol
	{
		rage.vis_mindam = Config::GetFloat("rageaim-pistol-mindamage-visible");
		rage.hid_mindam = Config::GetFloat("rageaim-pistol-mindamage-hidden");
		rage.hitchance = Config::GetFloat("rageaim-pistol-hitchance") / 100.f;
		rage.FireIfLethal = Config::GetBool("rageaim-pistol-fireiflethal");
		GetRageHitboxes(0);
		return true;
	}
	else if (WeaponClass == 37)	//smg
	{
		rage.vis_mindam = Config::GetFloat("rageaim-smg-mindamage-visible");
		rage.hid_mindam = Config::GetFloat("rageaim-smg-mindamage-hidden");
		rage.hitchance = Config::GetFloat("rageaim-smg-hitchance") / 100.f;
		rage.FireIfLethal = Config::GetBool("rageaim-smg-fireiflethal");
		GetRageHitboxes(1);
		return true;
	}
	else if (WeaponClass == 36)	//heavy
	{
		rage.vis_mindam = Config::GetFloat("rageaim-heavy-mindamage-visible");
		rage.hid_mindam = Config::GetFloat("rageaim-heavy-mindamage-hidden");
		rage.hitchance = Config::GetFloat("rageaim-heavy-hitchance") / 100.f;
		rage.FireIfLethal = Config::GetBool("rageaim-heavy-fireiflethal");
		GetRageHitboxes(2);
		return true;
	}
	else if (WeaponClass == 38)	//rifles (includes scouts, awps, autos)
	{
		if (weaponID == 40) //scout
		{
			rage.vis_mindam = Config::GetFloat("rageaim-scout-mindamage-visible");
			rage.hid_mindam = Config::GetFloat("rageaim-scout-mindamage-hidden");
			rage.hitchance = Config::GetFloat("rageaim-scout-hitchance") / 100.f;
			rage.FireIfLethal = Config::GetBool("rageaim-scout-fireiflethal");
			GetRageHitboxes(3);
			return true;
		}
		else if (weaponID == 9) //awp
		{
			rage.vis_mindam = Config::GetFloat("rageaim-awp-mindamage-visible");
			rage.hid_mindam = Config::GetFloat("rageaim-awp-mindamage-hidden");
			rage.hitchance = Config::GetFloat("rageaim-awp-hitchance") / 100.f;
			rage.FireIfLethal = Config::GetBool("rageaim-awp-fireiflethal");
			GetRageHitboxes(4);
			return true;
		}
		else if (weaponID == 11 || weaponID == 38) //autos
		{
			rage.vis_mindam = Config::GetFloat("rageaim-auto-mindamage-visible");
			rage.hid_mindam = Config::GetFloat("rageaim-auto-mindamage-hidden");
			rage.hitchance = Config::GetFloat("rageaim-auto-hitchance") / 100.f;
			rage.FireIfLethal = Config::GetBool("rageaim-auto-fireiflethal");
			GetRageHitboxes(5);
			return true;
		}
		else //rifles
		{
			rage.vis_mindam = Config::GetFloat("rageaim-rifle-mindamage-visible");
			rage.hid_mindam = Config::GetFloat("rageaim-rifle-mindamage-hidden");
			rage.hitchance = Config::GetFloat("rageaim-rifle-hitchance") / 100.f;
			rage.FireIfLethal = Config::GetBool("rageaim-rifle-fireiflethal");
			GetRageHitboxes(6);
			return true;
		}
	}
	else //grenades tasers and knifes
		return false;
}

bool Aimbot::TryOnShot()
{
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		if (!ValidPlayer(it->second)) continue;
		
		// if within good bt onshot range... 
		// (should be 200.f but reducing for reliability to 150.f)
		if (I::globalvars->m_curTime - it->second.LastShotTime < 150.f)
		{
			int shot_tick = TimeToTicks(it->second.LastShotTime);
			// find the matrix
			
			for (auto& a : it->second.BacktrackRecords)
			{
				// WE HAVE FOUND A GOOD ONE!
				if (TimeToTicks(a.SimulationTime) == shot_tick)
				{
					Vec head = a.Bone(8);
					// if we can't hit it...
					if (!autowall->CanScanBacktrack(it->second.entity, head, G::LocalPlayerWeaponData, 1, true, HITGROUP_HEAD))
						goto SKIP; // go on to another entity

					QAngle Angle = CalculateAngle(head);
					Angle -= (G::LocalPlayer->GetAimPunchAngle() * 2);

					// If not silent aim, adjust angles, otherwise do silent
					if (!Config::GetBool("rage-aim-silent"))
						I::engine->SetViewAngles(Angle);
					G::cmd->viewangles = Angle;

					// If autoshoot.. FIRE!
					if (Config::GetBool("rage-aim-autoshoot"))
						G::cmd->buttons |= IN_ATTACK;

					if (G::pSendPacket)
						*G::pSendPacket = true;

					G::cmd->tick_count = shot_tick;
					return true;
				}
			}	
		}
		SKIP:
			continue;
	}
	return false;
}

void Aimbot::GetRageHitboxes(int gun)
{
	uint16_t selection = 0;
	rage.hitboxes.clear();
	rage.hitboxes.resize(0);
	switch (gun)
	{
	case 0:
		selection = Config::GetSelections("rageaim-pistol-hitbox");
		break;
	case 1:
		selection = Config::GetSelections("rageaim-smg-hitbox");
		break;
	case 2:
		selection = Config::GetSelections("rageaim-heavy-hitbox");
		break;
	case 3:
		selection = Config::GetSelections("rageaim-scout-hitbox");
		break;
	case 4:
		selection = Config::GetSelections("rageaim-awp-hitbox");
		break;
	case 5:
		selection = Config::GetSelections("rageaim-auto-hitbox");
		break;
	case 6:
		selection = Config::GetSelections("rageaim-rifle-hitbox");
		break;
	default:
		return;
		break;
	}
	if (selection & (1 << 0))			// head
		rage.hitboxes.push_back(HITBOX_HEAD);
	if (selection & (1 << 1))			// neck
		rage.hitboxes.push_back(HITBOX_NECK);
	if (selection & (1 << 2))			// upper chest
		rage.hitboxes.push_back(HITBOX_UPPER_CHEST);
	if (selection & (1 << 3))			// lower chest
		rage.hitboxes.push_back(HITBOX_LOWER_CHEST);
	if (selection & (1 << 4))			// stomach
		rage.hitboxes.push_back(HITBOX_STOMACH);
	if (selection & (1 << 5))			// pelvis
		rage.hitboxes.push_back(HITBOX_PELVIS);
	if (selection & (1 << 6))			// Upper arms
	{
		rage.hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
		rage.hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
	}
	if (selection & (1 << 7))			// Lower arms
	{
		rage.hitboxes.push_back(HITBOX_RIGHT_FOREARM);
		rage.hitboxes.push_back(HITBOX_LEFT_FOREARM);
		rage.hitboxes.push_back(HITBOX_RIGHT_HAND);
		rage.hitboxes.push_back(HITBOX_LEFT_HAND);
	}
	if (selection & (1 << 8))			// Upper legs
	{
		rage.hitboxes.push_back(HITBOX_RIGHT_THIGH);
		rage.hitboxes.push_back(HITBOX_LEFT_THIGH);
	}
	if (selection & (1 << 9))			// Lower legs
	{
		rage.hitboxes.push_back(HITBOX_RIGHT_CALF);
		rage.hitboxes.push_back(HITBOX_LEFT_CALF);
	}
	if (selection & (1 << 10))			// TOES
	{
		rage.hitboxes.push_back(HITBOX_RIGHT_FOOT);
		rage.hitboxes.push_back(HITBOX_LEFT_FOOT);
	}
}

void Aimbot::GetClosestEntityNotScanned(int& RecordUserID, float& Distance)
{
	float CrossEntDist = FLT_MAX;
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		bool skip = false;
		for (auto a : PlayersScanned)
		{
			if (a == it->first)
				skip = true;
		}
		if (skip) continue;
		Player player = it->second;
		if (!ValidPlayer(player)) continue;
		Vec Angle = aimbot->CalculateAngle(it->second.EyePos);
		float CrossDist = aimbot->CrosshairDist(Angle);
		if (CrossDist < CrossEntDist)
		{
			Distance = CrossDist;
			RecordUserID = it->first;
			CrossEntDist = CrossDist;
		}
	}
}

/*
In this case we are doing a full scan to try and grab the points in which damage possible. This 
is quite laggy. It return once it has found points that are good. It's like multipoint but only
for closest entity
*/
bool Aimbot::ScanPlayer(int RecordUserID, Vec& Point)
{
	if (G::EntList[RecordUserID].index == G::LocalPlayerIndex) // entity is Localplayer
		return false;

	if (!(G::EntList[RecordUserID].entity)) // entity DOES NOT exist
		return false;

	if (!(G::EntList[RecordUserID].health > 0)) // entity is NOT alive
		return false;

	if (G::EntList[RecordUserID].team == G::LocalPlayerTeam) // Entity is on same team
		return false;

	if (G::EntList[RecordUserID].dormant)	// Entity is dormant
		return false;

	if (!ValidSimTime(G::EntList[RecordUserID].CurSimTime)) // if not valid simtime
		return false;

	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(G::EntList[RecordUserID].model);
	if (!StudioModel) return false; //if cant get the model

	float damage = 0.f;

	// Hitboxes
	for (auto HITBOX : rage.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);

		Vec min = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);
		Vec mid = (max + min) / 2;

		float radius = StudioBox->m_flRadius * rage.hitchance;

		// Calc Left Point
		Vec left = G::EntList[RecordUserID].entity->GetLeft(mid, radius, G::LocalPlayer);

		// Calc Left Angle
		QAngle LeftAngle = CalculateAngle(left);

		// Calc Left Hitchance
		float LeftHitChance = CalculateHitchance(LeftAngle, left, G::EntList[RecordUserID].entity, HITBOX);

		// If the left hitchance is up to snuff...
		if (LeftHitChance >= rage.hitchance)
		{
			// if the point is visible
			bool visible = autowall->IsVisible(left, G::EntList[RecordUserID].entity);

			// no need to autowall if visible...
			if (visible)
			{
				damage = autowall->GetDamageVis(G::EntList[RecordUserID].entity, left, true);
				if (damage >= rage.vis_mindam)
				{
					Point = left;
					return true;
				}
				// Fire if lethal crap
				else if (rage.FireIfLethal && damage >= G::EntList[RecordUserID].health)
				{
					Point = left;
					return true;
				}
			}
			else
			{
				damage = autowall->GetDamage(G::EntList[RecordUserID].entity, left, true);
				if (damage >= rage.hid_mindam)
				{
					Point = left;
					return true;
				}
				// Fire if lethal crap
				else if (rage.FireIfLethal && damage >= G::EntList[RecordUserID].health)
				{
					Point = left;
					return true;
				}
			}
		}


		// Calc Right Point
		Vec right = G::EntList[RecordUserID].entity->GetRight(mid, radius, G::LocalPlayer);

		// Calc Right Angle
		QAngle RightAngle = CalculateAngle(right);

		// Calc Right Hitchance
		float RightHitChance = CalculateHitchance(RightAngle, right, G::EntList[RecordUserID].entity, HITBOX);

		// If the left hitchance is up to snuff...
		if (RightHitChance >= rage.hitchance)
		{
			// if the point is visible
			bool visible = autowall->IsVisible(right, G::EntList[RecordUserID].entity);

			// no need to autowall if visible...
			if (visible)
			{
				damage = autowall->GetDamageVis(G::EntList[RecordUserID].entity, right, true);
				if (damage >= rage.vis_mindam)
				{
					Point = right;
					return true;
				}
				// Fire if lethal crap
				else if (rage.FireIfLethal && damage >= G::EntList[RecordUserID].health)
				{
					Point = right;
					return true;
				}
			}
			else
			{
				damage = autowall->GetDamage(G::EntList[RecordUserID].entity, right, true);
				if (damage >= rage.hid_mindam)
				{
					Point = right;
					return true;
				}
				// Fire if lethal crap
				else if (rage.FireIfLethal && damage >= G::EntList[RecordUserID].health)
				{
					Point = right;
					return true;
				}
			}
		}
	}
	return false;
}

/*
This function finds the point that matches the conditions, than returns, regardless of whether
theres a better point to be found. One optimization could be going in order of points that
typicaly do more damage, to prioritize them. Will ignore for now.

This code is optimized for speed over damage/accuracy

// might be able to do multithreading w/
https://www.unknowncheats.me/forum/counterstrike-global-offensive/335391-fixing-tls-multithreaded-engine-calls.html
*/

bool Aimbot::ScanPlayerMin(int RecordUserID, Vec& Point)
{
	if (G::EntList[RecordUserID].index == G::LocalPlayerIndex) // entity is Localplayer
		return false;

	if (!(G::EntList[RecordUserID].entity)) // entity DOES NOT exist
		return false;

	if (!(G::EntList[RecordUserID].health > 0)) // entity is NOT alive
		return false;

	if (G::EntList[RecordUserID].team == G::LocalPlayerTeam) // Entity is on same team
		return false;

	if (G::EntList[RecordUserID].dormant)	// Entity is dormant
		return false;

	if (!ValidSimTime(G::EntList[RecordUserID].CurSimTime)) // if not valid simtime
		return false;

	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(G::EntList[RecordUserID].model);
	if (!StudioModel) return false; //if cant get the model

	bool ValidPointFound = false;

	// Hitboxes
	for (auto HITBOX : rage.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);

		Vec min = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmin.Transform(G::EntList[RecordUserID].Matrix[StudioBox->bone]);

		// Get mid aimpoint
		Vec mid = (max + min) / 2;
		
		// Check for hitchance before autowalling...
		QAngle MidAng = CalculateAngle(mid);

		// Attempt for visible damage
		float Damage = autowall->GetDamageVis(G::EntList[RecordUserID].entity, mid, true);

		// If there is visible min damage to be done
		if (Damage > 0.f)
		{
			// Get hitchance
			float MidHitChance = CalculateHitchance(MidAng, mid, G::EntList[RecordUserID].entity, HITBOX);

			// If hitchances aren't proper
			if (MidHitChance < rage.hitchance)
				continue;

			if (Damage >= rage.vis_mindam)
			{
				ValidPointFound = true;
				Point = mid;
				return true;
			}
			// Fire if lethal crap
			else if (rage.FireIfLethal && Damage >= G::EntList[RecordUserID].health)
			{
				Point = mid;
				return true;
			}
		}
		// Otherwise use the non visible damages
		else
		{
			// Get autowall damage
			Damage = autowall->GetDamage(G::EntList[RecordUserID].entity, mid, true);

			// no point in calc hitchance if no damage to be done
			if (Damage <= 0)
				continue;

			// Get hitchance
			float MidHitChance = CalculateHitchance(MidAng, mid, G::EntList[RecordUserID].entity, HITBOX);

			// If hitchances aren't proper
			if (MidHitChance < rage.hitchance)
				continue;

			// if there is damage to be done, and hitchance is possible...
			if (Damage >= rage.hid_mindam)
			{
				ValidPointFound = true;
				Point = mid;
				return true;
			}
			// Fire if lethal crap
			else if (rage.FireIfLethal && Damage >= G::EntList[RecordUserID].health)
			{
				Point = mid;
				return true;
			}
		}
	}
	return ValidPointFound;
}

Vec Aimbot::CalculateAngle(Vec Target)
{
	Vec EyePos = G::LocalPlayer->GetEyePos();//starting head position
	return CalculateAngle(EyePos, Target);
}

Vec Aimbot::CalculateAngle(Vec Target, Vec Source)
{
	static float Distance;	//Total distance between target and starting head position
	Distance = sqrtf(pow(Source.x - Target.x, 2) + pow(Source.y - Target.y, 2) + pow(Source.z - Target.z, 2));

	static float Height;	//total difference in height between target and starting head position
	Height = Source.z - Target.z;

	static Vec Angle;
	Angle.x = RAD2DEG(asin(-Height / Distance));								//pitch angle
	Angle.y = RAD2DEG(atan2(Source.y - Target.y, Source.x - Target.x));			//yaw angle

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

void Aimbot::Run()
{
	Legit();
	Rage();
}

