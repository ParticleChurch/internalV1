#pragma once
#include "JSON/JSON.h"
#include "animation.hpp"

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

namespace GUI2
{
	constexpr bool ConsoleOutput = false;

	extern bool Ejected;
	extern float LoadProgress;
	extern float VisibleLoadProgress;
	extern Animation::Anim* IntroAnimation;
	extern Animation::Anim* SearchAnimation;

	extern ImVec2 DefaultMenuSize;
	extern ImVec2 MinMenuSize;

	extern bool IsSearching;
	extern char* SearchQuery;
	extern Config2::Tab* ActiveTab;

	extern void LoadingScreen();
	extern void AuthenticationScreen(float Opacity = 1.f);
	extern void DrawNormalTab(Config2::Tab* t);
	extern void DrawActiveTab();
	extern void MainScreen(float ContentOpacity = 1.f, bool Interactable = true);
	extern void Init();
	extern void Main();
};