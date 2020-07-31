#pragma once
#include "Entity.hpp"
namespace G {
	// The injected DLL's module
	extern HMODULE DLLModule;
	extern bool KillDLL;

	//for hacks
	extern Entity* Localplayer;

	extern CUserCmd* cmd;
	extern Vec CM_StartAngle;
	extern Vec CM_EndAngle;
	extern void CM_Start(CUserCmd*);
	extern void CM_End();
};