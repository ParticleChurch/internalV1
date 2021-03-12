#pragma once

class Resolver
{
private:
	// Deal with Gameevents
	int ImpactEndUserID;

	// Deal with weaponFireEvent
	void LogWeaponFire(GameEvent* event);

	// Deal with LogBulletImpact
	void LogBulletImpact(GameEvent* event);

	// Deal with PlayerHurt
	void LogPlayerHurt(GameEvent* event);
public:
	// userid, shots
	std::map<int, int> OldShotsMissed;
	std::map<int, int> ShotsMissed;

	// userid, aimbot priority/reliability
	std::map<int, int> Priority;

	//crappy resolver flags
	std::string ResolverFlag[64];

	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void Resolve(int stage);
	void AnimationFix(Entity* entity);

	void BruteForce(Entity* entity, int index);

};

extern Resolver* resolver;