#include "../Include.hpp"

AntiAim* antiaim = new AntiAim();

void AntiAim::legit()
{
	//if (!Config::antiaim.Enable)
	//	return;
	//if (!Config::antiaim.Legit.Enable)
	//	return;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;
		
	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		return;

	static float lastUpdate = 0;
	if ( // TODO: Config::GetState("antiaim-inversion")
		fabsf(lastUpdate - I::globalvars->m_curTime) > 0.2f
	) {

		lastUpdate = I::globalvars->m_curTime;
		Left = !Left;
		side *= -1;
	}

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle();
	
	//side by default = left
	if (!BreakLBY)
	{
		float amount = Delta * 2 * side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = Delta * -1 * side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket)
		fake = G::cmd->viewangles;
	else if (!BreakLBY)
	{
		real.x = G::cmd->viewangles.x;
		real.y = G::cmd->viewangles.y + (Delta * side * -1);
	}

	fake.NormalizeAngle();
	real.NormalizeAngle();
}

bool AntiAim::LBYBreak()
{
	static float NextUpdate = 0;
	float velocity = G::LocalPlayer->GetVecVelocity().VecLength2D();
	float time = G::LocalPlayer->GetServerTime();
	if (!(G::LocalPlayer->GetFlags() & 1) || (!(G::LocalPlayer->IsAlive()))) {
		return false;
	}
	if (velocity > 0.1f) {
		NextUpdate = time + 0.22f;
	}
	if (NextUpdate <= time)
	{
		NextUpdate = time + 1.1f;
		return true;
	}

	return false;
}

void AntiAim::rage()
{
	if (!(G::LocalPlayer->GetHealth() > 0))
		return;

	G::cmd->viewangles.x = 89;
	G::cmd->viewangles.y += 180;

	real = fake = G::cmd->viewangles;
	real.NormalizeAngle();
	fake.NormalizeAngle();

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle();

	if (!BreakLBY)
	{
		static float amount = Delta;
		if (*G::pSendPacket)				
			G::cmd->viewangles.y += amount;
	}
	
	if (BreakLBY)
	{
		static float amount = Delta * -2;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket)
		fake = G::cmd->viewangles;
	else
		real = G::cmd->viewangles;

	real.NormalizeAngle();
	fake.NormalizeAngle();
}
