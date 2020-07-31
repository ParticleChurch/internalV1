#include "Include.hpp"

bool GUI::Main()
{
	bool KillGui = false;

	// LoginWindow
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("Hack", 0, ImGuiWindowFlags_NoResize);

	// Unload button
	ImGui::SetCursorPos(ImVec2(100, 30));
	if (ImGui::Button("Unload"))
		KillGui = true;

	static char EmailField[256] = "Username";
	ImGui::InputText("input text", EmailField, IM_ARRAYSIZE(EmailField));
	std::cout << EmailField << std::endl;

	ImGui::End();
	return KillGui;
}