#pragma once
class Aimbot
{
public:
	Vec CalculateAngle(Vec Target);
	float CrosshairDist(Vec TargetAngle);
	//functions for legit
	void Legit();
	void Smooth(Vec& Angle);
	//functions for rage
	void Rage();
};

extern Aimbot* aimbot;