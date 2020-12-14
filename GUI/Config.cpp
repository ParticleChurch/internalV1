#include "../Include.hpp"

namespace Config {
	std::string StringifyKeybindType(KeybindType type)
	{
		return
			type == KeybindType::Toggle ? "Toggle" :
			type == KeybindType::HoldToEnable ? "Hold To Enable" :
			"Hold To Disable";
	}
	std::string StringifyVK(int VirturalKey)
	{
		// TODO: make our own func, not valves retarded "CAPSLOCK" and "q"
		return I::inputsystem->VirtualKeyToString(VirturalKey);
	}

	UserInfoT UserInfo = UserInfoT{};
	std::vector<Tab*> Tabs = {};
	std::map<std::string, Property*> PropertyLookup;

	void Init()
	{
		/* OFFENSE TAB */
		{
			Tab* t = new Tab("Offense");
			{
				Widget* w = t->AddWidget("Aimbot");

				

				w->AddProperty(false, 0, "enable-aimbot", "Enable", true, true, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "enable-silentaim", "Silent Aim", true, true);
				w->AddProperty(false, 0, "aimbot-autoshoot", "Autoshoot", true, true);
				w->AddProperty(false, 1, "aimbot-autowall", "Autowall", true, true);
				w->AddProperty(false, 1, "aimbot-aimstep", "Aimstep", "degrees", 0, 180, 0, 0, 0);
				w->AddSeparator();

#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"

				CEditGroup* AimGunGroup = w->AddEditGroup("aimbot-");

				AimGunGroup->AddGroup("pistol-", "Pistol");
				w->AddProperty(false, 0, "aimbot-pistol-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-pistol-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-pistol-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-pistol-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				/*
				Here's an example of how you could read this value:

				{
					uint16_t selections = Config::GetSelections("test-multi-select");
					std::cout << "You have options 0x" << std::hex << selections << std::dec << " = {";
					for (int optionIndex = 0; optionIndex < 5; optionIndex++)
					{
						if (selections & (1 << optionIndex)) // <===== this is the actual test
						{
							std::cout << optionIndex << ", ";
						}
					}
					std::cout << "} selected!" << std::endl;
				}

				*/
				w->AddProperty(false, 0, "aimbot-pistol-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-pistol-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-pistol-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-pistol-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-pistol-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("smg-", "SMG");
				w->AddProperty(false, 0, "aimbot-smg-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-smg-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-smg-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-smg-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-smg-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-smg-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-smg-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-smg-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-smg-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("heavy-", "Heavy");
				w->AddProperty(false, 0, "aimbot-heavy-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-heavy-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-heavy-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-heavy-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-heavy-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-heavy-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-heavy-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-heavy-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-heavy-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("rifle-", "Rifle");
				w->AddProperty(false, 0, "aimbot-rifle-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-rifle-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-rifle-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-rifle-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-rifle-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-rifle-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-rifle-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-rifle-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-rifle-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("scout-", "Scout");
				w->AddProperty(false, 0, "aimbot-scout-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-scout-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-scout-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-scout-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-scout-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-scout-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-scout-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-scout-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-scout-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("awp-", "AWP");
				w->AddProperty(false, 0, "aimbot-awp-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-awp-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-awp-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-awp-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-awp-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-awp-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-awp-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-awp-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-awp-if-lethal", "Baim If Lethal", true, true);

				AimGunGroup->AddGroup("auto-", "Auto");
				w->AddProperty(false, 0, "aimbot-auto-smoothing-method", "Smoothing Method", CDropdown{ "None", "EaseCubic", "EaseQuart", "Linear" });
				w->AddProperty(false, 1, "aimbot-auto-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-auto-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "aimbot-auto-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "aimbot-auto-hitbox-priority", "Hitbox Priority", CDropdown{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "aimbot-auto-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-auto-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-auto-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "aimbot-auto-if-lethal", "Baim If Lethal", true, true);

#undef HITBOXES_CONFIG

			}
			{
				Widget* w = t->AddWidget("Triggerbot");

				w->AddProperty(false, 0, "enable-triggerbot", "Enable", true, true, KeybindOptions(true, true, true));
				w->AddProperty(false, 1, "triggerbot-through-wall", "Through Wall", true, true);
				w->AddProperty(false, 0, "triggerbot-delay", "Delay", "MS", 0, 1000, 0, 0, 0);
				w->AddProperty(false, 2, "triggerbot-magnet", "Magnet", true, true);
			}
			{
				Widget* w = t->AddWidget("Other");
				w->AddProperty(false, 1, "backtracking-time", "Backtracking Time", "MS", 0, 200, 0, 0, 0);
			}
			Tabs.push_back(t);
		}

		/* DEFENSE TAB */
		{
			Tab* t = new Tab("Defense");
			{
				Widget* w = t->AddWidget("Fake Lag");

				w->AddProperty(false, 0, "antiaim-fakelag-tick", "Amount", "ticks", 0, 16, 0, 0, 0);
				w->AddProperty(false, 0, "antiaim-fakelag-distance", "Distance", "units", 0, 64, 0, 0, 0); //4096				
			}
			{
				Widget* w = t->AddWidget("Legit");

				w->AddProperty(false, 1, "antiaim-legit-enable", "Enable", true, true);
				w->AddProperty(false, 1, "antiaim-legit-max-angle", "Max Desync Angle", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "antiaim-legit-invert", "Invert AA", new CInverter("Left", "Right"));
			}
			{
				Widget* w = t->AddWidget("Rage");
				//General
				w->AddProperty(false, 0, "antiaim-rage-enable", "Enable", true, true);

				//Pitch
				w->AddProperty(false, 0, "antiaim-rage-pitch", "Pitch", CDropdown{ "Up", "Down", "Trolling" });

				//Yaw - Real
				w->AddProperty(false, 2, "antiaim-rage-real", "Real Offset", "DEG", -180, 180, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-real-jitter", "Real Jitter Offset", "DEG", 0, 180, 0, 0, 0);
				//TODO - add 3 keybinds for manual antiaim (left, right, and back) //we need just keybinds lol

				//Yaw - Fake
				w->AddProperty(false, 2, "antiaim-rage-fake", "Fake Offset", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-fake-jitter", "Fake Jitter Offset", "%", 0, 100, 1, 100, 100);
				w->AddProperty(false, 1, "antiaim-rage-invert", "Invert AA", new CInverter("", ""));
				w->AddProperty(false, 2, "antiaim-rage-fake-invert", "Invert on Hit", false, false);
			}
			{
				Widget* w = t->AddWidget("Visuals");

				w->AddText("Fake"); w->BeginIndent();
				w->AddProperty(false, 0, "antiaim-visual-fake", "Visualize", false, false);
				w->AddProperty(false, 2, "antiaim-visual-fake-lag", "Visualize Lag", false, false);
				w->AddProperty(false, 1, "antiaim-visual-fake-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 1, "antiaim-visual-fake-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent(); w->AddSeparator();

				w->AddText("Real"); w->BeginIndent();
				w->AddProperty(false, 1, "antiaim-visual-real", "Visualize", false, false);
				w->AddProperty(false, 1, "antiaim-visual-real-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 1, "antiaim-visual-real-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();
			}
			Tabs.push_back(t);
		}

		/* VISUALS TAB */
		{
			Tab* t = new Tab("Visuals");
			{
				Widget* w = t->AddWidget("Chams");

				w->AddText("Enemies"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-cham-enemy-visible-enable", "Visible Chams", false, false);
				w->AddProperty(false, 0, "visuals-cham-enemy-visible-color", "Visible Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-cham-enemy-visible-material", "Visible Material", CDropdown{ "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent" });
				w->AddProperty(false, 0, "visuals-cham-enemy-hidden-enable", "Hidden Chams", false, false);
				w->AddProperty(false, 0, "visuals-cham-enemy-hidden-color", "Hidden Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-cham-enemy-hidden-material", "Hidden Material", CDropdown{ "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent" });
				w->EndIndent();

				w->AddSeparator();

				w->AddText("Teammates"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-cham-friend-visible-enable", "Visible Chams", false, false);
				w->AddProperty(false, 0, "visuals-cham-friend-visible-color", "Visible Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-cham-friend-visible-material", "Visible Material", CDropdown{ "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent" });
				w->AddProperty(false, 0, "visuals-cham-friend-hidden-enable", "Hidden Chams", false, false);
				w->AddProperty(false, 0, "visuals-cham-friend-hidden-color", "Hidden Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-cham-friend-hidden-material", "Hidden Material", CDropdown{ "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent" });
				w->EndIndent();
			}
			{
				Widget* w = t->AddWidget("ESP");

				w->AddProperty(false, 1, "visuals-esp-other-grenadebox", "Grenade Box", false, false);
				w->AddProperty(false, 2, "visuals-esp-other-weapon-box", "Weapon Box", false, false);
				w->AddProperty(false, 2, "visuals-esp-other-weapon-name", "Weapon Names", false, false);
				
				CEditGroup* ESPGroup = w->AddEditGroup("visuals-esp-");

				ESPGroup->AddGroup("enemy-", "Enemies");
				w->AddProperty(false, 0, "visuals-esp-enemy-enable", "Enable", false, false);
				w->AddProperty(false, 0, "visuals-esp-enemy-bbox", "Bounding Box", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-enemy-bbox-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-enemy-name", "Name", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-enemy-name-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-enemy-snapline", "Snaplines", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-enemy-snapline-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-enemy-skeleton", "Skeleton", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-enemy-skeleton-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-enemy-health", "Health", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-health-color", "Foreground", new Color(0, 255, 0)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-health-color-background", "Background", new Color(255, 0, 0)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-enemy-ammo ", "Ammo", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-ammo-color", "Foreground", new Color(0, 0, 255)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-ammo-color-background ", "Background", new Color(100, 100, 100)); w->EndIndent();

				ESPGroup->AddGroup("friend-", "Friends");
				w->AddProperty(false, 0, "visuals-esp-friend-enable", "Enable", false, false);
				w->AddProperty(false, 0, "visuals-esp-friend-bbox", "Bounding Box", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-friend-bbox-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-friend-name", "Name", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-friend-name-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-friend-snapline", "Snaplines", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-friend-snapline-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-friend-skeleton", "Skeleton", false, false);
				w->BeginIndent(); w->AddProperty(false, 1, "visuals-esp-friend-skeleton-color", "Color", new Color(255, 255, 255)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-friend-health", "Health", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-health-color", "Foreground", new Color(0, 255, 0)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-health-color-background", "Background", new Color(255, 0, 0)); w->EndIndent();
				w->AddProperty(false, 0, "visuals-esp-friend-ammo ", "Ammo", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-ammo-color", "Foreground", new Color(0, 0, 255)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-ammo-color-background ", "Background", new Color(100, 100, 100)); w->EndIndent();
			}
			{
				Widget* w = t->AddWidget("World");
				//World
				w->AddProperty(false, 1, "visuals-world-enable", "World Changes", false, false);
				w->AddProperty(false, 2, "visuals-world-color", "World", new Color(0, 150, 255));
				w->AddProperty(false, 2, "visuals-world-opacity", "World Opacity", "%", 0, 100, 0, 0, 0);
				//Prop
				w->AddProperty(false, 1, "visuals-world-prop-enable", "Prop Changes", false, false);
				w->AddProperty(false, 2, "visuals-world-prop-color", "Prop", new Color(0, 150, 255));
				w->AddProperty(false, 2, "visuals-world-prop-opacity", "Prop Opacity", "%", 0, 100, 0, 0, 0);
				//Skybox
				w->AddProperty(false, 1, "visuals-world-skybox-enable", "Skybox Changes", false, false);
				//Skybox dropdown from https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Sky_List
				w->AddProperty(false, 0, "visuals-world-skybox-name", "Load Skybox", CDropdown{ "cs_baggage_skybox_", "cs_tibet", "vietnam", "sky_lunacy", "embassy", "italy", "jungle", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_day02_05", "nukeblank", "dustblank", "sky_venice", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02", "sky_csgo_night02b", "vertigo", "vertigoblue_hdr", "sky_dust" });
				w->AddProperty(false, 2, "visuals-world-skybox-color", "Skybox", new Color(0, 150, 255));

			}
			{
				Widget* w = t->AddWidget("Misc");
				w->AddProperty(false, 0, "visuals-misc-thirdperson", "Thirdperson", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 2, "visuals-misc-vote-indicator", "Vote Indicator", false, false);
				w->AddProperty(false, 1, "visuals-misc-spec-list", "Spectator List", false, false);
				w->AddProperty(false, 1, "visuals-misc-teamdamage-list", "Team-damage List", false, false);
			}
			Tabs.push_back(t);
		}

		/* MISC TAB */
		{
			Tab* t = new Tab("Misc");
			{
				Widget* w = t->AddWidget("Movement");
				w->AddProperty(false, 0, "misc-movement-bhop", "Bunnyhop", false, false);
				w->AddProperty(false, 1, "misc-movement-slowwalk", "Slow Walk", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 2, "misc-movement-slowwalk-speed", "Slow-Walk Speed", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 2, "misc-movement-airstuck", "Airstuck", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 1, "misc-movement-fastcrouch", "Fast Crouch", false, false);
				w->AddProperty(false, 2, "misc-movement-fakeduck", "Fake Duck", false, false, KeybindOptions(true, true, true));
				//TODO - add autostrafe dropdown (with option for none to be on, with one on at max) (or just add a bool idk yet lol)
			}
			{
				Widget* w = t->AddWidget("Other");
				w->AddProperty(false, 0, "misc-other-autoaccept", "AutoAccept", false, false);
				w->AddProperty(false, 2, "misc-other-killsay", "Kill Say", false, false);
				//TODO - add input area for custom killsay insult
				w->AddProperty(false, 2, "misc-other-fullautopistol", "Full Auto Pistol", false, false); //aka autopistol
				w->AddProperty(false, 2, "misc-other-fakeunbox", "Fake Unbox", false, false);
				//TODO - add area to input what they are unboxing
				w->AddProperty(false, 0, "misc-other-clantag", "Clantag", false, false);
				//TODO - add area to input what clantag they are displaying
				//TODO - add dropdown to indicate what kind of clan tag animation they are using
			}
			Tabs.push_back(t);
		}

		/* MENU TAB */
		{
			Tab* t = new Tab("Menu");
			{
				Widget* w = t->AddWidget("Menu");

				w->AddProperty(false, 0, "show-menu", "Show Menu", true, true, KeybindOptions(true, false, false));
				w->AddProperty(false, 0, "menu-complexity", "Menu Complexity", CDropdown{ "Beginner", "Intermediate", "Professional" }, false, 2, 2);
				w->AddProperty(false, 0, "show-help-link", "Show Help Link", true, true);
				w->AddProperty(false, 0, "show-watermark", "Watermark", true, true);
			}
			{
				Widget* w = t->AddWidget("Colors");

				w->AddProperty(false, 0, "menu-text-color", "Text Color", new Color(255, 255, 255));
				w->AddProperty(false, 0, "menu-eject-color", "Eject Color", new Color(200, 75, 75));

				w->AddSeparator();
				w->AddProperty(false, 0, "menu-background-color1", "Background Color 1", new Color(30, 30, 30));
				w->AddProperty(false, 0, "menu-background-color2", "Background Color 2", new Color(20, 20, 20));
				w->AddProperty(false, 0, "menu-background-color3", "Background Color 3", new Color(50, 50, 50));
				w->AddProperty(false, 0, "menu-background-color4", "Background Color 4", new Color(75, 75, 75));

				w->AddSeparator();
				w->AddProperty(false, 0, "menu-option-selector-background-color", "Option Selector Bg", new Color(20, 20, 20));
				w->AddProperty(false, 0, "menu-option-selector-active-color", "Option Selector Active", new Color(60, 60, 60));
				w->AddProperty(false, 0, "menu-option-selector-text-color", "Option Selector Text", new Color(255, 255, 255));
				w->AddProperty(false, 0, "menu-option-color1", "Option Color 1", new Color(230, 230, 230));
				w->AddProperty(false, 0, "menu-option-color2", "Option Color 2", new Color(0, 150, 255));
			}
			{
				Widget* w = t->AddWidget("Opacities");

				w->AddText("Coming soon in V2.0!!!");
				//w->AddProperty(false, 0, "menu-opacity", "Opacity", "%", 15, 100, 1, 100, 100);
			}
			Tabs.push_back(t);
		}

		/* CONFIG TAB */
		{
			Tab* t = new Tab("Config");
			{
				Widget* w = t->AddWidget("bruh");
				w->AddText("[insert config options here]");
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
		Bind(PropertyLookup.at("show-menu"), VK_INSERT);
		PropertyLookup.at("show-menu")->BindType = KeybindType::Toggle;

		// assume all keys are not pressed
		for (int i = 0; i < 256; i++)
		{
			KeyReleased(i);
		}
	}

	bool GetBool(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				std::cout << "Config::GetBool: nonexistant property: \"" << Name << "\"" << std::endl;
			return false;
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::BOOLEAN)
				return *(bool*)prop->Value;
			else
			{
				if (CONFIG_DEBUG)
					std::cout << "Config::GetBool: non boolean value: \"" << Name << "\"" << std::endl;
				return false;
			}
		}
	}
	float GetFloat(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				std::cout << "Config::GetFloat: nonexistant property: \"" << Name << "\"" << std::endl;
			return 0.f;
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::FLOAT)
				return ((CFloat*)prop->Value)->get();
			else
			{
				if (CONFIG_DEBUG)
					std::cout << "Config::GetFloat: non float value: \"" << Name << "\"" << std::endl;
				return 0.f;
			}
		}
	}
	Color GetColor(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				std::cout << "Config::GetColor: nonexistant property: \"" << Name << "\"" << std::endl;
			return Color(0, 0, 0);
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::COLOR)
				return *(Color*)prop->Value;
			else
			{
				if (CONFIG_DEBUG)
					std::cout << "Config::GetColor: non color value: \"" << Name << "\"" << std::endl;
				return Color(0, 0, 0);
			}
		}
	}
	size_t GetState(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				std::cout << "Config::GetState: nonexistant property: \"" << Name << "\"" << std::endl;
			return 0;
		}
		else
		{
			Property* prop = search->second;
			switch (prop->Type)
			{
			case PropertyType::DROPDOWN:
				return ((CDropdown*)prop->Value)->GetSelection();
			case PropertyType::INVERTER:
				return (size_t)(((CInverter*)prop->Value)->State);
			case PropertyType::EDITGROUP:
				return ((CEditGroup*)prop->Value)->SelectedGroup;
			default:
			{
				if (CONFIG_DEBUG)
					std::cout << "Config::GetState: non stateful value: \"" << Name << "\"" << std::endl;
				return 0;
			}
			}
		}
	}

	extern uint16_t GetSelections(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				std::cout << "Config::GetSelections: nonexistant property: \"" << Name << "\"" << std::endl;
			return 0;
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::MULTISELECT)
				return ((CMultiSelector*)prop->Value)->Get();
			else
			{
				if (CONFIG_DEBUG)
					std::cout << "Config::GetSelections: non multiselector value: \"" << Name << "\"" << std::endl;
				return 0;
			}
		}
	}

	void SetBool(std::string Name, bool Value)
	{
		Property* prop = PropertyLookup.at(Name);
		if (!prop) return;
		*(bool*)prop->Value = Value;
	}

	/* Keybind Stuff */
	std::map<int, std::vector<Property*>*> KeybindMap;
	void Bind(Property* Prop, WPARAM KeyCode)
	{
		if (KeyCode == 0) return Unbind(Prop); // binding a key to 0 is the same as unbinding
		if (Prop->KeyBind != 0) Unbind(Prop, true); // this key is already bound, first unbind it before preceeding
		if (KeyCode == VK_ESCAPE) return Unbind(Prop); // binding to escape just unbinds
			

		auto KeyIterator = KeybindMap.find(KeyCode);
		std::vector<Property*>* PropertyList = nullptr;
		if (KeyIterator == KeybindMap.end())
			KeybindMap.insert(std::pair<int, std::vector<Property*>*>(KeyCode, PropertyList = new std::vector<Property*>));
		else
			PropertyList = KeyIterator->second;

		Prop->KeyBind = KeyCode;
		PropertyList->push_back(Prop);
	};
	void Unbind(Property* Prop, bool __FORCE /* SEE DECLARATION B4 SETTING TRUE */)
	{
		auto KeyIterator = KeybindMap.find(Prop->KeyBind);
		if (KeyIterator == KeybindMap.end())
		{
			// this property isn't actually bound
			Prop->KeyBind = 0;
			if (!__FORCE && Prop->Name == "show-menu")
				return Bind(Prop, VK_INSERT);
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
		if (!__FORCE && Prop->Name == "show-menu")
			return Bind(Prop, VK_INSERT);
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
			switch (prop->Type)
			{
			case PropertyType::BOOLEAN:
			{
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
				break;
			}
			case PropertyType::INVERTER:
			{
				((CInverter*)prop->Value)->State = !((CInverter*)prop->Value)->State;
				break;
			}
			case PropertyType::DROPDOWN:
			{
				// TODO
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
