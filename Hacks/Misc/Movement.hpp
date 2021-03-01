class Movement
{
public:
	void AAMoveFix();
	void BunnyHop();
	void FastCrouch();
	void RageAutoStrafe();
	void LegitAutoStrafe();
	void SlowWalk();
	void FakeDuck();
	void LegSlide();

	void ActiveCounterStrafe();

	void FastStop();
};

extern Movement* movement;