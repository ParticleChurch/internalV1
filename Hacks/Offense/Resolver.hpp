#pragma once

class Resolver
{
private:
	std::deque<Vec> dequeBulletImpacts;
	bool bPlayerHurt[64]; //when player_hurt called
	bool bBulletImpact[64]; //when bullet_impact called
	int bShotsMissed[64];
public:
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