#include "Include.hpp"

bool GUI::Main()
{
	bool KillGui = false;

	// LoginWindow
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoResize);

	if (Config::UserInfo.IsAuthenticated) {
		// Unload button
		ImGui::SetCursorPos(ImVec2(150, 30));
		if (ImGui::Button("Unload", ImVec2(100, 40)))
			KillGui = true;

		// TODO: change config
		std::cout << "xd" << std::endl;
	}
	else {
		// Unload button
		ImGui::SetCursorPos(ImVec2(5, 200 - 27));
		if (ImGui::Button("Unload", ImVec2(80, 20)))
			KillGui = true;

		// Login Form
		ImGui::SetCursorPos(ImVec2(200 - 40, 30));
		ImGui::Text("Login");

		ImGui::InputText("", Config::UserInfo.Email, 256);
		std::cout << Config::UserInfo.Email << std::endl;
	}

	ImGui::End();
	return KillGui;
}