#include "Include.hpp"
#include "GUI/HTTP.hpp"

namespace GUI
{
	bool ShowMenu = false;
}

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
ImFont* Consolas16;
ImFont* Consolas12;
ImFont* Consolas8;

namespace ImGui
{
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
			ImGui::PushFont(Consolas12);

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
}

void GUI::LoadFonts(ImGuiIO& io)
{
	FontDefault = io.Fonts->AddFontDefault();
	Consolas16 = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.f);
	Consolas12 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12.f);
	Consolas8 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 8.f);
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

	bool PressedEject = false;

	ImGui::SetNextWindowSize(ImVec2(340, 110 + 45 + 45 + 45 + 10), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Login", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

	GUI::Rain(ImVec4(.5f, .5f, .5f, 1.f));

	/*
		LOGIN FORM
	*/
	ImGui::PushFont(Consolas16);
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
	ImGui::PushFont(Consolas16);
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
	bool isEjecting = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX / 2.f, WindowSizeY / 2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		if (Config::UserInfo.Paid || Config::UserInfo.Developer)
			isEjecting = PaidHackMenu();
		else
			isEjecting = FreeHackMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		GUI::ShowMenu = true;
		ProcessingLoginMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_NONE)
	{
		GUI::ShowMenu = true;
		isEjecting = LoginMenu();
	}

	return isEjecting;
}

//Paid Hack Func
bool GUI::PaidHackMenu()
{
	bool PressedEject = false;

	if (!ShowMenu)			//if they arent displaying the menu... just return
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

	void ColorPicker(std::string name, Color* COLOR)
	{
		ImGui::Text(name.c_str());
		ImGui::SameLine();
		ImVec4 color = ImVec4(COLOR->color[0] / 255.f, COLOR->color[1] / 255.f, COLOR->color[2] / 255.f, 1.f);
		bool open_popup = ImGui::ColorButton(name.c_str(), color);
		if (open_popup)
		{
			ImGui::OpenPopup((name + "picker").c_str());
		}

		if (ImGui::BeginPopup((name + "picker").c_str()))
		{
			ImGui::SetNextItemWidth(100);
			float color_[3] = { COLOR->color[0] / 255.f, COLOR->color[1] / 255.f, COLOR->color[2] / 255.f };
			ImGui::ColorPicker3(name.c_str(), color_, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
			*COLOR = Color(int(color_[0] * 255), int(color_[1] * 255), int(color_[2] * 255), 255);
			color = ImVec4(color_[0], color_[1], color_[2], 1);

			ImGui::EndPopup();
		}
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
		ImGui::ColorPicker("Fake Color", &Config::antiaim.Fake);
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
	ImGui::ColorPicker("Box Color", &Config::visuals.BoxColor);
	ImGui::Checkbox("Skeleton###VSkeleton", &Config::visuals.Skeleton);
	ImGui::Checkbox("Name###VName", &Config::visuals.Name);
	ImGui::Checkbox("Health###VHealth", &Config::visuals.Health);
	ImGui::Checkbox("Ammo###VAmmo", &Config::visuals.Ammo);
	ImGui::Separator();

	ImGui::Text("Players - Chams");
	ImGui::Checkbox("Visible Chams###VVisibleCHams", &Config::visuals.VisibleChams);
	ImGui::ColorPicker("Visible Cham Color", &Config::visuals.VisibleColor);
	ImGui::Checkbox("Through Wall Chams###VWallCHams", &Config::visuals.ThroughWallChams);
	ImGui::ColorPicker("Through Wall Cham Color", &Config::visuals.ThroughWallColor);

}

bool GUI::FreeHackMenu()
{
	bool PressedEject = false;

	if (!ShowMenu)			//if they arent displaying the menu... just return
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

	ImGui::GetStyle().Alpha = 0.7;
	ImGui::GetStyle().FrameBorderSize = 0.f;
	ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1.f, 1.f, 1.f, 0.f);

	ImGui::End();

	return PressedEject;
}


#define randf() (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
#define randint(m, n) (int)(floor(randf() * (n + 1.f - m) + m))
#define square(x) (x)*(x)
#define addImVec2(a, b) ImVec2((a).x + (b).x, (a).y + (b).y)

inline float lerp(float a, float b, float f)
{
	return (b - a) * f + a;
}

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
		this->opacity = lerp(0.2, 1, this->z);
		this->speed = lerp(15.f, 30.f, this->z);
		this->height = floor(lerp(8, 15, this->z));
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
				Raindrop drop(lerp(newXRect[0], newXRect[2], randf()), lerp(newXRect[1], newXRect[3], randf()));
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
				Raindrop drop(lerp(newYRect[0], newYRect[2], randf()), lerp(newYRect[1], newYRect[3], randf()));
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
			drop->x = lerp(0, rain_cx, randf());
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