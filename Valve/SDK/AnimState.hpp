#pragma once
struct AnimState
{
	char pad_0x0000[0x18]; //0x0000
	float m_flAnimUpdateTimer; //0x0018 
	char pad_0x001C[0xC]; //0x001C
	float m_flStartedMovingTime; //0x0028 
	float m_flLastMoveTime; //0x002C 
	char pad_0x0030[0x10]; //0x0030
	float m_flLastLBYTime; //0x0040 
	char pad_0x0044[0x8]; //0x0044
	float m_flRunAmount; //0x004C 
	char pad_0x0050[0x10]; //0x0050
	Entity* pEntity; //0x0060 
	Entity* pActiveWeapon; //0x0064 
	Entity* pLastActiveWeapon; //0x0068 
	float m_flLastClientSideAnimationUpdateTime; //0x006C 
	int m_iLastClientSideAnimationUpdateFramecount; //0x0070 
	float m_flEyeTimer; //0x0074 
	float m_flEyeYaw; //0x0078 
	float m_flEyePitch; //0x007C 
	float m_flGoalFeetYaw; //0x0080 
	float m_flCurrentFeetYaw; //0x0084 
	float m_flCurrentTorsoYaw; //0x0088 
	float m_flVelocityLean; //0x008C 
	float m_flLeanAmount; //0x0090 
	char pad_0x0094[0x4]; //0x0094
	float m_flFeetCycle; //0x0098 
	float m_flFeetYawRate; //0x009C 
	char pad_0x00A0[0x4]; //0x00A0
	float m_flDuckAmount; //0x00A4 
	float m_flLandingDuckAmount; //0x00A8 
	char pad_0x00AC[0x4]; //0x00AC
	Vec m_vOrigin; //0x00B0 
	Vec m_vLastOrigin; //0x00BC
	float m_flVelocityX; //0x00C8 
	float m_flVelocityY; //0x00CC 
	char pad_0x00D0[0x10]; //0x00D0
	float m_flMoveDirection1; //0x00E0 
	float m_flMoveDirection2; //0x00E4 
	char pad_0x00E8[0x4]; //0x00E8
	float m_flVelocity2D; //0x00EC 
	float m_flVerticalVelocity; //0x00F0 
	float m_flVelocityNormalized; //0x00F4 
	float m_flFeetSpeedForwardsOrSideWays; //0x00F8 
	float m_flFeetSpeedUnknownForwardOrSideways; //0x00FC 
	float m_flTimeSinceStartedMoving; //0x0100 
	float m_flTimeSinceStoppedMoving; //0x0104 
	bool m_bOnGround; //0x0108 
	bool m_bInHitGroundAnimation; //0x0109 
	char pad_0x010A[0xA]; //0x010A
	float m_flLastOriginZ; //0x0114 
	float m_flStandingFraction; //0x0118 
	float m_flRunningFraction; //0x011C 
	char pad_0x0120[0x4]; //0x0120
	float m_flUnknownFraction; //0x124
	char pad_0x0128[0x218]; //0x0128
	// sizeof: 0x340 = 832
};

struct AnimState2 {
	std::byte pad[164];
	float duckAmount;
	std::byte pad1[80];
	float footSpeed;
	float footSpeed2;
	std::byte pad2[22];
	float stopToFullRunningFraction;
	std::byte pad3[532];
	float velocitySubtractY;
};