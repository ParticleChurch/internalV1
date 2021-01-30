#pragma once

class Resolver
{
private:
	// 64 is the number of players
	Vec Loc;
	bool Hit[64];
	
public:
	int ShotsMissed[64];


	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void Resolve(int stage);

	void AnimationFix(Entity* entity);
	void ABSROTATION(Entity* entity);

	
};

extern Resolver* resolver;