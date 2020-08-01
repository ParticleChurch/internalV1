#include "Include.hpp"
#include "GUI/HTTP.hpp"

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

bool GUI::PaidHackMenu()
{
	bool PressedEject = false;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
	ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoScrollbar);

	ImGui::SetCursorPos(ImVec2(0, 30));
	if (ImGui::Button("Eject", ImVec2(70, 20)))
		PressedEject = true;

	ImGui::End();
	return PressedEject;
}

bool GUI::FreeHackMenu()
{
	bool PressedEject = false;

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
	ImGui::Begin("Hack (Free Version)", 0, ImGuiWindowFlags_NoScrollbar);

	ImGui::SetCursorPos(ImVec2(0, 30));
	if (ImGui::Button("Eject", ImVec2(70, 20)))
		PressedEject = true;

	ImGui::End();
	return PressedEject;
}

bool GUI::Main()
{
	bool PressedEject = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX/2.f, WindowSizeY/2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		if (Config::UserInfo.Paid || Config::UserInfo.Developer)
			PressedEject = GUI::PaidHackMenu();
		else
			PressedEject = GUI::FreeHackMenu();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Processing", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

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
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Login", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

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