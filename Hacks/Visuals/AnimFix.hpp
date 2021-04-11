#pragma once
class AnimFix
{
public:
	bool SendPacket;
	bool GotMatrix;
	Vec ViewAng;
	Vec FakeAng;
	AnimState* FakeAnimState;
	Matrix3x4 FakeMatrix[256];

	void UpdateReal();
	void UpdateVal(CUserCmd* cmd, bool& sendPacket);
};

extern AnimFix* animfix;

