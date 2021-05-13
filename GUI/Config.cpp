#include "../Include.hpp"
#include "../json.hpp"
#include "HTTP.hpp"
#include <sstream>
#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Stomach", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"
#define CHAM_MATERIALS_CONFIG "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent", "Glow"

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

namespace Config
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

		// OFFENCE
		{
			Tab* t = new Tab("Offence");
			t->TopPadding = -10;
			Group* OffenceMeta = t->Add("__META__");
			Property* OffenceMode = OffenceMeta->Add("offence-mode", "", new CHorizontalState({ "Legit", "Rage" }, false));

			// LEGIT
			{
				Group* g = t->Add("legit-Aimbot");
				p = g->Add("legit-aim-enable", "Enable", new CBoolean());
				g->BeginMaster(p);

				g->Add("legitaim-weapontype", "", new CHorizontalState({ "Pistol", "SMG", "Heavy", "Rifle", "Sniper" }));
				size_t index = 0;
				for (std::string wtype : {"pistol", "smg", "heavy", "rifle", "sniper"})
				{
					p = g->Add("legitaim-" + wtype + "-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
					g->Add("legitaim-" + wtype + "-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }))->VisibilityLinked = p;
					g->Add("legitaim-" + wtype + "-smoothing", "Smoothing Method", new CVerticalState({ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" }))->VisibilityLinked = p;
					g->Add("legitaim-" + wtype + "-smoothing-amount", "Smoothing Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
					CONFIG_VIS(p, nullptr, GetState("legitaim-weapontype"), index++);

					GetProperty("legitaim-" + wtype + "-smoothing")->IsPremium = true;
				}
				g->EndMaster();
			}
			{
				Group* g = t->Add("legit-Backtrack");
				g->Add("legitaim-backtrack-time", "Backtrack TIme", new CFloat(0, 200, 1, "MS"));
			}

			// RAGE
			{
				Group* g = t->Add("rage-Aimbot");
				g->BeginMaster(g->Add("rage-aim-enable", "Enable", new CBoolean()));

				g->Add("rage-aim-silent", "Silent Aim", new CBoolean());
				g->Add("rage-aim-autoshoot", "Auto Shoot", new CBoolean());
				g->Add("rage-aim-autoscope", "Auto Scope", new CBoolean());

				g->Add("rageaim-weapontype", "", new CHorizontalState({ "Pistol", "SMG", "Heavy", "Rifle", "Scout", "AWP", "Auto" }));
				size_t index = 0;
				for (std::string wtype : {"pistol", "smg", "heavy", "rifle", "scout", "awp", "auto"})
				{
					p = g->Add("rageaim-" + wtype + "-hitbox", "Select Hitbox Scan", new CMultiSelect({ HITBOXES_CONFIG }));
					g->Add("rageaim-" + wtype + "-mindamage-visible", "Visible Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-mindamage-hidden", "Hidden Min Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-hitchance", "Min Hitchance", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-multipoint", "Multipoint Scale", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-baimiflethal", "Baim if Lethal", new CBoolean())->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-override", "Min Damage Override", new CBoolean())->VisibilityLinked = p;
					g->Add("rageaim-" + wtype + "-override-damage", "Override Damage", new CFloat(0, 100, 0, "HP"))->VisibilityLinked = p;
					CONFIG_VIS(p, nullptr, GetState("rageaim-weapontype"), index++);

					GetProperty("rageaim-" + wtype + "-baimiflethal")->IsPremium = true;
					GetProperty("rageaim-" + wtype + "-override")->IsPremium = true;
					GetProperty("rageaim-" + wtype + "-override-damage")->IsPremium = true;
				}
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
				Group* g = t->Add("Legit Anti-Aim");
				g->Add("antiaim-legit-enable", "Enable", new CBoolean())->IsPremium = true;

				g->BeginMaster(p);
				p = g->Add("antiaim-legit-max-angle", "Desync Amount", new CFloat(0, 100, 1, "%"));
				g->Add("antiaim-legit-invert", "AA Direction", new CVerticalState({ "Left", "Right" }, true))->IsPremium = true;
				g->EndMaster();

				p->IsPremium = true;
				p->GetWarning = []() {
					static auto fakelagtick = GetProperty("antiaim-fakelag-tick");
					static auto fakelagdistance = GetProperty("antiaim-fakelag-distance");
					static auto me = GetProperty("antiaim-legit-max-angle");
					if (((CFloat*)me->Value)->Get() > 0.f && ((CFloat*)fakelagtick->Value)->Get() == 0.f || ((CFloat*)fakelagdistance->Value)->Get() == 0.f)
						return std::string("Cannot desync without fake lag.");
					else
						return std::string("");
				};
			}
			{
				Group* g = t->Add("Rage Anti-Aim");
				p = g->Add("antiaim-rage-enable", "Enable", new CBoolean());

				g->BeginMaster(p);

				g->Add("antiaim-rage-invert", "Invert Fake/Real", new CBoolean());
				g->Add("visuals-rage-yawbase", "Base Yaw", new CVerticalState({ "180 from Original", "Closest to Crosshair", "Clostest Distance", "Freestanding" }));
				g->Add("antiaim-rage-pitch", "Pitch", new CFloat(-89, 89, 1, "DEG"));

				g->Add("antiaim-type", "", new CHorizontalState({ "Manual", "Custom" }));

				p = g->Add("antiaim-manual-left", "Left", new CBoolean());
				g->Add("antiaim-manual-back", "Back", new CBoolean())->VisibilityLinked = p;
				g->Add("antiaim-manual-right", "Right", new CBoolean())->VisibilityLinked = p;
				g->Add("antiaim-manual-max", "Desync Amount", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				CONFIG_VIS(p, nullptr, GetState("antiaim-type"), 0);

				p = g->Add("antiaim-custom-real", "Real Offset", new CFloat(-180, 180, 1, "DEG"));
				g->Add("antiaim-custom-real-jitter", "Real Jitter", new CFloat(-180, 180, 1, "DEG"))->VisibilityLinked = p;
				g->Add("antiaim-custom-fake", "Fake Offset", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("antiaim-custom-fake-jitter", "Fake Jitter", new CFloat(0, 100, 1, "%"))->VisibilityLinked = p;
				g->Add("antiaim-custom-fake-invert", "Invert on Hit", new CBoolean())->VisibilityLinked = p;
				CONFIG_VIS(p, nullptr, GetState("antiaim-type"), 1);

				g->EndMaster();

				GetProperty("antiaim-manual-max")->GetWarning = []() {
					static auto fakelagtick = GetFloat("antiaim-fakelag-tick");
					static auto fakelagdistance = GetFloat("antiaim-fakelag-distance");
					static auto me = GetFloat("antiaim-manual-max");
					if ((me->Get() > 0.f) && (fakelagtick->Get() == 0.f || fakelagdistance->Get() == 0.f))
						return std::string("Cannot desync without fake lag.");
					else
						return std::string("");
				};
				GetProperty("antiaim-custom-fake")->GetWarning = []() {
					static auto fakelagtick = GetFloat("antiaim-fakelag-tick");
					static auto fakelagdistance = GetFloat("antiaim-fakelag-distance");
					static auto me = GetFloat("antiaim-custom-fake");
					if ((me->Get() > 0.f) && (fakelagtick->Get() == 0.f || fakelagdistance->Get() == 0.f))
						return std::string("Cannot desync without fake lag.");
					else
						return std::string("");
				};
				GetProperty("antiaim-custom-fake-jitter")->GetWarning = []() {
					static auto fakelagtick = GetFloat("antiaim-fakelag-tick");
					static auto fakelagdistance = GetFloat("antiaim-fakelag-distance");
					static auto me = GetFloat("antiaim-custom-fake-jitter");
					if ((me->Get() > 0.f) && (fakelagtick->Get() == 0.f || fakelagdistance->Get() == 0.f))
						return std::string("Cannot desync without fake lag.");
					else
						return std::string("");
				};
			}
		}

		// VISUALS
		{
			Tab* t = new Tab("Visuals");
			{
				Group* g = t->Add("Chams");

				// sliders at the top
				g->Add("chams-mode", "", new CHorizontalState({ "Enemies", "Friends", "Local" }));
				CONFIG_VIS(g->Add("chams-enemy-visibility", "", new CHorizontalState({ "Visible", "Hidden" })), nullptr, GetState("chams-mode"), 0);
				CONFIG_VIS(g->Add("chams-friend-visibility", "", new CHorizontalState({ "Visible", "Hidden" })), nullptr, GetState("chams-mode"), 1);
				CONFIG_VIS(g->Add("chams-local-visibility", "", new CHorizontalState({ "Fake", "Real" })), nullptr, GetState("chams-mode"), 2);

				// cham options
				for (std::string mode : {"enemy", "friend", "local"})
				{
					for (int vis = 0; vis < 2; vis++)
					{
						std::string vstr = mode == "local" ? (vis == 0 ? "fake" : "real") : (vis == 0 ? "visible" : "hidden");

						p = g->Add("visuals-chams-" + mode + "-" + vstr + "-enable", "Enable", new CBoolean());
						CONFIG_VIS(p, GetProperty("chams-" + mode + "-visibility"), GetState("chams-" + mode + "-visibility"), vis);

						g->BeginMaster(p);
						g->Add("visuals-chams-" + mode + "-" + vstr + "-color", "Color", new CColor(true))->VisibilityLinked = p;
						g->Add("visuals-chams-" + mode + "-" + vstr + "-material", "Material", new CVerticalState({ CHAM_MATERIALS_CONFIG }))->VisibilityLinked = p;
						g->EndMaster();

						GetProperty("visuals-chams-" + mode + "-" + vstr + "-material")->IsPremium = true;
					}
				}
			}
			{
				Group* g = t->Add("ESP");

				// slider at top
				g->Add("esp-mode", "", new CHorizontalState({ "Enemies", "Friends" }));

				// enemies
				{
					p = g->Add("visuals-esp-enemy-enable", "Enable", new CBoolean());
					CONFIG_VIS(p, nullptr, GetState("esp-mode"), 0);

					g->BeginMaster(p);

					g->Add("visuals-esp-enemy-bbox", "Bounding Box", new CBoolean())->VisibilityLinked = p;
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

					GetProperty("visuals-esp-enemy-resolverflags")->IsPremium = true;
					GetProperty("visuals-esp-enemy-weapon")->IsPremium = true;
				}

				// friends
				{
					p = g->Add("visuals-esp-friend-enable", "Enable", new CBoolean());
					CONFIG_VIS(p, nullptr, GetState("esp-mode"), 1);

					g->BeginMaster(p);

					g->Add("visuals-esp-friend-bbox", "Bounding Box", new CBoolean())->VisibilityLinked = p;
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
				g->Add("visuals-world-skybox-enable", "Skybox Affected", new CBoolean())->IsPremium = true;

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
				p = g->Add("visuals-misc-thirdperson", "Thirdperson", new CBoolean());
				g->Add("visuals-misc-thirdperson-distance", "Thirdperson Distance", new CFloat(0, 150, 1, ""))->Master = p;

				g->Add("visuals-misc-revealranks", "Reveal Ranks", new CBoolean())->IsPremium = true;
				g->Add("visuals-misc-grenadeprediction", "Grenade Prediction", new CBoolean());
				g->Add("visuals-misc-noscope", "No Scope", new CBoolean())->IsPremium = true;;
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
		if(true){
			Tab* t = new Tab("Skinchanger");

			// knives
			{
				Group* g = t->Add("Knives");
				(p = g->Add("skinchanger-knife-enable", "Enable Knife Changer", new CBoolean(false)))->IsPremium = true;

				g->BeginMaster(p);
				g->Add("skinchanger-knife-model", "Knife Model", new CVerticalState(Skins::KnifeNames, false));
				g->Add("skinchanger-knife-wear", "Skin Wear", new CFloat(0, 1, 4));
				g->Add("skinchanger-knife-nametag", "Nametag", new CTextInput(32));
				g->Add("skinchanger-knife-stattrak", "StatTrak", new CBoolean(false));
				g->Add("skinchanger-knife-stattrak-count", "StatTrak Kills", new CTextInput(6)); // 999999
				g->Add("skinchanger-knife-paintkit", "", new CPaintKit());
				g->EndMaster();

				strcpy(GetText("skinchanger-knife-stattrak-count")->Data, "69420");
			}

			// weapons
			{ 
				Group* g = t->Add("Guns");

				for (int i = 0; i < (int)Skins::Weapon::_COUNT; i++)
					WeaponPaintKits.push_back((CPaintKit*)g->Add("skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[i])), Skins::WeaponNames[i], new CPaintKit())->Value);
			}

			// gloves
			{
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
				(p = g->Add("misc-movement-autostrafe", "Autostrafe", new CVerticalState({ "None", "Rage", "Legit" })))->IsPremium = true;
				p->Master = GetProperty("misc-movement-bhop");
				//g->Add("misc-movement-bhop-chance", "Bunnyhop Chance", new CFloat(0, 100, 1, "%"));
				g->Add("misc-movement-autostop", "AutoStop", new CBoolean());
				g->Add("misc-movement-slowwalk", "Slow Walk", new CBoolean());
				CONFIG_VIS(g->Add("misc-movement-slowwalk-speed", "Slow-Walk Speed", new CFloat(0, 100, 1, "%")), nullptr, GetState("misc-movement-slowwalk"), 1);
				g->Add("misc-movement-fastcrouch", "Fast Crouch", new CBoolean());
				g->Add("misc-movement-fakeduck", "Fake Duck", new CBoolean());
				g->Add("misc-movement-leg", "Leg Slide", new CBoolean())->IsPremium = true;
				CONFIG_VIS(g->Add("misc-movement-leg-time", "Leg Slide Time", new CFloat(0, 2000, 1, "MS")), nullptr, GetState("misc-movement-leg"), 1);
			}
			{
				Group* g = t->Add("Other");
				g->Add("show-menu", "Menu Toggle", new CBoolean(true));
				g->Add("misc-other-logs", "Shot Logs", new CBoolean())->IsPremium = true;
				g->Add("misc-other-autoaccept", "AutoAccept", new CBoolean());
				(p = g->Add("misc-other-killsay", "KillSay", new CBoolean()))->IsPremium = true;
				g->Add("misc-other-killsay-input", "KillSay Text", new CTextInput())->Master = p;
				(p = g->Add("misc-other-clantag", "Clantag Changer", new CBoolean()))->IsPremium = true;
				g->Add("misc-other-clantag-input", "Clantag Text", new CTextInput(16))->Master = p;
				//TODO - add dropdown to indicate what kind of clan tag animation they are using

				GetProperty("misc-other-clantag-input")->IsPremium = true;
				GetProperty("misc-other-killsay-input")->IsPremium = true;
			}
		}

		// Experimental Features
		{
			Tab* t = new Tab("Experimental");
			{
				Group* g = t->Add("Tickbase Manipulation");
				g->Add("tickbase-shift", "Shift Amount", new CFloat(0, 55, 0, "TICKS"))->IsPremium = true;
				g->Add("tickbase-recharge", "Recharge Amount", new CFloat(0, 64, 0, "TICKS"))->IsPremium = true;
			}
			// Need to add buttons to run functions (maybe new CFunc(<insert address of func>)
			// for reseting and iterating resolver
			{
				Group* g = t->Add("Cheat Specific");
				g->Add("clantag-special", "Special Clantag", new CBoolean())->IsPremium = true;
				g->Add("resolver", "Resolver", new CBoolean())->IsPremium = true;
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
				Group* g = t->Add("Switches, Sliders, and Text");

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
				Group* g = t->Add("Slide-Selects and Dropdowns");

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
				Group* g = t->Add("Color Pickers");

				g->Add("theme-color-editor-background", "Background", new CColor(false));
				g->Add("theme-color-editor-text", "Text", new CColor(true));
				g->Add("theme-color-editor-border-size", "Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-color-editor-border", "Outline", new CColor(true));
			}

			{
				Group* g = t->Add("Legit/Rage Switch");

				g->Add("theme-legit-rage-switch-text", "Text", new CColor(true));
				g->Add("theme-legit-rage-switch-background", "Background", new CColor(true));
				g->Add("theme-legit-rage-switch-highlight", "Highlight", new CColor(true));
				g->Add("theme-legit-rage-switch-border-size", "Outline Thickness", new CFloat(0.f, 3.f, 0, "PX"));
				g->Add("theme-legit-rage-switch-outline", "Outline", new CColor(true));
			}

			{
				Group* g = t->Add("Misc");

				g->Add("theme-offscreen", "Allow Menu Off Screen", new CBoolean(false));

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

		GetState("antiaim-manual-left")->OnChange = []() {
			static auto me = GetState("antiaim-manual-left");
			static auto o1 = GetState("antiaim-manual-back");
			static auto o2 = GetState("antiaim-manual-right");

			if (me->Get())
			{
				o1->Set(false);
				o2->Set(false);
			}
			else if (!o1->Get() && !o2->Get())
			{
				me->Set(true);
			}
		};
		GetState("antiaim-manual-back")->OnChange = []() {
			static auto me = GetState("antiaim-manual-back");
			static auto o1 = GetState("antiaim-manual-left");
			static auto o2 = GetState("antiaim-manual-right");

			if (me->Get())
			{
				o1->Set(false);
				o2->Set(false);
			}
			else if (!o1->Get() && !o2->Get())
			{
				me->Set(true);
			}
		};
		GetState("antiaim-manual-right")->OnChange = []() {
			static auto me = GetState("antiaim-manual-right");
			static auto o1 = GetState("antiaim-manual-back");
			static auto o2 = GetState("antiaim-manual-left");

			if (me->Get())
			{
				o1->Set(false);
				o2->Set(false);
			}
			else if (!o1->Get() && !o2->Get())
			{
				me->Set(true);
			}
		};

		GetState("legit-aim-enable")->OnChange = []() {
			static auto me = GetState("legit-aim-enable");
			static auto o = GetState("rage-aim-enable");

			if (me->Get())
			{
				o->Set(false);
			}
		};
		GetState("rage-aim-enable")->OnChange = []() {
			static auto me = GetState("rage-aim-enable");
			static auto o = GetState("legit-aim-enable");

			if (me->Get())
			{
				o->Set(false);
			}
		};

		GetState("antiaim-legit-enable")->OnChange = []() {
			static auto me = GetState("antiaim-legit-enable");
			static auto o = GetState("antiaim-rage-enable");

			if (me->Get())
			{
				o->Set(false);
			}
		};
		GetState("antiaim-rage-enable")->OnChange = []() {
			static auto me = GetState("antiaim-rage-enable");
			static auto o = GetState("antiaim-legit-enable");

			if (me->Get())
			{
				o->Set(false);
			}
		};

		ImportTheme(ConfigConstants::ThemeDark, ConfigConstants::ThemeDarkSize);
		ImportConfig(ConfigConstants::ConfigOff, ConfigConstants::ConfigOffSize, false);
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

	void _KeyStateChanged(int index, bool currentlyPressed, bool checkPremium)
	{
		std::vector<void*>& Properties = Keybind::Binds[index];
		for (size_t i = 0; i < Properties.size(); i++)
		{
			Property* p = (Property*)Properties.at(i);

			if (checkPremium && p->IsPremium && !UserData::Premium)
				continue;

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
				L::Log((XOR("_KeyStateChanged - idk how to deal with bind on property ") + p->Name).c_str());
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
				std::vector<void*>& vec = Keybind::Binds[((CVerticalState*)p->Value)->BoundToKey];
				for (int64_t i = vec.size() - 1; i >= 0; i--)
					if (vec.at(i) == (void*)p)
						vec.erase(vec.begin() + i);
			}
			((CVerticalState*)p->Value)->BoundToKey = index;

			if (index >= 0)
				Keybind::Binds[index].push_back(p);

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
		} break;
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
			char* re = nullptr;
			try {
				re = (char*)realloc(*buffer, *bufferSpaceAllocated + (spaceRequired - vacantSpaceInBuffer));
			}
			catch (std::exception& e) {
				L::Log(e.what());
			}
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

	bool ImportSingleProperty(const char* buffer, size_t bufferSize, size_t* nBytesUsed, bool checkPremium)
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

		if (checkPremium && p->IsPremium && !UserData::Premium)
		{
			return true;
		}

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
			L::Log(XOR("Config::ExportTheme failed - initial malloc failed"));
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
				L::Log("Config::ExportTheme failed - couldn't find theme tab");
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
					L::Log(XOR("Config::ExportTheme failed - ExportSingleProperty failed"));
					free(buffer);
					return nullptr;
				}
			}
		}
		L::Log(XOR("Config::ExportTheme success"));

		*nBytesOut = size;
		return buffer;
	}

	char* ExportConfig(size_t* nBytesOut)
	{
		constexpr const char* header = "\x69\x04\x20PARTICLE.CHURCH/CONFIG";
		constexpr size_t headerSize = sizeof("\x69\x04\x20PARTICLE.CHURCH/CONFIG") - 1;

		size_t size = 0;
		size_t capacity = headerSize + 4096;
		char* buffer = (char*)malloc(capacity);
		if (!buffer)
		{
			L::Log(XOR("Config::ExportConfig failed - initial malloc failed"));
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
						L::Log(XOR("Config::ExportConfig failed - ExportSingleProperty failed"));
						free(buffer);
						return nullptr;
					}
				}
			}
		}
		L::Log(XOR("Config::ExportConfig success"));

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

	void ImportConfig(const char* buffer, size_t nBytes, bool checkPremium)
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
			if (!ImportSingleProperty(buffer + i, bruh, &bruh, checkPremium))
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
		L::Verbose("_PromptExportThemeFile passing execution to Config::ExportTheme()");
		size_t nBytesOut = 0;
		char* data = Config::ExportTheme(&nBytesOut);
		if (!data || nBytesOut == 0)
		{
			L::Log("_PromptExportThemeFile failed - Config::ExportTheme failed");
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

		L::Verbose("_PromptImportThemeFile passing execution to Config::ImportTheme()");
		Config::ImportTheme(buffer, bytes);
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
		L::Verbose("_PromptExportConfigFile passing execution to Config::ExportConfig()");
		size_t nBytesOut = 0;
		char* data = Config::ExportConfig(&nBytesOut);
		if (!data || nBytesOut == 0)
		{
			L::Log("_PromptExportConfigFile failed - Config::ExportConfig failed");
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

		L::Verbose("_PromptImportConfigFile passing execution to Config::ImportConfig()");
		Config::ImportConfig(buffer, bytes);
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
		for (size_t key = 0; key < Keybind::nKeys; key++)
		{
			int VK = Keybind::KeyMap[key];
			bool LastWasPressed = Keybind::KeyState[key];
			bool CurrentPressed = GetAsyncKeyState(VK) < 0;
			Keybind::KeyState[key] = CurrentPressed;

			if (UserData::SessionId == "")
				continue;

			bool IsMouse = VK == VK_LBUTTON;
			if (IsMouse && ImGui::IsAnyItemHovered() || ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup))
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
	std::string SessionId = "";
	bool Authenticated = false;
	bool Premium = false;
	uint32_t PremiumTimeRemaining = 0;
	uint32_t UserId = 0;

	bool LoginDebounce = false;
	std::string LoginError = "";
	TIME_POINT LoginErrorTime = TIME_POINT(std::chrono::seconds(0));
	std::string CredentialsFile = "";

	void SetLoginError(std::string err)
	{
		LoginError = err;
		LoginErrorTime = Animation::now();
	}

	bool PingDebounce = false;
	TIME_POINT LastPingTime = TIME_POINT(std::chrono::seconds(0));

	DWORD WINAPI AttemptLogin(LPVOID pInfo)
	{
		if (LoginDebounce) return 1;
		LoginDebounce = true;
		SetLoginError("");

		// get info from form
		LoginInformation* info = (LoginInformation*)pInfo;
		std::string inputEmail(info->Email.c_str());
		std::string inputPassword(info->Password.c_str());
		delete info;

		// dump to json
		nlohmann::json out = nlohmann::json::object();
		out["email"] = inputEmail;
		out["password"] = inputPassword;

		// call api
		HTTP::contentType = "application/json";
		DWORD bytesRead = 0;
		char* response = (char*)HTTP::Post("https://www.a4g4.com/API/new/login.php", out.dump(), &bytesRead);

		// validate response
		if (!response || bytesRead < 7) // {"x":1}
		{
			SetLoginError("Failed to contact server - Please check your firewall.");
			goto failed;
		}

		// parse response
		try {
			nlohmann::json parsed = nlohmann::json::parse(std::string(response, bytesRead));
			free(response); response = nullptr;

			if (!parsed["success"].get<bool>())
			{
				SetLoginError(parsed["error"].get<std::string>());
				goto failed;
			}
			else
			{
				SessionId = parsed["idSessions"].get<std::string>();
				Authenticated = true;
				UserId = parsed["idUsers"].get<uint32_t>();
				PremiumTimeRemaining = parsed["premiumTimeRemaining"].get<uint32_t>();
				Premium = PremiumTimeRemaining > 0;
				LastPingTime = Animation::now();
			}
		}
		catch (std::exception& e)
		{
			SetLoginError("Unknown Error - Try Again"); // invalid server response
			L::Log(e.what());
			goto failed;
		}

		// save login info to file
		try
		{
			L::Log("Saving credentials to file...", " ");
			auto f = std::ofstream(CredentialsFile, std::ios::binary);
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
				size_t EmailLen = inputEmail.length();
				memcpy(data + usedSize, inputEmail.c_str(), EmailLen);
				usedSize += EmailLen;
				if (EmailLen < 512)
					usedSize += 1; // add a null terminator

				// pass
				size_t PassLen = inputPassword.length();
				memcpy(data + usedSize, inputPassword.c_str(), PassLen);
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

		//succeeded:
		if (response) { free(response); response = nullptr; }
		LoginDebounce = false;
		return 0;

	failed:
		if (response) { free(response); response = nullptr; }
		Sleep(1000);
		LoginDebounce = false;
		return 1;
	}

	DWORD WINAPI GetUnauthenticatedSession(LPVOID pInfo)
	{
		if (LoginDebounce) return 1;
		LoginDebounce = true;
		SetLoginError("");

		// call api
		HTTP::contentType = "application/json";
		DWORD bytesRead = 0;
		char* response = (char*)HTTP::Post("https://www.a4g4.com/API/new/playfree.php", "{}", &bytesRead);

		// validate response
		if (!response || bytesRead < 7) // {"x":1}
		{
			SetLoginError("Failed to contact server - Please check your firewall.");
			goto failed;
		}

		// parse response
		try {
			nlohmann::json parsed = nlohmann::json::parse(std::string(response, bytesRead));
			free(response); response = nullptr;
			SessionId = parsed["idSessions"].get<std::string>();
			Authenticated = false;
			UserId = (uint32_t)-1;
			PremiumTimeRemaining = 0;
			Premium = PremiumTimeRemaining > 0;
			LastPingTime = Animation::now();
			Config::GetState("misc-other-killsay")->Set(true);
			Config::GetState("misc-other-clantag")->Set(true);
			for (std::string wtype : {"pistol", "smg", "heavy", "rifle", "sniper"})
				Config::GetState("legitaim-" + wtype + "-smoothing")->Set(3);
		}
		catch (std::exception& e)
		{
			SetLoginError("Unknown Error - Try Again"); // invalid server response
			L::Log(e.what());
			goto failed;
		}

		//succeeded:
		if (response) { free(response); response = nullptr; }
		LoginDebounce = false;
		return 0;

	failed:
		if (response) { free(response); response = nullptr; }
		Sleep(1000);
		LoginDebounce = false;
		return 1;
	}

	DWORD WINAPI PingServer(LPVOID pInfo)
	{
		if (PingDebounce) return false;
		PingDebounce = true;

		L::Log("Ping!");
		// dump to json
		nlohmann::json out = nlohmann::json::object();
		out["sid"] = UserData::SessionId;

		// call api
		HTTP::contentType = "application/json";
		DWORD bytesRead = 0;
		char* response = (char*)HTTP::Post("https://www.a4g4.com/API/new/ping.php", out.dump(), &bytesRead);
		if (response)
		{
			L::Log(std::string(response, bytesRead).c_str());
		}
		// validate response
		if (!response || bytesRead < 7) // {"x":1}
		{
			L::Log("Ping failed due to invalid server response");
			goto failed;
		}

		// parse response
		try {
			nlohmann::json parsed = nlohmann::json::parse(std::string(response, bytesRead));
			free(response); response = nullptr;

			if (!parsed["success"].get<bool>())
			{
				L::Log(("Ping failed w/ err: " + parsed["error"].get<std::string>()).c_str());
				goto failed;
			}
			else
			{
				PremiumTimeRemaining = parsed["premiumTimeRemaining"].get<uint32_t>();
				Premium = PremiumTimeRemaining > 0;
				LastPingTime = Animation::now();
			}
		}
		catch (std::exception& e)
		{
			L::Log("Ping failed w/ error: ", "");
			L::Log(e.what());
			goto failed;
		}

	failed:
		Sleep(1000); // prevent from trying for at least 10 seconds
		PingDebounce = false;
		return 0;
	}

	bool ConnectAPI()
	{
		DWORD bytes = 0;
		char* response = nullptr;

		// for some reason my brain isn't working right now
		// this is the best way I can think of doing this
		goto first;
	retry:
		if (response) free(response);
		Sleep(1000);

	first:
		response = (char*)HTTP::Post("https://www.a4g4.com/API/new/injected.php", "", &bytes);
		if (bytes == 0 || !response) goto retry;
		for (size_t i = 0; i < bytes; i++)
			if (response[i] < '0' || '9' < response[i])
				goto retry;

		return true;
	}
}