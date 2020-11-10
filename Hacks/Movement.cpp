#include "../Include.hpp"

Movement* movement = new Movement();

void Movement::AAMoveFix()
{
	if ((G::Localplayer->GetHealth() > 0) && fabsf(G::cmd->sidemove) < 5.0f) {
		G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
	}
}

void Movement::BunnyHop()
{
	if ((G::cmd->buttons & IN_JUMP) && (G::Localplayer->GetHealth() > 0) && !(G::Localplayer->GetFlags() & FL_ONGROUND)
		&& G::Localplayer->GetMoveType() != MOVETYPE_LADDER) {
		G::cmd->buttons &= ~IN_JUMP;
	}
}

void Movement::FastCrouch()
{
	if (G::Localplayer->GetHealth() > 0)
		G::cmd->buttons |= IN_BULLRUSH;
}

void Movement::RageAutoStrafe()
{
	static bool flip = false;
	if (G::pSendPacket && !(*G::pSendPacket) && (G::Localplayer->GetHealth() > 0) && !(G::Localplayer->GetFlags() & FL_ONGROUND)
		&& G::Localplayer->GetMoveType() != MOVETYPE_LADDER) {
		bool Left = G::cmd->sidemove > 300;
		bool Right = G::cmd->sidemove < -300;
		bool Side = (Left || Right) && !(Left && Right);

		bool Forward = G::cmd->forwardmove > 300;
		bool Backward = G::cmd->forwardmove < -300;
		bool Frwd = (Forward || Backward) && !(Forward && Backward);

		float Goal = 0;
		Goal += Side ? (Frwd ? (Left ? 90 + (Forward ? -45 : 45) : 270 + (Forward ? 45 : -45)) : (Left ? 90 : 270)) : (Frwd ? (Forward ? 0 : 180) : 0);

		Vec Vel = G::Localplayer->GetVecVelocity();

		float yaw_change = 0.f;
		if (Vel.VecLength2D() > 50.f)
		{
			float desync = fabsf(G::Localplayer->GetMaxDesyncAngle()) * 0.9;
			yaw_change = desync * fabsf(desync / Vel.VecLength2D());
		}


		float Velocity = RAD2DEG(atan2(Vel.y, Vel.x));
		float VelDelta = G::CM_StartAngle.y - Velocity;
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
	bool valid = (G::Localplayer->GetHealth() > 0) && !(G::Localplayer->GetFlags() & FL_ONGROUND) && G::Localplayer->GetMoveType() != MOVETYPE_LADDER;
	if (!valid)
		return;

	if (G::cmd->mousedx < -5)
		G::cmd->sidemove = -450;
	else if(G::cmd->mousedx > 5)
		G::cmd->sidemove = 450;
}

void Movement::SlowWalk(float fraction)
{
	bool valid = (G::Localplayer->GetHealth() > 0) && (G::Localplayer->GetFlags() & FL_ONGROUND) && G::Localplayer->GetMoveType() != MOVETYPE_LADDER;
	if (!valid)
		return;

	float maxSpeed = G::Localplayer->MaxAccurateSpeed() * fraction;

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

