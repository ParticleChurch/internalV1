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

bool GUI::Main()
{
	bool PressedEject = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX / 2.f, WindowSizeY / 2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		if (Config::UserInfo.Paid || Config::UserInfo.Developer)
			PressedEject = GUI::PaidHackMenu();
		else
			PressedEject = GUI::FreeHackMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		GUI::ShowMenu = true;
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Processing", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		// Eject button
		ImGui::SetCursorPos(ImVec2(10, 83));
		if (ImGui::Button("Eject", ImVec2(70, 20)))
			PressedEject = true;

		// Cancel button
		ImGui::SetCursorPos(ImVec2(360, 83));
		if (ImGui::Button("Cancel", ImVec2(70, 20)))
			Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;

		LoginWindowPosition = ImGui::GetWindowPos();
		ImGui::End();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_NONE)
	{
		GUI::ShowMenu = true;
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Login", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		// Login Form
		static char Email[257] = "";
		ImGui::SetCursorPos(ImVec2(59, 30));
		ImGui::Text("Email:"); ImGui::SameLine();
		ImGui::InputText("###Email", Email, 257);

		static char Password[65] = "";
		ImGui::SetCursorPos(ImVec2(38, 55));
		ImGui::Text("Password:"); ImGui::SameLine();
		ImGui::InputText("###Password", Password, 65);

		// Eject button
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

void Hotkey(int& key) noexcept //custom IMGUI shennanigans
{
	ImGui::SameLine();
	static bool Toggle = false;

	if (key)
	{
		if (ImGui::Button(std::string("[ " + std::string(I::inputsystem->VirtualKeyToString(key)) + " ]").c_str()))
			Toggle = true;
	}
	else
	{
		if (ImGui::Button("[ None ]"))
			Toggle = true;
	}

	if (Toggle)
	{
		DWORD menuKey = VK_INSERT;
		ImGui::SetTooltip("Press any key to change keybind");
		ImGuiIO& io = ImGui::GetIO();

		for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) {
			if (ImGui::IsKeyPressed(i) && i != menuKey) {
				key = i != VK_ESCAPE ? i : 0;
				Toggle = false;
			}
		}

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
			if (ImGui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != menuKey) {
				key = i + (i > 1 ? 2 : 1);
				Toggle = false;
			}
		}
	}
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
	ImGui::Checkbox("Enable###Enable", &Config::legitbot.Enable);
	ImGui::Separator();

	ImGui::Text("Aimbot");
	ImGui::Checkbox("Enable###EnableAim", &Config::legitbot.EnableAim);
	Hotkey(Config::legitbot.AimKey);
	ImGui::Checkbox("TeamCheck###TeamCheck", &Config::legitbot.TeamCheck);

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

	ImGui::SliderInt(("Fov###Fov" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].Fov, 0, 90);
	ImGui::SliderInt(("Yaw Speed###Yaw Speed" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].YawSpeed, 0, 100);
	ImGui::SliderInt(("Pitch Speed###Pitch Speed" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].PitchSpeed, 0, 100);
	ImGui::Checkbox(("Recoil Control###Recoil Control" + std::to_string(tab)).c_str(), &Config::legitbot.weapon[tab].RecoilControl);

	static const char* Hitboxes[] = { "Head", "Pelvis", "Stomach", "Chest", "Legs", "Feet" };
	
	static std::string preview = "";
	if (ImGui::BeginCombo(("Hitboxes##combo" + std::to_string(tab)).c_str(), preview.c_str())) // The second parameter is the label previewed before opening the combo.
	{
		Config::legitbot.weapon[tab].LHitboxes.clear();
		Config::legitbot.weapon[tab].LHitboxes.resize(0);
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
			}
		}
		ImGui::EndCombo();
	}

	if (Config::legitbot.weapon[tab].HSelected[0])
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_HEAD);
	if (Config::legitbot.weapon[tab].HSelected[1])
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_PELVIS);
	if (Config::legitbot.weapon[tab].HSelected[2])
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_STOMACH);
	if (Config::legitbot.weapon[tab].HSelected[3]) {
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LOWER_CHEST);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_CHEST);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_UPPER_CHEST);
	}
	if (Config::legitbot.weapon[tab].HSelected[4]) {
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_THIGH);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_THIGH);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_CALF);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_CALF);
	}
	if (Config::legitbot.weapon[tab].HSelected[5]) {
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_RIGHT_FOOT);
		Config::legitbot.weapon[tab].Hitboxes.push_back(HITBOX_LEFT_FOOT);
	}


	static int PriorityChoice = 0;
	ImGui::Combo(("Priority##combo" + std::to_string(tab)).c_str(), &PriorityChoice, Config::legitbot.weapon[tab].LHitboxes);
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

bool GUI::FreeHackMenu()
{
	bool PressedEject = false;

	if (!ShowMenu)			//if they arent displaying the menu... just return
		return PressedEject;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
	ImGui::Begin("Hack (Free Version)", 0, ImGuiWindowFlags_NoScrollbar);

	static bool DisplayLegit = true;
	if (DisplayLegit)
	{
		DisplayLegitTab();
	}

	ImGui::Separator();
	//ImGui::SetCursorPos(ImVec2(0, 30)); //leaving eject at end lol
	if (ImGui::Button("Eject", ImVec2(70, 20)))
		PressedEject = true;

	ImGui::End();
	return PressedEject;
}


