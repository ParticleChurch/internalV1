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

				w->AddProperty(false, 0, "enable-aimbot", "Enable", true, true);
				w->AddProperty(false, 0, "aimbot-autoshoot", "Autoshoot", true, true);
				w->AddProperty(false, 1, "aimbot-autowall", "Autowall", true, true);
				//w->MarkSeperator();

				// todo: gun options
			}
			{
				Widget* w = t->AddWidget("Triggerbot");

				w->AddProperty(false, 0, "enable-triggerbot", "Enable", true, true);
				w->AddProperty(false, 1, "triggerbot-through-wall", "Through Wall", true, true);
				w->AddProperty(false, 0, "triggerbot-delay", "Delay (ms)", 0, 1000, 0, 0, 0);
				w->AddProperty(false, 2, "triggerbot-magnet", "Magnet", true, true);
			}
			{
				Widget* w = t->AddWidget("Other");
				w->AddProperty(false, 1, "backtracking-time", "Max Macktracking Time (ms)", 0, 200, 0, 0, 0);
			}
			Tabs.push_back(t);
		}
		/* ANTI-AIM  TAB */
		{
			Tab* t = new Tab("Anti-Aim");
			{
				Widget* w = t->AddWidget("Fake Lag");
				//TODO - add dropdown menu for type of fake lag
				//TODO - add amount of the type of fake lag based on the type (distance vs ticks/time)
			}
			{
				Widget* w = t->AddWidget("Legit");

				w->AddProperty(false, 1, "antiaim-legit-enable", "Enable", true, true);
				w->AddProperty(false, 1, "antiaim-legit-max-angle", "Max Desync Angle (%%)", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "antiaim-legit-invert", "Inverter AA", false, false);
			}
			{
				Widget* w = t->AddWidget("Rage");
				//General
				w->AddProperty(false, 0, "antiaim-rage-enable", "Enable", true, true);

				//Pitch
				//TODO - add dropdown pitch option... (up, down, and trolling (up/down))

				//Yaw - Real
				w->AddProperty(false, 2, "antiaim-rage-real", "Real Offset", -180, 180, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-real-jitter", "Real Jitter Offset (deg)", 0, 180, 0, 0, 0);
				//TODO - add 3 keybinds for manual antiaim (left, right, and back) //we need just keybinds lol

				//Yaw - Fake
				w->AddProperty(false, 2, "antiaim-rage-fake", "Fake Offset (%%)", 0, 100, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-fake-jitter", "Fake Jitter Offset (%%)", 0, 100, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-fake-invert", "Invert on Hit", false, false);
			}
			{
				Widget* w = t->AddWidget("Visuals");
				//Fake
				w->AddProperty(false, 0, "antiaim-visual-fake", "Visualize Fake", false, false);
				w->AddProperty(false, 2, "antiaim-visual-fake-lag", "Visualize Lag", false, false);
				w->AddProperty(false, 1, "antiaim-visual-fake-color", "Fake Color", new Color(0, 150, 255));
				w->AddProperty(false, 1, "antiaim-visual-fake-opacity", "Fake Opacity", 0, 1, 3, 1, 1);

				//Real
				w->AddProperty(false, 1, "antiaim-visual-real", "Visualize Real", false, false);
				w->AddProperty(false, 1, "antiaim-visual-real-color", "Real Color", new Color(0, 150, 255));
				w->AddProperty(false, 1, "antiaim-visual-real-opacity", "Real Opacity", 0, 1, 3, 1, 1);
			}
			Tabs.push_back(t);
		}
		/* CONFIG/MENU TAB */
		{
			Tab* t = new Tab("Menu/Config");
			{
				Widget* w = t->AddWidget("Menu");

				//w->AddProperty(false, 0, "menu-complexity", "Menu Complexity", 0, 0, new CDropdown{ "beginner", "intermediate", "pro" });
				w->AddProperty(false, 0, "show-help-link", "Show Help Link", true, true);
				//w->MarkSeperator();
				
				w->AddProperty(false, 0, "show-watermark", "Watermark", true, true);
				w->AddProperty(false, 0, "menu-transparency", "Opacity", 0, 1, 3, 1, 1);
				w->AddProperty(false, 0, "menu-accent-color", "Accent Color", new Color(0, 150, 255));
				//w->MarkSeperator();

				w->AddProperty(false, 0, "config-show-menu", "Show Menu", true, true);
			}
			{
				Widget* w = t->AddWidget("Config");

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

		// default keybinds
		Bind(PropertyLookup.at("config-show-menu"), VK_INSERT);
		PropertyLookup.at("config-show-menu")->BindType = KeybindType::Toggle;

		// assume all keys are not pressed
		for (int i = 0; i < 256; i++)
		{
			KeyReleased(i);
		}
	}

	bool GetBool(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		if (!prop) return false;
		return *(bool*)prop->Value;
	}
	float GetFloat(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		if (!prop) return 0;
		return *(float*)prop->Value;
	}
	Color GetColor(std::string Name)
	{
		Property* prop = PropertyLookup.at(Name);
		if (!prop) return Color(0,0,0);
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

	/* Keybind Stuff */
	std::map<int, std::vector<Property*>*> KeybindMap;
	void Bind(Property* Prop, WPARAM KeyCode)
	{
		if (Prop->Type != PropertyType::BOOLEAN) return; // bruh
		if (KeyCode == 0) return Unbind(Prop); // binding a key to 0 is the same as unbinding
		if (Prop->KeyBind != 0) Unbind(Prop); // this key is already bound, first unbind it before preceeding

		auto KeyIterator = KeybindMap.find(KeyCode);
		std::vector<Property*>* PropertyList = nullptr;
		if (KeyIterator == KeybindMap.end())
			KeybindMap.insert(std::pair<int, std::vector<Property*>*>(KeyCode, PropertyList = new std::vector<Property*>));
		else
			PropertyList = KeyIterator->second;

		Prop->KeyBind = KeyCode;
		PropertyList->push_back(Prop);
	};
	void Unbind(Property* Prop)
	{
		if (Prop->Type != PropertyType::BOOLEAN) return; // bruh
		if (Prop->KeyBind == 0) return; // already unbound
		
		auto KeyIterator = KeybindMap.find(Prop->KeyBind);
		if (KeyIterator == KeybindMap.end())
		{
			// this property isn't actually bound
			Prop->KeyBind = 0;
			return;
		}
		
		// search for the property, and delete it from the list
		std::vector<Property*>* PropertyList = KeyIterator->second;
		for (size_t i = 0; i < PropertyList->size(); i++)
		{
			if (PropertyList->at(i) == Prop)
			{
				PropertyList->erase(PropertyList->begin() + i);
				Prop->KeyBind = 0;
				break;
			}
		}

		// if the vector is now empty, free it and remove it from the map
		if (PropertyList->empty())
		{
			KeybindMap.erase(KeyIterator);
			free(PropertyList);
		}
	};
	void KeyPressed(WPARAM KeyCode)
	{
		if (KeyCode == 0) return;

		// get a list of keys that have this bind
		auto KeyIterator = KeybindMap.find(KeyCode);
		if (KeyIterator == KeybindMap.end()) return; // there are no keys with this bind

		std::vector<Property*>* PropertyList = KeyIterator->second;
		for (size_t i = 0; i < PropertyList->size(); i++)
		{
			Property* prop = PropertyList->at(i);
			switch (prop->BindType)
			{
				case KeybindType::Toggle:
				{
					*(bool*)prop->Value = !(*(bool*)prop->Value);
					break;
				}
				case KeybindType::HoldToEnable:
				{
					*(bool*)prop->Value = true;
					break;
				}
				case KeybindType::HoldToDisable:
				{
					*(bool*)prop->Value = false;
					break;
				}
			}
		}
	};
	void KeyReleased(WPARAM KeyCode)
	{
		if (KeyCode == 0) return;

		// get a list of keys that have this bind
		auto KeyIterator = KeybindMap.find(KeyCode);
		if (KeyIterator == KeybindMap.end()) return; // there are no keys with this bind

		std::vector<Property*>* PropertyList = KeyIterator->second;
		for (size_t i = 0; i < PropertyList->size(); i++)
		{
			Property* prop = PropertyList->at(i);
			switch (prop->BindType)
			{
				case KeybindType::Toggle:
				{
					// it toggled when we pressed the key down, don't toggle again
					break;
				}
				case KeybindType::HoldToEnable:
				{
					*(bool*)prop->Value = false;
					break;
				}
				case KeybindType::HoldToDisable:
				{
					*(bool*)prop->Value = true;
					break;
				}
			}
		}
	};
}
