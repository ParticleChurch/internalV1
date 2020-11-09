#include "../Include.hpp"
#include "HTTP.hpp"

// will be set to the screen center
ImVec2 LoginWindowPosition(100, 100);

// Variables to help communicate between main thread and login thread (helping with race conditions)
std::mutex LoginMutex;
unsigned int LoginAttemptIndex = 0;      // equal to the number of times that the user pressed the login button
bool loginCanceled = false;

struct LoginInformation
{
	unsigned int AttemptID;
	char Email[257];
	char Password[65];
};

void AttemptLogin(LoginInformation* Info)
{
	APIResponseFormat_LoginAttempt Response = HTTP::APILogin(Info->Email, Info->Password);
	if (Response.Flags & LOGIN_FLAG_LOGIN_VALID)
	{
		// this user has entered a valid login
		if (Response.Flags & LOGIN_FLAG_BANNED)
		{
			// this user has been banned (todo: print "fuck you" then crash csgo)
			Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;
		}
		else if (Response.Flags & LOGIN_FLAG_ACCOUNT_ALREADY_IN_USE)
		{
			// this user is logged in elsewhere (todo: tell them they should enter free mode)
			Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;
		}
		else
		{
			Config::UserInfo.AuthStatus = AUTH_STATUS_COMPLETE;
			Config::UserInfo.Developer = Response.Flags & LOGIN_FLAG_ACCOUNT_IS_DEVELOPER;
			Config::UserInfo.Paid = Response.Flags & LOGIN_FLAG_ACCOUNT_PAID;
			Config::UserInfo.Email = Info->Email;
			Config::UserInfo.UserID = Response.UserID;
		}
	}
	else
	{
		// invalid login
		Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;
	}

	// deallocate resources
	free(Info);
}

void GUI::ProcessingLoginMenu()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowSize(ImVec2(440, 114));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x/2, io.DisplaySize.y/2), 0, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Processing", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);

	// Cancel button
	ImGui::SetCursorPos(ImVec2(360, 83));
	if (ImGui::Button("Cancel", ImVec2(70, 20)))
		Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;

	LoginWindowPosition = ImGui::GetWindowPos();
	ImGui::End();
}

ImFont* FontDefault;
ImFont* Arial8;
ImFont* Arial12;
ImFont* Arial14;
ImFont* Arial16;
ImFont* Arial18;
ImFont* Arial12Italics;
ImFont* Arial14Italics;
ImFont* Arial16Italics;
#define AYO_LOAD_FONT_BRUH(name, path, size) if (!(name = io.Fonts->AddFontFromFileTTF(path, size))){goto problemo;}
void GUI::LoadFonts(ImGuiIO& io)
{
	FontDefault = io.Fonts->AddFontDefault();
	AYO_LOAD_FONT_BRUH(Arial8, "C:\\Windows\\Fonts\\arial.ttf", 8.f);
	AYO_LOAD_FONT_BRUH(Arial12, "C:\\Windows\\Fonts\\arial.ttf", 12.f);
	AYO_LOAD_FONT_BRUH(Arial14, "C:\\Windows\\Fonts\\arial.ttf", 14.f);
	AYO_LOAD_FONT_BRUH(Arial16, "C:\\Windows\\Fonts\\arial.ttf", 16.f);
	AYO_LOAD_FONT_BRUH(Arial18, "C:\\Windows\\Fonts\\arial.ttf", 18.f);
	AYO_LOAD_FONT_BRUH(Arial12Italics, "C:\\Windows\\Fonts\\ariali.ttf", 12.f);
	AYO_LOAD_FONT_BRUH(Arial14Italics, "C:\\Windows\\Fonts\\ariali.ttf", 14.f);
	AYO_LOAD_FONT_BRUH(Arial16Italics, "C:\\Windows\\Fonts\\ariali.ttf", 16.f);

	return;
problemo:
	MessageBox(NULL, "Particle.church requires that you have the \"Arial\" font (and it's italics version) installed. It comes installed by default with Windows OS in C:/Windows/Fonts. Please download Airal to that location (as arial.ttf and ariali.ttf) then try injecting again.", "Missing Fonts", 0);
	// segfault lol
	int x = *(int*)0;
	// prevent dumbass compiler from ignoring our segfault >:(
	std::cout << x << std::endl;
}
Config::Property* GUI::CurrentlyChoosingKeybindFor = nullptr;
bool GUI::IgnoreLButton = false;

// random imgui utils
namespace ImGui
{
#define vec4toU32(v) ImGui::ColorConvertFloat4ToU32(v)
#define styleColor(r,g,b,a) (ImVec4((float)(r) / 255.f, (float)(g) / 255.f, (float)(b) / 255.f, (float)(a)))

	inline float lerp(float a, float b, float f)
	{
		return (b - a) * f + a;
	}

	inline double lerp(double a, double b, double f)
	{
		return (b - a) * f + a;
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

	template <typename T>
	T clamp(const T& n, const T& lower, const T& upper) {
		return max(lower, min(n, upper));
	}

	inline double easeInOutQuint(double x)
	{
		// https://easings.net/#easeInOutQuint
		return x < 0.5 ? 16.0 * x * x * x * x * x : 1.0 - powf(-2.0 * x + 2.0, 5.0) / 2.0;
	}

	void DrawRect(int x, int y, int w, int h, ImU32 color)
	{
		ImVec2 window = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(window.x + x, window.y + y), ImVec2(window.x + x + w, window.y + y + h), color);
	}

	bool TextInputWithPlaceholder(const char* Placeholder, int width, const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL)
	{
		ImVec2 CursorBefore = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(CursorBefore.x + 3, CursorBefore.y));
		ImGui::SetNextItemWidth(width - 6);
		bool changed = ImGui::InputText(label, buf, buf_size, flags, callback, user_data);

		bool placeholderAbove = ImGui::IsItemActive() || buf[0] != 0;

		if (placeholderAbove)
			ImGui::PushFont(Arial12);

		ImVec4* Colors = ImGui::GetStyle().Colors;
		ImVec4 ColorBefore = Colors[ImGuiCol_Text];
		ImVec2 CursorAfter = ImGui::GetCursorPos();

		ImGui::SetCursorPos(placeholderAbove ? ImVec2(CursorBefore.x, CursorBefore.y - 14) : ImVec2(CursorBefore.x + 3, CursorBefore.y));
		Colors[ImGuiCol_Text] = ImVec4(ColorBefore.x, ColorBefore.y, ColorBefore.z, ColorBefore.w / 2);
		ImGui::Text(Placeholder);

		Colors[ImGuiCol_Text] = ColorBefore;
		ImGui::SetCursorPos(CursorAfter);

		if (placeholderAbove)
			ImGui::PopFont();

		ImGui::DrawRect(CursorBefore.x, CursorBefore.y + ImGui::GetFontSize() + 2, width, 1, ImGui::ColorConvertFloat4ToU32(placeholderAbove ? ColorBefore : ImVec4(ColorBefore.x, ColorBefore.y, ColorBefore.z, ColorBefore.w * 0.75f)));

		return changed;
	}

	void ColorPicker(Color* oColor, std::string id, std::string name, ImVec2 PreviewSize, int PickerWidth = 200)
	{
		float colArray[] = { oColor->color[0] / 255.f , oColor->color[1] / 255.f , oColor->color[2] / 255.f };
		ImVec4 imColor = ImVec4(colArray[0], colArray[1], colArray[2], 1.f);

		// button
		if (ImGui::ColorButton(("###" + id).c_str(), imColor, 0, PreviewSize))
			ImGui::OpenPopup(("###" + id + "-picker").c_str());
		GUI::IgnoreLButton |= ImGui::IsItemHovered();

		// picker
		if (ImGui::BeginPopup(("###" + id + "-picker").c_str()))
		{
			ImGui::SetNextItemWidth(PickerWidth);

			ImGui::ColorPicker3("Preview", colArray, ImGuiColorEditFlags_NoSmallPreview);

			// if we are hovering, ignore because we're changing the color
			// if we are not hovering, ignore because we're closing the color picker
			GUI::IgnoreLButton = true;

			oColor->color[0] = (unsigned char)(colArray[0] * 255);
			oColor->color[1] = (unsigned char)(colArray[1] * 255);
			oColor->color[2] = (unsigned char)(colArray[2] * 255);

			ImGui::EndPopup();
		}
	}

	struct switchData {
		std::chrono::steady_clock::time_point TimeChanged;
		bool LastValue;
		switchData(std::chrono::steady_clock::time_point TimeChanged, bool LastValue)
		{
			this->TimeChanged = TimeChanged;
			this->LastValue = LastValue;
		}
	};
	std::map<ImGuiID, switchData> boolSwitchTimings;
	bool SwitchButton(std::string identifier, ImVec2 size, float animationTime, bool* value, bool clickable = true, int clearance = 2)
	{
		/*
			SETUP
		*/
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(("###" + identifier).c_str());

		// current time
		std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();

		// track this element
		auto timingRecord = boolSwitchTimings.find(id);
		if (timingRecord == boolSwitchTimings.end())
		{
			// say that it switched 10 seconds ago so that we dont play the animation
			boolSwitchTimings.insert(std::make_pair(id, switchData(now - std::chrono::seconds(10), *value)));
			timingRecord = boolSwitchTimings.find(id);
		}
		if (timingRecord->second.LastValue != *value)
			timingRecord->second.TimeChanged = now;
		// seconds
		double timeSinceChange = std::chrono::duration_cast<std::chrono::microseconds>(now - timingRecord->second.TimeChanged).count() / (double)1e6;
		timingRecord->second.LastValue = *value;

		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
		ItemSize(frame_bb, style.FramePadding.y);
		if (!ItemAdd(frame_bb, id, &frame_bb))
			return false;

		/*
			BEHAVIOR
		*/
		bool hovered = false;
		bool held = false;
		bool pressed = clickable && ButtonBehavior(frame_bb, id, &hovered, &held);
		const bool clicked = (hovered && g.IO.MouseClicked[0]);
		if (pressed)
		{
			*value = !(*value);
			MarkItemEdited(id);
			timeSinceChange = 0;
		}

		double animFactor = easeInOutQuint(clamp(timeSinceChange / animationTime, 0.0, 1.0));
		double enabledFactor = /* 1 = enabled, 0 = disabled*/ (*value) ? animFactor : 1.0 - animFactor;

		/*
			DRAW
		*/
		ImVec4 accentColor = style.Colors[ImGuiCol_Button];
		ImVec4 baseColor = style.Colors[ImGuiCol_ButtonHovered];
		ImVec4 backgroundColor = lerp(baseColor, accentColor, enabledFactor);
		//ImVec4 backgroundBorderColor = backgroundColor; backgroundBorderColor.w = 0.2f;
		ImVec4 grabColor = lerp(accentColor, baseColor, enabledFactor);
		//ImVec4 grabBorderColor = grabColor; grabBorderColor.w = 0.2f;

		ImVec2 grabCenter(
			lerp(frame_bb.Min.x + size.y/2, frame_bb.Max.x - size.y / 2, (float)enabledFactor),
			(frame_bb.Min.y + frame_bb.Max.y) / 2.f
		);

		window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, vec4toU32(backgroundColor), size.y / 2 + 2.f, ImDrawCornerFlags_All);
		//window->DrawList->AddRect(frame_bb.Min - ImVec2(1,1), frame_bb.Max + ImVec2(1, 1), vec4toU32(backgroundBorderColor), (size.y + 2.f) / 2 + 2.f, ImDrawCornerFlags_All); // border
		window->DrawList->AddCircleFilled(grabCenter, (size.y / 2) - clearance, vec4toU32(grabColor));
		//window->DrawList->AddCircle(grabCenter, ((size.y / 2) - clearance) + 1.f, vec4toU32(grabBorderColor)); // border

		RenderNavHighlight(frame_bb, id);
		return clicked;
	}

	void DrawBooleanProperty(Config::Property* p)
	{
		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - ImGui::GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); ImGui::SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// 3 cases, in order of importance
		bool IsMidBind = GUI::CurrentlyChoosingKeybindFor == p;
		bool IsBound = p->KeyBind != 0;
		bool IsBindable = p->Keybindability != Config::KeybindOptions::None;

		// move over to second column
		SetCursorPosX(120);

		// universal vars from here on out
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		PushFont(Arial14);
		if (IsMidBind)
		{
			// draw a frame to hold text
			std::string Label = "press any key";
			ImVec2 TextSize = ImGui::CalcTextSize(Label.c_str());

			// size = padding + text + padding
			ImVec2 ChildSize = ImVec2(4 + TextSize.x + 4, 20);
			PushStyleColor(ImGuiCol_FrameBg, styleColor(120, 120, 120, 1.f));
			BeginChildFrame(GetID((p->Name + "-keybind-prompt").c_str()), ChildSize);

			// draw text
			SetCursorPos(ImVec2(4, 3));
			Text(Label.c_str());

			// close child and pop its backgound color
			EndChild(); SameLine();
			PopStyleColor(1);

			// TODO: draw p->Keybindability dropdown

			// go back to top of line
			SetCursorPosY(LinePosY);

			// padding, clear button
			SetCursorPosX(GetCursorPosX() + 5);

			PushStyleColor(ImGuiCol_Button, styleColor(200, 75, 75, 1.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(180, 60, 60, 1.f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(220, 90, 90, 1.f));

			if (ImGui::Button("Clear", ImVec2(40, 20)))
			{
				Config::Unbind(p);
				GUI::CurrentlyChoosingKeybindFor = nullptr;
			}
			GUI::IgnoreLButton |= IsItemHovered();

			PopStyleColor(3);
		}
		else if (IsBound)
		{
			// get keyname text
			std::string KeyName = I::inputsystem->VirtualKeyToString(p->KeyBind);
			ImVec2 KeyTextSize = ImGui::CalcTextSize(KeyName.c_str());

			// draw switch in a child frame contains: padding + switch + padding + keyname + padding
			ImVec2 ChildSize = ImVec2(2 + 30 + 4 + KeyTextSize.x + 4, 20);
			PushStyleColor(ImGuiCol_FrameBg, styleColor(120, 120, 120, 1.f));
			BeginChildFrame(GetID((p->Name + "-keybind-vis").c_str()), ChildSize);

			// center switch on the left
			SetCursorPos(ImVec2(2, 2));
			SwitchButton(p->Name, ImVec2(30, 16), 0.2f, (bool*)p->Value, false); SameLine();

			// draw text on the right
			SetCursorPos(ImVec2(2 + 30 + 4, 3));
			Text(KeyName.c_str());

			// draw an invisible button across the whole child to detect clicks
			PushStyleColor(ImGuiCol_Border, styleColor(0, 0, 0, 0.f));
			PushStyleColor(ImGuiCol_Button, styleColor(0, 0, 0, 0.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(0, 0, 0, 0.1f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(0, 0, 0, 0.2f));

			SetCursorPos(ImVec2(0, 0));
			if (Button(("##" + p->Name + "-binded-button").c_str(), ChildSize))
				GUI::CurrentlyChoosingKeybindFor = p;
			GUI::IgnoreLButton |= IsItemHovered();

			PopStyleColor(4);

			// close child and pop its backgound color
			EndChild();
			PopStyleColor(1);
		}
		else // not bound
		{
			SetCursorPosY(LinePosY + 2); // move down to draw switch in center of line
			SetCursorPosX(GetCursorPosX() + 2); // move over to line up with binded properties
			SwitchButton(p->Name, ImVec2(30, 16), 0.2f, (bool*)p->Value);
			GUI::IgnoreLButton |= IsItemHovered();

			if (IsBindable)
			{
				SameLine(); // move to after the switch
				SetCursorPosY(LinePosY); // move back to top of line
				SetCursorPosX(GetCursorPosX() + 5); // add padding after switch

				// draw "Bind" button
				PushStyleColor(ImGuiCol_Border, styleColor(255, 255, 255, 0.5f));
				PushStyleColor(ImGuiCol_Button, styleColor(120, 120, 120, 1.f));
				PushStyleColor(ImGuiCol_ButtonHovered, styleColor(100, 100, 100, 1.f));
				PushStyleColor(ImGuiCol_ButtonActive, styleColor(160, 160, 160, 1.f));
				PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));

				if (ImGui::Button(("Bind##" + p->Name).c_str(), ImVec2(34, 20)))
					GUI::CurrentlyChoosingKeybindFor = p;
				GUI::IgnoreLButton |= IsItemHovered();
				
				PopStyleColor(4);
				PopStyleVar(2);
			}
		}

		// cleanup, leave cursor on next line
		PopFont();
		PopStyleVar(1);
		SetCursorPosY(LinePosY + 20);
	}

	void DrawColorProperty(Config::Property* p)
	{
		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - ImGui::GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); ImGui::SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// move over to second column
		SetCursorPosX(120);

		// draw color picker preview button thing
		Color* c = (Color*)p->Value;

		PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
		PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.f, 4.f));
		PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.5f)); // white

		SetCursorPosX(GetCursorPosX() + 2); // move over to line up with binded properties
		ColorPicker(c, p->Name, p->VisibleName, ImVec2(30, 20));

		PopStyleVar(3);
		PopStyleColor(1);
	}
}

bool GUI::LoginMenu()
{
	ImGuiIO& io = ImGui::GetIO();
	static bool init = false;
	bool isEjecting = false;
	if (!init)
	{
		init = true;

		ImGuiStyle& style = ImGui::GetStyle();

		// turn off all default rounding and padding
		style.WindowRounding = 6.f;
		style.WindowPadding = ImVec2(0, 0);
		style.ChildRounding = 0.0f;
		style.FrameRounding = 3.f;
		style.FramePadding = ImVec2(0, 0);
		style.GrabRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.FrameBorderSize = 0.f;
		style.ItemSpacing = ImVec2(0, 0);
		style.ItemInnerSpacing = ImVec2(0, 0);
		style.ChildBorderSize = 0.f;
		style.FrameBorderSize = 0.f;
		style.WindowBorderSize = 0.f;
		style.PopupBorderSize = 0.f;
		style.TabBorderSize = 0;

		// main window outline solid 1px white
		style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 1.f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 0.f);

		// buttons
		style.Colors[ImGuiCol_Button] = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 0.f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1, 1, 1, 0.3f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(1, 1, 1, 0.6f);
		style.Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 0.5f);
	}

	ImGui::SetNextWindowSize(ImVec2(340, 110 + 45 + 45 + 45 + 10), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Login", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

	GUI::Rain(ImVec4(.5f, .5f, .5f, 1.f));

	/*
		LOGIN FORM
	*/
	ImGui::PushFont(Arial16);
	ImGui::GetStyle().FrameBorderSize = 0.f;

	// Email
	static char Email[257] = "";
	ImGui::SetCursorPos(ImVec2(20, 32));
	ImGui::TextInputWithPlaceholder("Email Address", 300, "###Email", Email, 257);

	// Password
	static char Password[65] = "";
	ImGui::SetCursorPos(ImVec2(20, 70));
	ImGui::TextInputWithPlaceholder("Password", 300, "###Password", Password, 257);

	ImGui::PopFont();

	/*
		Buttons
	*/
	ImGui::PushFont(Arial16);
	ImGui::GetStyle().FrameBorderSize = 1.f;

	// Login
	ImGui::SetCursorPos(ImVec2(20, 110));
	if (ImGui::Button("Login", ImVec2(300, 35)))
	{
		// reset old data
		LoginAttemptIndex++;
		Config::UserInfo.AuthStatus = AUTH_STATUS_PROCESSING;

		// fill LoginInformation with recent data (malloc thread will still have access once scope deconstructs)
		// p.s. thread has to free this malloc, to prevent a *tiny*, but still existent, memory leak
		LoginInformation* Info = (LoginInformation*)malloc(sizeof(LoginInformation));
		Info->AttemptID = LoginAttemptIndex;
		strcpy(Info->Email, Email);
		strcpy(Info->Password, Password);

		// start login thread
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(AttemptLogin), Info, 0, 0);
		Sleep(0); // let thread begin
	}

	// Play Free
	ImGui::SetCursorPos(ImVec2(20, 110 + 45));
	if (ImGui::Button("Play Free", ImVec2(300, 35)))
	{
		Config::UserInfo.AuthStatus = AUTH_STATUS_COMPLETE;
		Config::UserInfo.Email = "free@a4g4.com";
		Config::UserInfo.UserID = INT_MAX;
		Config::UserInfo.Paid = false;
		Config::UserInfo.Developer = false;
	}

	// Eject
	ImGui::SetCursorPos(ImVec2(20, 110 + 45 + 45));
	isEjecting = ImGui::Button("Eject Now", ImVec2(300, 35));

	// Login
	/*ImGui::GetStyle().FrameBorderSize = 1.f;
	ImGui::SetCursorPos(ImVec2(10, 83));
	if (ImGui::Button("Eject", ImVec2(99, 20)))
		PressedEject = true;

	// Free2Play button
	ImGui::SetCursorPos(ImVec2(10 + 99 + 8, 83));
	if (ImGui::Button("Free Version", ImVec2(99, 20)))
	{
	}

	// Register button
	ImGui::SetCursorPos(ImVec2(10 + 99 + 8 + 99 + 8, 83));
	if (ImGui::Button("Register", ImVec2(99, 20)))
		ShellExecute(NULL, TEXT("open"), TEXT("https://www.a4g4.com/register"), NULL, NULL, 0);

	// Login button
	ImGui::SetCursorPos(ImVec2(10 + 99 + 8 + 99 + 8 + 99 + 8, 83));
	if (ImGui::Button("Login", ImVec2(99, 20)))
	{
		// reset old data
		LoginAttemptIndex++;
		Config::UserInfo.AuthStatus = AUTH_STATUS_PROCESSING;

		// fill LoginInformation with recent data (malloc thread will still have access once scope deconstructs)
		// p.s. thread has to free this malloc, to prevent a *tiny*, but still existent, memory leak
		LoginInformation* Info = (LoginInformation*)malloc(sizeof(LoginInformation));
		Info->AttemptID = LoginAttemptIndex;
		strcpy(Info->Email, Email);
		strcpy(Info->Password, Password);

		// start login thread
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(AttemptLogin), Info, 0, 0);
		Sleep(0); // let thread begin
	}

	LoginWindowPosition = ImGui::GetWindowPos();
	*/
	ImGui::PopFont();

	ImGui::End();
	return isEjecting;
}

bool GUI::Main()
{
	// by default, do not ignore
	// gui code will set to true if hovering over a button
	GUI::IgnoreLButton = false;

	bool isEjecting = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX / 2.f, WindowSizeY / 2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		isEjecting = HackMenu();
		/*
		if (Config::UserInfo.Paid || Config::UserInfo.Developer)
			isEjecting = PaidHackMenu();
		else
			isEjecting = FreeHackMenu();
		*/
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		Config::SetBool("config-show-menu", true);
		ProcessingLoginMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_NONE)
	{
		Config::SetBool("config-show-menu", true);
		isEjecting = LoginMenu();
	}

	return isEjecting;
}

//Paid Hack Func
bool GUI::PaidHackMenu()
{
	bool PressedEject = false;

	if (!Config::GetBool("config-show-menu"))			//if they arent displaying the menu... just return
		return PressedEject;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
	ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoScrollbar);

	ImGui::SetCursorPos(ImVec2(0, 30));
	if (ImGui::Button("Eject", ImVec2(70, 20)))
		PressedEject = true;

	ImGui::End();
	return PressedEject;
}


//Free Hack Func

void Hotkey(int& key, std::string name) noexcept //custom IMGUI shennanigans
{
	ImGui::SameLine();

	ImGui::Text("[ %s ]", key ? I::inputsystem->VirtualKeyToString(key) : "None");

	DWORD menuKey = VK_INSERT;
	
	ImGuiIO& io = ImGui::GetIO();

	if (!ImGui::IsItemHovered())
		return;

	ImGui::SetTooltip("Press any key to change keybind");

	for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) 
		if (ImGui::IsKeyPressed(i) && i != menuKey) 
			key = i != VK_ESCAPE ? i : 0;

	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) 
		if (ImGui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != menuKey) 
			key = i + (i > 1 ? 2 : 1);
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter, static_cast<void*>(&values), values.size());
	}
}

void DisplayLegitTab()
{
	ImGui::Checkbox("Enable###LEnable", &Config::legitbot.Enable);
	if (Config::legitbot.Enable)
		Config::ragebot.Enable = false;
	ImGui::Separator();

	ImGui::Text("Aimbot");
	ImGui::Checkbox("Enable###LEnableAim", &Config::legitbot.EnableAim);
	Hotkey(Config::legitbot.AimKey,"LAimKey");
	ImGui::Checkbox("Team Check###LTeamCheck", &Config::legitbot.TeamCheck);

	ImGui::Separator();
	static int tab = 0;
	if (ImGui::Button("Pistol"))
		tab = 0;
	ImGui::SameLine();
	if (ImGui::Button("Smg"))
		tab = 1;
	ImGui::SameLine();
	if (ImGui::Button("Heavy"))
		tab = 2;
	ImGui::SameLine();
	if (ImGui::Button("Rifle"))
		tab = 3;
	ImGui::SameLine();
	if (ImGui::Button("Sniper"))
		tab = 4;

	ImGui::Separator();
	if (tab == 0)
		ImGui::Text("Pistol");
	if (tab == 1)
		ImGui::Text("Smg");
	if (tab == 2)
		ImGui::Text("Heavy");
	if (tab == 3)
		ImGui::Text("Rifle");
	if (tab == 4)
		ImGui::Text("Sniper");

	ImGui::SliderInt(("Fov###LFov" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].Fov, 0, 90);
	ImGui::SliderInt(("Yaw Speed###LYaw Speed" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].YawSpeed, 0, 100);
	ImGui::SliderInt(("Pitch Speed###LPitch Speed" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].PitchSpeed, 0, 100);
	ImGui::Checkbox(("Recoil Control###LRecoil Control" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].RecoilControl);

	static const char* Hitboxes[] = { "Head", "Pelvis", "Stomach", "Chest", "Legs", "Feet" };
	
	static std::string preview = "";
	if (ImGui::BeginCombo(("Hitboxes##Lcombo" + std::to_string(tab)).c_str(), preview.c_str())) // The second parameter is the label previewed before opening the combo.
	{
		Config::legitbot.weapon[tab].LHitboxes.clear();
		Config::legitbot.weapon[tab].LHitboxes.resize(0);
		Config::legitbot.weapon[tab].Hitboxes.clear();
		Config::legitbot.weapon[tab].Hitboxes.resize(0);
		
		int NumSelected = 0;
		preview = "";
		for (int n = 0; n < IM_ARRAYSIZE(Hitboxes); n++)
		{
			ImGui::Selectable(Hitboxes[n], &Config::legitbot.weapon[tab].HSelected[n], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			if (Config::legitbot.weapon[tab].HSelected[n]) {
				NumSelected++;
				if(NumSelected > 1)
					preview.append(", " + std::string(Hitboxes[n]));
				else
					preview.append(Hitboxes[n]);
				Config::legitbot.weapon[tab].LHitboxes.push_back(Hitboxes[n]);


				if (n == 0)
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_HEAD);
				else if (n == 1)
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_PELVIS);
				else if (n == 2)
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_STOMACH);
				else if (n == 3)
				{
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LOWER_CHEST);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_CHEST);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_UPPER_CHEST);
				}
				else if (n == 4)
				{
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_THIGH);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_THIGH);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_CALF);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_CALF);
				}
				else if (n == 5)
				{
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_FOOT);
					Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_FOOT);
				}
			}
		}
		ImGui::EndCombo();
	}


	static int PriorityChoice = 0;
	static int PrevPriorityChoice = -1;
	ImGui::Combo(("Priority##Lcombo" + std::to_string(tab)).c_str(), &PriorityChoice, Config::legitbot.weapon[tab].LHitboxes);
	if (PrevPriorityChoice != PriorityChoice)
	{
		PrevPriorityChoice = PriorityChoice;
		Config::legitbot.weapon[tab].Priority.clear();
		Config::legitbot.weapon[tab].Priority.resize(0);
		if (PriorityChoice == 0)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_HEAD);
		}
		if (PriorityChoice == 1)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_PELVIS);
		}
		if (PriorityChoice == 2)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_STOMACH);
		}
		if (PriorityChoice == 3)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_LOWER_CHEST);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_CHEST);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_UPPER_CHEST);
		}
		if (PriorityChoice == 4)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_RIGHT_THIGH);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_LEFT_THIGH);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_RIGHT_CALF);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_LEFT_CALF);
		}
		if (PriorityChoice == 5)
		{
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_RIGHT_FOOT);
			Config::legitbot.weapon[tab].Priority.push_back(HITBOX_LEFT_FOOT);
		}
	} 
}

void DisplayRageTab() {
	ImGui::Checkbox("Enable###REnable", &Config::ragebot.Enable);
	if(Config::ragebot.Enable)
		Config::legitbot.Enable = false;
	ImGui::Separator();

	ImGui::Text("General");
	ImGui::Checkbox("Enable###REnableAim", &Config::ragebot.EnableAim);
	if(!Config::ragebot.AutoShoot)
		Hotkey(Config::ragebot.AimKey,"RAimKey");
	ImGui::Checkbox("Auto Shoot###RAutoShoot", &Config::ragebot.AutoShoot);
	ImGui::Checkbox("Silent Aim###RSilentAim", &Config::ragebot.SilentAim);
	ImGui::Checkbox("Team Check###RTeamCheck", &Config::ragebot.TeamCheck);
	ImGui::Checkbox("Aim Step###RAimStep", &Config::ragebot.AimStep);
	ImGui::Checkbox("Auto Scope###RAutoScope", &Config::ragebot.AutoScope);

	ImGui::Separator();
	static int tab = 0;
	if (ImGui::Button("Pistol"))
		tab = 0;
	ImGui::SameLine();
	if (ImGui::Button("Smg"))
		tab = 1;
	ImGui::SameLine();
	if (ImGui::Button("Heavy"))
		tab = 2;
	ImGui::SameLine();
	if (ImGui::Button("Rifle"))
		tab = 3;
	ImGui::SameLine();
	if (ImGui::Button("Scout"))
		tab = 4;
	ImGui::SameLine();
	if (ImGui::Button("AWP"))
		tab = 5;
	ImGui::SameLine();
	if (ImGui::Button("Auto"))
		tab = 6;

	ImGui::Separator();
	if (tab == 0)
		ImGui::Text("Pistol");
	if (tab == 1)
		ImGui::Text("Smg");
	if (tab == 2)
		ImGui::Text("Heavy");
	if (tab == 3)
		ImGui::Text("Rifle");
	if (tab == 4)
		ImGui::Text("Scout");
	if (tab == 5)
		ImGui::Text("AWP");
	if (tab == 6)
		ImGui::Text("Auto");

	static const char* Hitboxes[] = { "Head", "Pelvis", "Stomach", "Chest", "Legs", "Feet" };

	static std::string preview = "";
	if (ImGui::BeginCombo(("Hitboxes##Rcombo" + std::to_string(tab)).c_str(), preview.c_str())) // The second parameter is the label previewed before opening the combo.
	{
		Config::ragebot.weapon[tab].LHitboxes.clear();
		Config::ragebot.weapon[tab].LHitboxes.resize(0);
		int NumSelected = 0;
		preview = "";
		for (int n = 0; n < IM_ARRAYSIZE(Hitboxes); n++)
		{
			ImGui::Selectable(Hitboxes[n], &Config::ragebot.weapon[tab].HSelected[n], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			if (Config::ragebot.weapon[tab].HSelected[n]) {
				NumSelected++;
				if (NumSelected > 1)
					preview.append(", " + std::string(Hitboxes[n]));
				else
					preview.append(Hitboxes[n]);
				Config::ragebot.weapon[tab].LHitboxes.push_back(Hitboxes[n]);
			}
		}
		ImGui::EndCombo();
	}

	if (Config::ragebot.weapon[tab].HSelected[0])
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_HEAD);
	if (Config::ragebot.weapon[tab].HSelected[1])
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_PELVIS);
	if (Config::ragebot.weapon[tab].HSelected[2])
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_STOMACH);
	if (Config::ragebot.weapon[tab].HSelected[3]) {
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_LOWER_CHEST);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_CHEST);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_UPPER_CHEST);
	}
	if (Config::ragebot.weapon[tab].HSelected[4]) {
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_THIGH);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_THIGH);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_CALF);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_CALF);
	}
	if (Config::ragebot.weapon[tab].HSelected[5]) {
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_FOOT);
		Config::ragebot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_FOOT);
	}


	static int PriorityChoice = 0;
	ImGui::Combo(("Priority##Rcombo" + std::to_string(tab)).c_str(), &PriorityChoice, Config::ragebot.weapon[tab].LHitboxes);
	if (PriorityChoice == 0)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_HEAD);
	}
	if (PriorityChoice == 1)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_PELVIS);
	}
	if (PriorityChoice == 2)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_STOMACH);
	}
	if (PriorityChoice == 3)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_LOWER_CHEST);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_CHEST);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_UPPER_CHEST);
	}
	if (PriorityChoice == 4)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_RIGHT_THIGH);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_LEFT_THIGH);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_RIGHT_CALF);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_LEFT_CALF);
	}
	if (PriorityChoice == 5)
	{
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_RIGHT_FOOT);
		Config::ragebot.weapon[tab].Priority.push_back(HITBOX_LEFT_FOOT);
	}

	ImGui::SliderInt(("Visible Min Damage###VisibleMinDamage" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].MinDamageVisible, 0, 120);
	ImGui::SliderInt(("Autowall Min Damage###AutowallMinDamage" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].MinDamageAutowall, 0, 120);
	ImGui::Checkbox(("Body Aim On Lethal###BaimOnLethal" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].BaimOnLethal);
	ImGui::Text("Damage Overide");
	Hotkey(Config::ragebot.weapon[tab].OverideKey,"ROverideKey");
	ImGui::SliderInt(("Overide Damage###OverideDamage" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].OverideDamage, 0, 120);
	ImGui::Checkbox(("Auto Slow###AutoSlow" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].AutoSlow);
	ImGui::SliderInt(("Shot Delay (ms)###ShotDelay" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].ShotDelay, 0, 300);
	ImGui::SliderInt(("Hit Chance (%)###HitChance" + std::to_string(tab)).c_str(), &Config::ragebot.weapon[tab].HitChance, 0, 100);
}

void DisplayAntiAimTab() {
	ImGui::Checkbox("Enable###AAEnable", &Config::antiaim.Enable);
	ImGui::Separator();

	if (Config::antiaim.FakeLag.Enable || Config::antiaim.Legit.Enable || Config::antiaim.Rage.Enable)
	{
		ImGui::Checkbox("Visualize###AAVisualize", &Config::antiaim.Visualize);
		//ImGui::ColorPicker("Fake Color", "Fake Color", &Config::antiaim.Fake);
		ImGui::SliderInt("Opacity", &Config::antiaim.Opacity, 0, 100);
		ImGui::Separator();
	}

	ImGui::Text("FakeLag");
	ImGui::Checkbox("Enable###AAEnableFakeLag", &Config::antiaim.FakeLag.Enable);
	ImGui::SliderInt("Fake Lag Ticks", &Config::antiaim.FakeLag.FakeLagTicks, 0, 16);
	ImGui::Checkbox("Adaptive###AAAdaptive", &Config::antiaim.FakeLag.Adaptive);
	ImGui::Checkbox("Lag On Peak###AALagOnPeak", &Config::antiaim.FakeLag.LagOnPeak);

	ImGui::Separator();

	ImGui::Text("Legit");
	ImGui::Checkbox("Enable###AALegitEnable", &Config::antiaim.Legit.Enable);
	Hotkey(Config::antiaim.Legit.InverterKey, "InverterKey");

	ImGui::Separator();
	ImGui::Text("Rage");
	ImGui::Checkbox("Enable###AARageEnable", &Config::antiaim.Rage.Enable);
	ImGui::SliderInt("Real Jitter Amount", &Config::antiaim.Rage.RealJitterAmount, 0, 58);
	ImGui::Text("Left AA Key:");
	Hotkey(Config::antiaim.Rage.LeftKey, "LeftKey");
	ImGui::Text("Back AA Key:");
	Hotkey(Config::antiaim.Rage.BackKey, "BackKey");
	ImGui::Text("Right AA Key:");
	Hotkey(Config::antiaim.Rage.RightKey, "RightKey");
	ImGui::Checkbox("Add Fake###AARageAddFake", &Config::antiaim.Rage.AddFake);
	ImGui::SliderInt("Fake Jitter Amount", &Config::antiaim.Rage.FakeJitterAmount, 0, 58);	
}

void DisplayMovementTab() {
	ImGui::Checkbox("Enable###MoveEnable", &Config::movement.Enable);
	ImGui::Separator();

	ImGui::Checkbox("Bunny Hop###BHOP", &Config::movement.BunnyHop);
	ImGui::Checkbox("Fast Crouch###FastCrouch", &Config::movement.FastCrouch);
	ImGui::Checkbox("Legit Auto Strafe###LegitAutoStrafe", &Config::movement.LegitAutoStrafe);
	if (Config::movement.LegitAutoStrafe)
		Config::movement.RageAutoStrafe = false;
	ImGui::Checkbox("Rage Auto Strafe###RageAutoStrafe", &Config::movement.RageAutoStrafe);
	if (Config::movement.RageAutoStrafe)
		Config::movement.LegitAutoStrafe = false;
	ImGui::Checkbox("Slow Walk###SlowWalk", &Config::movement.SlowWalk);
	Hotkey(Config::movement.SlowWalkKey, "SlowWalkKey");
	ImGui::Checkbox("Slide Walk###SlideWalk", &Config::movement.SlideWalk);
}

void DisplayVisualsTab() {
	ImGui::Checkbox("Enable###VisualsEnable", &Config::visuals.Enable);
	ImGui::Separator();

	ImGui::Text("General");
	ImGui::Checkbox("Recoil Crosshair###VRecoilCrosshair", &Config::visuals.RecoilCrosshair);
	ImGui::Checkbox("ThirdPerson###VThirdPerson", &Config::visuals.ThirdPerson);
	Hotkey(Config::visuals.ThirdPersonKey, "ThirdPersonKey");
	ImGui::Separator();

	ImGui::Text("World");
	ImGui::Checkbox("Asus Walls###VAsusWalls", &Config::visuals.AsusWalls);
	ImGui::SliderInt("Asus Wall Opacity (%)###VAsusWallsValue", &Config::visuals.AsusWallVal, 0, 100);
	ImGui::Checkbox("Asus Props###VAsusProps", &Config::visuals.AsusProps);
	ImGui::SliderInt("Asus Props Opacity (%)###VAsusPropsValue", &Config::visuals.AsusPropVal, 0, 100);
	ImGui::Separator();

	ImGui::Text("Removals");
	ImGui::Checkbox("No Flash###VNoFlash", &Config::visuals.NoFlash);
	ImGui::Checkbox("No Smoke###VNoSmoke", &Config::visuals.NoSmoke);
	ImGui::Checkbox("No Scope###VNoScope", &Config::visuals.NoScope);
	ImGui::Separator();

	ImGui::Text("Players - ESP");
	ImGui::Checkbox("Radar###VRadar", &Config::visuals.Radar);
	ImGui::Checkbox("Boxes###VBoxes", &Config::visuals.Boxes);
	//ImGui::ColorPicker("Box Color", "Box Color", &Config::visuals.BoxColor);
	ImGui::Checkbox("Skeleton###VSkeleton", &Config::visuals.Skeleton);
	ImGui::Checkbox("Name###VName", &Config::visuals.Name);
	ImGui::Checkbox("Health###VHealth", &Config::visuals.Health);
	ImGui::Checkbox("Ammo###VAmmo", &Config::visuals.Ammo);
	ImGui::Separator();

	ImGui::Text("Players - Chams");
	ImGui::Checkbox("Visible Chams###VVisibleCHams", &Config::visuals.VisibleChams);
	//ImGui::ColorPicker("Visible Cham Color", "Visible Cham Color", &Config::visuals.VisibleColor);
	ImGui::Checkbox("Through Wall Chams###VWallCHams", &Config::visuals.ThroughWallChams);
	//ImGui::ColorPicker("Through Wall Cham Color", "Through Wall Cham Color", &Config::visuals.ThroughWallColor);

}

Config::Tab* CurrentTab = 0;
bool HackMenuPageHasScrollbar = false;
bool GUI::HackMenu()
{
	if (!Config::GetBool("config-show-menu"))
		return false;

	bool eject = false;
	if ((!CurrentTab) && (Config::Tabs.size() > 0))
		CurrentTab = Config::Tabs.at(0);

	ImGuiIO& io = ImGui::GetIO();

	// dimensions
	ImVec2 WindowCenter(io.DisplaySize.x / 2, io.DisplaySize.y / 2);
	ImGui::SetNextWindowPos(WindowCenter, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(WindowCenter, ImGuiCond_Once);

	// Styles
	int TitleBarHeight = 16;
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize = ImVec2(400, 5 + 30 * (Config::Tabs.size() + 1) + TitleBarHeight);
	style.FrameBorderSize = 0.f;
	style.ChildRounding = 0.f;
	style.WindowBorderSize = 0.f;
	style.WindowRounding = 5;
	style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(50.f, 50.f, 50.f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_TitleBgActive] = ImVec4(20.f / 255.f, 20.f / 255.f, 20.f / 255.f, 1.f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(30.f / 255.f, 30.f / 255.f, 30.f / 255.f, 0.5f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.f, 1.f, 1.f, 1.f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	
	// set title bar size to 16px with font = Arial16
	ImFont* font_before = ImGui::GetFont();
	ImGui::PushFont(Arial14Italics);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, (TitleBarHeight - ImGui::GetFontSize()) / 2.f));
	ImGui::Begin("PARTICLE.CHURCH - PRIVATE BETA v1.0.3", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	GUI::IgnoreLButton |= ImGui::IsItemHovered();
	ImGui::PushFont(font_before);
	ImGui::PopStyleVar();
	ImVec2 WindowPos = ImGui::GetWindowPos();

	// draw tab handles
	ImDrawList* DrawList = ImGui::GetWindowDrawList();
	ImU32 UnselectedTabHandleColor = vec4toU32(ImVec4(15.f / 255.f, 15.f / 255.f, 15.f / 255.f, 1.f));
	ImU32 SelectedTabHandleColor = vec4toU32(ImVec4(50.f / 255.f, 50.f / 255.f, 50.f / 255.f, 1.f));
	ImGui::PushFont(Arial16);
	for (size_t i = 0; i < Config::Tabs.size(); i++)
	{
		Config::Tab* Tab = Config::Tabs.at(i);
		ImVec2 TabHandlePos = ImVec2(WindowPos.x, TitleBarHeight + WindowPos.y + (30 * i));
		bool hovering = ImGui::IsMouseHoveringRect(TabHandlePos, TabHandlePos + ImVec2(90, 30), false);
		bool IsCurrent = Tab == CurrentTab;
		GUI::IgnoreLButton |= hovering && !IsCurrent;

		ImVec2 TabHandleSize = ImVec2(90, 30);
		if (!IsCurrent)
		{
			TabHandleSize.x -= hovering ? 5 : 10;
			TabHandlePos.x += hovering ? 5 : 10;
		}

		DrawList->AddRectFilled(
			TabHandlePos,
			TabHandlePos + TabHandleSize,
			IsCurrent ? SelectedTabHandleColor : UnselectedTabHandleColor,
			6.f, ImDrawCornerFlags_Left
		);
		ImGui::SetCursorPos(TabHandlePos - WindowPos);
		if (ImGui::Button(Tab->Name.c_str(), TabHandleSize))
			CurrentTab = Tab;
		if (!IsCurrent && ImGui::IsItemHovered())
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	// draw eject button
	{
		ImVec2 TabHandlePos = ImVec2(WindowPos.x + 5, WindowPos.y + TitleBarHeight + 30 * Config::Tabs.size() + 5);
		bool hovering = ImGui::IsMouseHoveringRect(TabHandlePos, TabHandlePos + ImVec2(85, 30), false);
		if (!hovering) TabHandlePos.x += 5;
		ImVec2 TabHandleSize = hovering ? ImVec2(85, 30) : ImVec2(80, 30);
		GUI::IgnoreLButton |= hovering; // i mean... if you're clicking eject, does it really matter?

		DrawList->AddRectFilled(
			TabHandlePos,
			TabHandlePos + TabHandleSize,
			vec4toU32(ImVec4(200.f / 255.f, 75.f / 255.f, 75.f / 255.f, 1.f)),
			6.f, ImDrawCornerFlags_Left
		);
		ImGui::SetCursorPos(TabHandlePos - WindowPos);
		ImGui::PushFont(Arial16Italics);
		eject = ImGui::Button("Eject", TabHandleSize);
		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		ImGui::PopFont();
	}
	ImGui::PopFont();

	// check that we have a selected tab
	if (!CurrentTab)
	{
		eject = true;
		goto END;
	}

	// draw selected tab
	ImGui::SetCursorPos(ImVec2(90, TitleBarHeight));

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(50.f / 255.f, 50.f / 255.f, 50.f / 255.f, 1.f));
	ImGui::BeginChildFrame(ImGui::GetID("###page"), ImVec2(0, 0), 0);
	ImGui::PopStyleColor();

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 1.f, 1.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.f, 1.f, 1.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(1.f, 1.f, 1.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(1.f, 1.f, 1.f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(75.f / 255.f, 75.f / 255.f, 75.f / 255.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_Button, Config::GetColor("menu-accent-color"));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(230.f / 255.f, 230.f / 255.f, 230.f / 255.f, 1.f));
	
	for (size_t i = 0; i < CurrentTab->Widgets.size(); i++)
	{
		ImGui::SetCursorPos(ImVec2(5, ImGui::GetCursorPosY() + 5));


		Config::Widget* Widget = CurrentTab->Widgets.at(i);
		if (!ImGui::BeginChildFrame(
			ImGui::GetID(Widget->Name.c_str()),
			ImVec2(ImGui::GetWindowWidth() - (HackMenuPageHasScrollbar ? 24 : 10), Widget->Height + 6),
			0 //ImGuiWindowFlags_NoScrollbar
		))
		{
			ImGui::EndChild();
			continue;
		}

		// title
		ImGui::PushFont(Arial18);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::Text(Widget->Name.c_str());
		ImGui::PopFont();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);

		// properties
		ImGui::PushFont(Arial16);
		for (size_t j = 0; j < Widget->Properties.size(); j++)
		{
			Config::Property* Property = Widget->Properties.at(j);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

			switch (Property->Type)
			{
			case Config::PropertyType::BOOLEAN:
			{
				ImGui::DrawBooleanProperty(Property);
				break;
			}
			case Config::PropertyType::FLOAT:
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
				Config::CFloat* f = (Config::CFloat*)Property->Value;
				float v = f->get();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
				ImGui::Text((Property->VisibleName + ": " + f->Stringify()).c_str());

				// Custom slider because imgui slider is hot garbage
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
				ImGui::SliderFloatEx(("###" + Property->Name).c_str(), &v, f->minimum, f->maximum, "", 1.f);
				GUI::IgnoreLButton |= ImGui::IsItemHovered();

				f->set(v);

				break;
			}
			case Config::PropertyType::COLOR:
			{
				ImGui::DrawColorProperty(Property);
				break;
			}
			default:
			{
				// TODO:
				ImGui::Text((Property->VisibleName + ": " + Property->Stringify()).c_str());
				break;
			}
			}
		}
		ImGui::PopFont();
		Widget->Height = ImGui::GetCurrentWindow()->ContentSize.y;
		ImGui::EndChild();
	}
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(7);
	HackMenuPageHasScrollbar = ImGui::GetContentRegionAvail().y < 0; // its gonna be a frame late, rip
	ImGui::EndChildFrame();

END:
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopFont();
	return eject;
}

bool GUI::FreeHackMenu()
{
	bool PressedEject = false;

	if (!Config::GetBool("config-show-menu"))			//if they arent displaying the menu... just return
		return PressedEject;

	static bool init = false;
	if (!init)
	{
		init = true;
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowRounding = 0.0f;
		style->WindowPadding = ImVec2(15, 15);
		style->ChildRounding = 0.0f;
		style->FrameRounding = 0.0f; //rounding for button
		style->FramePadding = ImVec2(7, 7);
		style->GrabRounding = 0.0f;
		style->PopupRounding = 0.0f;
		style->ScrollbarRounding = 0.0f;
		style->FrameBorderSize = 0.f;
		style->ItemSpacing = ImVec2(15, 15);
		style->ItemInnerSpacing = ImVec2(5, 5);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 1.f);

		style->Colors[ImGuiCol_Button]				= ImVec4(1.f, 1.f, 1.f, 0.f);
		style->Colors[ImGuiCol_ButtonHovered]		= ImVec4(1.f, 1.f, 1.f, 0.4f);
		style->Colors[ImGuiCol_ButtonActive]		= ImVec4(1.f, 1.f, 1.f, 0.0f);
		style->Colors[ImGuiCol_CheckMark]			= ImVec4(1.f, 1.f, 1.f, 1.f);
		style->Colors[ImGuiCol_FrameBg]				= ImVec4(1.f, 1.f, 1.f, 0.f);// Background of checkbox, radio button, plot, slider, text input
		style->Colors[ImGuiCol_FrameBgHovered]		= ImVec4(1.f, 1.f, 1.f, 0.1f);
		style->Colors[ImGuiCol_FrameBgActive]		= ImVec4(1.f, 1.f, 1.f, 0.f);
		style->Colors[ImGuiCol_FrameBgHovered]		= ImVec4(1.f, 1.f, 1.f, 0.f);// Background of checkbox, radio button, plot, slider, text input
		style->Colors[ImGuiCol_FrameBgActive]		= ImVec4(1.f, 1.f, 1.f, 0.f);// Background of checkbox, radio button, plot, slider, text input
		style->Colors[ImGuiCol_SliderGrab]			= ImVec4(1.f, 1.f, 1.f, 1.f);
		style->Colors[ImGuiCol_SliderGrabActive]	= ImVec4(1.f, 1.f, 1.f, 1.f);

		style->Colors[ImGuiCol_Header]				= ImVec4(1.f, 1.f, 1.f, 0.2f); // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style->Colors[ImGuiCol_HeaderHovered]		= ImVec4(1.f, 1.f, 1.f, 0.4f); // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style->Colors[ImGuiCol_HeaderActive]		= ImVec4(1.f, 1.f, 1.f, 0.4f); // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem	
	}
	

	ImGui::SetNextWindowSize(ImVec2(500, 800), ImGuiCond_Appearing);
	ImGui::Begin("Hack (Free Version)", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
	GUI::Rain();
	
	ImGui::GetStyle().Alpha = 1;
	ImGui::GetStyle().FrameBorderSize = 0.5f;
	ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.f, 1.f, 1.f, 1.f);

	static bool DisplayLegit = true;
	static bool DisplayRage = false;
	static bool DisplayAA = false;
	static bool DisplayMovement = false;
	static bool DisplayVisuals = false;

	if (ImGui::Button("Legit"))
	{
		
		DisplayLegit = true;
		DisplayRage = false;
		DisplayAA = false;
		DisplayMovement = false;
		DisplayVisuals = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Rage"))
	{
		Config::legitbot.Enable = false;
		DisplayLegit = false;
		DisplayRage = true;
		DisplayAA = false;
		DisplayMovement = false;
		DisplayVisuals = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Anti-Aim"))
	{
		DisplayLegit = false;
		DisplayRage = false;
		DisplayAA = true;
		DisplayMovement = false;
		DisplayVisuals = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Movement"))
	{
		DisplayLegit = false;
		DisplayRage = false;
		DisplayAA = false;
		DisplayMovement = true;
		DisplayVisuals = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Visuals"))
	{
		DisplayLegit = false;
		DisplayRage = false;
		DisplayAA = false;
		DisplayMovement = false;
		DisplayVisuals = true;
	}

	ImGui::Separator();

	if (DisplayLegit)
	{
		DisplayLegitTab();
	}
	if (DisplayRage)
	{
		DisplayRageTab();
	}
	if (DisplayAA)
	{
		DisplayAntiAimTab();
	}
	if (DisplayMovement)
	{
		DisplayMovementTab();
	}
	if (DisplayVisuals)
	{
		DisplayVisualsTab();
	}

	ImGui::Separator();
	//ImGui::SetCursorPos(ImVec2(0, 30)); //leaving eject at end lol
	if (ImGui::Button("Eject"))//, ImVec2(70, 20)))
		PressedEject = true;

	ImGui::GetStyle().Alpha = 0.7f;
	ImGui::GetStyle().FrameBorderSize = 0.f;
	ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.f, 1.f, 1.f, 0.f);

	ImGui::End();

	return PressedEject;
}


#define randf() (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
#define randint(m, n) (int)(floor(randf() * (n + 1.f - m) + m))
#define square(x) (x)*(x)

struct Raindrop {
	float x, y, z;
	int height;
	float speed; // pixels per second
	float opacity;

	Raindrop(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->z = pow(randf(), 2.f);
		this->opacity = ImGui::lerp(0.2f, 1.f, this->z);
		this->speed = ImGui::lerp(15.f, 30.f, this->z);
		this->height = floor(ImGui::lerp(8.f, 15.f, this->z));
	}
};

/*
	From here to EOF is for drawing rain
	ported from javascript on a4g4 lol
*/
std::chrono::steady_clock::time_point rain_time = std::chrono::high_resolution_clock::now();
std::vector<Raindrop> rain_drops;
ImVec2 rain_windowPos;
int rain_lx = 0;
int rain_ly = 0;
int rain_cx = 0;
int rain_cy = 0;
float rain_density = 0.2f;

void GUI::Rain(ImVec4 Color)
{
	static bool rain_random_init = false;
	if (!rain_random_init)
	{
		rain_random_init = true;
		srand(time(nullptr));
	}
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	rain_windowPos = ImGui::GetWindowPos();
	rain_cx = ImGui::GetWindowWidth();
	rain_cy = ImGui::GetWindowHeight();

	int newXRect[] = { rain_lx, -15, rain_cx, rain_cy };
	int newYRect[] = { 0, rain_ly, rain_lx, rain_cy };

	if (rain_cx > rain_lx)
	{
		int pixelsPerRaindrop = (1000.f / rain_density) / (float)(max(newXRect[3], 1));
		float count = float(newXRect[2] - newXRect[0]) / float(pixelsPerRaindrop);
		if (randf() < count) // don't ask
			for (int i = newXRect[0]; i < newXRect[2]; i += pixelsPerRaindrop)
			{
				Raindrop drop(ImGui::lerp(newXRect[0], newXRect[2], randf()), ImGui::lerp(newXRect[1], newXRect[3], randf()));
				rain_drops.push_back(drop);
			}
	}
	else if (rain_cx < rain_lx)
	{
		for (size_t i = 0; i < rain_drops.size(); i++)
		{
			if (rain_drops.at(i).x > rain_cx)
			{
				rain_drops.erase(rain_drops.begin() + i);
				i--;
			}
		}
	}

	if (rain_cy > rain_ly)
	{
		int pixelsPerRaindrop = (1000.f / rain_density) / (float)(max(newYRect[2], 1));
		float count = float(newYRect[3] - newXRect[1]) / float(pixelsPerRaindrop);
		if (randf() < count) // blah blah if count < 1 still needa chance but random to decide
			for (int i = newYRect[1]; i < newYRect[3]; i += pixelsPerRaindrop)
			{
				Raindrop drop(ImGui::lerp(newYRect[0], newYRect[2], randf()), ImGui::lerp(newYRect[1], newYRect[3], randf()));
				rain_drops.push_back(drop);
			}
	}
	else if (rain_cy < rain_ly)
	{
		for (size_t i = 0; i < rain_drops.size(); i++)
		{
			if (rain_drops.at(i).y >= rain_cy)
			{
				rain_drops.erase(rain_drops.begin() + i);
				i--;
			}
		}
	}

	auto now = std::chrono::high_resolution_clock::now();
	double secondsPassed = ((double)std::chrono::duration_cast<std::chrono::microseconds>(now - rain_time).count()) / 1e6f;
	rain_time = now;

	ImVec2 top, bottom;
	for (size_t i = 0; i < rain_drops.size(); i++)
	{
		Raindrop* drop = &rain_drops.at(i);
		if (drop->y > rain_cy)
		{ // hit bottom as result of resize
			rain_drops.erase(rain_drops.begin() + i);
			i--;
			continue;
		}

		drop->y += drop->speed * secondsPassed;
		if (drop->y > rain_cy)
		{ // hit bottom as result of gravity
			drop->y = -15.f;
			drop->x = ImGui::lerp(0, rain_cx, randf());
		}

		top.x = rain_windowPos.x + drop->x;
		bottom.x = top.x + 1;

		top.y = rain_windowPos.y + drop->y;
		bottom.y = top.y + drop->height;

		draw_list->AddRectFilled(
				top,
				bottom,
				ImGui::ColorConvertFloat4ToU32(ImVec4(Color.x, Color.y, Color.z, Color.w * drop->opacity))
			);
	}

	rain_lx = rain_cx;
	rain_ly = rain_cy;
}