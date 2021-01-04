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
	bool PredictedVal = true;
	void Start();
	bool End();
};

extern FakeLag* fakelag;

