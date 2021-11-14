#include "../../Include.hpp"

Movement* movement = new Movement();

void Movement::AAMoveFix()
{
	static Config::CState* LegitEnable = Config::GetState("antiaim-legit-enable");
	static Config::CState* RageEnable = Config::GetState("antiaim-rage-enable");
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
	static Config::CState* Enabled = Config::GetState("misc-movement-bhop");
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
	static Config::CState* Enabled = Config::GetState("misc-movement-fastcrouch");

	// If FastCrouch is NOT enabled in config...
	if (!Enabled->Get())
		return;

	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// Fastcrouch
	G::cmd->buttons |= IN_BULLRUSH;
}

void Movement::FastStopUser()
{
	static Config::CState* Enabled = Config::GetState("misc-movement-faststop");

	if (!Enabled->Get()) return;

	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	int movetype = G::LocalPlayer->GetMoveType();
	if (movetype == MOVETYPE_NOCLIP || movetype == MOVETYPE_LADDER || !(G::LocalPlayer->GetFlags() & 1) || G::cmd->buttons & IN_JUMP)
		return;

	if (G::cmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
		return;

	Vec velocity = G::LocalPlayer->GetVecVelocity();
	const float speed = velocity.VecLength2D();
	if (speed < 15.0f)
		return;

	QAngle direction = QAngle(0, RAD2DEG(atan2(velocity.y, velocity.x)), 0);

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

void Movement::RageAutoStrafe()
{
	static Config::CState* State = Config::GetState("misc-movement-autostrafe");

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
		float sin_rot = sinf(DEG2RAD(VelDir + looking + 1));
		float cos_rot = cosf(DEG2RAD(VelDir + looking + 1));

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
	static Config::CState* State = Config::GetState("misc-movement-autostrafe");

	if (State->Get() != 2)
		return;

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetFlags() & FL_ONGROUND) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	if (G::cmd->mousedx < -5)
		G::cmd->sidemove = -450;
	else if (G::cmd->mousedx > 5)
		G::cmd->sidemove = 450;
}

void Movement::SlowWalk()
{

	static Config::CState* Enabled = Config::GetState("misc-movement-slowwalk");
	static Config::CFloat* Speed = Config::GetFloat("misc-movement-slowwalk-speed");

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

	static Config::CState* Enabled = Config::GetState("misc-movement-fakeduck");

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
	static Config::CState* Enabled = Config::GetState("misc-movement-leg");
	static Config::CFloat* LegTime = Config::GetFloat("misc-movement-leg-time");

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

	if (switcher)
		G::cmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
}

void Movement::AutoStop()
{
	static Config::CState* RageAimbot = Config::GetState("rage-aim-enable");
	static Config::CState* AutoScope = Config::GetState("rage-aim-autoscope");
	static Config::CState* AutoStop = Config::GetState("misc-movement-autostop");

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

	// If attempting bhop or holding jump...
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

	L::Debug("Autostop autowall checking");

	// only do what aimbot sees as important
	int scans = 1;
	for (auto& a : aimbot->players)
	{
		Player p = lagcomp->PlayerList[a.first];
		if (p.Index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(p.ptrEntity)) // entity DOES NOT exist
			continue;

		if (!(p.Health > 0)) // entity is NOT alive
			continue;

		if (p.Team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (p.Dormant)	// Entity is dormant
			continue;

		Vec vel = p.ptrEntity->GetVecVelocity();
		vel *= (I::globalvars->m_intervalPerTick * 1); // one tick forward prediciton
		//velocity now holds our distance to move

		// Updating their next position
		Vec nPos = p.EyePos + vel;

		// Get damage from next position... (LOTS OF AWALL, idk what i'm supposed to do lmao)
		// --> reduced slightly with clipraytoentity
		// autowall if we can clipray
		if (autowall2->CanHitFloatingPoint(nPos, NextPos))
		{
			PossibleDamage = true;
			break;
		}
		scans++;
		// if we are going to scan too many people, return
		if (scans > aimbot->maxplayerscan - 1)
			return;
		
	}
	L::Debug("Autostop autowall - done");

	// If there's no possible way to do damage, return
	if (!PossibleDamage)
		return;

	//SLOWWALK Stop
	float maxSpeed = G::LocalPlayer->MaxAccurateSpeed() / 3.f;
	maxSpeed *= 0.9f; // force to slow down to 0.9 of max accurate speed

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

