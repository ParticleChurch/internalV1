#include "Include.hpp"

namespace G
{
	//for closing console
	HMODULE DLLModule = NULL;
	bool KillDLL = false;

	//Vars
	Entity* Localplayer = new Entity();

	//Createmove
	CUserCmd* cmd;
	Vec CM_StartAngle;
	Vec CM_EndAngle;

	void CM_Start(CUserCmd* cmd)
	{
		G::Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
		G::cmd = cmd;
		CM_StartAngle = cmd->viewangles;
	}
	void CM_End()
	{
		cmd->viewangles.Normalize(); //prevent csgo from hating us
		CM_EndAngle = cmd->viewangles;
	}
}