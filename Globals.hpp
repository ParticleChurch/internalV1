#pragma once
#include "Entity.hpp"

struct EntItem
{
	// Pointer to entity
	Entity* entity;
	// Index
	int index = -1;
	// if player
	bool player = false;
	// User ID
	int userid = -1;
	// How much Health
	int health = -1;
	// What team
	int team = -1;
	// If dormant (for esp)
	bool dormant = false;
	// Last sim time (to detect desync)
	float lastSimTime = -1;
	// Bone Matrix
	Matrix3x4 Matrix[256];
	// Model
	Model* model;// = new Model();
	// Shooting Position
	Vec EyePos;
};

namespace G {
	// The injected DLL's module
	extern HMODULE DLLModule;
	extern bool KillDLL;

	//General Variables
	extern int ChokeAmount;
	extern QAngle StartAngle;
	extern QAngle EndAngle;

	//LocalPlayer
	extern Entity* LocalPlayer;
	extern bool LocalPlayerAlive;
	extern int LocalPlayerIndex;
	extern int LocalPlayerTeam;
	extern Entity* LocalPlayerWeapon;
	extern WeaponData* LocalPlayerWeaponData;
	//maybe add flags and movetype?

	//Other Entities
	extern EntItem EntList[64];
	extern void UpdateEntities();

	//CreateMov
	extern CUserCmd* cmd;
	extern bool* pSendPacket;
	extern float StartForwardMove;
	extern float StartSideMove;

	//Global Functions
	extern void CM_Clamp();
	extern void CM_Start(CUserCmd*, bool*);
	extern void CM_MoveFixStart();
	extern void CM_MoveFixEnd();
	extern void CM_End();
};