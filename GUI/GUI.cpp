#include "../Include.hpp"
#include "HTTP.hpp"
// TODO: organize GUI namespace

// will be set to the screen center
ImVec2 LoginWindowPosition(100, 100);

std::mutex LoginMutex{};
uint32_t LoginAttemptIndex = 0;
bool IsAttemptingLogin = false;

struct LoginInformation
{
	uint32_t AttemptID;
	char Email[513];
	char Password[513];
};

DWORD WINAPI AttemptLogin(LPVOID pInfo)
{
	IsAttemptingLogin = true;

	// parse input
	LoginInformation* Info = (LoginInformation*)pInfo;
	std::string Email = std::string(Info->Email);
	std::string Password = std::string(Info->Password);
	std::replace(Email.begin(), Email.end(), '\n', ' ');
	std::replace(Password.begin(), Password.end(), '\n', ' ');

	// create outgoing JSON string
	JSONValue* EmailJSON = new JSONValue(std::wstring(Email.begin(), Email.end()));
	JSONValue* PasswordJSON = new JSONValue(std::wstring(Password.begin(), Password.end()));
	JSONObject InputRoot;
	InputRoot[L"email"] = EmailJSON;
	InputRoot[L"password"] = PasswordJSON;
	JSONValue* InputJSONV = new JSONValue(InputRoot);
	std::wstring InputW = InputJSONV->Stringify();
	std::string Input(InputW.begin(), InputW.end());
	delete InputJSONV;

	// send API request
	DWORD bytesRead = 0;
	HTTP::contentType = "application/json";
	byte* result = HTTP::Post("https://www.a4g4.com/API/dll/login.php", Input, &bytesRead);
	std::string output = std::string((char*)result, bytesRead);

	// parse output
	// example response: { "Valid": true, "UserId": 69, "SubscriptionRemaining": 69420, "AccountAge": 420, "SessionId": "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "Banned": false }
	JSONValue* value = JSON::Parse(std::wstring(output.begin(), output.end()).c_str());
	if (!value)
	{
		L::Log("Failed to parse JSON response");
		free(Info);
		IsAttemptingLogin = false;
		return 1;
	}
	JSONObject root;
	if (!value->IsObject())
	{
		L::Log("Failed to parse JSON response");
		free(Info);
		IsAttemptingLogin = false;
		delete value;
		return 1;
	}
	root = value->AsObject();

	// handle values

	LoginMutex.lock();
	// Valid
	{
		auto iterator = root.find(L"Valid");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsBool())
			goto JSONPARSE_FAILED;

		if (!value->AsBool())
		{
			free(Info);
			IsAttemptingLogin = false;
			Config::UserInfo::Clear();
			LoginMutex.unlock();
			delete value;
			return 1;
		}
	}

	// UserId
	{
		auto iterator = root.find(L"UserId");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsNumber())
			goto JSONPARSE_FAILED;

		Config::UserInfo::UserId = (int64_t)(value->AsNumber());
		if (Config::UserInfo::UserId <= 0)
			goto JSONPARSE_FAILED;
	}

	// SubscriptionRemaining
	{
		auto iterator = root.find(L"SubscriptionRemaining");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsNumber())
			goto JSONPARSE_FAILED;

		Config::UserInfo::TimeLeft = (int64_t)(value->AsNumber());
		Config::UserInfo::Premium = true;// Config::UserInfo::TimeLeft > 0;
	}

	// AccountAge
	{
		auto iterator = root.find(L"AccountAge");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsNumber())
			goto JSONPARSE_FAILED;

		Config::UserInfo::AccountAge = (int64_t)(value->AsNumber());
		if (Config::UserInfo::AccountAge <= 0)
			goto JSONPARSE_FAILED;
	}

	// SessionId
	{
		auto iterator = root.find(L"SessionId");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsString())
			goto JSONPARSE_FAILED;

		Config::UserInfo::SessionId = std::string(value->AsString().begin(), value->AsString().end());
		if (Config::UserInfo::SessionId.size() < 16)
			goto JSONPARSE_FAILED;
	}

	// Banned
	{
		auto iterator = root.find(L"Banned");
		if (iterator == root.end())
			goto JSONPARSE_FAILED;

		auto value = iterator->second;
		if (!value->IsBool())
			goto JSONPARSE_FAILED;

		Config::UserInfo::Banned = value->AsBool();
	}

	goto JSONPARSE_SUCCESS;
JSONPARSE_FAILED:
	L::Log("Failed to parse login reponse");
	free(Info);
	IsAttemptingLogin = false;
	Config::UserInfo::Clear();
	LoginMutex.unlock();
	delete value;
	return 1;

JSONPARSE_SUCCESS:
	Config::UserInfo::Email = Info->Email;
	Config::UserInfo::Premium = true;
	Config::UserInfo::Authenticated = true;
	IsAttemptingLogin = false;
	LoginMutex.unlock();
	delete value;
	return 0;
}

void GUI::ProcessingLoginMenu()
{
	ImGuiIO& io = ImGui::GetIO();
	static ImVec2 WindowSize(160, 75);
	ImGui::SetNextWindowSize(ImVec2(WindowSize));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x/2, io.DisplaySize.y/2), 0, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Processing", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);

	std::string txt = "Processing...";
	ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
	ImGui::SetCursorPosX(WindowSize.x / 2 - txtSize.x / 2);
	ImGui::SetCursorPosY(15);
	ImGui::Text("Processing...");

	// Cancel button
	ImGui::SetCursorPos(ImVec2(WindowSize.x / 2 - 70 / 2, WindowSize.y-35));
	if (ImGui::Button("Cancel", ImVec2(70, 20)))
	{
		LoginMutex.lock();
		LoginAttemptIndex++;
		Config::UserInfo::Clear();
		IsAttemptingLogin = false;
		LoginMutex.unlock();
	}

	LoginWindowPosition = ImGui::GetWindowPos();
	ImGui::End();
}

ImFont* FontDefault;
ImFont* Arial8;
ImFont* Arial12;
ImFont* Arial14;
ImFont* Arial16;
ImFont* Arial18;
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
	AYO_LOAD_FONT_BRUH(Arial8, "C:\\Windows\\Fonts\\arial.ttf", 8.f);
	AYO_LOAD_FONT_BRUH(Arial12, "C:\\Windows\\Fonts\\arial.ttf", 12.f);
	AYO_LOAD_FONT_BRUH(Arial14, "C:\\Windows\\Fonts\\arial.ttf", 14.f);
	AYO_LOAD_FONT_BRUH(Arial16, "C:\\Windows\\Fonts\\arial.ttf", 16.f);
	AYO_LOAD_FONT_BRUH(Arial18, "C:\\Windows\\Fonts\\arial.ttf", 18.f);
	AYO_LOAD_FONT_BRUH(Arial12Italics, "C:\\Windows\\Fonts\\ariali.ttf", 12.f);
	AYO_LOAD_FONT_BRUH(Arial14Italics, "C:\\Windows\\Fonts\\ariali.ttf", 14.f);
	AYO_LOAD_FONT_BRUH(Arial16Italics, "C:\\Windows\\Fonts\\ariali.ttf", 16.f);
	AYO_LOAD_FONT_BRUH(Arial18Italics, "C:\\Windows\\Fonts\\ariali.ttf", 18.f);
	AYO_LOAD_FONT_BRUH(Arial14BoldItalics, "C:\\Windows\\Fonts\\arialbi.ttf", 14.f);
	AYO_LOAD_FONT_BRUH(Arial18BoldItalics, "C:\\Windows\\Fonts\\arialbi.ttf", 18.f);
	AYO_LOAD_FONT_BRUH(Arial18Bold, "C:\\Windows\\Fonts\\arialbd.ttf", 18.f);

	return;
problemo:
	MessageBox(NULL, "a4g4.com requires that you have the \"Arial\" font (and it's italics version) installed. It comes installed by default with Windows OS in C:/Windows/Fonts. Please download Airal to that location (as arial.ttf and ariali.ttf) then try injecting again.", "Missing Fonts", 0);
	// segfault lol
	int x = *(int*)0;
	// prevent dumbass compiler from ignoring our segfault >:(
	L::Log(std::to_string(x).c_str());
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

	void ColorPicker(Color* oColor, std::string id, std::string name, ImVec2 PreviewSize)
	{
		float colArray[] = { oColor->color[0] / 255.f , oColor->color[1] / 255.f , oColor->color[2] / 255.f };
		ImVec4 imColor = ImVec4(colArray[0], colArray[1], colArray[2], 1.f);

		// button
		if (ImGui::ColorButton(("###" + id).c_str(), imColor, 0, PreviewSize))
			ImGui::OpenPopup(("###" + id + "-picker").c_str());
		GUI::IgnoreLButton |= ImGui::IsItemHovered();

		// picker
		SetNextWindowSize(ImVec2(202, 176 + 2 + 18 + 24));
		if (ImGui::BeginPopup(("###" + id + "-picker").c_str()))
		{
			PushFont(Arial14);
			SetCursorPos(ImVec2(4, 2));
			Text(name.c_str());
			PopFont();
			SetCursorPos(ImVec2(1, 1 + 18));

			SetNextItemWidth(200);
			ColorPicker3(("##" + id + "-picker-editor").c_str(), colArray, ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayHex);

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
		ImVec4 accentColor = Config::GetColor("menu-option-color1");
		ImVec4 baseColor = Config::GetColor("menu-option-color2");
		accentColor.w = baseColor.w = style.Alpha;
		ImVec4 backgroundColor = lerp(accentColor, baseColor, enabledFactor);
		//ImVec4 backgroundBorderColor = backgroundColor; backgroundBorderColor.w = 0.2f;
		ImVec4 grabColor = lerp(baseColor, accentColor, enabledFactor);
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
			SetCursorPosY(LinePosY);

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
			PopStyleVar();  SameLine();


			// padding, clear button
			SetCursorPosX(GetCursorPosX() + 5);

			// go back to top of line
			SetCursorPosY(LinePosY);

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

		SetCursorPosX(GetCursorPosX() + 2); // move over to line up with binded properties
		ColorPicker(c, p->Name, p->VisibleName, ImVec2(30, 20));

		PopStyleVar(3);
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
		ImVec4 Color1 = Config::GetColor("menu-option-color1");
		ImVec4 Color2 = Config::GetColor("menu-option-color2");
		Color1.w = Color2.w = style.Alpha;
		ImU32 AccentColor = vec4toU32(Color2);
		ImU32 BaseColor = vec4toU32(Color1);

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
		Config::CDropdown* dd = (Config::CDropdown*)p->Value;

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

		// change button
		ImVec4 Color1 = Config::GetColor("menu-option-color1");
		ImVec4 Color2 = Config::GetColor("menu-option-color2");

		PushStyleColor(ImGuiCol_Button, Color2);
		PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color2, Color1, 0.25f));
		PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color2, Color1, 0.5f));
		PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
		PushFont(Arial14);
		if (Button(("Change##" + p->Name + "-button").c_str(), ImVec2(55, 20)))
		{
			OpenPopup(("##" + p->Name + "-editor").c_str());
		}
		GUI::IgnoreLButton |= IsItemHovered();
		SameLine();
		PopStyleColor(3);
		PopStyleVar(1);

		// picker
		PushFont(Arial14);
		PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
		size_t nItems = dd->Options.size();
		SetNextWindowSize(ImVec2(150, nItems * 16 + 6));
		if (BeginPopup(("##" + p->Name + "-editor").c_str()))
		{
			GUI::IgnoreLButton = true;
			ImGuiWindow* popup = GetCurrentWindow();
			ImVec2 base = popup->DC.CursorPos;

			for (size_t i = 0; i < nItems; i++)
			{

				// register this as a hoverable item in imgui
				ImVec2 labelSize = CalcTextSize(dd->Stringify(i).c_str());
				ImRect BB(base + ImVec2(5, 5 + i * 16), base + ImVec2(5, 5 + i * 16) + ImVec2(12 + 5 + labelSize.x, 12));
				ImGuiID ID = GetID((p->Name + "-btn-" + std::to_string(i)).c_str());
				ItemAdd(BB, ID);
				bool hovered = false, active = false;
				bool pressed = ButtonBehavior(BB, ID, &hovered, &active);
				if (active)
				{
					SetActiveID(ID, popup);
					SetFocusID(ID, popup);
					FocusWindow(popup);
				}
				if (pressed)
				{
					dd->Select(i);
				}

				ImVec4 c = Color1;
				if (i != dd->GetSelection())
				{
					if (active)
						c = ImLerp(Color1, Color2, 0.4f);
					else if (hovered)
						c = ImLerp(Color1, Color2, 0.2f);
				}

				ImVec2 circlePos = base + ImVec2(5 + 6, 5 + 6 + i * 16);
				popup->DrawList->AddCircleFilled(circlePos, 6.f, vec4toU32(c));

				if (i == dd->GetSelection())
				{
					popup->DrawList->AddCircleFilled(circlePos, 4.f, vec4toU32(Color2));
				}

				ImVec2 textPos = ImVec2(5 + 12 + 5, 3 + i * 16);
				SetCursorPos(textPos);
				Text(dd->Stringify(i).c_str());

			}

			EndPopup();
		}
		PopStyleVar(1);
		PopFont();


		int pixelsForText = GetContentRegionMaxAbs().x - window->Pos.x - GUI::PropertyLabelsWidth - 55 - 5;
		SetCursorPosY(LinePosY + (20 - GetFontSize()) / 2);
		SetCursorPosX(GetCursorPosX() + 5);
		Text(dd->Stringify(dd->GetSelection()).c_str());

		PopFont();
		// cleanup for next property
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
			ImVec4 Color1 = Config::GetColor("menu-option-color1");
			ImVec4 Color2 = Config::GetColor("menu-option-color2");
			
			PushStyleColor(ImGuiCol_Button, Color2);
			PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color2, Color1, 0.25f));
			PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color2, Color1, 0.5f));
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

		std::string StateText = p->Stringify();
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
		int SliderWidth = AbsWidth - 7;
		int SliderHeight = 20;
		int SliderContentWidth = SliderWidth - 4;
		int SliderContentHeight = SliderHeight - 4;
		ImVec2 SliderCursorPos(6, (20 - SliderHeight) / 2 + LinePosY);
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
		window->DrawList->AddRectFilled(SliderBB.Min, SliderBB.Max, vec4toU32(Config::GetColor("menu-option-selector-background-color")), SliderHeight / 2 + 2);
		
		// draw selection background
		window->DrawList->AddRectFilled(HighlightPos, HighlightPos + ImVec2(HighlightWidth, 16), vec4toU32(Config::GetColor("menu-option-selector-active-color")), 10);

		// draw labels
		PushFont(Arial14);
		for (size_t i = 0; i < eg->Groups.size(); i++)
		{
			std::string VisibleName = eg->Groups.at(i)->VisibleName;
			ImVec2 TextSize = CalcTextSize(VisibleName.c_str());

			SetCursorPos(SliderCursorPos + ImVec2(2 + HighlightWidth * i + (HighlightWidth - TextSize.x) / 2, 2 + (16 - TextSize.y) / 2));
			PushStyleColor(ImGuiCol_Text, Config::GetColor("menu-option-selector-text-color"));
			Text(VisibleName.c_str());
			PopStyleColor();
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

	void DrawMultiSelector(Config::Property* p)
	{
		Config::CMultiSelector* ms = (Config::CMultiSelector*)p->Value;

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

		// change button
		ImVec4 Color1 = Config::GetColor("menu-option-color1");
		ImVec4 Color2 = Config::GetColor("menu-option-color2");

		PushStyleColor(ImGuiCol_Button, Color2);
		PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color2, Color1, 0.25f));
		PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color2, Color1, 0.5f));
		PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
		PushFont(Arial14);
		if (Button(("Change##" + p->Name + "-button").c_str(), ImVec2(55, 20)))
		{
			OpenPopup(("##" + p->Name + "-editor").c_str());
		}
		GUI::IgnoreLButton |= IsItemHovered();
		SameLine();
		PopStyleColor(3);
		PopStyleVar(1);

		// picker
		PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
		//SetNextItemWidth(1);
		size_t nItems = ms->Count();
		SetNextWindowSize(ImVec2(150, nItems * 16 + 6));
		if (BeginPopup(("##" + p->Name + "-editor").c_str()))
		{
			GUI::IgnoreLButton = true;

			int y = 3;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(-1.f, -1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
			for (size_t i = 0; i < nItems; i++)
			{
				bool x = ms->Get(i);

				if (x) // checked
				{
					ImGui::PushStyleColor(ImGuiCol_FrameBg, Color2);
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImLerp(Color2, Color1, 0.2f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImLerp(Color2, Color1, 0.4f));
				}
				else // unchecked
				{
					ImGui::PushStyleColor(ImGuiCol_FrameBg, Color1);
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImLerp(Color1, Color2, 0.2f));
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImLerp(Color1, Color2, 0.4f));
				}

				SetCursorPos(ImVec2(5, y + 2));
				if (ImGui::Checkbox(("###check-" + p->Name + "-" + ms->Stringify(i)).c_str(), &x))
					ms->Set(i, x);

				SetCursorPos(ImVec2(5 + 12 + 5, y + 1));
				Text(ms->Stringify(i).c_str());

				ImGui::PopStyleColor(3);

				y += 16;
			}
			ImGui::PopStyleVar(3);

			EndPopup();
		}
		PopStyleVar(1);

		
		int pixelsForText = GetContentRegionMaxAbs().x - window->Pos.x - GUI::PropertyLabelsWidth - 55 - 5;
		SetCursorPosY(LinePosY + (20 - GetFontSize()) / 2);
		SetCursorPosX(GetCursorPosX() + 5);
		Text(ms->Stringify().c_str());

		PopFont();
		// cleanup for next property
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
	}

	void DrawTextInput(Config::Property* p)
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

		// draw the text input
		PushFont(Arial14);
		SetNextItemWidth(GetContentRegionMaxAbs().x - window->Pos.x - 1 - GUI::PropertyLabelsWidth);
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20 - GetFontSize()) / 2, (20 - GetFontSize()) / 2));
		PushStyleColor(ImGuiCol_Border, styleColor(255, 255, 255, 0.5f));
		PushStyleColor(ImGuiCol_FrameBg, styleColor(120, 120, 120, 1.f));
		PushStyleColor(ImGuiCol_FrameBgHovered, styleColor(100, 100, 100, 1.f));
		PushStyleColor(ImGuiCol_FrameBgActive, styleColor(120, 120, 120, 1.f));

		auto t = (Config::CTextInput*)p->Value;
		bool changed = InputText(("##" + p->Name).c_str(), t->text, t->size);

		PopStyleColor(4);
		PopStyleVar(1);
		PopFont();

		// cleanup for next property
		SetCursorPosY(LinePosY + 20);
		SetCursorPosX(0);
	}

	void DrawConfigPage()
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		const ImGuiStyle& style = g.Style;

		int FloatRight = GetContentRegionMaxAbs().x - window->Pos.x - 1;

		ImVec4 Color1 = Config::GetColor("menu-option-color1");
		ImVec4 Color2 = Config::GetColor("menu-option-color2");
		ImVec4 Color3 = Config::GetColor("menu-eject-color");

		int y = 6;
		for (size_t i = 0; i < ConfigManager::Configs.size(); i++)
		{
			ConfigManager::ConfigData& cdata = ConfigManager::Configs.at(i);
			SetCursorPos(ImVec2(5, y + (20 - ImGui::GetFontSize()) / 2));
			Text(cdata.Name.c_str());


			PushStyleColor(ImGuiCol_Button, Color2);
			PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color2, Color1, 0.2f));
			PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color2, Color1, 0.4f));

			SetCursorPos(ImVec2(FloatRight - 55 - (cdata.Removable ? 25 : 0), y));
			if (Button(("Load##config-" + std::to_string(i)).c_str(), ImVec2(55, 20)))
			{
				ConfigManager::Load(cdata);
			}

			if (cdata.Removable)
			{
				PushStyleColor(ImGuiCol_Button, Color3);
				PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color3, Color1, 0.2f));
				PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color3, Color1, 0.4f));
				SetCursorPos(ImVec2(FloatRight - 20, y));
				if (Button(("X##config-" + std::to_string(i)).c_str(), ImVec2(20, 20)))
				{
					if (ConfigManager::Remove(i))
						i--;
				}
				PopStyleColor(3);
			}
			PopStyleColor(3);
			y += 25;
		}
		PushStyleColor(ImGuiCol_Button, Color2);
		PushStyleColor(ImGuiCol_ButtonHovered, ImLerp(Color2, Color1, 0.2f));
		PushStyleColor(ImGuiCol_ButtonActive, ImLerp(Color2, Color1, 0.4f));

		SetCursorPos(ImVec2(6, y));
		if (Button("Export Config##save-config", ImVec2(100, 20)))
		{
			ConfigManager::Export();
		}

		SetCursorPos(ImVec2(6 + 100 + 5, y));
		if (Button("Import Config##load-config", ImVec2(100, 20)))
		{
			ConfigManager::Import();
		}
		PopStyleColor(3);
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

	ImGui::SetNextWindowSize(ImVec2(340, 110 + 45 + 45 + 45 + 10));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), 0, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Login", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

	/*
		LOGIN FORM
	*/
	ImGui::PushFont(Arial16);
	ImGui::GetStyle().FrameBorderSize = 0.f;

	// Email
	static char Email[513] = "";
	ImGui::SetCursorPos(ImVec2(20, 32));
	ImGui::TextInputWithPlaceholder("Email Address", 300, "###Email", Email, 513);

	// Password
	static char Password[513] = "";
	ImGui::SetCursorPos(ImVec2(20, 70));
	ImGui::TextInputWithPlaceholder("Password", 300, "###Password", Password, 513);

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
		LoginMutex.lock();

		// reset old data
		LoginAttemptIndex++;
		Config::UserInfo::Clear();

		LoginMutex.unlock();

		// fill LoginInformation with recent data (malloc thread will still have access once scope deconstructs)
		// p.s. thread has to free this malloc, to prevent a *tiny*, but still existent, memory leak
		LoginInformation* Info = (LoginInformation*)malloc(sizeof(LoginInformation));
		if (Info)
		{
			Info->AttemptID = LoginAttemptIndex;
			strcpy(Info->Email, Email);
			strcpy(Info->Password, Password);

			// start login thread
			IsAttemptingLogin = true;
			CreateThread(NULL, 0, AttemptLogin, Info, 0, 0);
			Sleep(0); // let it start
		}
	}

	// Play Free
	ImGui::SetCursorPos(ImVec2(20, 110 + 45));
	if (ImGui::Button("Play Free", ImVec2(300, 35)))
	{
		Config::UserInfo::Clear();
		Config::UserInfo::Authenticated = true;
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

	if (IsAttemptingLogin)
	{
		Config::SetBool("show-menu", true);
		ImGui::PushFont(Arial14);
		ProcessingLoginMenu();
		ImGui::PopFont();
	}
	else if (!Config::UserInfo::Authenticated)
	{
		Config::SetBool("show-menu", true);
		isEjecting = LoginMenu();
	}
	else
	{
		isEjecting = HackMenu();
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
	ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiCond_Once);

	// Styles
	int TitleBarHeight = 16;
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(0, 0);
	style.WindowMinSize = ImVec2(521 /* don't ask */, 5 + 30 * (Config::Tabs.size() + 1) + TitleBarHeight);
	style.FrameBorderSize = 0.f;
	style.ChildRounding = 0.f;
	style.WindowBorderSize = 0.f;
	style.WindowRounding = 5;
	style.Alpha = 1.f;
	style.Colors[ImGuiCol_Text] = Config::GetColor("menu-text-color");
	style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(50.f, 50.f, 50.f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_TitleBgActive] = Config::GetColor("menu-background-color2");
	style.Colors[ImGuiCol_WindowBg] = Config::GetColor("menu-background-color1");
	style.Colors[ImGuiCol_CheckMark] = Config::GetColor("menu-option-color1");
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	//io.ConfigWindowsMoveFromTitleBarOnly = true;
	
	// set title bar size to 16px with font = Arial16
	ImFont* font_before = ImGui::GetFont();
	ImGui::PushFont(Arial14Italics);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, (TitleBarHeight - ImGui::GetFontSize()) / 2.f));
	ImGui::Begin("a4g4.com - PRIVATE BETA v1.0.3", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
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
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_Border, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_Separator, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, Config::GetColor("menu-text-color"));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, Config::GetColor("menu-background-color4"));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Config::GetColor("menu-background-color4"));
	
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

		if (Widget->Name == "Config" && CurrentTab->Name == "Config")
		{
			ImGui::PushFont(Arial16);
			ImGui::DrawConfigPage();
			Widget->Height = ImGui::GetCursorPosY();
			ImGui::PopFont();
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
				case Config::PropertyType::MULTISELECT:
				{
					ImGui::DrawMultiSelector(Property);
					break;
				}
				case Config::PropertyType::TEXTINPUT:
				{
					ImGui::DrawTextInput(Property);
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
		Widget->Height = ImGui::GetCursorPosY();
		ImGui::EndChild();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5); // add some padding to the bottom
	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(6);
	HackMenuPageHasScrollbar = ImGui::GetContentRegionAvail().y < 0; // its gonna be a frame late, rip
	ImGui::EndChildFrame();

END:
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopFont();
	return eject;
}


/*
	=========== GUI VERSION 2.0 ===========
*/
namespace GUI2
{
	bool Ejected = false;
	bool WantMouse = false;
	float LoadProgress = 0.f;
	float VisibleLoadProgress = 0.f;
	Animation::Anim* IntroAnimation = nullptr;
	Animation::Anim* IntroAnimation2 = nullptr;
	Animation::Anim* SearchAnimation = nullptr;

	ImVec2 DefaultMenuSize = ImVec2(650, 330);
	ImVec2 MinMenuSize = ImVec2(575, 242);
	int PropertyColumnPosition = 200;

	bool IsSearching = false;
	char* SearchQuery = nullptr;
	Config2::Tab* ActiveTab = nullptr;
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
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2, Size.y - Size.x / 2), Size.x / 2, -IM_PI/6.f, IM_PI*7.f/6.f, Segments);
		DrawList->PathArcTo(Position + ImVec2(Size.x / 2, Size.y / 12.f), Size.y / 12.f, IM_PI*7.f/6.f, IM_PI*11.f/6.f, Segments / 3);
		
		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(0,0,0, Opacity), true, StrokeSize);
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

		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255,255,255, Opacity), false, StrokeSize);
		DrawList->PathClear();
	}

	void DrawSearchIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
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

		DrawList->PathArcTo(Position + ImVec2(MainRadius, MainRadius), MainRadius, IM_PI / 4.f, 9.f*IM_PI/4.f, Segments);
		DrawList->PathLineTo(Position + ImVec2(Size.x, Size.y));

		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity), false, StrokeSize);
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

		DrawList->PathArcTo(Position + Size / 2.f, Size.x/2.f, 0, 2.f * IM_PI, Size.x);
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
		static Config2::CColor* ColorBase = Config2::GetColor("theme-info-icon");
		Config2::CColor Color = ColorBase->ModulateAlpha(Opacity);
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
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, Color32, false, StrokeSize*2.f);
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

	void DrawXIcon(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
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

		DrawList->AddLine(Position, Position + Size, IM_COL32(255, 255, 255, Opacity), StrokeSize);
		DrawList->AddLine(ImVec2(Position.x, Position.y + Size.y), ImVec2(Position.x + Size.x, Position.y), IM_COL32(255, 255, 255, Opacity), StrokeSize);
	}

	void DrawCheckmark(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
	{
		constexpr float AspectRatio = 4.f/3.f; // X / Y
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

		DrawList->AddLine(Position + ImVec2(Size.x, 0), Position + ImVec2(Size.x / 3.f, Size.y), IM_COL32(255, 255, 255, Opacity), StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 3.f, Size.y), Position + ImVec2(0, Size.y / 2.f), IM_COL32(255, 255, 255, Opacity), StrokeSize);
	}

	void DrawUpArrow(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
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

		DrawList->AddLine(Position + ImVec2(0, Size.y), Position + ImVec2(Size.x/2.f, 0.f), IM_COL32(255, 255, 255, Opacity), StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 2.f, 0.f), Position + Size, IM_COL32(255, 255, 255, Opacity), StrokeSize);
	}

	void DrawDownArrow(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(24.f, 24.f))
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

		DrawList->AddLine(Position, Position + ImVec2(Size.x / 2.f, Size.y), IM_COL32(255, 255, 255, Opacity), StrokeSize);
		DrawList->AddLine(Position + ImVec2(Size.x / 2.f, Size.y), Position + ImVec2(Size.x, 0.f), IM_COL32(255, 255, 255, Opacity), StrokeSize);
	}

	void DrawSelectionCursor(unsigned char Opacity = 255, ImVec2 Dimensions = ImVec2(10.f, 10.f))
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
		DrawList->PathLineTo(Position + ImVec2(0, Size.y));
		DrawList->PathLineTo(Position + ImVec2(Size.x, Size.y / 2));
		//DrawList->PathLineTo(Position);

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(255, 255, 255, Opacity));
		DrawList->PathClear();
	}

	void InputTextWithPlaceholder(std::string Identifier, std::string Placeholder, char* Buffer, size_t BufferLength)
	{
		bool Active = GetID(("##" + Identifier).c_str()) == GetActiveID();
		bool HasContent = BufferLength && Buffer[0];
		if (!HasContent && !Active)
		{
			// draw placeholder
			auto CursorBefore = ImGui::GetCursorPos();
			ImVec4 TextColorBefore = GImGui->Style.Colors[ImGuiCol_Text];
			bool NextItemHasWidth = GImGui->NextItemData.Flags & ImGuiNextItemDataFlags_HasWidth;
			float NextItemWidth = GImGui->NextItemData.Width;

			PushStyleColor(ImGuiCol_Text, lerp(TextColorBefore, ImVec4(0,0,0, TextColorBefore.w), 0.5f));

			TextEx(Placeholder.c_str());

			PopStyleColor();
			ImGui::SetCursorPos(CursorBefore);
			if (NextItemHasWidth)
			{
				GImGui->NextItemData.Flags |= ImGuiNextItemDataFlags_HasWidth;
				GImGui->NextItemData.Width = NextItemWidth;
			}
		}
		InputText(("##" + Identifier).c_str(), Buffer, BufferLength);
	}

	std::string TruncateToEllipsis(std::string Input, float MaxWidth)
	{
		float CurrentWidth = CalcTextSize(Input.c_str()).x;
		if (CurrentWidth <= MaxWidth) return Input;

		int PredictedCharCount = Input.size() * MaxWidth / CurrentWidth;
		std::string Output = Input.substr(0, PredictedCharCount);
		CurrentWidth = CalcTextSize((Output + "...").c_str()).x;

		if (CurrentWidth <= MaxWidth) // prediction was too short
		{
			int AddChars = 0;
			while (CurrentWidth <= MaxWidth)
			{
				AddChars++;
				Output = Input.substr(0, PredictedCharCount + AddChars);
				CurrentWidth = CalcTextSize((Output + "...").c_str()).x;
			}
			return Input.substr(0, PredictedCharCount + AddChars - 1) + "...";
		}
		else // prediction was too long
		{
			int RemoveChars = 0;
			while (CurrentWidth > MaxWidth && (RemoveChars < PredictedCharCount))
			{
				RemoveChars++;
				Output = Input.substr(0, PredictedCharCount - RemoveChars);
				CurrentWidth = CalcTextSize((Output + "...").c_str()).x;
			}
			return Input.substr(0, PredictedCharCount - RemoveChars) + "...";
		}
	}

	void ToolTip(std::string str, int ItemHeightForVerticalFlip = 20)
	{
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

		ImVec2 WindowPos = PointerPos - ImVec2(WindowSize.x/2, VerticalInvert ? (-PointerSize.y) : (WindowSize.y + PointerSize.y));

		// move horizontally
		if (WindowPos.x + WindowSize.x > io.DisplaySize.x)
		{
			WindowPos += ImVec2(io.DisplaySize.x - (WindowPos.x + WindowSize.x), 0);
			if (WindowPos.x < 0)
			{
				WindowPos -= ImVec2(WindowPos.x/2, 0);
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

		DrawList->AddConvexPolyFilled(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(85, 90, 95, 255));
		DrawList->AddPolyline(DrawList->_Path.Data, DrawList->_Path.Size, IM_COL32(42, 45, 47, 255), true, 2.f);
		DrawList->PathClear();

		// Draw Text
		DrawList->AddText(Arial16, 16, WindowPos + ImVec2(5 + TextOffset, 5), IM_COL32(255, 255, 255, 255), str.c_str());
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

	float DrawInputFloat(std::string Identifier, ImVec4 ColorA, ImVec4 ColorB, float Factor = 0.5f, ImVec2 Size = ImVec2(100, 16))
	{
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;

		ImRect BB(Window->DC.CursorPos, Window->DC.CursorPos + Size);
		ImGuiID ID = Window->GetID(Identifier.c_str());
		ImVec2 GrabCenter(Window->DC.CursorPos.x + Size.y / 2.f + Factor * (Size.x - Size.y), Window->DC.CursorPos.y + Size.y / 2.f);

		// draw bar
		{
			DrawList->AddRectFilled(Window->DC.CursorPos, Window->DC.CursorPos + Size, ColorConvertFloat4ToU32(ColorB), Size.y);
			DrawList->AddRectFilled(Window->DC.CursorPos + ImVec2(2, 2), Window->DC.CursorPos + Size - ImVec2(2, 2), ColorConvertFloat4ToU32(ColorA), Size.y);
			DrawList->AddRectFilled(Window->DC.CursorPos, ImVec2(GrabCenter.x + Size.y / 2.f, Window->DC.CursorPos.y + Size.y), ColorConvertFloat4ToU32(ColorB), Size.y);
		}

		// draw grab
		{
			DrawList->AddCircleFilled(GrabCenter, Size.y / 2.f - 2.f, ColorConvertFloat4ToU32(ColorA));
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
				Factor = (GImGui->IO.MousePos.x - (Window->DC.CursorPos.x + Size.y / 2.f)) / (Size.x - Size.y);
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
	int DrawTextProperty(Config2::Property* p)
	{
		auto Window = ImGui::GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(Pos + ImVec2(6, (20 - ImGui::GetFontSize()) / 2));
		ImGui::Text(p->VisibleName.c_str());

		return 20;
	}

	int DrawBooleanProperty(Config2::Property* p)
	{
		static Config2::CColor* PropertyBase = Config2::GetColor("theme-property-base");
		static Config2::CColor* PropertyAccent = Config2::GetColor("theme-property-accent");
		static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
		static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
		static Config2::CColor* ButtonBorder = Config2::GetColor("theme-button-border");
		static Config2::CColor* ButtonText = Config2::GetColor("theme-button-text");
		static Config2::CFloat* ButtonBorderSize = Config2::GetFloat("theme-button-border-size");
		Config2::CBoolean* Value = (Config2::CBoolean*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = GetCursorPos();


		bool MasterLocked = false;
		bool PremiumLocked = p->IsPremium && !UserData::Premium;

		// draw keybind shit
		if (Value->Bindable)
		{
			PushFont(Arial14);

			if (p == Config2::SettingKeybindFor) // this is being set right now
			{

				PushFont(Arial12);
				const char* Prefix = "MODE:";
				ImVec2 PrefixSize = CalcTextSize(Prefix);
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 39, (20 - PrefixSize.y) / 2));
				Text(Prefix);
				PopFont();

				PushStyleColor(ImGuiCol_Button, 0);
				PushStyleColor(ImGuiCol_ButtonHovered, 0);
				PushStyleColor(ImGuiCol_ButtonActive, 0);
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*ButtonBase);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);

				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 39 + PrefixSize.x + 5, 0));
				int ItemWidth = 60;
				BeginChild(("##bindmode-" + p->Name).c_str(), ImVec2(ItemWidth * 3, 20), true);
				auto bruh_window = GetCurrentWindow();
				auto bruh_dl = bruh_window->DrawList;
				SetCursorPos(ImVec2(ItemWidth * (int)Value->BindMode, 0));
				bruh_dl->AddRectFilled(bruh_window->DC.CursorPos, bruh_window->DC.CursorPos + ImVec2(ItemWidth, 20), *ButtonActive, 0, 0);
				for (int i = 0; i < 3; i++)
				{
					SetCursorPos(ImVec2(ItemWidth * i, 0));
					if (Button((Config2::KeybindTypeNames[i] + "##" + p->Name).c_str(), ImVec2(ItemWidth, 20)))
					{
						Value->BindMode = (Config2::KeybindMode)i;
					}
					GUI2::WantMouse |= IsItemHovered();
				}
				EndChild();
				PopStyleColor(5);
				PopStyleVar(2);

				PushFont(Arial12);
				const char* Suffix = "[PRESS A KEY]";
				ImVec2 SuffixSize = CalcTextSize(Suffix);
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 39 + PrefixSize.x + 5 + ItemWidth * 3 + 5, (20 - SuffixSize.y) / 2));
				Text(Suffix);
				PopFont();

			}
			else if (Value->BoundToKey >= 0) // this key is already bound
			{
				const char* Prefix = Value->BindMode == Config2::KeybindMode::TOGGLE ? "PRESS" : "HOLD";
				std::string KeyName = Keybind::KeyNames[Value->BoundToKey];
				const char* Suffix =
					Value->BindMode == Config2::KeybindMode::TOGGLE ? "TO TOGGLE" :
					Value->BindMode == Config2::KeybindMode::HOLDTOENABLE ? "TO ENABLE" : "TO DISABLE";

				PushFont(Arial12);
				ImVec2 PrefixSize = CalcTextSize(Prefix);
				ImVec2 SuffixSize = CalcTextSize(Suffix);
				PopFont();
				ImVec2 KeyNameSize = CalcTextSize(KeyName.c_str());

				int x = GUI2::PropertyColumnPosition + 39;
				SetCursorPos(Pos + ImVec2(x, (20 - PrefixSize.y) / 2));
				PushFont(Arial12);
				Text(Prefix);
				PopFont();
				x += 5 + PrefixSize.x;

				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

				SetCursorPos(Pos + ImVec2(x, 0));
				if (Button((KeyName + "##" + p->Name).c_str(), ImVec2(KeyNameSize.x + 10, 20)))
				{
					std::vector<void*>& vec = Keybind::Binds[Value->BoundToKey];
					for (size_t i = 0; i < vec.size(); i++)
						if (vec.at(i) == (void*)p)
							vec.erase(vec.begin() + i--);

					Value->BoundToKey = -1;
					Config2::SettingKeybindFor = nullptr;
					GUI2::WantMouse = true;
				}
				else if (IsItemHovered())
				{
					SetCursorPos(Pos + ImVec2(x + KeyNameSize.x / 2 + 5, 0));
					ToolTip("Click To Clear", 20);
					GUI2::WantMouse = true;
				}

				PopStyleColor(2);
				PopStyleVar(2);
				x += KeyNameSize.x + 10 + 5;


				SetCursorPos(Pos + ImVec2(x, (20 - SuffixSize.y) / 2));
				PushFont(Arial12);
				Text(Suffix);
				PopFont();

			}
			else if (!PremiumLocked) // the key is not bound & we are able to bind it
			{

				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 35, 0));

				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

				if (Button(("Bind##" + p->Name).c_str(), ImVec2(40, 20)))
				{
					Config2::SettingKeybindFor = p;
					GUI2::WantMouse = true;
				}
				else if (IsItemHovered())
				{
					GUI2::WantMouse = true;
				}

				PopStyleColor(2);
				PopStyleVar(2);
			}

			PopFont();
		}

		// draw switch
		if (true) // here only because vs won't let me collapse it otherwise for some reason
		{
			if (p->Master && p->Master->Type == Config2::PropertyType::BOOLEAN)
			{
				bool MasterAllows = ((Config2::CBoolean*)p->Master->Value)->Value.Get() != 0;
				bool ImEnabled = Value->Value.Get() != 0;

				MasterLocked = ImEnabled && !MasterAllows;
			}

			SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, (20 - 16) / 2));

			double TimePassed = Value->Value.GetTimeSinceChange();
			double AnimFactor = Animation::animate(TimePassed, 0.15f);
			if (Value->Value.Get() == 0)
				AnimFactor = 1.f - AnimFactor;

			bool Flipped = DrawBooleanSwitch("##" + p->Name, *PropertyBase, *PropertyAccent, AnimFactor);
			if (Flipped)
			{
				GUI2::WantMouse = true;
				Config2::SettingKeybindFor = nullptr;
				if (!PremiumLocked)
				{
					Value->Value.Invert();
					if (Value->BoundToKey >= 0)
					{
						if (Value->BindMode == Config2::KeybindMode::HOLDTODISABLE)
							Value->BindMode = Config2::KeybindMode::HOLDTOENABLE;
						else if (Value->BindMode == Config2::KeybindMode::HOLDTOENABLE)
							Value->BindMode = Config2::KeybindMode::HOLDTODISABLE;
					}
				}
			}

			if (IsItemHovered())
			{
				GUI2::WantMouse = true;
				if (PremiumLocked)
				{
					SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 30 / 2, 0));
					ToolTip("Premium users only", 20);
				}
			}
		}

		// draw label 
		{
			ImVec2 IconSize(14, 14);
			std::string ToolTipString = "Click for more info";
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			if (PremiumLocked)
			{
				DrawErrorIcon(255, IconSize);
				ToolTipString = "Premium users only";
			}
			else if (MasterLocked)
			{
				DrawWarningIcon(255, IconSize);
				ToolTipString = "This property has no effect because it is controlled by \"" + p->Master->VisibleName + "\"";
			}
			else
			{
				DrawInfoIcon(255, IconSize);
			}

			auto ID = GetID((p->Name + "-status-icon-hoverable").c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(ToolTipString, IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, ("http://a4g4.com/help/index.php#" + p->Name).c_str(), 0, 0, SW_SHOW);
				}
			}

			SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
			Text(TruncateToEllipsis(p->VisibleName, GUI2::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());
		}

		return 20;
	}

	int DrawFloatProperty(Config2::Property* p)
	{
		static Config2::CColor* PropertyBase = Config2::GetColor("theme-property-base");
		static Config2::CColor* PropertyAccent = Config2::GetColor("theme-property-accent");
		Config2::CFloat* Value = (Config2::CFloat*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = GetCursorPos();

		bool PremiumLocked = p->IsPremium && !UserData::Premium;

		// draw label 
		{
			ImVec2 IconSize(14, 14);
			std::string ToolTipString = "Click for more info";
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			if (PremiumLocked)
			{
				DrawErrorIcon(255, IconSize);
				ToolTipString = "Premium users only";
			}
			else
			{
				DrawInfoIcon(255, IconSize);
			}

			auto ID = GetID((p->Name + "-status-icon-hoverable").c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(ToolTipString, IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, ("http://a4g4.com/help/index.php#" + p->Name).c_str(), 0, 0, SW_SHOW);
				}
			}

			SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
			Text(TruncateToEllipsis(p->VisibleName, GUI2::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());
		}
		
		// draw bar
		float SpaceAfterBar = 43;
		float BarLength = Window->ContentRegionRect.GetWidth() - GUI2::PropertyColumnPosition - Pos.x - SpaceAfterBar;
		{
			SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, (20 - 16) / 2));

			float DrawValue = Value->GetFactor();
			float UserSetValue = DrawInputFloat(p->Name + "##bar", *PropertyBase, *PropertyAccent, DrawValue, ImVec2(BarLength, 16));

			if (DrawValue != UserSetValue)
			{
				GUI2::WantMouse = true;
				Config2::SettingKeybindFor = nullptr;
	
				if (!PremiumLocked)
					Value->SetFactor(UserSetValue);
			}

			if (IsItemHovered())
			{
				GUI2::WantMouse = true;
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 8.f + (BarLength - 16.f) * DrawValue, 0));
				ToolTip(PremiumLocked ? "Premium users only" : Value->Stringify(), 20);
			}
		}

		// draw unit
		{
			PushFont(Arial12);
			SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + BarLength + 5, (20 - GetFontSize()) / 2));
			TextEx(Value->Unit.c_str());
			PopFont();
		}

		return 20;
	}

	int DrawColorProperty(Config2::Property* p)
	{
		Config2::CColor* Value = (Config2::CColor*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = GetCursorPos();

		bool PremiumLocked = p->IsPremium && !UserData::Premium;

		// draw label 
		{
			ImVec2 IconSize(14, 14);
			std::string ToolTipString = "Click for more info";
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			if (PremiumLocked)
			{
				DrawErrorIcon(255, IconSize);
				ToolTipString = "Premium users only";
			}
			else
			{
				DrawInfoIcon(255, IconSize);
			}

			auto ID = GetID((p->Name + "-status-icon-hoverable").c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(ToolTipString, IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, ("http://a4g4.com/help/index.php#" + p->Name).c_str(), 0, 0, SW_SHOW);
				}
			}

			SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
			Text(TruncateToEllipsis(p->VisibleName, GUI2::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());
		}

		// draw color
		{
			SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, 0));
			ImVec4 ImColor = *Value;

			if (((int)Value->GetR() + (int)Value->GetG() + (int)Value->GetB()) < 127 * 3) // this is a dark color
				PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 127));
			else // this is a bright color
				PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 127));
			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			if (ColorButton(("##color-button-" + p->Name).c_str(), ImColor, ImGuiColorEditFlags_NoTooltip, ImVec2(40, 20)))
				OpenPopup(("##color-picker-" + p->Name).c_str());
			else if (IsItemHovered())
			{
				GUI2::WantMouse = true;
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 20, 0));
				ToolTip(Value->Stringify(), 20);
			}
			PopStyleVar(2);
			PopStyleColor(1);

			PushStyleColor(ImGuiCol_Border, IM_COL32(85 / 2, 90 / 2, 95 / 2, 255));
			SetNextWindowSize(ImVec2(210, Value->GetHasAlpha() ? 235 : 255));
			if (BeginPopup(("##color-picker-" + p->Name).c_str()))
			{
				GUI2::WantMouse = true;
				PushFont(Arial16);
				std::string Title = TruncateToEllipsis(p->VisibleName, 210 - 20);
				ImVec2 Size = ImGui::CalcTextSize(Title.c_str());
				SetCursorPos(ImVec2(210 /2, 13) - Size / 2);
				Text(Title.c_str());
				PopFont();

				SetCursorPos(ImVec2(5, 30));
				SetNextItemWidth(200);
				if (Value->GetHasAlpha())
				{
					ColorPicker4(("##color-editor-" + p->Name).c_str(), &ImColor.x,
						ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel |
						ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV
					);
				}
				else
				{
					ColorPicker3(("##color-editor-" + p->Name).c_str(), &ImColor.x,
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
			PopStyleColor(1);
		}

		return 20;
	}

	int DrawVerticalStatefulProperty(Config2::Property* p)
	{
		static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
		static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
		static Config2::CColor* ButtonHovered = Config2::GetColor("theme-button-hovered");
		static Config2::CColor* ButtonBorder = Config2::GetColor("theme-button-border");
		static Config2::CColor* ButtonText = Config2::GetColor("theme-button-text");
		static Config2::CFloat* ButtonBorderSize = Config2::GetFloat("theme-button-border-size");
		Config2::CVerticalState* Value = (Config2::CVerticalState*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = GetCursorPos();

		bool PremiumLocked = p->IsPremium && !UserData::Premium;

		// draw label 
		{
			ImVec2 IconSize(14, 14);
			std::string ToolTipString = "Click for more info";
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			if (PremiumLocked)
			{
				DrawErrorIcon(255, IconSize);
				ToolTipString = "Premium users only";
			}
			else
			{
				DrawInfoIcon(255, IconSize);
			}

			auto ID = GetID((p->Name + "-status-icon-hoverable").c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(ToolTipString, IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, ("http://a4g4.com/help/index.php#" + p->Name).c_str(), 0, 0, SW_SHOW);
				}
			}

			SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
			Text(TruncateToEllipsis(p->VisibleName, GUI2::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());
		}

		PushFont(Arial14);
		PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		// draw property
		{
			const char* popupName = ("##popup-" + p->Name).c_str();
			bool alreadyOpen = IsPopupOpen(popupName);
			int nItems = Value->StateNames.size();
			std::string CurrentSelection = Value->StateNames.at(Value->Value.Get());

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonActive, 0);
			PushStyleColor(ImGuiCol_ButtonHovered, 0);
			// button/child thing
			{
				bool open = false;

				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, 0));
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*ButtonBase);
				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
				BeginChild(("##button-child-" + p->Name).c_str(), ImVec2(200, 20));

				// fuck you imgui, please let me just use ImGuiStyleVar_ChildBorderSize
				// draw border manually
				auto retard_Window = GetCurrentWindow();
				auto retard_DrawList = Window->DrawList;
				retard_DrawList->AddRect(retard_Window->Pos, retard_Window->Pos + retard_Window->Size, IM_COL32(0, 0, 0, 127), 3.f);

				SetCursorPos(ImVec2(5, 3));
				Text(CurrentSelection.c_str());

				// dropdown arrow
				{
					SetCursorPos(ImVec2(200 - 15, 5));
					if (alreadyOpen)
						DrawUpArrow(255, ImVec2(9, 9));
					else
						DrawDownArrow(255, ImVec2(9, 9));
				}

				// dummy button across whole child
				{

					SetCursorPos(ImVec2(0, 0));
					if (Button(("##button-invis-" + p->Name).c_str(), ImVec2(200, 20)))
					{
						open = true;
					}
				}
				GUI2::WantMouse |= IsItemHovered() || IsItemActive();


				EndChild();
				PopStyleColor(2);
				PopStyleVar(2);

				if (open)
					OpenPopup(popupName);
			}
			PopStyleColor(3);

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonActive);
			PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonHovered);
			// popup
			{
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, 0));
				SetNextWindowPos(ImVec2(Window->DC.CursorPos + ImVec2(0, 25)));
				SetNextWindowSize(ImVec2(200, min(nItems, 10) * 20));
				PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 127));
				PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
				PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
				if (BeginPopup(popupName))
				{
					GUI2::WantMouse = true;
					for (size_t i = 0; i < Value->StateNames.size(); i++)
					{

						SetCursorPos(ImVec2(0, i * 20));
						if (Button(("##li" + p->Name + Value->StateNames.at(i)).c_str(), ImVec2(200, 20)))
						{
							Value->Value.Set(i);
							CloseCurrentPopup();
						}

						SetCursorPos(ImVec2(5, i * 20 + (20 - GetFontSize()) / 2));
						Text(Value->StateNames.at(i).c_str());
					}
					EndPopup();
				}
				PopStyleColor(1);
				PopStyleVar(2);
			}
			PopStyleColor(3);
		}
		PopFont();
		PopStyleVar(1);

		// keybind (yoinked from DrawBooleanProperty)
		if (Value->Bindable)
		{
			PushFont(Arial14);

			if (p == Config2::SettingKeybindFor) // this is being set right now
			{
				PushFont(Arial12);
				const char* Prefix = "[PRESS A KEY]";
				ImVec2 PrefixSize = CalcTextSize(Prefix);
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 209, (20 - PrefixSize.y) / 2));
				Text(Prefix);
				PopFont();
			}
			else if (Value->BoundToKey >= 0) // this key is already bound
			{
				const char* Prefix = "PRESS";
				std::string KeyName = Keybind::KeyNames[Value->BoundToKey];
				const char* Suffix = "TO LOOP";

				PushFont(Arial12);
				ImVec2 PrefixSize = CalcTextSize(Prefix);
				ImVec2 SuffixSize = CalcTextSize(Suffix);
				PopFont();
				ImVec2 KeyNameSize = CalcTextSize(KeyName.c_str());

				int x = GUI2::PropertyColumnPosition + 209;
				SetCursorPos(Pos + ImVec2(x, (20 - PrefixSize.y) / 2));
				PushFont(Arial12);
				Text(Prefix);
				PopFont();
				x += 5 + PrefixSize.x;

				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

				SetCursorPos(Pos + ImVec2(x, 0));
				if (Button((KeyName + "##" + p->Name).c_str(), ImVec2(KeyNameSize.x + 10, 20)))
				{
					std::vector<void*>& vec = Keybind::Binds[Value->BoundToKey];
					for (size_t i = 0; i < vec.size(); i++)
						if (vec.at(i) == (void*)p)
							vec.erase(vec.begin() + i--);

					Value->BoundToKey = -1;
					Config2::SettingKeybindFor = nullptr;
					GUI2::WantMouse = true;
				}
				else if (IsItemHovered())
				{
					SetCursorPos(Pos + ImVec2(x + KeyNameSize.x / 2 + 5, 0));
					ToolTip("Click To Clear", 20);
					GUI2::WantMouse = true;
				}

				PopStyleColor(2);
				PopStyleVar(2);
				x += KeyNameSize.x + 10 + 5;


				SetCursorPos(Pos + ImVec2(x, (20 - SuffixSize.y) / 2));
				PushFont(Arial12);
				Text(Suffix);
				PopFont();

			}
			else if (!PremiumLocked) // the key is not bound & we are able to bind it
			{
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition + 205, 0));

				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleColor(ImGuiCol_Border, (ImVec4)*ButtonBorder);
				PushStyleVar(ImGuiStyleVar_FrameBorderSize, ButtonBorderSize->Get());
				PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

				if (Button(("Bind##" + p->Name).c_str(), ImVec2(40, 20)))
				{
					Config2::SettingKeybindFor = p;
					GUI2::WantMouse = true;
				}
				else if (IsItemHovered())
				{
					GUI2::WantMouse = true;
				}

				PopStyleColor(2);
				PopStyleVar(2);
			}

			PopFont();
		}
		return 20;
	}

	int DrawMultiSelectorProperty(Config2::Property* p)
	{
		static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
		static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
		static Config2::CColor* ButtonHovered = Config2::GetColor("theme-button-hovered");
		static Config2::CColor* ButtonBorder = Config2::GetColor("theme-button-border");
		static Config2::CColor* ButtonText = Config2::GetColor("theme-button-text");
		static Config2::CFloat* ButtonBorderSize = Config2::GetFloat("theme-button-border-size");
		Config2::CMultiSelect* Value = (Config2::CMultiSelect*)p->Value;

		auto Window = GetCurrentWindow();
		auto DrawList = Window->DrawList;
		ImVec2 Pos = GetCursorPos();

		bool PremiumLocked = p->IsPremium && !UserData::Premium;

		// draw label 
		{
			ImVec2 IconSize(14, 14);
			std::string ToolTipString = "Click for more info";
			SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));

			if (PremiumLocked)
			{
				DrawErrorIcon(255, IconSize);
				ToolTipString = "Premium users only";
			}
			else
			{
				DrawInfoIcon(255, IconSize);
			}

			auto ID = GetID((p->Name + "-status-icon-hoverable").c_str());
			auto BB = ImRect(Window->DC.CursorPos, Window->DC.CursorPos + IconSize);
			ItemAdd(BB, ID);
			if (ItemHoverable(BB, ID))
			{
				SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ToolTip(ToolTipString, IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, ("http://a4g4.com/help/index.php#" + p->Name).c_str(), 0, 0, SW_SHOW);
				}
			}

			SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, (20 - GetFontSize()) / 2));
			Text(TruncateToEllipsis(p->VisibleName, GUI2::PropertyColumnPosition - (6 + IconSize.x + 6) - 10).c_str());
		}

		PushFont(Arial14);
		PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		// draw property
		{
			const char* popupName = ("##popup-" + p->Name).c_str();
			bool alreadyOpen = IsPopupOpen(popupName);
			int nItems = Value->StateNames.size();
			int selectedCount = Value->CountSelected();
			std::string CurrentSelection = "None selected";
			if (selectedCount > 0)
				CurrentSelection = std::to_string(selectedCount) + " selected";

			PushStyleColor(ImGuiCol_Button, 0);
			PushStyleColor(ImGuiCol_ButtonActive, 0);
			PushStyleColor(ImGuiCol_ButtonHovered, 0);
			// button/child thing
			{
				bool open = false;

				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, 0));
				PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*ButtonBase);
				PushStyleColor(ImGuiCol_Text, (ImVec4)*ButtonText);
				PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
				PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
				BeginChild(("##button-child-" + p->Name).c_str(), ImVec2(200, 20));

				// fuck you imgui, please let me just use ImGuiStyleVar_ChildBorderSize
				// draw border manually
				auto retard_Window = GetCurrentWindow();
				auto retard_DrawList = Window->DrawList;
				retard_DrawList->AddRect(retard_Window->Pos, retard_Window->Pos + retard_Window->Size, IM_COL32(0, 0, 0, 127), 3.f);

				SetCursorPos(ImVec2(5, 3));
				Text(CurrentSelection.c_str());

				// dropdown arrow
				{
					SetCursorPos(ImVec2(200 - 15, 5));
					if (alreadyOpen)
						DrawUpArrow(255, ImVec2(9, 9));
					else
						DrawDownArrow(255, ImVec2(9, 9));
				}

				// dummy button across whole child
				{

					SetCursorPos(ImVec2(0, 0));
					if (Button(("##button-invis-" + p->Name).c_str(), ImVec2(200, 20)))
					{
						open = true;
					}
				}
				GUI2::WantMouse |= IsItemHovered() || IsItemActive();


				EndChild();
				PopStyleColor(2);
				PopStyleVar(2);

				if (open)
					OpenPopup(popupName);
			}
			PopStyleColor(3);

			// popup
			{
				SetCursorPos(Pos + ImVec2(GUI2::PropertyColumnPosition, 0));
				SetNextWindowPos(ImVec2(Window->DC.CursorPos + ImVec2(0, 25)));
				SetNextWindowSize(ImVec2(200, min(nItems, 10) * 20));
				PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 127));
				PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
				PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
				if (BeginPopup(popupName))
				{
					GUI2::WantMouse = true;
					for (size_t i = 0; i < Value->StateNames.size(); i++)
					{
						bool selected = Value->Get(i);
						if (selected)
						{
							PushStyleColor(ImGuiCol_Button, (ImVec4)*ButtonActive);
							PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonActive);
							PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonHovered);
						}
						else
						{
							PushStyleColor(ImGuiCol_Button, 0);
							PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonHovered);
							PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonActive);
						}

						SetCursorPos(ImVec2(0, i * 20));
						if (Button(("##li" + p->Name + Value->StateNames.at(i)).c_str(), ImVec2(200, 20)))
						{
							Value->Set(i, !selected);
						}
						PopStyleColor(3);

						SetCursorPos(ImVec2(20, i * 20 + 3));
						Text(Value->StateNames.at(i).c_str());

						SetCursorPos(ImVec2(5, i * 20 + 5));
						if (selected)
							DrawCheckmark(255, ImVec2(9, 9));
					}
					EndPopup();
				}
				PopStyleColor(1);
				PopStyleVar(2);
			}
		}
		PopFont();
		PopStyleVar(1);

		return 20;
	}
}

void GUI2::LoadingScreen()
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
		IntroAnimation = Animation::newAnimation("intro-animation", 0);
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
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,ThisContentOpacity));
	ImGui::PushFont(Arial18Italics);

	ImGui::SetNextWindowPos(WindowCenter - FrameSize / 2, ImGuiCond_Always);
	ImGui::SetNextWindowSize(FrameSize, ImGuiCond_Always);
	ImGui::Begin("##LMAOBRUUHHHHHHHH", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);

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
		DrawList->PathArcTo(ImVec2(a.x, b.y), radius, IM_PI /2 , IM_PI, 16);
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

	std::string txt = std::to_string((int)(VisibleLoadProgress * 100.f + 0.5f)) + "%";
	ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
	ImGui::SetCursorPos(ImVec2(FrameSize.x/2 - txtSize.x/2, FrameSize.y * 0.8f));
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

void GUI2::AuthenticationScreen(float ContentOpacity)
{
	static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
	static Config2::CColor* ButtonHovered = Config2::GetColor("theme-button-hovered");
	static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
	static Config2::CColor* ButtonBorder = Config2::GetColor("theme-button-border");
	static Config2::CColor* ButtonText = Config2::GetColor("theme-button-text");
	static Config2::CFloat* ButtonBorderSize = Config2::GetFloat("theme-button-border-size");
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
	ImGui::Begin("##Authentication", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;
	ImGui::PushFont(Arial18);

	IM_COL32(85 / 2, 90 / 2, 95 / 2, 255),

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
		ImGui::InputTextWithPlaceholder("AccountEmail", "Email", Email, 512);

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
		ImGui::InputTextWithPlaceholder("AccountPassword", "Password", Password, 512);

		DrawList->AddLine(Window->Pos + Position + ImVec2(0, 29), Window->Pos + ImVec2(FrameSize.x - InputPadding, Position.y + 29), IM_COL32(175, 175, 175, ThisContentOpacity));
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
		if (ImGui::Button("Log In", ImVec2(ButtonWidth, 30)) && !IntroAnimation2 && !UserData::BusyAttemptingLogin)
		{
			UserData::LoginInformation* info = new UserData::LoginInformation{};
			info->Email = std::string(Email);
			info->Password = std::string(Password);
			CreateThread(NULL, 0, UserData::AttemptLogin, (LPVOID)info, 0, NULL);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 1));
		if (ImGui::Button("Register Online", ImVec2(ButtonWidth, 30)))
		{
			ShellExecute(0, 0, "http://a4g4.com/register/", 0, 0, SW_SHOW);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 2));
		if (ImGui::Button("Play Free", ImVec2(ButtonWidth, 30)) && !IntroAnimation2)
		{
			Config::UserInfo::Authenticated = true;
			Config::UserInfo::Premium = true;

			UserData::Initialized = true;
			UserData::Authenticated = true;
			UserData::Premium = false;

			IntroAnimation2 = Animation::newAnimation("intro-2", 0);
		}

		ImGui::SetCursorPos(ImVec2(XPos, YPos + (ButtonHeight + ButtonSpacing) * 3));
		if (ImGui::Button("Cancel / Eject", ImVec2(ButtonWidth, 30)))
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

void GUI2::AuthenticationIntro()
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
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255,255,255,255));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, floor(ImGui::lerp(2.f, 0.f, f) + 0.5f));

		ImGui::SetNextWindowSize(Size, ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::Begin("##intro-2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
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

void GUI2::DrawNormalTab(Config2::Tab* t, std::string GroupPrefix)
{
	static Config2::CColor* WidgetBackground = Config2::GetColor("theme-widget-background");
	static Config2::CColor* WidgetTitleText = Config2::GetColor("theme-widget-title");

	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;

	int WidgetWidth = Window->ContentRegionRect.GetWidth();
	int WidgetX = t->HorizontalPadding + ImGui::GetCursorPosX(), WidgetY = t->TopPadding + t->VerticalPadding + ImGui::GetCursorPosY();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
	for (size_t g = 0; g < t->Groups.size(); g++)
	{
		Config2::Group* Group = t->Groups[g];
		if (Group->Title == "__META__") continue;
		if (Group->Title.substr(0, GroupPrefix.size()) != GroupPrefix) continue;

		int GroupHeight = Group->GetDrawHeight();
		int GroupY = Group->Padding;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Group->Rounding);
		ImGui::PushFont(Arial16);

		// TODO: each group should have meta properties like "backgroundColor"
		ImGui::SetCursorPos(ImVec2(WidgetX, WidgetY));
		ImGui::BeginChild((t->Name + "-" + std::to_string(g)).c_str(), ImVec2(WidgetWidth - t->HorizontalPadding - WidgetX, GroupHeight), false, ImGuiWindowFlags_NoDecoration);
		auto GroupWindow = ImGui::GetCurrentWindow();

		if (Group->ShowTitle)
		{
			ImGui::SetCursorPos(ImVec2(5, GroupY));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::Text(Group->Title.substr(GroupPrefix.size()).c_str());
			ImGui::PopFont();
			ImGui::PopStyleColor(1);
			GroupY += 18 + 5;
		}

		// draw properties
		{
			size_t nDrawnProps = 0;
			for (size_t p = 0; p < Group->Properties.size(); p++)
			{
				auto Property = Group->Properties[p];
				if (Property->IsVisible && !Property->IsVisible()) continue;

				if (nDrawnProps > 0)
					GroupY += Group->Padding;

				ImGui::SetCursorPos(ImVec2(0, GroupY));

				switch (Property->Type)
				{
				case Config2::PropertyType::BOOLEAN:
					GroupY += ImGui::DrawBooleanProperty(Property);
					break;
				case Config2::PropertyType::FLOAT:
					GroupY += ImGui::DrawFloatProperty(Property);
					break;
				case Config2::PropertyType::COLOR:
					GroupY += ImGui::DrawColorProperty(Property);
					break;
				case Config2::PropertyType::VSTATEFUL:
					GroupY += ImGui::DrawVerticalStatefulProperty(Property);
					break;
				case Config2::PropertyType::MULTISELECT:
					GroupY += ImGui::DrawMultiSelectorProperty(Property);
					break;
				default:
				case Config2::PropertyType::LABEL:
					GroupY += ImGui::DrawTextProperty(Property);
					break;
				}

				nDrawnProps++;
			}
		}

		GroupY += Group->Padding;
		ImGui::EndChild();
		Group->SetDrawHeight(GroupY);

		ImGui::PopStyleVar(1);
		ImGui::PopFont();

		WidgetY += GroupHeight + t->VerticalPadding;
	}

	ImGui::PopStyleColor(1);

	return;
}

void GUI2::DrawActiveTab()
{
	static Config2::CColor* WidgetBackground = Config2::GetColor("theme-widget-background");
	static Config2::CColor* WidgetTitleText = Config2::GetColor("theme-widget-title");
	static Config2::CColor* PropertyText = Config2::GetColor("theme-property-text");
	static Config2::CColor* LegitRageSwitchBackground = Config2::GetColor("theme-legit-rage-switch-background");
	static Config2::CColor* LegitRageSwitchHighlight = Config2::GetColor("theme-legit-rage-switch-highlight");
	static Config2::CColor* LegitRageSwitchOutline = Config2::GetColor("theme-legit-rage-switch-outline");
	static Config2::CColor* LegitRageSwitchText = Config2::GetColor("theme-legit-rage-switch-text");
	static Config2::CColor* EjectBase = Config2::GetColor("theme-eject");
	static Config2::CColor* EjectButtonText = Config2::GetColor("theme-eject-button-text");
	static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
	static Config2::CColor* ButtonHovered = Config2::GetColor("theme-button-hovered");
	static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
	static Config2::CColor* ButtonBorder = Config2::GetColor("theme-button-border");
	static Config2::CColor* ButtonText = Config2::GetColor("theme-button-text");
	static Config2::CColor* SearchbarBackground = Config2::GetColor("theme-main-searchbar-background");
	static Config2::CColor* SearchbarText = Config2::GetColor("theme-main-searchbar-text");
	static Config2::CFloat* LegitRageSwitchBorderSize = Config2::GetFloat("theme-legit-rage-switch-border-size");
	static Config2::CFloat* ButtonBorderSize = Config2::GetFloat("theme-button-border-size");

	if (!ActiveTab) return;

	auto Window = ImGui::GetCurrentWindow();
	auto DrawList = Window->DrawList;

	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*PropertyText);

	if (ActiveTab->Name == "Offence" || ActiveTab->Name == "Defence")
	{
		bool IsOffencePage = ActiveTab->Name == "Offence";

		static Config2::Property* MasterSwitch = Config2::GetProperty(IsOffencePage ? "offence-mode" : "defence-mode");
		if (!MasterSwitch)
		{
			MasterSwitch = Config2::GetProperty(IsOffencePage ? "offence-mode" : "defence-mode");
			if (!MasterSwitch) return;
		}
		Config2::CHorizontalState* MasterMode = (Config2::CHorizontalState*)MasterSwitch->Value;

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
			ImGui::BeginChild(IsOffencePage ? "offence-main-top-gui-switch" : "defence-main-top-gui-switch", ImVec2(Width, 40), true, 0);

			auto bruh_window = ImGui::GetCurrentWindow();
			auto bruh_dl = bruh_window->DrawList;

			double TimePassed = MasterMode->Value.GetTimeSinceChange();
			double AnimFactor = Animation::animate(TimePassed, 0.15f, Animation::Interpolation::easeInOutQuint);
			if (MasterMode->Value.Get() == 0) AnimFactor = 1.f - AnimFactor;
			float XOffset = Animation::lerp(5.f, Width/2.f, AnimFactor);
			bruh_dl->AddRectFilled(bruh_window->Pos + ImVec2(XOffset, 5), bruh_window->Pos + ImVec2(XOffset + Width/2 - 5, 35), *LegitRageSwitchHighlight, 5.f);

			ImGui::SetCursorPos(ImVec2(0, 0));
			if (ImGui::Button(IsOffencePage ? "Legit##big-switch-offence" : "Legit##big-switch-defence", ImVec2(Width / 2, 40)))
			{
				WantMouse = true;
				Config2::SettingKeybindFor = nullptr;
				MasterMode->Value.Set(0);
			}
			WantMouse |= ImGui::IsItemHovered();

			ImGui::SetCursorPos(ImVec2(Width / 2, 0));
			if (ImGui::Button(IsOffencePage ? "Rage##big-switch-offence" : "Rage##big-switch-defence", ImVec2(Width / 2, 40)))
			{
				WantMouse = true;
				Config2::SettingKeybindFor = nullptr;
				MasterMode->Value.Set(1);
			}
			WantMouse |= ImGui::IsItemHovered();

			ImGui::EndChild();

			ImGui::PopStyleColor(6);
			ImGui::PopStyleVar(2);
			ImGui::PopFont();
		}

		// the rest of the shit
		ImGui::SetCursorPos(ImVec2(0, 60));
		DrawNormalTab(ActiveTab, MasterMode->Value.Get() == 0 ? "legit-" : "rage-");
	}
	else if (ActiveTab->Name == "Eject")
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::SetCursorPos(ImVec2(10.f, 10.f));
		ImGui::BeginChild("##eject-confirm", ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, 171));

		// title text
		{
			ImGui::PushFont(Arial18Bold);
			
			const char* Text = "Are you sure you would like to eject?";
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
			ImGui::TextEx("- your config will not be saved");
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f));
			ImGui::TextEx("- your theme will not be saved");
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f));
			ImGui::TextEx("- this will remove the cheat from your computer's memory");
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f + 18.f));
			ImGui::TextEx("- this will disable all effects");
			ImGui::SetCursorPos(ImVec2(20.f, 2 + 18.f + 15.f + 18.f + 18.f + 18.f + 18.f));
			ImGui::TextEx("- there is a *small* chance that CS:GO will crash");

			ImGui::PopFont();
		}

		// confirm button
		{
			ImVec4 Color = *EjectBase;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			ImGui::PushStyleColor(ImGuiCol_Button, Color);
			Color.w *= 0.85f;
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Color);
			Color.w *= 0.8f;
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Color);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*EjectButtonText);

			ImGui::PushFont(Arial18BoldItalics);

			ImGui::SetCursorPos(ImVec2(10.f, 131));
			Ejected |= ImGui::Button("EJECT NOW##confirm", ImVec2(ImGui::GetCurrentWindow()->Size.x - 20.f, 30.f));

			ImGui::PopFont();

			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(4);
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}
	else if (ActiveTab->Name == "Skinchanger")
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
			static int WindowHeight = 0;
			ImGui::SetCursorPos(ImVec2(10.f, y + 10));
			ImGui::BeginChild("##knife-skins", ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, WindowHeight));
			auto Widget = ImGui::GetCurrentWindow();

			static std::vector<std::string> Knifes = {
				"Talon", "talon",
				"Karambit", "karambit",
				"M9-Bayonet", "m9-bayonet",
				"Huntsman", "huntsman",
				"T Default", "terrorist-knife",
				"CT Default", "counter-terrorist-knife"
			};

			ImGui::SetCursorPos(ImVec2(10.f, 10.f));
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::Text("Knives");
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
			ImGui::Separator();

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
			ImGui::BeginChild("##weapon-skins", ImVec2(Window->ContentRegionRect.Max.x - Window->ContentRegionRect.Min.x - 20.f, WindowHeight + 10.f));
			auto Widget = ImGui::GetCurrentWindow();

			ImGui::SetCursorPos(ImVec2(10.f, 10.f));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
			ImGui::PushFont(Arial18BoldItalics);
			ImGui::Text("Guns");
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
					if (ImGui::Button(("##dummy-weapongroup-btn-" + Skins::WeaponGroupNames[i]).c_str(), ImVec2(62, 30)))
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
					if (ImGui::Button(("##dummy-weapongroup-btn-" + WeaponName).c_str(), ImVec2(110, 30)))
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
			Config2::Property* CurrentWeaponSkinProp =
				Config2::GetProperty("skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[(int)CurrentSelectedWeapon])));
			Config2::CPaintKit* CurrentPaintKit = (Config2::CPaintKit*)CurrentWeaponSkinProp->Value;

			bool WeaponHasSkinSelection = CurrentPaintKit->PaintKit->ID > 0;
			std::string WeaponSkinName = WeaponHasSkinSelection ? CurrentPaintKit->Stringify() : "None";

			// current skin for this weapon
			{
				static Animation::Anim* MyAnimation = Animation::newAnimation("weapon-current-skin-hover", 0);

				const char* label = "Current: ";
				ImVec2 labelSize = ImGui::CalcTextSize(label);

				ImGui::SetCursorPos(ImVec2(202, 37 + (24 - labelSize.y)/2));
				ImGui::Text(label);

				ImGui::SetCursorPos(ImVec2(202 + labelSize.x + 5, 37));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*SearchbarBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild("##skin-weapon-current-selection", ImVec2(Widget->Size.x - ImGui::GetCursorPosX() - 10, 24), false);
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(1);

				Animation::changed(MyAnimation, ImGui::IsWindowHovered());
				float AnimFactor = Animation::animate(Animation::age(MyAnimation), 0.15);
				if (!MyAnimation->state)
					AnimFactor = 1.f - AnimFactor;

				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)SearchbarText->ModulateAlpha(Animation::lerp(0.6f, 1.f, AnimFactor)));

				ImGui::SetCursorPos(ImVec2(4, 4));

				if (!WeaponHasSkinSelection)
					ImGui::Text("None"); 
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
						ImGui::DrawXIcon(Animation::lerp(0, 200, AnimFactor), ImVec2(9, 9));
						ImGui::SetCursorPos(ImVec2(maxX + 3, 3));
						ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(255, 255, 255, 150));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 255, 255, 50));
						if (ImGui::Button("##remove-active-weapon-skin", ImVec2(18, 18)))
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
				ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*SearchbarBackground);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
				ImGui::BeginChild("##skin-weapon-searchbar", ImVec2(Widget->Size.x - 202 - 10, 24), false);

				const char* InputLabel = "##WeaponSkinSearchTextInput";
				auto InputID = ImGui::GetID(InputLabel);
				static Animation::Anim* MySearchAnimation = Animation::newAnimation("weapon-skin-search");
				IsSearchingWeapon = ImGui::GetActiveID() == InputID || WeaponSearchQuery[0];

				float SearchAnimationFactor = Animation::animate(Animation::age(MySearchAnimation), 0.15);
				if (!IsSearchingWeapon)
					SearchAnimationFactor = 1.f - SearchAnimationFactor;

				Animation::changed(MySearchAnimation, IsSearchingWeapon);

				ImGui::SetCursorPos(ImVec2(5, 5));
				ImGui::DrawSearchIcon(200, ImVec2(14, 14));

				ImVec4 Color = *SearchbarText;
				ImGui::PushFont(Arial16);
				ImGui::PushStyleColor(ImGuiCol_Text, Color);
				ImGui::SetCursorPos(ImVec2(24, 4));
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (IsSearchingWeapon ? 48 : 28));
				ImGui::InputText(InputLabel, WeaponSearchQuery, 256);
				ImGui::PopStyleColor(1);

				if (!IsSearchingWeapon)
				{
					Color.w *= 0.6f;
					ImGui::SetCursorPos(ImVec2(24, 4));
					ImGui::PushStyleColor(ImGuiCol_Text, Color);
					ImGui::Text("Search Skins");
					ImGui::PopStyleColor(1);
				}
				else
				{
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 7, 7));
					ImGui::DrawXIcon(Animation::lerp(0, 200, Animation::animate(Animation::age(SearchAnimation), 0.15)), ImVec2(9, 9));
					// dummy button
					ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 3, 3));
					ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(255, 255, 255, 150));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 255, 255, 50));
					if (ImGui::Button("##cancel-search-dummy-weapon-skin", ImVec2(18, 18)))
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
				ImGui::BeginChild("##weapon-skin-listings", ImVec2(Widget->Size.x - 203 - 11, 37 + WeaponNamesHeight - ImGui::GetCursorPosY()), false);
				auto win = ImGui::GetCurrentWindow();
				Widget->DrawList->AddRect(
					win->Pos - ImVec2(1, 1),
					win->Pos + win->Size + ImVec2(1, 1),
					ButtonOutlineColor,
					0.f,0
				);

				std::string SearchQuery = WeaponSearchQuery;

				ImGui::PushFont(Arial16);
				size_t SkinsShown = 0;
				for (size_t i = 0; i < Skins::PaintKits.size(); i++)
				{
					Skins::PaintKit PK = Skins::PaintKits[i];
					std::vector<std::vector<int>> IntendedApplicants = {}; // {{Weapon Knife or Glove, Mode, Version},{same},{...}}
					for (size_t j = 0; j < PK.Weapons.size(); j++)
						IntendedApplicants.push_back({ (int)PK.Weapons[j], 0, (int)j });
					for (size_t j = 0; j < PK.Knives.size(); j++)
						IntendedApplicants.push_back({ (int)PK.Knives[j], 1, (int)j });
					for (size_t j = 0; j < PK.Gloves.size(); j++)
						IntendedApplicants.push_back({ (int)PK.Gloves[j], 2, (int)j });

					for (size_t j = 0; j < IntendedApplicants.size(); j++)
					{
						int NameId = IntendedApplicants[j][0];
						int Mode = IntendedApplicants[j][1];
						int Version = IntendedApplicants[j][2];

						std::string SkinName = 
							(Mode == 0 ? Skins::WeaponNames[NameId] : (Mode == 1 ? Skins::KnifeNames[NameId] : Skins::GloveNames[NameId])) +
							" | " + 
							PK.VisibleName;

						if (IsSearchingWeapon && !TextService::Contains(SearchQuery, SkinName, false))
							continue;

						ImGui::SetCursorPos(ImVec2(0, SkinsShown * 30));
						if (CurrentPaintKit->PaintKit && CurrentPaintKit->PaintKit->ID == PK.ID && Mode == CurrentPaintKit->Mode && Version == CurrentPaintKit->Version)
						{
							win->DrawList->AddRectFilled(win->DC.CursorPos, win->DC.CursorPos + ImVec2(win->Size.x, 30), ButtonSelectedColor);
						}
						else if (ImGui::Button(("##invisible-weapon-skin-select-" + SkinName).c_str(), ImVec2(win->Size.x, 30)))
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
	else if (ActiveTab->Name == "Theme")
	{
	int WidgetWidth = Window->ContentRegionRect.GetWidth();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
	ImGui::SetCursorPos(ImVec2(10, 10));
	ImGui::BeginChild("##theme-import/export", ImVec2(WidgetWidth - 20, 100), false, ImGuiWindowFlags_NoDecoration);
	auto InnerWindow = ImGui::GetCurrentWindow();

	ImGui::SetCursorPos(ImVec2(5, 5));
	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
	ImGui::PushFont(Arial18BoldItalics);
	ImGui::Text("Save / Load");
	ImGui::SetCursorPos(ImVec2(5, 52));
	ImGui::Text("Presets");
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

		auto ID = ImGui::GetID("theme-import-export-info-btn");
		auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
		ImGui::ItemAdd(BB, ID);
		if (ImGui::ItemHoverable(BB, ID))
		{
			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
			ImGui::ToolTip("Click for more info", IconSize.y);
			GUI2::WantMouse = true;
			if (GImGui->IO.MouseClicked[0])
			{
				ShellExecute(0, 0, "http://a4g4.com/help/index.php#theme", 0, 0, SW_SHOW);
			}
		}


		ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, 0));
		if (ImGui::Button("Export##theme", ImVec2(60, 20)))
		{
			Config2::PromptExportThemeFile();
		}

		ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6 + 60 + 6, 0));
		if (ImGui::Button("Import##theme", ImVec2(60, 20)))
		{
			Config2::PromptImportThemeFile();
		}

	}

	// preset buttons
	{
		ImVec2 IconSize(14, 14);
		ImVec2 Pos(0, 52 + 18 + 5);
		ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
		ImGui::DrawInfoIcon(255, IconSize);

		auto ID = ImGui::GetID("theme-presets-info-btn");
		auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
		ImGui::ItemAdd(BB, ID);
		if (ImGui::ItemHoverable(BB, ID))
		{
			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
			ImGui::ToolTip("Click for more info", IconSize.y);
			GUI2::WantMouse = true;
			if (GImGui->IO.MouseClicked[0])
			{
				ShellExecute(0, 0, "http://a4g4.com/help/index.php#theme", 0, 0, SW_SHOW);
			}
		}

		int x = 6 + IconSize.x + 6;
		ImGui::SetCursorPos(Pos + ImVec2(x, 0));
		if (ImGui::Button("Dark##themepreset", ImVec2(60, 20)))
		{
			Config2::ImportTheme(ConfigConstants::ThemeDark, ConfigConstants::ThemeDarkSize);
		}
		x += 66;

		ImGui::SetCursorPos(Pos + ImVec2(x, 0));
		if (ImGui::Button("Light##themepreset", ImVec2(60, 20)))
		{
			Config2::ImportTheme(ConfigConstants::ThemeLight, ConfigConstants::ThemeLightSize);
		}
		x += 66;

		ImGui::SetCursorPos(Pos + ImVec2(x, 0));
		if (ImGui::Button("Contrast##themepreset", ImVec2(60, 20)))
		{
			Config2::ImportTheme(ConfigConstants::ThemeContrast, ConfigConstants::ThemeContrastSize);
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
	else if (ActiveTab->Name == "Config")
	{
		int WidgetWidth = Window->ContentRegionRect.GetWidth();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*WidgetBackground);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPos(ImVec2(10, 10));
		ImGui::BeginChild("##config-import/export", ImVec2(WidgetWidth - 20, 100), false, ImGuiWindowFlags_NoDecoration);
		auto InnerWindow = ImGui::GetCurrentWindow();

		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*WidgetTitleText);
		ImGui::PushFont(Arial18BoldItalics);
		ImGui::Text("Save / Load");
		ImGui::SetCursorPos(ImVec2(5, 52));
		ImGui::Text("Presets");
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

			auto ID = ImGui::GetID("config-import-export-info-btn");
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip("Click for more info", IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, "http://a4g4.com/help/index.php#config", 0, 0, SW_SHOW);
				}
			}


			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6, 0));
			if (ImGui::Button("Export##config", ImVec2(60, 20)))
			{
				Config2::PromptExportConfigFile();
			}

			ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x + 6 + 60 + 6, 0));
			if (ImGui::Button("Import##config", ImVec2(60, 20)))
			{
				Config2::PromptImportConfigFile();
			}

		}

		// preset buttons
		{
			ImVec2 IconSize(14, 14);
			ImVec2 Pos(0, 52 + 18 + 5);
			ImGui::SetCursorPos(Pos + ImVec2(6, (20 - IconSize.y) / 2));
			ImGui::DrawInfoIcon(255, IconSize);

			auto ID = ImGui::GetID("config-presets-info-btn");
			auto BB = ImRect(InnerWindow->DC.CursorPos, InnerWindow->DC.CursorPos + IconSize);
			ImGui::ItemAdd(BB, ID);
			if (ImGui::ItemHoverable(BB, ID))
			{
				ImGui::SetCursorPos(Pos + ImVec2(6 + IconSize.x / 2, (20 - IconSize.y) / 2));
				ImGui::ToolTip("Click for more info", IconSize.y);
				GUI2::WantMouse = true;
				if (GImGui->IO.MouseClicked[0])
				{
					ShellExecute(0, 0, "http://a4g4.com/help/index.php#config", 0, 0, SW_SHOW);
				}
			}

			int x = 6 + IconSize.x + 6;
			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button("Off##configpreset", ImVec2(60, 20)))
			{
				Config2::ImportConfig(ConfigConstants::ConfigOff, ConfigConstants::ConfigOffSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button("Legit##configpreset", ImVec2(60, 20)))
			{
				Config2::ImportConfig(ConfigConstants::ConfigLegit, ConfigConstants::ConfigLegitSize);
			}
			x += 66;

			ImGui::SetCursorPos(Pos + ImVec2(x, 0));
			if (ImGui::Button("Rage##configpreset", ImVec2(60, 20)))
			{
				Config2::ImportConfig(ConfigConstants::ConfigRage, ConfigConstants::ConfigRageSize);
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

void GUI2::MainScreen(float ContentOpacity, bool Interactable)
{
	unsigned char ThisContentOpacity = (unsigned char)(ContentOpacity * 255.f);
	static Config2::CColor* TopbarBackground = Config2::GetColor("theme-topbar-background");
	static Config2::CColor* TopbarText = Config2::GetColor("theme-topbar-text");
	static Config2::CColor* TablistBackground = Config2::GetColor("theme-overlay-background");
	static Config2::CColor* TablistText = Config2::GetColor("theme-overlay-text");
	static Config2::CColor* ActiveTablistText = Config2::GetColor("theme-overlay-active-text");
	static Config2::CColor* SearchbarBackground = Config2::GetColor("theme-main-searchbar-background");
	static Config2::CColor* SearchbarText = Config2::GetColor("theme-main-searchbar-text");
	static Config2::CColor* ButtonBase = Config2::GetColor("theme-button-background");
	static Config2::CColor* ButtonHovered = Config2::GetColor("theme-button-hovered");
	static Config2::CColor* ButtonActive = Config2::GetColor("theme-button-active");
	static Config2::CColor* EjectBase = Config2::GetColor("theme-eject");
	static Config2::CColor* MainBorder = Config2::GetColor("theme-border");
	static Config2::CColor* MainBackground = Config2::GetColor("theme-background");
	static Config2::CFloat* MainBorderSize = Config2::GetFloat("theme-border-size");


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
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 2.f);
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
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(85, 90, 95, 255));
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)*ButtonBase);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)*ButtonHovered);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)*ButtonActive);

	ImGui::PushFont(Arial14BoldItalics);
	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*TopbarText);
	int TitleBarHeight = 24;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, (TitleBarHeight - ImGui::GetFontSize())/2.f));
	ImGui::Begin((std::string("A4G4 - ") + (UserData::Premium ? "FULL VERSION" : "TRIAL VERSION")).c_str(), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | (Interactable ? 0 : ImGuiWindowFlags_NoInputs));
	ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(1);

	// init
	auto Window = ImGui::GetCurrentWindow();
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

	if (!ActiveTab && Config2::Tabs.size() > 0)
		ActiveTab = Config2::Tabs.at(0);

	float SearchAnimationFactor = Animation::animate(Animation::age(SearchAnimation), 0.15);
	if (!IsSearching)
		SearchAnimationFactor = 1.f - SearchAnimationFactor;

	ImVec2 OverlaySize(Animation::lerp(150, Window->Size.x, SearchAnimationFactor), Window->Size.y - TitleBarHeight);
	ImVec2 OverlayPosition(0, TitleBarHeight);

	// background gif/color idk yet
	{

	}

	// right side
	{
		ImGui::SetCursorPos(ImVec2(150, TitleBarHeight));
		ImGui::BeginChild("##main-right-side");

		auto w = ImGui::GetCurrentWindow();
		ImGui::PushClipRect(Window->Pos + OverlayPosition + ImVec2(OverlaySize.x, 0), Window->Pos + Window->Size, false);

		GUI2::DrawActiveTab();

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

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0,0,0,0));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		ImGui::SetCursorPos(OverlayPosition);
		ImGui::BeginChild("##left-side", OverlaySize, false, ImGuiWindowFlags_NoSavedSettings);

		// search bar
		{
			ImGui::SetCursorPos(ImVec2(5,5));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)*SearchbarBackground);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
			ImGui::BeginChild("##left-side-searchbar", ImVec2(OverlaySize.x - 10, 24), false, ImGuiWindowFlags_NoSavedSettings);

			const char* InputLabel = "##SearchTextInput";
			auto InputID = ImGui::GetID(InputLabel);

			IsSearching = ImGui::GetActiveID() == InputID || (SearchQuery && SearchQuery[0]);
			Animation::changed(SearchAnimation, IsSearching);

			ImGui::SetCursorPos(ImVec2(5,5));
			ImGui::DrawSearchIcon(200, ImVec2(14, 14));

			ImGui::PushFont(Arial16);
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*SearchbarText);
			ImGui::SetCursorPos(ImVec2(24, 4));
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (IsSearching ? 48 : 28));
			ImGui::InputText(InputLabel, SearchQuery, 256);
			ImGui::PopStyleColor(1);
			WantMouse |= ImGui::IsItemHovered();

			if (!IsSearching)
			{
				ImGui::SetCursorPos(ImVec2(24, 4));
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)SearchbarText->ModulateAlpha(0.6f));
				ImGui::Text("Search");
				ImGui::PopStyleColor(1);
			}
			else
			{
				Config2::SettingKeybindFor = nullptr;
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 7, 7));
				ImGui::DrawXIcon(Animation::lerp(0, 200, SearchAnimationFactor), ImVec2(9, 9));
				// dummy button
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 24 + 3, 3));
				ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(255, 255, 255, 150));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 255, 255, 50));
				if (ImGui::Button("##cancel-search-dummy", ImVec2(18, 18)))
				{
					ZeroMemory(SearchQuery, 256);
					if (ImGui::GetActiveID() == InputID)
						ImGui::ClearActiveID();
				}
				WantMouse |= ImGui::IsItemHovered();
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

			for (size_t i = 0; i < Config2::Tabs.size(); i++)
			{
				Config2::Tab* t = Config2::Tabs.at(i);
				bool isSelected = t == ActiveTab;
				bool isEject = t->Name == "Eject";

				if (isEject || isSelected)
				{
					ImGui::PushFont(Arial18Bold);

					if (isEject)
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)*EjectBase);
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
				if (ImGui::Button(("##dummy-tab-select-" + t->Name).c_str(), ImVec2(OverlaySize.x - 10, 20)))
				{
					ActiveTab = t;
					isSelected = true;
					Config2::SettingKeybindFor = nullptr;
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
				ImGui::DrawSelectionCursor(255, ImVec2(10, 8));
			}

			// hover triangle
			if (HoveredTabIndex >= 0 && SelectedTabIndex != HoveredTabIndex)
			{
				WantMouse = true;
				ImGui::SetCursorPos(ImVec2(12, 34 + 20 * HoveredTabIndex + (18 - 8) / 2));
				ImGui::DrawSelectionCursor(100, ImVec2(10, 8));
			}
		}
		// search results
		else
		{
			ImGui::SetCursorPos(ImVec2(5, 5 + 24 + 5));
			ImGui::Text("lmao good luck finding that ig");
			ImGui::SetCursorPosX(5);
			ImGui::Text(("you are searching for: " + std::string(SearchQuery)).c_str());
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
	}

	if (ThisContentOpacity < 255)
		ImGui::GetForegroundDrawList()->AddRectFilled(Window->Pos, Window->Pos + Window->Size, IM_COL32(30, 30, 30, 255 - ThisContentOpacity), 5.f);

	ImGui::End();
	ImGui::PopFont();

	ImGui::PopStyleVar(9);
	ImGui::PopStyleColor(14);
}

void GUI2::Init()
{
	L::Verbose("GUI2::Init running");
	while (!SearchQuery)
		if (SearchQuery = new char[256])
			ZeroMemory(SearchQuery, 256);

	SearchAnimation = Animation::newAnimation("search-open/close", 0);
	L::Verbose("GUI2::Init complete");
}

void GUI2::Main()
{
	L::Verbose("GUI2::Main executed");
	static bool Init = false;
	if (!Init)
	{
		GUI2::Init();
		Init = true;
	}

	WantMouse = false;
	if (IntroAnimation2 && IntroAnimation2->state != 69)
	{
		L::Verbose("GUI2::AuthenticationIntro running");
		AuthenticationIntro();
		L::Verbose("GUI2::AuthenticationIntro complete");
	}
	else if (UserData::Initialized)
	{
		if (Config::GetBool("show-menu"))
		{
			/*L::Verbose("GUI2::MainScreen running");
			MainScreen();
			L::Verbose("GUI2::MainScreen complete");*/

			
			L::Verbose("GUI::Main running");
			Ejected |= GUI::Main();
			L::Verbose("GUI::Main complete");
			
		}
	}
	else if (VisibleLoadProgress <= 1.f) // if == 1, currently animating
	{
		L::Verbose("GUI2::LoadingScreen running");
		LoadingScreen();
		L::Verbose("GUI2::LoadingScreen complete");
	}
	else
	{
		L::Verbose("GUI2::AuthenticationScreen running");
		AuthenticationScreen();
		L::Verbose("GUI2::AuthenticationScreen complete");
	}

	L::Verbose("Config2::ProcessKeys running");
	Config2::ProcessKeys();
	L::Verbose("Config2::ProcessKeys complete");



	L::Verbose("GUI2::Main complete");
}