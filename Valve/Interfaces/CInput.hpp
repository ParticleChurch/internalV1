#pragma once

class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd;
	unsigned long m_crc;
};

class CInput
{
public:
	char _pad0[0xAC];
	bool m_fCameraInterceptingMouse;
	bool m_fCameraInThirdPerson;
	bool m_fCameraMovingWithMouse;
	Vec m_vecCameraOffset;
	bool m_fCameraDistanceMove;
	int m_nCameraOldX;
	int m_nCameraOldY;
	int m_nCameraX;
	int m_nCameraY;
	bool m_CameraIsOrthographic;
	Vec m_angPreviousViewAngles;
	Vec m_angPreviousViewAnglesTilt;
	float m_flLastForwardMove{};
	int m_nClearInputState{};
	CUserCmd* commands{};
	CVerifiedUserCmd* verified_commands{};
	CUserCmd* GetUserCmd(int sequence_number)
	{
		using OriginalFn = CUserCmd * (__thiscall*)(void*, int, int);
		return GetVFunc<OriginalFn>(this, 8)(this, 0, sequence_number);
	}

	CUserCmd* GetUserCmd(int nSlot, int sequence_number)
	{
		typedef CUserCmd* (__thiscall* GetUserCmd_t)(void*, int, int);
		return GetVFunc<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
	}

	CVerifiedUserCmd* GetVerifiedCmd(int sequence_number)
	{
		auto verifiedCommands = *(CVerifiedUserCmd**)(reinterpret_cast<uint32_t>(this) + 0xF8);
		return &verifiedCommands[sequence_number % 150];
	}

	void CamToFirstPerson()
	{
		typedef void (__thiscall* CamToFirstPerson_)(void*);
		GetVFunc<CamToFirstPerson_>(this, 36)(this);
	}

	int CamInThirdPerson(int slot = -1)
	{
		typedef int (__thiscall* CamInThirdPerson_)(void*, int);
		return GetVFunc<CamInThirdPerson_>(this, 32)(this, slot);
	}
};