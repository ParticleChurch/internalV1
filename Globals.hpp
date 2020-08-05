#pragma once
#include "Entity.hpp"
namespace G {
	// The injected DLL's module
	extern HMODULE DLLModule;
	extern bool KillDLL;

	//for hacks
	extern Entity* Localplayer;

	//CM stuff
	extern CUserCmd* cmd;
	extern bool* pSendPacket;
	extern Vec CM_StartAngle;
	extern Vec CM_EndAngle;
	extern QAngle RealAngle;
	extern QAngle FakeAngle;
	extern void CM_Clamp();
	extern void CM_Start(CUserCmd*, bool*);
	extern void CM_MoveFixStart();
	extern void CM_End();
};