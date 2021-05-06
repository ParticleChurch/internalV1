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

	if (I::engine->GetNetChannelInfo()->ChokedPackets - 1 > TrigTicks)
		return true;
	return false;
}

void FakeLag::LagOnPeak()
{
	if (!G::LocalPlayer)
		return;
	if (!G::LocalPlayerAlive)
		return;

	static Config::CFloat* FakeLagTrigDist = Config::GetFloat("antiaim-fakelag-trigger-distance");
	static Config::CFloat* FakeLagTrigTick = Config::GetFloat("antiaim-fakelag-trigger-tick");

	// no point in autowalling/lagging... if values are too low
	if (FakeLagTrigDist->Get() < 1 || FakeLagTrigTick->Get() < 1)
	{
		LaggingOnPeak = false;
		return;
	}

	// no point in lagging if velocity is too low
	if (int(G::LocalPlayer->GetVecVelocity().VecLength2D()) < 100)
	{
		LaggingOnPeak = false;
		return;
	}

	bool DamageOutgoing = false;

	std::map<int, Player>::iterator it;
	for (it = lagcomp->PlayerList.begin(); it != lagcomp->PlayerList.end(); it++)
	{
		if (it->second.Index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(it->second.ptrEntity)) // entity DOES NOT exist
			continue;

		if (!(it->second.Health > 0)) // entity is NOT alive
			continue;

		if (it->second.Team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (it->second.Dormant)	// Entity is dormant
			continue;

		if (!ValidSimTime(it->second.Records.front().SimulationTime)) // if not valid simtime
			continue;

		if (autowall->CanHitFloatingPoint(NextPos, it->second.EyePos, true))
		{
			DamageOutgoing = true;
			break;
		}
	}



	static bool ChokeOnce = false;

	// If there is damage incoming, we are about to send packets, and we have not choked before
	if (DamageOutgoing && PredictedVal && !ChokeOnce)
	{
		ChokeOnce = true;				// We have choked once
		LaggingOnPeak = true;
	}
	else if (!DamageOutgoing && ChokeOnce) // no damage incoming, reset the choke once
	{
		ChokeOnce = false;
	}



	// If there we should lag on peak... 
	if (LaggingOnPeak)
	{
		TrigDistance = FakeLagTrigDist->Get();
		TrigTicks = FakeLagTrigTick->Get();
	}
}

void FakeLag::Start()
{
	static Config::CState* FakeDuck = Config::GetState("misc-movement-fakeduck");
	static Config::CFloat* FakeLagDist = Config::GetFloat("antiaim-fakelag-distance");
	static Config::CFloat* FakeLagTicks = Config::GetFloat("antiaim-fakelag-tick");

	// Fake duck exception
	if (FakeDuck->Get())
	{
		TrigDistance = 64.f;
		TrigTicks = 14;
		PredictedVal = I::engine->GetNetChannelInfo()->ChokedPackets >= TrigTicks;
		return;
	}

	// Get Defualt Limits
	TrigDistance = FakeLagDist->Get();
	TrigTicks = FakeLagTicks->Get();

	// Update Velocity
	Vec velocity = G::LocalPlayer->GetVecVelocity();
	velocity *= (I::globalvars->m_intervalPerTick * 2);
	//velocity now holds our distance to move (doubled for safety)

	// Updating our next position
	NextPos = G::LocalPlayer->GetEyePos() + velocity;

	// If breaking distance or time --> Send packets
	PredictedVal = DistanceBreaker() || TimeBreaker();

	// Updating Limits if trigger 
	LagOnPeak();

	// If breaking distance or time --> Send packets
	PredictedVal = DistanceBreaker() || TimeBreaker();

	if (PredictedVal)
		PrevPos = G::LocalPlayer->GetEyePos();

	if (LaggingOnPeak && PredictedVal)
		LaggingOnPeak = false;
}

bool FakeLag::End()
{
	return PredictedVal;
}
