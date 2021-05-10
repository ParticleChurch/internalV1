#pragma once

struct PlayerRes
{
	// Log Shots
	int OldShotsMissed = 0;
	int ShotsMissed = 0;
	bool LogShot = false;
	// Resolver Specific
	float SimTime = 0;
	float OldSimTime = 0;
	bool IsDesyncing = false;
	std::string ResolverFlag;
	// Hitting ONSHOT
	bool Shot = false;
	float ShotTime = 0.f;
	float LookAtLocalAng = 0.f;
	QAngle PrevAng;
	int LagTime = 0;
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
	// for logging pred error
	bool LogPredError = false;
	// For logging spread
	bool LogShot = false;
	bool BacktrackShot = false;
	int UserID = -1;

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
	void FindShot(Entity* entity, int UserID);
	float GetBackwardYaw(Entity* entity);
	void DetectSide(Entity* entity, int* side);

};

extern Resolver* resolver;