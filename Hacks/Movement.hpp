class Movement
{
public:
	void AAMoveFix();
	void BunnyHop();
	void FastCrouch();
	void RageAutoStrafe();
	void LegitAutoStrafe();
	void SlowWalk();
	void Airstuck();
	void FakeDuck();

	void FastStop();
};

extern Movement* movement;