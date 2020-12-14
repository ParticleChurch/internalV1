#pragma once
class World
{
private:
	void WorldMod(Material* mat);
	void PropMod(Material* mat);
	void SkyboxLoad(std::string);
	void SkyboxMod(Material* mat);
public:
	void Run(int CurStage);
};

extern World* world;