#pragma once
class Aimbot
{
public:
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	//functions for legit
	void Legit();
	void Smooth(Vec& Angle, int ConfigIndex);
	//functions for rage
	void Rage();
};

extern Aimbot* aimbot;