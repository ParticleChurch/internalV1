#include "../Include.hpp"

AntiAim* antiaim = new AntiAim();

void AntiAim::legit()
{
	if (!Config::antiaim.Enable)
		return;
	if (!Config::antiaim.Legit.Enable)
		return;
		
	if (G::Localplayer->GetMoveType() == LADDER)
		return;



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
	if (!G::Localplayer->IsAlive())
		return;

	G::cmd->viewangles.x = 89;
	G::cmd->viewangles.y += 180;

	if (G::Localplayer->GetVecVelocity().VecLength() > 100)
		return;

	bool BreakLBY = LBYBreak();
	 
	static float Delta = 58.f;
	static bool right = false;
	if (Delta < -58.f) {
		right = true;
	}
	else if (Delta > 58.f)
		right = false;

	if (right)
		Delta += 2.f;
	else
		Delta -= 2.f;

	if (!BreakLBY)
	{
		float amount = Delta;
		if (*G::pSendPacket)				
			G::cmd->viewangles.y += amount;
	}
	
	if (BreakLBY)
	{
		float amount = Delta * -2;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}
}
