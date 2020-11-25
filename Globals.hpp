#pragma once
#include "Entity.hpp"
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
	//maybe add flags and movetype?

	//CreateMove
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