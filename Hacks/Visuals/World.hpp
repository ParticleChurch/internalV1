#pragma once
class World
{
private:
	void WorldMod(Material* mat);
	void PropMod(Material* mat);
	void SkyboxLoad(std::string);
	void SkyboxMod(Material* mat);
	void LightMod();
public:
	void RunFSN();
	void Run_DoPostScreenEffect();
};

extern World* world;