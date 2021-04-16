#pragma once

class Resolver
{
private:
	// Deal with Gameevents
	int ImpactEndUserID;
	int ImpactHitgroup;

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

	// userid, originalyaw
	std::map<int, float> OrigYaw;

	// userid, aimbot priority/reliability
	std::map<int, int> Priority;
	int GetPriority(int UserID)
	{
		if (Priority.find(UserID) == Priority.end())
			return 0;
		return Priority[UserID];
	}

	//crappy resolver flags
	std::string ResolverFlag[64];

	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void Resolve();
	void AnimationFix(Entity* entity);
	void BruteForce(Entity* entity, int index);
	
	void PreResolver(int stage);
	void PostResolver(int stage);

};

extern Resolver* resolver;