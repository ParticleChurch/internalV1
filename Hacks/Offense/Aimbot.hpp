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
		bool BaimIfLethal;
	}rage;

	// General Private Functions
	float CalculateHitchance(QAngle vangles, const Vec& point, Entity* player, Hitboxes hbox);
	float CalculatePsudoHitchance();
	bool ValidPlayer(Player player);
	

	// Legit Functions
	void Legit();
	bool UpdateLegitVal();
	void GetLegitHitboxes(int gun);
	void GetClosestEntity(int& RecordUserID, float& Distance);
	QAngle GetClosestAng(int RecordUserID, bool& valid);
	void Smooth(QAngle& Ang);
	void SmoothLinear(QAngle& Ang);
	void SmoothSlowToFast(QAngle& Ang);
	void SmoothFastToSlow(QAngle& Ang);

	// Rage Functions/var
	void Rage();
	Vec AimPoint;
	bool backtrackaim = false;
	Matrix3x4 TargetMatrix[265];
	int TargetUserID = -1;
	int TargetTickCount = -1;
	bool UpdateRageVal();
	void GetRageHitboxes(int gun);

	// Sorts UserID's of players in an order that makes sense for autowalling
	std::vector<std::pair<int, float>> players;
	void SortPlayers(std::vector<std::pair<int, float>>& values);
	
	// Scans through players and attempts to autowall them (uses multithreading)
	bool ScanPlayers();
	// Scans a given player and attempts to autowall them 
	bool ScanPlayer(int UserID, Vec& Point);
	
	// Scans a given player backtrack and attempts to autowall them
	bool ScanPlayerBacktrack(int UserID, Vec& Point);

public:
	int GetHitGroup(int Hitbox);


	int maxplayerscan = 5;
	// General Public Functions
	Vec CalculateAngle(Vec Target);
	Vec CalculateAngle(Vec Source, Vec Target);
	float CrosshairDist(Vec TargetAngle);
	
	// CHAET RUN MAEN
	void Run();
};

extern Aimbot* aimbot;
