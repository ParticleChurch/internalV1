#include "../../Include.hpp"

Movement* movement = new Movement();

void Movement::AAMoveFix()
{
	static Config2::CState* LegitEnable = Config2::GetState("antiaim-legit-enable");
	static Config2::CState* RageEnable = Config2::GetState("antiaim-rage-enable");
	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// If not on ground
	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND)) return;

	// If neither options are ticked... return
	if (!LegitEnable->Get() && !RageEnable->Get()) return;

	if (fabsf(G::cmd->sidemove) < 5.0f) {
		G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
	}
}

void Movement::BunnyHop()
{
	static Config2::CState* Enabled = Config2::GetState("misc-movement-bhop");

	// If bhop is NOT enabled in config...
	if (!Enabled->Get())
		return;

	// If we are holding jump, localplayer alive, and we aren't on the ground, and we arent on a ladder...
	// Inverse the jump (aka space will not send unless on ground when holding space)
	if ((G::cmd->buttons & IN_JUMP) && G::LocalPlayerAlive && !(G::LocalPlayer->GetFlags() & FL_ONGROUND)
		&& G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER) {
		G::cmd->buttons &= ~IN_JUMP;
	}
}

void Movement::FastCrouch()
{
	static Config2::CState* Enabled = Config2::GetState("misc-movement-fastcrouch");

	// If FastCrouch is NOT enabled in config...
	if (!Enabled->Get())
		return;

	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// Fastcrouch
	G::cmd->buttons |= IN_BULLRUSH;
}

void Movement::RageAutoStrafe()
{
	static Config2::CState* State = Config2::GetState("misc-movement-autostrafe");

	if (State->Get() != 1) return;

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetFlags() & FL_ONGROUND) return;

	// Direction we are looking
	float looking = G::StartAngle.y;

	// which direction we want to go
	Vec DirVec = Vec(G::cmd->sidemove, G::cmd->forwardmove, 0);
	float Dir = RAD2DEG(atan2(DirVec.x, DirVec.y));

	// which direction we currently are going 
	Vec Vel = G::LocalPlayer->GetVecVelocity();
	float VelDir = NormalizeYaw(-RAD2DEG(atan2(Vel.y, Vel.x)));		// Velocity direction

	// Get difference between direction we want to go, and direction we are going...
	float Delta = NormalizeYaw(Dir - VelDir - looking);

	// TRANSITIONING!
	if (fabsf(Delta) > 10.f)
	{
		// Calculate a step thing
		float sin_rot = sinf(DEG2RAD(VelDir + looking+1));
		float cos_rot = cosf(DEG2RAD(VelDir + looking+1));
		
		// turning left
		if (Delta < 0)
		{
			G::cmd->forwardmove = sin_rot * 450.f;
			G::cmd->sidemove = -cos_rot * 450.f;
		}
		// turning right
		else
		{
			G::cmd->forwardmove = -sin_rot * 450.f;
			G::cmd->sidemove = cos_rot * 450.f;
		}
	}
	else
	{
		bool flip = G::cmd->command_number % 2;
		// OMG WE BE STRAFIN BOI!
		// Calculate a step thing
		float sin_rot = sinf(DEG2RAD(VelDir + looking));
		float cos_rot = cosf(DEG2RAD(VelDir + looking));

		// turning left
		if (flip)
		{
			G::cmd->forwardmove = sin_rot * 450.f;
			G::cmd->sidemove = -cos_rot * 450.f;
		}
		// turning right
		else
		{
			G::cmd->forwardmove = -sin_rot * 450.f;
			G::cmd->sidemove = cos_rot * 450.f;
		}
		G::cmd->viewangles.y += flip ? -5 : 5;
	}
}

void Movement::LegitAutoStrafe()
{
	static Config2::CState* State = Config2::GetState("misc-movement-autostrafe");

	if (State->Get() != 2)
		return;

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetFlags() & FL_ONGROUND) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	if (G::cmd->mousedx < -5)
		G::cmd->sidemove = -450;
	else if(G::cmd->mousedx > 5)
		G::cmd->sidemove = 450;
}

void Movement::SlowWalk() 
{

	static Config2::CState* Enabled = Config2::GetState("misc-movement-slowwalk");
	static Config2::CFloat* Speed = Config2::GetFloat("misc-movement-slowwalk-speed");

	// IF slowwalk isn't enabled
	if (!Enabled->Get())
		return;

	// valid - valid time to slow walk
	bool valid = G::LocalPlayer && G::LocalPlayerAlive && (G::LocalPlayer->GetFlags() & FL_ONGROUND) && G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER;
	// If not a valid time to slow walk
	if (!valid)
		return;

	float maxSpeed = G::LocalPlayer->MaxAccurateSpeed() / 3.f;
	maxSpeed -= 5;
	maxSpeed *= (Speed->Get() / 100.f);

	if (G::cmd->forwardmove && G::cmd->sidemove) {
		const float maxSpeedRoot = maxSpeed * static_cast<float>(M_SQRT1_2);
		G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
		G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
	}
	else if (G::cmd->forwardmove) {
		G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeed : maxSpeed;
	}
	else if (G::cmd->sidemove) {
		G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeed : maxSpeed;
	}

	FastStop();
}

void Movement::FakeDuck()
{
	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	static Config2::CState* Enabled = Config2::GetState("misc-movement-fakeduck");

	// If FakeDuck is NOT enabled in config...
	if (!Enabled->Get()) return;
	
	// FAKEDUCK
	G::cmd->buttons |= IN_BULLRUSH;
	if (I::engine->GetNetChannelInfo()->ChokedPackets > (14 / 2))
		G::cmd->buttons |= IN_DUCK;
	else
		G::cmd->buttons &= ~IN_DUCK;
}

void Movement::LegSlide()
{
	static Config2::CState* Enabled = Config2::GetState("misc-movement-leg");
	static Config2::CFloat* LegTime = Config2::GetFloat("misc-movement-leg-time");

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	if (!Enabled->Get()) return;

	static bool switcher = false;
	static float time = I::globalvars->m_curTime;
	if (I::globalvars->m_curTime - time > LegTime->Get() / 1000.f)
	{
		time = I::globalvars->m_curTime;
		switcher = !switcher;
	}

	if(switcher)
		G::cmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
}

void Movement::AutoStop()
{
	static Config2::CState* RageAimbot = Config2::GetState("rage-aim-enable");
	static Config2::CState* AutoScope = Config2::GetState("rage-aim-autoscope");
	static Config2::CState* AutoStop = Config2::GetState("misc-movement-autostop");

	if (!AutoScope->Get() && !AutoStop->Get())
		return;

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	// If there is bad weapon
	if (!G::LocalPlayerWeapon)
		return;

	if (!G::LocalPlayerWeaponData)
		return;

	// If attempting bhop or olding jump...
	if (G::cmd->buttons & IN_JUMP)
		return;

	// If we can't shoot, why are we bothering stopping?
	if (!G::LocalPlayer->CanShoot()) return;

	// If not pew pew weapon
	int weaponID = (int)G::LocalPlayerWeapon->GetWeaponId();
	int WeaponClass = (int)GetWeaponClass((WeaponId)weaponID);
	if (WeaponClass == 40 || WeaponClass == 0) // if nade or knife/taser/other
		return;


	// valid - valid time to auto stop
	bool valid = G::LocalPlayer && G::LocalPlayerAlive && (G::LocalPlayer->GetFlags() & FL_ONGROUND) && G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER;
	// If not a valid time to auto stop/walk
	if (!valid)
		return;

	// Update Velocity 
	Vec velocity = G::LocalPlayer->GetVecVelocity();
	velocity *= (I::globalvars->m_intervalPerTick * 2);
	//velocity now holds our distance to move (doubled for safety)

	// Updating our next position
	Vec NextPos = G::LocalPlayer->GetEyePos() + velocity;

	bool PossibleDamage = false;

	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		if (it->second.index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(it->second.entity)) // entity DOES NOT exist
			continue;

		if (!(it->second.health > 0)) // entity is NOT alive
			continue;

		if (it->second.team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (it->second.dormant)	// Entity is dormant
			continue;

		// Get damage from next position... (LOTS OF AWALL, idk what i'm supposed to do lmao)
		if (autowall->GetDamage(NextPos, it->second.EyePos, it->second.entity))
		{
			PossibleDamage = true;
			break;
		}
	}

	// If there's no possible way to do damage, return
	if (!PossibleDamage)
		return;

	//SLOWWALK Stop
	float maxSpeed = G::LocalPlayer->MaxAccurateSpeed() / 3.f;
	maxSpeed -= 5;

	if (AutoStop->Get())
	{
		if (G::cmd->forwardmove && G::cmd->sidemove) {
			const float maxSpeedRoot = maxSpeed * static_cast<float>(M_SQRT1_2);
			G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
			G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
		}
		else if (G::cmd->forwardmove) {
			G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeed : maxSpeed;
		}
		else if (G::cmd->sidemove) {
			G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeed : maxSpeed;
		}

		FastStop();
	}

	if (!RageAimbot->Get())
		return;

	if (!AutoScope->Get())
		return;
	// AUTOSCOPE
	// THis shouldn't be here, but it makes sense 
	// If scoped already
	if (G::LocalPlayer->IsScoped() || G::LocalPlayerWeapon->ScopeLevel() > 0)
		return;

	int WeapIndex = GetWeaponIndex((WeaponId)weaponID);
	// if scoped weapon
	if (WeapIndex <= 34 && WeapIndex >= 29)
		G::cmd->buttons |= IN_ATTACK2;
		
}

void Movement::FastStop()
{
	if (!G::LocalPlayer) return;
	if (!G::LocalPlayerAlive) return;

	Entity* weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon) return;

	WeaponData* data = weapon->GetWeaponData();
	if (!data) return;

	auto get_standing_accuracy = [&]() -> const float
	{
		const auto max_speed = G::LocalPlayer->IsScoped() ? data->MaxSpeedAlt : data->MaxSpeed;
		return max_speed / 3.f;
	};

	Vec velocity = G::LocalPlayer->GetVecVelocity();
	QAngle direction = QAngle(0, RAD2DEG(atan2(velocity.y, velocity.x)), 0);
	float speed = velocity.VecLength2D();

	if (speed < get_standing_accuracy())
		return;

	//direction.y now holds delta between current viewangle, and velocity angle
	direction.y = G::cmd->viewangles.y - direction.y;

	//direction now holds forward/sidemove to achieve velocity angle (mentioned above)
	float SIN = sinf(DEG2RAD(direction.y)) * 450;
	float COS = cosf(DEG2RAD(direction.y)) * 450;
	direction.x = COS;
	direction.y = SIN;

	Vec negated_direction = direction * -speed;
	G::cmd->forwardmove = negated_direction.x;
	G::cmd->sidemove = negated_direction.y;
}

