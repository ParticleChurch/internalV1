#pragma once
class Aimbot
{
public:
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	void Smooth(Vec& Angle);
	int GetHitGroup(int Hitbox);

	void Run();
};

extern Aimbot* aimbot;