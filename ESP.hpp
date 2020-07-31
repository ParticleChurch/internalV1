#pragma once
class ESP
{
private:
	Color Current = Color(255,255,255,255);

	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char Name[128]);
	void DrawSnapLines(Vec TL, Vec BR);
	void DrawHealth(Vec TL, Vec BR, int Health);
public:
	void Run();

	
};

extern ESP* esp;