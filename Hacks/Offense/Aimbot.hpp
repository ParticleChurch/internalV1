#pragma once
class Aimbot
{
public:
	// Math Shiz
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	void Smooth(Vec& Angle);

	// Hitbox/Hitgroup Management
	int GetHitGroup(int Hitbox);
	std::vector<Hitboxes> priority;
	void GetPriorityHitbox(int gun);
	std::vector<Hitboxes> hboxes;
	void GetHitboxes(int gun);

	// Data stuff
	int smooth_method = 0;
	float smooth_amount = 0;
	float fov = 0;
	int priority_hitbox = 0;
	float mindamage_visible = 0;
	float mindamage_hidden = 0;
	float hitchance = 0;

	// secret stuff
	float accuracy_amount = 0.0f;

	// Scan Stuff
	void VisibleScan(int& BestIndex, int& BestDamage, Vec& BestAimPoint);
	void AutowallScan(int& BestIndex, int& BestDamage, Vec& BestAimPoint);

	// Run stuff
	void Run();

	// LEGIT
	std::vector<Hitboxes> LegitHitboxes;
	void Legit();
	int GetClosestToCrosshair();
	QAngle GetClosestBoneToCrosshairAngle(int Index, float& distance, bool& valid);
	void DoRecoilComponensation(QAngle& ang);

	void SmoothLinear(QAngle& Ang, float x, float y);
	void SmoothSlowToFast(QAngle& Ang, float x, float y);
	void SmoothFastToSlow(QAngle& Ang, float x, float y);

	void rage();
};

extern Aimbot* aimbot;
