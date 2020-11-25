#include "Include.hpp"

namespace G
{
	//for closing console
	HMODULE DLLModule = NULL;
	bool KillDLL = false;

	//General Variables
	int ChokeAmount = 2;
	QAngle StartAngle;
	QAngle EndAngle;

	//LocalPlayer
	Entity* LocalPlayer = new Entity();
	bool LocalPlayerAlive = false;
	int LocalPlayerIndex = 0;\

	//CreateMove
	CUserCmd* cmd;
	bool* pSendPacket;
	float StartForwardMove;
	float StartSideMove;

	void CM_Clamp()
	{
		//Angle stuff
		if ((cmd->viewangles).x > 89) {
			(cmd->viewangles).x = 89;
		}
		else if ((cmd->viewangles).x < -89) {
			(cmd->viewangles).x = -89;
		}

		while ((cmd->viewangles).y > 180) {
			(cmd->viewangles).y -= 360;
		}
		while ((cmd->viewangles).y < -180) {
			(cmd->viewangles).y += 360;
		}
		(cmd->viewangles).z = 0;

		//side/forwardMove/upmove
		if (cmd->sidemove > 450.f) {
			cmd->sidemove = 450.f;
		}
		if (cmd->sidemove < -450.f) {
			cmd->sidemove = -450.f;
		}
		if (cmd->forwardmove > 450.f) {
			cmd->forwardmove = 450.f;
		}
		if (cmd->forwardmove < -450.f) {
			cmd->forwardmove = -450.f;
		}
		if (cmd->upmove > 320.f) {
			cmd->upmove = 320.f;
		}
		if (cmd->upmove < -320.f) {
			cmd->upmove = -320.f;
		}
	}

	void CM_Start(CUserCmd* cmd, bool* pSendPacket)
	{
		G::StartAngle = cmd->viewangles;
		G::cmd = cmd;
		G::pSendPacket = pSendPacket;
	}
	void CM_MoveFixStart()
	{
		
		G::StartForwardMove = G::cmd->forwardmove;
		G::StartSideMove = G::cmd->sidemove;
	}
	void CM_MoveFixEnd()
	{
		//fix movement
		static float deltaView;
		static float f1;
		static float f2;

		if (StartAngle.y < 0.f)
			f1 = 360.0f + StartAngle.y;
		else
			f1 = StartAngle.y;

		if (cmd->viewangles.y < 0.0f)
			f2 = 360.0f + cmd->viewangles.y;
		else
			f2 = cmd->viewangles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);

		deltaView = 360.0f - deltaView;

		cmd->forwardmove = cos(DEG2RAD(deltaView)) * StartForwardMove + cos(DEG2RAD(deltaView + 90.f)) * StartSideMove;
		cmd->sidemove = sin(DEG2RAD(deltaView)) * StartForwardMove + sin(DEG2RAD(deltaView + 90.f)) * StartSideMove;
	}
	void CM_End()
	{
		cmd->viewangles.NormalizeAngle(); //prevent csgo from hating us
		EndAngle = cmd->viewangles;

		/*if (*G::pSendPacket) {
			G::FakeAngle = G::cmd->viewangles;
			G::Localplayer->SetupBones(G::FakeMatrix, 128, 0x100, I::globalvars->m_curTime);
		}
		else
			G::RealAngle = G::cmd->viewangles;*/

		CM_Clamp();
	}
}