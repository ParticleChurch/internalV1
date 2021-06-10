#include "../Include.hpp"

#define PREMIUM_USERS_ONLY XOR("Upgrade to premium to access this feature.")

ImFont* FontDefault;
ImFont* Arial8;
ImFont* Arial12;
ImFont* Arial14;
ImFont* Arial16;
ImFont* Arial18;
ImFont* Arial16Bold;
ImFont* Arial18Bold;
ImFont* Arial12Italics;
ImFont* Arial14Italics;
ImFont* Arial16Italics;
ImFont* Arial18Italics;
ImFont* Arial14BoldItalics;
ImFont* Arial18BoldItalics;
#define AYO_LOAD_FONT_BRUH(name, path, size) if (!(name = io.Fonts->AddFontFromFileTTF(path, size))){goto problemo;}
void GUI::LoadFonts(ImGuiIO& io)
{
	FontDefault = io.Fonts->AddFontDefault();
	AYO_LOAD_FONT_BRUH(Arial8, XOR("C:\\Windows\\Fonts\\arial.ttf"), 8.f);
	AYO_LOAD_FONT_BRUH(Arial12, XOR("C:\\Windows\\Fonts\\arial.ttf"), 12.f);
	AYO_LOAD_FONT_BRUH(Arial14, XOR("C:\\Windows\\Fonts\\arial.ttf"), 14.f);
	AYO_LOAD_FONT_BRUH(Arial16, XOR("C:\\Windows\\Fonts\\arial.ttf"), 16.f);
	AYO_LOAD_FONT_BRUH(Arial18, XOR("C:\\Windows\\Fonts\\arial.ttf"), 18.f);
	AYO_LOAD_FONT_BRUH(Arial12Italics, XOR("C:\\Windows\\Fonts\\ariali.ttf"), 12.f);
	AYO_LOAD_FONT_BRUH(Arial14Italics, XOR("C:\\Windows\\Fonts\\ariali.ttf"), 14.f);
	AYO_LOAD_FONT_BRUH(Arial16Italics, XOR("C:\\Windows\\Fonts\\ariali.ttf"), 16.f);
	AYO_LOAD_FONT_BRUH(Arial18Italics, XOR("C:\\Windows\\Fonts\\ariali.ttf"), 18.f);
	AYO_LOAD_FONT_BRUH(Arial14BoldItalics, XOR("C:\\Windows\\Fonts\\arialbi.ttf"), 14.f);
	AYO_LOAD_FONT_BRUH(Arial18BoldItalics, XOR("C:\\Windows\\Fonts\\arialbi.ttf"), 18.f);
	AYO_LOAD_FONT_BRUH(Arial16Bold, XOR("C:\\Windows\\Fonts\\arialbd.ttf"), 16.f);
	AYO_LOAD_FONT_BRUH(Arial18Bold, XOR("C:\\Windows\\Fonts\\arialbd.ttf"), 18.f);

	return;
problemo:
	MessageBox(NULL, XOR("a4g4.com requires that you have the \"Arial\" font (and it's italics version) installed. It comes installed by default with Windows OS in C:/Windows/Fonts. Please download Airal to that location (as arial.ttf and ariali.ttf) then try injecting again."), XOR("Missing Fonts"), 0);
	// segfault lol
	int x = *(int*)0;
	// prevent dumbass compiler from ignoring our segfault >:(
	L::Log(std::to_string(x).c_str());
}

// random imgui utils
namespace ImGui
{
	inline float lerp(float a, float b, float f)
	{
		return (b - a) * f + a;
	}
	
	inline double lerp(double a, double b, double f)
	{
		return (b - a) * f + a;
	}
	
	ImVec2 lerp(ImVec2 a, ImVec2 b, float f)
	{
		return ImVec2(
			lerp(a.x, b.x, f),
			lerp(a.y, b.y, f)
		);
	}
	
	ImVec4 lerp(ImVec4 a, ImVec4 b, float f)
	{
		return ImVec4(
			lerp(a.x, b.x, f),
			lerp(a.y, b.y, f),
			lerp(a.z, b.z, f),
			lerp(a.w, b.w, f)
		);
	}
}

/*
	=========== GUI VERSION 2.0 ===========
*/
namespace GUI
{
	bool Ejected = false;
	bool WantKeyboard = false;
	float LoadProgress = 0.f;
	float VisibleLoadProgress = 0.f;
	Animation::Anim* IntroAnimation = nullptr;
	Animation::Anim* IntroAnimation2 = nullptr;
	Animation::Anim* SearchAnimation = nullptr;

	ImVec2 DefaultMenuSize = ImVec2(650, 330);
	ImVec2 MinMenuSize = ImVec2(595, 245);
	int PropertyColumnPosition = 200;

	bool IsSearching = false;
	char SearchQuery[256];
	char CachedSearch[256];
	std::vector<SearchResult> SearchResults = {};
	Config::Tab* ActiveTab = nullptr;
	
	ImGuiWindow* MainWindow = nullptr;
};

// Util drawers
namespace ImGui
{
	void DrawTeardrop(ImVec2 ScreenPosition, ImVec2 Dimensions, unsigned char Opacity = 255)
	{
		constexpr float AspectRatio = 30.f / 45.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		float StrokeSize = Size.x / 10.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		ImVec2 Position = ScreenPosition + (Dimensions - Size) / 2;

		// C = PI*Size.x
		int Segments = max(9, IM_PI * Size.x * 0.2f);

		// bottom curve
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2, Size.y - Size.x / 2), Size.x / 2, -IM_PI / 6.f, IM_PI * 7.f / 6.f, Segments);
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2, Size.y / 12.f), Size.y / 12.f, IM_PI * 7.f / 6.f, IM_PI * 11.f / 6.f, Segments / 3);

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(0, 0, 0, Opacity), true, StrokeSize);
		DrawList->PathClear();
	}

	void DrawAccountIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 25.f / 30.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float CornerRadius = Size.x / 8.f;
		int Segments = max(4, IM_PI * CornerRadius * 2.f / 4.f);

		DrawList->PathArcTo(Position + ImVec2(CornerRadius, Size.y - CornerRadius), CornerRadius, IM_PI / 2.f, IM_PI, Segments); // bl
		DrawList->PathArcTo(Position + ImVec2(CornerRadius, Size.y * 0.7f), CornerRadius, IM_PI, IM_PI * 4.f / 3.f, max(4, Segments / 2)); // tl
		DrawList->PathLineTo(Position + ImVec2(Size.x * 0.333f, Size.y / 2.f)); // tml
		DrawList->PathLineTo(Position + ImVec2(Size.x * 0.667f, Size.y / 2.f)); // tmr
		DrawList->PathArcTo(Position + ImVec2(Size.x - CornerRadius, Size.y * 0.7f), CornerRadius, IM_PI * 5.f / 3.f, 2.f * IM_PI, max(4, Segments / 2));// tr
		DrawList->PathArcTo(Position + ImVec2(Size.x - CornerRadius, Size.y - CornerRadius), CornerRadius, 2.f * IM_PI, IM_PI * 5.f / 2.f, Segments); // br

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, CornerRadius * 2), CornerRadius * 2, 0, IM_PI * 2.f, Segments * 4);

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->PathClear();
	}

	void DrawPasswordIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 25.f / 30.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float CornerRadius = Size.x / 8.f;
		int Segments = max(4, IM_PI * CornerRadius * 2.f / 4.f);

		DrawList->PathArcTo(Position + ImVec2(Size.x - CornerRadius, Size.y - CornerRadius), CornerRadius, 0.f, IM_PI / 2.f, Segments); // br
		DrawList->PathArcTo(Position + ImVec2(CornerRadius, Size.y - CornerRadius), CornerRadius, IM_PI / 2.f, IM_PI, Segments); // bl
		DrawList->PathArcTo(Position + ImVec2(CornerRadius, Size.y / 2.f + CornerRadius), CornerRadius, IM_PI, 3.f * IM_PI / 2.f, Segments); // tl
		DrawList->PathArcTo(Position + ImVec2(Size.x - CornerRadius, Size.y / 2.f + CornerRadius), CornerRadius, 3.f * IM_PI / 2.f, 2.f * IM_PI, Segments); // tr

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->PathClear();

		float StrokeSize = Size.x / 10.f;
		float ShackleRadius = Size.x / 4.f;
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f - ShackleRadius, Size.y / 2.f + StrokeSize / 2.f));
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f - ShackleRadius, ShackleRadius + StrokeSize / 2.f));
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, ShackleRadius + StrokeSize / 2.f), ShackleRadius, -IM_PI, 0.f, Segments * 4.f);
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f + ShackleRadius, Size.y / 2.f + StrokeSize / 2.f));

		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity), false, StrokeSize);
		DrawList->PathClear();
	}

	void DrawSearchIcon(ImU32 Color, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 1.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float StrokeSize = Size.x / 10.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		//ImVec2 StokeVector = ImVec2(StrokeSize, StrokeSize);
		float MainRadius = Size.x * 0.75f / 2.f;
		int Segments = max(4, IM_PI * MainRadius * 2.f);

		DrawList->PathArcTo(Position + ImVec2(MainRadius, MainRadius), MainRadius, IM_PI / 4.f, 9.f * IM_PI / 4.f, Segments);
		DrawList->PathLineTo(Position + ImVec2(Size.x, Size.y));

		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, Color, false, StrokeSize);
		DrawList->PathClear();
	}

	void DrawErrorIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(20.f, 20.f))
	{
		constexpr float AspectRatio = 1.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float StrokeSize = 1.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->PathArcTo(Position + Size / 2.f, Size.x / 2.f, 0, 2.f * IM_PI, Size.x);
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 50, 50, Opacity), true, StrokeSize);
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.3f), Size.x / 8.f, IM_PI, 2.f * IM_PI, 8);
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.75f));
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 50, 50, Opacity));
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.75f), Size.x / 10.f, 0, 2.f * IM_PI, 8);
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 50, 50, Opacity));
		DrawList->PathClear();
	}

	void DrawInfoIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(20.f, 20.f))
	{
		static Config::CColor* ColorBase = Config::GetColor(XOR("theme-info-icon"));
		Config::CColor Color = ColorBase->ModulateAlpha(Opacity);
		ImU32 Color32 = Color;

		constexpr float AspectRatio = 1.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float StrokeSize = 1.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->PathArcTo(Position + Size / 2.f, Size.x / 2.f, 0, 2.f * IM_PI, Size.x);
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, Color32, true, StrokeSize);
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.3f), StrokeSize, 0, 2.f * IM_PI, 8);
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, Color32);
		DrawList->PathClear();

		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.4f + StrokeSize));
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.8f));
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, Color32, false, StrokeSize * 2.f);
		DrawList->PathClear();
	}

	void DrawWarningIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(20.f, 20.f))
	{
		constexpr float AspectRatio = 1.1547005383792517f; // X / Y, aspect ratio of equilateral triangle = 2/sqrt(3)
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		// C = 2*PI*radius
		float StrokeSize = 1.f;
		float CornerRadius = Size.x / 10.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->PathArcTo(Position + ImVec2(CornerRadius, Size.y - CornerRadius), CornerRadius, IM_PI / 2.f, 7.f * IM_PI / 6.f, max(CornerRadius, 4));
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, CornerRadius), CornerRadius, 7.f * IM_PI / 6.f, 11.f * IM_PI / 6.f, max(CornerRadius, 4));
		DrawList->PathArcTo(Position + ImVec2(Size.x - CornerRadius, Size.y - CornerRadius), CornerRadius, 11.f * IM_PI / 6.f, 5.f * IM_PI / 2.f, max(CornerRadius, 4));
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 0, Opacity), true, StrokeSize);
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.35f), Size.x / 10.f, IM_PI, 2.f * IM_PI, 8);
		DrawList->PathLineTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.75f));
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 0, Opacity));
		DrawList->PathClear();

		DrawList->PathArcTo(Position + ImVec2(Size.x / 2.f, Size.y * 0.8f), Size.x / 14.f, 0, 2.f * IM_PI, 8);
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 0, Opacity));
		DrawList->PathClear();
	}

	void DrawXIcon(ImU32 Color, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 1.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		float StrokeSize = Size.x / 10.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->AddLine(Position, Position + Size, Color, StrokeSize);
		DrawList->AddLine(ImVec2(Position.x, Position.y + Size.y), ImVec2(Position.x + Size.x, Position.y), Color, StrokeSize);
	}

	void DrawCheckmark(ImU32 Color, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 4.f / 3.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		float StrokeSize = Size.x / 10.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->AddLine(Position + ImVec2(Size.x, 0), Position + ImVec2(Size.x / 3.f, Size.y), Color, StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 3.f, Size.y), Position + ImVec2(0, Size.y / 2.f), Color, StrokeSize);
	}

	void DrawUpArrow(ImU32 Color, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 1.6f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		float StrokeSize = Size.x / 5.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->AddLine(Position + ImVec2(0, Size.y), Position + ImVec2(Size.x / 2.f, 0.f), Color, StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 2.f, 0.f), Position + Size, Color, StrokeSize);
	}

	void DrawDownArrow(ImU32 Color, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 1.6f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		float StrokeSize = Size.x / 5.f;
		Size -= ImVec2(StrokeSize, StrokeSize);
		Position += ImVec2(StrokeSize, StrokeSize) / 2.f;

		DrawList->AddLine(Position, Position + ImVec2(Size.x / 2.f, Size.y), Color, StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 2.f, Size.y), Position + ImVec2(Size.x, 0.f), Color, StrokeSize);
	}

	void DrawSelectionCursor(ImU32 Color, ImVec2 Dimensions = ImVec2(10.f, 10.f))
	{
		constexpr float AspectRatio = 1.f; // X / Y
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float Ratio = Dimensions.x / Dimensions.y;
		ImVec2 Size = Ratio > AspectRatio ?
			ImVec2(Dimensions.x * AspectRatio / Ratio, Dimensions.y) :
			ImVec2(Dimensions.x, Dimensions.y * Ratio / AspectRatio); // to wide ? shorten X : shorten Y
		ImVec2 Position = Window->DC.CursorPos + (Dimensions - Size) / 2;

		DrawList->PathLineTo(Position);
		DrawList->PathLineTo(Position + ImVec2(Size.x, Size.y / 2));
		DrawList->PathLineTo(Position + ImVec2(0, Size.y));
		//DrawList->PathLineTo(Position);

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, Color);
		DrawList->PathClear();
	}

	void InputTextWithPlaceholder(std::string Identifier, std::string Placeholder, char* Buffer, size_t BufferLength, ImGuiInputTextFlags Flags = 0)
	{
		bool Active = GetID((XOR("##") + Identifier).c_str()) == GetActiveID();
		bool HasContent = BufferLength && Buffer[0];
		if (!HasContent && !Active)
		{
			// draw placeholder
			auto CursorBefore = ImGui::GetCursorPos();
			ImVec4 TextColorBefore = GImGui->Style.Colors[ImGuiCol_Text];
			bool NextItemHasWidth = GImGui->NextItemData.Flags & ImGuiNextItemDataFlags_HasWidth;
			float NextItemWidth = GImGui->NextItemData.Width;

			PushStyleColor(ImGuiCol_Text, lerp(TextColorBefore, ImVec4(0, 0, 0, TextColorBefore.w), 0.5f));

			TextEx(Placeholder.c_str());

			PopStyleColor();
			ImGui::SetCursorPos(CursorBefore);
			if (NextItemHasWidth)
			{
				GImGui->NextItemData.Flags |= ImGuiNextItemDataFlags_HasWidth;
				GImGui->NextItemData.Width = NextItemWidth;
			}
		}
		InputText((XOR("##") + Identifier).c_str(), Buffer, BufferLength, Flags);
		GUI::WantKeyboard |= IsItemActive();
	}

	std::string TruncateToEllipsis(std::string Input, float MaxWidth)
	{
		float CurrentWidth = CalcTextSize(Input.c_str()).x;
		if (CurrentWidth <= MaxWidth) return Input;

		int PredictedCharCount = Input.size() * MaxWidth / CurrentWidth;
		std::string Output = Input.substr(0, PredictedCharCount);
		CurrentWidth = CalcTextSize((Output + XOR("...")).c_str()).x;

		if (CurrentWidth <= MaxWidth) // prediction was too short
		{
			int AddChars = 0;
			while (CurrentWidth <= MaxWidth)
			{
				AddChars++;
				Output = Input.substr(0, PredictedCharCount + AddChars);
				CurrentWidth = CalcTextSize((Output + XOR("...")).c_str()).x;
			}
			return Input.substr(0, PredictedCharCount + AddChars - 1) + XOR("...");
		}
		else // prediction was too long
		{
			int RemoveChars = 0;
			while (CurrentWidth > MaxWidth && (RemoveChars < PredictedCharCount))
			{
				RemoveChars++;
				Output = Input.substr(0, PredictedCharCount - RemoveChars);
				CurrentWidth = CalcTextSize((Output + XOR("...")).c_str()).x;
			}
			return Input.substr(0, PredictedCharCount - RemoveChars) + XOR("...");
		}
	}

	void ToolTip(std::string str, int ItemHeightForVerticalFlip = 20)
	{
		static Config::CColor* TooltipBackground = Config::GetColor(XOR("theme-tooltip-background"));
		static Config::CColor* TooltipText = Config::GetColor(XOR("theme-tooltip-text"));
		static Config::CColor* TooltipBorder = Config::GetColor(XOR("theme-tooltip-border"));
		static Config::CFloat* TooltipBorderSize = Config::GetFloat(XOR("theme-tooltip-border-size"));

		constexpr int MinWidth = 20;

		ImGuiIO& io = ImGui::GetIO();

		PushFont(Arial16);
		ImVec2 TextDimensions = CalcTextSize(str.c_str());
		PopFont();

		ImVec2 WindowSize = TextDimensions + ImVec2(10, 10);
		int TextOffset = 0;
		if (WindowSize.x < MinWidth)
		{
			TextOffset = (MinWidth - WindowSize.x) / 2;
			WindowSize.x = MinWidth;
		}
		ImVec2 PointerSize = ImVec2(5, 5);
		int VerticalPadding = 5;
		ItemHeightForVerticalFlip += VerticalPadding * 2;

		ImVec2 PointerPos = GetCurrentWindow()->DC.CursorPos - ImVec2(0, 5);

		// invert vertically if there is not enough space above, and there is more space below
		bool VerticalInvert = (PointerPos.y < (WindowSize.y + PointerSize.y)) && (PointerPos.y < (io.DisplaySize.y - PointerPos.y - ItemHeightForVerticalFlip));
		if (VerticalInvert)
		{
			PointerPos += ImVec2(0, ItemHeightForVerticalFlip);
		}

		ImVec2 WindowPos = PointerPos - ImVec2(WindowSize.x / 2, VerticalInvert ? (-PointerSize.y) : (WindowSize.y + PointerSize.y));

		// move horizontally
		if (WindowPos.x + WindowSize.x > io.DisplaySize.x)
		{
			WindowPos += ImVec2(io.DisplaySize.x - (WindowPos.x + WindowSize.x), 0);
			if (WindowPos.x < 0)
			{
				WindowPos -= ImVec2(WindowPos.x / 2, 0);
			}
		}

		auto DrawList = GetForegroundDrawList();

		if (!VerticalInvert)
		{
			DrawList->PathLineTo(PointerPos - PointerSize); // top left of pointer
			DrawList->PathArcTo(WindowPos + ImVec2(5, WindowSize.y - 5), 5, IM_PI / 2.f, IM_PI, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(5, 5), 5, IM_PI, 3.f * IM_PI / 2.f, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(WindowSize.x - 5, 5), 5, 3.f * IM_PI / 2.f, 2.f * IM_PI, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(WindowSize.x - 5, WindowSize.y - 5), 5, 2.f * IM_PI, 5.f * IM_PI / 2.f, 4);
			DrawList->PathLineTo(PointerPos + ImVec2(PointerSize.x, -PointerSize.y)); // top right of pointer
			DrawList->PathLineTo(PointerPos);
		}
		else
		{
			DrawList->PathLineTo(PointerPos + PointerSize); // bottom right of pointer
			DrawList->PathArcTo(WindowPos + ImVec2(WindowSize.x - 5, 5), 5, 3.f * IM_PI / 2.f, 2.f * IM_PI, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(WindowSize.x - 5, WindowSize.y - 5), 5, 2.f * IM_PI, 5.f * IM_PI / 2.f, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(5, WindowSize.y - 5), 5, IM_PI / 2.f, IM_PI, 4);
			DrawList->PathArcTo(WindowPos + ImVec2(5, 5), 5, IM_PI, 3.f * IM_PI / 2.f, 4);
			DrawList->PathLineTo(PointerPos + ImVec2(-PointerSize.x, PointerSize.y)); // bottom left of pointer
			DrawList->PathLineTo(PointerPos);
		}

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, *TooltipBackground);
		if (TooltipBorderSize->Get() > 0)
			DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, *TooltipBorder, true, TooltipBorderSize->Get());
		DrawList->PathClear();

		// Draw Text
		DrawList->AddText(Arial16, 16, WindowPos + ImVec2(5 + TextOffset, 5), *TooltipText, str.c_str());
	}

	bool DrawBooleanSwitch(std::string Identifier, ImVec4 ColorA, ImVec4 ColorB, float SwitchFactor = 0.f, ImVec2 Size = ImVec2(30, 16))
	{
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		float OuterRadius = Size.y / 2.f;
		float InnerRadius = OuterRadius - 2.f;
		ImVec2 LeftCenter = Window->DC.CursorPos + ImVec2(OuterRadius, OuterRadius);
		ImVec2 RightCenter = Window->DC.CursorPos + Size - ImVec2(OuterRadius, OuterRadius);
		ImVec2 GrabCenter = lerp(LeftCenter, RightCenter, SwitchFactor);

		DrawList->PathArcTo(LeftCenter, OuterRadius, IM_PI / 2.f, 3.f * IM_PI / 2.f, ceil(OuterRadius));
		DrawList->PathArcTo(RightCenter, OuterRadius, 3.f * IM_PI / 2.f, 5.f * IM_PI / 2.f, ceil(OuterRadius));
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, ColorConvertFloat4ToU32(lerp(ColorA, ColorB, SwitchFactor)));
		DrawList->PathClear();

		DrawList->PathArcTo(GrabCenter, InnerRadius, 0.f, 2.f * IM_PI, ceil(InnerRadius * 2.f));
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, ColorConvertFloat4ToU32(lerp(ColorB, ColorA, SwitchFactor)));
		DrawList->PathClear();

		ImGui::PushStyleColor(ImGuiCol_Button, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, OuterRadius);

		// dummy button
		bool Clicked = ImGui::Button(Identifier.c_str(), Size);

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(1);

		return Clicked;
	}

	float DrawInputFloat(std::string Identifier, Config::CColor* Background, Config::CColor* Foreground, float Factor = 0.5f, ImVec2 Size = ImVec2(100, 10), bool* Active = nullptr)
	{
		if (Active) *Active = false;
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Position = Window->DC.CursorPos + ImVec2(Size.y / 2.f, 0);
		Size.x -= Size.y;

		ImRect BB(Position, Position + Size);
		ImGuiID ID = Window->GetID(Identifier.c_str());

		// draw bar
		{
			constexpr float InactiveHeight = 5.f;
			constexpr float ActiveHeight = 7.f;
			float GrabRadius = Size.y / 2.f;
			float GrabOffset = Size.x * Factor;

			// inactive portion
			DrawList->AddRectFilled(
				Position + ImVec2(GrabOffset, (Size.y - InactiveHeight) / 2.f),
				Position + ImVec2(Size.x + InactiveHeight / 2.f, (Size.y + InactiveHeight) / 2.f),
				Background->ModulateAlpha(0.3f),
				InactiveHeight
			);

			// active portion
			DrawList->AddRectFilled(
				Position + ImVec2(-ActiveHeight / 2.f, (Size.y - ActiveHeight) / 2.f),
				Position + ImVec2(GrabOffset, (Size.y + ActiveHeight) / 2.f),
				*Background,
				ActiveHeight
			);

			// grab
			DrawList->AddCircleFilled(
				Position + ImVec2(GrabOffset, Size.y / 2.f),
				GrabRadius,
				*Foreground,
				16
			);
		}

		// behavior
		ItemAdd(BB, ID);
		if (ItemHoverable(BB, ID) && GImGui->IO.MouseClicked[0])
		{
			SetActiveID(ID, Window);
			SetFocusID(ID, Window);
			FocusWindow(Window);
		}
		if (GImGui->ActiveId == ID)
		{
			if (GImGui->ActiveIdSource == ImGuiInputSource_Mouse && GImGui->IO.MouseDown[0])
			{
				Factor = (GImGui->IO.MousePos.x - Position.x) / Size.x;
				if (Active) *Active = true;
			}
			else
			{
				ClearActiveID();
			}
		}
		if (Factor < 0.f) Factor = 0.f;
		if (Factor > 1.f) Factor = 1.f;

		return Factor;
	}
}

// Property drawers
namespace ImGui
{
	enum class PropertyIcon {
		Info = 1,
		Warning,
		Error
	};

	void DrawGeneralLabel(Config::Property* p, PropertyIcon icon = PropertyIcon::Info, std::string iconText = "Click for more info.")
	{
		auto Window = ImGui::GetCurrentWindow();
		ImVec2 Pos = GetCursorPos();

		// draw icon 
		ImVec2 IconSize(14, 14);
		{
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			switch (icon)
			{
			default:
			case PropertyIcon::Info:
				DrawInfoIcon(255, IconSize);
				break;
			case PropertyIcon::Warning:
				DrawWarningIcon(255, IconSize);
				break;
			case PropertyIcon::Error:
				DrawErrorIcon(255, IconSize);
				break;
			}

			auto ID = GetID((p->Name + XOR("-status-icon-hoverable")).c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(iconText, IconSize.y);
				if (GImGui->IO.MouseClicked[0])
					ShellExecute(0, 0, (XOR("https://www.a4g4.com/features#") + p->Name).c_str(), 0, 0, SW_HIDE);
			}
		}

		// draw label
		SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
		Text(TruncateToEllipsis(p->VisibleName, GUI::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());

		// setup for property to be drawn
		SetCursorPos(Pos + ImVec2(GUI::PropertyColumnPosition, 0));
	}

	void DrawGeneralKeybind(Config::Property* p)
	{
		static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
		static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
		static Config::CColor* ButtonBorder = Config::GetColor(XOR("theme-button-border"));
		static Config::CColor* ButtonText = Config::GetColor(XOR("theme-button-text"));
		static Config::CFloat* ButtonBorderSize = Config::GetFloat(XOR("theme-button-border-size"));

		int currentBinding = -1;
		switch (p->Type)
		{
		case Config::PropertyType::HSTATEFUL:
			currentBinding = ((Config::CHorizontalState*)p->Value)->BoundToKey;
			break;
		case Config::PropertyType::VSTATEFUL:
			currentBinding = ((Config::CVerticalState*)p->Value)->BoundToKey;
			break;
		case Config::PropertyType::FUNCTION:
			currentBinding = ((Config::CFunction*)p->Value)->BoundToKey;
			break;
		case Config::PropertyType::BOOLEAN:
			currentBinding = ((Config::CBoolean*)p->Value)->BoundToKey;
			break;
		default:
			return;
		}

		auto Window = ImGui::GetCurrentWindow();
		ImVec2 Pos = GetCursorPos();

		if (p == Config::SettingKeybindFor) // the bind is currently being set
		{
			PushFont(Arial12);
			bool HasBindModes = p->Type == Config::PropertyType::BOOLEAN && p->Name != XOR("show-menu");

			if (HasBindModes)
			{
				Config::CBoolean* Value = (Config::CBoolean*)p->Value;
				const char* Prefix = XOR("MODE:");
				ImVec2 PrefixSize = CalcTextSize(Prefix);
				constexpr int BindModeOptionWidth = 60;
				const char* Suffix = XOR("[PRESS A KEY]");
				ImVec2 SuffixSize = CalcTextSize(Suffix);

				// Prefix 
				SetCursorPos(Pos + ImVec2(0, (20 - PrefixSize.y) / 2));
				Text(Prefix);

				// [Toggle, Enable, Disable]
				PushFont(Arial14);
				PushStyleColor(ImGuiCol_Button, 0);
				PushStyleColor(ImGuiCol_ButtonHovered, 0);
				PushStyleColor(ImGuiCol_ButtonActive, 0);
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*ButtonBase);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				SetCursorPos(Pos + ImVec2(PrefixSize.x + 5, 0));
				{
					BeginChild((XOR("##bindmode-") + p->Name).c_str(), ImVec2(BindModeOptionWidth * 3, 20), true);
					{
						auto ChildWindow = GetCurrentWindow();
						auto ChildDrawList = ChildWindow->DrawList;

						// current selection
						SetCursorPos(ImVec2(BindModeOptionWidth * (int)Value->BindMode, 0));
						ChildDrawList->AddRectFilled(ChildWindow->DC.CursorPos, ChildWindow->DC.CursorPos + ImVec2(BindModeOptionWidth, 20), *ButtonActive, 0, 0);

						// other options
						for (int i = 0; i < 3; i++)
						{
							SetCursorPos(ImVec2(BindModeOptionWidth * i, 0));
							if (Button((Config::KeybindTypeNames[i] + XOR("##") + p->Name).c_str(), ImVec2(BindModeOptionWidth, 20)))
							{
								Value->BindMode = (Config::KeybindMode)i;
							}
						}
					}
					EndChild();
				}
				PopFont();
				PopStyleColor(5);
				PopStyleVar(2);

				// Suffix
				SetCursorPos(Pos + ImVec2(PrefixSize.x + 5 + BindModeOptionWidth * 3 + 5, (20 - SuffixSize.y) / 2));
				Text(Suffix);
			}
			else
			{
				SetCursorPos(Pos + ImVec2(0, (20 - 12) / 2));
				Text(XOR("[PRESS A KEY]"));
			}

			PopFont();
		}
		else if (currentBinding >= 0) // already bound (click to rebind)
		{
			PushFont(Arial12);
			const char* Prefix = XOR("PRESS");
			const char* Suffix = XOR("TO LOOP");
			std::string KeyName = Keybind::KeyNames[currentBinding];
			char __buffer[32];
			if (p->Type == Config::PropertyType::BOOLEAN)
			{
				const Config::CBoolean* Value = (Config::CBoolean*)p->Value;

				if (Value->BindMode != Config::KeybindMode::TOGGLE)
					Prefix = XOR("HOLD");
				
				Suffix =
					Value->BindMode == Config::KeybindMode::TOGGLE ? XOR("TO TOGGLE") :
					Value->BindMode == Config::KeybindMode::HOLDTOENABLE ? XOR("TO ENABLE") : XOR("TO DISABLE");
			}
			else if (p->Type == Config::PropertyType::FUNCTION)
			{
				const Config::CFunction* Value = (Config::CFunction*)p->Value;
				ZeroMemory(__buffer, 32);
				size_t i = 0;
				__buffer[i++] = 'T';
				__buffer[i++] = 'O';
				__buffer[i++] = ' ';

				for (size_t j = 0; i < 31 && j < Value->Label.length(); i++, j++)
				{
					char s = Value->Label.at(j);
					if ('a' <= s && s <= 'z')
						s -= 'a' - 'A';
					__buffer[i] = s;
				}

				Suffix = const_cast<const char*>(__buffer);
			}

			ImVec2 PrefixSize = CalcTextSize(Prefix);
			ImVec2 SuffixSize = CalcTextSize(Suffix);
			ImVec2 KeyNameSize = CalcTextSize(KeyName.c_str());

			// Prefix 
			SetCursorPos(Pos + ImVec2(0, (20 - PrefixSize.y) / 2));
			Text(Prefix);

			// [keyname]
			PushFont(Arial14);
			PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
			PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
			PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
			SetCursorPos(Pos + ImVec2(PrefixSize.x + 5, 0));
			if (Button((KeyName + XOR("##") + p->Name).c_str(), ImVec2(KeyNameSize.x + 10, 20)))
			{
				if (p->Name == XOR("show-menu"))
				{
					Config::SettingKeybindFor = p;
				}
				else
				{
					Config::_BindToKey(p, -1);
					Config::SettingKeybindFor = nullptr;
				}
			}
			if (IsItemHovered())
			{
				SetCursorPos(Pos + ImVec2(PrefixSize.x + 5 + (KeyNameSize.x + 10) / 2, 0));
				ToolTip(p->Name == XOR("show-menu") ? XOR("Click To Edit") : XOR("Click To Clear"), 20);
			}
			PopFont();
			PopStyleColor(2);
			PopStyleVar(2);

			// Suffix
			SetCursorPos(Pos + ImVec2(PrefixSize.x + 5 + KeyNameSize.x + 10 + 5, (20 - PrefixSize.y) / 2));
			Text(Suffix);

			PopFont();
		}
		else // not bound but is bindable
		{
			PushFont(Arial14);
			PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
			PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
			PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
			SetCursorPos(Pos);
			if (Button((XOR("Bind##") + p->Name).c_str(), ImVec2(40, 20)))
			{
				Config::SettingKeybindFor = p;
			}
			PopFont();
			PopStyleColor(2);
			PopStyleVar(2);
		}
	}

	int DrawLabelProperty(Config::Property* p)
	{
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(Pos + ImVec2(6, (20 - ImGui::GetFontSize()) / 2));
		ImGui::Text(p->VisibleName.c_str());
		return 20;
	}

	int DrawTextInputProperty(Config::Property* p)
	{
		static Config::CColor* TextInputBackground = Config::GetColor(XOR("theme-text-input-background"));
		static Config::CColor* TextInputText = Config::GetColor(XOR("theme-text-input-text"));
		Config::CTextInput* Value = (Config::CTextInput*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		ImVec2 InputBasePos = GetCursorPos();

		// draw input
		{
			SetCursorPos(InputBasePos);
			PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*TextInputBackground);
			PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
			BeginChild((XOR("##entry-child-") + p->Name).c_str(), ImVec2(Window->Size.x - 10 - GetCursorPosX(), 20), false);

			auto w = ImGui::GetCurrentWindow();

			PushFont(Arial14);
			bool IsTyping = ImGui::GetActiveID() == w->GetID((XOR("##entry-child-text") + p->Name).c_str());
			SetCursorPos(ImVec2(5, 3));
			SetNextItemWidth(w->Size.x - 10);
			InputText((XOR("##entry-child-text") + p->Name).c_str(), Value->Data, Value->DataSize, PremiumLocked ? ImGuiInputTextFlags_ReadOnly : 0);
			GUI::WantKeyboard |= IsItemActive();

			if (IsTyping)
			{
				Config::SettingKeybindFor = nullptr;
			}
			else if (!Value->Data || Value->DataSize == 0 || Value->Data[0] == '\0')
			{
				SetCursorPos(ImVec2(5, 3));
				PushStyleColor(ImGuiCol_Text, (ImVec4)TextInputText->ModulateAlpha(0.6f));
				Text(XOR("Type here..."));
				PopStyleColor(1);
			}
			PopFont();

			EndChild();
			PopStyleColor(1);
			PopStyleVar(1);
		}

		return 20;
	}

	int DrawBooleanProperty(Config::Property* p)
	{
		static Config::CColor* PropertyBase = Config::GetColor(XOR("theme-property-base"));
		static Config::CColor* PropertyAccent = Config::GetColor(XOR("theme-property-accent"));
		Config::CBoolean* Value = (Config::CBoolean*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		// draw switch
		double TimePassed = Value->Value.GetTimeSinceChange();
		double AnimFactor = Animation::animate(TimePassed, 0.15f);
		if (Value->Value.Get() == 0)
			AnimFactor = 1.f - AnimFactor;
		SetCursorPosY(GetCursorPosY() + 2);
		ImVec2 SwitchPos = GetCursorPos();
		bool Flipped = DrawBooleanSwitch(XOR("##") + p->Name, *PropertyBase, *PropertyAccent, AnimFactor);

		// switch behavior
		if (Flipped && !PremiumLocked)
		{
			Config::SettingKeybindFor = nullptr;
			Value->Value.Invert();
			if (Value->BoundToKey >= 0)
			{
				if (Value->BindMode == Config::KeybindMode::HOLDTODISABLE)
					Value->BindMode = Config::KeybindMode::HOLDTOENABLE;
				else if (Value->BindMode == Config::KeybindMode::HOLDTOENABLE)
					Value->BindMode = Config::KeybindMode::HOLDTODISABLE;
			}
		}

		// hover premium lock
		if (PremiumLocked && IsItemHovered())
		{
			SetCursorPos(SwitchPos + ImVec2(15, 0));
			ToolTip(PREMIUM_USERS_ONLY, 20);
		}

		// keybind
		if (!PremiumLocked && Value->Bindable)
		{
			SetCursorPos(SwitchPos + ImVec2(36, -2));
			DrawGeneralKeybind(p);
		}

		return 20;
	}

	int DrawFloatProperty(Config::Property* p)
	{
		static Config::CColor* PropertyBase = Config::GetColor(XOR("theme-property-base"));
		static Config::CColor* PropertyAccent = Config::GetColor(XOR("theme-property-accent"));
		Config::CFloat* Value = (Config::CFloat*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		// draw bar
		constexpr float SpaceAfterBar = 43.f;
		constexpr int BarHeight = 14;
		float BarLength = Window->ContentRegionRect.GetWidth() - GetCursorPosX() - SpaceAfterBar;
		ImVec2 BarBase = GetCursorPos();
		{
			SetCursorPos(BarBase + ImVec2(0, (20 - BarHeight) / 2));

			float DrawValue = Value->GetFactor();
			bool Active = false;
			float UserSetValue = DrawInputFloat(p->Name + XOR("##bar"), PropertyBase, PropertyAccent, DrawValue, ImVec2(BarLength, BarHeight), &Active);

			if (!PremiumLocked && Active)
			{
				Config::SettingKeybindFor = nullptr;
				Value->SetFactor(UserSetValue);
			}

			if (IsItemHovered() || Active)
			{
				SetCursorPos(BarBase + ImVec2((BarLength - 16.f) * DrawValue + 8.f, (20 - BarHeight) / 2));
				ToolTip(PremiumLocked ? PREMIUM_USERS_ONLY : Value->Stringify(), 20);
			}
		}

		// draw unit
		{
			PushFont(Arial12);
			SetCursorPos(BarBase + ImVec2(BarLength + 5, (20 - 12) / 2));
			TextEx(Value->Unit.c_str());
			PopFont();
		}

		return 20;
	}

	int DrawColorProperty(Config::Property* p)
	{
		static Config::CColor* EditorBase = Config::GetColor(XOR("theme-color-editor-background"));
		static Config::CColor* EditorText = Config::GetColor(XOR("theme-color-editor-text"));
		static Config::CColor* EditorBorder = Config::GetColor(XOR("theme-color-editor-border"));
		static Config::CFloat* EditorBorderSize = Config::GetFloat(XOR("theme-color-editor-border-size"));

		Config::CColor* Value = (Config::CColor*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		ImVec2 ColorButtonBase = GetCursorPos();

		// draw color
		{
			ImVec4 ImColor = *Value;

			if (((int)Value->GetR() + (int)Value->GetG() + (int)Value->GetB()) < 127 * 3) // this is a dark color
				PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 127));
			else // this is a bright color
				PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 127));
			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			if (ColorButton((XOR("##color-button-") + p->Name).c_str(), ImColor, ImGuiColorEditFlags_NoTooltip, ImVec2(40, 20)) && !PremiumLocked)
				OpenPopup((XOR("##color-picker-") + p->Name).c_str());
			else if (IsItemHovered())
			{
				SetCursorPos(ColorButtonBase + ImVec2(20, 0));
				ToolTip(PremiumLocked ? PREMIUM_USERS_ONLY : Value->Stringify(), 20);
			}
			PopStyleVar(2);
			PopStyleColor(1);
			PushStyleColor(ImGuiCol_PopupBg, (ImVec4)*EditorBase);
			PushStyleColor(ImGuiCol_Text, (ImVec4)*EditorText);
			PushStyleColor(ImGuiCol_Border, (ImVec4)*EditorBorder);
			PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)EditorText->ModulateAlpha(0.15f));
			PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)EditorText->ModulateAlpha(0.3f));
			PushStyleVar(ImGuiStyleVar_PopupBorderSize, EditorBorderSize->Get());
			SetNextWindowSize(ImVec2(210, Value->GetHasAlpha() ? 235 : 255));
			if (BeginPopup((XOR("##color-picker-") + p->Name).c_str()))
			{
				Config::SettingKeybindFor = nullptr;

				PushFont(Arial16);
				std::string Title = TruncateToEllipsis(p->VisibleName, 210 - 20);
				ImVec2 Size = CalcTextSize(Title.c_str());
				SetCursorPos(ImVec2(210 / 2, 13) - Size / 2);
				Text(Title.c_str());
				PopFont();

				SetCursorPos(ImVec2(5, 30));
				SetNextItemWidth(200);
				if (Value->GetHasAlpha())
				{
					ColorPicker4((XOR("##color-editor-") + p->Name).c_str(), &ImColor.x,
						ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel |
						ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV
					);
				}
				else
				{
					ColorPicker3((XOR("##color-editor-") + p->Name).c_str(), &ImColor.x,
						ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSmallPreview |
						ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV
					);
				}

				Value->SetR((unsigned char)((int)(ImColor.x * 255.f + 0.5f) % 256));
				Value->SetG((unsigned char)((int)(ImColor.y * 255.f + 0.5f) % 256));
				Value->SetB((unsigned char)((int)(ImColor.z * 255.f + 0.5f) % 256));
				Value->SetA((unsigned char)((int)(ImColor.w * 255.f + 0.5f) % 256));

				EndPopup();
			}
			PopStyleColor(5);
			PopStyleVar(1);
		}

		return 20;
	}

	int DrawVerticalStatefulProperty(Config::Property* p)
	{
		static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
		static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
		static Config::CColor* ButtonHovered = Config::GetColor(XOR("theme-button-hovered"));
		static Config::CColor* ButtonBorder = Config::GetColor(XOR("theme-button-border"));
		static Config::CColor* ButtonText = Config::GetColor(XOR("theme-button-text"));
		static Config::CColor* DropdownBase = Config::GetColor(XOR("theme-dropdown-background"));
		static Config::CColor* DropdownText = Config::GetColor(XOR("theme-dropdown-text"));
		static Config::CColor* DropdownBorder = Config::GetColor(XOR("theme-dropdown-border"));
		static Config::CFloat* DropdownBorderSize = Config::GetFloat(XOR("theme-dropdown-border-size"));
		static Config::CFloat* ButtonBorderSize = Config::GetFloat(XOR("theme-button-border-size"));
		Config::CVerticalState* Value = (Config::CVerticalState*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		ImVec2 DropdownPos = GetCursorPos();

		PushFont(Arial14);
		PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		PushStyleColor(ImGuiCol_Text, (ImVec4)*DropdownText);
		// draw property
		{
			bool alreadyOpen = IsPopupOpen((XOR("##popup-") + p->Name).c_str());
			int nItems = Value->StateNames.size();
			std::string CurrentSelection = Value->StateNames.at(Value->Value.Get());

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonActive, 0);
			PushStyleColor(ImGuiCol_ButtonHovered, 0);
			// button/child thing
			{
				bool open = false;
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*DropdownBase);
				PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
				BeginChild((XOR("##button-child-") + p->Name).c_str(), ImVec2(200, 20));

				// fuck you imgui, please let me just use ImGuiStyleVar_ChildBorderSize
				// draw border manually
				auto ChildWindow = GetCurrentWindow();
				auto ChildDrawList = Window->DrawList;
				if (DropdownBorderSize->Get() > 0.f)
					ChildDrawList->AddRect(ChildWindow->Pos, ChildWindow->Pos + ChildWindow->Size, *DropdownBorder, 3.f, ImDrawCornerFlags_All, DropdownBorderSize->Get());

				SetCursorPos(ImVec2(5, 3));
				Text(CurrentSelection.c_str());

				// dropdown arrow
				{
					SetCursorPos(ImVec2(200 - 15, 5));
					if (alreadyOpen)
						DrawUpArrow(*DropdownText, ImVec2(9, 9));
					else
						DrawDownArrow(*DropdownText, ImVec2(9, 9));
				}

				// dummy button across whole child
				{
					SetCursorPos(ImVec2(0, 0));
					if (Button((XOR("##button-invis-") + p->Name).c_str(), ImVec2(200, 20)) && !PremiumLocked)
					{
						Config::SettingKeybindFor = nullptr;
						open = true;
					}
				}
				bool HoveringOverOpener = IsItemHovered();

				EndChild();
				PopStyleColor(1);
				PopStyleVar(2);

				if (PremiumLocked && HoveringOverOpener)
				{
					SetCursorPos(DropdownPos + ImVec2(100, 0));
					ToolTip(PREMIUM_USERS_ONLY, 20);
				}

				if (open)
					OpenPopup((XOR("##popup-") + p->Name).c_str());
			}
			PopStyleColor(3);

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)DropdownText->ModulateAlpha(0.1f));
			PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)DropdownText->ModulateAlpha(0.2f));
			// popup
			{
				SetCursorPos(DropdownPos);
				SetNextWindowPos(ImVec2(Window->DC.CursorPos + ImVec2(0, 25)));
				SetNextWindowSize(ImVec2(200, min(nItems, 10) * 20));
				PushStyleColor(ImGuiCol_Border, (ImVec4)*DropdownBorder);
				PushStyleColor(ImGuiCol_PopupBg, (ImVec4)*DropdownBase);
				PushStyleVar(ImGuiStyleVar_PopupBorderSize, DropdownBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
				if (BeginPopup((XOR("##popup-") + p->Name).c_str()))
				{
					Config::SettingKeybindFor = nullptr;
					for (size_t i = 0; i < Value->StateNames.size(); i++)
					{

						SetCursorPos(ImVec2(0, i * 20));
						if (Button((XOR("##li") + p->Name + Value->StateNames.at(i)).c_str(), ImVec2(200, 20)))
						{
							Value->Value.Set(i);
							CloseCurrentPopup();
						}

						SetCursorPos(ImVec2(5, i * 20 + (20 - GetFontSize()) / 2));
						Text(Value->StateNames.at(i).c_str());
					}
					EndPopup();
				}
				PopStyleColor(2);
				PopStyleVar(2);
			}
			PopStyleColor(3);
		}
		PopFont();
		PopStyleVar(1);
		PopStyleColor(1);

		// keybind
		if (!PremiumLocked && Value->Bindable)
		{
			SetCursorPos(DropdownPos + ImVec2(205, 0));
			DrawGeneralKeybind(p);
		}

		return 20;
	}

	int DrawHorizontalStatefulProperty(Config::Property* p)
	{
		static Config::CColor* HStateBackground = Config::GetColor(XOR("theme-hstate-background"));
		static Config::CColor* HStateHighlight = Config::GetColor(XOR("theme-hstate-highlight"));
		static Config::CColor* HStateText = Config::GetColor(XOR("theme-hstate-text"));
		Config::CHorizontalState* Value = (Config::CHorizontalState*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (p->VisibleName != "")
		{
			if (PremiumLocked)
				DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
			else if (MasterLocked)
				DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
			else
			{
				std::string w = p->GetWarning();
				if (w == "")
					DrawGeneralLabel(p);
				else
					DrawGeneralLabel(p, PropertyIcon::Warning, w);
			}
		}

		// setup bar position
		ImVec2 BarPosition = GetCursorPos();
		int BarWidth = 0;
		if (p->VisibleName != "")
		{
			if (Value->PixelWidth > 0)
			{
				BarWidth = Value->PixelWidth;
			}
			else
			{
				BarWidth = GetContentRegionMaxAbs().x - Window->DC.CursorPos.x - 6;
			}
		}
		else
		{
			BarWidth = GetContentRegionMaxAbs().x - Window->DC.CursorPos.x - 12;
			BarPosition += ImVec2(6, 0);
		}

		SetCursorPos(BarPosition);
		ImVec2 Size(BarWidth, 20);
		ImRect BB(Window->DC.CursorPos, Window->DC.CursorPos + Size);
		ImGuiID ID = GetID((p->Name + XOR("-interactable")).c_str());

		// vars
		size_t nItems = Value->StateNames.size();
		float pixelsPerItem = (float)(BarWidth - 4) / (float)nItems;
		int itemsStartX = Window->DC.CursorPos.x + 2;
		int itemsEndX = Window->DC.CursorPos.x + BarWidth - 2;

		// interaction behavior
		{
			ItemAdd(BB, ID);
			bool hovered = ItemHoverable(BB, ID);
			bool active = GImGui->ActiveId == ID;

			if (hovered && GImGui->IO.MouseClicked[ImGuiMouseButton_Left] && !PremiumLocked)
			{
				SetActiveID(ID, Window);
				active = true;
			}

			if (active)
			{
				if (!GImGui->IO.MouseDown[ImGuiMouseButton_Left] || GImGui->ActiveIdSource != ImGuiInputSource_Mouse || PremiumLocked)
				{
					ClearActiveID();
					active = false;
				}
				else
				{
					Config::SettingKeybindFor = nullptr;

					float MouseX = GImGui->IO.MousePos.x;
					float DX = MouseX - itemsStartX;
					if (DX < 0) DX = 0;
					else if (DX > (BarWidth - 4)) DX = BarWidth - 4;

					size_t ClickedIndex = (size_t)(DX / pixelsPerItem);
					Value->Value.Set(ClickedIndex);
				}
			}

			if (hovered && PremiumLocked)
			{
				SetCursorPos(BarPosition + ImVec2(BarWidth / 2, 0));
				ToolTip(PREMIUM_USERS_ONLY, 20);
				SetCursorPos(BarPosition);
			}
		}

		// draw
		{
			// figure out where highlight should go
			float GoalX = itemsStartX + Value->Value.Get() * pixelsPerItem;
			float LastX = itemsStartX + Value->Value.GetLastValue() * pixelsPerItem;
			float CurrentX = Animation::lerp(LastX, GoalX, Animation::animate(Value->Value.GetTimeSinceChange(), 0.1));
			ImVec2 Highlight(CurrentX, BB.Min.y + 2);

			// background
			DrawList->AddRectFilled(BB.Min, BB.Max, *HStateBackground, 10.f);
			DrawList->AddRectFilled(Highlight, Highlight + ImVec2(pixelsPerItem, 16), *HStateHighlight, 10.f);

			PushStyleColor(ImGuiCol_Text, (ImVec4)*HStateText);
			PushFont(Arial14);
			for (size_t i = 0; i < nItems; i++)
			{
				ImVec2 tsize = CalcTextSize(Value->StateNames.at(i).c_str());
				SetCursorPos(BarPosition + ImVec2(2.f + pixelsPerItem * i + (pixelsPerItem - tsize.x) / 2.f, (Size.y - tsize.y) / 2.f));
				TextEx(Value->StateNames.at(i).c_str());
			}
			PopFont();
			PopStyleColor(1);
		}

		//keybind
		if (Value->Bindable && !PremiumLocked)
		{
			SetCursorPos(BarPosition + ImVec2(BarWidth + 10, 0));
			DrawGeneralKeybind(p);
		}

		return 20;
	}

	int DrawMultiSelectorProperty(Config::Property* p)
	{
		static Config::CColor* DropdownBase = Config::GetColor(XOR("theme-dropdown-background"));
		static Config::CColor* DropdownText = Config::GetColor(XOR("theme-dropdown-text"));
		static Config::CColor* DropdownBorder = Config::GetColor(XOR("theme-dropdown-border"));
		static Config::CFloat* DropdownBorderSize = Config::GetFloat(XOR("theme-dropdown-border-size"));
		Config::CMultiSelect* Value = (Config::CMultiSelect*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		ImVec2 DropdownPos = GetCursorPos();

		PushFont(Arial14);
		PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		PushStyleColor(ImGuiCol_Text, (ImVec4)*DropdownText);
		// draw property
		{
			bool alreadyOpen = IsPopupOpen((XOR("##popup-") + p->Name).c_str());
			int nItems = Value->StateNames.size();
			std::string CurrentSelection = std::to_string(Value->CountSelected()) + XOR(" selected");

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonActive, 0);
			PushStyleColor(ImGuiCol_ButtonHovered, 0);
			// button/child thing
			{
				bool open = false;
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*DropdownBase);
				PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
				BeginChild((XOR("##button-child-") + p->Name).c_str(), ImVec2(200, 20));

				// fuck you imgui, please let me just use ImGuiStyleVar_ChildBorderSize
				// draw border manually
				auto ChildWindow = GetCurrentWindow();
				auto ChildDrawList = Window->DrawList;
				if (DropdownBorderSize->Get() > 0.f)
					ChildDrawList->AddRect(ChildWindow->Pos, ChildWindow->Pos + ChildWindow->Size, *DropdownBorder, 3.f, ImDrawCornerFlags_All, DropdownBorderSize->Get());

				SetCursorPos(ImVec2(5, 3));
				Text(CurrentSelection.c_str());

				// dropdown arrow
				{
					SetCursorPos(ImVec2(200 - 15, 5));
					if (alreadyOpen)
						DrawUpArrow(*DropdownText, ImVec2(9, 9));
					else
						DrawDownArrow(*DropdownText, ImVec2(9, 9));
				}

				// dummy button across whole child
				{
					SetCursorPos(ImVec2(0, 0));
					if (Button((XOR("##button-invis-") + p->Name).c_str(), ImVec2(200, 20)) && !PremiumLocked)
					{
						Config::SettingKeybindFor = nullptr;
						open = true;
					}
				}
				bool HoveringOverOpener = IsItemHovered();

				EndChild();
				PopStyleColor(1);
				PopStyleVar(2);

				if (PremiumLocked && HoveringOverOpener)
				{
					SetCursorPos(DropdownPos + ImVec2(100, 0));
					ToolTip(PREMIUM_USERS_ONLY, 20);
				}

				if (open)
					OpenPopup((XOR("##popup-") + p->Name).c_str());
			}
			PopStyleColor(3);

			// popup
			{
				ImVec4 HoverColor = DropdownText->ModulateAlpha(0.1f);
				ImVec4 ActiveColor = DropdownText->ModulateAlpha(0.2f);

				SetCursorPos(DropdownPos);
				SetNextWindowPos(ImVec2(Window->DC.CursorPos + ImVec2(0, 25)));
				SetNextWindowSize(ImVec2(200, min(nItems, 10) * 20));
				PushStyleColor(ImGuiCol_Border, (ImVec4)*DropdownBorder);
				PushStyleColor(ImGuiCol_PopupBg, (ImVec4)*DropdownBase);
				PushStyleVar(ImGuiStyleVar_PopupBorderSize, DropdownBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
				if (BeginPopup((XOR("##popup-") + p->Name).c_str()))
				{
					Config::SettingKeybindFor = nullptr;
					for (size_t i = 0; i < Value->StateNames.size(); i++)
					{
						bool selected = Value->Get(i);
						if (selected)
						{
							PushStyleColor(ImGuiCol_Button, ActiveColor);
							PushStyleColor(ImGuiCol_ButtonHovered, ActiveColor);
							PushStyleColor(ImGuiCol_ButtonActive, HoverColor);
						}
						else
						{
							PushStyleColor(ImGuiCol_Button, 0);
							PushStyleColor(ImGuiCol_ButtonHovered, HoverColor);
							PushStyleColor(ImGuiCol_ButtonActive, ActiveColor);
						}

						SetCursorPos(ImVec2(0, i * 20));
						if (Button((XOR("##li") + p->Name + Value->StateNames.at(i)).c_str(), ImVec2(200, 20)))
						{
							Value->Set(i, !selected);
						}
						PopStyleColor(3);

						SetCursorPos(ImVec2(20, i * 20 + 3));
						Text(Value->StateNames.at(i).c_str());

						SetCursorPos(ImVec2(5, i * 20 + 5));
						if (selected)
							DrawCheckmark(*DropdownText, ImVec2(9, 9));
					}
					EndPopup();
				}
				PopStyleColor(2);
				PopStyleVar(2);
			}
		}
		PopFont();
		PopStyleVar(1);
		PopStyleColor(1);

		return 20;
	}

	int DrawFunctionProperty(Config::Property* p)
	{
		static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
		static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
		static Config::CColor* ButtonBorder = Config::GetColor(XOR("theme-button-border"));
		static Config::CColor* ButtonText = Config::GetColor(XOR("theme-button-text"));
		static Config::CFloat* ButtonBorderSize = Config::GetFloat(XOR("theme-button-border-size"));
		Config::CFunction* Value = (Config::CFunction*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;

		// draw label
		bool PremiumLocked = p->IsPremium && !UserData::Premium;
		bool MasterLocked = p->Master && p->Master->Type == Config::PropertyType::BOOLEAN && !((Config::CBoolean*)p->Master->Value)->Value.Get();
		if (PremiumLocked)
			DrawGeneralLabel(p, PropertyIcon::Error, PREMIUM_USERS_ONLY);
		else if (MasterLocked)
			DrawGeneralLabel(p, PropertyIcon::Warning, XOR("This property has no effect because it is controlled by ") + p->Master->VisibleName + XOR("."));
		else
		{
			std::string w = p->GetWarning();
			if (w == "")
				DrawGeneralLabel(p);
			else
				DrawGeneralLabel(p, PropertyIcon::Warning, w);
		}

		// draw manual button
		ImVec2 Pos = GetCursorPos();
		if (Value->Label != "")
		{
			PushFont(Arial14);

			ImVec2 LabelSize = CalcTextSize(Value->Label.c_str());
			PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
			PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
			PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
			if (PremiumLocked) PushItemFlag(ImGuiItemFlags_Disabled, true);
			if (Button((Value->Label + XOR("##manual-") + p->Name).c_str(), ImVec2(LabelSize.x + 10, 20)) && Value->Callback)
			{
				Value->Callback();
			}
			if (PremiumLocked) PopItemFlag();
			PopStyleColor(2);
			PopStyleVar(2);

			PopFont();


			if (PremiumLocked && IsItemHovered())
			{
				SetCursorPos(Pos + ImVec2(LabelSize.x / 2 + 5, 0));
				ToolTip(PREMIUM_USERS_ONLY, 20);
			}

			Pos += ImVec2(LabelSize.x + 10 + 5, 0);
		}

		// keybind
		if (!PremiumLocked && Value->Bindable)
		{
			SetCursorPos(Pos);
			DrawGeneralKeybind(p);
		}

		return 20;
	}
}

void RealSetWindowDimensions(ImGuiWindow* window, ImVec2 position, ImVec2 size)
{
	ImVec2 pdelta = position - window->Pos;
	ImVec2 sdelta = size - window->SizeFull;

	ImGui::SetWindowPos(window, position);
	ImGui::SetWindowSize(window, size);

	ImGuiContext* GImGui = ImGui::GetCurrentContext();
	if (GImGui->MovingWindow == window)
		GImGui->ActiveIdClickOffset -= pdelta;
	else if (window->ResizeBorderHeld)
		GImGui->ActiveIdClickOffset -= sdelta;
}

void GUI::ClampToScreen()
{
	if (!MainWindow) return;
	static float lx = MainWindow->Pos.x;
	static float ly = MainWindow->Pos.y;
	float x = MainWindow->Pos.x;
	float y = MainWindow->Pos.y;
	float sx = MainWindow->SizeFull.x;
	float sy = MainWindow->SizeFull.y;
	ImVec2 screen = ImGui::GetIO().DisplaySize;


	bool moved = x != lx || y != ly;
	bool offScreen = x < 0 || y < 0 || x + sx > screen.x || y + sy > screen.y;

	if (offScreen)
	{
		if (moved)
		{
			if (sx <= screen.x)
			{
				// clamp to screen
				x = std::clamp(x, 0.f, screen.x - sx);
			}
			else
			{
				// resize and place on left
				x = 0.f;
				sx = max(screen.x, MinMenuSize.x);
			}

			if (sy <= screen.y)
			{
				// clamp to screen
				y = std::clamp(y, 0.f, screen.y - sy);
			}
			else
			{
				// resize and place on top
				y = 0.f;
				sy = max(screen.y, MinMenuSize.y);
			}
		}
		else
		{
			if (x < 0)
			{
				// decrease size and move right
				sx = std::clamp(sx + x, MinMenuSize.x, screen.x);
				x = 0.f;
			}
			else if (x + sx > screen.x)
			{
				// decrease size and move left
				sx = std::clamp(screen.x - x, MinMenuSize.x, screen.x);
				x = screen.x - sx;
			}

			if (y < 0)
			{
				// decrease size and move down
				sy = std::clamp(sy + y, MinMenuSize.y, screen.y);
				y = 0.f;
			}
			else if (y + sy > screen.y)
			{
				// decrease size and move up
				sy = std::clamp(screen.y - y, MinMenuSize.y, screen.y);
				y = screen.y - sy;
			}
		}

		// update gui with new params
		RealSetWindowDimensions(MainWindow, ImVec2(x,y), ImVec2(sx, sy));
	}

	lx = x;
	ly = y;
}

void GUI::LoadingScreen()
{
	// animation
	static auto LastTime = Animation::now();
	auto Now = Animation::now();
	double TimeDelta = Animation::delta(Now, LastTime);
	LastTime = Now;

	{
		float Milliseconds = TimeDelta * 1000.f;
		VisibleLoadProgress = min(VisibleLoadProgress + 0.01f * Milliseconds / 10.f, LoadProgress);
	}

	if (!IntroAnimation && VisibleLoadProgress >= 1.f)
	{
		IntroAnimation = Animation::newAnimation(XOR("intro-animation"), 0);
		IntroAnimation->changed = Now;
	}

	float MovementFactor = 0.f;
	float OpacityFactor = 0.f;
	if (IntroAnimation)
	{
		double t = Animation::delta(Now, IntroAnimation->changed);
		if (IntroAnimation->state == 0)
		{
			MovementFactor = Animation::animate(t, 0.5f, Animation::Interpolation::easeInOutQuint);
			OpacityFactor = 0.f;
		}
		else
		{
			MovementFactor = 1.f;
			OpacityFactor = Animation::animate(t, 0.25f);
		}
	}
	unsigned char ThisContentOpacity = 255 - (MovementFactor * 255);
	unsigned char ThisOpacity = IntroAnimation ? (IntroAnimation->state == 0 ? 255 : 0) : 255;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 WindowCenter(io.DisplaySize.x / 2, io.DisplaySize.y / 2);
	ImVec2 FrameSize(ImGui::lerp(200 + 10, 300 + 10, MovementFactor), ImGui::lerp(200 + 10, 288 + 10, MovementFactor));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(40, 40, 40, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(40, 40, 40, 0));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, ThisContentOpacity));
	ImGui::PushFont(Arial18Italics);

	ImGui::SetNextWindowPos(WindowCenter - FrameSize / 2, ImGuiCond_Always);
	ImGui::SetNextWindowSize(FrameSize, ImGuiCond_Always);
	ImGui::Begin(XOR("##LMAOBRUUHHHHHHHH"), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove);

	auto DrawList = ImGui::GetWindowDrawList();

	ImU32 BgColor = ImGui::ColorConvertFloat4ToU32(ImGui::lerp(
		ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, ThisOpacity / 255.f),
		ImVec4(53.f / 255.f, 54.f / 255.f, 58.f / 255.f, ThisOpacity / 255.f),
		MovementFactor
	));

	// draw real background because imgui borders do not have enough segments
	if (MovementFactor == 0.f)
		DrawList->AddCircleFilled(WindowCenter, 100.f, BgColor, 64); // C = 2*100*pi = 628
	else
	{
		float radius = ImGui::lerp(100, 10, MovementFactor);
		ImVec2 a = WindowCenter - (FrameSize - ImVec2(10, 10)) / 2 + ImVec2(radius, radius);
		ImVec2 b = WindowCenter + (FrameSize - ImVec2(10, 10)) / 2 - ImVec2(radius, radius);
		// tl, tr, br, bl
		DrawList->PathArcTo(a, radius, -IM_PI, -IM_PI / 2, 16);
		DrawList->PathArcTo(ImVec2(b.x, a.y), radius, -IM_PI / 2, 0, 16);
		DrawList->PathArcTo(b, radius, 0, IM_PI / 2, 16);
		DrawList->PathArcTo(ImVec2(a.x, b.y), radius, IM_PI / 2, IM_PI, 16);
		DrawList->PathFillConvex(BgColor);
	}

	// draw progress
	if (MovementFactor == 0.f)
	{
		DrawList->PathArcTo(WindowCenter, 95.f, -IM_PI / 2, -IM_PI / 2 - VisibleLoadProgress * 2 * IM_PI, 64);
		DrawList->PathStroke(IM_COL32(255, 255, 255, ThisOpacity), false, 2.f);
	}
	else
	{
		float offset = ImGui::lerp(20, 11, MovementFactor);
		float radius = ImGui::lerp(95.f, 10.f, MovementFactor);
		ImVec2 a = WindowCenter - (FrameSize - ImVec2(offset, offset)) / 2 + ImVec2(radius, radius);
		ImVec2 b = WindowCenter + (FrameSize - ImVec2(offset, offset)) / 2 - ImVec2(radius, radius);
		// tl, tr, br, bl
		DrawList->PathArcTo(a, radius, -IM_PI, -IM_PI / 2, 16);
		DrawList->PathArcTo(ImVec2(b.x, a.y), radius, -IM_PI / 2, 0, 16);
		DrawList->PathArcTo(b, radius, 0, IM_PI / 2, 16);
		DrawList->PathArcTo(ImVec2(a.x, b.y), radius, IM_PI / 2, IM_PI, 16);
		DrawList->PathStroke(IM_COL32(255, 255, 255, ThisOpacity), true, 2.f);
	}

	// draw logo
	{
		ImVec2 Size = FrameSize / 2.f;
		ImVec2 Position = WindowCenter - Size / 2.f;
		Position -= FrameSize * ImVec2(0, 0.05f);
		ImGui::DrawTeardrop(Position, Size, ThisContentOpacity);
	}

	std::string txt = std::to_string((int)(VisibleLoadProgress * 100.f + 0.5f)) + XOR("%");
	ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
	ImGui::SetCursorPos(ImVec2(FrameSize.x / 2 - txtSize.x / 2, FrameSize.y * 0.8f));
	ImGui::TextEx(txt.c_str());

	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	if (IntroAnimation && IntroAnimation->state == 1)
		AuthenticationScreen(OpacityFactor);

	if (OpacityFactor >= 1.f && MovementFactor >= 1.f)
	{
		VisibleLoadProgress = 1.01f;
	}
	else if (MovementFactor >= 1.f && IntroAnimation->state == 0)
	{
		IntroAnimation->state = 1;
		IntroAnimation->changed = Now;
	}
}

void GUI::AuthenticationScreen(float ContentOpacity)
{
	static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
	static Config::CColor* ButtonHovered = Config::GetColor(XOR("theme-button-hovered"));
	static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
	static Config::CColor* ButtonBorder = Config::GetColor(XOR("theme-button-border"));
	static Config::CColor* ButtonText = Config::GetColor(XOR("theme-button-text"));
	static Config::CFloat* ButtonBorderSize = Config::GetFloat(XOR("theme-button-border-size"));
	// TODO: set background color + outline color to config theme

	unsigned char ThisContentOpacity = ContentOpacity * 255;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 WindowCenter(io.DisplaySize.x / 2, io.DisplaySize.y / 2);
	ImVec2 FrameSize(300, 288);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(53, 54, 58, 255));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(53, 54, 58, ThisContentOpacity));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, ThisContentOpacity));
	ImGui::PushFont(Arial18Italics);


	ImGui::SetNextWindowPos(WindowCenter - FrameSize / 2, ImGuiCond_Always);
	ImGui::SetNextWindowSize(FrameSize, ImGuiCond_Always);
	ImGui::Begin(XOR("##Authentication"), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	// login errors
	{
		if (UserData::LoginError.length() > 0 && Animation::delta(Animation::now(), UserData::LoginErrorTime) < 10.0)
		{
			ImGui::SetCursorPos(ImVec2(FrameSize.x / 2, 0));
			ImGui::ToolTip(UserData::LoginError, FrameSize.y);
		}
	}

	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;

	ImGui::PushFont(Arial18);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, 0);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, 0);

	int InputPadding = FrameSize.x * 0.075f;

	// email
	static char Email[512];
	{
		ImVec2 Position(InputPadding, InputPadding);

		ImGui::SetCursorPos(Position);
		ImGui::DrawAccountIcon(ThisContentOpacity);

		ImGui::SetCursorPos(Position + ImVec2(30, (26 - 18) / 2));
		ImGui::SetNextItemWidth(FrameSize.x - ImGui::GetCursorPosX() - InputPadding);
		ImGui::InputTextWithPlaceholder(XOR("AccountEmail"), XOR("Email"), Email, 512);

		DrawList->AddLine(Window->Pos + Position + ImVec2(0, 29), Window->Pos + ImVec2(FrameSize.x - InputPadding, Position.y + 29), IM_COL32(175, 175, 175, ThisContentOpacity));
	}

	// password
	static char Password[512];
	{
		ImVec2 Position(InputPadding, InputPadding + 38);

		ImGui::SetCursorPos(Position);
		ImGui::DrawPasswordIcon(ThisContentOpacity);

		ImGui::SetCursorPos(Position + ImVec2(30, (26 - 18) / 2));
		ImGui::SetNextItemWidth(FrameSize.x - ImGui::GetCursorPosX() - InputPadding);
		ImGui::InputTextWithPlaceholder(XOR("AccountPassword"), XOR("Password"), Password, 512, ImGuiInputTextFlags_Password);

		DrawList->AddLine(Window->Pos + Position + ImVec2(0, 29), Window->Pos + ImVec2(FrameSize.x - InputPadding, Position.y + 29), IM_COL32(175, 175, 175, ThisContentOpacity));
	}

	// fill Email and Password with UserData::CredentialsFile
	static bool EmailPassInit = false;
	if (!EmailPassInit)
	{
		EmailPassInit = true;
		ZeroMemory(Email, 512);
		ZeroMemory(Password, 512);

		auto f = std::ifstream(UserData::CredentialsFile, std::ios::binary);
		if (f.is_open()) // otherwise the file does not exist, oh well
		{
			char file[1025]; // 1 byte for xor key, up to 512 for email, up to 512 for pass
			ZeroMemory(file, 1025);

			f.read(file, 1025);

			char k = file[0];
			for (size_t i = 1; i < 1025; i++)
				file[i] ^= k;

			size_t EmailLen = strnlen_s(file + 1, 511) + 1;
			size_t PassLen = strnlen_s(file + 1 + EmailLen, 511) + 1;

			memcpy(Email, file + 1, EmailLen);
			memcpy(Password, file + 1 + EmailLen, PassLen);
		}
	}

	ImGui::PopStyleColor(3);

	// button colors
	if (ContentOpacity < 1.f)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ButtonText->ModulateAlpha(ThisContentOpacity));
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ButtonBase->ModulateAlpha(ThisContentOpacity));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ButtonHovered->ModulateAlpha(ThisContentOpacity));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ButtonActive->ModulateAlpha(ThisContentOpacity));
		ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ButtonBorder->ModulateAlpha(ThisContentOpacity));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)*ButtonBase);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonActive);
		ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	// buttons
	{
		int XPos = InputPadding;
		int YPos = 88 + InputPadding;
		int ButtonWidth = FrameSize.x - XPos - XPos;
		int ButtonHeight = 30;
		int ButtonSpacing = 12;

		ImGui::SetCursorPos(ImVec2(XPos, YPos));
		// todo: show loading spinner instead of button when UserData::BusyAttemptingLogin 
		if (ImGui::Button(UserData::LoginDebounce ? XOR("---") : XOR("Log in"), ImVec2(ButtonWidth, 30)) && !IntroAnimation2 && !UserData::LoginDebounce)
		{
			UserData::LoginInformation* info = new UserData::LoginInformation{};
			info->Email = std::string(Email);
			info->Password = std::string(Password);
			CreateThread(NULL, 0, UserData::AttemptLogin, (LPVOID)info, 0, NULL);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 1));
		if (ImGui::Button(XOR("Register Online"), ImVec2(ButtonWidth, 30)))
		{
			ShellExecute(0, 0, XOR("http://a4g4.com/register/"), 0, 0, SW_SHOW);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 2));
		if (ImGui::Button(UserData::LoginDebounce ? XOR("---") : XOR("Play Free"), ImVec2(ButtonWidth, 30)) && !IntroAnimation2 && !UserData::LoginDebounce)
		{
			CreateThread(NULL, 0, UserData::GetUnauthenticatedSession, (LPVOID)nullptr, 0, NULL);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 3));
		if (ImGui::Button(XOR("Cancel / Eject"), ImVec2(ButtonWidth, 30)))
			Ejected = true;
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(5);

	ImGui::PopFont();
	ImGui::End();
	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(4);
	ImGui::PopFont();
}

void GUI::AuthenticationIntro()
{
	// 0 = this is the first ever call of this function
	// 1 = tweening opacity out
	// 2 = tweening size
	// 3 = tweening opacity in
	// 69 = done
	if (!(IntroAnimation2->state == 1 || IntroAnimation2->state == 2 || IntroAnimation2->state == 3))
		Animation::changed(IntroAnimation2, 1);

	double age = Animation::age(IntroAnimation2);
	switch (IntroAnimation2->state)
	{
	default:
	case 1:
		AuthenticationScreen(1.f - Animation::animate(age, 0.15f, Animation::Interpolation::linear));
		if (age >= 0.15f)
			Animation::changed(IntroAnimation2, 2);
		break;
	case 2:
	{
		ImGuiIO& io = ImGui::GetIO();
		float f = Animation::animate(age, .35f, Animation::Interpolation::easeInOutQuart);

		ImVec2 Size = ImGui::lerp(ImVec2(300, 288), DefaultMenuSize, f);
		ImVec4 Color = ImGui::lerp(ImVec4(53.f / 255.f, 54.f / 255.f, 58.f / 255.f, 1.f), ImVec4(30.f / 255.f, 30.f / 255.f, 30.f / 255.f, 1.f), f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, Color);
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, floor(ImGui::lerp(2.f, 0.f, f) + 0.5f));

		ImGui::SetNextWindowSize(Size, ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::Begin(XOR("##intro-2"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
		ImGui::End();

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(1);

		if (age >= .35f)
			Animation::changed(IntroAnimation2, 3);
	}
	break;
	case 3:
		MainScreen(Animation::animate(age, 0.25f, Animation::Interpolation::linear), false);
		if (age >= 0.25f)
			Animation::changed(IntroAnimation2, 69); // we done here
		break;
	}
}

void GUI::DrawNormalTab(Config::Tab* t)
{
	static Config::CColor* WidgetBackground = Config::GetColor(XOR("theme-widget-background"));
	static Config::CColor* WidgetTitleText = Config::GetColor(XOR("theme-widget-title"));
	static Config::CColor* PropertyHighlight = Config::GetColor(XOR("theme-search-result-highlight"));

	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;

	int WidgetWidth = Window->ContentRegionRect.GetWidth();
	int WidgetX = t->HorizontalPadding + ImGui::GetCursorPosX(), WidgetY = t->TopPadding + t->VerticalPadding + ImGui::GetCursorPosY();

	Config::Property* ScrollTo = nullptr;
	if (t->Highlight.Property && t->Highlight.Scroll)
	{
		Config::Property* next = ScrollTo = t->Highlight.Property;

		// move all the sliders around so that this property will be visible
		do
		{
			if (next->Visible.State)
				next->Visible.State->Set(next->Visible.StateEquals);
		} while (next = next->VisibilityLinked);

		t->Highlight.Scroll = false;
	}

	ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
	int ScrollPos = 0;
	for (size_t g = 0; g < t->Groups.size(); g++)
	{
		Config::Group* Group = t->Groups[g];
		if (Group->Name == XOR("__META__")) continue;

		size_t GroupVisibleProperties = Group->CountVisibleProperties();
		if (GroupVisibleProperties == 0) continue;

		int GroupHeight = Group->Padding + GroupVisibleProperties * (20 + Group->Padding);
		if (Group->ShowName)
			GroupHeight += 18 + Group->Padding;

		int GroupY = Group->Padding;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Group->Rounding);
		ImGui::PushFont(Arial16);

		ImGui::SetCursorPos(ImVec2(WidgetX, WidgetY));
		ImGui::BeginChild((t->Name + XOR("-") + std::to_string(g)).c_str(), ImVec2(WidgetWidth - t->HorizontalPadding - WidgetX, GroupHeight), false, ImGuiWindowFlags_NoDecoration);
		auto GroupWindow = ImGui::GetCurrentWindow();
		auto GroupDrawList = GroupWindow->DrawList;

		if (Group->ShowName)
		{
			ImGui::SetCursorPos(ImVec2(5, GroupY));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::Text(Group->Name.c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor(1);
			GroupY += 18 + Group->Padding;
		}

		// draw properties
		{
			size_t nDrawnProps = 0;
			for (size_t p = 0; p < Group->Properties.size(); p++)
			{
				auto Property = Group->Properties[p];
				if (!Property->IsVisible())
				{
					continue;
				}

				if (nDrawnProps > 0)
					GroupY += Group->Padding;
				
				ImGui::SetCursorPos(ImVec2(0, GroupY));

				if (ScrollTo == Property)
				{
					ScrollPos = WidgetY + GroupY;
				}

				double timeSinceHighlight = t->Highlight.Property == Property ? Animation::delta(Animation::now(), t->Highlight.Time) : 1000.0;
				if (timeSinceHighlight < 2.0)
				{
					float factor = 1.f;
					if (timeSinceHighlight < 0.25)
						factor = Animation::animate(timeSinceHighlight, 0.25);
					else if (timeSinceHighlight > 1.0)
						factor = 1.f - Animation::animate(timeSinceHighlight - 1.0, 1.0);

					GroupDrawList->AddRectFilled(
						GroupWindow->DC.CursorPos,
						GroupWindow->DC.CursorPos + ImVec2(ImGui::GetWindowContentRegionWidth(), 20),
						PropertyHighlight->ModulateAlpha(factor)
					);
				}

				switch (Property->Type)
				{
				case Config::PropertyType::BOOLEAN:
					GroupY += ImGui::DrawBooleanProperty(Property);
					break;
				case Config::PropertyType::FLOAT:
					GroupY += ImGui::DrawFloatProperty(Property);
					break;
				case Config::PropertyType::COLOR:
					GroupY += ImGui::DrawColorProperty(Property);
					break;
				case Config::PropertyType::VSTATEFUL:
					GroupY += ImGui::DrawVerticalStatefulProperty(Property);
					break;
				case Config::PropertyType::HSTATEFUL:
					GroupY += ImGui::DrawHorizontalStatefulProperty(Property);
					break;
				case Config::PropertyType::MULTISELECT:
					GroupY += ImGui::DrawMultiSelectorProperty(Property);
					break;
				case Config::PropertyType::TEXTINPUT:
					GroupY += ImGui::DrawTextInputProperty(Property);
					break;
				case Config::PropertyType::FUNCTION:
					GroupY += ImGui::DrawFunctionProperty(Property);
					break;
				case Config::PropertyType::LABEL:
				default:
					GroupY += ImGui::DrawLabelProperty(Property);
					break;
				}

				if (ScrollTo == Property)
				{
					// the average of before and after will be the center
					ScrollPos = (ScrollPos + (WidgetY + GroupY)) / 2;
				}

				nDrawnProps++;
			}
		}

		GroupY += Group->Padding;
		ImGui::EndChild();

		ImGui::PopStyleVar(1);
		ImGui::PopFont();

		WidgetY += GroupHeight + t->VerticalPadding;
	}
	if (ScrollTo)
		ImGui::SetScrollFromPosY(ScrollPos, 0.5f);

	ImGui::PopStyleColor(1);

	return;
}

void GUI::DrawActiveTab()
{
	static Config::CColor* WidgetBackground = Config::GetColor(XOR("theme-widget-background"));
	static Config::CColor* WidgetTitleText = Config::GetColor(XOR("theme-widget-title"));
	static Config::CColor* PropertyText = Config::GetColor(XOR("theme-property-text"));
	static Config::CColor* LegitRageSwitchBackground = Config::GetColor(XOR("theme-legit-rage-switch-background"));
	static Config::CColor* LegitRageSwitchHighlight = Config::GetColor(XOR("theme-legit-rage-switch-highlight"));
	static Config::CColor* LegitRageSwitchOutline = Config::GetColor(XOR("theme-legit-rage-switch-outline"));
	static Config::CColor* LegitRageSwitchText = Config::GetColor(XOR("theme-legit-rage-switch-text"));
	static Config::CColor* EjectLabel = Config::GetColor(XOR("theme-eject"));
	static Config::CColor* EjectButton = Config::GetColor(XOR("theme-eject-button"));
	static Config::CColor* EjectButtonText = Config::GetColor(XOR("theme-eject-button-text"));
	static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
	static Config::CColor* ButtonHovered = Config::GetColor(XOR("theme-button-hovered"));
	static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
	static Config::CColor* ButtonBorder = Config::GetColor(XOR("theme-button-border"));
	static Config::CColor* ButtonText = Config::GetColor(XOR("theme-button-text"));
	static Config::CColor* TextInputBackground = Config::GetColor(XOR("theme-text-input-background"));
	static Config::CColor* TextInputText = Config::GetColor(XOR("theme-text-input-text"));
	static Config::CFloat* LegitRageSwitchBorderSize = Config::GetFloat(XOR("theme-legit-rage-switch-border-size"));
	static Config::CFloat* ButtonBorderSize = Config::GetFloat(XOR("theme-button-border-size"));

	if (!ActiveTab) return;

	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;

	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*PropertyText);

	if (ActiveTab->Name == XOR("Offence") || ActiveTab->Name == XOR("Defence"))
	{
		bool IsOffencePage = ActiveTab->Name == XOR("Offence");

		static Config::Property* MasterSwitch = Config::GetProperty(IsOffencePage ? XOR("offence-mode") : XOR("defence-mode"));
		if (!MasterSwitch)
		{
			MasterSwitch = Config::GetProperty(IsOffencePage ? XOR("offence-mode") : XOR("defence-mode"));
			if (!MasterSwitch) return;
		}
		Config::CHorizontalState* MasterMode = (Config::CHorizontalState*)MasterSwitch->Value;

		// big switch at the top
		{
			int Width = Window->ContentRegionRect.GetWidth() - 20;

			ImGui::PushStyleColor(ImGuiCol_Button, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*LegitRageSwitchBackground);
			ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)*LegitRageSwitchOutline);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*LegitRageSwitchText);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, LegitRageSwitchBorderSize->Get());
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.f);
			ImGui::PushFont(Arial18BoldItalics);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild(IsOffencePage ? XOR("offence-main-top-gui-switch") : XOR("defence-main-top-gui-switch"), ImVec2(Width, 40), true, 0);

			auto bruh_window = ImGui::GetCurrentWindow();
			auto bruh_dl = bruh_window->DrawList;

			double TimePassed = MasterMode->Value.GetTimeSinceChange();
			double AnimFactor = Animation::animate(TimePassed, 0.15f, Animation::Interpolation::easeInOutQuint);
			if (MasterMode->Value.Get() == 0) AnimFactor = 1.f - AnimFactor;
			float XOffset = Animation::lerp(5.f, Width / 2.f, AnimFactor);
			bruh_dl->AddRectFilled(bruh_window->Pos + ImVec2(XOffset, 5), bruh_window->Pos + ImVec2(XOffset + Width / 2 - 5, 35), *LegitRageSwitchHighlight, 5.f);

			ImGui::SetCursorPos(ImVec2(0, 0));
			if (ImGui::Button(IsOffencePage ? XOR("Legit##big-switch-offence") : XOR("Legit##big-switch-defence"), ImVec2(Width / 2, 40)))
			{
				Config::SettingKeybindFor = nullptr;
				MasterMode->Value.Set(0);
			}

			ImGui::SetCursorPos(ImVec2(Width / 2, 0));
			if (ImGui::Button(IsOffencePage ? XOR("Rage##big-switch-offence") : XOR("Rage##big-switch-defence"), ImVec2(Width / 2, 40)))
			{
				Config::SettingKeybindFor = nullptr;
				MasterMode->Value.Set(1);
			}

			ImGui::EndChild();

			ImGui::PopStyleColor(6);
			ImGui::PopStyleVar(2);
			ImGui::PopFont();
		}

		// the rest of the shit
		ImGui::SetCursorPos(ImVec2(0, 60));
		DrawNormalTab(ActiveTab);
	}
	else if (ActiveTab->Name == XOR("Eject"))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::SetCursorPos(ImVec2(10.f, 10.f));
		ImGui::BeginChild(XOR("##eject-confirm"), ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, 171));

		// title text
		{
			ImGui::PushFont(Arial18Bold);

			const char* Text = XOR("Are you sure you would like to eject?");
			float TextSize = ImGui::CalcTextSize(Text).x;

			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::SetCursorPos(ImVec2(ImGui::GetCurrentWindow()->Size.x / 2.f - TextSize / 2.f, 10));
			ImGui::TextEx(Text);
			ImGui::PopStyleColor(1);

			ImGui::PopFont();
		}

		// description
		{
			ImGui::PushFont(Arial16);

			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f));
			ImGui::TextEx(XOR("- your config will not be saved"));
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f));
			ImGui::TextEx(XOR("- your theme will not be saved"));
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f));
			ImGui::TextEx(XOR("- this will remove the cheat from your computer's memory"));
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f + 18.f));
			ImGui::TextEx(XOR("- this will disable all effects"));
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f + 18.f + 18.f));
			ImGui::TextEx(XOR("- there is a *small* chance that CS:GO will crash"));

			ImGui::PopFont();
		}

		// confirm button
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)*EjectButton);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)EjectButton->ModulateAlpha(0.8f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)EjectButton->ModulateAlpha(0.6f));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*EjectButtonText);

			ImGui::PushFont(Arial18BoldItalics);

			ImGui::SetCursorPos(ImVec2(10.f, 131));
			Ejected |= ImGui::Button(XOR("EJECT NOW##confirm"), ImVec2(ImGui::GetCurrentWindow()->Size.x - 20.f, 30.f));

			ImGui::PopFont();

			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(4);
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}
	else if (ActiveTab->Name == XOR("Skinchanger"))
	{
		ImU32 ButtonBackgroundColor = *ButtonBase;
		ImU32 ButtonHoveringColor = *ButtonHovered;
		ImU32 ButtonSelectedColor = *ButtonActive;
		ImU32 ButtonOutlineColor = *ButtonBorder;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
		int y = 0;

		// knife changer
		{
			static Config::CState* KnifeModel = Config::GetState(XOR("skinchanger-knife-model"));
			static Config::CPaintKit* KnifePaintkit = Config::GetPaintKit(XOR("skinchanger-knife-paintkit"));

			static int WindowHeight = 0;
			ImGui::SetCursorPos(ImVec2(10.f, y + 10));
			ImGui::BeginChild(XOR("##knife-skins"), ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, WindowHeight + 10.f));
			auto Widget = ImGui::GetCurrentWindow();

			ImGui::SetCursorPos(ImVec2(10.f, 9.f));
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::Text(XOR("Knives"));
			ImGui::PopStyleColor(1);
			ImGui::PopFont();

			// enable switch
			ImGui::PushFont(Arial16);
			static Config::Property* KnifeChangerEnable = Config::GetProperty(XOR("skinchanger-knife-enable"));
			ImGui::SetCursorPos(ImVec2(5.f, 33.f));
			ImGui::DrawBooleanProperty(KnifeChangerEnable);

			// knife models
			static Config::Property* KnifeModelProperty = Config::GetProperty(XOR("skinchanger-knife-model"));
			ImGui::SetCursorPos(ImVec2(5.f, 58.f));
			ImGui::DrawVerticalStatefulProperty(KnifeModelProperty);

			// knife wear
			static Config::Property* KnifeWearProperty = Config::GetProperty(XOR("skinchanger-knife-wear"));
			ImGui::SetCursorPos(ImVec2(5.f, 83.f));
			ImGui::DrawFloatProperty(KnifeWearProperty);

			// current skin + clear button
			bool KnifeHasSkinSelection = KnifePaintkit->PaintKit->ID > 0;
			std::string KnifeSkinName = KnifeHasSkinSelection ? KnifePaintkit->Stringify() : XOR("None");
			{
				static Animation::Anim* MyAnimation = Animation::newAnimation(XOR("knife-current-skin-hover"), 0);

				const char* label = XOR("Current: ");
				ImVec2 labelSize = ImGui::CalcTextSize(label);

				ImGui::SetCursorPos(ImVec2(10, 108 + (24 - labelSize.y) / 2));
				ImGui::Text(label);

				ImGui::SetCursorPos(ImVec2(10 + labelSize.x + 5, 108));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*TextInputBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild(XOR("##skin-knife-current-selection"), ImVec2(Widget->Size.x - ImGui::GetCursorPosX() - 10, 24), false);
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(1);

				Animation::changed(MyAnimation, ImGui::IsWindowHovered() || ImGui::GetActiveID() == ImGui::GetID(XOR("##remove-active-knife-skin")));
				float AnimFactor = Animation::animate(Animation::age(MyAnimation), 0.15);
				if (!MyAnimation->state)
					AnimFactor = 1.f - AnimFactor;

				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)TextInputText->ModulateAlpha(Animation::lerp(0.6f, 1.f, AnimFactor)));

				ImGui::SetCursorPos(ImVec2(4, 4));

				if (!KnifeHasSkinSelection)
					ImGui::Text(XOR("None"));
				else
				{
					auto win = ImGui::GetCurrentWindow();
					int maxX = win->Size.x - 24;

					if (AnimFactor > 0)
					{
						ImGui::PushClipRect(win->Pos, win->Pos + ImVec2(maxX, win->Size.y), false);
						ImGui::Text(KnifeSkinName.c_str());
						ImGui::PopClipRect();


						ImGui::SetCursorPos(ImVec2(maxX + 7, 7));
						ImGui::DrawXIcon(TextInputText->ModulateAlpha(AnimFactor * 0.8f), ImVec2(9, 9));
						ImGui::SetCursorPos(ImVec2(maxX + 3, 3));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)TextInputText->ModulateAlpha(0.25f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)TextInputText->ModulateAlpha(0.5f));
						if (ImGui::Button(XOR("##remove-active-weapon-skin"), ImVec2(18, 18)))
						{
							KnifePaintkit->ClearSelection();
							KnifeHasSkinSelection = false;
						}
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::Text(KnifeSkinName.c_str());
					}
				}

				ImGui::PopFont();
				ImGui::PopStyleColor(1);

				ImGui::EndChild();
			}
			ImGui::PopFont();


			// search bar
			static bool IsSearchingKnife = false;
			static char* KnifeSearchQuery = nullptr;
			while (!KnifeSearchQuery) if (KnifeSearchQuery = new char[256]) ZeroMemory(KnifeSearchQuery, 256);
			{
				ImGui::SetCursorPos(ImVec2(10, 137));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*TextInputBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild(XOR("##skin-knife-searchbar"), ImVec2(Widget->Size.x - 20, 24), false);

				const char* InputLabel = XOR("##KnifeSkinSearchTextInput");
				auto InputID = ImGui::GetID(InputLabel);
				static Animation::Anim* MySearchAnimation = Animation::newAnimation(XOR("knife-skin-search"));
				IsSearchingKnife = ImGui::GetActiveID() == InputID || KnifeSearchQuery[0];

				float SearchAnimationFactor = Animation::animate(Animation::age(MySearchAnimation), 0.15);
				if (!IsSearchingKnife)
					SearchAnimationFactor = 1.f - SearchAnimationFactor;

				Animation::changed(MySearchAnimation, IsSearchingKnife);

				ImGui::SetCursorPos(ImVec2(5, 5));
				ImGui::DrawSearchIcon(TextInputText->ModulateAlpha(0.8f), ImVec2(14, 14));

				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TextInputText);
				ImGui::SetCursorPos(ImVec2(24, 4));
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (IsSearchingKnife ? 48 : 28));
				ImGui::InputText(InputLabel, KnifeSearchQuery, 256);
				ImGui::PopStyleColor(1);
				WantKeyboard |= ImGui::IsItemActive();

				if (!IsSearchingKnife)
				{
					ImGui::SetCursorPos(ImVec2(24, 4));
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)TextInputText->ModulateAlpha(0.6f));
					ImGui::Text(XOR("Search Skins"));
					ImGui::PopStyleColor(1);
				}
				else
				{
					float F = (float)Animation::animate(Animation::age(SearchAnimation), 0.15);
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 7, 7));
					ImGui::DrawXIcon(TextInputText->ModulateAlpha(F * 0.8f), ImVec2(9, 9));
					// dummy button
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 3, 3));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)TextInputText->ModulateAlpha(0.25f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)TextInputText->ModulateAlpha(0.5f));
					if (ImGui::Button(XOR("##cancel-search-dummy-knife-skin"), ImVec2(18, 18)))
					{
						ZeroMemory(KnifeSearchQuery, 256);
						if (ImGui::GetActiveID() == InputID)
							ImGui::ClearActiveID();
					}
					ImGui::PopStyleColor(3);
				}
				ImGui::PopFont();

				ImGui::EndChild();
				ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);
			}

			// skin list
			{
				ImGui::SetCursorPos(ImVec2(11, 168));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ButtonBackgroundColor);
				ImGui::PushStyleColor(ImGuiCol_Button, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
				ImGui::BeginChild(XOR("##weapon-skin-listings"), ImVec2(Widget->Size.x - 22, 135), false);
				auto win = ImGui::GetCurrentWindow();
				Widget->DrawList->AddRect(
					win->Pos - ImVec2(1, 1),
					win->Pos + win->Size + ImVec2(1, 1),
					ButtonOutlineColor,
					0.f, 0
				);

				std::string SearchQuery = KnifeSearchQuery;

				ImGui::PushFont(Arial16);
				size_t SkinsShown = 0;
				for (size_t i = 0; i < Skins::NPaintKits; i++)
				{
					Skins::PaintKit PK = Skins::PaintKits[i];
					std::vector<std::vector<int>> IntendedApplicants = {}; // {{Weapon Knife or Glove, Mode, Version},{same},{...}}
					for (size_t j = 0; PK.Weapons[j] != Skins::Weapon::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Weapons[j], 0, (int)j });
					for (size_t j = 0; PK.Knives[j] != Skins::Knife::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Knives[j], 1, (int)j });
					for (size_t j = 0; PK.Gloves[j] != Skins::Glove::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Gloves[j], 2, (int)j });

					for (size_t j = 0; j < IntendedApplicants.size(); j++)
					{
						int NameId = IntendedApplicants[j][0];
						int Mode = IntendedApplicants[j][1];
						int Version = IntendedApplicants[j][2];

						std::string SkinName =
							(Mode == 0 ? Skins::WeaponNames[NameId] : (Mode == 1 ? Skins::KnifeNames[NameId] : Skins::GloveNames[NameId])) +
							XOR(" | ") +
							PK.VisibleName;

						if (IsSearchingKnife && !TextService::Contains(SearchQuery, SkinName, false))
							continue;

						ImGui::SetCursorPos(ImVec2(0, SkinsShown * 30));
						if (KnifePaintkit->PaintKit && KnifePaintkit->PaintKit->ID == PK.ID && Mode == KnifePaintkit->Mode && Version == KnifePaintkit->Version)
						{
							win->DrawList->AddRectFilled(win->DC.CursorPos, win->DC.CursorPos + ImVec2(win->Size.x, 30), ButtonSelectedColor);
						}
						else if (ImGui::Button((XOR("##invisible-knife-skin-select-") + SkinName).c_str(), ImVec2(win->Size.x, 30)))
						{
							KnifePaintkit->PaintKit = &Skins::PaintKits[i];
							KnifePaintkit->Mode = Mode;
							KnifePaintkit->Version = Version;
						}
						else if (ImGui::IsItemHovered())
						{
							ImGui::SetCursorPos(ImVec2(0, SkinsShown * 30));
							win->DrawList->AddRectFilled(win->DC.CursorPos, win->DC.CursorPos + ImVec2(win->Size.x, 30), ButtonHoveringColor);
						}

						ImGui::SetCursorPos(ImVec2(5, SkinsShown * 30 + (30 - ImGui::GetFontSize()) / 2));
						ImGui::Text(SkinName.c_str());
						SkinsShown++;
					}
				}
				ImGui::PopFont();

				ImGui::EndChild();
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(4);
			}
			WindowHeight = Widget->ContentSize.y;
			ImGui::EndChild();
			y = ImGui::GetCursorPosY();
		}

		static size_t SelectedWeaponGroup = 0;
		static size_t SelectedWeaponIndex = 0;
		int WeaponNamesHeight = Skins::WeaponGroupWeapons[SelectedWeaponGroup].size() * 30;

		// weapon changer
		{
			static int WindowHeight = 0;
			ImGui::SetCursorPos(ImVec2(10.f, y + 10));
			ImGui::BeginChild(XOR("##weapon-skins"), ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, WindowHeight + 10.f));
			auto Widget = ImGui::GetCurrentWindow();

			ImGui::SetCursorPos(ImVec2(10.f, 10.f));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::Text(XOR("Guns"));
			ImGui::PopFont();
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10.f, 37.f));

			Widget->DrawList->AddRect(
				Widget->DC.CursorPos - ImVec2(1, 1),
				Widget->DC.CursorPos + ImVec2(63, (int)Skins::WeaponGroup::_COUNT * 30 + 1),
				ButtonOutlineColor,
				5.f,
				ImDrawCornerFlags_All
			);
			Widget->DrawList->AddRectFilled(
				Widget->DC.CursorPos,
				Widget->DC.CursorPos + ImVec2(62, (int)Skins::WeaponGroup::_COUNT * 30),
				ButtonBackgroundColor,
				5.f,
				ImDrawCornerFlags_All
			);
			y = 37;

			ImGui::PushFont(Arial16);
			ImGui::PushStyleColor(ImGuiCol_Button, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
			// group labels
			{
				for (int i = 0; i < (int)Skins::WeaponGroup::_COUNT; i++)
				{
					ImGui::SetCursorPos(ImVec2(10.f, y));
					if (ImGui::Button((XOR("##dummy-weapongroup-btn-") + Skins::WeaponGroupNames[i]).c_str(), ImVec2(62, 30)))
					{
						SelectedWeaponGroup = i;
						SelectedWeaponIndex = 0;
					}

					ImGui::SetCursorPos(ImVec2(10.f, y));
					if (SelectedWeaponGroup == i || ImGui::IsItemHovered())
						Widget->DrawList->AddRectFilled(
							Widget->DC.CursorPos,
							Widget->DC.CursorPos + ImVec2(62, 30),
							SelectedWeaponGroup == i ? ButtonSelectedColor : ButtonHoveringColor,
							5.f,
							i == 0 ? ImDrawCornerFlags_Top : i == (int)Skins::WeaponGroup::_COUNT - 1 ? ImDrawCornerFlags_Bot : 0
						);

					ImGui::SetCursorPos(ImVec2(20.f, y + (30 - 16) / 2));
					ImGui::TextEx(Skins::WeaponGroupNames[i].c_str());

					y += 30;
				}
				// draw weapon names
				if (SelectedWeaponIndex >= Skins::WeaponGroupWeapons[SelectedWeaponGroup].size())
					SelectedWeaponIndex = 0;
				WeaponNamesHeight = Skins::WeaponGroupWeapons[SelectedWeaponGroup].size() * 30;

				ImGui::SetCursorPos(ImVec2(82, 37.f));
				Widget->DrawList->AddRect(
					Widget->DC.CursorPos - ImVec2(1, 1),
					Widget->DC.CursorPos + ImVec2(111, WeaponNamesHeight + 1),
					ButtonOutlineColor,
					5.f,
					ImDrawCornerFlags_All
				);
				Widget->DrawList->PathLineTo(Widget->Pos + ImVec2(81, 37 + SelectedWeaponGroup * 30 + 20));
				Widget->DrawList->PathLineTo(Widget->Pos + ImVec2(75, 37 + SelectedWeaponGroup * 30 + 17));
				Widget->DrawList->PathLineTo(Widget->Pos + ImVec2(74, 37 + SelectedWeaponGroup * 30 + 15));
				Widget->DrawList->PathLineTo(Widget->Pos + ImVec2(75, 37 + SelectedWeaponGroup * 30 + 13));
				Widget->DrawList->PathLineTo(Widget->Pos + ImVec2(81, 37 + SelectedWeaponGroup * 30 + 10));
				Widget->DrawList->PathFillConvex(ButtonOutlineColor);

				Widget->DrawList->AddRectFilled(
					Widget->DC.CursorPos,
					Widget->DC.CursorPos + ImVec2(110, WeaponNamesHeight),
					ButtonBackgroundColor,
					5.f,
					ImDrawCornerFlags_All
				);
				y = 37;

				// weapon labels
				for (size_t i = 0; i < Skins::WeaponGroupWeapons[SelectedWeaponGroup].size(); i++)
				{
					Skins::Weapon Weapon = Skins::WeaponGroupWeapons[SelectedWeaponGroup][i];
					std::string WeaponName = Skins::WeaponNames[(int)Weapon];

					ImGui::SetCursorPos(ImVec2(82, y));
					if (ImGui::Button((XOR("##dummy-weapongroup-btn-") + WeaponName).c_str(), ImVec2(110, 30)))
					{
						SelectedWeaponIndex = i;
					}

					ImGui::SetCursorPos(ImVec2(82, y));
					if (SelectedWeaponIndex == i || ImGui::IsItemHovered())
						Widget->DrawList->AddRectFilled(
							Widget->DC.CursorPos,
							Widget->DC.CursorPos + ImVec2(110, 30),
							SelectedWeaponIndex == i ? ButtonSelectedColor : ButtonHoveringColor,
							5.f,
							i == 0 ? ImDrawCornerFlags_Top : i == (Skins::WeaponGroupWeapons[SelectedWeaponGroup].size() - 1) ? ImDrawCornerFlags_Bot : 0
						);

					ImGui::SetCursorPos(ImVec2(92, y + (30 - 16) / 2));
					ImGui::TextEx(WeaponName.c_str());

					y += 30;
				}
			}

			ImGui::PopStyleColor(3);

			Skins::Weapon CurrentSelectedWeapon = Skins::WeaponGroupWeapons[SelectedWeaponGroup][SelectedWeaponIndex];
			Config::Property* CurrentWeaponSkinProp =
				Config::GetProperty(XOR("skinchanger-weapon-") + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[(int)CurrentSelectedWeapon])));
			Config::CPaintKit* CurrentPaintKit = (Config::CPaintKit*)CurrentWeaponSkinProp->Value;

			bool WeaponHasSkinSelection = CurrentPaintKit->PaintKit->ID > 0;
			std::string WeaponSkinName = WeaponHasSkinSelection ? CurrentPaintKit->Stringify() : XOR("None");

			// current skin for this weapon
			{
				static Animation::Anim* MyAnimation = Animation::newAnimation(XOR("weapon-current-skin-hover"), 0);

				const char* label = XOR("Current: ");
				ImVec2 labelSize = ImGui::CalcTextSize(label);

				ImGui::SetCursorPos(ImVec2(202, 37 + (24 - labelSize.y) / 2));
				ImGui::Text(label);

				ImGui::SetCursorPos(ImVec2(202 + labelSize.x + 5, 37));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*TextInputBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild(XOR("##skin-weapon-current-selection"), ImVec2(Widget->Size.x - ImGui::GetCursorPosX() - 10, 24), false);
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(1);

				Animation::changed(MyAnimation, ImGui::IsWindowHovered() || ImGui::GetActiveID() == ImGui::GetID(XOR("##remove-active-weapon-skin")));
				float AnimFactor = Animation::animate(Animation::age(MyAnimation), 0.15);
				if (!MyAnimation->state)
					AnimFactor = 1.f - AnimFactor;

				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)TextInputText->ModulateAlpha(Animation::lerp(0.6f, 1.f, AnimFactor)));

				ImGui::SetCursorPos(ImVec2(4, 4));

				if (!WeaponHasSkinSelection)
					ImGui::Text(XOR("None"));
				else
				{
					auto win = ImGui::GetCurrentWindow();
					int maxX = win->Size.x - 24;

					if (AnimFactor > 0)
					{
						ImGui::PushClipRect(win->Pos, win->Pos + ImVec2(maxX, win->Size.y), false);
						ImGui::Text(WeaponSkinName.c_str());
						ImGui::PopClipRect();


						ImGui::SetCursorPos(ImVec2(maxX + 7, 7));
						ImGui::DrawXIcon(TextInputText->ModulateAlpha(AnimFactor * 0.8f), ImVec2(9, 9));
						ImGui::SetCursorPos(ImVec2(maxX + 3, 3));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)TextInputText->ModulateAlpha(0.25f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)TextInputText->ModulateAlpha(0.5f));
						if (ImGui::Button(XOR("##remove-active-weapon-skin"), ImVec2(18, 18)))
						{
							CurrentPaintKit->ClearSelection();
							WeaponHasSkinSelection = false;
						}
						ImGui::PopStyleColor(3);
					}
					else
					{
						ImGui::Text(WeaponSkinName.c_str());
					}
				}

				ImGui::PopFont();
				ImGui::PopStyleColor(1);

				ImGui::EndChild();
			}

			ImGui::PopFont();

			static bool IsSearchingWeapon = false;
			static char* WeaponSearchQuery = nullptr;
			while (!WeaponSearchQuery) if (WeaponSearchQuery = new char[256]) ZeroMemory(WeaponSearchQuery, 256);

			// search bar
			{
				ImGui::SetCursorPos(ImVec2(202, 37 + 24 + 5));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*TextInputBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild(XOR("##skin-weapon-searchbar"), ImVec2(Widget->Size.x - 202 - 10, 24), false);

				const char* InputLabel = XOR("##WeaponSkinSearchTextInput");
				auto InputID = ImGui::GetID(InputLabel);
				static Animation::Anim* MySearchAnimation = Animation::newAnimation(XOR("weapon-skin-search"));
				IsSearchingWeapon = ImGui::GetActiveID() == InputID || WeaponSearchQuery[0];

				float SearchAnimationFactor = Animation::animate(Animation::age(MySearchAnimation), 0.15);
				if (!IsSearchingWeapon)
					SearchAnimationFactor = 1.f - SearchAnimationFactor;

				Animation::changed(MySearchAnimation, IsSearchingWeapon);

				ImGui::SetCursorPos(ImVec2(5, 5));
				ImGui::DrawSearchIcon(TextInputText->ModulateAlpha(0.8f), ImVec2(14, 14));

				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TextInputText);
				ImGui::SetCursorPos(ImVec2(24, 4));
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (IsSearchingWeapon ? 48 : 28));
				ImGui::InputText(InputLabel, WeaponSearchQuery, 256);
				ImGui::PopStyleColor(1);
				WantKeyboard |= ImGui::IsItemActive();

				if (!IsSearchingWeapon)
				{
					ImGui::SetCursorPos(ImVec2(24, 4));
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)TextInputText->ModulateAlpha(0.6f));
					ImGui::Text(XOR("Search Skins"));
					ImGui::PopStyleColor(1);
				}
				else
				{
					float F = (float)Animation::animate(Animation::age(SearchAnimation), 0.15);
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 7, 7));
					ImGui::DrawXIcon(TextInputText->ModulateAlpha(F * 0.8f), ImVec2(9, 9));
					// dummy button
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 3, 3));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)TextInputText->ModulateAlpha(0.25f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)TextInputText->ModulateAlpha(0.5f));
					if (ImGui::Button(XOR("##cancel-search-dummy-weapon-skin"), ImVec2(18, 18)))
					{
						ZeroMemory(WeaponSearchQuery, 256);
						if (ImGui::GetActiveID() == InputID)
							ImGui::ClearActiveID();
					}
					ImGui::PopStyleColor(3);
				}
				ImGui::PopFont();

				ImGui::EndChild();
				ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);
			}

			// skin list
			{
				ImGui::SetCursorPos(ImVec2(203, 37 + 24 + 5 + 24 + 5 + 1));
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ButtonBackgroundColor);
				ImGui::PushStyleColor(ImGuiCol_Button, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
				ImGui::BeginChild(XOR("##weapon-skin-listings"), ImVec2(Widget->Size.x - 203 - 11, 37 + WeaponNamesHeight - ImGui::GetCursorPosY()), false);
				auto win = ImGui::GetCurrentWindow();
				Widget->DrawList->AddRect(
					win->Pos - ImVec2(1, 1),
					win->Pos + win->Size + ImVec2(1, 1),
					ButtonOutlineColor,
					0.f, 0
				);

				std::string SearchQuery = WeaponSearchQuery;

				ImGui::PushFont(Arial16);
				size_t SkinsShown = 0;
				for (size_t i = 0; i < Skins::NPaintKits; i++)
				{
					Skins::PaintKit PK = Skins::PaintKits[i];
					std::vector<std::vector<int>> IntendedApplicants = {}; // {{Weapon Knife or Glove, Mode, Version},{same},{...}}
					for (size_t j = 0; PK.Weapons[j] != Skins::Weapon::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Weapons[j], 0, (int)j });
					for (size_t j = 0; PK.Knives[j] != Skins::Knife::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Knives[j], 1, (int)j });
					for (size_t j = 0; PK.Gloves[j] != Skins::Glove::INVALID; j++)
						IntendedApplicants.push_back({ (int)PK.Gloves[j], 2, (int)j });

					for (size_t j = 0; j < IntendedApplicants.size(); j++)
					{
						int NameId = IntendedApplicants[j][0];
						int Mode = IntendedApplicants[j][1];
						int Version = IntendedApplicants[j][2];

						std::string SkinName =
							(Mode == 0 ? Skins::WeaponNames[NameId] : (Mode == 1 ? Skins::KnifeNames[NameId] : Skins::GloveNames[NameId])) +
							XOR(" | ") +
							PK.VisibleName;

						if (IsSearchingWeapon && !TextService::Contains(SearchQuery, SkinName, false))
							continue;

						ImGui::SetCursorPos(ImVec2(0, SkinsShown * 30));
						if (CurrentPaintKit->PaintKit && CurrentPaintKit->PaintKit->ID == PK.ID && Mode == CurrentPaintKit->Mode && Version == CurrentPaintKit->Version)
						{
							win->DrawList->AddRectFilled(win->DC.CursorPos, win->DC.CursorPos + ImVec2(win->Size.x, 30), ButtonSelectedColor);
						}
						else if (ImGui::Button((XOR("##invisible-weapon-skin-select-") + SkinName).c_str(), ImVec2(win->Size.x, 30)))
						{
							CurrentPaintKit->PaintKit = &Skins::PaintKits[i];
							CurrentPaintKit->Mode = Mode;
							CurrentPaintKit->Version = Version;
						}
						else if (ImGui::IsItemHovered())
						{
							ImGui::SetCursorPos(ImVec2(0, SkinsShown * 30));
							win->DrawList->AddRectFilled(win->DC.CursorPos, win->DC.CursorPos + ImVec2(win->Size.x, 30), ButtonHoveringColor);
						}

						ImGui::SetCursorPos(ImVec2(5, SkinsShown * 30 + (30 - ImGui::GetFontSize()) / 2));
						ImGui::Text(SkinName.c_str());
						SkinsShown++;
					}
				}
				ImGui::PopFont();

				ImGui::EndChild();
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(4);
			}

			WindowHeight = Widget->ContentSize.y;
			ImGui::EndChild();
			y = ImGui::GetCursorPosY();
		}

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(1);
	}
	else if (ActiveTab->Name == XOR("Theme"))
	{
		int WidgetWidth = Window->ContentRegionRect.GetWidth();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPos(ImVec2(10, 10));
		ImGui::BeginChild(XOR("##theme-import/export"), ImVec2(WidgetWidth - 20, 100), false, ImGuiWindowFlags_NoDecoration);
		auto InnerWindow = ImGui::GetCurrentWindow();

		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
		ImGui::PushFont(Arial18BoldItalics);
		ImGui::Text(XOR("Save / Load"));
		ImGui::SetCursorPos(ImVec2(5, 52));
		ImGui::Text(XOR("Presets"));
		ImGui::PopFont();
		ImGui::PopStyleColor(1);

		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
		ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

		// icon + import/export buttons
		{
			ImVec2 IconSize(14, 14);
			ImVec2 Pos(0, 5 + 18 + 5);
			ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
			ImGui::DrawInfoIcon(255, IconSize);

			auto ID = ImGui::GetID(XOR("theme-import-export-info-btn"));
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip(XOR("Click for more info"), IconSize.y);
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, XOR("http://a4g4.com/help/index.php#theme"), 0, 0, SW_SHOW);
				}
			}


			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, 0));
			if (ImGui::Button(XOR("Export##theme"), ImVec2(60, 20)))
			{
				Config::PromptExportThemeFile();
			}

			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6 + 60 + 6, 0));
			if (ImGui::Button(XOR("Import##theme"), ImVec2(60, 20)))
			{
				Config::PromptImportThemeFile();
			}

		}

		// preset buttons
		{
			ImVec2 IconSize(14, 14);
			ImVec2 Pos(0, 52 + 18 + 5);
			ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
			ImGui::DrawInfoIcon(255, IconSize);

			auto ID = ImGui::GetID(XOR("theme-presets-info-btn"));
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip(XOR("Click for more info"), IconSize.y);
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, XOR("http://a4g4.com/help/index.php#theme"), 0, 0, SW_SHOW);
				}
			}

			int x = 6 + IconSize.x + 6;
			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Dark##themepreset"), ImVec2(60, 20)))
			{
				Config::ImportTheme(ConfigConstants::ThemeDark, ConfigConstants::ThemeDarkSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Light##themepreset"), ImVec2(60, 20)))
			{
				Config::ImportTheme(ConfigConstants::ThemeLight, ConfigConstants::ThemeLightSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Contrast##themepreset"), ImVec2(60, 20)))
			{
				Config::ImportTheme(ConfigConstants::ThemeContrast, ConfigConstants::ThemeContrastSize);
			}
			x += 66;
		}

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);

		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);

		// the rest of the shit
		ImGui::SetCursorPos(ImVec2(0, 120));
		DrawNormalTab(ActiveTab);
	}
	else if (ActiveTab->Name == XOR("Config"))
	{
		int WidgetWidth = Window->ContentRegionRect.GetWidth();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPos(ImVec2(10, 10));
		ImGui::BeginChild(XOR("##config-import/export"), ImVec2(WidgetWidth - 20, 100), false, ImGuiWindowFlags_NoDecoration);
		auto InnerWindow = ImGui::GetCurrentWindow();

		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
		ImGui::PushFont(Arial18BoldItalics);
		ImGui::Text(XOR("Save / Load"));
		ImGui::SetCursorPos(ImVec2(5, 52));
		ImGui::Text(XOR("Presets"));
		ImGui::PopFont();
		ImGui::PopStyleColor(1);

		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
		ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

		// icon + import/export buttons
		{
			ImVec2 IconSize(14, 14);
			ImVec2 Pos(0, 5 + 18 + 5);
			ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
			ImGui::DrawInfoIcon(255, IconSize);

			auto ID = ImGui::GetID(XOR("config-import-export-info-btn"));
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip(XOR("Click for more info"), IconSize.y);
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, XOR("http://a4g4.com/help/index.php#config"), 0, 0, SW_SHOW);
				}
			}


			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, 0));
			if (ImGui::Button(XOR("Export##config"), ImVec2(60, 20)))
			{
				Config::PromptExportConfigFile();
			}

			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6 + 60 + 6, 0));
			if (ImGui::Button(XOR("Import##config"), ImVec2(60, 20)))
			{
				Config::PromptImportConfigFile();
			}

		}

		// preset buttons
		{
			ImVec2 IconSize(14, 14);
			ImVec2 Pos(0, 52 + 18 + 5);
			ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
			ImGui::DrawInfoIcon(255, IconSize);

			auto ID = ImGui::GetID(XOR("config-presets-info-btn"));
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip(XOR("Click for more info"), IconSize.y);
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, XOR("http://a4g4.com/help/index.php#config"), 0, 0, SW_SHOW);
				}
			}

			int x = 6 + IconSize.x + 6;
			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Off##configpreset"), ImVec2(60, 20)))
			{
				Config::ImportConfig(ConfigConstants::ConfigOff, ConfigConstants::ConfigOffSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Legit##configpreset"), ImVec2(60, 20)))
			{
				Config::ImportConfig(ConfigConstants::ConfigLegit, ConfigConstants::ConfigLegitSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button(XOR("Rage##configpreset"), ImVec2(60, 20)))
			{
				Config::ImportConfig(ConfigConstants::ConfigRage, ConfigConstants::ConfigRageSize);
			}
			x += 66;
		}

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(2);

		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);

		// the rest of the shit
		ImGui::SetCursorPos(ImVec2(0, 120));
		DrawNormalTab(ActiveTab);
	}
	else
	{
		DrawNormalTab(ActiveTab);
	}

	ImGui::PopStyleColor(1);
}

void GUI::MainScreen(float ContentOpacity, bool Interactable)
{
	unsigned char ThisContentOpacity = (unsigned char)(ContentOpacity * 255.f);
	static Config::CColor* TopbarBackground = Config::GetColor(XOR("theme-topbar-background"));
	static Config::CColor* TopbarText = Config::GetColor(XOR("theme-topbar-text"));
	static Config::CColor* TablistBackground = Config::GetColor(XOR("theme-overlay-background"));
	static Config::CColor* TablistText = Config::GetColor(XOR("theme-overlay-text"));
	static Config::CColor* ActiveTablistText = Config::GetColor(XOR("theme-overlay-active-text"));
	static Config::CColor* SearchbarBackground = Config::GetColor(XOR("theme-main-searchbar-background"));
	static Config::CColor* SearchbarText = Config::GetColor(XOR("theme-main-searchbar-text"));
	static Config::CColor* ButtonBase = Config::GetColor(XOR("theme-button-background"));
	static Config::CColor* ButtonHovered = Config::GetColor(XOR("theme-button-hovered"));
	static Config::CColor* ButtonActive = Config::GetColor(XOR("theme-button-active"));
	static Config::CColor* EjectLabel = Config::GetColor(XOR("theme-eject"));
	static Config::CColor* MainBorder = Config::GetColor(XOR("theme-border"));
	static Config::CColor* MainBackground = Config::GetColor(XOR("theme-background"));
	static Config::CColor* ScrollbarBackground = Config::GetColor(XOR("theme-scrollbar-background"));
	static Config::CColor* ScrollbarGrab = Config::GetColor(XOR("theme-scrollbar-grabber"));
	static Config::CColor* ActiveTabPointer = Config::GetColor(XOR("theme-overlay-active-pointer"));
	static Config::CFloat* MainBorderSize = Config::GetFloat(XOR("theme-border-size"));


	// Initially center the window w/ default size
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), Interactable ? ImGuiCond_Once : ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(DefaultMenuSize, Interactable ? ImGuiCond_Once : ImGuiCond_Always);

	// some global styles cuz lmao
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, MinMenuSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, (ImVec4)*TopbarBackground);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4)*TopbarBackground);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)*MainBackground);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)*ButtonBase);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonHovered);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonActive);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, (ImVec4)*ScrollbarBackground);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, (ImVec4)(ScrollbarGrab->ModulateAlpha(0.6f)));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, (ImVec4)(ScrollbarGrab->ModulateAlpha(0.8f)));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, (ImVec4)*ScrollbarGrab);

	ImGui::PushFont(Arial14BoldItalics);
	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TopbarText);
	int TitleBarHeight = 24;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, (TitleBarHeight - ImGui::GetFontSize()) / 2.f));
	std::string MainTitle =
		std::string(XOR("A4G4 - ")) +
		std::string((UserData::Premium ? XOR("FULL VERSION ") : XOR("TRIAL VERSION "))) +
		std::string((UserData::Authenticated ? XOR("(USER ") + std::to_string(UserData::UserId) + XOR(")") : XOR("(UNAUTHENTICATED)")));
	ImGui::Begin(MainTitle.c_str(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | (Interactable ? 0 : ImGuiWindowFlags_NoInputs));
	ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(1);

	// init
	auto Window = MainWindow = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;


	// draw border because imgui borders are inset which makes me wanna kms
	{
		float BorderThickness = MainBorderSize->Get();
		if (BorderThickness > 0.f)
		{
			auto BackgroundDrawList = ImGui::GetBackgroundDrawList();
			BackgroundDrawList->AddRect(
				Window->Pos - ImVec2(BorderThickness / 2.f, BorderThickness / 2.f),
				Window->Pos + Window->Size + ImVec2(BorderThickness / 2.f, BorderThickness / 2.f),
				*MainBorder,
				ImGui::GetStyle().WindowRounding,
				ImDrawCornerFlags_All,
				BorderThickness
			);
		}
	}

	if (!ActiveTab && Config::Tabs.size() > 0)
		ActiveTab = Config::Tabs.at(0);

	float SearchAnimationFactor = Animation::animate(Animation::age(SearchAnimation), 0.15f);
	if (!IsSearching)
		SearchAnimationFactor = 1.f - SearchAnimationFactor;

	ImVec2 OverlaySize(Animation::lerp(150, Window->Size.x, SearchAnimationFactor), Window->Size.y - TitleBarHeight);
	ImVec2 OverlayPosition(0, TitleBarHeight);

	// background gif/color idk yet
	{

	}

	// right side
	{
		ImVec2 ChildPos = Window->Pos + OverlayPosition + ImVec2(OverlaySize.x, 0);
		ImVec2 ChildSize = Window->Size - OverlayPosition - ImVec2(OverlaySize.x, 0);

		ImGui::SetCursorPos(ImVec2(150, TitleBarHeight));
		ImGui::BeginChild(XOR("##main-right-side"), Window->Size - OverlayPosition - ImVec2(150, 0));

		auto w = ImGui::GetCurrentWindow();
		ImGui::PushClipRect(ChildPos, ChildPos + ChildSize, false);

		if (ActiveTab && ActiveTab->InitialPaint)
		{
			ActiveTab->InitialPaint = false;
			ImGui::SetScrollY(ActiveTab->ScrollHeight);
		}
		GUI::DrawActiveTab();
		if (ActiveTab)
			ActiveTab->ScrollHeight = ImGui::GetScrollY();

		// blank space at the bottom to fix weird scroll bar
		ImGui::PushStyleColor(ImGuiCol_Button, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
		ImGui::Button(XOR("##BRO"), ImVec2(1, ActiveTab->VerticalPadding));
		ImGui::PopStyleColor(3);

		ImGui::PopClipRect();

		ImGui::EndChild();
	}

	// left side
	{

		// draw background because imgui is fuckign retarded and can't clip the bottom corners correctly
		DrawList->AddRectFilled(
			Window->Pos + OverlayPosition, Window->Pos + OverlayPosition + OverlaySize,
			*TablistBackground,
			5.f, (SearchAnimationFactor > 0.5f) ? ImDrawCornerFlags_Bot : ImDrawCornerFlags_BotLeft
		);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		ImGui::SetCursorPos(OverlayPosition);
		ImGui::BeginChild(XOR("##left-side"), OverlaySize, false);

		// search bar
		{
			ImGui::SetCursorPos(ImVec2(5, 5));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*SearchbarBackground);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
			ImGui::BeginChild(XOR("##left-side-searchbar"), ImVec2(ImGui::GetWindowContentRegionWidth() - 10, 24), false);

			const char* InputLabel = XOR("##SearchTextInput");
			auto InputID = ImGui::GetID(InputLabel);

			// open search window if it wasnt already
			if (ImGui::GetActiveID() == InputID || (SearchQuery && SearchQuery[0]))
			{
				if (!IsSearching)
				{
					IsSearching = true;
					Animation::changed(SearchAnimation, IsSearching);
				}
			}

			ImGui::SetCursorPos(ImVec2(5, 5));
			ImGui::DrawSearchIcon(SearchbarText->ModulateAlpha(0.8f), ImVec2(14, 14));

			ImGui::PushFont(Arial16);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*SearchbarText);
			ImGui::SetCursorPos(ImVec2(24, 4));
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (IsSearching ? 48 : 28));
			ImGui::InputText(InputLabel, SearchQuery, 255);
			ImGui::PopStyleColor(1);
			WantKeyboard |= ImGui::IsItemActive();

			if (!IsSearching)
			{
				ImGui::SetCursorPos(ImVec2(24, 4));
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)SearchbarText->ModulateAlpha(0.6f));
				ImGui::Text(XOR("Search"));
				ImGui::PopStyleColor(1);
			}
			else
			{
				float w = ImGui::GetWindowContentRegionWidth();
				Config::SettingKeybindFor = nullptr;
				ImGui::SetCursorPos(ImVec2(w - 24 + 7, 7));
				ImGui::DrawXIcon(SearchbarText->ModulateAlpha(SearchAnimationFactor * 0.8f), ImVec2(9, 9));
				// dummy button
				ImGui::SetCursorPos(ImVec2(w - 24 + 3, 3));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)SearchbarText->ModulateAlpha(0.25f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)SearchbarText->ModulateAlpha(0.5f));
				if (ImGui::Button(XOR("##cancel-search-dummy"), ImVec2(18, 18)))
				{
					ZeroMemory(SearchQuery, 256);
					if (ImGui::GetActiveID() == InputID)
						ImGui::ClearActiveID();

					// close search window
					IsSearching = false;
					Animation::changed(SearchAnimation, IsSearching);
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::PopFont();

			ImGui::EndChild();
			ImGui::PopStyleColor(1);
			ImGui::PopStyleVar(1);
		}

		// tabs list
		if (!IsSearching)
		{
			ImGui::PushFont(Arial18);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TablistText);
			ImGui::PushStyleColor(ImGuiCol_Button, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);

			int x = 29, y = 5 + 24 + 5;
			int SelectedTabIndex = -1;
			int HoveredTabIndex = -1;

			for (size_t i = 0; i < Config::Tabs.size(); i++)
			{
				Config::Tab* t = Config::Tabs.at(i);
				bool isSelected = t == ActiveTab;
				bool isEject = t->Name == XOR("Eject");

				if (isEject || isSelected)
				{
					ImGui::PushFont(Arial18Bold);

					if (isEject)
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*EjectLabel);
					else
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ActiveTablistText);
				}


				ImGui::SetCursorPos(ImVec2(x, y));
				ImGui::TextEx(t->Name.c_str());

				if (isEject || isSelected)
				{
					ImGui::PopFont();
					ImGui::PopStyleColor(1);
				}

				// dummy button to detect clicks + hover
				ImGui::SetCursorPos(ImVec2(5, y - 1));
				if (ImGui::Button((XOR("##dummy-tab-select-") + t->Name).c_str(), ImVec2(OverlaySize.x - 10, 20)))
				{
					ActiveTab = t;
					ActiveTab->InitialPaint = true;
					isSelected = true;
					Config::SettingKeybindFor = nullptr;
				}

				if (ImGui::IsItemHovered())
					HoveredTabIndex = i;
				if (isSelected)
					SelectedTabIndex = i;

				y += ImGui::GetFontSize() + 2;
			}

			ImGui::PopStyleColor(4);
			ImGui::PopFont();


			// selected triangle
			if (SelectedTabIndex >= 0)
			{
				ImGui::SetCursorPos(ImVec2(12, 34 + 20 * SelectedTabIndex + (18 - 8) / 2));
				ImGui::DrawSelectionCursor(*ActiveTabPointer, ImVec2(10, 8));
			}

			// hover triangle
			if (HoveredTabIndex >= 0 && SelectedTabIndex != HoveredTabIndex)
			{
				ImGui::SetCursorPos(ImVec2(12, 34 + 20 * HoveredTabIndex + (18 - 8) / 2));
				ImGui::DrawSelectionCursor(ActiveTabPointer->ModulateAlpha(0.4f), ImVec2(10, 8));
			}
		}
		// search results
		else
		{
			ImGui::SetCursorPos(ImVec2(10, 5 + 24 + 8));

			PerformSearch();
			
			ImGui::PushFont(Arial16);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TablistText);

			for (SearchResult& res : SearchResults)
			{
				ImGui::SetCursorPosX(15);

				ImGui::PushFont(Arial16Bold);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ActiveTablistText);
				ImGui::Text(res.title.c_str());
				ImGui::PopStyleColor();
				ImGui::PopFont();
				
				for (auto& prop : res.properties)
				{
					ImGui::SetCursorPosX(25);

					std::string identifier = "##" + res.title + "-" + prop["ids"][0].get<std::string>();
					ImGuiID id = ImGui::GetID(identifier.c_str());

					ImVec2 size = ImGui::CalcTextSize(prop["name"].get<std::string>().c_str());
					ImVec2 pos = ImGui::GetCursorPos();

					bool brighten = ImGui::GetHoveredID() == id || ImGui::GetActiveID() == id;
					if (brighten)
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*ActiveTablistText);
					ImGui::Text(prop["name"].get<std::string>().c_str());
					if (brighten)
						ImGui::PopStyleColor();
					
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.f);
					ImGui::SetCursorPos(pos);
					if (ImGui::Button(identifier.c_str(), size))
					{
						// close search window
						ZeroMemory(SearchQuery, 256);
						IsSearching = false;
						Animation::changed(SearchAnimation, IsSearching);

						Config::Property* p;
						Config::Group* g;
						Config::Tab* t;
						if (
								(p = Config::GetProperty(prop["ids"][0].get<std::string>())) &&
								(g = p->Parent) &&
								(t = g->Parent)
							)
						{
							ActiveTab = t;
							t->Highlight.Property = p;
							t->Highlight.Time = Animation::now();
							t->Highlight.Scroll = true;
						}
					}
					ImGui::PopStyleVar();
				}
				
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
			}

			if (SearchResults.size() == 0)
			{
				const char* text = "No Results";
				ImVec2 sz = ImGui::CalcTextSize(text);
				ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - sz.x) / 2);
				ImGui::Text(text);
			}

			ImGui::PopStyleColor();
			ImGui::PopFont();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
	}

	if (ThisContentOpacity < 255)
		ImGui::GetForegroundDrawList()->AddRectFilled(Window->Pos, Window->Pos + Window->Size, IM_COL32(30, 30, 30, 255 - ThisContentOpacity), 5.f);

	ImGui::End();
	ImGui::PopFont();

	ImGui::PopStyleVar(8);
	ImGui::PopStyleColor(17);
}

void GUI::PerformSearch()
{
	// only search when needed
	if (strncmp(CachedSearch, SearchQuery, 256) == 0)
		return;
	memcpy(CachedSearch, SearchQuery, 256);

	// get a list of search terms
	std::vector<std::string> searchTerms = {};
	char word[256];
	size_t o = 0;

	// check if every word is a stopword
	bool allStopWords = true;
	while (allStopWords && SearchQuery[o])
	{
		// clear previous word
		ZeroMemory(word, 256);

		// fill new word
		size_t start = o;
		for (char c = SearchQuery[o]; c; c = SearchQuery[++o])
		{
			if (!IS_ALPHA(c))
				break;
			else
				word[o - start] = MAKE_LOWER(c);
		}

		// just move o to the start of the next word
		while (SearchQuery[o] && !IS_ALPHA(SearchQuery[o]))
			o++;

		// check if this word is a stopword
		bool isStopWord = false;
		for (auto& stopword : Config::SearchStopwords)
		{
			if (stopword.get<std::string>() == std::string(word)) // it will always be /0 terminated, vs2019 is dumb
			{
				isStopWord = true;
				break;
			}
		}
		if (!isStopWord)
		{
			allStopWords = false;
			break;
		}
	}

	// loop through words
	o = 0;
	while (SearchQuery[o])
	{
		// clear previous word
		ZeroMemory(word, 256);

		// fill new word
		size_t start = o;
		for (char c = SearchQuery[o]; c; c = SearchQuery[++o])
		{
			if (!IS_ALPHA(c))
				break;
			else
				word[o - start] = MAKE_LOWER(c);
		}

		// just move o to the start of the next word
		while (SearchQuery[o] && !IS_ALPHA(SearchQuery[o]))
			o++;

		// check if this word is a stopword
		bool isStopWord = false;
		if (!allStopWords) // if they're all stop words, then none of them are
		{
			for (auto& stopword : Config::SearchStopwords)
			{
				if (stopword.get<std::string>() == std::string(word))
				{
					isStopWord = true;
					break;
				}
			}
		}

		if (!isStopWord)
		{
			searchTerms.push_back(word);
		}
	}

	SearchResults.clear();
	for (auto& [tabName, tab] : Config::SearchableFeatures.items())
	{
		for (auto& [groupName, group] : tab.items())
		{
			SearchResults.push_back(SearchResult{ tabName + " / " + groupName , {} });
			SearchResult& result = SearchResults.at(SearchResults.size() - 1);

			for (auto& prop : group)
			{
				// check if the search should return this property
				bool matches = true;
				for (std::string& term : searchTerms)
				{
					bool termHasMatch = false;
					for (auto& keyword : prop["keywords"])
					{
						if (keyword.get<std::string>().find(term, 0) != std::string::npos)
						{
							termHasMatch = true;
							break;
						}
					}
					if (!termHasMatch)
					{
						matches = false;
						break;
					}
				}

				if (matches)
				{
					result.properties.push_back(prop);
				}
			}

			if (result.properties.size() == 0)
			{
				SearchResults.pop_back(); // nvm, remove it lol
			}
		}
	}
}

void GUI::Init()
{
	L::Verbose(XOR("GUI::Init running"));
	ZeroMemory(SearchQuery, 256);
	ZeroMemory(CachedSearch, 256);
	strcpy(CachedSearch, "this string is here so that `PerformSearch` will actually run on the first call");

	SearchAnimation = Animation::newAnimation(XOR("search-open/close"), 0);
	L::Verbose(XOR("GUI::Init complete"));
}

void GUI::Main()
{
	static auto MenuOpen = Config::GetState(XOR("show-menu"));
	++Config::GUIFramesRenderedCounter;
	L::Verbose((XOR("GUI::Main executed (frame ") + std::to_string(Config::GUIFramesRenderedCounter) + XOR(")")).c_str());
	static bool Init = false;
	if (!Init)
	{
		GUI::Init();
		Init = true;
	}

	WantKeyboard = false;
	if (IntroAnimation2 && IntroAnimation2->state != 69)
	{
		L::Verbose(XOR("GUI::AuthenticationIntro running"));
		AuthenticationIntro();
		L::Verbose(XOR("GUI::AuthenticationIntro complete"));
	}
	else if (UserData::SessionId != "")
	{
		if (MenuOpen->Get() && !IsSteamOverlayOpen())
		{
			L::Verbose(XOR("GUI::MainScreen running"));
			MainScreen();
			L::Verbose(XOR("GUI::MainScreen complete"));
		}
	}
	else if (VisibleLoadProgress <= 1.f) // if == 1, currently animating
	{
		L::Verbose(XOR("GUI::LoadingScreen running"));
		LoadingScreen();
		L::Verbose(XOR("GUI::LoadingScreen complete"));
	}
	else
	{
		L::Verbose(XOR("GUI::AuthenticationScreen running"));
		if (!IsSteamOverlayOpen())
			AuthenticationScreen();
		L::Verbose(XOR("GUI::AuthenticationScreen complete"));
	}

	L::Verbose(XOR("Config::ProcessKeys running"));
	Config::ProcessKeys();
	L::Verbose(XOR("Config::ProcessKeys complete"));


	if (UserData::SessionId != "") // if we should be pinging
	{
		double SecondsSincePing = Animation::delta(Animation::now(), UserData::LastPingTime);
		if (SecondsSincePing > 300 && !UserData::PingDebounce) // can ping now
			CreateThread(NULL, 0, UserData::PingServer, (LPVOID)nullptr, 0, NULL);
		else if (SecondsSincePing > 420) // egregious ping delay
			GUI::Ejected = true;
	}
	L::Verbose(XOR("GUI::Main complete"));
}

CHudElement* GUI::FindHudElement(const char* Name)
{
	static void* PHUD = *(void**)(FindPattern("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
	static auto func = (CHudElement * (__thiscall*)(void*, const char*))FindPattern("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28");
	return func(PHUD, Name);
}

void GUI::ResetInventoryHud()
{
	static auto ClearHudWeaponIcon = (int(__thiscall*)(void*, int))FindPattern("client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C");
	CHudElement* hudWeaponSelection = GUI::FindHudElement("CCSGO_HudWeaponSelection");
	if (hudWeaponSelection)
	{
		DWORD hudWeapons = (DWORD)hudWeaponSelection - 0xA0;
		int* weaponCount = (int*)(hudWeapons + 0x80);
		if (hudWeapons && weaponCount)
			while (*weaponCount)
				ClearHudWeaponIcon((void*)(hudWeapons), *weaponCount - 1);
	}
}

bool GUI::IsSteamOverlayOpen()
{
	// this would work, but it will break pretty easily when steam updates
	// and will likely return true always, so the menu will never show up
	//static bool* isOpen = (bool*)((DWORD)GetModuleHandleA("gameoverlayrenderer.dll") + 0x14B7E4);
	//return *isOpen;

	// this solution is probably "worse" but it is less likely to render our
	// cheat useless when it does break
	static int* isOpen = (int*)((DWORD)GetModuleHandleA("gameoverlayrenderer.dll") + 0x14B7E4);
	return *isOpen == 1;
}
