#include "../../Include.hpp"

FakeLag* fakelag = new FakeLag();

bool FakeLag::DistanceBreaker()
{
	// If our next position breaks it...(64 units) (64*64 = 4096)
	if ((NextPos - PrevPos).VecLength() > TrigDistance)
		return true;
	return false;
}

bool FakeLag::TimeBreaker()
{
	// we can't choke more than 62 ticks because of some protection (prev exploits)
	// which is around 16 packets (a more accurate way would be to count ticks, meh)
	if (I::engine->GetNetChannelInfo()->ChokedPackets >= TrigTicks)
		return true;
	return false;
}

void FakeLag::LagOnPeak()
{
	LaggingOnPeak = false;
	if (!G::LocalPlayer)
		return;
	if (!G::LocalPlayerAlive)
		return;

	for (auto a : G::EntList)
	{
		if (a.index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(a.entity)) // entity DOES NOT exist
			continue;

		if (!a.player) // entity is NOT player
			continue;

		if (!(a.health > 0)) // entity is NOT alive
			continue;

		if (a.team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (a.dormant)	// Entity is dormant
			continue;

		if (!backtrack->Valid(a.lastSimTime)) // if not valid simtime
			continue;

		if (autowall->GetDamage(a.entity, G::LocalPlayer, NextPos) > 1)
		{
			// For now
			TrigDistance = Config::GetFloat("antiaim-fakelag-trigger-distance");
			TrigTicks = Config::GetFloat("antiaim-fakelag-trigger-tick");
			LaggingOnPeak = true;
			return;
		}
	}
}

void FakeLag::Start()
{
	// Fake duck exception
	if (Config::GetBool("misc-movement-fakeduck"))
	{
		TrigDistance = 64.f;
		TrigTicks = 14;
		PredictedVal = I::engine->GetNetChannelInfo()->ChokedPackets >= TrigTicks;
		return;
	}

	// Get Defualt Limits
	TrigDistance = Config::GetFloat("antiaim-fakelag-distance");
	TrigTicks = Config::GetFloat("antiaim-fakelag-tick");

	// Update Velocity
	Vec velocity = G::LocalPlayer->GetVecVelocity();
	velocity *= (I::globalvars->m_intervalPerTick * 2); 
	//velocity now holds our distance to move (doubled for safety)

	// Updating our next position
	NextPos = G::LocalPlayer->GetEyePos() + velocity;

	// Updating limits if trigger 
	LagOnPeak();

	// If breaking distance or time..
	if (DistanceBreaker() || TimeBreaker())
	{
		PrevPos = G::LocalPlayer->GetEyePos();
		PredictedVal =  true;
		return;
	}

	PredictedVal = false;
}

bool FakeLag::End()
{
	return PredictedVal;
}
