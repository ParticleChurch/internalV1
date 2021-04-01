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

	bool DamageIncoming = false;

	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		if (it->second.index == G::LocalPlayerIndex) // entity is Localplayer
			continue;

		if (!(it->second.entity)) // entity DOES NOT exist
			continue;

		if (!(it->second.health > 0)) // entity is NOT alive
			continue;

		if (it->second.team == G::LocalPlayerTeam) // Entity is on same team
			continue;

		if (it->second.dormant)	// Entity is dormant
			continue;

		if (!ValidSimTime(it->second.CurSimTime)) // if not valid simtime
			continue;

		if (autowall->Damage(NextPos) > 1)
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

	static Config2::CFloat* FakeLagTrigDist = Config2::GetFloat("antiaim-fakelag-trigger-distance");
	static Config2::CFloat* FakeLagTrigTick = Config2::GetFloat("antiaim-fakelag-trigger-tick");

	// If there we should lag on peak... 
	if (LaggingOnPeak)
	{
		TrigDistance = FakeLagTrigDist->Get();
		TrigTicks = FakeLagTrigTick->Get(); 
	}
}

void FakeLag::Start()
{
	static Config2::CState* FakeDuck = Config2::GetState("misc-movement-fakeduck");
	static Config2::CFloat* FakeLagDist = Config2::GetFloat("antiaim-fakelag-distance");
	static Config2::CFloat* FakeLagTicks = Config2::GetFloat("antiaim-fakelag-tick");

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
