#pragma once
class FakeLag
{
private:
	Vec PrevPos;
	float DistPrevPos; // 64 units
	Vec NextPos;
	bool DistanceBreaker();
	bool TimeBreaker();
public:
	bool Run();
};

extern FakeLag* fakelag;

