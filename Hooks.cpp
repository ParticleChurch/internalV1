#include "Include.hpp"

namespace H
{
	HookManager EndScene;
	HookManager Reset;
	
	//GUI Vars
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
}

void H::Init()
{
	PDWORD pD3d9Device = *(PDWORD*)(FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	IDirect3DDevice9* D3d9Device = (IDirect3DDevice9*)*pD3d9Device;

	while (CSGOWindow == NULL) {
		CSGOWindow = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		Sleep(10);
	}

	std::cout << "Hooking.." << std::endl;
	EndScene.Setup(D3d9Device);
	std::cout << "Endscene...";
	EndScene.HookIndex(42, EndSceneHook);
	std::cout << "Success!" << std::endl;

	Reset.Setup(D3d9Device);
	std::cout << "Reset...";
	Reset.HookIndex(16, ResetHook);
	std::cout << "Success!" << std::endl;
}

long __stdcall H::EndSceneHook(IDirect3DDevice9* device)
{
	static auto oEndScene = EndScene.GetOriginal<decltype(&EndSceneHook)>(42);
	if (!D3dInit) {
		D3dInit = true;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplWin32_Init(CSGOWindow);
		ImGui_ImplDX9_Init(device);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test");
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(device);
}

long __stdcall H::ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	static auto oReset = Reset.GetOriginal<decltype(&ResetHook)>(16);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
	return oReset(device, pPresentationParameters);
}

