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
		/* OFFENSE TAB */
		{
			Tab* t = new Tab("Offense");
			{
				Widget* w = t->AddWidget("Aimbot");
				w->AddProperty(false, 0, "enable-aimbot", "Enable", new bool(true), new bool(true));
				w->AddProperty(false, 0, "aimbot-autoshoot", "Autoshoot", new bool(true), new bool(true));
				w->AddProperty(false, 1, "aimbot-autowall", "Autowall", new bool(true), new bool(true));

				// gun options
			}
			{
				Widget* w = t->AddWidget("Triggerbot");
				w->AddProperty(false, 0, "enable-triggerbot", "Enable", new bool(true), new bool(true));
				w->AddProperty(false, 1, "triggerbot-through-wall", "Through Wall", new bool(true), new bool(true));
				w->AddProperty(false, 0, "triggerbot-delay", "Delay (ms)", new CFloat(0, 1000, 0, 0), new CFloat(0, 1000, 0, 0));
				w->AddProperty(false, 2, "triggerbot-magnet", "Magnet", new bool(true), new bool(true));
			}
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