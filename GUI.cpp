#include "Include.hpp"

bool GUI::Main()
{
	bool KillGui = false;

	// LoginWindow
	ImGui::SetNextWindowSize(ImVec2(400 + 40, 200 + 20 + 40));
	ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	if (Config::UserInfo.IsAuthenticated) {
		// Unload button
		ImGui::SetCursorPos(ImVec2(20 + 150, 20 + 20));
		if (ImGui::Button("Unload", ImVec2(100, 40)))
			KillGui = true;

		// TODO: change config
		std::cout << "xd" << std::endl;
	}
	else {
		// Unload button
		ImGui::SetCursorPos(ImVec2(10, 220));
		if (ImGui::Button("Unload", ImVec2(70, 30)))
			KillGui = true;

		// Login button
		ImGui::SetCursorPos(ImVec2(360, 220));
		if (ImGui::Button("Login", ImVec2(70, 30)))
			KillGui = true;

		// Login Form
		ImGui::SetCursorPos(ImVec2(41, 30));
		ImGui::Text("Email:"); ImGui::SameLine();
		ImGui::InputText("", Config::UserInfo.Email, 256);

		static char Password[64] = "";
		ImGui::SetCursorPos(ImVec2(20, 55));
		ImGui::Text("Password:"); ImGui::SameLine();
		ImGui::InputText("", Password, 256);
	}

	ImGui::End();
	return KillGui;
}