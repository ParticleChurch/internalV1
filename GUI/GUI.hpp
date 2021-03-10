#pragma once
#include "JSON/JSON.h"
#include "animation.hpp"
#include "SkinConstants.hpp"
#include "TextService.hpp"
#include "Config.hpp"

namespace GUI2
{
	constexpr bool ConsoleOutput = true;

	extern bool Ejected;
	extern bool WantMouse;
	extern float LoadProgress;
	extern float VisibleLoadProgress;
	extern Animation::Anim* IntroAnimation;
	extern Animation::Anim* IntroAnimation2;
	extern Animation::Anim* SearchAnimation;

	extern ImVec2 DefaultMenuSize;
	extern ImVec2 MinMenuSize;
	extern int PropertyColumnPosition;

	extern bool IsSearching;
	extern char* SearchQuery;
	extern Config2::Tab* ActiveTab;

	extern void LoadFonts(ImGuiIO& io);

	extern void LoadingScreen();
	extern void AuthenticationScreen(float Opacity = 1.f);
	extern void AuthenticationIntro();
	extern void DrawNormalTab(Config2::Tab* t, std::string GroupPrefix = "");
	extern void DrawActiveTab();
	extern void MainScreen(float ContentOpacity = 1.f, bool Interactable = true);
	extern void Init();
	extern void Main();
};