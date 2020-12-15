#include "Include.hpp"

namespace G
{
	//for closing console
	HMODULE DLLModule = NULL;
	bool KillDLL = false;

	//General Variables
	int ChokeAmount = 3;
	QAngle StartAngle;
	QAngle EndAngle;

	//LocalPlayer
	Entity* LocalPlayer = new Entity();
	bool LocalPlayerAlive = false;
	int LocalPlayerIndex = 0;
	int LocalPlayerTeam = -1;
	Entity* LocalPlayerWeapon;
	WeaponData* LocalPlayerWeaponData;

	// Other Entitys
	EntItem EntList[64];
	void UpdateEntities()
	{

		for (int i = 1; i < 65; i++)
		{
			if (i == G::LocalPlayerIndex)
				continue;

			EntList[i].index = i;
			Entity* ent = I::entitylist->GetClientEntity(i);
			if (!ent)
			{
				// WE DONT NEED TO FREE NOW, JUST AT THE VERY END after unhooking and maybe not even then...
				if (ent && EntList[i].entity)
				{
					free(EntList[i].entity);
					free(EntList[i].model);
				}
				continue;
			}
			else {
				if (!EntList[i].entity)
					EntList[i].entity = new Entity();
				EntList[i].entity = ent;
			}

			static player_info_t info;
			EntList[i].player = I::engine->GetPlayerInfo(i, &info);
			if (EntList[i].player)	// If Player
				EntList[i].userid = info.userid;
			else
				continue;
			EntList[i].health = EntList[i].entity->GetHealth();
			EntList[i].team = EntList[i].entity->GetTeam();
			EntList[i].dormant = EntList[i].entity->IsDormant();
			EntList[i].lastSimTime = EntList[i].entity->GetSimulationTime();
			if (backtrack->Valid(EntList[i].lastSimTime))
				EntList[i].entity->SetupBones(EntList[i].Matrix, 128, 0x100);
			Model* model = EntList[i].entity->GetModel();
			if (model)
			{
				if (!EntList[i].model)
				{
					EntList[i].model = new Model();
					EntList[i].model = model;
				}
				else
				{
					EntList[i].model = model;
				}
			}
			else
			{
				free(EntList[i].model);
			}
			EntList[i].EyePos = EntList[i].entity->GetEyePos();
		}
	}
	
	//CreateMove
	CUserCmd* cmd;
	bool* pSendPacket;
	float StartForwardMove;
	float StartSideMove;

	//Global Functions
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

	// Pattern Scans / Convars
	ConVar* UpdateRate;
	ConVar* MaxUpdateRate;
	ConVar* Interp;
	ConVar* InterpRatio;
	ConVar* MinInterpRatio;
	ConVar* MaxInterpRatio;
	ConVar* MaxUnlag;
	DWORD AcceptMatchPattern;
	DWORD pD3d9DevicePattern;
	DWORD LoadSkyboxPattern;
	DWORD TraceToExitPattern;

	void PatternConvarInit()
	{
		UpdateRate = I::cvar->FindVar("cl_updaterate");
		MaxUpdateRate = I::cvar->FindVar("sv_maxupdaterate");
		Interp = I::cvar->FindVar("cl_interp");
		InterpRatio = I::cvar->FindVar("cl_interp_ratio");
		MinInterpRatio = I::cvar->FindVar("sv_client_min_interp_ratio");
		MaxInterpRatio = I::cvar->FindVar("sv_client_max_interp_ratio");
		MaxUnlag = I::cvar->FindVar("sv_maxunlag");

		AcceptMatchPattern = FindPattern("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12");
		while(AcceptMatchPattern == 0)
			AcceptMatchPattern = FindPattern("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12");
		pD3d9DevicePattern = FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C");
		while (pD3d9DevicePattern == 0)
			pD3d9DevicePattern = FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C");
		LoadSkyboxPattern = FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
		while (LoadSkyboxPattern == 0)
			LoadSkyboxPattern = FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
	}
}