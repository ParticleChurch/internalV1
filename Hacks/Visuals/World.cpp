#include "../../Include.hpp"

World* world = new World();

void World::WorldMod(Material* mat)
{
	mat->ColorModulate(Config::GetColor("visuals-world-color"));
	mat->AlphaModulate(Config::GetFloat("visuals-world-opacity") / 100.f);
}

void World::PropMod(Material* mat)
{
	mat->ColorModulate(Config::GetColor("visuals-world-prop-color"));
	mat->AlphaModulate(Config::GetFloat("visuals-world-prop-opacity") / 100.f);
	
}

void World::SkyboxLoad(std::string name)
{
	static auto load_named_sky = reinterpret_cast<void(__fastcall*)(const char*)>(G::LoadSkyboxPattern);
	static auto sv_skyname = I::cvar->FindVar("sv_skyname");
	sv_skyname->onChangeCallbacks.size = 0;

	static auto r_3dsky = I::cvar->FindVar("r_3dsky");
	r_3dsky->onChangeCallbacks.size = 0;
	r_3dsky->SetValue(0);

	load_named_sky(name.c_str());
}

void World::SkyboxMod(Material* mat)
{
	mat->ColorModulate(Config::GetColor("visuals-world-skybox-color"));
}

void World::Run(int CurStage)
{
	if (CurStage == FRAME_RENDER_START && G::LocalPlayer && G::LocalPlayerAlive && I::engine->IsInGame()) {
		static int LastState = -1;
		if (Config::GetBool("visuals-world-skybox-enable") && Config::GetState("visuals-world-skybox-name") != LastState)
		{
			LastState = Config::GetState("visuals-world-skybox-name");
			static std::vector<std::string> skyboxes = {"cs_baggage_skybox_", "cs_tibet", "vietnam", "sky_lunacy", "embassy", "italy", "jungle", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_day02_05", "nukeblank", "dustblank", "sky_venice", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02", "sky_csgo_night02b", "vertigo", "vertigoblue_hdr", "sky_dust" };
			SkyboxLoad(skyboxes[LastState]);
		}

		bool UpdateWorld = false;
		if (Config::GetBool("visuals-world-enable"))
		{
			static Color LastColorWorld;
			static int LastAlphaWorld;
			if (LastColorWorld != Config::GetColor("visuals-world-color") ||
				LastAlphaWorld != (int)Config::GetFloat("visuals-world-opacity"))
			{
				LastColorWorld = Config::GetColor("visuals-world-color");
				LastAlphaWorld = (int)Config::GetFloat("visuals-world-opacity");
				UpdateWorld = true;
			}
		}
		

		bool UpdateProp = false;
		/*if (Config::GetBool("visuals-world-prop-enable"))
		{
			static Color LastColorProp;
			static int LastAlphaProp;
			if (LastColorProp != Config::GetColor("visuals-world-prop-color") ||
				LastAlphaProp != (int)Config::GetFloat("visuals-world-prop-opacity"))
			{
				LastColorProp = Config::GetColor("visuals-world-prop-color");
				LastAlphaProp = (int)Config::GetFloat("visuals-world-prop-opacity");
				UpdateProp = true;
			}
		}*/

		bool UpdateSkybox = false;
		if (Config::GetBool("visuals-world-skybox-enable"))
		{
			static Color LastColorSky;
			if (LastColorSky != Config::GetColor("visuals-world-skybox-color"))
			{
				LastColorSky = Config::GetColor("visuals-world-skybox-color");
				UpdateSkybox = true;
			}
		}
		

		for (auto i = I::materialsystem->FirstMaterial(); i != I::materialsystem->InvalidMaterial(); i = I::materialsystem->NextMaterial(i))
		{
			auto mat = I::materialsystem->GetMaterial(i);
			if (!mat)
				continue;

			auto tex_name = mat->GetTextureGroupName();
			
			if(strstr(tex_name, "World") && UpdateWorld)
				WorldMod(mat);
			if (strstr(tex_name, "StaticProp") && UpdateProp)
				PropMod(mat);
			if (strstr(tex_name, "SkyBox") && UpdateSkybox)
				SkyboxMod(mat);
		}
	}
}
