#include "Include.hpp"

namespace G
{
	//for closing console
	HMODULE DLLModule = NULL;
	bool KillDLL = false;
	std::string CSGODirectory = "";
	std::string CSGOExePath = "";

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

		// parse G::CSGOExePath and G::CSGODirectory
		char exePath[MAX_PATH];
		GetModuleFileNameA(NULL, exePath, MAX_PATH);
		CSGOExePath = std::string(exePath);
		size_t slash = CSGOExePath.rfind("\\");
		if (slash >= CSGOExePath.length())
			CSGODirectory = "C:\\";
		else
			CSGODirectory = G::CSGOExePath.substr(0, slash);
		UserData::CredentialsFile = CSGODirectory + "\\credentials.pclogin";
	}

	void Init()
	{
		UpdateRate = I::cvar->FindVar("cl_updaterate");
		GUI::LoadProgress += 0.02f;
		MaxUpdateRate = I::cvar->FindVar("sv_maxupdaterate");
		GUI::LoadProgress += 0.02f;
		Interp = I::cvar->FindVar("cl_interp");
		GUI::LoadProgress += 0.02f;
		InterpRatio = I::cvar->FindVar("cl_interp_ratio");
		GUI::LoadProgress += 0.02f;
		MinInterpRatio = I::cvar->FindVar("sv_client_min_interp_ratio");
		GUI::LoadProgress += 0.02f;
		MaxInterpRatio = I::cvar->FindVar("sv_client_max_interp_ratio");
		GUI::LoadProgress += 0.02f;
		MaxUnlag = I::cvar->FindVar("sv_maxunlag");
		GUI::LoadProgress += 0.02f;

		while (!(AcceptMatchPattern = FindPattern("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")));
		GUI::LoadProgress += 0.3f;
		while (!(LoadSkyboxPattern  = FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
		GUI::LoadProgress += 0.3f;
	}
}