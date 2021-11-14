#pragma once
class IVEngineClient
{
public:
	DECLARE_VIRTUAL_METHOD(void, GetScreenSize, 5, (int& width, int& height), (width, height));
	DECLARE_VIRTUAL_METHOD(bool, GetPlayerInfo, 8, (int iIndex, player_info_t* pInfo), (iIndex, pInfo));
	DECLARE_VIRTUAL_METHOD(bool, GetPlayerForUserID, 9, (int UserID), (UserID));
	DECLARE_VIRTUAL_METHOD(bool, IsConsoleOpen, 11, (), ());
	DECLARE_VIRTUAL_METHOD(int, GetLocalPlayer, 12, (), ());
	DECLARE_VIRTUAL_METHOD(void, GetViewAngles, 18, (Vec& angle), (angle));
	DECLARE_VIRTUAL_METHOD(void, SetViewAngles, 19, (Vec& angle), (angle));
	DECLARE_VIRTUAL_METHOD(int, GetMaxClients, 20, (), ());
	DECLARE_VIRTUAL_METHOD(bool, IsInGame, 26, (), ());
	DECLARE_VIRTUAL_METHOD(const Matrix3x4&, WorldToScreenMatrix, 37, (), ());
	DECLARE_VIRTUAL_METHOD(const char*, GetLevelName, 52, (), ());
	DECLARE_VIRTUAL_METHOD(const char*, GetLevelNameShort, 53, (), ());
	DECLARE_VIRTUAL_METHOD(NetChannel*, GetNetChannelInfo, 78, (), ());
	DECLARE_VIRTUAL_METHOD(bool, IsTakingScreenshot, 92, (), ());
	DECLARE_VIRTUAL_METHOD(void, ExecuteClientCmd, 108, (const char* szCmdString), (szCmdString));
	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, ClientCmd_Unrestricted, 114, (const char* szCmdString), (const char*, char), (szCmdString, 0));
};

