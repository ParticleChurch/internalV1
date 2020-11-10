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
	VMTManager inputVMT;
	VMTManager modelrenderVMT;

	VMTManager soundVMT;

	EndScene oEndScene;
	Reset oReset;
	CreateMove oCreateMove;
	PaintTraverse oPaintTraverse;
	FrameStageNotify oFrameStageNotify;
	LockCursor oLockCursor;
	FireEventClientSide oFireEventClientSide;
	FireEvent oFireEvent;
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


	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9VMT.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Endscene...Success!");

	//Sleep(200);

	std::cout << "Reset...";
	oReset = (Reset)d3d9VMT.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Reset...Success!");

	//Sleep(200);

	std::cout << "CreateMove...";
	oCreateMove = (CreateMove)clientmodeVMT.HookMethod((DWORD)&CreateMoveHook, 24);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo CreateMove...Success!");

	//Sleep(200);

	std::cout << "PaintTraverse...";
	oPaintTraverse = (PaintTraverse)panelVMT.HookMethod((DWORD)&PaintTraverseHook, 41);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo PaintTraverse...Success!");

	//Sleep(200);

	std::cout << "FrameStageNotify...";
	backtrack->Init();
	oFrameStageNotify = (FrameStageNotify)clientVMT.HookMethod((DWORD)&FrameStageNotifyHook, 37);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FrameStageNotify...Success!");

	//Sleep(200);

	std::cout << "LockCursor...";
	oLockCursor = (LockCursor)surfaceVMT.HookMethod((DWORD)&LockCursorHook, 67);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo LockCursor...Success!");

	//Sleep(200);

	std::cout << "FireEventClientSide...";
	oFireEventClientSide = (FireEventClientSide)gameeventmanagerVMT.HookMethod((DWORD)&FireEventClientSideHook, 9);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEventClientSide...Success!");

	//Sleep(200);

	std::cout << "FireEvent...";
	oFireEvent = (FireEvent)gameeventmanagerVMT.HookMethod((DWORD)&FireEventHook, 8);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEvent...Success!");

	//Sleep(200);

	std::cout << "hkCamToFirstPeronVMT...";
	ohkCamToFirstPeron = (hkCamToFirstPeron)inputVMT.HookMethod((DWORD)&hkCamToFirstPeronHook, 36);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo hkCamToFirstPeronVMT...Success!");

	//Sleep(200);

	std::cout << "DoPostScreenEffects...";
	oDoPostScreenEffects = (DoPostScreenEffects)clientmodeVMT.HookMethod((DWORD)&DoPostScreenEffectsHook, 44);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DoPostScreenEffects...Success!");

	//Sleep(200);

	std::cout << "DrawModelExecute...";
	oDrawModelExecute = (DrawModelExecute)modelrenderVMT.HookMethod((DWORD)&DrawModelExecuteHook, 21);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DrawModelExecute...Success!");	

	//Sleep(200);

	std::cout << "EmitSound...";
	oEmitSound = (EmitSound)soundVMT.HookMethod((DWORD)&EmitSoundHook, 5);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo EmitSound...Success!");
}

void H::UnHook()
{
	I::inputsystem->EnableInput(true);

	std::cout << "WndProc...";
	D3dInit = false; //for wndproc... haven't found better solution
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	std::cout << "Success!" << std::endl;

	std::cout << "modelrenderVMT...";
	modelrenderVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "inputVMT...";
	inputVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "gameeventmanagerVMT...";
	gameeventmanagerVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "surfaceVMT...";
	surfaceVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "panelVMT...";
	panelVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "d3d9VMT...";
	d3d9VMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "clientmodeVMT...";
	if(G::pSendPacket) //make sure it isnt already a nullptr
		*G::pSendPacket = true;
	clientmodeVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "clientVMT...";
	clientVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;

	std::cout << "soundVMT...";
	soundVMT.RestoreOriginal();
	std::cout << "Success!" << std::endl;
	
	std::cout << "Freeing Console...Ejected!";
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

		GUI::LoadFonts(io);

		ImGui_ImplWin32_Init(CSGOWindow);
		ImGui_ImplDX9_Init(device);
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GUI::Main())
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
	if (D3dInit && Config::GetBool("config-show-menu") && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	I::inputsystem->EnableInput(IsKeyboardInput || !Config::GetBool("config-show-menu"));
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool __stdcall H::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	if (!cmd->command_number)
		return true;

	if (I::engine->IsInGame() && cmd && G::Localplayer) 
	{
		
		float ServerTime = I::globalvars->ServerTime(cmd);

		if (Config::GetBool("config-show-menu")) {
			cmd->buttons = 0;
			cmd->upmove = 0;
			cmd->weaponselect = 0;
			return false;
		}

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pSendPacket;

		

		bSendPacket = I::engine->GetNetChannelInfo()->ChokedPackets >= G::ChokeAmount;
		
		G::CM_Start(cmd, pSendPacket);

		//Movement
		movement->SlowWalk();
		movement->AAMoveFix();
		movement->BunnyHop();
		movement->FastCrouch();
	
		G::CM_MoveFixStart();

		
		antiaim->rage();

		// decide when to enable desync
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

		backtrack->run();
		
		G::CM_MoveFixEnd();

		movement->RageAutoStrafe();
		movement->LegitAutoStrafe();

		G::CM_End();	
		
	}

	oCreateMove(I::clientmode, flInputSampleTime, cmd);
	return false; //silent aim on false (only for client)
}

void __stdcall H::PaintTraverseHook(int vguiID, bool force, bool allowForcing)
{
	oPaintTraverse(I::panel, vguiID, force, allowForcing);
	if (I::panel && strcmp(I::panel->GetName(vguiID), "MatSystemTopPanel") == 0) {
		if (!G::Localplayer || !I::engine->IsInGame() || !(G::Localplayer->GetHealth() > 0))
			return;

		esp->Run();
	}
}

void __stdcall H::FrameStageNotifyHook(int curStage)
{
	if (curStage == FRAME_RENDER_START)
	{
		if (I::engine->IsInGame()) {
			G::Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());

			if (!G::Localplayer)
				return oFrameStageNotify(curStage);;

			if (!(G::Localplayer->GetHealth() > 0))
				return oFrameStageNotify(curStage);;

			//this is for accurate angles (aa, etc)
			static DWORD offset = N::GetOffset("DT_CSPlayer", "deadflag");
			if (offset == 0)
				offset = N::GetOffset("DT_CSPlayer", "deadflag");

			//if (I::input->m_fCameraInThirdPerson)
			//	*(Vec*)((DWORD)G::Localplayer + offset + 4) = G::FakeAngle;

			backtrack->update();

			//*G::Localplayer->pGetFlashMaxAlpha() = 0;

			int LPIndex = I::engine->GetLocalPlayer();
			for (int i = 1; i <= I::engine->GetMaxClients(); i++) {
				Entity* entity = I::entitylist->GetClientEntity(i);
				if (!entity || i == LPIndex || entity->IsDormant() || !(entity->GetHealth() > 0)) continue;
				*reinterpret_cast<int*>(entity + 0xA28) = 1;							//visible???
				*reinterpret_cast<int*>(entity + 0xA30) = I::globalvars->m_frameCount; //sim time?
			}
		}
	}


	if (curStage == FRAME_RENDER_START && G::Localplayer && G::Localplayer->GetHealth() > 0 && I::engine->IsInGame()) {
		static auto load_named_sky = reinterpret_cast<void(__fastcall*)(const char*)>(FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	
		static auto sv_skyname = I::cvar->FindVar("sv_skyname");
		sv_skyname->onChangeCallbacks.size = 0;

		static auto r_3dsky = I::cvar->FindVar("r_3dsky");
		r_3dsky->onChangeCallbacks.size = 0;
		r_3dsky->SetValue(0);

		load_named_sky("sky_csgo_night02");

		for (auto i = I::materialsystem->FirstMaterial(); i != I::materialsystem->InvalidMaterial(); i = I::materialsystem->NextMaterial(i))
		{
			auto mat = I::materialsystem->GetMaterial(i);
			if (!mat)
				continue;

			auto tex_name = mat->GetTextureGroupName();
			
			if (strstr(tex_name, "World") || strstr(tex_name, "StaticProp") || strstr(tex_name, "SkyBox"))
			{
				// TODO: bruh this literally just makes my screen black
				mat->ColorModulate(20 / 255.f, 20 / 255.f, 30 / 255.f);
				// I::modelrender->ForcedMaterialOverride(mat);
			}
		}
	}
	
	return oFrameStageNotify(curStage);
}

void __stdcall H::LockCursorHook()
{
	if (Config::GetBool("config-show-menu")) {
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
			antiaim->Left = !antiaim->Left;
			antiaim->side *= -1;
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
		//I::engine->ClientCmd_Unrestricted("say ok"); //kill say
	}
	break;	
	}
	return oFireEventClientSide(I::gameeventmanager, event);
}

bool __stdcall H::FireEventHook(GameEvent* event, bool bDontBroadcast) //THIS WORKS HAHAHAHA
{
	if (!(I::engine->IsInGame()))
		return oFireEvent(I::gameeventmanager, event, bDontBroadcast);
	if (!event)
		return oFireEvent(I::gameeventmanager, event, bDontBroadcast);

	switch (StrHash::HashRuntime(event->GetName())) {
		/*
		bullet_impact
		Note: Every time a bullet hits something

		Name:	bullet_impact
		Structure:	
		short	userid	
		float	x	
		float	y	
		float	z	
		*/
		case StrHash::Hash("bullet_impact"):
		{
			Vec			HitLocation		= Vec(event->GetInt("x"), event->GetInt("y"), event->GetInt("z"));
			const int	LocalIndex		= I::engine->GetLocalPlayer();
			const int	UserIndex		= I::engine->GetPlayerForUserID(event->GetInt("userid"));
			Entity*		Entity			= I::entitylist->GetClientEntity(UserIndex);
				
		}
		break;
	}
	return oFireEvent(I::gameeventmanager, event, bDontBroadcast);
}

void __fastcall H::hkCamToFirstPeronHook()
{
	ohkCamToFirstPeron(I::input);
}

Vec GetIdealCameraPos(float distance)
{
	Vec PlayerPos = G::Localplayer->GetEyePos();	//pleyer center position
	Vec Ideal = PlayerPos;							//Final ideal angle
	QAngle FPAng = G::CM_StartAngle;				//flipped player angle
	FPAng.x *= -1;
	FPAng.y += 180;
	FPAng.NormalizeAngle();

	Ideal.z += distance * sin(DEG2RAD(FPAng.x));
	float Hz = distance * cos(DEG2RAD(FPAng.x)); //horizonal distance

	Ideal.x += Hz * cos(DEG2RAD(FPAng.y));
	Ideal.y += Hz * sin(DEG2RAD(FPAng.y));

	return Ideal;
}

float GetCameraBoomLength(float distance)
{
	Vec IdealCameraPos = GetIdealCameraPos(distance);	//ideal camera position
	Vec PlayerPos = G::Localplayer->GetEyePos();		//player center position

	trace_t Trace;
	Ray_t Ray(PlayerPos, IdealCameraPos);
	CTraceFilter Filter(I::entitylist->GetClientEntity(I::engine->GetLocalPlayer()));
	I::enginetrace->TraceRay(Ray, MASK_ALL, &Filter, &Trace);

	if (Trace.Fraction <= 1)
		return distance * Trace.Fraction * 0.9;
	else
		return distance;
}

void __stdcall H::DoPostScreenEffectsHook(int param)
{
	if (I::engine->IsInGame() && G::Localplayer->GetHealth() > 0) {
		if (true)
		{
			I::input->m_fCameraInThirdPerson = true;
			I::input->m_vecCameraOffset = Vec(G::CM_StartAngle.x, G::CM_StartAngle.y, GetCameraBoomLength(150.f));
		}
		else
		{
			I::input->m_fCameraInThirdPerson = false;
			I::input->m_vecCameraOffset = Vec(G::CM_StartAngle.x, G::CM_StartAngle.y, 0);
		}
	}

	return oDoPostScreenEffects(I::clientmode, param);
}

void __fastcall H::DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	chams->Init();
	chams->Run(thisptr, edx, ctx, state, info, customBoneToWorld);
}

void __stdcall H::EmitSoundHook(SoundData data)
{
	static DWORD loc = FindPattern("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12");
	static std::add_pointer_t<bool __stdcall(const char*)> acceptMatch = reinterpret_cast<decltype(acceptMatch)>(loc);
	if (!strcmp(data.soundEntry, "UIPanorama.popup_accept_match_beep"))
	{
		H::console.clear();
		H::console.resize(0);
		H::console.push_back("FOUND MATCH ATTEMPTING ACCEPT!");
		acceptMatch("accept");
		/*auto window = FindWindowW(L"Valve001", NULL);
		FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
		FlashWindowEx(&flash);
		ShowWindow(window, SW_RESTORE);*/

		//Comment multiple lines of code: [ctrl] + [shift] + [/]
	}
	return oEmitSound(I::sound, data);
}
