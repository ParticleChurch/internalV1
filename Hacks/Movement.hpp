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

	// Maybe walkbot (LOL)
	std::deque<Vec> locs;
	void FollowBot();
};

extern Movement* movement;