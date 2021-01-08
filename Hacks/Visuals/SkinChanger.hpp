#pragma once
class SkinChanger
{
private:
	void ForceSkin(Entity* weap, int paintkit);
public:
	void run(int stage);
};

extern SkinChanger* skinchanger;

