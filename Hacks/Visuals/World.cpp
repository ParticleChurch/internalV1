#include "../../Include.hpp"

World* world = new World();

void World::WorldMod(Material* mat)
{
	if(!Config::GetBool("visuals-world-enable"))
		return;

	static Color LastColor;
	if (LastColor != Config::GetColor("visuals-world-color"))
	{
		LastColor = Config::GetColor("visuals-world-color");
		mat->ColorModulate(LastColor);
	}
	static float lastAlpha;
	if (lastAlpha != Config::GetFloat("visuals-world-opacity") / 100.f)
	{
		lastAlpha = Config::GetFloat("visuals-world-opacity") / 100.f;
		mat->AlphaModulate(lastAlpha);
	}
}

void World::PropMod(Material* mat)
{
	if (!Config::GetBool("visuals-world-prop-enable"))
		return;

	static Color LastColor;
	if (LastColor != Config::GetColor("visuals-world-prop-color"))
	{
		LastColor = Config::GetColor("visuals-world-prop-color");
		mat->ColorModulate(LastColor);
	}
	static float lastAlpha;
	if (lastAlpha != Config::GetFloat("visuals-world-prop-opacity") / 100.f)
	{
		lastAlpha = Config::GetFloat("visuals-world-prop-opacity") / 100.f;
		mat->AlphaModulate(lastAlpha);
	}
	
}

void World::SkyboxLoad()
{
	static auto load_named_sky = reinterpret_cast<void(__fastcall*)(const char*)>(FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	static auto sv_skyname = I::cvar->FindVar("sv_skyname");
	sv_skyname->onChangeCallbacks.size = 0;

	static auto r_3dsky = I::cvar->FindVar("r_3dsky");
	r_3dsky->onChangeCallbacks.size = 0;
	r_3dsky->SetValue(0);

	load_named_sky("sky_csgo_night02");
}

void World::SkyboxMod(Material* mat)
{
	if (!Config::GetBool("visuals-world-skybox-enable"))
		return;

	static Color LastColor;
	if (LastColor != Config::GetColor("visuals-world-skybox-color"))
	{
		LastColor = Config::GetColor("visuals-world-skybox-color");
		mat->ColorModulate(LastColor);
	}
}

void World::Run(int CurStage)
{
	if (CurStage == FRAME_RENDER_START && G::LocalPlayer && G::LocalPlayerAlive && I::engine->IsInGame()) {
		//SkyboxLoad();
		for (auto i = I::materialsystem->FirstMaterial(); i != I::materialsystem->InvalidMaterial(); i = I::materialsystem->NextMaterial(i))
		{
			auto mat = I::materialsystem->GetMaterial(i);
			if (!mat)
				continue;

			auto tex_name = mat->GetTextureGroupName();
			
			if(strstr(tex_name, "World"))
				WorldMod(mat);
			if (strstr(tex_name, "StaticProp"))
				PropMod(mat);
			if (strstr(tex_name, "SkyBox"))
				SkyboxMod(mat);
		}
	}
}
