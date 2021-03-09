#include "Include.hpp"

namespace G
{
	//for closing console
	HMODULE DLLModule = NULL;
	bool KillDLL = false;

	//General Variables
	QAngle StartAngle;
	QAngle EndAngle;
	bool IsInGame;

	//LocalPlayer
	Entity* LocalPlayer = new Entity();
	bool LocalPlayerAlive = false;
	int LocalPlayerIndex = 0;
	int LocalPlayerTeam = -1;
	Entity* LocalPlayerWeapon;
	WeaponData* LocalPlayerWeaponData;

	// Other Entitys
	std::map<int, Player> EntList;
	void UpdateEntities()
	{
		/*
		std::map<int, Player>::iterator it;
		for (it = G::EntList.begin(); it != G::EntList.end(); it++)
		{
			//it->first is key	  (User Id)
			//it->second is value (Player struct)
		}
		*/

		std::map<int, Player>::iterator it;
		for (it = G::EntList.begin(); it != G::EntList.end(); it++)
		{
			it->second.Valid = false;
		}
		

		player_info_t TempInfo;
		for (int i = 1; i < 65; i++)
		{
			// Localplayer Check
			if (i == G::LocalPlayerIndex) continue;

			// Entity Existance Check
			Entity* ent = I::entitylist->GetClientEntity(i);
			if (!ent) continue;

			// If Not Player
			if (!I::engine->GetPlayerInfo(i, &TempInfo)) continue;

			int UserId = TempInfo.userid;
				
			// If not in the list... ADD IT BOY
			if (EntList.find(UserId) == EntList.end())
			{
				Player NewPlayer;
				EntList.insert(std::pair<int, Player>(UserId, NewPlayer));
			}
			
			// Update Player Crap
			EntList[UserId].entity		= ent;
			EntList[UserId].weap		= ent->GetActiveWeapon();
			EntList[UserId].index		= i;
			EntList[UserId].health		= ent->GetHealth();
			EntList[UserId].team		= ent->GetTeam();
			EntList[UserId].dormant		= ent->IsDormant();
			EntList[UserId].Flags		= ent->GetFlags();
			EntList[UserId].Valid		= true;
			EntList[UserId].CurSimTime	= ent->GetSimulationTime();
			EntList[UserId].EyePos		= ent->GetEyePos();
			EntList[UserId].info		= TempInfo;
			

			// If it is a Valid SimTime
			if (ValidSimTime(EntList[UserId].CurSimTime))
			{
				ent->SetupBones(EntList[UserId].Matrix, 128, 0x100);
				EntList[UserId].LastValidSimTime = EntList[UserId].CurSimTime;	
			}

			Model* model = EntList[UserId].entity->GetModel();
			if (model)
			{
				if (!EntList[UserId].model)
				{
					EntList[UserId].model = new Model();
					EntList[UserId].model = model;
				}
				else
				{
					EntList[UserId].model = model;
				}
			}
			else
			{
				free(EntList[UserId].model);
			}
			

			// Update Resolver Shit
			// This part is experimental
			if (EntList[UserId].weap)
				EntList[UserId].LastShotTime = EntList[UserId].weap->GetLastShotTime();

			// If LBY off by more then 1 degree...
			float lby = ent->GetLBY();
			if (fabsf(EntList[UserId].CurLBY - lby) > 1.f)
			{
				EntList[UserId].OldLBY = EntList[UserId].CurLBY;
				EntList[UserId].CurLBY = lby;
			}
			EntList[UserId].InAir = !(EntList[UserId].Flags & FL_ONGROUND);
		}
	}
	
	//CreateMove
	CUserCmd* cmd;
	bool* pSendPacket;
	float StartForwardMove;
	float StartSideMove;

	bool DoubleTap = true;

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

	void GUIInit()
	{
		while (!(pD3d9DevicePattern = FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C")));
	}

	void Init()
	{
		UpdateRate = I::cvar->FindVar("cl_updaterate");
		GUI2::LoadProgress += 0.02f;
		MaxUpdateRate = I::cvar->FindVar("sv_maxupdaterate");
		GUI2::LoadProgress += 0.02f;
		Interp = I::cvar->FindVar("cl_interp");
		GUI2::LoadProgress += 0.02f;
		InterpRatio = I::cvar->FindVar("cl_interp_ratio");
		GUI2::LoadProgress += 0.02f;
		MinInterpRatio = I::cvar->FindVar("sv_client_min_interp_ratio");
		GUI2::LoadProgress += 0.02f;
		MaxInterpRatio = I::cvar->FindVar("sv_client_max_interp_ratio");
		GUI2::LoadProgress += 0.02f;
		MaxUnlag = I::cvar->FindVar("sv_maxunlag");
		GUI2::LoadProgress += 0.02f;

		while (!(AcceptMatchPattern = FindPattern("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")));
		GUI2::LoadProgress += 0.3f;
		while (!(LoadSkyboxPattern  = FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
		GUI2::LoadProgress += 0.3f;
	}
}