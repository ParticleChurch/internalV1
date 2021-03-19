#include "Include.hpp"

//for windprc hook
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void CapsuleOverlay(Entity* pPlayer, Color col, float duration)
{
	if (!pPlayer)
		return;

	studiohdr_t* pStudioModel = I::modelinfo->GetStudioModel(pPlayer->GetModel());
	if (!pStudioModel)
		return;

	static Matrix3x4 pBoneToWorldOut[128];
	if (!pPlayer->SetupBones(pBoneToWorldOut, 128, 0x00000100, 0))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	auto VectorTransform2 = [](const Vec in1, Matrix3x4 in2, Vec& out)
	{

		out.x = DotProduct(in1, Vec(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out.y = DotProduct(in1, Vec(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out.z = DotProduct(in1, Vec(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	};

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vec vMin, vMax;
		VectorTransform2(pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform2(pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->m_flRadius > -1)
		{
			I::debugoverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->m_flRadius, col.r(), col.g(), col.b(), 255, duration);
		}
	}
}

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
		L::Verbose("EventListener::FireGameEvent -> processing event: ", "");
		L::Verbose(event->GetName());
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
				static auto LegitInvert = Config2::GetState("antiaim-legit-invert");
				LegitInvert->Invert();
			}

			/*Entity* ent = I::entitylist->GetClientEntity(userid);
			if (ent && userid != localIdx)
				CapsuleOverlay(ent, Color(255, 255, 255, 100), 1);*/
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
			int DeathIndex = I::engine->GetPlayerForUserID(event->GetInt("userid"));

			const auto localIdx = I::engine->GetLocalPlayer();
			if (I::engine->GetPlayerForUserID(event->GetInt("attacker")) != localIdx || DeathIndex == localIdx)
				break;
			I::engine->ClientCmd_Unrestricted("play player/neck_snap_01");
			/*Entity* ent = I::entitylist->GetClientEntity(DeathIndex);
			if (ent)
				CapsuleOverlay(ent, Color(255,0,0,255), 1);*/
			
		}
		break;
		}
		
		killsay->run(event);
		resolver->LogShots(event);
		esp->Run_GameEvent(event);

		L::Verbose("EventListener::FireGameEvent -> end processing event: ", "");
		L::Verbose(event->GetName());
	}
};

namespace H
{
	VMTManager* d3d9VMT = nullptr;
	VMTManager* clientVMT = nullptr;
	VMTManager* clientmodeVMT = nullptr;
	VMTManager* surfaceVMT = nullptr;
	VMTManager* panelVMT = nullptr;
	VMTManager* gameeventmanagerVMT = nullptr;
	VMTManager* inputVMT = nullptr;
	VMTManager* modelrenderVMT = nullptr;
	VMTManager* soundVMT = nullptr;

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
	IDirect3DDevice9* D3d9Device; // PIXEL/SKINCHANGER FIX?
	bool D3dInit = false;
	HWND CSGOWindow = NULL;
	WNDPROC oWndProc = NULL;

	//TEMP
	std::vector < std::string> console;
	bool ThirdPersonToggle = false;
	EventListener* g_EventListener;

	//DT???
	WriteUsercmdDeltaToBuffer oWriteUsercmdDeltaToBuffer;
	void* WriteUsercmdDeltaToBufferReturn;
	uintptr_t WriteUsercmd;

	
}

template<typename ...Args>
void ConsoleColorMsg(const Color& color, const char* fmt, Args ...args)
{
	using ConColorMsg = void(*)(const Color&, const char*, ...);
	static ConColorMsg con_color_msg = nullptr;
	if (!con_color_msg) {
		con_color_msg = reinterpret_cast<ConColorMsg>(GetProcAddress(
			GetModuleHandleA("tier0.dll"),
			"?ConColorMsg@@YAXABVColor@@PBDZZ")
			);
	}

	con_color_msg(color, fmt, args...);
}

void H::GUIInit()
{
	PDWORD pD3d9Device = *(PDWORD*)(G::pD3d9DevicePattern + 1);
	D3d9Device = (IDirect3DDevice9*)*pD3d9Device;

	while (!(CSGOWindow = FindWindowW(L"Valve001", nullptr))) Sleep(10);

	d3d9VMT = new VMTManager((VMT*)D3d9Device);
	surfaceVMT = new VMTManager((VMT*)I::surface);
	
	oReset = (Reset)d3d9VMT->Hook(16, &ResetHook);
	oLockCursor = (LockCursor)surfaceVMT->Hook(67, &LockCursorHook);
	oEndScene = (EndScene)d3d9VMT->Hook(42, &EndSceneHook);

	oWndProc = WNDPROC(SetWindowLongPtrW(CSGOWindow, GWLP_WNDPROC, LONG_PTR(WndProc)));
}

void H::Init()
{
	WriteUsercmdDeltaToBufferReturn = *(reinterpret_cast<void**>(FindPattern("engine.dll", "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84")));
	WriteUsercmd = FindPattern("client.dll", " 55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");

	// hooks
	clientVMT = new VMTManager((VMT*)I::client);
	GUI2::LoadProgress += 0.01f;
	clientmodeVMT = new VMTManager((VMT*)I::clientmode);
	GUI2::LoadProgress += 0.01f;
	panelVMT = new VMTManager((VMT*)I::panel);
	GUI2::LoadProgress += 0.01f;
	gameeventmanagerVMT = new VMTManager((VMT*)I::gameeventmanager);
	GUI2::LoadProgress += 0.01f;
	inputVMT = new VMTManager((VMT*)I::input);
	GUI2::LoadProgress += 0.01f;
	modelrenderVMT = new VMTManager((VMT*)I::modelrender);
	GUI2::LoadProgress += 0.01f;
	soundVMT = new VMTManager((VMT*)I::sound);
	GUI2::LoadProgress += 0.01f;

	oCreateMove = (CreateMove)clientmodeVMT->Hook(24, &CreateMoveHook);
	oFrameStageNotify = (FrameStageNotify)clientVMT->Hook(37, &FrameStageNotifyHook);
	oPaintTraverse = (PaintTraverse)panelVMT->Hook(41, &PaintTraverseHook);
	oWriteUsercmdDeltaToBuffer = (WriteUsercmdDeltaToBuffer)clientVMT->Hook(24, &WriteUsercmdDeltaToBufferHook);
	ohkCamToFirstPeron = (hkCamToFirstPeron)inputVMT->Hook(36, &hkCamToFirstPeronHook);
	oDoPostScreenEffects = (DoPostScreenEffects)clientmodeVMT->Hook(44, &DoPostScreenEffectsHook);
	oDrawModelExecute = (DrawModelExecute)modelrenderVMT->Hook(21, &DrawModelExecuteHook);
	oEmitSound = (EmitSound)soundVMT->Hook(5, &EmitSoundHook);
	clantag->reset();
	GUI2::LoadProgress += 0.05f;

	g_EventListener = new EventListener();
	GUI2::LoadProgress += 0.05f;
}

void H::UnHook()
{
	// deal with dev conslole
	H::console.clear();
	H::console.resize(0);

	// make sure to disable fakelag & replace wndproc
	if (G::pSendPacket) *G::pSendPacket = true;
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);

	// unhook vmts
	if (d3d9VMT 		    ) d3d9VMT 		   	  ->UnhookAll();
	if (clientVMT 		    ) clientVMT 		  ->UnhookAll();
	if (clientmodeVMT 	    ) clientmodeVMT 	  ->UnhookAll();
	if (surfaceVMT 		    ) surfaceVMT 		  ->UnhookAll();
	if (panelVMT 		    ) panelVMT 		   	  ->UnhookAll();
	if (gameeventmanagerVMT ) gameeventmanagerVMT ->UnhookAll();
	if (inputVMT 		    ) inputVMT 		   	  ->UnhookAll();
	if (modelrenderVMT 	    ) modelrenderVMT 	  ->UnhookAll();
	if (soundVMT 		    ) soundVMT 		   	  ->UnhookAll();

}

void H::Free()
{
	if (d3d9VMT 		    ) delete d3d9VMT 		   	 ;
	if (clientVMT 		    ) delete clientVMT 			 ;
	if (clientmodeVMT 	    ) delete clientmodeVMT 		 ;
	if (surfaceVMT 		    ) delete surfaceVMT 		 ;
	if (panelVMT 		    ) delete panelVMT 		   	 ;
	if (gameeventmanagerVMT ) delete gameeventmanagerVMT ;
	if (inputVMT 		    ) delete inputVMT 		   	 ;
	if (modelrenderVMT 	    ) delete modelrenderVMT 	 ;
	if (soundVMT 		    ) delete soundVMT 		   	 ;

	// delete hack classes
	delete g_EventListener;
	delete miscvisuals;
	delete world;
	delete chams;
	delete esp;
	delete killsay;
	delete clantag;
	delete movement;
	delete antiaim;
	delete fakelag;
	delete doubletap;
	delete resolver;
	delete aimbot;
	delete autowall;
	delete backtrack;
}

long __stdcall H::EndSceneHook(IDirect3DDevice9* device)
{
	L::Verbose("H::EndSceneHook - begin");

	// init imgui
	if (!D3dInit) {
		L::Verbose("H::EndSceneHook d3d9 init begin");
		D3dInit = true;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		GUI2::LoadFonts(io);

		ImGui_ImplWin32_Init(CSGOWindow);
		ImGui_ImplDX9_Init(device);
		L::Verbose("H::EndSceneHook d3d9 init complete");
	}

	//* store pixelstate
	IDirect3DStateBlock9* PixelState; IDirect3DVertexDeclaration9* VertexDeclare; IDirect3DVertexShader9* VertexShader;
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &PixelState);
	device->GetVertexDeclaration(&VertexDeclare);
	device->GetVertexShader(&VertexShader);
	//*/
	
	// draw imgui 
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//debugger console
		ImGui::Begin("console");
		if (ImGui::Button("Clear Console"))
		{
			H::console.clear();
			H::console.resize(0);
		}
		ImGui::Text("DT");
		ImGui::SliderInt("###dtamount", &doubletap->shift_ticks, 0, 16);
		if (ImGui::Button("Reset Resolver"))
		{
			for (auto& a : resolver->ShotsMissed)
				a.second = 0;
		}
		if (ImGui::Button("Randomize Resolver"))
		{
			for (auto& a : resolver->ShotsMissed)
				a.second = rand() % 10;
		}
		
		for (auto a : console)
			ImGui::Text(a.c_str());
		ImGui::End();
		

		GUI2::Main();
		G::KillDLL |= GUI2::Ejected;

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	
	//* restore pixelstate
	PixelState->Apply();
	device->SetVertexDeclaration(VertexDeclare);
	device->SetVertexShader(VertexShader);
	PixelState->Release();
	//*/

	L::Verbose("H::EndSceneHook - complete");
	return oEndScene(device);
}

long __stdcall H::ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	L::Verbose("H::ResetHook - begin");
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
	
	auto ogOutput = oReset(device, pPresentationParameters);
	L::Verbose("H::ResetHook - complete");
	return ogOutput;
}

LRESULT __stdcall H::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	L::Verbose("H::WndProc - begin (", "");
	L::Verbose(std::to_string((DWORD)hWnd).c_str(), ", ");
	L::Verbose(std::to_string(uMsg).c_str(), ", ");
	L::Verbose(std::to_string(wParam).c_str(), ", ");
	L::Verbose(std::to_string(lParam).c_str(), ")\n");

	if (!D3dInit)
	{
		L::Verbose("H::WndProc - not initalized so calling original");
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	static auto MenuOpen = Config2::GetState("show-menu");
	bool IsKeyboardInput = uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_CHAR;
	bool IsMouseInput =
		uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSEWHEEL || uMsg == WM_INPUT || // wm_input is not always mouse but usually will be
		uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK ||
		uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK ||
		uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK ||
		uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK;

	L::Verbose(IsKeyboardInput ? "H::WndProc - keyboard input" : (IsMouseInput ? "H::WndProc - mouse input" : "H::WndProc - unknown input"));
	
	// give imgui input
	if (MenuOpen->Get())
	{
		bool im = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		L::Verbose("H::WndProc - imgui returned ", im ? "true\n" : "false\n");
	}
	
	// determine if csgo should receive this input
	ImGuiIO& io = ImGui::GetIO();
	bool HideInputFromCSGO = (IsMouseInput && io.WantCaptureMouse) || (IsKeyboardInput && io.WantCaptureKeyboard);
	if (HideInputFromCSGO)
		L::Verbose("H::WndProc - CS:GO will not receive this input");

	bool og = false;
	if (!HideInputFromCSGO)
		og = CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	L::Verbose("H::WndProc - complete (returning ", og ? "true)\n" : "false)\n");
	return og;
}

float RandomVal(float min, float max)
{
	int Delta = max - min;
	float val = rand() % Delta;
	return min + val;
}

bool __stdcall H::CreateMoveHook(float flInputSampleTime, CUserCmd* cmd)
{
	L::Verbose("H::CreateMoveHook - begin");
	bool oFunc = oCreateMove(flInputSampleTime, cmd);
	
	if (!oFunc || !cmd || !cmd->command_number)
	{
		L::Verbose("!oFunc || !cmd || !cmd->command_number");
		return oFunc;
	}
		
	L::Verbose("I::engine->IsInGame() && cmd && G::LocalPlayer");
	if (I::engine->IsInGame() && cmd && G::LocalPlayer)
	{
		L::Verbose("Assembly");
		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);

		L::Verbose("CM_Start");
		G::CM_Start(cmd, pSendPacket);

		// Fake lag Calculations
		L::Verbose("fakelag->Start();");
		fakelag->Start();

		// Doubletap Stuff
		doubletap->start();

		// Update server time
		L::Verbose("I::globalvars->ServerTime(cmd);");
		float ServerTime = I::globalvars->ServerTime(cmd);

		// Movement
		L::Verbose("movement");
		movement->AutoStop();
		movement->BunnyHop();	
		movement->SlowWalk();
		movement->FastCrouch();
		movement->AAMoveFix();
		movement->FakeDuck();
		movement->LegitAutoStrafe();
		movement->RageAutoStrafe();
		movement->LegSlide();
		

		// nade visuals
		L::Verbose("miscvisuals");
		miscvisuals->GrenadePrediction();
		//miscvisuals->ChangeViewModel();
	
		L::Verbose("CM_MoveFixStart");
		G::CM_MoveFixStart();

		// Fake Lag
		L::Verbose("fakelag->End();");
		*G::pSendPacket = fakelag->End();
		
		// AA
		L::Verbose("antiaim");
		antiaim->legit();
		antiaim->rage();

		// Clantag
		L::Verbose("clantag");
		clantag->run();
		L::Verbose("RankRevealer");
		miscvisuals->RankRevealer();

		// bad use (E) and attack (LBUTTON)
		if ((G::cmd->buttons & IN_USE) )
		{
			*G::pSendPacket = true;
			G::cmd->viewangles = G::StartAngle;
		}
		else if ((G::cmd->buttons & IN_ATTACK) && G::LocalPlayer->CanShoot()
			&& G::LocalPlayerWeapon
			&& GetWeaponClass(G::LocalPlayerWeapon->GetWeaponId()) != 40) // grenade
		{
			*G::pSendPacket = true;
			G::cmd->viewangles = G::StartAngle;
		}

		L::Verbose("aimbot");
		aimbot->Run();

		if (G::LocalPlayerWeapon && 
			GetWeaponClass(G::LocalPlayerWeapon->GetWeaponId()) == 40 && 
			G::LocalPlayerWeapon->GetGrenadeThrowTime() > 0)
		{
			// we are currently releasing a grenade, do not FUCK with angles
			G::cmd->viewangles = G::StartAngle;
		}

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

		L::Verbose("backtrack");
		backtrack->run();

		L::Verbose("CM_MoveFixEnd");
		G::CM_MoveFixEnd();

		

		// bad animation fix (for third person)

		static Config2::CState* LegitAA = Config2::GetState("antiaim-legit-enable");
		static Config2::CState* RageAA = Config2::GetState("antiaim-rage-enable");
		if ((G::cmd->buttons & IN_ATTACK) || (G::cmd->buttons & IN_USE) || 
			(!LegitAA->Get() && !RageAA->Get()))
		{
			antiaim->real = G::cmd->viewangles;
			antiaim->fake = G::cmd->viewangles;

			antiaim->real.NormalizeAngle();
			antiaim->fake.NormalizeAngle();
			
		}
		
		doubletap->end();

		L::Verbose("CM_End");
		G::CM_End();	

		//movement->RageAutoStrafe();
	}

	L::Verbose("H::CreateMoveHook - complete");
	return false; //silent aim on false (only for client)
}

void __stdcall H::PaintTraverseHook(int vguiID, bool force, bool allowForcing)
{
	L::Verbose("H::PaintTraverseHook - begin");

	static Config2::CState* NoScope = Config2::GetState("visuals-misc-noscope");

	if (strcmp("HudZoom", I::panel->GetName(vguiID)) == 0 && NoScope->Get())
		return;

	oPaintTraverse(I::panel, vguiID, force, allowForcing);
	if (I::panel && strcmp(I::panel->GetName(vguiID), "MatSystemTopPanel") == 0) {
		if (!G::LocalPlayer || !I::engine->IsInGame())
			return;

		miscvisuals->NoScope();
		esp->Run_PaintTraverse();
	}

	L::Verbose("H::PaintTraverseHook - complete");
}

bool fresh_tick()
{
	static int old_tick_count;

	if (old_tick_count != I::globalvars->m_tickCount)
	{
		old_tick_count = I::globalvars->m_tickCount;
		return true;
	}

	return false;
}

void LocalAnimFix(Entity* entity)
{
	if (!entity || !entity->GetHealth() || !G::cmd)
		return;

	AnimState* anim = G::LocalPlayer->GetAnimstate();
	if (!anim)
		return;

	bool* ClientAnims = entity->ClientAnimations();
	if (!ClientAnims)
		return;

	float duck = anim->m_fDuckAmount;

	static float proper_abs = anim->m_flGoalFeetYaw;
	
	static std::array<float, 24> sent_pose_params = entity->m_flPoseParameter();
	static AnimationLayer backup_layers[15];
	
	if (fresh_tick())
	{
		AnimationLayer* OldLayerPtr = entity->GetAnimOverlays();
		if (!OldLayerPtr)
			return;

		std::memcpy(backup_layers, OldLayerPtr, (sizeof(AnimationLayer) * 15));
		*ClientAnims = true;
		entity->UpdateAnimationState(anim, antiaim->real); // idek

		if (anim)
			anim->m_iLastClientSideAnimationUpdateFramecount = I::globalvars->m_frameCount - 1;
		
		entity->UpdateClientSideAnimation();
		
		if (G::pSendPacket && *G::pSendPacket)
		{
			proper_abs = anim->m_flGoalFeetYaw;
			sent_pose_params = entity->m_flPoseParameter();
		}
		
	}
	
	*ClientAnims = false;
	entity->SetAbsAngles(Vec(0, proper_abs, 0)); // MAYBE BAD?
	anim->m_flUnknownFraction = duck;// 
	std::memcpy(entity->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));
	entity->m_flPoseParameter() = sent_pose_params;
	
}

void __stdcall H::FrameStageNotifyHook(int stage)
{
	oFrameStageNotify(stage);

	L::Verbose("H::FrameStageNotifyHook - begin stage ", "");  L::Verbose(std::to_string(stage).c_str());

	// post processing
	static bool* disablePostProcessing = *reinterpret_cast<bool**>(FindPattern("client.dll", "83 EC 4C 80 3D") + 5);
	if (stage == FRAME_RENDER_START || stage == FRAME_RENDER_END)
		*disablePostProcessing = stage == FRAME_RENDER_START;

	// update globals
	G::IsInGame = I::engine->IsInGame();
	if (G::IsInGame)
	{
		G::LocalPlayerIndex = I::engine->GetLocalPlayer();
		G::LocalPlayer = I::entitylist->GetClientEntity(G::LocalPlayerIndex);
		G::LocalPlayerAlive = G::LocalPlayer ? G::LocalPlayer->GetHealth() > 0 && G::LocalPlayer->GetLifeState() == LIFE_ALIVE: false;
		G::LocalPlayerTeam = G::LocalPlayer ? G::LocalPlayer->GetTeam() : 0;
		G::LocalPlayerWeapon = G::LocalPlayer ? G::LocalPlayer->GetActiveWeapon() : nullptr;
		G::LocalPlayerWeaponData = G::LocalPlayerWeapon ? G::LocalPlayerWeapon->GetWeaponData() : nullptr;

		if (!G::LocalPlayer || !G::LocalPlayerAlive)
			backtrack->ClearRecords();

		if (G::LocalPlayer)
			LocalAnimFix(G::LocalPlayer);
		else
			return;
	}
	else
	{
		G::EntList.clear();
		G::LocalPlayerAlive = false;
		G::LocalPlayer = nullptr;
		G::LocalPlayerIndex = 0;
		G::LocalPlayerTeam = 0;
		G::LocalPlayerWeapon = nullptr;
		return;
	}

	/*
	if (curStage == FRAME_NET_UPDATE_END)
	{
		// stop retard weapon jiggle - might be what is crashing (was before)
		static ConVar* r_jiggle_bones = I::cvar->FindVar("r_jiggle_bones");
		if (r_jiggle_bones && G::IsInGame)
		{
			r_jiggle_bones->onChangeCallbacks.size = 0;
			if (r_jiggle_bones->GetInt() > 0)
				r_jiggle_bones->SetValue(0);
		}
	}
	*/

	static int deadflagOffset = N::GetOffset("DT_CSPlayer", "deadflag");
	static auto ThirdPerson = Config2::GetState("visuals-misc-thirdperson");

	switch (stage)
	{
	case FRAME_RENDER_START:
	{
		// update our local entlist
		G::UpdateEntities();

		// third person
		if (ThirdPerson->Get()) *(Vec*)((DWORD)G::LocalPlayer + deadflagOffset + 4) = antiaim->real;

		// bones out of view
		Entity* entity;
		for (int i = 1; i <= I::engine->GetMaxClients(); i++) {
			if (i == G::LocalPlayerIndex || !(entity = I::entitylist->GetClientEntity(i)) || entity->IsDormant() || entity->GetHealth() <= 0) continue;

			*(int*)(entity + 0xA30) = I::globalvars->m_frameCount; //skip occlusion check
			*(int*)(entity + 0xA28) = 0; ///clear occlusion flags --> should be zero ig (normally set to 1 in my code ig? idk am retar)
		}

		esp->RunFSN();
		backtrack->RunFSN();
		world->RunFSN();
	} break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
	{
		resolver->Resolve();
		miscvisuals->NoFlash();
		miscvisuals->NoSmokeFSN();
		SkinChanger::RunFSN();
	} break;
	}

	/*
	
		if (stage == FRAME_RENDER_START && G::LocalPlayer && G::LocalPlayerAlive && ThirdPerson->Get())
		{
			static auto util_playerbyindex = FindPattern("server.dll", "85 C9 7E 2A A1");
			static auto draw_server_hitboxes = FindPattern("server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE");

			auto t = -1.0f;

			auto idx = I::engine->GetLocalPlayer();

			__asm {
				mov ecx, idx
				call util_playerbyindex
				cmp eax, 0
				je gay
				pushad
				movss xmm1, t
				push 0
				mov ecx, eax
				call draw_server_hitboxes
				popad
			}

		gay:
			{

			}
		}
	*/
	L::Verbose("H::FrameStageNotifyHook - completed stage ", "");  L::Verbose(std::to_string(stage).c_str());
}

void __stdcall H::LockCursorHook()
{
	static auto MenuOpen = Config2::GetState("show-menu");
	L::Verbose("H::LockCursorHook - begin");
	if (MenuOpen->Get())
		I::surface->UnlockCursor();
	else
		oLockCursor(I::surface);
	L::Verbose("H::LockCursorHook - end");
}

void __fastcall H::hkCamToFirstPeronHook()
{
	L::Verbose("H::hkCamToFirstPeronHook - begin");
	miscvisuals->ThirdPerson_hkCamToFirstPeron();
	L::Verbose("H::hkCamToFirstPeronHook - end");
}

void __stdcall H::DoPostScreenEffectsHook(int param)
{
	L::Verbose("H::DoPostScreenEffectsHook - begin");
	miscvisuals->ThirdPerson_DoPostScreenEffects();
	miscvisuals->NoSmoke_DoPostScreenEffects();
	world->Run_DoPostScreenEffect();
	L::Verbose("H::DoPostScreenEffectsHook - end");

	return oDoPostScreenEffects(I::clientmode, param);
}

void __fastcall H::DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	/*return H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);*/
	L::Verbose("H::DrawModelExecuteHook - begin");
	
	chams->Init(); // BRO, there MUST be a better way to do this
	chams->Run(thisptr, edx, ctx, state, info, customBoneToWorld);
	// 

	L::Verbose("H::DrawModelExecuteHook - complete");
}

void __stdcall H::EmitSoundHook(SoundData data)
{
	L::Verbose("H::EmitSoundHook - begin");
	static std::add_pointer_t<bool __stdcall(const char*)> acceptMatch = reinterpret_cast<decltype(acceptMatch)>(G::AcceptMatchPattern);
	static Config2::CState* Enable = Config2::GetState("misc-other-autoaccept");

	if (!strcmp(data.soundEntry, "UIPanorama.popup_accept_match_beep") && Enable->Get())
	{
		acceptMatch("accept");
		/*auto window = FindWindowW(L"Valve001", NULL);
		FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
		FlashWindowEx(&flash);
		ShowWindow(window, SW_RESTORE);*/

	//	//Comment multiple lines of code: [ctrl] + [shift] + [/]
	}
	oEmitSound(I::sound, data);

	L::Verbose("H::EmitSoundHook - complete");
}

static void CWriteUsercmd(void* buf, CUserCmd* Cin, CUserCmd* Cout)
{
	static auto WriteUsercmdF = H::WriteUsercmd;

	__asm
	{
		mov ecx, buf
		mov edx, Cin
		push Cout
		call WriteUsercmdF
		add esp, 4
	}
}

bool __fastcall H::WriteUsercmdDeltaToBufferHook(void* ecx, void* edx, int slot, void* buf, int from, int to, bool isnewcommand)
{
	//return oWriteUsercmdDeltaToBuffer(ecx, slot, buf, from, to, isnewcommand);
	static auto ofunct = oWriteUsercmdDeltaToBuffer;

	if (doubletap->m_tick_to_shift <= 0)
		return ofunct(ecx, slot, buf, from, to, isnewcommand);

	if (from != -1)
		return true;

	auto CL_SendMove = []() {
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)FindPattern("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98");

		CL_SendMoveF();
	};

	int* pNumBackupCommands = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x30);
	int* pNumNewCommands = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x2C);
	auto net_channel = *reinterpret_cast<NetChannel**>(reinterpret_cast<uintptr_t>(I::clientstate) + 0x9C);
	int32_t new_commands = *pNumNewCommands;

	int32_t next_cmdnr = I::clientstate->m_last_outgoing_command + I::clientstate->m_choked_commands + 1;
	int32_t total_new_commands = min(doubletap->m_tick_to_shift, 16);
	doubletap->m_tick_to_shift -= total_new_commands;
	doubletap->m_tick_to_recharge += total_new_commands;

	from = -1;
	*pNumNewCommands = total_new_commands;
	*pNumBackupCommands = 0;

	for (to = next_cmdnr - new_commands + 1; to <= next_cmdnr; to++) {
		if (!ofunct(ecx, slot, buf, from, to, isnewcommand))
			return false;

		from = to;
	}

	CUserCmd* last_realCmd = I::input->GetUserCmd(slot, from);
	CUserCmd fromCmd;

	if (last_realCmd)
		fromCmd = *last_realCmd;

	CUserCmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count++;

	for (int i = new_commands; i <= total_new_commands; i++) {
		CWriteUsercmd(buf, &toCmd, &fromCmd);
		fromCmd = toCmd;
		toCmd.command_number++;
		toCmd.tick_count++;
	}
	return true;
}