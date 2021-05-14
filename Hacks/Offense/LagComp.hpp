#pragma once
// Structs
struct Tick
{
	// Matrixes
	Matrix3x4 Matrix[128];			// Original Matrix

	// Facts
	bool Dormant;			// Is player dormant
	Vec Velocity;			// Players velocity
	Vec Origin;				// Where is player
	Vec AbsOrigin;			// where is player abs
	Vec HeadPos;			// Head position
	Vec EyeAng;				// Eye Angles at current record
	float SimulationTime;	// Sim time
	float Duck;				// How much duck
	float LBY;				// LowerBodyYawTarget
	int Flags;				// Where are they
	int Choked;				// How many ticks choked
};

struct Player
{
	// Big facts
	Entity* ptrEntity;		// Entity ptr
	Matrix3x4 Matrix[128];	// Original Matrix
	Entity* ptrWeap;		// Active weapon
	Model* ptrModel;		// Get model
	int Index;				// Index
	int Health;				// Health
	float SimulationTime;	// SimulationTime
	int Team;				// Team
	player_info_t info;		// UserInfo
	bool Valid;				// Valid updated player
	Vec Origin;				// Current Origin
	Vec EyePos;				// Current EyePos
	bool Dormant;			// If entity is dormant

	std::deque<Tick> Records; // LagComp Records
};

class LagComp
{
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

