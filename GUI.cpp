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
	unsigned int AttemptID; // 4
	char Email[257];        // 261
	char Password[65];      // 326 bytes
};

struct LoginApiResponse
{
	char LoginFlags;
	unsigned int UserID; // little endian
};

void AttemptLogin(LoginInformation* Info)//LoginInformation* Info)
{
	HTTP::Arguments Data;
	Data.Add("Email", Info->Email);
	Data.Add("Password", Info->Password);
	
	std::string ServerResponse = HTTP::API("login", Data);

	// If there is a more recently started thread, then quit - we're useless now
	if (Config::UserInfo.AuthStatus != AUTH_STATUS_PROCESSING || Info->AttemptID < LoginAttemptIndex)
	{
		free(Info);
		return;
	}

	// validate response and collect data
	std::cout << "Server sent " << ServerResponse.length() << "Bytes" << std::endl;
	if (!ServerResponse._Starts_with("APISUCCESS\n"))
	{
		std::cout << "API bad response: " << std::endl;
		std::cout << ServerResponse << std::endl;
		Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;
		free(Info);
		return;
	};

	std::string DataString = ServerResponse.substr(ServerResponse.find_first_of("\n") + 1);
	int sep = DataString.find_first_of("\n") + 1;
	char LoginFlags = (char)std::stoi(DataString.substr(0, sep));
	int UserID = std::stoi(DataString.substr(sep));

	std::cout << "Flags begin" << std::endl;
	std::cout << LoginFlags << std::endl;
	std::cout << UserID << std::endl;
	std::cout << "Flags end" << std::endl;

	// handle response
	//std::cout << "uid:" << Response.UserID << std::endl;

	Config::UserInfo.AuthStatus = AUTH_STATUS_NONE;
	
	// deallocate resources
	free(Info);
	//free(&Response);
}

bool GUI::Main()
{
	bool KillGui = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX/2.f, WindowSizeY/2.f);

	if (Config::UserInfo.AuthStatus == AUTH_STATUS_COMPLETE)
	{
		ImGui::SetNextWindowSize(ImVec2(20 + 50, 200 + 20 + 40));
		ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		// Unload button
		ImGui::SetCursorPos(ImVec2(20 + 150, 20 + 20));
		if (ImGui::Button("Unload", ImVec2(100, 40)))
			KillGui = true;

		// TODO: change config
		std::cout << "authenticated" << std::endl;
		ImGui::End();
	}
	else if (Config::UserInfo.AuthStatus == AUTH_STATUS_PROCESSING)
	{
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Processing", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		// Eject button
		ImGui::SetCursorPos(ImVec2(10, 83));
		if (ImGui::Button("Eject", ImVec2(70, 20)))
			KillGui = true;

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

		// Unload button
		ImGui::SetCursorPos(ImVec2(10, 83));
		if (ImGui::Button("Eject", ImVec2(70, 20)))
			KillGui = true;

		// Login button
		ImGui::SetCursorPos(ImVec2(360, 83));
		if (ImGui::Button("Login", ImVec2(70, 20)))
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

	return KillGui;
}