#include "../../Include.hpp"

Movement* movement = new Movement();

void Movement::AAMoveFix()
{
	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// If not on ground
	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND)) return;

	// If neither options are ticked... return
	if (!Config::GetBool("antiaim-legit-enable") && !Config::GetBool("antiaim-rage-enable")) return;

	if (fabsf(G::cmd->sidemove) < 5.0f) {
		G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
	}
}

void Movement::BunnyHop()
{
	// If bhop is NOT enabled in config...
	if (!Config::GetBool("misc-movement-bhop"))
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
	// If FastCrouch is NOT enabled in config...
	if (!Config::GetBool("misc-movement-fastcrouch"))
		return;

	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// Fastcrouch
	G::cmd->buttons |= IN_BULLRUSH;
}

void Movement::RageAutoStrafe()
{
	return;
	// if (Config::GetState("misc-movement-autostrafe") != 1) return;
	Config::SetBool("misc-movement-bhop", true);

	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetFlags() & FL_ONGROUND) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	bool Left = G::StartSideMove > 300;
	bool Right = G::StartSideMove < -300;
	bool Side = (Left || Right) && !(Left && Right);

	bool Forward = G::StartForwardMove > 300;
	bool Backward = G::StartForwardMove < -300;
	bool Frwd = (Forward || Backward) && !(Forward && Backward);

	float Goal = 0;
	Goal += Side ? (Frwd ? (Left ? 90 + (Forward ? -45 : 45) : 270 + (Forward ? 45 : -45)) : (Left ? 90 : 270)) : (Frwd ? (Forward ? 0 : 180) : 0);
	while (Goal > 180)
		Goal -= 360;
	Goal *= -1;

	Vec Vel = G::LocalPlayer->GetVecVelocity();

	float yaw_change = 0.f;
	if (Vel.VecLength2D() > 50.f)
	{
		float desync = fabsf(G::LocalPlayer->GetMaxDesyncAngle()) * 0.9;
		yaw_change = desync * fabsf(desync / Vel.VecLength2D());
	}

	float VelDir	= -RAD2DEG(atan2(Vel.y, Vel.x));		// Velocity direction
	float GoalDir	= G::StartAngle.y - Goal;			// Goal Direction
	while (GoalDir > 180) 
		GoalDir -= 360;
	while (GoalDir < -180) 
		GoalDir += 360;

	float VelDelta	= VelDir - GoalDir;			// velocity direction difference between where we are going, and wanting to go
	while (VelDelta > 180)
		VelDelta -= 360;
	while (VelDelta < -180)
		VelDelta += 360;
	VelDelta = fabsf(VelDelta);

	float SIN = sinf(DEG2RAD(VelDelta)) * 450;
	float COS = cosf(DEG2RAD(VelDelta)) * 450;
	
	//SIN COS - 
	//ROTATE CW
	/*G::cmd->forwardmove = SIN;
	G::cmd->sidemove = COS;*/

	/*G::cmd->forwardmove = SIN;
	G::cmd->sidemove = COS;*/
	

	/*static bool flip = false;
	if (G::pSendPacket && !(*G::pSendPacket) && G::LocalPlayerAlive && !(G::LocalPlayer->GetFlags() & FL_ONGROUND)
		&& G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER) {
		bool Left = G::StartSideMove > 300;
		bool Right = G::StartSideMove < -300;
		bool Side = (Left || Right) && !(Left && Right);

		bool Forward = G::StartForwardMove > 300;
		bool Backward = G::StartForwardMove < -300;
		bool Frwd = (Forward || Backward) && !(Forward && Backward);

		float Goal = 0;
		Goal += Side ? (Frwd ? (Left ? 90 + (Forward ? -45 : 45) : 270 + (Forward ? 45 : -45)) : (Left ? 90 : 270)) : (Frwd ? (Forward ? 0 : 180) : 0);

		Vec Vel = G::LocalPlayer->GetVecVelocity();

		float yaw_change = 0.f;
		if (Vel.VecLength2D() > 50.f)
		{
			float desync = fabsf(G::LocalPlayer->GetMaxDesyncAngle()) * 0.9;
			yaw_change = desync * fabsf(desync / Vel.VecLength2D());
		}


		float Velocity = RAD2DEG(atan2(Vel.y, Vel.x));
		float VelDelta = G::cmd->viewangles.y - Velocity;
		static float Angle = 180;
		Angle = VelDelta + 180;
		while (Angle > 180) {
			Angle -= 360;
		}

		while (Angle < -180) {
			Angle += 360;
		}

		Goal -= 180;
		float Delta = Goal - Angle;

		while (Delta > 180) {
			Delta -= 360;
		}

		while (Delta < -180) {
			Delta += 360;
		}

		if (fabsf(Delta) > yaw_change)
		{
			Delta *= (yaw_change / fabsf(Delta));
		}

		Angle += Delta;

		flip = !flip;
		float SIN = sinf(DEG2RAD(Angle)) * 450;
		float COS = cosf(DEG2RAD(Angle)) * 450;

		G::cmd->sidemove = flip ? COS : -COS;
		G::cmd->forwardmove = flip ? -SIN : SIN;
		G::cmd->viewangles.y += flip ? -yaw_change : yaw_change;

		G::CM_Clamp(); 
	}*/
}

void Movement::LegitAutoStrafe()
{
	if (Config::GetState("misc-movement-autostrafe") != 1)
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
	// If slowalk is NOT enabled in config...
	if (!Config::GetBool("misc-movement-slowwalk"))
		return;

	// valid - valid time to slow walk
	bool valid = G::LocalPlayer && G::LocalPlayerAlive && (G::LocalPlayer->GetFlags() & FL_ONGROUND) && G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER;
	// If not a valid time to slow walk
	if (!valid)
		return;

	float maxSpeed = G::LocalPlayer->MaxAccurateSpeed() / 3.f;
	maxSpeed -= 5;
	maxSpeed *= (Config::GetFloat("misc-movement-slowwalk-speed") / 100.f);

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

	// If FakeDuck is NOT enabled in config...
	if (!Config::GetBool("misc-movement-fakeduck")) return;
	
	// FAKEDUCK
	G::cmd->buttons |= IN_BULLRUSH;
	if (I::engine->GetNetChannelInfo()->ChokedPackets > (14 / 2))
		G::cmd->buttons |= IN_DUCK;
	else
		G::cmd->buttons &= ~IN_DUCK;
}

void Movement::LegSlide()
{
	if (!G::LocalPlayer) return;

	if (!G::LocalPlayerAlive) return;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;

	if (!Config::GetBool("misc-movement-leg")) return;

	static bool switcher = false;
	static float time = I::globalvars->m_curTime;
	if (I::globalvars->m_curTime - time > Config::GetFloat("misc-movement-leg-time") / 1000.f)
	{
		time = I::globalvars->m_curTime;
		switcher = !switcher;
	}

	if(switcher)
		G::cmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
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

