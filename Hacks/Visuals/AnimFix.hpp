#pragma once
class AnimFix
{
public:
	bool SendPacket;
	bool GotMatrix;
	Vec ViewAng;
	Vec FakeAng;

	// Fake SHIT
	bool UpdateFake = false;
	unsigned long EntHandle;
	float LocalSpawnTime = -1.f;
	AnimState* FakeAnimState;
	AnimState* FakeAnimStateAllocated;
	Matrix3x4 FakeMatrix[256];
	AnimationLayer FakeLayers[13];
	std::array< float, 24 > FakePoses;

	bool GenerateMatrix(Entity* pEntity, Matrix3x4* pBoneToWorldOut, int boneMask, float currentTime);
	void UpdateReal();
	void UpdateFakeAnim();
	void UpdateVal(CUserCmd* cmd, bool& sendPacket);
};

extern AnimFix* animfix;

