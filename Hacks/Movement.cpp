#include "../Include.hpp"

Movement* movement = new Movement();

void Movement::AAMoveFix()
{
	if ((G::LocalPlayer->GetHealth() > 0) && fabsf(G::cmd->sidemove) < 5.0f) {
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
	static bool flip = false;
	if (G::pSendPacket && !(*G::pSendPacket) && (G::LocalPlayer->GetHealth() > 0) && !(G::LocalPlayer->GetFlags() & FL_ONGROUND)
		&& G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER) {
		bool Left = G::cmd->sidemove > 300;
		bool Right = G::cmd->sidemove < -300;
		bool Side = (Left || Right) && !(Left && Right);

		bool Forward = G::cmd->forwardmove > 300;
		bool Backward = G::cmd->forwardmove < -300;
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
		float VelDelta = G::StartAngle.y - Velocity;
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
	}
}

void Movement::LegitAutoStrafe()
{
	bool valid = (G::LocalPlayer->GetHealth() > 0) && !(G::LocalPlayer->GetFlags() & FL_ONGROUND) && G::LocalPlayer->GetMoveType() != MOVETYPE_LADDER;
	if (!valid)
		return;

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

	float maxSpeed = G::LocalPlayer->MaxAccurateSpeed() * 0.333;
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
}

void Movement::Airstuck()
{
	// If Airstuck is NOT enabled in config...
	if (!Config::GetBool("misc-movement-airstuck"))
		return;

	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// Airstuck
	G::cmd->command_number = INT_MAX;
	G::cmd->tick_count = INT_MAX;
}

void Movement::FakeDuck()
{
	// If the localplayer is not alive...
	if (!G::LocalPlayer || !G::LocalPlayerAlive)
		return;

	// Set choke amount back to normal
	static int PrevChokeAmount = 0;
	static bool WasJustFakeducking = false;
	if (!Config::GetBool("misc-movement-fakeduck"))
	{
		if (WasJustFakeducking)
			G::ChokeAmount = PrevChokeAmount;
		else
			PrevChokeAmount = G::ChokeAmount;
	}
		
	// If FakeDuck is enabled in config...
	if (Config::GetBool("misc-movement-fakeduck"))
	{
		WasJustFakeducking = true;
		// Start the fakeduck, need to implement restoration of chokeamount
		G::ChokeAmount = 14;
		G::cmd->buttons |= IN_BULLRUSH;
		if (I::engine->GetNetChannelInfo()->ChokedPackets > (G::ChokeAmount / 2))
			G::cmd->buttons |= IN_DUCK;
		else
			G::cmd->buttons &= ~IN_DUCK;
	} else
		WasJustFakeducking = false;
}

void Movement::FollowBot()
{
	/*Entity* Leader;

	Vec cur_loc = G::Localplayer->GetVecOrigin();
	Vec ent_loc = Leader->GetVecOrigin();

	locs.push_back(ent_loc);
	while (locs.size() > 20) {
		locs.pop_front();
	}*/


}

