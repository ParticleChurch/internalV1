#pragma once
#include "animation.hpp"
#include "SkinConstants.hpp"
#include "../Utils/TextService.hpp"
#include "Config.hpp"

namespace GUI
{
	struct SearchResult {
		std::string title;
		std::vector<nlohmann::ordered_json> properties;
	};

	constexpr bool ConsoleOutput = true;

	extern bool Ejected;
	extern bool WantKeyboard;
	extern float LoadProgress;
	extern float VisibleLoadProgress;
	extern Animation::Anim* IntroAnimation;
	extern Animation::Anim* IntroAnimation2;
	extern Animation::Anim* SearchAnimation;

	extern ImVec2 DefaultMenuSize;
	extern ImVec2 MinMenuSize;
	extern int PropertyColumnPosition;

	extern bool IsSearching;
	extern char SearchQuery[];
	extern char CachedSearch[];
	extern std::vector<SearchResult> SearchResults;
	extern Config::Tab* ActiveTab;

	extern void LoadFonts(ImGuiIO& io);

	extern ImGuiWindow* MainWindow;
	extern void ClampToScreen();
	extern void LoadingScreen();
	extern void AuthenticationScreen(float Opacity = 1.f);
	extern void AuthenticationIntro();
	extern void DrawNormalTab(Config::Tab* t);
	extern void DrawActiveTab();
	extern void MainScreen(float ContentOpacity = 1.f, bool Interactable = true);
	extern void PerformSearch();
	extern void Init();
	extern void Main();

	extern CHudElement* FindHudElement(const char* Name);
	extern void ResetInventoryHud();
	extern bool IsSteamOverlayOpen();
};


namespace GUI2
{
	extern uint64_t frameCounter;

	void frame();

}