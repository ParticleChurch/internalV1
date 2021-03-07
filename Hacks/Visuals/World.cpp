#include "../../Include.hpp"

World* world = new World();

void World::WorldMod(Material* mat)
{
	static Config2::CColor* Clr = Config2::GetColor("visuals-world-color");

	mat->ColorModulate(Color(Clr->GetR(), Clr->GetG(), Clr->GetB()));
	mat->AlphaModulate(Clr->GetA() / 255.f);
}

void World::PropMod(Material* mat)
{
	//r_DrawSpecificStaticProp - set to 1 to get props working...
	static auto r_drawspecificstaticprop = I::cvar->FindVar("r_DrawSpecificStaticProp");
	r_drawspecificstaticprop->SetValue(1);

	static Config2::CColor* Clr = Config2::GetColor("visuals-world-prop-color");
	mat->ColorModulate(Color(Clr->GetR(), Clr->GetG(), Clr->GetB()));
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
	static Config2::CColor* Clr = Config2::GetColor("visuals-world-skybox-color");
	mat->ColorModulate(Color(Clr->GetR(), Clr->GetG(), Clr->GetB()));
}

void World::LightMod()
{
	static Config2::CColor* clr = Config2::GetColor("visuals-world-color");
	static Config2::CState* enable = Config2::GetState("visuals-world-enable");

	if (!enable->Get())
		return;

	static ConVar* red = I::cvar->FindVar("mat_ambient_light_r");
	static ConVar* green = I::cvar->FindVar("mat_ambient_light_g");
	static ConVar* blue = I::cvar->FindVar("mat_ambient_light_b");

	

	red->SetValue(clr->GetR() / 255.f);
	green->SetValue(clr->GetG() / 255.f);
	blue->SetValue(clr->GetB() / 255.f);
}

void World::Run_FrameStageNotify(int CurStage)
{
	static Config2::CState* World = Config2::GetState("visuals-world-enable");
	static Config2::CState* Prop = Config2::GetState("visuals-world-prop-enable");
	static Config2::CState* Skybox = Config2::GetState("visuals-world-skybox-enable");
	static Config2::CState* SkyboxState = Config2::GetState("visuals-world-skybox-name");
	static Config2::CColor* WorldClr = Config2::GetColor("visuals-world-color");
	static Config2::CColor* PropClr = Config2::GetColor("visuals-world-prop-color");
	static Config2::CColor* SkyboxClr = Config2::GetColor("visuals-world-skybox-color");

	if (CurStage == FRAME_RENDER_START && G::LocalPlayer && G::LocalPlayerAlive && I::engine->IsInGame()) {
		static int LastState = -1;
		if (Skybox->Get() && SkyboxState->Get() != LastState)
		{
			LastState = SkyboxState->Get();
			static std::vector<std::string> skyboxes = {"cs_baggage_skybox_", "cs_tibet", "vietnam", "sky_lunacy", "embassy", "italy", "jungle", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_day02_05", "nukeblank", "dustblank", "sky_venice", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02", "sky_csgo_night02b", "vertigo", "vertigoblue_hdr", "sky_dust" };
			SkyboxLoad(skyboxes[LastState]);
		}

		bool UpdateWorld = false;
		if (World->Get())
		{
			static Color LastColorWorld;
			static int LastAlphaWorld;
			if (LastColorWorld != Color(WorldClr->GetR(), WorldClr->GetG(), WorldClr->GetB()) ||
				LastAlphaWorld != (int)WorldClr->GetA())
			{
				LastColorWorld = Color(WorldClr->GetR(), WorldClr->GetG(), WorldClr->GetB());
				LastAlphaWorld = (int)WorldClr->GetA();
				UpdateWorld = true;
			}
		}
		

		bool UpdateProp = false;
		if (Prop->Get())
		{
			static Color LastColorProp;
			static int LastAlphaProp;
			if (LastColorProp != Color(PropClr->GetR(), PropClr->GetG(), PropClr->GetB()) ||
				LastAlphaProp != (int)PropClr->GetA())
			{
				LastColorProp = Color(PropClr->GetR(), PropClr->GetG(), PropClr->GetB());
				LastAlphaProp = (int)PropClr->GetA();
				UpdateProp = true;
			}
		}

		bool UpdateSkybox = false;
		if (Skybox->Get())
		{
			static Color LastColorSky;
			if (LastColorSky != Color(SkyboxClr->GetR(), SkyboxClr->GetG(), SkyboxClr->GetB()))
			{
				LastColorSky = Color(SkyboxClr->GetR(), SkyboxClr->GetG(), SkyboxClr->GetB());
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

void World::Run_DoPostScreenEffect()
{
	LightMod();
}
