#pragma once
class Aimbot
{
public:
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	//functions for legit
	void Smooth(Vec& Angle);
	//functions for rage
	void Run();
};

extern Aimbot* aimbot;