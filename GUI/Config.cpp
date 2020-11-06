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
		/* AIMBOT TAB */
		{
			Tab* t = new Tab("Aimbot");

			Tabs.push_back(t);
		}

		/* ANTI-AIM TAB */
		{
			Tab* t = new Tab("Anti-Aim");
			{
				Widget* w = t->AddWidget("Fake Lag");
			}
			{
				Widget* w = t->AddWidget("Legit Anti-Aim"); 
			}
			{
				Widget* w = t->AddWidget("Rage Anti-Aim");
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

		/* MISC TAB */
		{
			Tab* t = new Tab("Misc");
			{
				Widget* w = t->AddWidget("Movement");

				w->AddProperty(false, "movement-bhop", "Bunny Hop", "automatically jumps for you perfectly to \"bhop\"", new bool(false));
				w->AddProperty(false, "movement-slow-walk", "Slow Walk", "automatically slows your speed to increase weapon accuracy", new bool(false));
				w->AddProperty(false, "movement-slow-walk-speed", "Slow Walk Speed", "0 is stopped, 1 is max speed while accurate", new Config::CFloat(0.f, 1.f, 1.f, 0.001f));
				w->AddProperty(false, "movement-slow-walk-key", "SLOW WALK KEY, PLZ ADD KEY BINDING", "the key to slow walk", new bool(false));
				w->AddProperty(false, "movement-air-stuck", "Air Stuck", "stops you in air and hovers (only works on some community servers)", new bool(false));
				w->AddProperty(false, "movement-air-stuck-key", "AIR STUCK KEY, PLZ ADD KEY BINDING", "the key to air stuck", new bool(false));
				w->AddProperty(false, "movement-fast-crouch", "Fast Crouch", "makes your crouching speed faster", new bool(false));
				w->AddProperty(false, "movement-fake-duck", "Fake Duck", "Makes server beleive your'e ducking even though you aren't", new bool(false));
				w->AddProperty(false, "movement-fake-duck-key", "FAKE DUCK KEY, PLZ ADD KEY BINDING", "the key to fake duck", new bool(false));
				w->AddProperty(false, "movement-autostrafe", "ADD UNIQUE SELECTOR FOR AUTOSTRAFE", "automatically strafes in air for you", new bool(false));
			}

			{
				Widget* w = t->AddWidget("Other");

				w->AddProperty(false, "other-auto-accept", "Auto Accept", "automatically accept a competitive match", new bool(false));
				w->AddProperty(false, "other-vote-indicator", "Vote Indicator", "lists the last vote, and who voted what", new bool(false));
				w->AddProperty(false, "other-spectator-list", "Spectator List", "shows who is spectating you when your'e alive", new bool(false));
				w->AddProperty(false, "other-team-damage-list", "Team Damage List", "shows who has done what amount of damage to their teamates", new bool(false));
				w->AddProperty(false, "other-kill-say", "Kill Say", "Insults enemies after you kill them in chat", new bool(false));
				w->AddProperty(false, "other-auto-pistol", "Auto Pistol", "makes all pistols automatic", new bool(false));
				/*
				Fake Unbox - I want the user to be able to type what they want to say, then shoot a teamate and have it pop up, this means
				that we need to have an input
				Clantag - I want the user to be able to type what they want for their clantag
				*/
			}

			Tabs.push_back(t);
		}

		/* CONFIG TAB */
		{
			Tab* t = new Tab("Config");

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