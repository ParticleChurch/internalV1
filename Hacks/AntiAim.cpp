#include "../Include.hpp"

AntiAim* antiaim = new AntiAim();

void AntiAim::legit()
{

	if (G::Localplayer->GetMoveType() == LADDER)
		return;

	bool BreakLBY = LBYBreak();
	
	//this is for left side
	int side = 1;
	if (!BreakLBY)
	{
		static float amount = 58.f * 2 * side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		static float amount = 58.f * -1 * side;
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
	G::cmd->viewangles.x = 89;
	G::cmd->viewangles.y += 180;

	bool BreakLBY = LBYBreak();

	//this is for left side
	int side = 1;

	if (!BreakLBY)
	{
		static float amount = 58.f * side;
		if (*G::pSendPacket)				
			G::cmd->viewangles.y += amount;
	}
	
	if (BreakLBY)
	{
		static float amount = 58.f * -2 * side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}
	
}
