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
	VMTManager gameeventmanagerVMT;

	EndScene oEndScene;
	Reset oReset;
	CreateMove oCreateMove;
	PaintTraverse oPaintTraverse;
	FrameStageNotify oFrameStageNotify;
	LockCursor oLockCursor;
	FireEventClientSide oFireEventClientSide;
	
	//GUI Vars
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
	WNDPROC oWndProc = NULL;

	std::vector < std::string> console;
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
	I::engine->ClientCmd_Unrestricted("echo Hooking");
	d3d9VMT.Initialise((DWORD*)D3d9Device);
	clientVMT.Initialise((DWORD*)I::client);
	clientmodeVMT.Initialise((DWORD*)I::clientmode);
	surfaceVMT.Initialise((DWORD*)I::surface);
	panelVMT.Initialise((DWORD*)I::panel);
	gameeventmanagerVMT.Initialise((DWORD*)I::gameeventmanager);

	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9VMT.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Endscene...Success!");

	std::cout << "Reset...";
	oReset = (Reset)d3d9VMT.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Reset...Success!");

	std::cout << "CreateMove...";
	oCreateMove = (CreateMove)clientmodeVMT.HookMethod((DWORD)&CreateMoveHook, 24);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo CreateMove...Success!");

	std::cout << "PaintTraverse...";
	oPaintTraverse = (PaintTraverse)panelVMT.HookMethod((DWORD)&PaintTraverseHook, 41);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo PaintTraverse...Success!");

	std::cout << "FrameStageNotify...";
	oFrameStageNotify = (FrameStageNotify)clientVMT.HookMethod((DWORD)&FrameStageNotifyHook, 37);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FrameStageNotify...Success!");

	std::cout << "LockCursor...";
	oLockCursor = (LockCursor)surfaceVMT.HookMethod((DWORD)&LockCursorHook, 67);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo LockCursor...Success!");

	std::cout << "FireEventClientSide...";
	oFireEventClientSide = (FireEventClientSide)gameeventmanagerVMT.HookMethod((DWORD)&FireEventClientSideHook, 9);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEventClientSide...Success!");
}

void H::UnHook()
{
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	gameeventmanagerVMT.RestoreOriginal();
	panelVMT.RestoreOriginal();
	surfaceVMT.RestoreOriginal();
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

	if (GUI::Main())
		G::KillDLL = true;

	//temp code
	if(GetAsyncKeyState(VK_F11))
		G::KillDLL = true;

	ImGui::Begin("console");
	for (auto a : console)
		ImGui::Text(a.c_str());
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
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT) {
			GUI::ShowMenu = !GUI::ShowMenu;
		}
	}
	if (D3dInit && GUI::ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool __stdcall H::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	if (!cmd->command_number)
		return true;

	if (I::engine->IsInGame() && cmd) {
		if (GUI::ShowMenu) {
			cmd->buttons = 0;
			cmd->upmove = 0;
		}

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pSendPacket;

		bSendPacket = I::engine->GetNetChannelInfo()->ChokedPackets >= 4;

		G::CM_Start(cmd, pSendPacket);

		if (fabsf(G::cmd->sidemove) < 5.0f) {
			G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
		}

		G::CM_MoveFixStart();

		//ez bhop
		if ((cmd->buttons & IN_JUMP) && (G::Localplayer->GetHealth() > 0) && !(G::Localplayer->GetFlags() & FL_ONGROUND)) {
			cmd->buttons &= ~IN_JUMP;
		}

		//if(cmd->buttons & IN_ATTACK)
			//aimbot->Legit();

		antiaim->rage();
		//antiaim->legit();

		backtrack->run();

		if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_USE)
		{
			G::cmd->viewangles = G::CM_StartAngle;
		}

		//aimbot->Rage();

		G::CM_End();
	}
	
	oCreateMove(I::clientmode, flInputSampleTime, cmd);
	return false; //silent aim on false (only for client)
}

void __stdcall H::PaintTraverseHook(int vguiID, bool force, bool allowForcing)
{
	oPaintTraverse(I::panel, vguiID, force, allowForcing);
	if (I::panel && strcmp(I::panel->GetName(vguiID), "MatSystemTopPanel") == 0) {
		Entity* Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
		if (!Localplayer || !I::engine->IsInGame() || !(Localplayer->GetHealth() > 0))
			return;
		
		esp->Run();
		
	}
}

void __stdcall H::FrameStageNotifyHook(int curStage)
{
	backtrack->update(curStage);
	return oFrameStageNotify(curStage);
}

void __stdcall H::LockCursorHook()
{
	if (GUI::ShowMenu) {
		return I::surface->UnlockCursor();
	}
	return oLockCursor(I::surface);
}

bool __stdcall H::FireEventClientSideHook(GameEvent* event)
{
	if (!(I::engine->IsInGame()))
		return oFireEventClientSide(I::gameeventmanager, event);
	if (!event)
		return oFireEventClientSide(I::gameeventmanager, event);

	switch (StrHash::HashRuntime(event->GetName())) {
		case StrHash::Hash("player_hurt"):
		{
			//also add hitmarkers here...
			const auto localIdx = I::engine->GetLocalPlayer();
			if (I::engine->GetPlayerForUserID(event->GetInt("attacker")) != localIdx || I::engine->GetPlayerForUserID(event->GetInt("userid")) == localIdx)
				break;
			I::engine->ClientCmd_Unrestricted("play buttons/arena_switch_press_02");
		}
		break;

		case StrHash::Hash("player_death"):
		{
			//also add player kill say here
			const auto localIdx = I::engine->GetLocalPlayer();
			if (I::engine->GetPlayerForUserID(event->GetInt("attacker")) != localIdx || I::engine->GetPlayerForUserID(event->GetInt("userid")) == localIdx)
				break;
			I::engine->ClientCmd_Unrestricted("play player/neck_snap_01");
		}
		break;
		case StrHash::Hash("weapon_fire"):
		{
			const auto localIdx = I::engine->GetLocalPlayer();

			player_info_t Info;
			int index = I::engine->GetPlayerForUserID(event->GetInt("userid"));
			if (index < 65 && I::engine->GetPlayerInfo(index, &Info)) 
			{
				if (!backtrack->Records[index].empty())
				{
					backtrack->Records[index].back().Shooting = true;
					backtrack->Records[index].front().Shooting = true;
				}
			}
		}
		break;
	}
	return oFireEventClientSide(I::gameeventmanager, event);
}

