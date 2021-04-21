#pragma once

struct PlayerRes
{
	// Log Shots
	int OldShotsMissed = 0;
	int ShotsMissed = 0;
	// Resolver Specific
	float SimTime = 0;
	float OldSimTime = 0;
	bool IsDesyncing = false;
	std::string ResolverFlag;
	// Hitting ONSHOT
	bool Shot = false;
	float ShotTime = 0.f;
	float LookAtLocalAng = 0.f;
	//bool IsDormant = true;
	//bool IsMoving = false;
	//bool IsOnGround = false;
	//bool IsInAir = false;

	// Aimbot
	int priority = 0;

};

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
	//userid, PlayerRes
	std::map<int, PlayerRes> PlayerInfo;

	// Gets priority for a given userid
	int GetPriority(int UserID)
	{
		if (PlayerInfo.find(UserID) == PlayerInfo.end())
			return 0;
		return PlayerInfo[UserID].priority;
	}

	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void AnimationFix(Entity* entity);

	void Resolve();

	void ResolveEnt(Entity* entity, int Index);
	void UpdateEnt(Entity* entity, int UserID);

};

extern Resolver* resolver;