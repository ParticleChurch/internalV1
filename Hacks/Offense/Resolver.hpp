#pragma once

#define BRUTE_FORCE 0
#define JITTER_METHOD 1
#define BACK_METHOD 2

struct PlayerRes
{
	// Log Shots
	int OldShotsMissed = 0;
	int ShotsMissed = 0;
	bool LogShot = false;

	// Dumb Esp Tag
	std::string ResolverFlag;

	// Resolver Specific
	int LagTime = 0;			// How many ticks does user lag
	float SimTime = 0;			// Cur Sim time
	float OldSimTime = 0;		// Old Sim Time
	bool IsDesyncing = false;	// Are they desyncing
	

	// Resolve stuff
	float OffsetAngle = 0.f;
	// shots missed before we changed method of resolving
	int BeforeShotMissed = 0;
	int ResolveMethod = BRUTE_FORCE; //default to brute force
	//Jitter
	int FreestandSide;
	float LastAngle;
	float LastBrute;
	float Switch;
	float LastUpdateTime;
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
	int AimbotUserID = -1;

	//userid, PlayerRes
	std::map<int, PlayerRes> PlayerInfo;
	void LogShots(GameEvent* event);
	void AnimationFix(Entity* entity);

	void Resolve();

	void ResolveEnt(Entity* entity, int Index);
	bool DoesHaveJitter(Entity* entity, int UserID, int& newside);
	float GetBackwardsYaw(Entity* entity);
	// Specific to entities

};

extern Resolver* resolver;