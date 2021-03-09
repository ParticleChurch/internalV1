#pragma once
#include "Entity.hpp"

struct Player
{
	// Player Crap
	Entity* entity;			// Entity ptr
	Entity* weap;			// Active weapon
	int index = -1;			// Index
	int health = -1;		// Health
	int team = -1;			// Team
	bool dormant = false;	// Is dormant?
	int Flags = 0;
	bool Valid = false;		// Is this player valid/active?
	Matrix3x4 Matrix[128];	// Current Matrix
	Model* model;			// Current Model
	Vec EyePos;				// Current EyePos
	player_info_t info;		// UserInfo

	// Resolver Shit
	float CurSimTime = -0.f;		// Current Sim Time
	float LastValidSimTime = 0.f;	// Last Valid Sim Time
	float LastShotTime = 0.f;		// Get Last Shot Time
	float CurLBY = 0.f;				// Current Lower Body Yaw (LBY)
	float OldLBY = 0.f;				// Old LBY (update on LBY Change)
	float CurYawOffset;				// Current Yaw Offset
	int ShotsMissed;				// Shots missed
	bool InAir = false;				// Are they in the air

	// Backtrack Shit
	struct Tick
	{
		float SimulationTime;
		Matrix3x4 Matrix[256];

		Tick() {};
		Tick(float SimulationTime, Matrix3x4 Matrix[128]) {
			this->SimulationTime = SimulationTime;
			for (int i = 0; i < 128; i++) {
				this->Matrix[i] = Matrix[i];
			}
		}
		Vec Bone(int i) {
			return Vec(Matrix[i][0][3],
				Matrix[i][1][3],
				Matrix[i][2][3]);
		}
		void operator = (Tick const& Obj) {
			SimulationTime = Obj.SimulationTime;
			for (int i = 0; i < 128; i++) {
				Matrix[i] = Obj.Matrix[i];
			}
		}
		bool operator == (Tick const& Obj)
		{
			return (SimulationTime == Obj.SimulationTime);
		}
		bool operator != (Tick const& Obj)
		{
			return (SimulationTime != Obj.SimulationTime);
		}
	};
	std::deque<Tick> BacktrackRecords;
};

namespace G {
	// The injected DLL's module
	extern HMODULE DLLModule;
	extern bool KillDLL;

	//General Variables
	extern QAngle StartAngle;
	extern QAngle EndAngle;
	extern bool IsInGame;

	//LocalPlayer
	extern Entity* LocalPlayer;
	extern bool LocalPlayerAlive;
	extern int LocalPlayerIndex;
	extern int LocalPlayerTeam;
	extern Entity* LocalPlayerWeapon;
	extern WeaponData* LocalPlayerWeaponData;
	//maybe add flags and movetype?

	//Other Entities
	extern std::map<int, Player> EntList;
	extern void UpdateEntities();

	//CreateMove
	extern CUserCmd* cmd;
	extern bool* pSendPacket;
	extern float StartForwardMove;
	extern float StartSideMove;

	extern bool DoubleTap;

	//Global Functions
	extern void CM_Clamp();
	extern void CM_Start(CUserCmd*, bool*);
	extern void CM_MoveFixStart();
	extern void CM_MoveFixEnd();
	extern void CM_End();

	// Pattern Scans / Convars
	extern ConVar* UpdateRate;
	extern ConVar* MaxUpdateRate;
	extern ConVar* Interp;
	extern ConVar* InterpRatio;
	extern ConVar* MinInterpRatio;
	extern ConVar* MaxInterpRatio;
	extern ConVar* MaxUnlag;
	extern DWORD AcceptMatchPattern;
	extern DWORD pD3d9DevicePattern;
	extern DWORD LoadSkyboxPattern;

	extern void GUIInit();
	extern void Init();
};