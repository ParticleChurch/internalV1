#pragma once

class Resolver
{
private:
public:
	int OldShotsMissed[64];
	int ShotsMissed[64];
	std::string ResolverFlag[64];

	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void Resolve(int stage);
	void AnimationFix(Entity* entity);

	void BruteForce(Entity* entity, int index);

};

extern Resolver* resolver;