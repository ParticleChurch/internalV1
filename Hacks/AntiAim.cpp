#include "../Include.hpp"

AntiAim* antiaim = new AntiAim();

void AntiAim::legit()
{
	if (!Config::antiaim.Enable)
		return;
	if (!Config::antiaim.Legit.Enable)
		return;

	if (!(G::Localplayer->GetHealth() > 0))
		return;
		
	if (G::Localplayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (G::Localplayer->GetVecVelocity().VecLength() > 100)
		return;

	if (fabsf(G::cmd->sidemove) < 5.0f) {
		G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
	}

	int side = 1;
	static float lastUpdate = 0;
	if (GetAsyncKeyState(Config::antiaim.Legit.InverterKey) &&
		fabsf(lastUpdate - I::globalvars->m_curTime) > 0.2f) {

		lastUpdate = I::globalvars->m_curTime;
		Left = !Left;
		side *= -1;
	}

	bool BreakLBY = LBYBreak();
	
	//this is for left side
	
	if (!BreakLBY)
	{
		float amount = 58.f * 2 * side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = 58.f * -1 * side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}
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

	if (G::Localplayer->GetVecVelocity().VecLength() > 100)
		return;

	if (fabsf(G::cmd->sidemove) < 5.0f) {
		G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
	}

	bool BreakLBY = LBYBreak();
	 
	static float Delta = 58.f;

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
}
