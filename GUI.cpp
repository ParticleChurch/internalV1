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

bool GUI::LoginMenu()
{
	static bool init = false;
	static float MainColor = 20.f / 255.f;
	if (!init)
	{
		init = true;

		ImGuiStyle* style = &ImGui::GetStyle();

		// turn off all default rounding and padding
		style->WindowRounding = 0.0f;
		style->WindowPadding = ImVec2(0, 0);
		style->ChildRounding = 0.0f;
		style->FrameRounding = 0.0f;
		style->FramePadding = ImVec2(0, 0);
		style->GrabRounding = 0.0f;
		style->PopupRounding = 0.0f;
		style->ScrollbarRounding = 0.0f;
		style->FrameBorderSize = 1.f;
		style->ItemSpacing = ImVec2(0, 0);
		style->ItemInnerSpacing = ImVec2(0, 0);

		// main window outline solid 1px white
		style->Colors[ImGuiCol_Border] = ImVec4(1.f, 1.f, 1.f, 1.f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(MainColor, MainColor, MainColor, 1.f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(MainColor, MainColor, MainColor, 0.f);

		// buttons
		style->Colors[ImGuiCol_Button] = ImVec4(MainColor, MainColor, MainColor, 0.5f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
	}

	bool PressedEject = false;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(440, 114));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), 0, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Login", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);

	// Login Form
	ImGui::GetStyle().FrameBorderSize = 0.f;
	static char Email[257] = "";
	ImGui::SetCursorPos(ImVec2(59, 30));
	ImGui::Text("Email:"); ImGui::SameLine();
	ImGui::InputText("###Email", Email, 257);

	static char Password[65] = "";
	ImGui::SetCursorPos(ImVec2(38, 55));
	ImGui::Text("Password:"); ImGui::SameLine();
	ImGui::InputText("###Password", Password, 65);

	// Eject button
	ImGui::GetStyle().FrameBorderSize = 1.f;
	ImGui::SetCursorPos(ImVec2(10, 83));
	if (ImGui::Button("Eject", ImVec2(99, 20)))
		PressedEject = true;

	// Free2Play button
	ImGui::SetCursorPos(ImVec2(10 + 99 + 8, 83));
	if (ImGui::Button("Free Version", ImVec2(99, 20)))
	{
		Config::UserInfo.AuthStatus = AUTH_STATUS_COMPLETE;
		Config::UserInfo.Email = "free@a4g4.com";
		Config::UserInfo.UserID = INT_MAX;
		Config::UserInfo.Paid = false;
		Config::UserInfo.Developer = false;
	}

	// Register button
	ImGui::SetCursorPos(ImVec2(10 + 99 + 8 + 99 + 8, 83));
	if (ImGui::Button("Register", ImVec2(99, 20)))
		ShellExecute(NULL, TEXT("open"), TEXT("https://www.a4g4.com"), NULL, NULL, 0);

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
	ImGui::End();

	return PressedEject;
}

bool GUI::Main()
{
	bool PressedEject = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX / 2.f, WindowSizeY / 2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		if (Config::UserInfo.Paid || Config::UserInfo.Developer)
			PressedEject = PaidHackMenu();
		else
			PressedEject = FreeHackMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		GUI::ShowMenu = true;
		ProcessingLoginMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_NONE)
	{
		GUI::ShowMenu = true;
		PressedEject = LoginMenu();
	}

	return PressedEject;
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

	void ColorPicker(std::string name, Color* COLOR) {
		ImVec4 color = ImVec4(COLOR->color[0] / 255.f, COLOR->color[1] / 255.f, COLOR->color[2] / 255.f, 1.f);
		bool open_popup = ImGui::ColorButton(name.c_str(), color);
		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
		open_popup |= ImGui::Button((name + "pallet").c_str());
		if (open_popup)
		{
			ImGui::OpenPopup((name + "picker").c_str());
			//backup_color = color;
		}

		if (ImGui::BeginPopup((name + "picker").c_str()))
		{
			ImGui::SetNextItemWidth(100);
			static float color_[3];
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
	ImGui::Checkbox("Crouch In Air###CrouchInAir", &Config::movement.CrouchInAir);
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
	ImGui::ColorPicker("Boxe Color###VBoxeColor", &Config::visuals.BoxColor);
	ImGui::Checkbox("Skeleton###VSkeleton", &Config::visuals.Skeleton);
	ImGui::Checkbox("Name###VName", &Config::visuals.Name);
	ImGui::Checkbox("Health###VHealth", &Config::visuals.Health);
	ImGui::Checkbox("Ammo###VAmmo", &Config::visuals.Ammo);
	ImGui::Separator();

	ImGui::Text("Players - Chams");
	ImGui::Checkbox("Visible Chams###VVisibleCHams", &Config::visuals.VisibleChams);
	ImGui::ColorPicker("Visible Cham Color###VVisChamColor", &Config::visuals.VisibleColor);
	ImGui::Checkbox("Through Wall Chams###VWallCHams", &Config::visuals.ThroughWallChams);
	ImGui::ColorPicker("Through Wall Cham Color###VWallChamColor", &Config::visuals.ThroughWallColor);

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
	

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
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


#define randf() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
#define square(x) (x)*(x)
#define addImVec2(a, b) ImVec2((a).x + (b).x, (a).y + (b).y)

inline float lerp(float a, float b, float f)
{
	return (b - a) * f + a;
}

struct Raindrop {
	float x, y, z = -1.f;
	int height;
	float secondsToFall;
	ImU32 color;
};

bool RaindropsInitialized = false;
std::chrono::steady_clock::time_point RainLastMovedTime;

void GUI::Rain()
{
	/* get imgui window factors */
	ImVec2 WindowMin = ImGui::GetWindowPos();
	ImVec2 WindowMax = addImVec2(WindowMin, ImGui::GetWindowSize());
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	/* initalize raindrops */
	static Raindrop* Raindrops = (Raindrop*)malloc(sizeof(Raindrop) * Config::RaindropCount);
	if (!RaindropsInitialized)
	{
		for (unsigned int i = 0; i < Config::RaindropCount; i++)
		{
			Raindrops[i].x = randf();
			Raindrops[i].y = randf();
			Raindrops[i].z = square(randf());
			Raindrops[i].color = IM_COL32(255, 255, 255, 55 + 200 * Raindrops[i].z);
			Raindrops[i].height = (int)lerp(10, 15, Raindrops[i].z);
			Raindrops[i].secondsToFall = lerp(30, 10, Raindrops[i].z);
		}
		RaindropsInitialized = true;
	}

	/* timings */
	auto Now = std::chrono::high_resolution_clock::now();
	double SecondsSinceLastUpdate = ((double)std::chrono::duration_cast<std::chrono::microseconds>(Now - RainLastMovedTime).count()) / 1e6f;
	RainLastMovedTime = Now;

	/* draw rain */
	ImVec2 top, bottom;
	for (unsigned int i = 0; i < Config::RaindropCount; i++)
	{
		/* fall downwards */
		Raindrops[i].y += SecondsSinceLastUpdate / Raindrops[i].secondsToFall;
		if (Raindrops[i].y >= 1.f)
		{
			Raindrops[i].y = fmod(Raindrops[i].y, 1.f);
			Raindrops[i].x = randf();
		}

		/* calculate position in pixels */
		top.x = lerp(WindowMin.x, WindowMax.x, Raindrops[i].x);
		bottom.x = top.x + 1;

		top.y = lerp(WindowMin.y - Raindrops[i].height, WindowMax.y, Raindrops[i].y);
		bottom.y = top.y + Raindrops[i].height;

		/* add to draw queue */
		draw_list->AddRectFilled(top, bottom, Raindrops[i].color);
	}
}