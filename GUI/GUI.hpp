#pragma once
#include "JSON/JSON.h"

namespace GUI {
	constexpr int PropertyLabelsWidth = 150;
	extern bool IgnoreLButton; // ignore mouse1 click (don't toggle switches bound to mouse1)
	extern Config::Property* CurrentlyChoosingKeybindFor;

	extern void LoadFonts(ImGuiIO& io);
	extern bool Main();
	extern bool HackMenu();
	extern bool LoginMenu();
	extern void ProcessingLoginMenu();
}