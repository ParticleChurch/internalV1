#pragma once
class ESP
{
private:
	Color Current = Color(255,255,255,255);

	void DrawBoxes(Vec Top, Vec Bottom);
	void DrawName(Vec Top, char name[128]);
	void DrawSnapLines(Vec Bottom);
public:
	void Run();

	
};

extern ESP* esp;