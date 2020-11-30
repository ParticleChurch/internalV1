#include "../../Include.hpp"

FakeLag* fakelag = new FakeLag();

bool FakeLag::DistanceBreaker()
{
	// If our next position breaks it...(64 units) (64*64 = 4096)
	if ((NextPos - PrevPos).VecLength() > Config::GetFloat("antiaim-fakelag-distance"))
		return true;
	return false;
}

bool FakeLag::TimeBreaker()
{
	// we can't choke more than 62 ticks because of some protection (prev exploits)
	// which is around 16 packets (a more accurate way would be to count ticks, meh)
	if (I::engine->GetNetChannelInfo()->ChokedPackets >= Config::GetFloat("antiaim-fakelag-tick"))
		return true;
	return false;
}

bool FakeLag::Run()
{
	// Fake duck exception
	if (Config::GetBool("misc-movement-fakeduck"))
	{
		return I::engine->GetNetChannelInfo()->ChokedPackets >= G::ChokeAmount;
	}
	Vec velocity = G::LocalPlayer->GetVecVelocity();
	velocity *= I::globalvars->m_intervalPerTick;
	//velocity now holds our distance to move

	// Updating our next position
	NextPos = G::LocalPlayer->GetVecOrigin() + velocity;
	if (DistanceBreaker() || TimeBreaker())
	{
		PrevPos = G::LocalPlayer->GetVecOrigin();
		return true;
	}

	return false;
}
