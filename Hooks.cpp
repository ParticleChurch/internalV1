#include "Include.hpp"

//for windprc hook
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class EventListener : public GameEventListener
{
public:
	EventListener()
	{
		I::gameeventmanager->AddListener(this, "bullet_impact");	// Resolver
		I::gameeventmanager->AddListener(this, "player_hurt");		// Resolver/Hitmarker
		I::gameeventmanager->AddListener(this, "weapon_fire");		// Resolver
		I::gameeventmanager->AddListener(this, "player_death");		// Hitmarker
	}
	~EventListener()
	{
		I::gameeventmanager->RemoveListener(this);
	}

	virtual void FireGameEvent(GameEvent* event)
	{
		switch (StrHash::HashRuntime(event->GetName())) {
		case StrHash::Hash("player_hurt"):
		{
			/*
			player_hurt
			Name:	player_hurt
			Structure:
			short	userid	user ID of who was hurt
			short	attacker	user ID of who attacked
			byte	health	remaining health points
			byte	armor	remaining armor points
			string	weapon	weapon name attacker used, if not the world
			short	dmg_health	damage done to health
			byte	dmg_armor	damage done to armor
			byte	hitgroup	hitgroup that was damaged
			*/

			//also add hitmarkers here...
			const auto localIdx = I::engine->GetLocalPlayer();
			int attacker = I::engine->GetPlayerForUserID(event->GetInt("attacker"));
			int userid = I::engine->GetPlayerForUserID(event->GetInt("userid"));
			int HitGroup = event->GetInt("hitgroup");


			if (attacker == localIdx && userid != localIdx)
				I::engine->ClientCmd_Unrestricted("play buttons/arena_switch_press_02");

			if (userid == localIdx && HitGroup == 1) { //if hitting head
				bool orig = Config::GetState("antiaim-legit-invert");
				Config::SetBool("antiaim-legit-invert", !orig);
				orig = Config::GetState("antiaim-rage-invert");
				Config::SetBool("antiaim-rage-invert", !orig);
			}
		}
		break;
		case StrHash::Hash("player_death"):
		{
			/*
			player_death
			Note: When a client dies

			Name:	player_death
			Structure:
			short	userid	user ID who died
			short	attacker	user ID who killed
			short	assister	user ID who assisted in the kill
			bool	assistedflash	assister helped with a flash
			string	weapon	weapon name killer used
			string	weapon_itemid	inventory item id of weapon killer used
			string	weapon_fauxitemid	faux item id of weapon killer used
			string	weapon_originalowner_xuid
			bool	headshot	singals a headshot
			short	dominated	did killer dominate victim with this kill
			short	revenge	did killer get revenge on victim with this kill
			short	wipe	To do: check if indicates on a squad wipeout in Danger Zone
			short	penetrated	number of objects shot penetrated before killing target
			bool	noreplay	if replay data is unavailable, this will be present and set to false
			bool	noscope	kill happened without a scope, used for death notice icon
			bool	thrusmoke	hitscan weapon went through smoke grenade
			bool	attackerblind	attacker was blind from flashbang
			*/

			const auto localIdx = I::engine->GetLocalPlayer();
			if (I::engine->GetPlayerForUserID(event->GetInt("attacker")) != localIdx || I::engine->GetPlayerForUserID(event->GetInt("userid")) == localIdx)
				break;
			I::engine->ClientCmd_Unrestricted("play player/neck_snap_01");
		}
		break;
		}
		killsay->run(event);
		resolver->LogShots(event);
		esp->Run_GameEvent(event);
	}
};

namespace H
{
	VMTManager d3d9VMT;
	VMTManager clientVMT;
	VMTManager clientmodeVMT;
	VMTManager surfaceVMT;
	VMTManager panelVMT;
	VMTManager gameeventmanagerVMT;
	VMTManager inputVMT;
	VMTManager modelrenderVMT;
	VMTManager soundVMT;

	EndScene oEndScene;
	Reset oReset;
	CreateMove oCreateMove;
	PaintTraverse oPaintTraverse;
	FrameStageNotify oFrameStageNotify;
	LockCursor oLockCursor;
	hkCamToFirstPeron ohkCamToFirstPeron;
	DoPostScreenEffects oDoPostScreenEffects;
	DrawModelExecute oDrawModelExecute;
	EmitSound oEmitSound;

	//GUI Vars
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
	WNDPROC oWndProc = NULL;

	//TEMP
	std::vector < std::string> console;
	bool ThirdPersonToggle = false;

	EventListener* g_EventListener;
}


void H::Init()
{
	PDWORD pD3d9Device = *(PDWORD*)(G::pD3d9DevicePattern + 1);
	IDirect3DDevice9* D3d9Device = (IDirect3DDevice9*)*pD3d9Device;

	// TODO: this is definetly not the best way to do this, lmao
	while (!(CSGOWindow = FindWindowA(NULL, "Counter-Strike: Global Offensive"))) {
		Sleep(10);
	}
	GUI2::LoadProgress = 0.2f;

	oWndProc = (WNDPROC)GetWindowLongPtr(CSGOWindow, GWL_WNDPROC);
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)WndProc);

	std::cout << "\nHooking..." << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Hooking");
	d3d9VMT.Initialise((DWORD*)D3d9Device);
	clientVMT.Initialise((DWORD*)I::client);
	clientmodeVMT.Initialise((DWORD*)I::clientmode);
	surfaceVMT.Initialise((DWORD*)I::surface);
	panelVMT.Initialise((DWORD*)I::panel);
	gameeventmanagerVMT.Initialise((DWORD*)I::gameeventmanager);
	inputVMT.Initialise((DWORD*)I::input);
	modelrenderVMT.Initialise((DWORD*)I::modelrender);
	soundVMT.Initialise((DWORD*)I::sound);
	GUI2::LoadProgress = 0.25f;

	
	H::g_EventListener = new EventListener();

	static int SleepTime = 100;

	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9VMT.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Endscene...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.3f;

	std::cout << "Reset...";
	oReset = (Reset)d3d9VMT.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Reset...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.35f;

	std::cout << "CreateMove...";
	oCreateMove = (CreateMove)clientmodeVMT.HookMethod((DWORD)&CreateMoveHook, 24);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo CreateMove...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.4f;

	std::cout << "PaintTraverse...";
	oPaintTraverse = (PaintTraverse)panelVMT.HookMethod((DWORD)&PaintTraverseHook, 41);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo PaintTraverse...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.45f;

	std::cout << "FrameStageNotify...";
	oFrameStageNotify = (FrameStageNotify)clientVMT.HookMethod((DWORD)&FrameStageNotifyHook, 37);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FrameStageNotify...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.5f;

	std::cout << "LockCursor...";
	oLockCursor = (LockCursor)surfaceVMT.HookMethod((DWORD)&LockCursorHook, 67);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo LockCursor...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.55f;

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.6f;

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.65f;

	std::cout << "hkCamToFirstPeronVMT...";
	ohkCamToFirstPeron = (hkCamToFirstPeron)inputVMT.HookMethod((DWORD)&hkCamToFirstPeronHook, 36);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo hkCamToFirstPeronVMT...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.7f;

	std::cout << "DoPostScreenEffects...";
	oDoPostScreenEffects = (DoPostScreenEffects)clientmodeVMT.HookMethod((DWORD)&DoPostScreenEffectsHook, 44);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DoPostScreenEffects...Success!");

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.75f;

	std::cout << "DrawModelExecute...";
	oDrawModelExecute = (DrawModelExecute)modelrenderVMT.HookMethod((DWORD)&DrawModelExecuteHook, 21);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DrawModelExecute...Success!");	

	Sleep(SleepTime);
	GUI2::LoadProgress = 0.8f;

	std::cout << "EmitSound...";
	oEmitSound = (EmitSound)soundVMT.HookMethod((DWORD)&EmitSoundHook, 5);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo EmitSound...Success!");

	GUI2::LoadProgress = 0.85f;
}

void H::UnHook()
{
	I::inputsystem->EnableInput(true);


	//std::cout << "WndProc...";
	D3dInit = false; //for wndproc... haven't found better solution
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	//std::cout << "Success!" << std::endl;

	//std::cout << "modelrenderVMT...";
	modelrenderVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "inputVMT...";
	inputVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "gameeventmanagerVMT...";
	gameeventmanagerVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "surfaceVMT...";
	surfaceVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "panelVMT...";
	panelVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "d3d9VMT...";
	d3d9VMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "clientmodeVMT...";
	if(G::pSendPacket) //make sure it isnt already a nullptr
		*G::pSendPacket = true;
	clientmodeVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "clientVMT...";
	clientVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	//std::cout << "soundVMT...";
	soundVMT.RestoreOriginal();
	//std::cout << "Success!" << std::endl;

	delete g_EventListener;
	delete aimbot;
	delete backtrack;
	delete movement;
	delete antiaim;
	delete autowall;
}

void H::Eject()
{
	H::UnHook();
}

long __stdcall H::EndSceneHook(IDirect3DDevice9* device)
{
	if (!D3dInit) {
		D3dInit = true;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		GUI::LoadFonts(io);

		ImGui_ImplWin32_Init(CSGOWindow);
		ImGui_ImplDX9_Init(device);
	}
	

	//DONT ALLOW GAME CAPTURE, i think lol
	static uintptr_t gameoverlay_return_address = 0;

	if (!gameoverlay_return_address) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

		char mod[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

		if (strstr(mod, "gameoverlay"))
			gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
	}

	if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress()))
		return oEndScene(device);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	/*
	if (GUI::Main())
		G::KillDLL = true;

	
	//*/
	//*
	if (Config::GetBool("show-console"))
	{
		ImGui::Begin("console");
		for (auto a : console)
			ImGui::Text(a.c_str());
		ImGui::Text("Ooooh Secret setting. Experiment XD");
		ImGui::SliderFloat("###accuracy", &aimbot->accuracy_amount, 0, 100);

		/*ImGui::Text("Edge Amount");
		ImGui::SliderFloat("###headedge", &antiaim->HEADEDGE, 0, 100);*/
		if (ImGui::Button("Reset Resolver"))
		{
			for (int i = 0; i < 64; i++)
				resolver->ShotsMissed[i] = rand() % 20;
		}
		ImGui::End();
	}
	skinchanger->Menu();
	GUI2::Main();
	//*/
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	if (GUI2::Ejected)
	{
		G::KillDLL = true;
		H::Eject();
		Config2::Free();
	}

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
	bool IsKeyboardInput = uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_CHAR;

	// keydowns
	int KeyDown = -1;
	if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
		KeyDown = wParam;
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
		KeyDown = VK_LBUTTON;
	else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK)
		KeyDown = VK_RBUTTON;
	else if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK)
		KeyDown = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
	else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK)
		KeyDown = VK_MBUTTON;

	// keyups
	int KeyUp = -1;
	if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
		KeyUp = wParam;
	else if (uMsg == WM_LBUTTONUP)
		KeyUp = VK_LBUTTON;
	else if (uMsg == WM_RBUTTONUP)
		KeyUp = VK_RBUTTON;
	else if (uMsg == WM_XBUTTONUP)
		KeyUp = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
	else if (uMsg == WM_MBUTTONUP)
		KeyUp = VK_MBUTTON;

	/*
		check keybinds
	*/
	// TODO: check if they are:
	//     - typing in the menu
	//     - typing in csgo chat
	//     - typing in steam overlay
	if (GUI::CurrentlyChoosingKeybindFor)
	{
		if (KeyDown > 0 && KeyDown != VK_LBUTTON)
		{
			Config::Bind(GUI::CurrentlyChoosingKeybindFor, KeyDown);
			GUI::CurrentlyChoosingKeybindFor = nullptr;
		}
		else if (KeyUp == VK_LBUTTON && !GUI::IgnoreLButton)
		{
			Config::Bind(GUI::CurrentlyChoosingKeybindFor, VK_LBUTTON);
			GUI::CurrentlyChoosingKeybindFor = nullptr;
		}
	}
	else if (KeyDown > 0 && !(KeyDown == VK_LBUTTON && GUI::IgnoreLButton))
		Config::KeyPressed(KeyDown);
	else if (KeyUp > 0 && !(KeyUp == VK_LBUTTON && GUI::IgnoreLButton))
		Config::KeyReleased(KeyUp);

	// give imgui input
	if (D3dInit && Config::GetBool("show-menu") && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	I::inputsystem->EnableInput(!Config::GetBool("show-menu") || IsKeyboardInput && (!ImGui::GetIO().WantCaptureKeyboard));
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

float RandomVal(float min, float max)
{
	int Delta = max - min;
	float val = rand() % Delta;
	return min + val;
}

bool __stdcall H::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	if (!cmd->command_number)
		return true;

	if (I::engine->IsInGame() && cmd && G::LocalPlayer)
	{
		
		float ServerTime = I::globalvars->ServerTime(cmd);

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);

		// Fake lag Calculations
		fakelag->Start();
		
		G::CM_Start(cmd, pSendPacket);

		// Movement
		movement->BunnyHop();	
		movement->SlowWalk();
		movement->FastCrouch();
		movement->AAMoveFix();
		movement->FakeDuck();
		movement->LegitAutoStrafe();
		movement->LegSlide();

		// nade visuals
		miscvisuals->GrenadePrediction();
	
		G::CM_MoveFixStart();

		// Fake Lag
		*G::pSendPacket = fakelag->End();

		// AA
		antiaim->legit();
		antiaim->rage();

		


		// Clantag
		clantag->run();
		miscvisuals->RankRevealer();

		// bad use (E) and attack (LBUTTON)
		if (G::cmd->buttons & IN_USE)
		{
			*G::pSendPacket = true;
			G::cmd->viewangles = G::StartAngle;
		}
		else if ((G::cmd->buttons & IN_ATTACK) && G::LocalPlayer->CanShoot())
		{
			*G::pSendPacket = true;
			G::cmd->viewangles = G::StartAngle;
		}
			

		aimbot->Run();
		/*aimbot->Legit();*/

		// decide when to enable desync
		// we need to implement in movefixend or something lol

		/*
		bool desync = true;
		if (G::Localplayer->GetHealth() > 0)
		{
			if (cmd->buttons & IN_USE)
				desync = false;
			else
			{
				auto ActiveWeapon = G::Localplayer->GetActiveWeapon();
				auto WeaponData = ActiveWeapon->GetWeaponData();
				bool UsingKnifeOrGun = WeaponData->IsGun() || WeaponData->Type == WeaponType::Knife;
				if (UsingKnifeOrGun)
				{
					bool CanAttack = (G::Localplayer->GetNextAttack() - ServerTime) <= 0;
					bool CanPrimary = CanAttack && ((ActiveWeapon->GetNextPrimaryAttack() - ServerTime) <= 0) && (G::Localplayer->GetAmmo() > 0 || (WeaponData->Type == WeaponType::Knife));
					bool CanSecondary = CanAttack && ((ActiveWeapon->GetNextSecondaryAttack() - ServerTime) <= 0);

					if (!CanPrimary)
						cmd->buttons &= ~IN_ATTACK;

					bool PrimaryAttack = CanPrimary && cmd->buttons & IN_ATTACK;
					bool SecondaryAttack = CanSecondary && cmd->buttons & IN_ATTACK2 && WeaponData->Type == WeaponType::Knife;

					// disable desync for this tick
					if (PrimaryAttack || SecondaryAttack)
						G::cmd->viewangles = G::CM_StartAngle;
				}
				else if (ActiveWeapon->GetGrenadeThrowTime() > 0)
				{
					// we are currently releasing a grenade, do not desync or the nade path will be fucked
					G::cmd->viewangles = G::CM_StartAngle;
				}
			}
		}

		if (!desync)
			G::cmd->viewangles = G::CM_StartAngle;	

		if (GetAsyncKeyState(VK_LMENU))
			aimbot->Rage();

		
		*/
		backtrack->run();
		G::CM_MoveFixEnd();

		movement->RageAutoStrafe();

		// bad animation fix (for third person)
		if ((G::cmd->buttons & IN_ATTACK) || (G::cmd->buttons & IN_USE) || 
			(!Config::GetBool("antiaim-legit-enable") && !Config::GetBool("antiaim-rage-enable")))
		{
			antiaim->real = G::cmd->viewangles;
			antiaim->fake = G::cmd->viewangles;

			antiaim->real.NormalizeAngle();
			antiaim->fake.NormalizeAngle();
		}
		

		G::CM_End();	

		//movement->RageAutoStrafe();
	}

	oCreateMove(I::clientmode, flInputSampleTime, cmd);
	return false; //silent aim on false (only for client)
}

namespace H{
	
	std::vector<BulletTracer> traces;
}

void __stdcall H::PaintTraverseHook(int vguiID, bool force, bool allowForcing)
{
	if (strcmp("HudZoom", I::panel->GetName(vguiID)) == 0 && Config::GetBool("visuals-misc-noscope"))
		return;

	

	oPaintTraverse(I::panel, vguiID, force, allowForcing);
	if (I::panel && strcmp(I::panel->GetName(vguiID), "MatSystemTopPanel") == 0) {
		if (!G::LocalPlayer || !I::engine->IsInGame())
			return;

		miscvisuals->NoScope();
		esp->Run_PaintTraverse();
	}
}

void __stdcall H::FrameStageNotifyHook(int curStage)
{
	resolver->Resolve(curStage);

	if (curStage == FRAME_NET_UPDATE_END)
	{
		// PLAYER ANIM FIX FOR LOCAL PLAYER ONLY
		static ConVar* r_jiggle_bones = I::cvar->FindVar("r_jiggle_bones");
		r_jiggle_bones->onChangeCallbacks.size = 0;
		if (r_jiggle_bones->GetInt() > 0)
			r_jiggle_bones->SetValue(0);

	}
	if (curStage == FRAME_RENDER_START)
	{
		for (int i = 0; i < traces.size(); i++)
		{
			auto cur = traces[i];
			//draw a line from local player's head position to the hit point
			I::debugoverlay->AddLineOverlay(cur.src, cur.end, 125, 125, 1, true, -1);
			//draw a box at the hit point
			I::debugoverlay->AddBoxOverlay(cur.end, Vec(-2, -2, -2), Vec(2, 2, 2), Vec(0, 0, 0), 255, 0, 0, 127, -1.f);

			//if the item is older than 5 seconds, delete it
			if (fabs(I::globalvars->m_curTime - cur.SimTime) > 5.f)
				traces.erase(traces.begin() + i);
		}
		while (traces.size() > 8)
			traces.erase(traces.begin());
	}
		
	
	if (curStage == FRAME_RENDER_START && I::engine->IsInGame())
	{
		G::LocalPlayerIndex = I::engine->GetLocalPlayer();
		G::LocalPlayer = I::entitylist->GetClientEntity(G::LocalPlayerIndex);

		if (!G::LocalPlayer)
		{
			G::LocalPlayerAlive = false;
			return oFrameStageNotify(curStage);
		}

		G::LocalPlayerAlive = G::LocalPlayer->GetHealth() > 0;

		if (!G::LocalPlayerAlive)
			return oFrameStageNotify(curStage);

		G::LocalPlayerTeam = G::LocalPlayer->GetTeam();
		G::LocalPlayerWeapon = G::LocalPlayer->GetActiveWeapon();
		if (G::LocalPlayerWeapon)
			G::LocalPlayerWeaponData = G::LocalPlayerWeapon->GetWeaponData();

		G::UpdateEntities();

		//this is for accurate angles (aa, etc)
		static DWORD offset = N::GetOffset("DT_CSPlayer", "deadflag");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "deadflag");

		if (I::input->m_fCameraInThirdPerson)
			*(Vec*)((DWORD)G::LocalPlayer + offset + 4) = antiaim->real;

			

			//*G::Localplayer->pGetFlashMaxAlpha() = 0;
		

		for (int i = 1; i <= I::engine->GetMaxClients(); i++) {
			Entity* entity = I::entitylist->GetClientEntity(i);
			if (!entity || i == G::LocalPlayerIndex || entity->IsDormant() || !(entity->GetHealth() > 0)) continue;
			*reinterpret_cast<int*>(entity + 0xA28) = 1;							//visible???
			*reinterpret_cast<int*>(entity + 0xA30) = I::globalvars->m_frameCount; //sim time?
		}
	}

	miscvisuals->NoFlash(curStage);
	miscvisuals->NoSmoke_FrameStageNotify();

	backtrack->update(curStage);
	world->Run(curStage);

	skinchanger->run(curStage);

	return oFrameStageNotify(curStage);
}

void __stdcall H::LockCursorHook()
{
	if (Config::GetBool("show-menu")) {
		return I::surface->UnlockCursor();
	}
	return oLockCursor(I::surface);
}

void __fastcall H::hkCamToFirstPeronHook()
{
	miscvisuals->ThirdPerson_hkCamToFirstPeron();
}

void __stdcall H::DoPostScreenEffectsHook(int param)
{
	miscvisuals->ThirdPerson_DoPostScreenEffects();
	miscvisuals->NoSmoke_DoPostScreenEffects();

	return oDoPostScreenEffects(I::clientmode, param);
}

void __fastcall H::DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	/*return H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);*/
	chams->Init();
	chams->Run(thisptr, edx, ctx, state, info, customBoneToWorld);
	// 
}

void __stdcall H::EmitSoundHook(SoundData data)
{
	static std::add_pointer_t<bool __stdcall(const char*)> acceptMatch = reinterpret_cast<decltype(acceptMatch)>(G::AcceptMatchPattern);
	if (!strcmp(data.soundEntry, "UIPanorama.popup_accept_match_beep") && Config::GetBool("misc-other-autoaccept"))
	{
		acceptMatch("accept");
		/*auto window = FindWindowW(L"Valve001", NULL);
		FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
		FlashWindowEx(&flash);
		ShowWindow(window, SW_RESTORE);*/

	//	//Comment multiple lines of code: [ctrl] + [shift] + [/]
	}
	return oEmitSound(I::sound, data);
}
