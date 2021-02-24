#pragma once
class ESP
{
private:
	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char Name[128], Color clr);
	void DrawWeapon(Vec TL, Vec BR, Entity*);
	void DrawSnapLines(Vec TL, Vec BR);
	void DrawHealth(Vec TL, Vec BR, int Health, Color fg, Color bg);
	void DrawSkeleton(Entity*);
	void DrawBacktrackingDots();
	void GetBounds(Entity* ent, Vec& TL, Vec& BR);

	struct BulletTracer
	{
		Vec src;
		Vec end;
		float SimTime;
	};
	std::vector<BulletTracer> traces;
	
public:
	std::vector<Vec> points;
	void Run_PaintTraverse();
	void Run_FrameStageNotify(int stage);
	void Run_GameEvent(GameEvent* event);

	
};

extern ESP* esp;