#include "../Include.hpp"

UserInfoStruct Config::UserInfo = UserInfoStruct{};

namespace Config
{
	Config_LegitBot legitbot;
	Config_RageBot ragebot;
	Config_AntiAim antiaim;
	Config_Movement movement;
	Config_Visuals visuals;

	unsigned int RaindropCount = 100;
}

/* V2 stuff i guess*/
namespace Config {
	std::vector<Tab*> Tabs = {};
	std::map<std::string, Property*> PropertyLookup;

	void Init()
	{
		/* GENERAL TAB */
		{
			Tab* t = new Tab("General");
			{
				Widget* w = t->AddWidget("General");
				w->AddProperty(false, "master-enable-general", "Enable", "cock", new bool(true));
			}
			Tabs.push_back(t);
		}

		/* VISUALS TAB */
		{
			Tab* t = new Tab("Visuals");
			{
				Widget* w = t->AddWidget("General");
				w->AddProperty(false, "master-enable-visuals", "Enable", "enables/disables all visuals", new bool(true));
			}
			{
				Widget* w = t->AddWidget("Enemy ESP");

				w->AddProperty(false, "esp-enemy-visible-chams", "Visible Chams", "show chams for visible enemies", new bool(true));
				w->AddProperty(false, "esp-enemy-visible-cham-color", "Visible Cham Color", "color of visible enemy chams", new Color(50, 230, 50, 255));
				w->AddProperty(false, "esp-enemy-visible-cham-opacity", "Visible Cham Opacity", "0 is seethrough, 1 is opaque. see \"Visible Chams\"", new Config::CFloat(0.f, 1.f, 0.25f, 0.001f));

				w->AddProperty(false, "esp-enemy-wall-chams", "Thru Wall Chams", "show chams for enemies behind walls", new bool(true));
				w->AddProperty(false, "esp-enemy-wall-cham-color", "Thru Wall Color", "color of enemy chams behind walls", new Color(230, 50, 50, 255));
				w->AddProperty(false, "esp-enemy-wall-cham-opacity", "Thru Wall Cham Opacity", "0 is seethrough, 1 is opaque. see \"Thru Wall Chams\"", new Config::CFloat(0.f, 1.f, 0.75f, 0.001f));

				w->AddProperty(false, "esp-enemy-bounding-box", "Bounding Box", "show rectangle outline of enemies", new bool(true));
				w->AddProperty(false, "esp-enemy-bounding-box-color", "Bounding Box Color", "color of enemys' \"Bounding Box\"", new Color(255, 255, 255));
				w->AddProperty(false, "esp-enemy-bounding-box-opacity", "Bounding Box Opacity", "0 is seethrough, 1 is opaque. see \"Bounding Box\"", new Config::CFloat(0.f, 1.f, 1.f, 0.001f));

				w->AddProperty(false, "esp-enemy-loadout", "Loadout", "show weapons, grenades, etc. for each enemy (currently held item will be highlighted)", new bool(true));
				w->AddProperty(false, "esp-enemy-extra-info", "Extra Info", "enemy: is flashed? is reloading? out of ammo? etc.", new bool(true));
			}
			{
				Widget* w = t->AddWidget("Team ESP");
			}
			Tabs.push_back(t);
		}

		/* AIMBOT TAB */
		{
			Tab* t = new Tab("Aimbot");

			Tabs.push_back(t);
		}


		/* CONFIG TAB */
		{
			Tab* t = new Tab("Config");

			Tabs.push_back(t);
		}


		/* MISC TAB */
		{
			Tab* t = new Tab("Misc");

			Tabs.push_back(t);
		}



		/* LOAD INTO HASHMAP FOR FAST LOOKUPS */
		for (size_t i = 0; i < Tabs.size(); i++)
		{
			Tab* t = Tabs.at(i);
			for (size_t j = 0; j < t->Widgets.size(); j++)
			{
				Widget* w = t->Widgets.at(j);
				for (size_t k = 0; k < w->Properties.size(); k++)
				{
					Property* p = w->Properties.at(k);
					PropertyLookup.insert(std::pair<std::string, Property*>(p->Name, p));
				}
			}
		}
	}

	bool GetBool(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		return *(bool*)prop->Value;
	}
	float GetFloat(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		return *(float*)prop->Value;
	}
	Color GetColor(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		return *(Color*)prop->Value;
	}

	void SetBool(std::string Name, bool Value)
	{
		Property* prop = PropertyLookup.at(Name);
		*(bool*)prop->Value = Value;
	}
	void SetFloat(std::string Name, float Value)
	{
		Property* prop = PropertyLookup.at(Name);
		*(float*)prop->Value = Value;
	}
	void SetColor(std::string Name, Color Value)
	{
		Property* prop = PropertyLookup.at(Name);
		*(Color*)prop->Value = Value;
	}
}