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

float Aimbot::CalculatePsudoHitchance()
{
	if (!G::LocalPlayerWeapon)
		return 0;

	float inaccuracy = 0;
	int weapIndex = GetWeaponIndex(G::LocalPlayerWeapon->GetWeaponId());

	switch (weapIndex)
	{
		// Pistols__________________________
	case 1: // glock
		inaccuracy = 0.0042f;
		break;
	case 2: // p2000
		inaccuracy = 0.003680f;
		break;
	case 3: // usp
		inaccuracy = 0.003680f;
		break;
	case 4: // deulies
		inaccuracy = 0.00525f;
		break;
	case 5: // P250
		inaccuracy = 0.006830f;
		break;
	case 6: // Tec9
		inaccuracy = 0.00368f;
		break;
	case 7: // 5-7
		inaccuracy = 0.00683f;
		break;
	case 8: // CZ
		inaccuracy = 0.0076f;
		break;
	case 9: // Deagle
		inaccuracy = 0.00218f;
		break;
	case 10: // Revolver
		inaccuracy = 0.005f;
		break;
		// Heavy__________________________
	case 11: // Nova
		inaccuracy = 0.00525f;
		break;
	case 12: // XM
		inaccuracy = 0.00525f;
		break;
	case 13: // SawedOFF
		inaccuracy = 0.00525f;
		break;
	case 14: // Mag7
		inaccuracy = 0.00525f;
		break;
	case 15: // M249
		inaccuracy = 0.00534f;
		break;
	case 16: // Negev
		inaccuracy = 0.00763f;
		break;
		// Smg__________________________
	case 17: // Mac10
		inaccuracy = 0.00998f;
		break;
	case 18: // MP9
		inaccuracy = 0.0055f;
		break;
	case 19: // MP7
		inaccuracy = 0.00592f;
		break;
	case 20: // MP5
		inaccuracy = 0.00592f;
		break;
	case 21: // UMP
		inaccuracy = 0.01007f;
		break;
	case 22: // P90
		inaccuracy = 0.01024f;
		break;
	case 23: // BIZON
		inaccuracy = 0.0105f;
		break;
		// Rifles__________________________
	case 24: // Galil
		inaccuracy = 0.00525f;
		break;
	case 25: // Famas
		inaccuracy = 0.00739f;
		break;
	case 26: // AK
		inaccuracy = 0.00481f;
		break;
	case 27: // M4A1
		inaccuracy = 0.0041f;
		break;
	case 28: // M4A1s
		inaccuracy = 0.0041f;
		break;
	case 29: // Scout
		inaccuracy = 0.0028f;
		break;
	case 30: // Sg553
		inaccuracy = 0.005f;
		break;
	case 31: // AUG
		inaccuracy = 0.00311f;
		break;
	case 32: // AWP
		inaccuracy = 0.0015f;
		break;
	case 33: // AUTO
		inaccuracy = 0.0015f;
		break;
	case 34: // AUTO
		inaccuracy = 0.0015f;
		break;
	default: // idk whatever
		inaccuracy = 0.f;
		break;
	}

	inaccuracy /= G::LocalPlayerWeapon->GetInaccuracy();

	return inaccuracy;
}

bool Aimbot::ValidPlayer(Player player)
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
	for (it = lagcomp->PlayerList.begin(); it != lagcomp->PlayerList.end(); it++)
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
	static Config::CState* Enable = Config::GetState("legit-aim-enable"); // terrible property name btw

	// Is legitbot enabled (if they are left clicking and bound mouse1, assume it's enabled lol
	if (!Enable->Get() && Config::GetKeybind("legit-aim-enable") != VK_LBUTTON) return;

	// If it's bound, and we aren't attacking, return
	if (Config::GetKeybind("legit-aim-enable") == VK_LBUTTON && !(G::cmd->buttons & IN_ATTACK))
		return;

	// Make sure it's a different tick
	static int tickcount = G::cmd->tick_count;
	if (tickcount == G::cmd->tick_count)
		return;

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

	// If it is, remove attack 
	if (G::LocalPlayerWeapon &&
		// it is sniper
		(G::LocalPlayerWeapon->GetWeaponId() == WeaponId::SSG08 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::SCAR20 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::G3SG1 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::AWP ||
			// it is pistol
			GetWeaponClass(G::LocalPlayerWeapon->GetWeaponId()) == 35) &&
		// bound left click and distance is out of fov...
		Config::GetKeybind("legit-aim-enable") == VK_LBUTTON && G::cmd->buttons & IN_ATTACK)
	{
		G::cmd->buttons &= ~IN_ATTACK; // remove attack
	}

	// Get Closest Hitbox
	bool valid;
	QAngle Ang = GetClosestAng(RecordUserID, valid);
	if (!valid) return;

	// If more than a 1 tap... start doing aimpunch
	if (G::LocalPlayer->GetShotsFired() > 1)
		Ang -= (G::LocalPlayer->GetAimPunchAngle() * 2);

	// We have found a valid angle to target and go towards
	Smooth(Ang);

	I::engine->SetViewAngles(Ang);

	// If within .6 degrees, and we are using sniper
	float NewDist = aimbot->CrosshairDist(Ang);
	if (G::LocalPlayerWeapon &&
		// it is sniper
		(G::LocalPlayerWeapon->GetWeaponId() == WeaponId::SSG08 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::SCAR20 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::G3SG1 ||
			G::LocalPlayerWeapon->GetWeaponId() == WeaponId::AWP ||
		// it is pistol
		GetWeaponClass(G::LocalPlayerWeapon->GetWeaponId()) == 35) &&
		// bound left click and distance is out of fov...
		NewDist <= 0.6 && Config::GetKeybind("legit-aim-enable") == VK_LBUTTON)
	{
		G::cmd->buttons |= IN_ATTACK; // add attack back on YEET
	}
}

bool Aimbot::UpdateLegitVal()
{
	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return false;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return false;

	int weaponID = (int)weapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);

	static Config::CFloat* Pistol_FOV			= Config::GetFloat("legitaim-pistol-fov");
	static Config::CState* Pistol_Smooth		= Config::GetState("legitaim-pistol-smoothing");
	static Config::CFloat* Pistol_SmoothAmount = Config::GetFloat("legitaim-pistol-smoothing-amount");

	static Config::CFloat* SMG_FOV				= Config::GetFloat("legitaim-smg-fov");
	static Config::CState* SMG_Smooth			= Config::GetState("legitaim-smg-smoothing");
	static Config::CFloat* SMG_SmoothAmount	= Config::GetFloat("legitaim-smg-smoothing-amount");

	static Config::CFloat* Heavy_FOV			= Config::GetFloat("legitaim-heavy-fov");
	static Config::CState* Heavy_Smooth		= Config::GetState("legitaim-heavy-smoothing");
	static Config::CFloat* Heavy_SmoothAmount  = Config::GetFloat("legitaim-heavy-smoothing-amount");

	static Config::CFloat* Rifle_FOV			= Config::GetFloat("legitaim-rifle-fov");
	static Config::CState* Rifle_Smooth		= Config::GetState("legitaim-rifle-smoothing");
	static Config::CFloat* Rifle_SmoothAmount	= Config::GetFloat("legitaim-rifle-smoothing-amount");

	static Config::CFloat* Sniper_FOV			= Config::GetFloat("legitaim-sniper-fov");
	static Config::CState* Sniper_Smooth		= Config::GetState("legitaim-sniper-smoothing");
	static Config::CFloat* Sniper_SmoothAmount = Config::GetFloat("legitaim-sniper-smoothing-amount");


	// I could use a switch case for this meh
	if (WeaponClass == 35)		//pistol
	{
		legit.fov = Pistol_FOV->Get();
		legit.smoothing = Pistol_Smooth->Get();
		legit.smoothing_amount = Pistol_SmoothAmount->Get();
		GetLegitHitboxes(0);
		return true;
	}
	else if (WeaponClass == 37)	//smg
	{
		legit.fov = SMG_FOV->Get();
		legit.smoothing = SMG_Smooth->Get();
		legit.smoothing_amount = SMG_SmoothAmount->Get();
		GetLegitHitboxes(1);
		return true;
	}
	else if (WeaponClass == 36)	//heavy
	{
		legit.fov = Heavy_FOV->Get();
		legit.smoothing = Heavy_Smooth->Get();
		legit.smoothing_amount = Heavy_SmoothAmount->Get();
		GetLegitHitboxes(2);
		return true;
	}
	else if (WeaponClass == 38)	//rifles (includes scouts, awps, autos)
	{
		if (weaponID == 40 || weaponID == 9 || weaponID == 11 || weaponID == 38) //snipers
		{
			legit.fov = Sniper_FOV->Get();
			legit.smoothing = Sniper_Smooth->Get();
			legit.smoothing_amount = Sniper_SmoothAmount->Get();
			GetLegitHitboxes(4);
			return true;
		}
		else //rifles
		{
			legit.fov = Rifle_FOV->Get();
			legit.smoothing = Rifle_Smooth->Get();
			legit.smoothing_amount = Rifle_SmoothAmount->Get();
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

	static Config::CMultiSelect* Pistol_Hitbox = Config::GetSelected("legitaim-pistol-hitbox");
	static Config::CMultiSelect* SMG_Hitbox = Config::GetSelected("legitaim-smg-hitbox");
	static Config::CMultiSelect* Heavy_Hitbox = Config::GetSelected("legitaim-heavy-hitbox");
	static Config::CMultiSelect* Rifle_Hitbox = Config::GetSelected("legitaim-rifle-hitbox");
	static Config::CMultiSelect* Sniper_Hitbox = Config::GetSelected("legitaim-sniper-hitbox");

	switch (gun)
	{
	case 0:
		selection = Pistol_Hitbox->Mask;
		break;
	case 1:
		selection = SMG_Hitbox->Mask;
		break;
	case 2:
		selection = Heavy_Hitbox->Mask;
		break;
	case 3:
		selection = Rifle_Hitbox->Mask;
		break;
	case 4:
		selection = Sniper_Hitbox->Mask;
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
	
	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(lagcomp->PlayerList[RecordUserID].ptrModel);
	if (!StudioModel) return BestAng; //if cant get the model

	for (auto HITBOX : legit.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);

		Vec min = StudioBox->bbmin.Transform(lagcomp->PlayerList[RecordUserID].Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmin.Transform(lagcomp->PlayerList[RecordUserID].Matrix[StudioBox->bone]);
		Vec mid = (max + min) / 2;

		// no need to "visible autowall" if not visible
		bool visible = autowall->IsVisible(mid, lagcomp->PlayerList[RecordUserID].ptrEntity);

		// if not visible, continue
		if (!visible)
			continue;

		float dam = autowall->Damage(mid, HITBOX);

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
void Aimbot::Rage()
{
	static Config::CState* Enable = Config::GetState("rage-aim-enable");
	static Config::CState* Silent = Config::GetState("rage-aim-silent");
	static Config::CState* AutoShoot = Config::GetState("rage-aim-autoshoot");
	static Config::CState* FakeDuck = Config::GetState("misc-movement-fakeduck");

	// Make sure it's a different tick
	static int tickcount = G::cmd->tick_count;
	if (tickcount == G::cmd->tick_count) return;
	tickcount = G::cmd->tick_count;

	// Is ragebot enabled
	if (!Enable->Get()) return;

	// If the player can shoot a weapon... 
	if (!G::LocalPlayer->CanShoot()) return;

	// Is the current weapon alright/Get config Values
	if (!UpdateRageVal()) return;

	// If we are lagging on peak...
	if (fakelag->LaggingOnPeak) return;

	// Sort players
	SortPlayers(this->players);

	// Scan through the players
	if (ScanPlayers())
	{
		L::Verbose("ScanPlayer - shooting at players");

		/*esp->points.clear();
		esp->points.resize(0);
		esp->points.push_back(this->AimPoint);*/
		QAngle Angle = CalculateAngle(this->AimPoint);
		Angle -= (G::LocalPlayer->GetAimPunchAngle() * 2);

		// If not silent aim, adjust angles, otherwise do silent
		if (!Silent->Get())
			I::engine->SetViewAngles(Angle);
		G::cmd->viewangles = Angle;

		// If autoshoot.. FIRE!
		if (AutoShoot->Get())
		{
			resolver->BacktrackShot = backtrackaim ? true : false;
			resolver->LogShot = true;
			G::cmd->buttons |= IN_ATTACK;
			G::cmd->tick_count = TargetTickCount;
			CapsuleOverlay2(lagcomp->PlayerList[TargetUserID].ptrEntity, Color(255, 0, 0, 255), 2, TargetMatrix);
		}

		// only force send if not fakeducking... (as that will break fd)
		if (G::pSendPacket && !FakeDuck->Get())
			*G::pSendPacket = true;
	} 
}

void Aimbot::SortPlayers(std::vector<std::pair<int, float>>& values)
{
	//update every .1 sec
	static float LastTimeUpdate = I::globalvars->m_curTime;
	if (fabsf(I::globalvars->m_curTime - LastTimeUpdate) < 0.1)
		return;
	LastTimeUpdate = I::globalvars->m_curTime;

	values.clear();
	values.resize(0);

	Vec localorigin = G::LocalPlayer->GetVecOrigin();
	for (auto a : lagcomp->PlayerList)
	{
		// If bad entity, continue
		if (!lagcomp->ValidRecord(a.second))
			continue;
		
		values.push_back(std::pair(a.first, (100 - a.second.Health) * 8 + 80000 / (a.second.Origin - localorigin).VecLength2D()));
	}

	//Trade secret sort struct for epic blazin fast aimbot
	struct {
		bool operator()(std::pair<int, float> a, std::pair<int, float> b) const
		{
			// otherwise sort by proper values
			return a.second > b.second;
		}
	} targetSort;

	// we do a bit of sortin
	std::sort(values.begin(), values.end(), targetSort);
}

static ThreadHandle_t StartThread(ThreadFunc_t start, void* arg)
{
	using CreateSimpleThread_t = ThreadHandle_t(__cdecl*)(ThreadFunc_t, void*, SIZE_T);
	static auto CreateSimpleThread = (CreateSimpleThread_t)GetProcAddress(GetModuleHandleA("tier0.dll"), "CreateSimpleThread");
	return CreateSimpleThread(start, arg, 0);
}

bool Aimbot::ScanPlayers()
{
	L::Verbose("ScanPlayers");
	// for now I'm doing scan player along with backtrack cuz I might as well
	int i = 1;
	backtrackaim = false;
	for (auto &a : this->players)
	{
		// scan players
		if (ScanPlayer(a.first, this->AimPoint))
			return true;
		// scan backtrack of players
		if (ScanPlayerBacktrack(a.first, this->AimPoint))
		{
			backtrackaim = true;
			return true;
		}
			
		
		i++;

		// check if done too many scans
		if (i > maxplayerscan)
			return false;
	}
	return false;
}

// need to implement the canhit floating point to get weird delay crap nonsense to work...
bool Aimbot::ScanPlayer(int UserID, Vec& Point)
{
	L::Verbose("ScanPlayer");
	if (lagcomp->PlayerList.empty())
		return false;

	/*L::Verbose("ScanPlayer - notempty");*/

	if (lagcomp->PlayerList.find(UserID) == lagcomp->PlayerList.end())
		return false;

	/*L::Verbose("ScanPlayer - notend");*/

	if (lagcomp->PlayerList[UserID].Index == G::LocalPlayerIndex) // entity is Localplayer
		return false;

	/*L::Verbose("ScanPlayer - passedindexcheck");*/

	if (!(lagcomp->PlayerList[UserID].ptrEntity)) // entity DOES NOT exist
		return false;

	/*L::Verbose("ScanPlayer - passedptrEntitycheck");*/

	if (!(lagcomp->PlayerList[UserID].Health > 0)) // entity is NOT alive
		return false;

	/*L::Verbose("ScanPlayer - passedhealthcheck");*/

	if (lagcomp->PlayerList[UserID].Team == G::LocalPlayerTeam) // Entity is on same team
		return false;

	/*L::Verbose("ScanPlayer - passedteamcheck");*/

	if (lagcomp->PlayerList[UserID].Dormant)	// Entity is dormant
		return false;

	/*L::Verbose("ScanPlayer - passeddormantcheck");*/

	if (!lagcomp->PlayerList[UserID].ptrModel) return false;

	//L::Verbose("ScanPlayer - passedmodelcheck");

	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(lagcomp->PlayerList[UserID].ptrModel);
	if (!StudioModel) return false; //if cant get the model

	L::Verbose("ScanPlayer - passed checks");

	float damage = 0.f;

	for (auto HITBOX : rage.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);
		float radius = StudioBox->m_flRadius * 0.85;

		L::Verbose("ScanPlayer - got hitbox");

		Vec min = StudioBox->bbmin.Transform(lagcomp->PlayerList[UserID].Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmax.Transform(lagcomp->PlayerList[UserID].Matrix[StudioBox->bone]);

		Vec point = min.z > max.z ? min : max;
		if (HITBOX == HITBOX_HEAD)
		{
			// we scan left, one tick, then scan right the next, and then the top tick, then repeat
			// this is to hopefully de-lag, though we can potentially do bad by 1 tick.
			// this is counter by the fact that, because we aren't lagging, we will be most up to date
			// maybe I'll add 2 implementations, with the user deciding which one to use
			switch (G::cmd->tick_count % 3)
			{
			case 0: // target top left
				point = lagcomp->PlayerList[UserID].ptrEntity->GetTopLeft(point, radius, G::LocalPlayer);
				break;
			case 1: // target top right
				point = lagcomp->PlayerList[UserID].ptrEntity->GetTopRight(point, radius, G::LocalPlayer);
				break; // target very top/forehead
				point.z += radius;
			}	
		}

		// Calc Mid Angle
		L::Verbose("ScanPlayer - CalculateAngle");
		QAngle MidAngle = CalculateAngle(point);

		// Calc Mid Hitchance
		L::Verbose("ScanPlayer - CalculateHitchance");
		float MidHitchance = CalculateHitchance(MidAngle, point, lagcomp->PlayerList[UserID].ptrEntity, HITBOX);
		
		// If the left hitchance is up to snuff...
		if (MidHitchance >= rage.hitchance)
		{
			// if the point is visible
			L::Verbose("ScanPlayer - IsVisible");
			bool visible = autowall->IsVisible(point, lagcomp->PlayerList[UserID].ptrEntity);

			// no need to autowall if visible...
			L::Verbose("ScanPlayer - Damage");
			damage = autowall->Damage(point, HITBOX, true);
			if (visible && damage >= rage.vis_mindam)
			{
				this->TargetUserID = UserID;
				L::Verbose("ScanPlayer - memcpy");
				std::memcpy(TargetMatrix, lagcomp->PlayerList[UserID].Matrix, 256 * sizeof(Matrix3x4));
				// IT SHOULD BE + GETLERP BUT IDK Y THIS WORKS BRUGH
				TargetTickCount =/* TimeToTicks*/(lagcomp->PlayerList[UserID].SimulationTime) / I::globalvars->m_intervalPerTick;
				
				Point = point;
				return true;
			}
			else if (damage >= rage.hid_mindam)
			{
				this->TargetUserID = UserID;
				L::Verbose("ScanPlayer - memcpy");
				std::memcpy(TargetMatrix, lagcomp->PlayerList[UserID].Matrix, 256 * sizeof(Matrix3x4));

				// IT SHOULD BE + GETLERP BUT IDK Y THIS WORKS BRUGH
				TargetTickCount = /*TimeToTicks*/(lagcomp->PlayerList[UserID].SimulationTime) / I::globalvars->m_intervalPerTick;

				Point = point;
				return true;
			}
		}
	}
	return false;
}

//https://github.com/sstokic-tgm/Gladiatorcheatz-v2/blob/master/features/LagCompensation.cpp 
// need to fight with FinishLagCompensation function to see how to rage backtrack
// STILL IN PROGRESS!
bool Aimbot::ScanPlayerBacktrack(int UserID, Vec& Point)
{
	L::Verbose("ScanPlayerBacktrack");
	if (lagcomp->PlayerList[UserID].Index == G::LocalPlayerIndex) // entity is Localplayer
		return false;

	if (!(lagcomp->PlayerList[UserID].ptrEntity)) // entity DOES NOT exist
		return false;

	if (!(lagcomp->PlayerList[UserID].Health > 0)) // entity is NOT alive
		return false;

	if (lagcomp->PlayerList[UserID].Team == G::LocalPlayerTeam) // Entity is on same team
		return false;

	if (lagcomp->PlayerList[UserID].Dormant)	// Entity is dormant
		return false;
	
	if (lagcomp->PlayerList[UserID].Records.empty()) // If no records...
		return false;

	//scan fastest vel tick
	Tick TargetTick = lagcomp->PlayerList[UserID].Records.back();
	for (auto& a : lagcomp->PlayerList[UserID].Records)
	{
		// attempt to onshot...
		if (a.Shot)
		{
			TargetTick = a;
		}
	}
	if (!lagcomp->PlayerList[UserID].ptrModel) return false;

	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(lagcomp->PlayerList[UserID].ptrModel);
	if (!StudioModel) return false; //if cant get the model

	float damage = 0.f;

	for (auto HITBOX : rage.hitboxes)
	{
		mstudiobbox_t* StudioBox = StudioModel->GetHitboxSet(0)->GetHitbox(HITBOX);
		if (!StudioBox) continue;	//if cant get the hitbox...
		int HitGroup = GetHitGroup(HITBOX);

		Vec min = StudioBox->bbmin.Transform(TargetTick.Matrix[StudioBox->bone]);
		Vec max = StudioBox->bbmax.Transform(TargetTick.Matrix[StudioBox->bone]);
		Vec mid = (max + min) / 2;

		// Calc Mid Angle
		QAngle MidAngle = CalculateAngle(mid);

		// Calc Mid Hitchance
		float MidHitchance = CalculatePsudoHitchance();

		// If the left hitchance is up to snuff...
		if (MidHitchance >= rage.hitchance)
		{
			if (autowall->CanHitFloatingPoint(mid, true))
			{
				this->TargetUserID = UserID;
				std::memcpy(TargetMatrix, TargetTick.Matrix, 256 * sizeof(Matrix3x4));
				TargetTickCount = /*TimeToTicks*/(TargetTick.SimulationTime + GetLerp()) / I::globalvars->m_intervalPerTick;
				Point = mid;
				return true;
			}
		}
	}
	return false;
}

bool Aimbot::UpdateRageVal()
{
	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return false;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return false;

	int weaponID = (int)weapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);

	static Config::CFloat* Pistol_VisMin		= Config::GetFloat("rageaim-pistol-mindamage-visible");
	static Config::CFloat* Pistol_HidMin		= Config::GetFloat("rageaim-pistol-mindamage-hidden");
	static Config::CFloat* Pistol_HitChance	= Config::GetFloat("rageaim-pistol-hitchance");
	static Config::CState* Pistol_BaimIfLethal	= Config::GetState("rageaim-pistol-baimiflethal");
	static Config::CState* Pistol_Override		= Config::GetState("rageaim-pistol-override");
	static Config::CFloat* Pistol_OverrideDam  = Config::GetFloat("rageaim-pistol-override-damage");

	static Config::CFloat* Smg_VisMin			= Config::GetFloat("rageaim-smg-mindamage-visible");
	static Config::CFloat* Smg_HidMin			= Config::GetFloat("rageaim-smg-mindamage-hidden");
	static Config::CFloat* Smg_HitChance		= Config::GetFloat("rageaim-smg-hitchance");
	static Config::CState* Smg_BaimIfLethal	= Config::GetState("rageaim-smg-baimiflethal");
	static Config::CState* Smg_Override		= Config::GetState("rageaim-smg-override");
	static Config::CFloat* Smg_OverrideDam		= Config::GetFloat("rageaim-smg-override-damage");

	static Config::CFloat* Heavy_VisMin		= Config::GetFloat("rageaim-heavy-mindamage-visible");
	static Config::CFloat* Heavy_HidMin		= Config::GetFloat("rageaim-heavy-mindamage-hidden");
	static Config::CFloat* Heavy_HitChance		= Config::GetFloat("rageaim-heavy-hitchance");
	static Config::CState* Heavy_BaimIfLethal	= Config::GetState("rageaim-heavy-baimiflethal");
	static Config::CState* Heavy_Override		= Config::GetState("rageaim-heavy-override");
	static Config::CFloat* Heavy_OverrideDam	= Config::GetFloat("rageaim-heavy-override-damage");

	static Config::CFloat* Scout_VisMin		= Config::GetFloat("rageaim-scout-mindamage-visible");
	static Config::CFloat* Scout_HidMin		= Config::GetFloat("rageaim-scout-mindamage-hidden");
	static Config::CFloat* Scout_HitChance		= Config::GetFloat("rageaim-scout-hitchance");
	static Config::CState* Scout_BaimIfLethal	= Config::GetState("rageaim-scout-baimiflethal");
	static Config::CState* Scout_Override		= Config::GetState("rageaim-scout-override");
	static Config::CFloat* Scout_OverrideDam	= Config::GetFloat("rageaim-scout-override-damage");

	static Config::CFloat* AWP_VisMin			= Config::GetFloat("rageaim-awp-mindamage-visible");
	static Config::CFloat* AWP_HidMin			= Config::GetFloat("rageaim-awp-mindamage-hidden");
	static Config::CFloat* AWP_HitChance		= Config::GetFloat("rageaim-awp-hitchance");
	static Config::CState* AWP_BaimIfLethal	= Config::GetState("rageaim-awp-baimiflethal");
	static Config::CState* AWP_Override		= Config::GetState("rageaim-awp-override");
	static Config::CFloat* AWP_OverrideDam		= Config::GetFloat("rageaim-awp-override-damage");

	static Config::CFloat* Auto_VisMin			= Config::GetFloat("rageaim-auto-mindamage-visible");
	static Config::CFloat* Auto_HidMin			= Config::GetFloat("rageaim-auto-mindamage-hidden");
	static Config::CFloat* Auto_HitChance		= Config::GetFloat("rageaim-auto-hitchance");
	static Config::CState* Auto_BaimIfLethal	= Config::GetState("rageaim-auto-baimiflethal");
	static Config::CState* Auto_Override		= Config::GetState("rageaim-auto-override");
	static Config::CFloat* Auto_OverrideDam	= Config::GetFloat("rageaim-auto-override-damage");

	static Config::CFloat* Rifle_VisMin		= Config::GetFloat("rageaim-rifle-mindamage-visible");
	static Config::CFloat* Rifle_HidMin		= Config::GetFloat("rageaim-rifle-mindamage-hidden");
	static Config::CFloat* Rifle_HitChance		= Config::GetFloat("rageaim-rifle-hitchance");
	static Config::CState* Rifle_BaimIfLethal	= Config::GetState("rageaim-rifle-baimiflethal");
	static Config::CState* Rifle_Override		= Config::GetState("rageaim-rifle-override");
	static Config::CFloat* Rifle_OverrideDam	= Config::GetFloat("rageaim-rifle-override-damage");
	

	// I could use a switch case for this meh
	if (WeaponClass == 35)		//pistol
	{
		if (!Pistol_Override->Get())
		{
			rage.vis_mindam = Pistol_VisMin->Get();
			rage.hid_mindam = Pistol_HidMin->Get();
		}
		else
		{
			rage.vis_mindam = Pistol_OverrideDam->Get();
			rage.hid_mindam = Pistol_OverrideDam->Get();
		}
		rage.hitchance = Pistol_HitChance->Get() / 100.f;
		rage.BaimIfLethal = Pistol_BaimIfLethal->Get();
		GetRageHitboxes(0);
		return true;
	}
	else if (WeaponClass == 37)	//smg
	{
		if (!Smg_Override->Get())
		{
			rage.vis_mindam = Smg_VisMin->Get();
			rage.hid_mindam = Smg_HidMin->Get();
		} 
		else
		{
			rage.vis_mindam = Smg_OverrideDam->Get();
			rage.hid_mindam = Smg_OverrideDam->Get();
		}
		rage.hitchance = Smg_HitChance->Get() / 100.f;
		rage.BaimIfLethal = Smg_BaimIfLethal->Get();
		GetRageHitboxes(1);
		return true;
	}
	else if (WeaponClass == 36)	//heavy
	{
		if (!Heavy_Override->Get())
		{
			rage.vis_mindam = Heavy_VisMin->Get();
			rage.hid_mindam = Heavy_HidMin->Get();
		} 
		else
		{
			rage.vis_mindam = Heavy_OverrideDam->Get();
			rage.hid_mindam = Heavy_OverrideDam->Get();
		}
		rage.hitchance = Heavy_HitChance->Get() / 100.f;
		rage.BaimIfLethal = Heavy_BaimIfLethal->Get();
		GetRageHitboxes(2);
		return true;
	}
	else if (WeaponClass == 38)	//rifles (includes scouts, awps, autos)
	{
		if (weaponID == 40) //scout
		{
			if (!Scout_Override->Get())
			{
				rage.vis_mindam = Scout_VisMin->Get();
				rage.hid_mindam = Scout_HidMin->Get();
			}
			else
			{
				rage.vis_mindam = Scout_OverrideDam->Get();
				rage.hid_mindam = Scout_OverrideDam->Get();
			}
			rage.hitchance = Scout_HitChance->Get() / 100.f;
			rage.BaimIfLethal = Scout_BaimIfLethal->Get();
			GetRageHitboxes(3);
			return true;
		}
		else if (weaponID == 9) //awp
		{
			if (!Auto_Override->Get())
			{
				rage.vis_mindam = AWP_VisMin->Get();
				rage.hid_mindam = AWP_HidMin->Get();
			}
			else
			{
				rage.vis_mindam = AWP_OverrideDam->Get();
				rage.hid_mindam = AWP_OverrideDam->Get();
			}
			rage.hitchance = AWP_HitChance->Get() / 100.f;
			rage.BaimIfLethal = AWP_BaimIfLethal->Get();
			GetRageHitboxes(4);
			return true;
		}
		else if (weaponID == 11 || weaponID == 38) //autos
		{
			if (!Auto_Override->Get())
			{
				rage.vis_mindam = Auto_VisMin->Get();
				rage.hid_mindam = Auto_VisMin->Get();
			}
			else
			{
				rage.vis_mindam = Auto_OverrideDam->Get();
				rage.hid_mindam = Auto_OverrideDam->Get();
			}
			rage.hitchance = Auto_VisMin->Get() / 100.f;
			rage.BaimIfLethal = Auto_VisMin->Get();
			GetRageHitboxes(5);
			return true;
		}
		else //rifles
		{
			if (!Rifle_Override->Get())
			{
				rage.vis_mindam = Rifle_VisMin->Get();
				rage.hid_mindam = Rifle_HidMin->Get();
			}
			else
			{
				rage.vis_mindam = Rifle_OverrideDam->Get();
				rage.hid_mindam = Rifle_OverrideDam->Get();
			}
			rage.hitchance = Rifle_HitChance->Get() / 100.f;
			rage.BaimIfLethal = Rifle_BaimIfLethal->Get();
			GetRageHitboxes(6);
			return true;
		}
	}
	else //grenades tasers and knifes
		return false;
}

void Aimbot::GetRageHitboxes(int gun)
{
	uint16_t selection = 0;
	rage.hitboxes.clear();
	rage.hitboxes.resize(0);

	
	static Config::CMultiSelect* Pistol_Hitbox_R	= Config::GetSelected("rageaim-pistol-hitbox");
	static Config::CMultiSelect* SMG_Hitbox_R = Config::GetSelected("rageaim-smg-hitbox");
	static Config::CMultiSelect* Heavy_Hitbox_R = Config::GetSelected("rageaim-heavy-hitbox");
	static Config::CMultiSelect* Rifle_Hitbox_R = Config::GetSelected("rageaim-rifle-hitbox");
	static Config::CMultiSelect* Scout_Hitbox_R = Config::GetSelected("rageaim-scout-hitbox");
	static Config::CMultiSelect* AWP_Hitbox_R = Config::GetSelected("rageaim-awp-hitbox");
	static Config::CMultiSelect* Auto_Hitbox_R = Config::GetSelected("rageaim-auto-hitbox");

	switch (gun)
	{
	case 0:
		L::Verbose("pistol");
		selection = Pistol_Hitbox_R->Mask;
		break;
	case 1:
		L::Verbose("smg");
		selection = SMG_Hitbox_R->Mask;
		break;
	case 2:
		L::Verbose("heavy");
		selection = Heavy_Hitbox_R->Mask;
		break;
	case 3:
		L::Verbose("scoupt");
		selection = Scout_Hitbox_R->Mask;
		break;
	case 4:
		L::Verbose("awp");
		selection = AWP_Hitbox_R->Mask;
		break;
	case 5:
		L::Verbose("auto");
		selection = Auto_Hitbox_R->Mask;
		break;
	case 6:
		L::Verbose("rifle");
		selection = Rifle_Hitbox_R->Mask;
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
		rage.hitboxes.push_front(HITBOX_UPPER_CHEST); // PRIORITY SECOND TO UPER CHEST
	if (selection & (1 << 3))			// lower chest
		rage.hitboxes.push_back(HITBOX_LOWER_CHEST);
	if (selection & (1 << 4))			// stomach
		rage.hitboxes.push_back(HITBOX_STOMACH);
	if (selection & (1 << 5))			// pelvis
		rage.hitboxes.push_front(HITBOX_PELVIS); // PRIORITY FIRST TO PELVIS
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

/*
void Aimbot::HandleBaimConditions(int RecordUserID)
{
	// If not enabled
	if (!rage.BaimIfLethal) return;

	// if no data return
	if (!G::LocalPlayerWeaponData) return;

	// Get player + player health
	Player p = G::EntList[RecordUserID];
	int health = p.health;
	
	// calculate damage --> assume trace is .99 of the way there
	float Damage = G::LocalPlayerWeaponData->Damage;
	Damage = autowall->GetDamageMultiplier(HITGROUP_STOMACH) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, 0.99 * G::LocalPlayerWeaponData->Range / 500.0f);
	float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
	if (autowall->IsArmored(HITGROUP_STOMACH, p.entity->HasHelmet()))
		Damage -= (p.entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? p.entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);
	
	// If potentially lethal
	//  --> push only pelvis
	if (p.health < Damage * 1.2) //1.2 is just a minor MANUAL adjustment lol
	{
		// Push only pelvis
		rage.hitboxes.clear();
		rage.hitboxes.resize(0);
		rage.hitboxes.push_back(HITBOX_UPPER_CHEST);
		rage.hitboxes.push_back(HITBOX_PELVIS);
	}
}
*/

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

void Aimbot::Run()
{
	Legit();
	Rage();
}

