#include "Include.hpp"

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
	std::vector<Tab> Tabs = {};
	std::map<std::string, Property*> PropertyLookup;

	void Init()
	{
		/* VISUALS */
		Tab Visuals("Visuals");
		{
			Widget General("General");
			{
				General.Properties.push_back(
					Property(false, BooleanValue, "enable-visuals", "Enable", "Enable/Disable All Visuals", new bool(true))
				);
				General.Properties.push_back(
					Property(true, BooleanValue, "recoil-crosshair", "Recoil Crosshair", "Show Recoil Crosshair On Screen", new bool(true))
				);
				General.Properties.push_back(
					Property(false, BooleanValue, "third-person", "Third Person", "Play in Third Person", new bool(true))
				);
			} Visuals.Widgets.push_back(General);

			Widget World("World");
			{
				General.Properties.push_back(
					Property(false, BooleanValue, "transparent-walls", "Transparent Walls", "Enable/Disable Transparent Walls", new bool(true))
				);
				General.Properties.push_back(
					Property(false, DecimalValue, "transparent-walls-value", "Wall Transparency", "How 'See-Through' the Walls Are", new float(0.5))
				);
				General.Properties.push_back(
					Property(false, BooleanValue, "transparent-props", "Transparent Props", "Enable/Disable Transparent Props", new bool(true))
				);
				General.Properties.push_back(
					Property(false, DecimalValue, "transparent-props-value", "Props Transparency", "How 'See-Through' the Props Are", new float(0.5))
				);
			} Visuals.Widgets.push_back(World);

		} Tabs.push_back(Visuals);

		/* LEGITBOT */
		Tab LegitAimbot("Legit Aimbot");
		{
			Widget General("General");
			{
				General.Properties.push_back(
					Property(false, BooleanValue, "enable-legitbot", "Enable", "Enable/Disable Legit Aimbot", new bool(true))
				);
			} LegitAimbot.Widgets.push_back(General);

		} Tabs.push_back(LegitAimbot);


		/* LOAD INTO HASHMAP FOR FAST LOOKUPS */
		for (size_t i = 0; i < Tabs.size(); i++)
		{
			Tab& t = Tabs.at(i);
			for (size_t j = 0; j < t.Widgets.size(); j++)
			{
				Widget& w = t.Widgets.at(j);
				for (size_t k = 0; k < w.Properties.size(); k++)
				{
					Property& p = w.Properties.at(k);
					PropertyLookup.insert(std::pair<std::string, Property*>(p.Name, &p));
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