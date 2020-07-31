#pragma once
class Aimbot
{
public:
	Vec CalculateAngle(Vec Target);
	float CrosshairDist(Vec TargetAngle);
};

extern Aimbot* aimbot;