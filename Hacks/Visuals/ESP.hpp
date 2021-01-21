#pragma once
class ESP
{
private:
	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char Name[128], Color clr);
	void DrawSnapLines(Vec TL, Vec BR);
	void DrawHealth(Vec TL, Vec BR, int Health, Color fg, Color bg);
	void DrawSkeleton(Entity*);
	void DrawBacktrackingDots();
	
public:
	std::vector<Vec> points;
	void Run_PaintTraverse();
	void Run_GameEvent(GameEvent* event);

	
};

extern ESP* esp;