#include "../Include.hpp"

AntiAim* antiaim = new AntiAim();

void AntiAim::legit()
{
	//if (!Config::antiaim.Enable)
	//	return;
	//if (!Config::antiaim.Legit.Enable)
	//	return;

	if (!G::Localplayer)
		return;

	if (!(G::Localplayer->GetHealth() > 0))
		return;
		
	if (G::Localplayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	static float lastUpdate = 0;
	if (GetAsyncKeyState(Config::antiaim.Legit.InverterKey) &&
		fabsf(lastUpdate - I::globalvars->m_curTime) > 0.2f) {

		lastUpdate = I::globalvars->m_curTime;
		Left = !Left;
		side *= -1;
	}

	bool BreakLBY = LBYBreak();
	float Delta = G::Localplayer->GetMaxDesyncAngle();
	
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

	fake.Normalize();
	real.Normalize();

	H::console.clear();
	H::console.resize(0);
	H::console.push_back(fake.str());
	H::console.push_back(real.str());
}

bool AntiAim::LBYBreak()
{
	static float NextUpdate = 0;
	float velocity = G::Localplayer->GetVecVelocity().VecLength2D();
	float time = G::Localplayer->GetServerTime();
	if (!(G::Localplayer->GetFlags() & 1) || (!(G::Localplayer->IsAlive()))) {
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
	if (!(G::Localplayer->GetHealth() > 0))
		return;

	G::cmd->viewangles.x = 89;
	G::cmd->viewangles.y += 180;



	real = fake = G::cmd->viewangles;
	real.Normalize();
	fake.Normalize();

	bool BreakLBY = LBYBreak();
	float Delta = G::Localplayer->GetMaxDesyncAngle();

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

	real.Normalize();
	fake.Normalize();
}
