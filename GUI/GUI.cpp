#include "../Include.hpp"
#include "HTTP.hpp"
// TODO: organize GUI namespace

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
	inline T clamp(const T& n, const T& lower, const T& upper) {
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
		ImVec4 accentColor = Config::GetColor("menu-property-accent-color");
		ImVec4 baseColor = Config::GetColor("menu-property-base-color");
		baseColor.w = accentColor.w = style.Alpha;
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
		int TextOffset = (20 - GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// 3 cases, in order of importance
		bool IsMidBind = GUI::CurrentlyChoosingKeybindFor == p;
		bool IsBound = p->KeyBind != 0;
		bool IsBindable = p->Keybindability.Count() > 0;

		// move over to second column
		SetCursorPosX(GUI::PropertyLabelsWidth);

		// universal vars from here on out
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		PushFont(Arial14);
		if (IsMidBind)
		{
			// draw a frame to hold text
			std::string Label = "press any key";
			ImVec2 TextSize = CalcTextSize(Label.c_str());

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

			// Dropdown for Keybindability
			SetCursorPosX(GetCursorPosX() + 5);
			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, (20 - GetFontSize()) / 2));
			SetNextItemWidth(115);
			if (ImGui::BeginCombo(("##" + p->Name + "-keybind-mode-dropdown").c_str(), Config::StringifyKeybindType(p->BindType).c_str(), 0))
			{
				GUI::IgnoreLButton = true;
				for (int i = 0; i < 3; i++)
				{
					Config::KeybindType bt = (Config::KeybindType)i;
					if (p->Keybindability.IsEnabled(bt))
					{
						std::string txt = Config::StringifyKeybindType(bt).c_str();
						const bool is_selected = bt == p->BindType;
						if (Selectable(txt.c_str(), is_selected))
							p->BindType = bt;
						if (is_selected)
							SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			GUI::IgnoreLButton |= IsItemHovered();
			PopStyleVar();
			SameLine();


			// go back to top of line
			SetCursorPosY(LinePosY);

			// padding, clear button
			SetCursorPosX(GetCursorPosX() + 5);

			PushStyleColor(ImGuiCol_Button, styleColor(200, 75, 75, 1.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(180, 60, 60, 1.f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(220, 90, 90, 1.f));

			if (Button("Clear", ImVec2(40, 20)))
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
			std::string KeyName = Config::StringifyVK(p->KeyBind);
			ImVec2 KeyTextSize = CalcTextSize(KeyName.c_str());

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

				if (Button(("Bind##" + p->Name).c_str(), ImVec2(34, 20)))
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
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// move over to second column
		SetCursorPosX(GUI::PropertyLabelsWidth);

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

	void DrawFloatProperty(Config::Property* p, int GrabClearance = 2)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		const ImGuiStyle& style = g.Style;

		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); SameLine();

		// draw slider
		// AbsWidth = 120 (for label) + 2 (padding to line up with switches) + slider + 5 + 100 (label and unit)
		// solve for slider width: slider = AbsWidth - 120 - 2 - 5 - 100;
		int AbsWidth = GetContentRegionMaxAbs().x - window->Pos.x;
		int SliderWidth = AbsWidth - GUI::PropertyLabelsWidth - 2 - 5 - 100;
		int SliderHeight = 16;
		ImVec2 SliderCursorPos(GUI::PropertyLabelsWidth + 2, (20 - SliderHeight) / 2 + LinePosY);
		ImVec2 SliderPos = window->Pos - window->Scroll + SliderCursorPos;
		ImVec2 SliderSize(SliderWidth, SliderHeight);
		ImRect SliderBB(SliderPos, SliderPos + SliderSize);
		ImGuiID SliderID = GetID((p->Name + "-slider").c_str());

		// register this as a hoverable item in imgui
		ItemAdd(SliderBB, SliderID);
		bool hovered = ItemHoverable(SliderBB, SliderID);
		bool clicked = (hovered && g.IO.MouseClicked[0]);
		if (clicked)
		{
			SetActiveID(SliderID, window);
			SetFocusID(SliderID, window);
			FocusWindow(window);
		}
		GUI::IgnoreLButton |= hovered;

		// calculations
		Config::CFloat* CValue = (Config::CFloat*)p->Value;
		double Factor = (CValue->get() - CValue->minimum) / (CValue->maximum - CValue->minimum);
		int SliderLeftCenterX = SliderBB.Min.x + SliderHeight / 2;
		int SliderRightCenterX = SliderBB.Max.x - SliderHeight / 2;
		int SliderCenterY = SliderBB.Min.y + SliderHeight / 2;
		int SliderEndsCenterWidth = SliderRightCenterX - SliderLeftCenterX;
		int PixelsMoveGrab = round((double)SliderEndsCenterWidth * Factor);

		// draw background
		ImVec4 v4AccentColor = Config::GetColor("menu-property-accent-color");
		ImVec4 v4BaseColor = Config::GetColor("menu-property-base-color");
		v4BaseColor.w = v4AccentColor.w = style.Alpha;
		ImU32 AccentColor = vec4toU32(v4AccentColor);
		ImU32 BaseColor = vec4toU32(v4BaseColor);

		window->DrawList->AddRectFilled(SliderBB.Min, SliderBB.Max, AccentColor, SliderHeight / 2 + 2);
		window->DrawList->AddRectFilled(SliderBB.Min + ImVec2(2, 2), SliderBB.Max - ImVec2(2, 2), BaseColor, SliderHeight / 2 + 2);

		// draw bar + grab
		ImVec2 GrabCenter(SliderLeftCenterX + PixelsMoveGrab, SliderCenterY);
		window->DrawList->AddRectFilled(SliderBB.Min, ImVec2(GrabCenter.x + SliderHeight / 2, SliderBB.Max.y), AccentColor, SliderHeight / 2 + 2);
		window->DrawList->AddCircleFilled(GrabCenter, SliderHeight / 2 - GrabClearance, BaseColor);

		// slider behavior
		if (g.ActiveId == SliderID)
		{
			if (g.ActiveIdSource == ImGuiInputSource_Mouse && g.IO.MouseDown[0])
			{
				float MousePosX = g.IO.MousePos.x;
				float MouseClickedFactor = ((float)MousePosX - (float)SliderLeftCenterX) / (float)SliderEndsCenterWidth;
				MouseClickedFactor = clamp(MouseClickedFactor, 0.f, 1.f);
				float ClickedValue = MouseClickedFactor * (CValue->maximum - CValue->minimum) + CValue->minimum;
				CValue->set(ClickedValue);
			}
			else
				ClearActiveID(); // only allow mouse1 activation
		}
		// input and unit, unit gets 25px, input gets 65, 10 goes to padding
		PushFont(Arial14);

		// draw input box
		SetCursorPosX(AbsWidth - 100);
		SetCursorPosY(LinePosY);
		float v = CValue->get();
		
		SetNextItemWidth(65);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20 - GetFontSize()) / 2, (20 - GetFontSize()) / 2));
		PushStyleColor(ImGuiCol_Border, styleColor(255, 255, 255, 0.5f));
		PushStyleColor(ImGuiCol_FrameBg, styleColor(120, 120, 120, 1.f));
		PushStyleColor(ImGuiCol_FrameBgHovered, styleColor(100, 100, 100, 1.f));
		PushStyleColor(ImGuiCol_FrameBgActive, styleColor(120, 120, 120, 1.f));

		if (InputFloat(("##" + p->Name + "-input").c_str(), &v, 0.f, 0.f, ("%." + std::to_string(CValue->decimals) + "f").c_str()))
			CValue->set(v);
		GUI::IgnoreLButton |= IsItemHovered();

		PopStyleColor(4);
		PopStyleVar(1);
		SameLine();

		// draw unit
		SetCursorPosX(AbsWidth - 25 - 6);
		SetCursorPosY(LinePosY);
		TextEx(p->UnitLabel.c_str());

		PopFont();

		// put cursor on the next line
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
	}

	void DrawDropdownProperty(Config::Property* p)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		const ImGuiStyle& style = g.Style;

		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// move over to second column
		SetCursorPosX(GUI::PropertyLabelsWidth);

		// draw the dropdown
		Config::CDropdown* DD = (Config::CDropdown*)p->Value;
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, (20 - GetFontSize()) / 2));
		SetNextItemWidth(GetContentRegionMaxAbs().x - window->Pos.x - GUI::PropertyLabelsWidth - 5);
		if (ImGui::BeginCombo(("##" + p->Name).c_str(), DD->Stringify(DD->GetSelection()).c_str(), 0))
		{
			GUI::IgnoreLButton = true;
			size_t CurrentSelection = DD->GetSelection();
			for (size_t i = 0; i < DD->Options.size(); i++)
			{
				const bool IsSelection = i == CurrentSelection;
				std::string txt = DD->Stringify(i);
				if (Selectable(txt.c_str(), IsSelection))
					DD->Select(i);
				if (IsSelection)
					SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		GUI::IgnoreLButton |= IsItemHovered();
		PopStyleVar();

		// put cursor on the next line
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
	}

	void DrawInverterProperty(Config::Property* p)
	{
		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		// line height = 20px, move down to draw text in center
		int TextOffset = (20 - GetFontSize()) / 2;
		SetCursorPosY(LinePosY + TextOffset);

		// draw property name
		Text(p->VisibleName.c_str()); SameLine();

		// move back up to top of line
		SetCursorPosY(LinePosY);

		// 3 cases, in order of importance
		bool IsMidBind = GUI::CurrentlyChoosingKeybindFor == p;
		bool IsBound = p->KeyBind != 0;

		// move over to second column
		SetCursorPosX(GUI::PropertyLabelsWidth);

		// universal vars from here on out
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		PushFont(Arial14);
		if (IsMidBind)
		{
			// draw a frame to hold text
			std::string Label = "press any key";
			ImVec2 TextSize = CalcTextSize(Label.c_str());

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

			// go back to top of line
			SetCursorPosY(LinePosY);

			// padding, clear button
			SetCursorPosX(GetCursorPosX() + 5);

			PushStyleColor(ImGuiCol_Button, styleColor(200, 75, 75, 1.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(180, 60, 60, 1.f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(220, 90, 90, 1.f));

			if (Button("Clear", ImVec2(40, 20)))
			{
				Config::Unbind(p);
				GUI::CurrentlyChoosingKeybindFor = nullptr;
			}
			GUI::IgnoreLButton |= IsItemHovered();

			PopStyleColor(3);
		}
		else if (IsBound)
		{
			std::string ButtonText = Config::StringifyVK(p->KeyBind);
			ImVec2 TextSize = CalcTextSize(ButtonText.c_str());

			PushStyleColor(ImGuiCol_Border, styleColor(255, 255, 255, 0.5f));
			PushStyleColor(ImGuiCol_Button, styleColor(120, 120, 120, 1.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(100, 100, 100, 1.f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(160, 160, 160, 1.f));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));

			if (Button((ButtonText + "##" + p->Name).c_str(), ImVec2(TextSize.x + 10, 20)))
				GUI::CurrentlyChoosingKeybindFor = p;
			GUI::IgnoreLButton |= IsItemHovered();

			PopStyleColor(4);
			PopStyleVar(2);
		}
		else // not bound
		{
			ImVec4 Color1 = Config::GetColor("menu-property-accent-color");
			ImVec4 Color2 = Config::GetColor("menu-property-base-color");
			
			PushStyleColor(ImGuiCol_Button, Color1);
			PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color1, Color2, 0.25f));
			PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color1, Color2, 0.5f));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
			if (Button(("Invert##" + p->Name + "-button").c_str(), ImVec2(40, 20)))
			{
				((Config::CInverter*)p->Value)->State = !((Config::CInverter*)p->Value)->State;
			}
			GUI::IgnoreLButton |= IsItemHovered();
			PopStyleColor(3);
			PopStyleVar(1);

			SameLine(); // move to after the invert btn
			SetCursorPosY(LinePosY); // move back to top of line
			SetCursorPosX(GetCursorPosX() + 5); // add padding after switch

			// draw "Bind" button
			PushStyleColor(ImGuiCol_Border, styleColor(255, 255, 255, 0.5f));
			PushStyleColor(ImGuiCol_Button, styleColor(120, 120, 120, 1.f));
			PushStyleColor(ImGuiCol_ButtonHovered, styleColor(100, 100, 100, 1.f));
			PushStyleColor(ImGuiCol_ButtonActive, styleColor(160, 160, 160, 1.f));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));

			if (Button(("Bind##" + p->Name).c_str(), ImVec2(34, 20)))
				GUI::CurrentlyChoosingKeybindFor = p;
			GUI::IgnoreLButton |= IsItemHovered();

			PopStyleColor(4);
			PopStyleVar(2);
		}

		std::string StateText = ((Config::CInverter*)p->Value)->Stringify();
		if (StateText.size() > 0 && !IsMidBind)
		{
			PushFont(Arial16);
			SameLine();
			SetCursorPosX(GetCursorPosX() + 5);

			std::string text = "Current: " + StateText;
			ImVec2 textsize = CalcTextSize(text.c_str());


			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
			PushStyleColor(ImGuiCol_FrameBg, styleColor(0, 0, 0, 0.f));
			BeginChildFrame(GetID((p->Name + "-weird-margins-frame-wtf-bruh").c_str()), ImVec2(textsize.x, 20));
			PopStyleColor();
			PopStyleVar();
			
			SetCursorPosY((20 - GetFontSize()) / 2);
			Text(text.c_str());

			EndChild();
			PopFont();
		}

		// cleanup, leave cursor on next line
		PopFont();
		PopStyleVar(1);
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
	}

	// this method is a fucking mess, written at 4 am but it works
	struct editGroupData
	{
		std::chrono::steady_clock::time_point TimeChanged;
		float LastValue;
		editGroupData(std::chrono::steady_clock::time_point TimeChanged, float LastValue)
		{
			this->TimeChanged = TimeChanged;
			this->LastValue = LastValue;
		}
	};
	std::map<ImGuiID, editGroupData> editGroupTimings;
	void DrawEditGroup(Config::Property* p)
	{
		Config::CEditGroup* eg = (Config::CEditGroup*)p->Value;

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		const ImGuiStyle& style = g.Style;

		// draw this property at the current height
		int LinePosY = GetCursorPosY();

		int AbsWidth = GetContentRegionMaxAbs().x - window->Pos.x;
		int SliderWidth = AbsWidth - 5;
		int SliderHeight = 20;
		int SliderContentWidth = SliderWidth - 4;
		int SliderContentHeight = SliderHeight - 4;
		ImVec2 SliderCursorPos(5, (20 - SliderHeight) / 2 + LinePosY);
		ImVec2 SliderPos = window->Pos - window->Scroll + SliderCursorPos;
		ImVec2 SliderSize(SliderWidth, SliderHeight);
		ImRect SliderBB(SliderPos, SliderPos + SliderSize);
		ImGuiID SliderID = GetID((p->Name + "-slider").c_str());

		// random calculations used later
		float HighlightWidth = (float)SliderContentWidth / (float)eg->Groups.size();
		int SliderContentMinX = SliderPos.x + 2;
		int SliderContentMaxX = SliderContentMinX + SliderContentWidth;

		// register this as a hoverable item in imgui
		ItemAdd(SliderBB, SliderID);
		bool hovered = ItemHoverable(SliderBB, SliderID);
		bool clicked = (hovered && g.IO.MouseClicked[0]);
		if (clicked)
		{
			SetActiveID(SliderID, window);
			SetFocusID(SliderID, window);
			FocusWindow(window);
		}
		GUI::IgnoreLButton |= hovered;

		// animation
		std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
		auto timingRecord = editGroupTimings.find(SliderID);
		if (timingRecord == editGroupTimings.end())
		{
			editGroupTimings.insert(std::make_pair(SliderID, editGroupData(now - std::chrono::seconds(10), 0.f)));
			timingRecord = editGroupTimings.find(SliderID);
		}
		double timeSinceChange = std::chrono::duration_cast<std::chrono::microseconds>(now - timingRecord->second.TimeChanged).count() / (double)1e6;
		double animFactor = easeInOutQuint(std::clamp(timeSinceChange / 0.1 /* animation time */, 0.0, 1.0));

		// more calculations
		float idealHighlightPosX = SliderContentMinX + HighlightWidth * eg->SelectedGroup;
		float lastHighlightPosX = timingRecord->second.LastValue;
		float actualHighlightPosX = lerp((double)lastHighlightPosX, (double)idealHighlightPosX, animFactor);
		ImVec2 HighlightPos(actualHighlightPosX, SliderPos.y + 2);

		// draw background
		window->DrawList->AddRectFilled(SliderBB.Min, SliderBB.Max, IM_COL32(20, 20, 20, 255), SliderHeight / 2 + 2);

		// draw selection background
		window->DrawList->AddRectFilled(HighlightPos, HighlightPos + ImVec2(HighlightWidth, 16), IM_COL32(60, 60, 60, 255), 10);

		// draw labels
		PushFont(Arial14);
		for (size_t i = 0; i < eg->Groups.size(); i++)
		{
			std::string VisibleName = eg->Groups.at(i)->VisibleName;
			ImVec2 TextSize = CalcTextSize(VisibleName.c_str());

			SetCursorPos(SliderCursorPos + ImVec2(2 + HighlightWidth * i + (HighlightWidth - TextSize.x) / 2, 2 + (16 - TextSize.y) / 2));
			Text(VisibleName.c_str());
		}
		PopFont();

		// slider behavior
		if (g.ActiveId == SliderID)
		{
			if (g.ActiveIdSource == ImGuiInputSource_Mouse && g.IO.MouseDown[0])
			{
				float MousePosX = g.IO.MousePos.x;
				float relX = MousePosX - (SliderPos.x + 2.f);
				relX = std::clamp(relX, 0.f, SliderPos.x + SliderSize.x - 2.f);
				size_t clickIndex = relX / HighlightWidth;
				if (clickIndex >= eg->Groups.size()) clickIndex = eg->Groups.size() - 1;

				if (eg->SelectedGroup != clickIndex)
				{
					eg->SelectedGroup = clickIndex;
					timingRecord->second.LastValue = actualHighlightPosX;
					timingRecord->second.TimeChanged = now;
				}
			}
			else
				ClearActiveID(); // only allow mouse1 activation
		}

		// cleanup for next property
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
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
		Config::SetBool("show-menu", true);
		ProcessingLoginMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_NONE)
	{
		Config::SetBool("show-menu", true);
		isEjecting = LoginMenu();
	}

	return isEjecting;
}

Config::Tab* CurrentTab = 0;
bool HackMenuPageHasScrollbar = false;
bool GUI::HackMenu()
{
	if (!Config::GetBool("show-menu"))
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
	style.WindowMinSize = ImVec2(420, 5 + 30 * (Config::Tabs.size() + 1) + TitleBarHeight);
	style.FrameBorderSize = 0.f;
	style.ChildRounding = 0.f;
	style.WindowBorderSize = 0.f;
	style.WindowRounding = 5;
	style.Alpha = Config::GetFloat("menu-opacity") / 100.f;
	style.Colors[ImGuiCol_Text] = Config::GetColor("menu-text-color");
	style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(50.f, 50.f, 50.f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_TitleBgActive] = Config::GetColor("menu-background-color2");
	style.Colors[ImGuiCol_WindowBg] = Config::GetColor("menu-background-color1");
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
	Color UnselectedColor = Config::GetColor("menu-background-color2");
	Color SelectedColor = Config::GetColor("menu-background-color3");
	ImU32 UnselectedTabHandleColor = IM_COL32(UnselectedColor.r(), UnselectedColor.g(), UnselectedColor.b(), style.Alpha * 255.f);
	ImU32 SelectedTabHandleColor = IM_COL32(SelectedColor.r(), SelectedColor.g(), SelectedColor.b(), style.Alpha * 255.f);
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
		Color col = Config::GetColor("menu-eject-color");
		DrawList->AddRectFilled(
			TabHandlePos,
			TabHandlePos + TabHandleSize,
			IM_COL32(col.r(), col.g(), col.b(), style.Alpha * 255.f),
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


	ImGui::PushStyleColor(ImGuiCol_FrameBg, Config::GetColor("menu-background-color3"));
	{
		ImVec4 _framebgcol = style.Colors[ImGuiCol_FrameBg];
		DrawList->AddRectFilled(ImGui::GetWindowPos() + ImGui::GetCursorPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), IM_COL32(_framebgcol.x, _framebgcol.y, _framebgcol.z, style.Alpha), style.FrameRounding, ImDrawCornerFlags_Bot);
	}
	ImGui::BeginChildFrame(ImGui::GetID(("##page-" + CurrentTab->Name).c_str()), ImVec2(0, 0), 0);
	ImGui::PopStyleColor();

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_Border, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_Separator, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, Config::GetColor("menu-background-color4"));
	
	for (size_t i = 0; i < CurrentTab->Widgets.size(); i++)
	{
		Config::Widget* Widget = CurrentTab->Widgets.at(i);
		ImGui::SetCursorPos(ImVec2(5, ImGui::GetCursorPosY() + 5));
		if (!ImGui::BeginChildFrame(
			ImGui::GetID(Widget->Name.c_str()),
			ImVec2(ImGui::GetWindowWidth() - (HackMenuPageHasScrollbar ? 24 : 10), Widget->Height + 6),
			ImGuiWindowFlags_NoScrollbar
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
			if (!Widget->IsVisible(Property))
				continue;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::SetCursorPosX(5 + Property->Indentation);

			switch (Property->Type)
			{
				case Config::PropertyType::BOOLEAN:
				{
					ImGui::DrawBooleanProperty(Property);
					break;
				}
				case Config::PropertyType::FLOAT:
				{
					ImGui::DrawFloatProperty(Property);
					break;
				}
				case Config::PropertyType::COLOR:
				{
					ImGui::DrawColorProperty(Property);
					break;
				}
				case Config::PropertyType::TEXT:
				{
					ImGui::Text(Property->VisibleName.c_str());
					break;
				}
				case Config::PropertyType::DROPDOWN:
				{
					ImGui::DrawDropdownProperty(Property);
					break;
				}
				case Config::PropertyType::INVERTER:
				{
					ImGui::DrawInverterProperty(Property);
					break;
				}
				case Config::PropertyType::EDITGROUP:
				{
					ImGui::DrawEditGroup(Property);
					break;
				}
				default:
				{
					ImGui::Text((Property->VisibleName + ": " + Property->Stringify()).c_str());
					break;
				}
			}

			// check separator
			if (Property->HasSeparatorAfter)
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Separator();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
			}
		}
		ImGui::PopFont();
		Widget->Height = ImGui::GetCurrentWindow()->ContentSize.y;
		ImGui::EndChild();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5); // add some padding to the bottom
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(5);
	HackMenuPageHasScrollbar = ImGui::GetContentRegionAvail().y < 0; // its gonna be a frame late, rip
	ImGui::EndChildFrame();

END:
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopFont();
	return eject;
}