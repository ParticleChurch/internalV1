#pragma once

namespace GUI {
	extern bool Main();
	extern void Rain(ImVec4 Color = ImVec4(1.f, 1.f, 1.f, 1.f));
	extern void ProcessingLoginMenu();
	extern bool LoginMenu();

	extern Config::Property* CurrentlyChoosingKeybindFor;
	extern bool HackMenu();


	//Paid Hack Helpers
	extern bool PaidHackMenu();

	//Free Hack Helpers
	extern bool FreeHackMenu();

	// Fonts
	extern void LoadFonts(ImGuiIO& io);
}