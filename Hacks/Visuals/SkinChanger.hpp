#pragma once
struct SkinChanger
{
	void ForceSkin(Entity* weap, int paintkit);
	void run(int stage);
};

extern SkinChanger* skinchanger;

