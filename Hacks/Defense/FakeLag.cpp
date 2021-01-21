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
	/*float latency = 0;
	NetChannel* info = I::engine->GetNetChannelInfo();
	if (info)
		latency += info->GetLatency(0) + info->GetLatency(1);
*/

	auto network = I::engine->GetNetChannelInfo();
	float delta = 0.f;
	if (network)
	{
		delta = network->GetLatency(0) + network->GetLatency(1);
	}
	
	if (I::engine->GetNetChannelInfo()->ChokedPackets + int(delta / I::globalvars->m_intervalPerTick) >= TrigTicks)
		return true;
	return false;
}

void FakeLag::LagOnPeak()
{
	if (!G::LocalPlayer)
		return;
	if (!G::LocalPlayerAlive)
		return;

	bool DamageIncoming = false;
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
			DamageIncoming = true;
			break;
		}
	}
	
	static bool ChokeOnce = false;

	// If there is damage incoming, we are about to send packets, and we have not choked before
	if (DamageIncoming && PredictedVal && !ChokeOnce)
	{
		ChokeOnce = true;				// We have choked once
		LaggingOnPeak = true;
	}
	else if(!DamageIncoming && ChokeOnce) // no damage incoming, reset the choke once
	{
		ChokeOnce = false;
	}

	// If there we should lag on peak... 
	if (LaggingOnPeak)
	{
		TrigDistance = Config::GetFloat("antiaim-fakelag-trigger-distance");
		TrigTicks = Config::GetFloat("antiaim-fakelag-trigger-tick");
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
