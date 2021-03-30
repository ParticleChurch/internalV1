#pragma once
class Aimbot
{
private:
	// Data stuff
	struct legit
	{
		float fov;
		std::vector<Hitboxes> hitboxes;
		int smoothing;
		float smoothing_amount;
		float hidden_max = 10.f;
	}legit;

	struct rage
	{
		std::deque<Hitboxes> hitboxes;
		int hid_mindam; // hidden min damage
		int vis_mindam; // visible min damage
		float hitchance;
		bool FireIfLethal;
	}rage;

	// General Private Functions
	float CalculateHitchance(QAngle vangles, const Vec& point, Entity* player, Hitboxes hbox);
	bool ValidPlayer(Player player);
	int GetHitGroup(int Hitbox);
	void GetClosestEntity(int& RecordUserID, float& Distance);

	// Legit Functions
	void Legit();
	bool UpdateLegitVal();
	void GetLegitHitboxes(int gun);
	QAngle GetClosestAng(int RecordUserID, bool& valid);
	void Smooth(QAngle& Ang);
	void SmoothLinear(QAngle& Ang);
	void SmoothSlowToFast(QAngle& Ang);
	void SmoothFastToSlow(QAngle& Ang);

	// Rage Functions/var
	std::vector<int> PlayersScanned = { };
	void Rage();
	bool UpdateRageVal();
	bool TryOnShot();
	void GetRageHitboxes(int gun);
	void GetClosestEntityNotScanned(int& RecordUserID, float& Distance);
	bool ScanPlayer(int RecordUserID, Vec& Point);
	bool ScanPlayerMin(int RecordUserID, Vec& Point);

public:
	// General Public Functions
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	
	// CHAET RUN MAEN
	void Run();
};

extern Aimbot* aimbot;
