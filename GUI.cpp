#include "Include.hpp"
#include "GUI/HTTP.hpp"

ImVec2 LoginWindowPosition(100, 100);
bool BeganProcessingLogin = false;

bool GUI::Main()
{
	bool KillGui = false;
	int WindowSizeX, WindowSizeY;
	I::engine->GetScreenSize(WindowSizeX, WindowSizeY);
	ImVec2 WindowCenter(WindowSizeX/2.f, WindowSizeY/2.f);

	if (Config::UserInfo.AuthenticationStatus == AUTHENTICATION_COMPLETE)
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
	else if (Config::UserInfo.AuthenticationStatus == AUTHENTICATION_PROCESSING)
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
			Config::UserInfo.AuthenticationStatus = AUTHENTICATION_NONE;

		LoginWindowPosition = ImGui::GetWindowPos();
		ImGui::End();

		if (!BeganProcessingLogin)
		{
			BeganProcessingLogin = true;
			HTTP::GET();
		}
	}
	else if (Config::UserInfo.AuthenticationStatus == AUTHENTICATION_NONE)
	{
		ImGui::SetNextWindowSize(ImVec2(440, 114));
		ImGui::SetNextWindowPos(WindowCenter, 0, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Login", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		// Login Form
		ImGui::SetCursorPos(ImVec2(59, 30));
		ImGui::Text("Email:"); ImGui::SameLine();
		ImGui::InputText("###Email", Config::UserInfo.Email, 256);

		static char Password[64] = "";
		ImGui::SetCursorPos(ImVec2(38, 55));
		ImGui::Text("Password:"); ImGui::SameLine();
		ImGui::InputText("###Password", Password, 64);

		// Unload button
		ImGui::SetCursorPos(ImVec2(10, 83));
		if (ImGui::Button("Eject", ImVec2(70, 20)))
			KillGui = true;

		// Login button
		ImGui::SetCursorPos(ImVec2(360, 83));
		if (ImGui::Button("Login", ImVec2(70, 20)))
		{
			Config::UserInfo.AuthenticationStatus = AUTHENTICATION_PROCESSING;
			BeganProcessingLogin = false;
		}

		LoginWindowPosition = ImGui::GetWindowPos();
		ImGui::End();
	}

	return KillGui;
}