#pragma once
class ESP
{
private:
	Color Current = Color(255,255,255,255);

	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char Name[128]);
	void DrawSnapLines(Vec TL, Vec BR);
	void DrawHealth(Vec TL, Vec BR, int Health);
	void DrawSkeleton(Entity*);
	void DrawBacktrackingDots();
	void DrawTraces();
	void DrawPoints();
public:
	std::deque<trace_t> traces;
	std::deque<Vec> points;
	void Run();

	
};

extern ESP* esp;