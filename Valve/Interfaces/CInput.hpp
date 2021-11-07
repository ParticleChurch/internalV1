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

	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(CUserCmd*, GetUserCmd, 8, (int sequence_number), (int, int), (0, sequence_number));
	DECLARE_VIRTUAL_METHOD(CUserCmd*, GetUserCmd, 8, (int nSlot, int sequence_number), (nSlot, sequence_number));
	DECLARE_VIRTUAL_METHOD(void, CamToFirstPerson, 36, (), ());
	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(int, CamInThirdPerson, 32, (int slot = -1), (int), (slot));

	CVerifiedUserCmd* GetVerifiedCmd(int sequence_number)
	{
		auto verifiedCommands = *(CVerifiedUserCmd**)(reinterpret_cast<uint32_t>(this) + 0xF8);
		return &verifiedCommands[sequence_number % 150];
	}
};