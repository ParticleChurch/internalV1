#include "Include.hpp"

//for windprc hook
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace H
{
	VMTManager d3d9VMT;
	VMTManager clientVMT;
	VMTManager clientmodeVMT;
	VMTManager surfaceVMT;
	VMTManager panelVMT;

	EndScene oEndScene;
	Reset oReset;
	CreateMove oCreateMove;
	PaintTraverse oPaintTraverse;
	
	//GUI Vars
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
	WNDPROC oWndProc = NULL;
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
	d3d9VMT.Initialise((DWORD*)D3d9Device);
	clientVMT.Initialise((DWORD*)I::client);
	clientmodeVMT.Initialise((DWORD*)I::clientmode);
	surfaceVMT.Initialise((DWORD*)I::surface);
	panelVMT.Initialise((DWORD*)I::panel);

	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9VMT.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;

	std::cout << "Reset...";
	oReset = (Reset)d3d9VMT.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;

	std::cout << "CreateMove...";
	oCreateMove = (CreateMove)clientmodeVMT.HookMethod((DWORD)&CreateMoveHook, 24);
	std::cout << "Success!" << std::endl;

	std::cout << "PaintTraverse...";
	oPaintTraverse = (PaintTraverse)panelVMT.HookMethod((DWORD)&PaintTraverseHook, 41);
	std::cout << "Success!" << std::endl;
}

void H::UnHook()
{
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	panelVMT.RestoreOriginal();
	d3d9VMT.RestoreOriginal();
	clientmodeVMT.RestoreOriginal();
	clientVMT.RestoreOriginal();
	D3dInit = false; //for wndproc... haven't found better solution
	FreeConsole();
}

void H::Eject()
{
	H::UnHook();
	FreeConsole();
	FreeLibraryAndExitThread(G::DLLModule, 0);
}

long __stdcall H::EndSceneHook(IDirect3DDevice9* device)
{
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

	//if (GUI::Main())
		//G::KillDLL = true;

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

bool __stdcall H::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	//ez bhop
	Entity* localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	if ((cmd->buttons & IN_JUMP) && (localplayer->GetHealth() > 0) && !(localplayer->GetFlags() & FL_ONGROUND)) {
		cmd->buttons &= ~IN_JUMP;
	}
	oCreateMove(I::clientmode, flInputSampleTime, cmd);
	return true; //silent aim on false (only for client)
}

bool WorldToScreen(Vec& in, Vec& out)
{
	auto matrix = I::engine->WorldToScreenMatrix().c;

	float w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

	if (w > 0.001f) {
		static int width = -1;
		static int height = -1;
		I::surface->GetScreenSize(width, height);
		out.x = width / 2 * (1 + (matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3]) / w);
		out.y = height / 2 * (1 - (matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3]) / w);
		out.z = 0.0f;
		return true;
	}
	return false;
}

void __stdcall H::PaintTraverseHook(int vguiID, bool force, bool allowForcing)
{
	oPaintTraverse(I::panel, vguiID, force, allowForcing);
	if (I::panel && strcmp(I::panel->GetName(vguiID), "MatSystemTopPanel") == 0) {
		Entity* Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
		if (!Localplayer || !I::engine->IsInGame() || !(Localplayer->GetHealth() > 0))
			return;
		
		I::surface->DrawSetColor(Color(255.f, 255.f, 255.f, 255.f));
		for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
		{
			if (i == I::engine->GetLocalPlayer())
				continue;

			Entity* Ent = I::entitylist->GetClientEntity(i);
			if (!Ent)
				continue;

			player_info_t PlayerInfo;
			if (!I::engine->GetPlayerInfo(i, &PlayerInfo))
				continue;

			Vec Screen;
			Vec Loc = Ent->GetBonePos(8);

			
			if (WorldToScreen(Loc, Screen))
			{
				int xSize = 0;
				int ySize = 0;

				I::engine->GetScreenSize(xSize, ySize);
				I::surface->DrawLine(xSize/2, ySize/2, Screen.x, Screen.y);
			}
			
		}
		
	}
}

