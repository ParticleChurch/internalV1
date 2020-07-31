#pragma once
class ESP
{
private:
	Color Current = Color(255,255,255,255);

	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char name[128]);
	void DrawSnapLines(Vec TL, Vec BR);
public:
	void Run();

	
};

extern ESP* esp;