#include "../../Include.hpp"

FakeLag* fakelag = new FakeLag();

bool FakeLag::DistanceBreaker()
{
	// If our next position breaks it...(64 units)
	H::console.push_back("Dist: " + std::to_string((NextPos - PrevPos).VecLength()));
	if ((NextPos - PrevPos).VecLength() > 4096)
		return true;
	return false;
}

bool FakeLag::TimeBreaker()
{
	// we can't choke more than 62 ticks because of some protection (prev exploits)
	// which is around 16 packets 
	H::console.push_back("Pack: " + std::to_string(I::engine->GetNetChannelInfo()->ChokedPackets));
	if (I::engine->GetNetChannelInfo()->ChokedPackets > 16)
		return true;
	return false;
}

bool FakeLag::Run()
{
	H::console.clear();
	H::console.resize(0);
	Vec velocity = G::LocalPlayer->GetVecVelocity();
	velocity *= I::globalvars->m_intervalPerTick;
	//velocity now holds our distance to move

	

	// Updating our next position
	NextPos = G::LocalPlayer->GetVecOrigin() + velocity;
	H::console.push_back("Current Pos: " + G::LocalPlayer->GetVecOrigin().str());
	H::console.push_back("Next Pos: " + NextPos.str());
	if (DistanceBreaker() || TimeBreaker())
	{
		PrevPos = G::LocalPlayer->GetVecOrigin();
		return true;
	}

	return false;
}
