#pragma once
class NetChannel {
public:
	//VIRTUAL_METHOD(float, getLatency, 9, (int flow), (this, flow))
	float GetLatency(int flow)
	{
		typedef float(__thiscall* ogetLatency)(void*, int);
		return GetVFunc<ogetLatency>(this, 9)(this, flow);
	}

	std::byte pad[44];
	int ChokedPackets;
};