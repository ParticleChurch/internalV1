﻿#include "../Include.hpp"

namespace Config {

	namespace UserInfo
	{
		bool Authenticated = false; // is this user logged in at all?
		bool Banned = false; // has this user been banned
		bool Premium = false;
		int64_t TimeLeft = -1; // seconds left of their premium subscription
		int64_t UserId = 0x7FFFFFFFFFFFFFFF;
		std::string Email = "unauthenticated@a4g4.com";
		int64_t AccountAge = 1; // seconds since the account has been created
		std::string SessionId = "ABC";
		void Clear()
		{
			Authenticated = false;
			Banned = false;
			Premium = false;
			TimeLeft = -1;
			UserId = 0x7FFFFFFFFFFFFFFF;
			Email = "unauthenticated@a4g4.com";
			AccountAge = 1;
			SessionId = "ABC";
		}
	}

	std::string StringifyKeybindType(KeybindType type)
	{
		return
			type == KeybindType::Toggle ? "Toggle" :
			type == KeybindType::HoldToEnable ? "Hold To Enable" :
			"Hold To Disable";
	}
	std::string StringifyVK(int VirturalKey)
	{
		//return I::inputsystem->VirtualKeyToString(VirturalKey);
		return VK::GetName(VirturalKey);
	}

	std::vector<Tab*> Tabs = {};
	std::map<std::string, Property*> PropertyLookup;

	void Init()
	{
		// currently doing legit/rage offsensive tabs till new gui
		/* Legit Tab */
		{
			Tab* t = new Tab("Legit-Offensive");
			{
				
				Widget* w = t->AddWidget("Aimbot");
				w->AddProperty(true, 0, "legit-aim-enable", "Enable", false, false, KeybindOptions(true, true, true));


#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Stomach", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"
				
				CEditGroup* AimGunGroup = w->AddEditGroup("legitaim-");

				AimGunGroup->AddGroup("pistol-", "Pistol");
				w->AddProperty(false, 1, "legitaim-pistol-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "legitaim-pistol-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "legitaim-pistol-smoothing", "Smoothing Method", CDropdown{ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" });
				w->AddProperty(false, 1, "legitaim-pistol-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);

				AimGunGroup->AddGroup("smg-", "SMG");
				w->AddProperty(false, 1, "legitaim-smg-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "legitaim-smg-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "legitaim-smg-smoothing", "Smoothing Method", CDropdown{ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" });
				w->AddProperty(false, 1, "legitaim-smg-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);

				AimGunGroup->AddGroup("heavy-", "Heavy");
				w->AddProperty(false, 1, "legitaim-heavy-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "legitaim-heavy-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "legitaim-heavy-smoothing", "Smoothing Method", CDropdown{ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" });
				w->AddProperty(false, 1, "legitaim-heavy-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);

				AimGunGroup->AddGroup("rifle-", "Rifle");
				w->AddProperty(false, 1, "legitaim-rifle-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "legitaim-rifle-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "legitaim-rifle-smoothing", "Smoothing Method", CDropdown{ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" });
				w->AddProperty(false, 1, "legitaim-rifle-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);

				AimGunGroup->AddGroup("sniper-", "Snipers");
				w->AddProperty(false, 1, "legitaim-sniper-fov", "FOV", "DEG", 0, 180, 1, 0, 0);
				w->AddProperty(false, 1, "legitaim-sniper-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 0, "legitaim-sniper-smoothing", "Smoothing Method", CDropdown{ "None", "Slow-to-Fast", "Fast-to-Slow", "Linear" });
				w->AddProperty(false, 1, "legitaim-sniper-smoothing-amount", "Smoothing Amount", "%", 0, 100, 1, 0, 0);
	
#undef HITBOXES_CONFIG
			}
			{
				Widget* w = t->AddWidget("Other");
				w->AddProperty(false, 1, "backtracking-time", "Backtracking Time", "MS", 0, 200, 0, 0, 0);
			}
			Tabs.push_back(t);
		}

		/* Rage Tab */
		{
			Tab* t = new Tab("Rage-Offensive");
			{

				Widget* w = t->AddWidget("Aimbot");
				w->AddProperty(true, 0, "rage-aim-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(true, 0, "rage-aim-silent", "Silent Aim", true, true);
				w->AddProperty(true, 0, "rage-aim-autoshoot", "Auto Shoot", true, true);


#define HITBOXES_CONFIG "Head", "Neck", "Upper-Chest", "Lower-Chest", "Stomach", "Pelvis", "Upper-Arms", "Lower-Arms", "Upper-Legs", "Lower-Legs", "Toes"

				CEditGroup* AimGunGroup = w->AddEditGroup("rageaim-");

				AimGunGroup->AddGroup("pistol-", "Pistol");
				w->AddProperty(false, 1, "rageaim-pistol-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-pistol-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-pistol-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-pistol-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-pistol-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("smg-", "SMG");
				w->AddProperty(false, 1, "rageaim-smg-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-smg-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-smg-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-smg-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-smg-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("heavy-", "Heavy");
				w->AddProperty(false, 1, "rageaim-heavy-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-heavy-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-heavy-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-heavy-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-heavy-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("rifle-", "Rifle");
				w->AddProperty(false, 1, "rageaim-rifle-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-rifle-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-rifle-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-rifle-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-rifle-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("scout-", "Scout");
				w->AddProperty(false, 1, "rageaim-scout-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-scout-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-scout-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-scout-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-scout-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("awp-", "AWP");
				w->AddProperty(false, 1, "rageaim-awp-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-awp-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-awp-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-awp-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-awp-fireiflethal", "Fire if Lethal", false, false);

				AimGunGroup->AddGroup("auto-", "Auto");
				w->AddProperty(false, 1, "rageaim-auto-hitbox", "Select Hitbox Scan", CMultiSelector{ HITBOXES_CONFIG });
				w->AddProperty(false, 1, "rageaim-auto-mindamage-visible", "Visible Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-auto-mindamage-hidden", "Hidden Min Damage", "HP", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-auto-hitchance", "Min Hitchance", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "rageaim-auto-fireiflethal", "Fire if Lethal", false, false);

#undef HITBOXES_CONFIG
			}
			// backtracking max here
			Tabs.push_back(t);
		}

		/* DEFENSE TAB */
		{
			Tab* t = new Tab("Defense");
			{
				Widget* w = t->AddWidget("Fake Lag");

				w->AddProperty(false, 0, "antiaim-fakelag-tick", "Amount", "ticks", 0, 16, 0, 3, 3);
				w->AddProperty(false, 0, "antiaim-fakelag-distance", "Distance", "units", 0, 64, 0, 64, 64); //4096	

				w->AddProperty(false, 0, "antiaim-fakelag-trigger-tick", "Trigger Amount", "ticks", 0, 16, 0, 3, 3);
				w->AddProperty(false, 0, "antiaim-fakelag-trigger-distance", "Trigger Distance", "units", 0, 64, 0, 64, 64); //4096	
			}
			{
				Widget* w = t->AddWidget("Antiaim");
				CEditGroup* AntiAim = w->AddEditGroup("antiaim-");

				AntiAim->AddGroup("legit-", "Legit");
				w->AddProperty(false, 1, "antiaim-legit-enable", "Enable", false, false);
				w->AddProperty(false, 1, "antiaim-legit-max-angle", "Max Desync Angle", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 1, "antiaim-legit-invert", "Invert AA", new CInverter("Left", "Right"));

				AntiAim->AddGroup("rage-", "Rage");
				w->AddProperty(false, 0, "antiaim-rage-enable", "Enable", false, false);
				w->AddProperty(false, 0, "antiaim-rage-pitch", "Pitch", CDropdown{ "Up", "Down", "Trolling" });
				w->AddProperty(false, 2, "antiaim-rage-real", "Real Offset", "DEG", -180, 180, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-real-jitter", "Real Jitter Offset", "DEG", 0, 180, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-fake", "Fake Offset", "%", 0, 100, 0, 0, 0);
				w->AddProperty(false, 2, "antiaim-rage-fake-jitter", "Fake Jitter Offset", "%", 0, 100, 1, 100, 100);
				w->AddProperty(false, 1, "antiaim-rage-invert", "Invert AA", new CInverter("", ""));
				w->AddProperty(false, 2, "antiaim-rage-fake-invert", "Invert on Hit", false, false);
			}
			{
				/*Widget* w = t->AddWidget("Visuals");

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
				w->EndIndent();*/
			}
			Tabs.push_back(t);
		}

		/* VISUALS TAB */
		{
			Tab* t = new Tab("Visuals");
			{
				Widget* w = t->AddWidget("Chams");

#define CHAM_MATERIALS "Normal", "Flat", "Animated", "Glass", "Crystal", "Chrome", "Pearlescent", "Glow"

				CEditGroup* ESPGroup = w->AddEditGroup("visuals-chams-");

				ESPGroup->AddGroup("enemy-", "Enemies");
				w->AddText("Visible", "visuals-chams-enemy-visible-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-enemy-visible-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-enemy-visible-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-enemy-visible-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-enemy-visible-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

				w->AddText("Hidden", "visuals-chams-enemy-hidden-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-enemy-hidden-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-enemy-hidden-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-enemy-hidden-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-enemy-hidden-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

				/*
				w->AddText("Backtrack", "visuals-chams-enemy-backtrack-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-enemy-backtrack-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-enemy-backtrack-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-enemy-backtrack-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-enemy-backtrack-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();
				*/

				ESPGroup->AddGroup("friend-", "Firends");
				w->AddText("Visible", "visuals-chams-friend-visible-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-friend-visible-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-friend-visible-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-friend-visible-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-friend-visible-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

				w->AddText("Hidden", "visuals-chams-friend-hidden-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-friend-hidden-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-friend-hidden-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-friend-hidden-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-friend-hidden-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

				ESPGroup->AddGroup("localplayer-", "Localplayer");
				w->AddText("Real", "visuals-chams-localplayer-real-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-localplayer-real-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-localplayer-real-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-localplayer-real-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-localplayer-real-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

				w->AddText("Fake", "visuals-chams-localplayer-fake-label"); w->BeginIndent();
				w->AddProperty(false, 0, "visuals-chams-localplayer-fake-enable", "Enable", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-chams-localplayer-fake-color", "Color", new Color(0, 150, 255));
				w->AddProperty(false, 0, "visuals-chams-localplayer-fake-material", "Material", CDropdown{ CHAM_MATERIALS });
				w->AddProperty(false, 2, "visuals-chams-localplayer-fake-opacity", "Opacity", "%", 0, 100, 1, 100, 100);
				w->EndIndent();

#undef CHAM_MATERIALS
			}
			{
				Widget* w = t->AddWidget("ESP");

				/*w->AddProperty(false, 1, "visuals-esp_other-grenadebox", "Grenade Box", false, false);
				w->AddProperty(false, 2, "visuals-esp_other-weapon-box", "Weapon Box", false, false);
				w->AddProperty(false, 2, "visuals-esp_other-weapon-name", "Weapon Names", false, false);*/
				
				CEditGroup* ESPGroup = w->AddEditGroup("visuals-esp-");

				ESPGroup->AddGroup("enemy-", "Enemies");
				w->AddProperty(false, 0, "visuals-esp-enemy-enable", "Enable", false, false, KeybindOptions(true, true, true));
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
				w->AddProperty(false, 0, "visuals-esp-enemy-resolverflags", "Resolver Flags", false, false);
				/*w->AddProperty(false, 0, "visuals-esp-enemy-ammo ", "Ammo", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-ammo-color", "Foreground", new Color(0, 0, 255)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-enemy-ammo-color-background ", "Background", new Color(100, 100, 100)); w->EndIndent();*/

				ESPGroup->AddGroup("friend-", "Friends");
				w->AddProperty(false, 0, "visuals-esp-friend-enable", "Enable", false, false, KeybindOptions(true, true, true));
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
				/*w->AddProperty(false, 0, "visuals-esp-friend-ammo ", "Ammo", false, false);
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-ammo-color", "Foreground", new Color(0, 0, 255)); w->EndIndent();
				w->BeginIndent(); w->AddProperty(false, 2, "visuals-esp-friend-ammo-color-background ", "Background", new Color(100, 100, 100)); w->EndIndent();*/
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
			/*{
				Widget* w = t->AddWidget("View Model");
				w->AddProperty(false, 0, "visuals-fov", "FOV", "", 0, 100.f, 2, 0, 0);
				w->AddProperty(false, 0, "visuals-view-x", "Viewmodel X", "", -20.f, 20.f, 3, 0, 0);
				w->AddProperty(false, 0, "visuals-view-y", "Viewmodel Y", "", -20.f, 20.f, 3, 0, 0);
				w->AddProperty(false, 0, "visuals-view-z", "Viewmodel Z", "", -20.f, 20.f, 3, 0, 0);
			}*/
			{
				Widget* w = t->AddWidget("Misc");
				w->AddProperty(false, 0, "visuals-misc-thirdperson", "Thirdperson", false, false, KeybindOptions(true, true, true));
				w->AddProperty(false, 0, "visuals-misc-thirdperson-distance", "Thirdperson Distance", "", 0, 500, 0, 150, 150);

				w->AddProperty(false, 0, "visuals-misc-revealranks", "Reveal Ranks", false, false, KeybindOptions(false, false, false));
				w->AddProperty(false, 0, "visuals-misc-grenadeprediction", "Grenade Prediction", false, false, KeybindOptions(false, false, false));
				w->AddProperty(false, 0, "visuals-misc-noscope", "No Scope", false, false, KeybindOptions(false, false, false));
				w->AddProperty(false, 0, "visuals-misc-nosmoke", "No Smoke", false, false, KeybindOptions(false, false, false));
				w->AddProperty(false, 0, "visuals-misc-noflash", "No Flash", false, false, KeybindOptions(false, false, false));
				w->AddProperty(false, 0, "visuals-misc-tracers", "Bullet Tracers", false, false, KeybindOptions(false, false, false));
				
				/*w->AddProperty(false, 2, "visuals-misc-skin", "Skin ID", "", 0, 1000, 0, 0, 0);*/
				/*w->AddProperty(false, 2, "visuals-misc-vote-indicator", "Vote Indicator", false, false);
				w->AddProperty(false, 1, "visuals-misc-spec-list", "Spectator List", false, false);
				w->AddProperty(false, 1, "visuals-misc-teamdamage-list", "Team-damage List", false, false);*/
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
				w->AddProperty(false, 1, "misc-movement-fastcrouch", "Fast Crouch", false, false);
				w->AddProperty(false, 2, "misc-movement-fakeduck", "Fake Duck", false, false, KeybindOptions(true, true, true));
				//TODO - add rage autostrafe
				w->AddProperty(false, 0, "misc-movement-autostrafe", "Autostrafe", CDropdown{ "None",/* "Rage",*/ "Legit" });
				w->AddProperty(false, 2, "misc-movement-leg", "Leg Slide", false, false);
				w->AddProperty(false, 1, "misc-movement-leg-time", "Leg Slide Time", "MS", 0, 2000, 0, 0, 0);
			}
			{
				Widget* w = t->AddWidget("Other");
				w->AddProperty(false, 0, "misc-other-autoaccept", "AutoAccept", false, false);
				w->AddProperty(false, 2, "misc-other-killsay", "Kill Say", false, false);
				w->AddProperty(false, 0, "misc-other-killsay-input", "Kill Say Text", 256, "Get pwnd by a4g4.com!");
				//TODO - add input area for custom killsay insult
				//w->AddProperty(false, 2, "misc-other-fullautopistol", "Full Auto Pistol", false, false); //aka autopistol
				// w->AddProperty(false, 2, "misc-other-fakeunbox", "Fake Unbox", false, false);
				// TODO - add area to input what they are unboxing
				/*w->AddProperty(false, 0, "misc-other-clantag", "Clantag", false, false);*/
				w->AddProperty(false, 0, "misc-other-clantag-input", "Clantag", 256, "a4g4.com");
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
				w->AddProperty(false, 0, "show-console", "Show Console", false, false, KeybindOptions(true, true, true));
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
				Widget* w = t->AddWidget("Config");
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

		// default values
		LoadFromString(ConfigManager::ConfigAllDisabled);

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
				L::Log(("Config::GetBool: nonexistant property: \"" + Name + "\"").c_str());
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
					L::Log(("Config::GetBool: non boolean value: \"" + Name + "\"").c_str());
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
				L::Log(("Config::GetFloat: nonexistant property: \"" + Name + "\"").c_str());
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
					L::Log(("Config::GetFloat: non float value: \"" + Name + "\"").c_str());
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
				L::Log(("Config::GetColor: nonexistant property: \"" + Name + "\"").c_str());
			return Color(0,0,0);
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::COLOR)
				return *(Color*)prop->Value;
			else
			{
				if (CONFIG_DEBUG)
					L::Log(("Config::GetColor: non color value: \"" + Name + "\"").c_str());
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
				L::Log(("Config::GetState: nonexistant property: \"" + Name + "\"").c_str());
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
					L::Log(("Config::GetState: non stateful value: \"" + Name + "\"").c_str());
				return 0;
			}
			}
		}
	}
	uint16_t GetSelections(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				L::Log(("Config::GetSelections: nonexistant property: \"" + Name + "\"").c_str());
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
					L::Log(("Config::GetSelections: non multiselector value: \"" + Name + "\"").c_str());
				return 0;
			}
		}
	}
	std::string GetText(std::string Name)
	{
		auto search = PropertyLookup.find(Name);
		if (search == PropertyLookup.end())
		{
			if (CONFIG_DEBUG)
				L::Log(("Config::GetText: nonexistant property: \"" + Name + "\"").c_str());
			return "";
		}
		else
		{
			Property* prop = search->second;
			if (prop->Type == PropertyType::TEXTINPUT)
				return ((CTextInput*)prop->Value)->text;
			else
			{
				if (CONFIG_DEBUG)
					L::Log(("Config::GetText: non text input value: \"" + Name + "\"").c_str());
				return "";
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
	}

	void ClearAllKeybinds()
	{
		for (size_t t = 0; t < Tabs.size(); t++)
		{
			Tab* tab = Tabs.at(t);
			for (size_t w = 0; w < tab->Widgets.size(); w++)
			{
				Widget* widget = tab->Widgets.at(w);
				for (size_t p = 0; p < widget->Properties.size(); p++)
				{
					Property* prop = widget->Properties.at(p);
					Unbind(prop);
					prop->BindType = KeybindType::Toggle;
				}
			}
		}
	}

	std::string ExportToString()
	{
		std::string s = "";
		s += "# a4g4.com config generated on mm/dd/yyyy @ hh:mm:ss\n";
		s += "# For help making a config, please visit https://a4g4.com/help/config\n";
		s += "# \n";
		s += "# The format for this file is as follows,\n";
		s += "# and is explained in greater detail at above link:\n";
		s += "# property-name : keybind_type @ key_name: value\n";

		for (size_t t = 0; t < Tabs.size(); t++)
		{
			Tab* tab = Tabs.at(t);
			if (tab->Name == "Config") continue;

			// ##############
			// ## TAB NAME ##
			// ##############
			s += "\n";
			s += "\n";
			s += "\n";
			s += "###";
			for (size_t i = 0; i < tab->Name.size(); i++) s += "#";
			s += "###\n";
			s += "## " + tab->Name + " ##\n";
			s += "###";
			for (size_t i = 0; i < tab->Name.size(); i++) s += "#";
			s += "###\n";

			for (size_t w = 0; w < tab->Widgets.size(); w++)
			{
				Widget* widget = tab->Widgets.at(w);

				// ## WIDGET NAME
				s += "\n";
				s += "## " + widget->Name + "\n";

				// determine longest property name
				size_t propertyNameLength = 0;
				for (size_t p = 0; p < widget->Properties.size(); p++)
				{
					Property* prop = widget->Properties.at(p);
					if (prop->Type == PropertyType::TEXT) continue;
					if (prop->Type == PropertyType::EDITGROUP) continue;
					if (prop->Name.size() > propertyNameLength)
						propertyNameLength = prop->Name.size();
				}

				for (size_t p = 0; p < widget->Properties.size(); p++)
				{
					Property* prop = widget->Properties.at(p);
					if (prop->Type == PropertyType::TEXT) continue;
					if (prop->Type == PropertyType::EDITGROUP) continue;

					s += prop->Name;
					for (size_t i = prop->Name.size(); i < propertyNameLength; i++)
						s += " ";
					s += " :";
					if (prop->KeyBind > 0)
					{
						switch (prop->BindType)
						{
						default:
						case KeybindType::Toggle:
							s += " toggle @ ";
							break;
						case KeybindType::HoldToEnable:
							s += " hold_enable @ ";
							break;
						case KeybindType::HoldToDisable:
							s += " hold_disable @ ";
							break;
						}
						s += VK::GetName(prop->KeyBind) + " ";
					}
					s += ": " + prop->Stringify() + "\n";
				}
			}
		}

		return s;
	}
	size_t LoadFromString(std::string in)
	{
		size_t numPropsAffected = 0;

		// splice out everything between a "#" and a "\n" (aka comments)
		{
			size_t tag_index = 0;
			size_t newline_index = 0;
			while (true)
			{
				tag_index = in.find('#');
				if (tag_index == std::string::npos) break; // all comments are removed!

				newline_index = in.find("\n", tag_index); // first newline that comes after the tag
				if (newline_index == std::string::npos) newline_index = in.size(); // if no newline, this comment goes to EOF

				in = in.erase(tag_index, newline_index - tag_index);
			}
		}

		// replace \r\n with \n
		{
			size_t crlf = 0;
			while ((crlf = in.find("\r\n", crlf)) != std::string::npos)
				in.replace(crlf, 2, "\n");
		}

		// remove repeating spaces
		{
			std::string::iterator new_end = std::unique(in.begin(), in.end(), [](char lhs, char rhs) { return (lhs == ' ') && (rhs == ' '); });
			in.erase(new_end, in.end());
		}

		// remove repeating newlines
		{
			std::string::iterator new_end = std::unique(in.begin(), in.end(), [](char lhs, char rhs) { return (lhs == '\n') && (rhs == '\n'); });
			in.erase(new_end, in.end());
		}

		// loop through lines
		{
			size_t a = 0, b = 0, splitterA = 0, splitterB = 0, splitterC = 0;
			std::string line;
			while (b != std::string::npos)
			{
				b = in.find('\n', a);
				line = in.substr(a, b - a);
				a = b + 1;

				// require two colons
				if ((splitterA = line.find(":")) == std::string::npos) continue;
				if ((splitterB = line.find(":", splitterA + 1)) == std::string::npos) continue;

				std::string propName = line.substr(0, splitterA);
				std::string keybind = line.substr(splitterA + 1, splitterB - splitterA - 1);
				std::string value = line.substr(splitterB + 1);
				trim(propName);
				trim(keybind);
				trim(value);

				auto p_iter = PropertyLookup.find(propName);
				if (p_iter == PropertyLookup.end()) continue;
				
				Property* p = p_iter->second;

				bool keybindParsed = (keybind.size() == 0);
				if ((splitterC = keybind.find("@")) != std::string::npos)
				{
					std::string btype = keybind.substr(0, splitterC);
					std::string key = keybind.substr(splitterC + 1);
					trim(btype);
					trim(key);
					std::transform(btype.begin(), btype.end(), btype.begin(), [](unsigned char c) { return std::tolower(c); });

					KeybindType t = KeybindType::INVALID;
					if (btype == "toggle") t = KeybindType::Toggle;
					else if (btype == "hold_enable") t = KeybindType::HoldToEnable;
					else if (btype == "hold_disable") t = KeybindType::HoldToDisable;

					if (t != KeybindType::INVALID)
					{
						int vk = VK::GetCode(key.c_str());
						if (vk > 0)
						{
							Bind(p, vk);
							p->BindType = t;
							keybindParsed = true;
						}
					}
				}

				bool valueParsed = p->Parse(value);
				if (!valueParsed)
					L::Log(("Failed to parse " + propName + ", w/ val = " + value).c_str());
				if (!keybindParsed)
					L::Log(("Failed to keybind " + propName + ", w/ key = " + keybind).c_str());

				if (keybindParsed || valueParsed)
					numPropsAffected++;
			}
		}

		return numPropsAffected;
	}
}

namespace Config2
{
	std::map<std::string, Property*> PropertyTable{};
	std::vector<Tab*> Tabs{};
	Property* SettingKeybindFor = nullptr;
	std::string KeybindTypeNames[] = {
		"Toggle", "Enable", "Disable"
	};

	void Init()
	{
		Tab* t;
		Group* g;
		Property* p;

		// OFFENCE
		{
			t = new Tab("Offence");
			t->TopPadding = -10;
			Group* OffenceMeta = t->Add("__META__");
			Property* OffenceMode = OffenceMeta->Add("offence-mode", "", new CHorizontalState({ "Legit", "Rage" }, false, true));

			/*
				LEGIT PAGE
			*/
			{
				g = t->Add("legit-Aimbot");

				Property* Enable = g->Add("legit-aimbot-enable", "Enable", new CBoolean());

				p = g->Add("legit-aimbot-fov", "FOV", new CFloat(0, 180, 1, "DEG"));
				p = g->Add("legit-aimbot-smoothing", "Smoothing", new CFloat(0, 100, 1, "%"));
			}


			/*
				RAGE PAGE
			*/
			{
				g = t->Add("rage-Aimbot");

				Property* Enable = g->Add("rage-aimbot-enable", "Enable", new CBoolean());

				p = g->Add("rage-aimbot-silent", "Silent Aim", new CBoolean());
				p->Master = Enable;

				p = g->Add("rage-aimbot-autofire", "Auto Fire", new CBoolean());
				p->Master = Enable;

				p = g->Add("rage-aimbot-lethal", "Ignore Hitboxes If Lethal", new CBoolean());
				p->Master = Enable;
			}
		}

		// DEFENCE
		{
			t = new Tab("Defence");
			t->TopPadding = -10;
			Group* OffenceMeta = t->Add("__META__");
			Property* OffenceMode = OffenceMeta->Add("defence-mode", "", new CHorizontalState({ "Legit", "Rage" }, false, true));

			/*
				LEGIT PAGE
			*/

			// fakelag
			{
				g = t->Add("legit-Fake Lag");

				Property* Enable = g->Add("legit-fakelag-enable", "Enable", new CBoolean());

				p = g->Add("legit-fakelag-time", "Max Time", new CFloat(1, 16, 0, "TICKS"));
				p->Master = Enable;
				p = g->Add("legit-fakelag-distance", "Max Distance", new CFloat(1, 64, 0, "UNITS"));
				p->Master = Enable;
				p = g->Add("legit-fakelag-trigger-time", "Triggered Max Time", new CFloat(1, 16, 0, "TICKS"));
				p->Master = Enable;
				p = g->Add("legit-fakelag-trigger-fistance", "Triggered Max Distance", new CFloat(1, 64, 0, "UNITS"));
				p->Master = Enable;
			}

			// anti aim
			{
				g = t->Add("legit-Anti-Aim");

				Property* Enable = g->Add("legit-antiaim-enable", "Enable", new CBoolean());

				p = g->Add("legit-antiaim-desync", "Desync", new CFloat(1, 100, 1, "%"));
				p->Master = Enable;
			}


			/*
				RAGE PAGE
			*/
			// fakelag
			{
				g = t->Add("rage-Fake Lag");

				Property* Enable = g->Add("rage-fakelag-enable", "Enable", new CBoolean());

				p = g->Add("rage-fakelag-time", "Max Time", new CFloat(1, 16, 0, "TICKS"));
				p->Master = Enable;
				p = g->Add("rage-fakelag-distance", "Max Distance", new CFloat(1, 64, 0, "UNITS"));
				p->Master = Enable;
				p = g->Add("rage-fakelag-trigger-time", "Triggered Max Time", new CFloat(1, 16, 0, "TICKS"));
				p->Master = Enable;
				p = g->Add("rage-fakelag-trigger-fistance", "Triggered Max Distance", new CFloat(1, 64, 0, "UNITS"));
				p->Master = Enable;
			}

			// anti aim
			{
				g = t->Add("rage-Anti-Aim");

				Property* Enable = g->Add("rage-antiaim-enable", "Enable", new CBoolean());

				p = g->Add("rage-antiaim-real-offset", "Rotate Real", new CFloat(0, 180, 1, "DEG"));
				p->Master = Enable;

				p = g->Add("rage-antiaim-real-jitter", "Real Rotation Jitter", new CFloat(0, 180, 1, "DEG"));
				p->Master = Enable;

				p = g->Add("rage-antiaim-fake-offset", "Rotate Fake", new CFloat(0, 100, 1, "%"));
				p->Master = Enable;

				p = g->Add("rage-antiaim-fake-jitter", "Fake Rotation Jitter", new CFloat(0, 100, 1, "%"));
				p->Master = Enable;

				p = g->Add("rage-antiaim-invert-on-hit", "Invert Rotation On Hit", new CBoolean());
				p->Master = Enable;
			}
		}

		// VISUALS
		{
			Tab* t = new Tab("Visuals");
		}

		// SKINCHANGER
		{
			Tab* t = new Tab("Skinchanger");

			{ // knives
				Group* g = t->Add("Knives");

				g->Add("skinchanger-knife-skin", "x", new CPaintKit());
				g->Add("skinchanger-knife-model", "x", new CKnife());
			}

			{ // weapons
				Group* g = t->Add("Guns");

				for (int i = 0; i < (int)Skins::Weapon::_COUNT; i++)
					g->Add("skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[i])), Skins::WeaponNames[i], new CPaintKit());
			}
		}

		// MOVEMENT
		{
			Tab* t = new Tab("Movement");
		}

		// MISC
		{
			Tab* t = new Tab("Misc");
		}

		// THEME
		{
			Tab* t = new Tab("Theme");
			t->TopPadding = -10;

			{
				Group* g = t->Add("General");

				g->Add("theme-topbar-background", "Topbar Background", new CColor(true));
				g->Add("theme-topbar-text", "Topbar Text", new CColor(true));
				g->Add("theme-border-thickness", "Outline Thickness", new CFloat(0.f, 5.f, 1, "PX"));
				p = g->Add("theme-border-color", "Outline", new CColor(true));
				p->IsVisible = []() { return GetFloat("theme-border-thickness") > 0.f; };

				g->Add("theme-tab-background-use-image", "Background Uses Image", new CBoolean(false));
				p = g->Add("theme-tab-background-image", "Background Image", new CLabel());
				p->IsVisible = []() { return GetBoolean("theme-tab-background-use-image"); };
				p = g->Add("theme-tab-background-color", "Background Color", new CColor(false));
				p->IsVisible = []() { return !GetBoolean("theme-tab-background-use-image"); };

				g->Add("theme-tablist-background", "Tab List Background", new CColor(true));
				g->Add("theme-tablist-text", "Tab List Text", new CColor(false));
				g->Add("theme-eject-tab-text", "Eject Tab", new CColor(false));

				g->Add("theme-searchbar-background", "Search Bar Background", new CColor(false));
				g->Add("theme-searchbar-text", "Search Bar Text", new CColor(false));
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

		LoadTheme(ConfigConstants::ThemeDefaultDark, ConfigConstants::ThemeDefaultDarkSize);
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
		if (p->Type != PropertyType::BOOLEAN) return -1;

		CBoolean* v = (CBoolean*)p->Value;
		if (v->BoundToKey >= 0)
			return Keybind::KeyMap[v->BoundToKey];

		return -1;
	}

	bool GetBoolean(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return false;

		#define BRO(x) (((CBoolean*)((x)->Value))->Value)

		if (p->Master && p->Master->Type == PropertyType::BOOLEAN)
			return BRO(p) && BRO(p->Master);
		else
			return BRO(p);

		#undef BRO
	}

	float GetFloat(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return false;

		return ((CFloat*)p->Value)->Get();
	}

	int GetPaintKit(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return false;

		return ((CPaintKit*)p->Value)->PaintKit->ID;
	}

	int GetState(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return 0;

		switch (p->Type)
		{
		case PropertyType::BOOLEAN: // I write Config::GetBoolean, and you use this? >:(
			return (int)(((CBoolean*)p->Value)->Value);
		case PropertyType::HSTATEFUL:
			return (int)(((CHorizontalState*)p->Value)->State);
		}

		return 0;
	}

	CColor GetColor(std::string Name)
	{
		auto p = GetProperty(Name);
		if (!p) return CColor(true);

		return *(CColor*)p->Value;
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
		case PropertyType::BOOLEAN:
		{
			valueLength = 1;
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::FLOAT:
		{
			valueLength = sizeof(float);
			spaceRequired += p->Name.length() + 1;
			spaceRequired += valueLength;
		} break;
		case PropertyType::COLOR:
		{
			valueLength = sizeof(unsigned char) * 4;
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
			case PropertyType::BOOLEAN:
			{
				(*buffer)[*bufferSpaceOccupied] = ((CBoolean*)p->Value)->Value ? 0xFF : 0x0;
			} break;
			case PropertyType::FLOAT:
			{
				float v = ((CFloat*)p->Value)->Get();
				memcpy(*buffer + *bufferSpaceOccupied, (void*)&v, sizeof(float));
			} break;
			case PropertyType::COLOR:
			{
				CColor* c = (CColor*)p->Value;
				(*buffer)[*bufferSpaceOccupied + 0] = c->R;
				(*buffer)[*bufferSpaceOccupied + 1] = c->G;
				(*buffer)[*bufferSpaceOccupied + 2] = c->B;
				(*buffer)[*bufferSpaceOccupied + 3] = c->A;
			} break;
			}
			*bufferSpaceOccupied += spaceRequired;
		}

		L::Verbose("ExportSingleProperty success");
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

	void LoadTheme(const char* Theme, size_t nBytes)
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

		if (memcmp(header, Theme, headerSize))
		{
			L::Log("Tried to load a theme with invalid header");
			return;
		}

		size_t i = headerSize;
		while (i < nBytes)
		{
			size_t remaining = nBytes - i;
			size_t valueLength = 0;
			char* name = nullptr;
			size_t nameSize = 0;

			// +1 because there must be at least 1 more byte for the propertyName
			if (remaining < sizeof(size_t) + 1) break;
			valueLength = *(size_t*)(Theme + i);
			i += sizeof(size_t);
			remaining -= sizeof(size_t);

			// hhhhhxxxxYYY

			// get name
			name = (char*)(Theme + i);
			for (size_t j = 0; j < remaining; j++)
			{
				if (name[j] == '\0')
				{
					nameSize = j + 1;
					break;
				}
			}

			// didn't find a null terminator
			if (nameSize == 0) break;
			i += nameSize;
			remaining -= nameSize;

			// make sure there is enough space for the value
			if (remaining < valueLength) break;

			Property* p = GetProperty(name);
			if (!p)
			{
				// this property does not exist, probably an older version of the config editor
				L::Log(XOR("Theme attempted to load invalid property: "), "");
				L::Log(name);
				i += valueLength;
				continue;
			}

			switch (p->Type)
			{
			case PropertyType::BOOLEAN:
			{
				if (valueLength != 1) goto INVALID_LENGTH;
				((CBoolean*)p->Value)->Value = *(Theme + i) > 0x7f;
			} break;
			case PropertyType::FLOAT:
			{
				if (valueLength != sizeof(float)) goto INVALID_LENGTH;
				((CFloat*)p->Value)->Set(*(float*)(Theme + i));
			} break;
			case PropertyType::COLOR:
			{
				if (valueLength != sizeof(unsigned char) * 4) goto INVALID_LENGTH;
				CColor* c = (CColor*)p->Value;
				c->R = *(Theme + i + 0);
				c->G = *(Theme + i + 1);
				c->B = *(Theme + i + 2);
				c->A = *(Theme + i + 3);
			} break;
			}

			L::Log(XOR("Loaded theme for property: "), "");
			L::Log(p->Name.c_str());
			i += valueLength;
			continue;

		INVALID_LENGTH:
			L::Log(XOR("Got unexpected value length for property: "), p->Name.c_str());
			L::Log(XOR(" with type = "), std::to_string((int)p->Type).c_str());
			L::Log(XOR(" (got "), std::to_string(valueLength).c_str());
			L::Log(XOR(" bytes)"));
			i += valueLength;
		}
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
			if (!GetSaveFileName(&ofn) || filename[0] == '\0')
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
			if (!GetOpenFileName(&ofn) || filename[0] == '\0')
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

		L::Verbose("_PromptImportThemeFile passing execution to Config2::LoadTheme()");
		Config2::LoadTheme(buffer, bytes);
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

	void ProcessKeys()
	{
		auto io = ImGui::GetIO();

		Keybind::Lock = true;
		while (Keybind::KeyChangeStack.size() > 0)
		{
			auto log = Keybind::KeyChangeStack[0];
			bool ismouse = Keybind::KeyMap[log.Key] == VK_LBUTTON;
			L::Verbose(("ProcessKeys - key " + std::to_string(log.Key) + (log.State ? " down" : " up")).c_str());

			if (ismouse && log.WantCaptureMouse)
				goto CONT;

			if (SettingKeybindFor && log.State)
			{
				switch (SettingKeybindFor->Type)
				{
				case PropertyType::BOOLEAN:
				{
					if (Keybind::KeyMap[log.Key] == VK_ESCAPE) break;

					CBoolean* b = (CBoolean*)SettingKeybindFor->Value;
					b->BoundToKey = log.Key;
					Keybind::Binds[log.Key].push_back(SettingKeybindFor);
				}
				break;
				default:
					L::Log(("ProcessKeys - idk how to deal with bind on non-boolean property " + SettingKeybindFor->Name).c_str());
					break;
				}

				SettingKeybindFor = nullptr;
			}
			else
			{
				std::vector<void*> Properties = Keybind::Binds[log.Key];
				for (size_t i = 0; i < Properties.size(); i++)
				{
					Property* p = (Property*)Properties[i];
					switch (p->Type)
					{
					case PropertyType::BOOLEAN:
					{
						CBoolean* b = (CBoolean*)p->Value;
						switch (b->BindMode)
						{
						default:
						case KeybindMode::TOGGLE:
							if (log.State)
								b->Flip();
							break;
						case KeybindMode::HOLDTOENABLE:
							if (b->Value != log.State)
								b->Flip();
							break;
						case KeybindMode::HOLDTODISABLE:
							if (b->Value == log.State)
								b->Flip();
							break;
						}
					}
					break;
					default:
						L::Log(("ProcessKeys - idk how to deal with bind on non-boolean property " + p->Name).c_str());
						break;
					}
				}
			}

			CONT:
			Keybind::KeyChangeStack.erase(Keybind::KeyChangeStack.begin());
		}
		Keybind::Lock = false;
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
	double ClientTimestamp = -1.0;
	double ServerTimestamp = -1.0;

	bool Initialized = false;
	bool Authenticated = false;
	std::string Email = "unauthenticated@a4g4.com";
	uint64_t UserID = (uint64_t)-1;
	bool Banned = false;
	bool Premium = false;
	bool PremiumAutoRenews = false;
	double AccountRegistrationTimestamp = -1.0;

	double NextBillDueTimestamp = -1.0;
	double PremiumExpiresTimestamp = -1.0;
}