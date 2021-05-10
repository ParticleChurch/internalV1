#pragma once
// Structs
struct Tick
{
	// Matrixes
	Matrix3x4 Matrix[256];			// Original Matrix
	Matrix3x4 ResolvedMatrix[256];	// Resolved Matrix

	// Facts
	bool Dormant;			// Is player dormant
	Vec Velocity;			// Players velocity
	Vec Origin;				// Where is player
	Vec AbsOrigin;			// where is player abs
	Vec HeadPos;			// Head position
	Vec EyeAng;				// Eye Angles at current record
	Vec obb_mins;			// idek
	Vec obb_maxs;			// idek
	float SimulationTime;	// Sim time
	int TickCount;			// current tick count
	float Duck;				// How much duck
	float LBY;				// LowerBodyYawTarget
	float LastShotTime;		// Last shot time
	float SpawnTime;		// When did they spawn
	int Flags;				// Where are they
	int Choked;				// How many ticks choked
	bool Shot;				// Is player shooting this tick
};
struct Player
{
	// Big facts
	Entity* ptrEntity;		// Entity ptr
	Matrix3x4 Matrix[256];	// Original Matrix
	Entity* ptrWeap;		// Active weapon
	Model* ptrModel;		// Get model
	int Index;				// Index
	int Health;				// Health
	float SimulationTime;	// SimulationTime
	int Team;				// Team
	player_info_t info;		// UserInfo
	bool Valid;				// Valid updated player
	Vec Origin;				// Current Origin
	Vec AbsOrigin;			// where is player abs
	Vec obb_mins;			// idek
	Vec obb_maxs;			// idek
	Vec EyePos;				// Current EyePos
	bool Dormant;			// If entity is dormant
	int ShotsMissed;		// Shots missed

	std::deque<Tick> Records; // LagComp Records
};

class LagComp
{
private:
	bool ValidSimulationTime(float SimTime);
public:
	// Vars
	std::map<int, Player> PlayerList;

	// Funcs
	bool ValidRecord(Player player);
	void Update();
	void ClearRecords();
	void ClearPlayerList();
};

extern LagComp* lagcomp;

