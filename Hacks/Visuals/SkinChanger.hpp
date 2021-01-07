#pragma once
class SkinChanger
{
private:
	void ForceSkin(Entity* weap, int skinid);
public:
	void run(int stage);
};

extern SkinChanger* skinchanger;

