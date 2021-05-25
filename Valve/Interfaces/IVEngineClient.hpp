#pragma once
class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(void*, int&, int&);
		return GetVFunc<oGetScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int iIndex, player_info_t* pInfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(void*, int, player_info_t*);
		return GetVFunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
	}

	int GetPlayerForUserID(int UserID)
	{
		typedef int(__thiscall* oGetPlayerForUserID)(void*, int);
		return GetVFunc<oGetPlayerForUserID>(this, 9)(this, UserID);
	}

	bool IsConsoleOpen()
	{
		typedef bool(__thiscall* oIsConsoleOpen)(void*);
		return GetVFunc<oIsConsoleOpen>(this, 11)(this);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* oGetLocalPlayer)(void*);
		return GetVFunc<oGetLocalPlayer>(this, 12)(this);
	}

	void GetViewAngles(Vec& angle)
	{
		typedef void(__thiscall* oGetViewAngles)(void*, Vec&);
		return GetVFunc<oGetViewAngles>(this, 18)(this, angle);
	}

	void SetViewAngles(Vec& angle)
	{
		typedef void(__thiscall* oSetViewAngles)(void*, Vec&);
		return GetVFunc<oSetViewAngles>(this, 19)(this, angle);
	}

	int GetMaxClients()
	{
		typedef int(__thiscall* oGetMaxClients)(void*);
		return GetVFunc<oGetMaxClients>(this, 20)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall* oIsInGame)(void*);
		return GetVFunc<oIsInGame>(this, 26)(this);
	}

	const Matrix3x4& WorldToScreenMatrix()
	{
		typedef const Matrix3x4& (__thiscall* oWorldToScreenMatrix)(PVOID);
		return GetVFunc<oWorldToScreenMatrix>(this, 37)(this);
	}

	const char* GetLevelName()
	{
		typedef const char* (__thiscall* oGetLevelName)(void*);
		return GetVFunc<oGetLevelName>(this, 52)(this);
	}

	const char* GetLevelNameShort()
	{
		typedef const char* (__thiscall* oGetLevelName)(void*);
		return GetVFunc<oGetLevelName>(this, 53)(this);
	}

	bool IsTakingScreenshot()
	{
		typedef bool(__thiscall* oIsTakingScreenshot)(void*);
		return GetVFunc<oIsTakingScreenshot>(this, 92)(this);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		typedef void(__thiscall* oExecuteClientCmd)(void*, const char*);
		return GetVFunc<oExecuteClientCmd>(this, 108)(this, szCmdString);
	}

	NetChannel* GetNetChannelInfo()
	{
		typedef NetChannel* (__thiscall* oGetNetChannelInfo)(void*);
		return GetVFunc<oGetNetChannelInfo>(this, 78)(this);
	}

	void ClientCmd_Unrestricted(const char* szCmdString)
	{
		typedef void(__thiscall* oClientCmd_Unrestricted)(PVOID, const char*, char);
		return GetVFunc<oClientCmd_Unrestricted>(this, 114)(this, szCmdString, 0);
	}

};

