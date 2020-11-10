#pragma once

namespace GUI {
	extern bool Main();
	extern void ProcessingLoginMenu();
	extern bool LoginMenu();

	extern Config::Property* CurrentlyChoosingKeybindFor;
	extern bool IgnoreLButton;
	extern bool HackMenu();
	extern void LoadFonts(ImGuiIO& io);
}