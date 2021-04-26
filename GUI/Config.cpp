#include "../Include.hpp"

// config macro utils
#define CONFIG_PROPERTY_TYPE_CHECK(p, t, ret) \
if (L::OutputMode != L::LogMode::None && p->Type != t){ \
	L::Log("ERROR: You used the wrong getter for property: ", ""); \
	L::Log(p->Name.c_str()); \
	return ret; \
}
#define CONFIG_VIS(p, link, state, statev) \
{ \
	Property* _p = (p); \
	Property* _l = (link); \
	CState* _s = (state); \
	int _v = (statev); \
	_p->VisibilityLinked = _l; \
	_p->Visible.State = _s; \
	_p->Visible.StateEquals = _v; \
}

namespace Config2
{
	std::vector<CPaintKit*> WeaponPaintKits = {};
	std::vector<CPaintKit*> KnifePaintKits = {};
	uint64_t GUIFramesRenderedCounter = 0;
	std::map<std::string, Property*> PropertyTable{};
	std::vector<Tab*> Tabs{};
	Property* SettingKeybindFor = nullptr;
	std::string KeybindTypeNames[] = {
		"Toggle", "Enable", "Disable"
	};

	void Init()
	{
		//Tab* t;
		//Group* g;
		Property* p;
		Property* p1;

		// OFFENCE
		{
			// IDK WHY EVERYTHING IN OFFENSE DOESN"T WORK!
			Tab* t = new Tab("Offence");
			t->TopPadding = -10;
			Group* OffenceMeta = t->Add("__META__");
			Property* OffenceMode = OffenceMeta->Add("offence-mode", "", new CHorizontalState({ "Legit", "Rage" }, false));

			/*
				LEGIT PAGE
			*/
			{
				Group* g = t->Add("legit-Aimbot");

				p = g->Add("legit-aim-enable", "Enable", new CBoolean());
				g->Add("legitaim-weapontype", "", new CHorizontalState({ "Pistol", "SMG", "Heavy", "Rifle", "Sniper" }));
				g->BeginMaster(p);

#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Stomach", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"
				p = g->Add("legitaim-pistol-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), 0);
				g->Add("legitaim-pistol-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
				g->Add("legitaim-pistol-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
				g->Add("legitaim-pistol-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;

				p = g->Add("legitaim-smg-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), 1);
				g->Add("legitaim-smg-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
				g->Add("legitaim-smg-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
				g->Add("legitaim-smg-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;

				p = g->Add("legitaim-heavy-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), 2);
				g->Add("legitaim-heavy-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
				g->Add("legitaim-heavy-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
				g->Add("legitaim-heavy-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;

				p = g->Add("legitaim-rifle-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), 3);
				g->Add("legitaim-rifle-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
				g->Add("legitaim-rifle-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
				g->Add("legitaim-rifle-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;

				p = g->Add("legitaim-sniper-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), 4);
				g->Add("legitaim-sniper-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
				g->Add("legitaim-sniper-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
				g->Add("legitaim-sniper-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
#undef HITBOXES_CONFIG

				g->EndMaster();

				
			}
			{
				Group* g = t->Add("legit-Backtrack");
				g->Add("legitaim-backtrack-time", "Backtrack TIme", new CFloat(0, 200, 1, "MS"));
			}


			/*
				RAGE PAGE
			*/
			{
				Group* g = t->Add("rage-Aimbot");

				g->BeginMaster(g->Add("rage-aim-enable", "Enable", new CBoolean()));
				g->Add("rage-aim-silent", "Silent Aim", new CBoolean());
				g->Add("rage-aim-autoshoot", "Auto Shoot", new CBoolean());
				g->Add("rage-aim-autoscope", "Auto Scope", new CBoolean());

				g->Add("rageaim-weapontype", "", new CHorizontalState({ "Pistol", "SMG", "Heavy", "Rifle", "Scout", "AWP", "Auto" }));

#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Stomach", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"

				p = g->Add("rageaim-pistol-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 0);
				g->Add("rageaim-pistol-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-pistol-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-pistol-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-pistol-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-pistol-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-pistol-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

				p = g->Add("rageaim-smg-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 1);
				g->Add("rageaim-smg-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-smg-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-smg-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-smg-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-smg-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-smg-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

				p = g->Add("rageaim-heavy-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 2);
				g->Add("rageaim-heavy-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-heavy-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-heavy-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-heavy-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-heavy-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-heavy-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

				p = g->Add("rageaim-rifle-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 3);
				g->Add("rageaim-rifle-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-rifle-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-rifle-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-rifle-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-rifle-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-rifle-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

				p = g->Add("rageaim-scout-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 4);
				g->Add("rageaim-scout-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-scout-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-scout-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-scout-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-scout-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-scout-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
					
				p = g->Add("rageaim-awp-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 5);
				g->Add("rageaim-awp-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-awp-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-awp-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-awp-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-awp-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-awp-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

				p = g->Add("rageaim-auto-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
				CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), 6);
				g->Add("rageaim-auto-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-auto-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
				g->Add("rageaim-auto-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("rageaim-auto-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-auto-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
				g->Add("rageaim-auto-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;

#undef HITBOXES_CONFIG
			}
		}

		// DEFENCE
		{
			Tab* t = new Tab("Defense");
			{
				Group* g = t->Add("Fake Lag");

				g->Add("antiaim-fakelag-tick", "Amount", new CFloat(0, 16, 0, "TICKS"));
				g->Add("antiaim-fakelag-distance", "Distance", new CFloat(0, 64, 1, "UNITS")); //4096	
				
				g->Add("antiaim-fakelag-trigger-tick", "Trigger Amount", new CFloat(0, 16, 0, "TICKS"));
				g->Add("antiaim-fakelag-trigger-distance", "Trigger Distance", new CFloat(0, 64, 1, "UNITS")); //4096	
			}
			{
				Group* g = t->Add("Antiaim");
				g->Add("antiaim-mode", "", new CHorizontalState({ "Legit", "Rage" }));

				p = g->Add("antiaim-legit-enable", "Enable", new CBoolean());
				p->Visible.State = GetState("antiaim-mode");
				p->Visible.StateEquals = 0;
				g->Add("antiaim-legit-max-angle", "Max Desync Angle", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("antiaim-legit-invert", "AA Direction", new CVerticalState({ "Left", "Right" }, true))->VisibilityLinked = p;
				
				p = g->Add("antiaim-rage-enable", "Enable", new CBoolean());
				p->Visible.State = GetState("antiaim-mode");
				p->Visible.StateEquals = 1;
				g->Add("antiaim-rage-pitch", "Pitch", new CVerticalState({ "Up", "Down", "Trolling" }))->VisibilityLinked = p;
				g->Add("antiaim-rage-real", "Real Offset", new CFloat(-180, 180, 1, "DEG"))->VisibilityLinked = p;
				g->Add("antiaim-rage-real-jitter", "Real Jitter Offset", new CFloat(-180, 180, 1, "DEG"))->VisibilityLinked = p;
				g->Add("antiaim-rage-fake", "Fake Offset", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("antiaim-rage-fake-jitter", "Fake Jitter Offset", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("antiaim-rage-fake-invert", "Invert on Hit", new CBoolean())->VisibilityLinked = p;
			}
		}

		// VISUALS
		{
			Tab* t = new Tab("Visuals");
			{
				Group* g = t->Add("Chams");

				// sliders at the top
				g->Add("chams-mode", "", new CHorizontalState({ "Enemies", "Friends", "Local"}));

				p = g->Add("chams-enemy-visibility", "", new CHorizontalState({ "Visible", "Hidden" }));
				p->Visible.State = GetState("chams-mode");
				p->Visible.StateEquals = 0;

				p = g->Add("chams-friend-visibility", "", new CHorizontalState({ "Visible", "Hidden" }));
				p->Visible.State = GetState("chams-mode");
				p->Visible.StateEquals = 1;

				p = g->Add("chams-local-visibility", "", new CHorizontalState({ "Fake", "Real" }));
				p->Visible.State = GetState("chams-mode");
				p->Visible.StateEquals = 2;

				// properties
#define CHAM_MATERIALS "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent", "Glow"
				// Enemies && Visible
				p = g->Add("visuals-chams-enemy-visible-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-enemy-visibility");
				p->Visible.State = GetState("chams-enemy-visibility");
				p->Visible.StateEquals = 0;
				g->Add("visuals-chams-enemy-visible-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-enemy-visible-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;

				// Enemies && Hidden
				p = g->Add("visuals-chams-enemy-hidden-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-enemy-visibility");
				p->Visible.State = GetState("chams-enemy-visibility");
				p->Visible.StateEquals = 1;
				g->Add("visuals-chams-enemy-hidden-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-enemy-hidden-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;

				// Friend && Visible
				p = g->Add("visuals-chams-friend-visible-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-friend-visibility");
				p->Visible.State = GetState("chams-friend-visibility");
				p->Visible.StateEquals = 0;
				g->Add("visuals-chams-friend-visible-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-friend-visible-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;

				// Friend && Hidden
				p = g->Add("visuals-chams-friend-hidden-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-friend-visibility");
				p->Visible.State = GetState("chams-friend-visibility");
				p->Visible.StateEquals = 1;
				g->Add("visuals-chams-friend-hidden-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-friend-hidden-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;

				// Self && Real
				p = g->Add("visuals-chams-localplayer-real-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-local-visibility");
				p->Visible.State = GetState("chams-local-visibility");
				p->Visible.StateEquals = 1;
				g->Add("visuals-chams-localplayer-real-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-localplayer-real-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;

				// Self && Fake
				p = g->Add("visuals-chams-localplayer-fake-enable", "Enable", new CBoolean());
				p->VisibilityLinked = GetProperty("chams-local-visibility");
				p->Visible.State = GetState("chams-local-visibility");
				p->Visible.StateEquals = 0;
				g->Add("visuals-chams-localplayer-fake-color", "Color", new CColor(true))->VisibilityLinked = p;
				g->Add("visuals-chams-localplayer-fake-material", "Material", new CVerticalState({ CHAM_MATERIALS }))->VisibilityLinked = p;
#undef CHAM_MATERIALS
			}
			{
				Group* g = t->Add("ESP");
				g->Add("esp-mode", "", new CHorizontalState({ "Enemies", "Friends"}));

				p = g->Add("visuals-esp-enemy-enable", "Enable", new CBoolean());
				p->Visible.State = GetState("esp-mode");
				p->Visible.StateEquals = 0;
				g->BeginMaster(p);
				(p1 = g->Add("visuals-esp-enemy-bbox", "Bounding Box", new CBoolean()))->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-bbox-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-bbox"), 1);
				g->Add("visuals-esp-enemy-name", "Name", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-name-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-name"), 1);
				g->Add("visuals-esp-enemy-indicator", "Indicators", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-indicator-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-indicator"), 1);
				g->Add("visuals-esp-enemy-skeleton", "Skeleton", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-skeleton-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-skeleton"), 1);
				g->Add("visuals-esp-enemy-health", "Health", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-health-color", "Foreground", new CColor(true)), p, GetState("visuals-esp-enemy-health"), 1);
				CONFIG_VIS(g->Add("visuals-esp-enemy-health-color-background", "Background", new CColor(true)), p, GetState("visuals-esp-enemy-health"), 1);
				g->Add("visuals-esp-enemy-resolverflags", "Resolver Flags", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-resolverflags-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-resolverflags"), 1);
				g->Add("visuals-esp-enemy-weapon", "Weapon", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-enemy-weapon-color", "Color", new CColor(true)), p, GetState("visuals-esp-enemy-weapon"), 1);
				g->EndMaster();

				p = g->Add("visuals-esp-friend-enable", "Enable", new CBoolean());
				p->Visible.State = GetState("esp-mode");
				p->Visible.StateEquals = 1;
				g->BeginMaster(p);
				(p1 = g->Add("visuals-esp-friend-bbox", "Bounding Box", new CBoolean()))->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-friend-bbox-color", "Color", new CColor(true)), p, GetState("visuals-esp-friend-bbox"), 1);
				g->Add("visuals-esp-friend-name", "Name", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-friend-name-color", "Color", new CColor(true)), p, GetState("visuals-esp-friend-name"), 1);
				g->Add("visuals-esp-friend-skeleton", "Skeleton", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-friend-skeleton-color", "Color", new CColor(true)), p, GetState("visuals-esp-friend-skeleton"), 1);
				g->Add("visuals-esp-friend-health", "Health", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(g->Add("visuals-esp-friend-health-color", "Foreground", new CColor(true)), p, GetState("visuals-esp-friend-health"), 1);
				CONFIG_VIS(g->Add("visuals-esp-friend-health-color-background", "Background", new CColor(true)), p, GetState("visuals-esp-friend-health"), 1);
				g->EndMaster();
			}
			{
				Group* g = t->Add("World");
				//World
				g->Add("visuals-world-enable", "Ambience Affected", new CBoolean());
				CONFIG_VIS(g->Add("visuals-world-color", "Color", new CColor(true)), nullptr, GetState("visuals-world-enable"), 1);
				//Prop
				g->Add("visuals-world-prop-enable", "Props Affected", new CBoolean());
				CONFIG_VIS(g->Add("visuals-world-prop-color", "Color", new CColor(true)), nullptr, GetState("visuals-world-prop-enable"), 1);
				//Skybox
				g->Add("visuals-world-skybox-enable", "Skybox Affected", new CBoolean());
				//Skybox dropdown from https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Sky_List
				CONFIG_VIS(g->Add("visuals-world-skybox-name", "Skybox", new CVerticalState({
						"cs_baggage_skybox_", "cs_tibet", "vietnam", "sky_lunacy", "embassy",
						"italy", "jungle", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr",
						"sky_day02_05", "nukeblank", "dustblank", "sky_venice", "sky_cs15_daylight03_hdr",
						"sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02",
						"sky_csgo_night02b", "vertigo", "vertigoblue_hdr", "sky_dust"
					})
				), nullptr, GetState("visuals-world-skybox-enable"), 1);
				CONFIG_VIS(g->Add("visuals-world-skybox-color", "Color", new CColor(true)), nullptr, GetState("visuals-world-skybox-enable"), 1);

			}
			/*{
				Widget* w = t->AddWidget("View Model");
				w->AddProperty(false, 0, "visuals-fov", "FOV", "", 0, 100.f, 2, 0, 0);
				w->AddProperty(false, 0, "visuals-view-x", "Viewmodel X", "", -20.f, 20.f, 3, 0, 0);
				w->AddProperty(false, 0, "visuals-view-y", "Viewmodel Y", "", -20.f, 20.f, 3, 0, 0);
				w->AddProperty(false, 0, "visuals-view-z", "Viewmodel Z", "", -20.f, 20.f, 3, 0, 0);
			}*/
			{
				Group* g = t->Add("Misc");
				g->Add("visuals-misc-thirdperson", "Thirdperson", new CBoolean());
				g->Add("visuals-misc-thirdperson-distance", "Thirdperson Distance", new CFloat(0, 150, 1, ""));
				
				g->Add("visuals-misc-revealranks", "Reveal Ranks", new CBoolean());
				g->Add("visuals-misc-grenadeprediction", "Grenade Prediction", new CBoolean());
				g->Add("visuals-misc-noscope", "No Scope", new CBoolean());
				g->Add("visuals-misc-nosmoke", "No Smoke", new CBoolean());
				g->Add("visuals-misc-noflash", "No Flash", new CBoolean());
				g->Add("visuals-misc-tracers", "Bullet Tracers", new CBoolean());
				g->Add("visuals-misc-forcecrosshair", "Force Crosshair", new CBoolean());

				/*w->AddProperty(false, 2, "visuals-misc-skin", "Skin ID", "", 0, 1000, 0, 0, 0);*/
				/*w->AddProperty(false, 2, "visuals-misc-vote-indicator", "Vote Indicator", false, false);
				w->AddProperty(false, 1, "visuals-misc-spec-list", "Spectator List", false, false);
				w->AddProperty(false, 1, "visuals-misc-teamdamage-list", "Team-damage List", false, false);*/
			}
		}

		// SKINCHANGER
		{
			Tab* t = new Tab("Skinchanger");

			{ // knives
				Group* g = t->Add("Knives");
				g->Add("skinchanger-knife-enable", "Enable Knife Changer", new CBoolean(false));
				g->Add("skinchanger-knife-model", "Knife Model", new CVerticalState(Skins::KnifeNames, false));
				g->Add("skinchanger-knife-wear", "Skin Wear", new CFloat(0, 1, 4));
				g->Add("skinchanger-knife-nametag", "Nametag", new CTextInput(32));
				g->Add("skinchanger-knife-stattrak", "StatTrak", new CBoolean(false));
				g->Add("skinchanger-knife-stattrak-count", "StatTrak Kills", new CTextInput(6)); // 999999
				g->Add("skinchanger-knife-paintkit", "", new CPaintKit());

				strcpy(GetText("skinchanger-knife-stattrak-count")->Data, "69420");
			}

			{ // weapons
				Group* g = t->Add("Guns");

				for (int i = 0; i < (int)Skins::Weapon::_COUNT; i++)
					WeaponPaintKits.push_back((CPaintKit*)g->Add("skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[i])), Skins::WeaponNames[i], new CPaintKit())->Value);
			}

			{ // gloves
				Group* g = t->Add("Gloves");

				g->Add("skinchanger-glove-bruh", "Coming Soon!", new CLabel());
			}
		}

		// MISC
		{
			Tab* t = new Tab("Misc");
			{
				Group* g = t->Add("Movement");
				g->Add("misc-movement-bhop", "Bunnyhop", new CBoolean());
				g->Add("misc-movement-autostrafe", "Autostrafe", new CVerticalState({ "None", "Rage", "Legit" }));
				//g->Add("misc-movement-bhop-chance", "Bunnyhop Chance", new CFloat(0, 100, 1, "%"));
				g->Add("misc-movement-autostop", "AutoStop", new CBoolean());
				g->Add("misc-movement-slowwalk", "Slow Walk", new CBoolean());
				g->Add("misc-movement-slowwalk-speed", "Slow-Walk Speed", new CFloat(0, 100, 1, "%"));
				g->Add("misc-movement-fastcrouch", "Fast Crouch", new CBoolean());
				g->Add("misc-movement-fakeduck", "Fake Duck", new CBoolean());
				g->Add("misc-movement-leg", "Leg Slide", new CBoolean());
				g->Add("misc-movement-leg-time", "Leg Slide Time", new CFloat(0, 2000, 1, "MS"));

			}
			{
				Group* g = t->Add("Other");
				g->Add("show-menu", "Menu Toggle", new CBoolean(true));
				g->Add("misc-other-logs", "Shot Logs", new CBoolean());
				g->Add("misc-other-autoaccept", "AutoAccept", new CBoolean());
				g->Add("misc-other-killsay", "KillSay", new CBoolean());
				g->Add("misc-other-killsay-input", "KillSay Text", new CTextInput());
				g->Add("misc-other-clantag", "Clantag Changer", new CBoolean());
				g->Add("misc-other-clantag-input", "Clantag Text", new CTextInput(16));
				//TODO - add dropdown to indicate what kind of clan tag animation they are using
			}
		}

		// DEV Features
		{
			Tab* t = new Tab("Dev");
			{
				Group* g = t->Add("Tickbase Manipulation");
				g->Add("tickbase-shift", "Shift Amount", new CFloat(0, 55, 0, "TICKS"));
				g->Add("tickbase-recharge", "Recharge Amount", new CFloat(0, 64, 0, "TICKS"));
			}
			// Need to add buttons to run functions (maybe new CFunc(<insert address of func>)
			// for reseting and iterating resolver
			{
				Group* g = t->Add("Cheat Specific");
				g->Add("clantag-special", "Special Clantag", new CBoolean());
			}
		}

		// THEME
		{
			Tab* t = new Tab("Theme");
			t->TopPadding = -10;

			{
				Group* g = t->Add("General");

				g->Add("theme-background", "Background Color", new CColor(false));
				g->Add("theme-background-image", "Background Image (Coming Soon!)", new CLabel());

				g->Add("theme-widget-background", "Widget Background", new CColor(true));
				g->Add("theme-widget-title", "Widget Title", new CColor(true));

				g->Add("theme-topbar-background", "Topbar Background", new CColor(true));
				g->Add("theme-topbar-text", "Topbar Text", new CColor(true));
				g->Add("theme-border-size", "Outline Thickness", new CFloat(0.f, 5.f, 1, "PX"));
				g->Add("theme-border", "Outline", new CColor(true));

				g->Add("theme-scrollbar-background", "Scrollbar Background", new CColor(true));
				g->Add("theme-scrollbar-grabber", "Scrollbar Grabber", new CColor(true));
			}

			{
				Group* g = t->Add("Left Side Overlay");

				g->Add("theme-overlay-background", "Background", new CColor(true));
				g->Add("theme-overlay-text", "Text", new CColor(true));
				g->Add("theme-overlay-active-text", "Active Tab Text", new CColor(true));
				g->Add("theme-overlay-active-pointer", "Active Tab Arrow", new CColor(true));

				g->Add("theme-main-searchbar-background", "Search Bar Background", new CColor(true));
				g->Add("theme-main-searchbar-text", "Search Bar Text", new CColor(true));
			}

			{
				Group* g = t->Add("General Properties");

				g->Add("theme-property-example1", "Example Reference 1", new CBoolean());
				g->Add("theme-property-example2", "Example Reference 2", new CFloat(69.f, 420.f, 2, "UNIT"));
				g->Add("theme-property-example6", "Example Reference 3", new CTextInput(69));

				g->Add("theme-property-text", "Text", new CColor(true));
				g->Add("theme-property-base", "Base", new CColor(false));
				g->Add("theme-property-accent", "Accent", new CColor(false));

				g->Add("theme-button-text", "Button Text", new CColor(true));
				g->Add("theme-button-background", "Button Background", new CColor(false));
				g->Add("theme-button-hovered", "Button Hovered", new CColor(false));
				g->Add("theme-button-active", "Button Pressed", new CColor(false));
				g->Add("theme-button-border-size", "Button Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-button-border", "Button Outline", new CColor(false));

				g->Add("theme-text-input-background", "Text Input Background", new CColor(false));
				g->Add("theme-text-input-text", "Text Input", new CColor(true));

				g->Add("theme-info-icon", "Info Icon", new CColor(false));
				g->Add("theme-warning", "Warning Icon", new CColor(false));
				g->Add("theme-error", "Error Icon", new CColor(false));
			}

			{
				Group* g = t->Add("Slide-Select & Dropdowns");

				g->Add("theme-property-example5", "Slide-Select Reference", new CHorizontalState({ "Here", "Are", "Some", "Options" }));
				g->Add("theme-property-example3", "Dropdown Reference 1", new CMultiSelect({ "Never", "Gonna", "Give", "You", "Up" }));
				g->Add("theme-property-example4", "Dropdown Reference 2", new CVerticalState({ "Never", "Gonna", "Let", "You", "Down" }, true));

				g->Add("theme-hstate-background", "Slide-Select Background", new CColor(false));
				g->Add("theme-hstate-highlight", "Slide-Select Highlight", new CColor(false));
				g->Add("theme-hstate-text", "Slide-Select Text", new CColor(true));

				g->Add("theme-dropdown-background", "Dropdown Background", new CColor(false));
				g->Add("theme-dropdown-text", "Dropdown Text", new CColor(true));
				g->Add("theme-dropdown-border-size", "Dropdown Outline Size", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-dropdown-border", "Dropdown Outline", new CColor(false));
			}

			{
				Group* g = t->Add("Color Editor");

				g->Add("theme-color-editor-background", "Background", new CColor(false));
				g->Add("theme-color-editor-text", "Text", new CColor(true));
				g->Add("theme-color-editor-border-size", "Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-color-editor-border", "Outline", new CColor(true));
			}

			{
				Group* g = t->Add("Legit/Rage Switch For Offence & Defence");

				g->Add("theme-legit-rage-switch-text", "Text", new CColor(true));
				g->Add("theme-legit-rage-switch-background", "Background", new CColor(true));
				g->Add("theme-legit-rage-switch-highlight", "Highlight", new CColor(true));
				g->Add("theme-legit-rage-switch-border-size", "Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-legit-rage-switch-outline", "Outline", new CColor(true));
			}

			{
				Group* g = t->Add("Misc");

				g->Add("theme-eject", "Eject Label", new CColor(true));
				g->Add("theme-eject-button", "Eject Button", new CColor(true));
				g->Add("theme-eject-button-text", "Eject Button Text", new CColor(true));

				g->Add("theme-tooltip-background", "Tooltip Background", new CColor(false));
				g->Add("theme-tooltip-text", "Tooltip Text", new CColor(true));
				g->Add("theme-tooltip-border-size", "Tooltip Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-tooltip-border", "Tooltip Outline", new CColor(true));
			}
		}

		

		// CONFIG
		{
			Tab* t = new Tab("Config");
		}

		// EJECT
		{
			Tab* t = new Tab("Eject");
		}

		ImportTheme(ConfigConstants::ThemeDark, ConfigConstants::ThemeDarkSize);
		ImportConfig(ConfigConstants::ConfigOff, ConfigConstants::ConfigOffSize);
		_BindToKey(GetProperty("show-menu"), Keybind::ReverseKeyMap(VK_INSERT));
		GetState("show-menu")->Set(1);
	}

	Property* GetProperty(std::string Name)
	{
		auto Search = PropertyTable.find(Name);
		if (Search == PropertyTable.end())
		{
			L::Log(("nonexistent property: \"" + Name + "\"").c_str());
			return nullptr;
		}
		else
			return Search->second;
	}

	int GetKeybind(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return -1;

		switch (p->Type)
		{
		case PropertyType::BOOLEAN:
		{
			CBoolean* v = (CBoolean*)p->Value;
			if (v->BoundToKey >= 0)
				return Keybind::KeyMap[v->BoundToKey];
		} break;
		case PropertyType::HSTATEFUL:
		{
			CHorizontalState* v = (CHorizontalState*)p->Value;
			if (v->BoundToKey >= 0)
				return Keybind::KeyMap[v->BoundToKey];
		} break;
		}

		return -1;
	}

	CFloat* GetFloat(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;
		CONFIG_PROPERTY_TYPE_CHECK(p, PropertyType::FLOAT, nullptr);

		return (CFloat*)p->Value;
	}

	CPaintKit* GetPaintKit(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;
		CONFIG_PROPERTY_TYPE_CHECK(p, PropertyType::PAINTKIT, nullptr);

		return (CPaintKit*)p->Value;
	}

	CState* GetState(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;

		switch (p->Type)
		{
		case PropertyType::BOOLEAN:
			return &((CBoolean*)p->Value)->Value;
		case PropertyType::HSTATEFUL:
			return &((CHorizontalState*)p->Value)->Value;
		case PropertyType::VSTATEFUL:
			return &((CVerticalState*)p->Value)->Value;
		default:
			L::Log("ERROR: You used the wrong getter for property: ", "");
			L::Log(p->Name.c_str());
			return nullptr;
		}
	}

	CColor* GetColor(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;
		CONFIG_PROPERTY_TYPE_CHECK(p, PropertyType::COLOR, nullptr);

		return (CColor*)p->Value;
	}

	CMultiSelect* GetSelected(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;
		CONFIG_PROPERTY_TYPE_CHECK(p, PropertyType::MULTISELECT, nullptr);

		return (CMultiSelect*)p->Value;
	}

	CTextInput* GetText(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return nullptr;
		CONFIG_PROPERTY_TYPE_CHECK(p, PropertyType::TEXTINPUT, nullptr);

		return (CTextInput*)p->Value;
	}

	void _KeyStateChanged(int index, bool currentlyPressed)
	{
		std::vector<void*>& Properties = Keybind::Binds[index];
		for (size_t i = 0; i < Properties.size(); i++)
		{
			Property* p = (Property*)Properties.at(i);
			switch (p->Type)
			{
			case PropertyType::BOOLEAN:
			{
				CBoolean* b = (CBoolean*)p->Value;
				switch (b->BindMode)
				{
				default:
				case KeybindMode::TOGGLE:
					if (currentlyPressed)
						b->Value.Invert();
					break;
				case KeybindMode::HOLDTOENABLE:
					if (!!b->Value.Get() != currentlyPressed)
						b->Value.Invert();
					break;
				case KeybindMode::HOLDTODISABLE:
					if (!!b->Value.Get() == currentlyPressed)
						b->Value.Invert();
					break;
				}
			} break;
			case PropertyType::HSTATEFUL:
			{
				if (currentlyPressed)
					((CHorizontalState*)p->Value)->Value.Increment();
			} break;
			case PropertyType::VSTATEFUL:
			{
				if (currentlyPressed)
					((CVerticalState*)p->Value)->Value.Increment();
			} break;
			default:
				L::Log((XOR("_KeyStateChanged - idk how to deal with bind on non-boolean property ") + p->Name).c_str());
				return;
			}
		}
	}

	void _BindToKey(Property* p, int index)
	{
		if (index < 0 || index >= Keybind::nKeys || Keybind::KeyMap[index] == VK_ESCAPE)
			index = p->Name == "show-menu" ? Keybind::ReverseKeyMap(VK_INSERT) : -1;

		bool ForceUpdate = index >= 0;

		switch (p->Type)
		{
		case PropertyType::BOOLEAN:
		{
			// unbind if already bound
			if (((CBoolean*)p->Value)->BoundToKey >= 0)
			{
				std::vector<void*>& vec = Keybind::Binds[((CBoolean*)p->Value)->BoundToKey];
				for (size_t i = 0; i < vec.size(); i++)
					if (vec.at(i) == (void*)p)
						vec.erase(vec.begin() + i--);
				((CBoolean*)p->Value)->BoundToKey = -1;
			}

			// bind if wants us to
			if (index >= 0)
			{
				((CBoolean*)p->Value)->BoundToKey = index;
				Keybind::Binds[index].push_back(p);
			}

			if (((CBoolean*)p->Value)->BindMode == KeybindMode::TOGGLE)
				ForceUpdate = false;
		} break;
		case PropertyType::VSTATEFUL:
		{
			// unbind if already bound
			if (((CVerticalState*)p->Value)->BoundToKey >= 0)
			{
				std::vector<void*>& vec = Keybind::Binds[((CBoolean*)p->Value)->BoundToKey];
				for (size_t i = 0; i < vec.size(); i++)
					if (vec.at(i) == (void*)p)
						vec.erase(vec.begin() + i--);
				((CVerticalState*)p->Value)->BoundToKey = -1;
			}

			// bind if wants us to
			if (index >= 0)
			{
				((CVerticalState*)p->Value)->BoundToKey = index;
				Keybind::Binds[index].push_back(p);
			}
			ForceUpdate = false;
		} break;
		case PropertyType::HSTATEFUL:
		{
			// unbind if already bound
			if (((CHorizontalState*)p->Value)->BoundToKey >= 0)
			{
				std::vector<void*>& vec = Keybind::Binds[((CBoolean*)p->Value)->BoundToKey];
				for (size_t i = 0; i < vec.size(); i++)
					if (vec.at(i) == (void*)p)
						vec.erase(vec.begin() + i--);
				((CHorizontalState*)p->Value)->BoundToKey = -1;
			}

			// bind if wants us to
			if (index >= 0)
			{
				((CHorizontalState*)p->Value)->BoundToKey = index;
				Keybind::Binds[index].push_back(p);
			}
			ForceUpdate = false;
		}
		default:
			L::Log((XOR("_BindToKey - idk how to deal with bind on non-boolean property ") + p->Name).c_str());
			return;
		}

		if (ForceUpdate)
			_KeyStateChanged(index, GetKeyState(Keybind::KeyMap[index]) < 0);
	}

	bool ExportSingleProperty(Property* p, char** buffer, size_t* bufferSpaceOccupied, size_t* bufferSpaceAllocated)
	{
		if (!p || !buffer || !bufferSpaceOccupied || !bufferSpaceAllocated)
		{
			L::Log(XOR("ExportSingleProperty failed - null parameter(s)"));
			return false;
		}
		L::Verbose(XOR("ExportSingleProperty running for p = "), "");
		L::Verbose(p->Name.c_str());

		size_t vacantSpaceInBuffer = *bufferSpaceAllocated - *bufferSpaceOccupied;
		size_t spaceRequired = sizeof(size_t);
		size_t valueLength = 0; // bytes

		// determine space_needed
		switch (p->Type)
		{
		case PropertyType::TEXTINPUT:
		{
			valueLength = ((CTextInput*)p->Value)->DataSize;
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::BOOLEAN:
		{
			valueLength = 1;
			if (((CBoolean*)p->Value)->Bindable)
				valueLength += sizeof(int) + 1;

			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::FLOAT:
		{
			valueLength = sizeof(float);
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::PAINTKIT:
		{
			valueLength = sizeof(int) * 3;
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::COLOR:
		{
			valueLength = sizeof(unsigned char) * 4;
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::HSTATEFUL:
		{
			valueLength = sizeof(int);
			if (((CHorizontalState*)p->Value)->Bindable)
				valueLength += sizeof(int);

			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::VSTATEFUL:
		{
			valueLength = sizeof(int);
			if (((CVerticalState*)p->Value)->Bindable)
				valueLength += sizeof(int);

			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::MULTISELECT:
		{
			valueLength = sizeof(uint64_t);
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		default:
			L::Log((XOR("ExportSingleProperty - Warning, idk how to export this property type: ") + std::to_string((int)p->Type)).c_str());
			return true;
		}

		// adjust capacity
		if (vacantSpaceInBuffer < spaceRequired)
		{
			char* re = (char*)realloc(*buffer, *bufferSpaceAllocated + (spaceRequired - vacantSpaceInBuffer));
			if (!re)
			{
				L::Log(XOR("ExportSingleProperty failed - could not expand buffer"));
				return false;
			}
			else
			{
				*buffer = re;
				*bufferSpaceAllocated += spaceRequired - vacantSpaceInBuffer;
				L::Verbose(XOR("ExportSingleProperty reallocated to @"), "");
				L::Verbose((std::to_string((DWORD)*buffer) + " and new capacity: " + std::to_string(*bufferSpaceAllocated)).c_str());
			}
		}

		// write to buffer
		{
			// value length
			memcpy(*buffer + *bufferSpaceOccupied, (void*)(&valueLength), sizeof(size_t));
			*bufferSpaceOccupied += sizeof(size_t);
			spaceRequired -= sizeof(size_t);

			// property name
			memcpy(*buffer + *bufferSpaceOccupied, (void*)(p->Name.c_str()), p->Name.length() + 1);
			*bufferSpaceOccupied += p->Name.length() + 1;
			spaceRequired -= p->Name.length() + 1;

			// value
			switch (p->Type)
			{
			case PropertyType::TEXTINPUT:
			{
				memcpy(*buffer + *bufferSpaceOccupied, ((CTextInput*)p->Value)->Data, ((CTextInput*)p->Value)->DataSize);
			} break;
			case PropertyType::BOOLEAN:
			{
				(*buffer)[*bufferSpaceOccupied] = ((CBoolean*)p->Value)->Value.Get() != 0 ? '\xFF' : '\x00';
				if (((CBoolean*)p->Value)->Bindable)
				{
					(*buffer)[*bufferSpaceOccupied + 1] = (char)((CBoolean*)p->Value)->BindMode;
					*(int*)(*buffer + *bufferSpaceOccupied + 2) = ((CBoolean*)p->Value)->BoundToKey >= 0 ? Keybind::KeyMap[((CBoolean*)p->Value)->BoundToKey] : -1;
				}
			} break;
			case PropertyType::FLOAT:
			{
				float v = ((CFloat*)p->Value)->Get();
				memcpy(*buffer + *bufferSpaceOccupied, (void*)&v, sizeof(float));
			} break;
			case PropertyType::PAINTKIT:
			{
				int* pos = (int*)(*buffer + *bufferSpaceOccupied);
				pos[0] = ((CPaintKit*)p->Value)->PaintKit->ID;
				pos[1] = ((CPaintKit*)p->Value)->Mode;
				pos[2] = ((CPaintKit*)p->Value)->Version;
			} break;
			case PropertyType::COLOR:
			{
				CColor* c = (CColor*)p->Value;
				(*buffer)[*bufferSpaceOccupied + 0] = c->GetR();
				(*buffer)[*bufferSpaceOccupied + 1] = c->GetG();
				(*buffer)[*bufferSpaceOccupied + 2] = c->GetB();
				(*buffer)[*bufferSpaceOccupied + 3] = c->GetA();
			} break;
			case PropertyType::HSTATEFUL:
			{
				char* pos = *buffer + *bufferSpaceOccupied;
				*(size_t*)pos = ((CHorizontalState*)p->Value)->Value.Get();
				if (((CHorizontalState*)p->Value)->Bindable)
				{
					*(int*)(pos + sizeof(size_t) + sizeof(char)) = ((CHorizontalState*)p->Value)->BoundToKey >= 0 ? Keybind::KeyMap[((CHorizontalState*)p->Value)->BoundToKey] : -1;
				}
			} break;
			case PropertyType::VSTATEFUL:
			{
				char* pos = *buffer + *bufferSpaceOccupied;
				*(size_t*)pos = ((CVerticalState*)p->Value)->Value.Get();
				if (((CVerticalState*)p->Value)->Bindable)
				{
					*(int*)(pos + sizeof(size_t) + sizeof(char)) = ((CVerticalState*)p->Value)->BoundToKey >= 0 ? Keybind::KeyMap[((CVerticalState*)p->Value)->BoundToKey] : -1;
				}
			} break;
			case PropertyType::MULTISELECT:
			{
				char* pos = *buffer + *bufferSpaceOccupied;
				*(uint64_t*)pos = ((CMultiSelect*)p->Value)->Mask;
			} break;
			}
			*bufferSpaceOccupied += spaceRequired;
		}

		L::Verbose("ExportSingleProperty success");
		return true;
	}

	bool ImportSingleProperty(const char* buffer, size_t bufferSize, size_t* nBytesUsed)
	{
		if (bufferSize < sizeof(size_t)) return false;
		
		// read value size
		size_t valueSize = *(size_t*)buffer;
		bufferSize -= sizeof(size_t);
		if (nBytesUsed) *nBytesUsed = sizeof(size_t);

		// read property name & get pointer
		const char* name = buffer + sizeof(size_t);
		size_t nameSize = 0;
		while (bufferSize > 0 && name[nameSize++] != '\0') bufferSize--;
		if (nBytesUsed) *nBytesUsed += nameSize;
		if (nameSize < 1) return false;
		Property* p = GetProperty(name);
		if (!p)
		{
			L::Log("Failed to load nonexistent property: ", "\"");
			L::Log(std::string(name, nameSize).c_str(), "\", ");
			L::Log("so i'm just gonna try and skip to the next one");
			if (nBytesUsed) *nBytesUsed += valueSize;
			return false;
		}

		// read value
		if (bufferSize < valueSize) return false;
		if (nBytesUsed) *nBytesUsed += valueSize;
		const char* value = buffer + sizeof(size_t) + nameSize;
		switch (p->Type)
		{
		case PropertyType::TEXTINPUT:
		{
			ZeroMemory(((CTextInput*)p->Value)->Data, ((CTextInput*)p->Value)->DataSize);
			memcpy(((CTextInput*)p->Value)->Data, value, min(((CTextInput*)p->Value)->DataSize, valueSize));
		} break;
		case PropertyType::BOOLEAN:
		{
			if (valueSize != 1 && valueSize != 1 + 1 + sizeof(int)) return false;

			((CBoolean*)p->Value)->Value.Set(value[0] == 0 ? false : true);
			if (valueSize > 1 && ((CBoolean*)p->Value)->Bindable)
			{
				switch (value[1])
				{
				case 0:
					((CBoolean*)p->Value)->BindMode = KeybindMode::TOGGLE;
					break;
				case 1:
					((CBoolean*)p->Value)->BindMode = KeybindMode::HOLDTOENABLE;
					break;
				case 2:
					((CBoolean*)p->Value)->BindMode = KeybindMode::HOLDTODISABLE;
					break;
				default:
					L::Verbose(XOR("ImportSingleProperty got a weird bind mode for boolean"));
					break; // just leave it at whatever it was
				}
				_BindToKey(p, Keybind::ReverseKeyMap(*(int*)(value + 2)));
			}
		} break;
		case PropertyType::FLOAT:
		{
			if (valueSize != sizeof(float)) return false;
			((CFloat*)p->Value)->Set(*(float*)(value));
		} break;
		case PropertyType::PAINTKIT:
		{
			if (valueSize != sizeof(int) * 3) return false;
			const Skins::PaintKit* pk = Skins::PaintKitFromID(((int*)value)[0]);
			if (!pk) return false;
			((CPaintKit*)p->Value)->PaintKit = pk;
			((CPaintKit*)p->Value)->Mode = ((int*)value)[1];
			((CPaintKit*)p->Value)->Version = ((int*)value)[2];
		} break;
		case PropertyType::COLOR:
		{
			if (valueSize != sizeof(unsigned char) * 4) return false;
			CColor* c = (CColor*)p->Value;
			c->SetR((unsigned char)value[0]);
			c->SetG((unsigned char)value[1]);
			c->SetB((unsigned char)value[2]);
			c->SetA((unsigned char)value[3]);
		} break;
		case PropertyType::HSTATEFUL:
		{
			if (valueSize != sizeof(size_t) && valueSize != sizeof(size_t) + sizeof(int)) return false;

			((CHorizontalState*)p->Value)->Value.Set(*(size_t*)(value + 0));
			if (valueSize > sizeof(size_t) && ((CHorizontalState*)p->Value)->Bindable)
				_BindToKey(p, Keybind::ReverseKeyMap(*(int*)(value + sizeof(size_t))));
		} break;
		case PropertyType::VSTATEFUL:
		{
			if (valueSize != sizeof(size_t) && valueSize != sizeof(size_t) + sizeof(int)) return false;

			((CVerticalState*)p->Value)->Value.Set(*(size_t*)(value + 0));
			if (valueSize > sizeof(size_t) && ((CVerticalState*)p->Value)->Bindable)
				_BindToKey(p, Keybind::ReverseKeyMap(*(int*)(value + sizeof(size_t))));
		} break;
		case PropertyType::MULTISELECT:
		{
			if (valueSize != sizeof(uint64_t)) return false;
			((CMultiSelect*)p->Value)->Mask = *(uint64_t*)value;
		} break;
		default:
			L::Log((XOR("ImportSingleProperty - Warning, idk how to import this property type: ") + std::to_string((int)p->Type)).c_str());
			return false;
		}
		return true;
	}

	char* ExportTheme(size_t* nBytesOut)
	{
		constexpr const char* header = "\x69\x04\x20PARTICLE.CHURCH/THEME";
		constexpr size_t headerSize = sizeof("\x69\x04\x20PARTICLE.CHURCH/THEME") - 1;

		size_t size = 0;
		size_t capacity = headerSize;
		char* buffer = (char*)malloc(headerSize);
		if (!buffer)
		{
			L::Log(XOR("Config2::ExportTheme failed - initial malloc failed"));
			return nullptr;
		}

		memcpy(buffer, header, headerSize);
		size += headerSize;

		static Tab* ThemeTab = nullptr;
		if (!ThemeTab)
		{
			for (size_t i = 0; i < Tabs.size(); i++)
			{
				if (Tabs.at(i)->Name == "Theme")
				{
					ThemeTab = Tabs.at(i);
					break;
				}
			}

			if (!ThemeTab)
			{
				L::Log("Config2::ExportTheme failed - couldn't find theme tab");
				free(buffer);
				return nullptr;
			}
		}

		for (size_t g = 0; g < ThemeTab->Groups.size(); g++)
		{
			Group* group = ThemeTab->Groups.at(g);
			for (size_t i = 0; i < group->Properties.size(); i++)
			{
				Property* p = group->Properties.at(i);
				if (!ExportSingleProperty(p, &buffer, &size, &capacity))
				{
					L::Log(XOR("Config2::ExportTheme failed - ExportSingleProperty failed"));
					free(buffer);
					return nullptr;
				}
			}
		}
		L::Log(XOR("Config2::ExportTheme success"));

		*nBytesOut = size;
		return buffer;
	}

	char* ExportConfig(size_t* nBytesOut)
	{
		constexpr const char* header = "\x69\x04\x20PARTICLE.CHURCH/CONFIG";
		constexpr size_t headerSize = sizeof("\x69\x04\x20PARTICLE.CHURCH/CONFIG") - 1;

		size_t size = 0;
		size_t capacity = headerSize;
		char* buffer = (char*)malloc(headerSize);
		if (!buffer)
		{
			L::Log(XOR("Config2::ExportConfig failed - initial malloc failed"));
			return nullptr;
		}

		memcpy(buffer, header, headerSize);
		size += headerSize;
		
		for (size_t t = 0; t < Tabs.size(); t++)
		{
			Tab* tab = Tabs.at(t);
			if (tab->Name == "Theme") continue;

			for (size_t g = 0; g < tab->Groups.size(); g++)
			{
				Group* group = tab->Groups.at(g);
				for (size_t i = 0; i < group->Properties.size(); i++)
				{
					Property* p = group->Properties.at(i);
					if (p->Name == "show-menu") continue;

					if (!ExportSingleProperty(p, &buffer, &size, &capacity))
					{
						L::Log(XOR("Config2::ExportConfig failed - ExportSingleProperty failed"));
						free(buffer);
						return nullptr;
					}
				}
			}
		}
		L::Log(XOR("Config2::ExportConfig success"));

		*nBytesOut = size;
		return buffer;
	}

	void ImportTheme(const char* buffer, size_t nBytes)
	{
		constexpr const char* header = "\x69\x04\x20PARTICLE.CHURCH/THEME";
		constexpr size_t headerSize = sizeof("\x69\x04\x20PARTICLE.CHURCH/THEME") - 1;

		L::Verbose("Loading theme with ", std::to_string(nBytes).c_str());
		L::Verbose(" bytes");
		if (nBytes < headerSize)
		{
			L::Log("Tried to load a theme that doesn't have enough bytes");
			return;
		}

		if (memcmp(header, buffer, headerSize))
		{
			L::Log("Tried to load a theme with invalid header");
			return;
		}

		size_t i = headerSize;
		while (i < nBytes)
		{
			size_t bruh = nBytes - i;
			if (!ImportSingleProperty(buffer + i, bruh, &bruh))
			{
				L::Log(XOR("Failed to import property... This theme is probably fucked0"));
			}
			i += bruh;
		}
	}

	void ImportConfig(const char* buffer, size_t nBytes)
	{
		constexpr const char* header = "\x69\x04\x20PARTICLE.CHURCH/CONFIG";
		constexpr size_t headerSize = sizeof("\x69\x04\x20PARTICLE.CHURCH/CONFIG") - 1;

		L::Verbose("Loading config with ", std::to_string(nBytes).c_str());
		L::Verbose(" bytes");
		if (nBytes < headerSize)
		{
			L::Log("Tried to load a config that doesn't have enough bytes");
			return;
		}

		if (memcmp(header, buffer, headerSize))
		{
			L::Log("Tried to load a config with invalid header");
			return;
		}

		size_t i = headerSize;
		while (i < nBytes)
		{
			size_t bruh = nBytes - i;
			if (!ImportSingleProperty(buffer + i, bruh, &bruh))
			{
				L::Log("Failed to import property... This config is probably fucked");
			}
			i += bruh;
		}
	}

	void _ResetCWD()
	{
		// not sure why I can't use _chdir
		_wchdir(std::wstring(G::CSGODirectory.begin(), G::CSGODirectory.end()).c_str());
	}

	DWORD WINAPI _PromptExportThemeFile(void* _)
	{
		L::Log("_PromptExportThemeFile executing...");

		// prompt user to save file
		char filename[MAX_PATH];
		{
			ZeroMemory(&filename, MAX_PATH);
			strcpy(filename, "autoload.pctheme");

			OPENFILENAME ofn{};
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Particle Theme File (.pctheme)\0*.pctheme\0All Files\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Export Theme";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

			L::Verbose("_PromptExportThemeFile initiating user input");
			bool UserTerminated = !GetSaveFileName(&ofn);
			_ResetCWD();
			if (UserTerminated || filename[0] == '\0')
			{
				L::Log("_PromptExportThemeFile failed - user terminated");
				return 1;
			}
			L::Verbose("_PromptExportThemeFile got filepath: ", "");
			L::Verbose(filename);
		}

		// open the file
		L::Verbose("_PromptExportThemeFile opening file");
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			L::Log("_PromptExportThemeFile failed - cannot open file");
			return 2;
		}

		// export the theme
		L::Verbose("_PromptExportThemeFile passing execution to Config2::ExportTheme()");
		size_t nBytesOut = 0;
		char* data = Config2::ExportTheme(&nBytesOut);
		if (!data || nBytesOut == 0)
		{
			L::Log("_PromptExportThemeFile failed - Config2::ExportTheme failed");
			file.close();
			return 3;
		}

		// write to file
		L::Verbose("_PromptExportThemeFile writing to file and closing");
		file.write(data, nBytesOut);
		file.close();
		return 0;
	}

	DWORD WINAPI _PromptImportThemeFile(void* _)
	{
		L::Log("_PromptImportThemeFile executing...");

		// prompt user to open file
		char filename[MAX_PATH];
		{
			ZeroMemory(&filename, MAX_PATH);

			OPENFILENAME ofn{};
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Particle Theme File (.pctheme)\0*.pctheme\0All Files\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Import Theme";
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER;

			L::Verbose("_PromptImportThemeFile initiating user input");
			bool UserTerminated = !GetOpenFileName(&ofn);
			_ResetCWD();
			if (UserTerminated || filename[0] == '\0')
			{
				L::Log("_PromptImportThemeFile failed - user terminated");
				return 1;
			}
			L::Verbose("_PromptImportThemeFile got filepath: ", "");
			L::Verbose(filename);
		}
		
		// open the file
		L::Verbose("_PromptImportThemeFile opening file");
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			L::Log("_PromptImportThemeFile failed - cannot open file");
			return 2;
		}

		// calculate its length
		file.seekg(0, std::ios::end);
		std::streampos bytes = file.tellg();
		if (bytes > 100000)
		{
			L::Log("_PromptImportThemeFile failed - file too big: ", "");
			L::Log(std::to_string(bytes).c_str(), " bytes\n");
			return 3;
		}
		file.seekg(0);
		file.clear();
		L::Verbose("_PromptImportThemeFile got file length: ", "");
		L::Verbose(std::to_string(bytes).c_str());

		// read it into memory
		char* buffer = (char*)malloc(bytes);
		if (!buffer)
		{
			L::Log("_PromptImportThemeFile failed - couldn't malloc file space");
			return 4;
		}
		L::Verbose("_PromptImportThemeFile reading file into memory & closing file handle");
		if (!file.read(buffer, bytes))
		{
			L::Log("_PromptImportThemeFile failed - couldn't read file data");
			file.close();
			free(buffer);
			return 5;
		}
		file.close();

		L::Verbose("_PromptImportThemeFile passing execution to Config2::ImportTheme()");
		Config2::ImportTheme(buffer, bytes);
		free(buffer);
		return 0;
	}

	DWORD WINAPI _PromptExportConfigFile(void* _)
	{
		L::Log("_PromptExportConfigFile executing...");

		// prompt user to save file
		char filename[MAX_PATH];
		{
			ZeroMemory(&filename, MAX_PATH);
			strcpy(filename, "autoload.pccfg");

			OPENFILENAME ofn{};
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Particle Config File (.pccfg)\0*.pccfg\0All Files\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Export Config";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

			L::Verbose("_PromptExportConfigFile initiating user input");
			bool UserTerminated = !GetSaveFileName(&ofn);
			_ResetCWD();
			if (UserTerminated || filename[0] == '\0')
			{
				L::Log("_PromptExportConfigFile failed - user terminated");
				return 1;
			}
			L::Verbose("_PromptExportConfigFile got filepath: ", "");
			L::Verbose(filename);
		}

		// open the file
		L::Verbose("_PromptExportConfigFile opening file");
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			L::Log("_PromptExportConfigFile failed - cannot open file");
			return 2;
		}

		// export the theme
		L::Verbose("_PromptExportConfigFile passing execution to Config2::ExportConfig()");
		size_t nBytesOut = 0;
		char* data = Config2::ExportConfig(&nBytesOut);
		if (!data || nBytesOut == 0)
		{
			L::Log("_PromptExportConfigFile failed - Config2::ExportConfig failed");
			file.close();
			return 3;
		}

		// write to file
		L::Verbose("_PromptExportConfigFile writing to file and closing");
		file.write(data, nBytesOut);
		file.close();
		return 0;
	}

	DWORD WINAPI _PromptImportConfigFile(void* _)
	{
		L::Log("_PromptImportConfigFile executing...");

		// prompt user to open file
		char filename[MAX_PATH];
		{
			ZeroMemory(&filename, MAX_PATH);

			OPENFILENAME ofn{};
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Particle Config File (.pccfg)\0*.pccfg\0All Files\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Import Config";
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER;

			L::Verbose("_PromptImportConfigFile initiating user input");
			bool UserTerminated = !GetOpenFileName(&ofn);
			_ResetCWD();
			if (UserTerminated || filename[0] == '\0')
			{
				L::Log("_PromptImportConfigFile failed - user terminated");
				return 1;
			}
			L::Verbose("_PromptImportConfigFile got filepath: ", "");
			L::Verbose(filename);
		}

		// open the file
		L::Verbose("_PromptImportConfigFile opening file");
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			L::Log("_PromptImportConfigFile failed - cannot open file");
			return 2;
		}

		// calculate its length
		file.seekg(0, std::ios::end);
		std::streampos bytes = file.tellg();
		if (bytes > 100000)
		{
			L::Log("_PromptImportConfigFile failed - file too big: ", "");
			L::Log(std::to_string(bytes).c_str(), " bytes\n");
			return 3;
		}
		file.seekg(0);
		file.clear();
		L::Verbose("_PromptImportConfigFile got file length: ", "");
		L::Verbose(std::to_string(bytes).c_str());

		// read it into memory
		char* buffer = (char*)malloc(bytes);
		if (!buffer)
		{
			L::Log("_PromptImportConfigFile failed - couldn't malloc file space");
			return 4;
		}
		L::Verbose("_PromptImportConfigFile reading file into memory & closing file handle");
		if (!file.read(buffer, bytes))
		{
			L::Log("_PromptImportConfigFile failed - couldn't read file data");
			file.close();
			free(buffer);
			return 5;
		}
		file.close();

		L::Verbose("_PromptImportConfigFile passing execution to Config2::ImportConfig()");
		Config2::ImportConfig(buffer, bytes);
		free(buffer);
		return 0;
	}

	void PromptImportThemeFile()
	{
		HANDLE t = CreateThread(NULL, 0, _PromptImportThemeFile, NULL, NULL, NULL);
		if (t) CloseHandle(t);
	}

	void PromptExportThemeFile()
	{
		HANDLE t = CreateThread(NULL, 0, _PromptExportThemeFile, NULL, NULL, NULL);
		if (t) CloseHandle(t);
	}

	void PromptImportConfigFile()
	{
		HANDLE t = CreateThread(NULL, 0, _PromptImportConfigFile, NULL, NULL, NULL);
		if (t) CloseHandle(t);
	}

	void PromptExportConfigFile()
	{
		HANDLE t = CreateThread(NULL, 0, _PromptExportConfigFile, NULL, NULL, NULL);
		if (t) CloseHandle(t);
	}

	void ProcessKeys()
	{
		// im gonna rely on the compiler to make this more efficient
		// because i do not want to sacrifice readability
		for (size_t key = 0; key < Keybind::nKeys; key++)
		{
			int VK = Keybind::KeyMap[key];
			bool LastWasPressed = Keybind::KeyState[key];
			bool CurrentPressed = GetAsyncKeyState(VK) < 0;
			Keybind::KeyState[key] = CurrentPressed;

			if (!UserData::Initialized)
				continue;

			bool IsMouse = VK == VK_LBUTTON;
			if (IsMouse && GUI2::WantMouse)
			{
				continue;
			}
			else if (LastWasPressed != CurrentPressed)
			{
				if (SettingKeybindFor && CurrentPressed)
				{
					_BindToKey(SettingKeybindFor, key);
					SettingKeybindFor = nullptr;
				}
				else
				{
					_KeyStateChanged(key, CurrentPressed);
				}
			}
		}
	}

	void Free()
	{
		for (auto x = PropertyTable.begin(); x != PropertyTable.end(); x++)
		{
			delete x->second;
		}
		for (size_t i = 0; i < Tabs.size(); i++)
		{
			delete Tabs[i];
		}
	}
};

namespace UserData
{
	TIME_POINT LastSuccessfulServerContact = std::chrono::steady_clock::time_point(std::chrono::seconds(0));
	TIME_POINT LastServerContactAttempt = std::chrono::steady_clock::time_point(std::chrono::seconds(0));

	bool Initialized = false;
	bool Authenticated = false;
	std::string Email = "";
	std::string SessionID = "";
	uint64_t UserID = (uint64_t)-1;
	bool Premium = false;

	bool BusyAttemptingLogin = false;
	size_t LoginAttemptCounter = 0;
	size_t PingAttemptCounter = 0;
	std::string LoginError = "";
	TIME_POINT LoginErrorOriginTime = std::chrono::steady_clock::time_point(std::chrono::seconds(0));
	
	std::string CredentialsFile = "";
	DWORD WINAPI AttemptLogin(LPVOID pInfo)
	{
		size_t myAttemptId = ++LoginAttemptCounter;
		BusyAttemptingLogin = true;

		LoginInformation* info = (LoginInformation*)pInfo;
		std::string InputEmail(info->Email.c_str());
		std::string InputPassword(info->Password.c_str());
		delete info;

		// create outgoing JSON string
		JSONValue* EmailJSON = new JSONValue(std::wstring(InputEmail.begin(), InputEmail.end()));
		JSONValue* PasswordJSON = new JSONValue(std::wstring(InputPassword.begin(), InputPassword.end()));
		JSONObject InputRoot;
		InputRoot[L"email"] = EmailJSON;
		InputRoot[L"password"] = PasswordJSON;
		JSONValue* InputJSONV = new JSONValue(InputRoot);
		std::wstring InputW = InputJSONV->Stringify();
		std::string OutgoingJSONString(InputW.begin(), InputW.end());
		delete InputJSONV;

		// send API request
		DWORD bytesRead = 0;
		HTTP::contentType = "application/json";
		byte* result = HTTP::Post("https://www.a4g4.com/API/dll/login.php", OutgoingJSONString, &bytesRead);
		std::string ServerResponseJSONString = std::string((char*)result, bytesRead);
		free(result);
		L::Log(ServerResponseJSONString.c_str());
		/* Example output:
		{
			"session": "559a56ff2bcf669b9413d543ba49d583e04d1c508c8bdc1433f426d626a6556e",
			"serverTime": 1614573349,
			"success": true,
			"id": 4,
			"accountAge": 93536,
			"email": "dev@a4g4.com",
			"subscription": {
				"exists": true,
				"id": "sub_J24l9bQSc0wCMh",
				"status": 1,
				"active": true,
				"timeRemainging": 2667348,
				"grace": false,
				"nextPaymentDate": "April 1st, 2021, at 1:31 am",
				"nextPaymentTimestamp": 1617240697
			}
		}
		*/

		// parse output
		JSONValue* ServerResponse = JSON::Parse(std::wstring(ServerResponseJSONString.begin(), ServerResponseJSONString.end()).c_str());

		// make sure that this thread is still the most recent handler available
		if (myAttemptId != LoginAttemptCounter)
		{
			if (ServerResponse) delete ServerResponse;
			return 1;
		}

		// json invalid
		if (!ServerResponse || !ServerResponse->IsObject())
		{
			UserData::LoginError = "Invalid server response - please contact a developer.";
			UserData::LoginErrorOriginTime = Animation::now();
			L::Log("Failed to parse JSON response for login attempt");
			if (ServerResponse) delete ServerResponse;
			Sleep(1000);
			BusyAttemptingLogin = false;
			return 1;
		}
		JSONObject root = ServerResponse->AsObject();

		// check success
		{
			auto success = root.find(L"success");
			if (success == root.end() || !success->second->IsBool() || !success->second->AsBool())
			{
				auto err = root.find(L"error");
				if (err != root.end() && err->second->IsString())
				{
					UserData::LoginError = std::string(err->second->AsString().begin(), err->second->AsString().end());
					L::Log("Login attempt failed w/ err:", "");
					L::Log(UserData::LoginError.c_str());
				}
				else
				{
					UserData::LoginError = "Unknown error - Try again";
					L::Log("Login attempt failed w/o error description");
				}
				UserData::LoginErrorOriginTime = Animation::now();
				if (ServerResponse) delete ServerResponse;
				Sleep(1000);
				BusyAttemptingLogin = false;
				return 1;
			}
		}

		if (myAttemptId != LoginAttemptCounter)
		{
			if (ServerResponse) delete ServerResponse;
			return 1;
		}

		// get user information
		{
			auto id = root.find(L"id");
			auto email = root.find(L"email");
			auto session = root.find(L"session");
			auto subscription = root.find(L"subscription");

			if (id == root.end() || !id->second->IsNumber()) goto VALUE_MISSING;
			if (email == root.end() || !email->second->IsString()) goto VALUE_MISSING;
			if (session == root.end() || !session->second->IsString()) goto VALUE_MISSING;
			if (subscription == root.end() || !subscription->second->IsObject()) goto VALUE_MISSING;

			auto subscriptionRoot = subscription->second->AsObject();
			auto subscriptionExists = subscriptionRoot.find(L"exists");
			auto premium = subscriptionRoot.find(L"active");
			if (subscriptionExists == subscriptionRoot.end() || !subscriptionExists->second->IsBool()) goto VALUE_MISSING;

			if (subscriptionExists->second->AsBool())
			{
				if (premium == subscriptionRoot.end() || !premium->second->IsBool()) goto VALUE_MISSING;
			}

			// set these values in the config
			LastSuccessfulServerContact = Animation::now();
			UserData::Initialized = true;
			UserData::Authenticated = true;
			UserData::Premium = subscriptionExists->second->AsBool() && premium->second->AsBool();
			UserData::Email = std::string(email->second->AsString().begin(), email->second->AsString().end());
			UserData::SessionID = std::string(session->second->AsString().begin(), session->second->AsString().end());
			UserData::UserID = (uint64_t)id->second->AsNumber();
			GUI2::IntroAnimation2 = Animation::newAnimation("intro-2", 0);
			if (ServerResponse) delete ServerResponse;
			BusyAttemptingLogin = false;

			// save this login to file
			try
			{
				L::Log("Saving credentials to file");
				auto f = std::ofstream(UserData::CredentialsFile, std::ios::binary);
				if (f.is_open()) // if fails, whatever, they'll have to type password again
				{
					size_t usedSize = 0;
					char data[1025]; // 1 for xor key, 512 for email, 512 for password
					ZeroMemory(data, 1025);

					// xor key
					char k = (char)(rand() % 256);
					data[0] = k;
					usedSize = 1;

					// email
					size_t EmailLen = InputEmail.length();
					memcpy(data + usedSize, InputEmail.c_str(), EmailLen);
					usedSize += EmailLen;
					if (EmailLen < 512)
						usedSize += 1; // add a null terminator

					// pass
					size_t PassLen = InputPassword.length();
					memcpy(data + usedSize, InputPassword.c_str(), PassLen);
					usedSize += PassLen;
					if (PassLen < 512)
						usedSize += 1; // add a null terminator

					// "encrypt"
					for (size_t i = 1; i < usedSize; i++)
						data[i] ^= k;

					f.write(data, usedSize);
					f.close();
					L::Log("Success!");
				}
			}
			catch (const std::exception& e)
			{
				L::Log("Failed to save user credentials w/ error: ", "");
				L::Log(e.what());
			}
		}

		return 0;

	VALUE_MISSING:
		UserData::LoginError = "Invalid server response - please contact a developer.";
		UserData::LoginErrorOriginTime = Animation::now();
		if (ServerResponse) delete ServerResponse;
		Sleep(1000);
		BusyAttemptingLogin = false;
		return 1;
	}
	
	DWORD WINAPI PingServer(LPVOID pInfo)
	{
		size_t AttemptIndex = ++PingAttemptCounter;

		// create outgoing JSON string
		JSONValue* SessionJSON = new JSONValue(std::wstring(UserData::SessionID.begin(), UserData::SessionID.end()));
		JSONObject InputRoot;
		InputRoot[L"session"] = SessionJSON;
		JSONValue* InputJSONV = new JSONValue(InputRoot);
		std::wstring InputW = InputJSONV->Stringify();
		std::string OutgoingJSONString(InputW.begin(), InputW.end());
		delete InputJSONV;

		// send API request
		DWORD bytesRead = 0;
		HTTP::contentType = "application/json";
		byte* result = HTTP::Post("https://www.a4g4.com/API/dll/ping.php", OutgoingJSONString, &bytesRead);
		std::string ServerResponseJSONString = std::string((char*)result, bytesRead);
		free(result);
		L::Log(ServerResponseJSONString.c_str());
		/* Example output:
		{
			"session": "559a56ff2bcf669b9413d543ba49d583e04d1c508c8bdc1433f426d626a6556e",
			"serverTime": 1614573349,
			"success": true,
			"id": 4,
			"accountAge": 93536,
			"email": "dev@a4g4.com",
			"subscription": {
				"exists": true,
				"id": "sub_J24l9bQSc0wCMh",
				"status": 1,
				"active": true,
				"timeRemainging": 2667348,
				"grace": false,
				"nextPaymentDate": "April 1st, 2021, at 1:31 am",
				"nextPaymentTimestamp": 1617240697
			}
		}
		*/

		// parse output
		JSONValue* ServerResponse = JSON::Parse(std::wstring(ServerResponseJSONString.begin(), ServerResponseJSONString.end()).c_str());

		// make sure that this thread is still the most recent handler available
		if (AttemptIndex != PingAttemptCounter)
		{
			if (ServerResponse) delete ServerResponse;
			return 1;
		}

		// json invalid
		if (!ServerResponse || !ServerResponse->IsObject())
		{
			L::Log("Failed to parse JSON response for ping attempt");
			if (ServerResponse) delete ServerResponse;
			return 1;
		}
		JSONObject root = ServerResponse->AsObject();

		// check success
		{
			auto success = root.find(L"success");
			if (success == root.end() || !success->second->IsBool() || !success->second->AsBool())
			{
				auto err = root.find(L"error");
				if (err != root.end() && err->second->IsString())
				{
					L::Log("Ping attempt failed w/ err:", "");
					L::Log(std::string(err->second->AsString().begin(), err->second->AsString().end()).c_str());
				}
				else
				{
					L::Log("Ping attempt failed w/o error description");
				}
				if (ServerResponse) delete ServerResponse;
				return 1;
			}
		}

		if (AttemptIndex != PingAttemptCounter)
		{
			if (ServerResponse) delete ServerResponse;
			return 1;
		}

		// get user information
		{
			auto subscription = root.find(L"subscription");
			if (subscription == root.end() || !subscription->second->IsObject()) goto VALUE_MISSING;

			auto subscriptionRoot = subscription->second->AsObject();
			auto subscriptionExists = subscriptionRoot.find(L"exists");
			auto premium = subscriptionRoot.find(L"active");
			if (subscriptionExists == subscriptionRoot.end() || !subscriptionExists->second->IsBool()) goto VALUE_MISSING;

			if (subscriptionExists->second->AsBool())
			{
				if (premium == subscriptionRoot.end() || !premium->second->IsBool()) goto VALUE_MISSING;
			}

			// set these values in the config
			UserData::Premium = subscriptionExists->second->AsBool() && premium->second->AsBool();
		}

		LastSuccessfulServerContact = Animation::now();
		if (ServerResponse) delete ServerResponse;
		return 0;

	VALUE_MISSING:
		if (ServerResponse) delete ServerResponse;
		return 1;
	}
}