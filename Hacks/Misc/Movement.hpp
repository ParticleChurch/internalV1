class Movement
{
public:
	void AAMoveFix();
	void BunnyHop();
	void FastCrouch();
	void FastStopUser();
	void RageAutoStrafe();
	void LegitAutoStrafe();
	void SlowWalk();
	void FakeDuck();
	void LegSlide();

	void AutoStop();

	void FastStop();
};

extern Movement* movement;