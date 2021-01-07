#pragma once
class FakeLag
{
private:
	Vec PrevPos;
	float DistPrevPos; // 64 units
	Vec NextPos;

	// Trigger Limits
	float TrigDistance;
	float TrigTicks;

	bool DistanceBreaker();
	bool TimeBreaker();
	void LagOnPeak();
public:
	bool PredictedVal = true;
	bool LaggingOnPeak = false;
	void Start();
	bool End();
};

extern FakeLag* fakelag;

