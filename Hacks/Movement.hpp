class Movement
{
public:
	void AAMoveFix();
	void BunnyHop();
	void FastCrouch();
	void RageAutoStrafe();
	void LegitAutoStrafe();
	void SlowWalk(float fraction = 0.333f);
	
};

extern Movement* movement;