#pragma once
class LagComp
{
public:
	struct Tick
	{
		// Matrixes
		Matrix3x4 Matrix[256];			// Original Matrix
		Matrix3x4 ResolvedMatrix[256];	// Resolved Matrix

		// Big facts
		bool Dormant;			// Is player dormant
		Vec Velocity;			// Players velocity
		Vec Origin;				// Where is player
		Vec AbsOrigin;			// ABS where is player
		Vec VecMins;			// Min loc
		Vec VecMaxs;			// Max loc
		float SimulationTime;	// Sim time
		float InterpTime;		// InterpTime
		float Duck;				// How much duck
		float LBY;				// LowerBodyYawTarget
		float LastShotTIme;		// Last shot time
		float SpawnTime;		// When did they spawn
		int Flags;				// Where are they
		int EFlags;				// IDK weird animstate crap
		int Effects;			// IDK weird animstate crap
		int Choked;				// How many ticks choked
		bool Shot;				// Is player shooting this tick

		// Animstate crap
		AnimationLayer layers[13];	// Onions like layers
		float poses[24];			// Player poses
		AnimState* animstate;		// Animstate crap
	};

	void PostPlayerUpdate();
};

extern LagComp* lagcomp;

