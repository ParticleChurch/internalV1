#include "../../Include.hpp"

World* world = new World();

void World::WorldMod(Material* mat)
{
	static Config::CColor* Clr = Config::GetColor("visuals-world-color");

	mat->ColorModulate(Clr->GetR() / 255.f, Clr->GetG() / 255.f, Clr->GetB() / 255.f);
	mat->AlphaModulate(Clr->GetA() / 255.f);
}

void World::PropMod(Material* mat)
{
	//r_DrawSpecificStaticProp - set to 1 to get props working...
	static auto r_drawspecificstaticprop = I::cvar->FindVar("r_DrawSpecificStaticProp");
	r_drawspecificstaticprop->SetValue(1);

	static Config::CColor* Clr = Config::GetColor("visuals-world-prop-color");
	mat->ColorModulate(Clr->GetR() / 255.f, Clr->GetG() / 255.f, Clr->GetB() / 255.f);
	mat->AlphaModulate(Clr->GetA() / 255.f);
	
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
	static Config::CColor* Clr = Config::GetColor("visuals-world-skybox-color");
	mat->ColorModulate(Clr->GetR() / 255.f, Clr->GetG() / 255.f, Clr->GetB() / 255.f);
}

void World::LightMod()
{
	static Config::CColor* clr = Config::GetColor("visuals-world-color");
	static Config::CState* enable = Config::GetState("visuals-world-enable");

	if (!enable->Get())
		return;

	static ConVar* red = I::cvar->FindVar("mat_ambient_light_r");
	static ConVar* green = I::cvar->FindVar("mat_ambient_light_g");
	static ConVar* blue = I::cvar->FindVar("mat_ambient_light_b");

	

	red->SetValue(clr->GetR() / 255.f);
	green->SetValue(clr->GetG() / 255.f);
	blue->SetValue(clr->GetB() / 255.f);
}

void World::RunFSN(int stage)
{
	static bool LastInGame = false;
	bool ForceUpdate = (LastInGame != G::IsInGame);

	LastInGame = G::IsInGame;

	if (stage != FRAME_RENDER_START)
		return;

	static Config::CState* World = Config::GetState("visuals-world-enable");
	static Config::CState* Prop = Config::GetState("visuals-world-prop-enable");
	static Config::CState* SkyboxEnabled = Config::GetState("visuals-world-skybox-enable");
	static Config::CState* SkyboxState = Config::GetState("visuals-world-skybox-name");
	static Config::CColor* WorldClr = Config::GetColor("visuals-world-color");
	static Config::CColor* PropClr = Config::GetColor("visuals-world-prop-color");
	static Config::CColor* SkyboxClr = Config::GetColor("visuals-world-skybox-color");

	bool UpdateWorld = false;
	if (World->Get())
	{
		static uint32_t LastColor = WorldClr->As32Bit() ^ 1;
		uint32_t DesiredColor = WorldClr->As32Bit();
		UpdateWorld = LastColor != DesiredColor;
		LastColor = DesiredColor;
	}
		
	bool UpdateProp = false;
	if (Prop->Get())
	{
		static uint32_t LastColor = PropClr->As32Bit() ^ 1;
		uint32_t DesiredColor = PropClr->As32Bit();
		UpdateProp = LastColor != DesiredColor;
		LastColor = DesiredColor;
	}

	bool UpdateSkybox = false;
	if (SkyboxEnabled->Get())
	{
		static int LastSkybox = SkyboxState->Get() ^ 1;
		static uint32_t LastColor = PropClr->As32Bit() ^ 1;

		int DesiredSkybox = SkyboxState->Get();
		uint32_t DesiredColor = PropClr->As32Bit();
		UpdateSkybox = LastColor != DesiredColor || LastSkybox != DesiredSkybox;
		LastSkybox = DesiredSkybox;
		LastColor = DesiredColor;
	}

	bool ShouldReturn = !UpdateSkybox && !UpdateProp && !UpdateWorld && !ForceUpdate;

	if (ShouldReturn)
		return; // everything already up to date

	if (UpdateSkybox)
	{
		static std::vector<std::string> skyboxes = { "cs_baggage_skybox_", "cs_tibet", "vietnam", "sky_lunacy", "embassy", "italy", "jungle", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_day02_05", "nukeblank", "dustblank", "sky_venice", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02", "sky_csgo_night02b", "vertigo", "vertigoblue_hdr", "sky_dust" };
		SkyboxLoad(skyboxes[SkyboxState->Get()]);
	}

	for (auto i = I::materialsystem->FirstMaterial(); i != I::materialsystem->InvalidMaterial(); i = I::materialsystem->NextMaterial(i))
	{
		auto mat = I::materialsystem->GetMaterial(i);
		if (!mat)
			continue;

		auto tex_name = mat->GetTextureGroupName();
			
		if(UpdateWorld && strstr(tex_name, "World"))
			WorldMod(mat);
		else if (UpdateProp && strstr(tex_name, "StaticProp"))
			PropMod(mat);
		else if (UpdateSkybox && strstr(tex_name, "SkyBox"))
			SkyboxMod(mat);
	}
}

void World::Run_DoPostScreenEffect()
{
	LightMod();
}
