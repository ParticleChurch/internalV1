#pragma once

class Resolver
{
private:
	// 64 is the number of players
	Vec Loc;
	bool Hit[64];
	
public:
	int ShotsMissed[64];


	int TargetIndex = 0;
	void LogShots(GameEvent* event);
	void Resolve(int stage);
	void ShowInfo();
	
};

extern Resolver* resolver;

// weaponfire
// othershit
// weaponfire
// playerhurt (if hurting!)