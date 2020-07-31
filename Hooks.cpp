#include "Include.hpp"

//for windprc hook
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace H
{
	VMTManager d3d9;

	EndScene oEndScene;
	Reset oReset;
	
	//GUI Vars
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
	WNDPROC oWndProc = NULL;

	bool Dead = false;
}

void H::Init()
{
	PDWORD pD3d9Device = *(PDWORD*)(FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	IDirect3DDevice9* D3d9Device = (IDirect3DDevice9*)*pD3d9Device;

	while (CSGOWindow == NULL) {
		CSGOWindow = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		Sleep(10);
	}

	oWndProc = (WNDPROC)GetWindowLongPtr(CSGOWindow, GWL_WNDPROC);
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)WndProc);

	std::cout << "Hooking..." << std::endl;
	d3d9.Initialise((DWORD*)D3d9Device);

	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;

	std::cout << "Reset...";
	oReset = (Reset)d3d9.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;
}

void H::UnHook()
{
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	d3d9.RestoreOriginal();
	D3dInit = false; //for wndproc... haven't found better solution
	FreeConsole();
	H::Dead = true;
}

void H::Eject()
{
	// TODO: make this function truly eject
	// it currently just stops all effects of the DLL
	// so it *seems* like it ejected, however, the library is still loaded
	// it's just not doing anything
	H::UnHook();
	FreeConsole();


	// this is the line that will actually free the library
	FreeLibraryAndExitThread(G::DLLModule, 0);

	// but instead it crashes CSGO, i believe our problem is the same as shown here
	// https://stackoverflow.com/questions/39780590/freelibraryandexitthread-crashes-program-when-unloading-injected-dll
}

long __stdcall H::EndSceneHook(IDirect3DDevice9* device)
{
	//static auto oEndScene = EndScene.GetOriginal<decltype(&EndSceneHook)>(42);
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

	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::Begin("Test");

	if (ImGui::Button("Unload"))
	{
		H::Dead = true;
	}

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(device);
}

long __stdcall H::ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
	return oReset(device, pPresentationParameters);
}

LRESULT __stdcall H::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (D3dInit && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

