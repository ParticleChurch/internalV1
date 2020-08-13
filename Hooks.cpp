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

	std::cout << "Hooking..." << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Hooking");
	d3d9VMT.Initialise((DWORD*)D3d9Device);
	clientVMT.Initialise((DWORD*)I::client);
	clientmodeVMT.Initialise((DWORD*)I::clientmode);
	surfaceVMT.Initialise((DWORD*)I::surface);
	panelVMT.Initialise((DWORD*)I::panel);
	gameeventmanagerVMT.Initialise((DWORD*)I::gameeventmanager);
	inputVMT.Initialise((DWORD*)I::input);
	modelrenderVMT.Initialise((DWORD*)I::modelrender);

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

	std::cout << "FireEvent...";
	oFireEvent = (FireEvent)gameeventmanagerVMT.HookMethod((DWORD)&FireEventHook, 8);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEvent...Success!");

	std::cout << "hkCamToFirstPeronVMT...";
	ohkCamToFirstPeron = (hkCamToFirstPeron)inputVMT.HookMethod((DWORD)&hkCamToFirstPeronHook, 36);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo hkCamToFirstPeronVMT...Success!");

	std::cout << "DoPostScreenEffects...";
	oDoPostScreenEffects = (DoPostScreenEffects)clientmodeVMT.HookMethod((DWORD)&DoPostScreenEffectsHook, 44);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DoPostScreenEffects...Success!");

	std::cout << "DrawModelExecute...";
	oDrawModelExecute = (DrawModelExecute)modelrenderVMT.HookMethod((DWORD)&DrawModelExecuteHook, 21);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DrawModelExecute...Success!");
}

void H::UnHook()
{
	*G::pSendPacket = true;
	SetWindowLongPtr(CSGOWindow, GWL_WNDPROC, (LONG_PTR)oWndProc);
	modelrenderVMT.RestoreOriginal();
	inputVMT.RestoreOriginal();
	gameeventmanagerVMT.RestoreOriginal();
	surfaceVMT.RestoreOriginal();
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

	if (GUI::Main())
		G::KillDLL = true;

	//temp code
	if (GetAsyncKeyState(VK_F11))
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

	if (I::engine->IsInGame() && cmd) 
	{
		I::globalvars->ServerTime(cmd);

		if (GUI::ShowMenu) {
			cmd->buttons = 0;
			cmd->upmove = 0;
			return false;
		}

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pSendPacket;

		bSendPacket = I::engine->GetNetChannelInfo()->ChokedPackets >= 5;

		/*
		static bool flip = false;
		if (!bSendPacket && !(G::Localplayer->GetFlags() & FL_ONGROUND))
		{
			if (cmd->sidemove > 300)
				cmd->viewangles.y -= 30;
			else if (cmd->sidemove < -300)
				cmd->viewangles.y += 30;
			flip = !flip;
			if (cmd->mousedx > 0.5 || flip)
			{
				cmd->viewangles.y += 2;
				cmd->sidemove = 450.0f;
			}
			else if (!flip || cmd->mousedx < 0.5)
			{
				cmd->viewangles.y -= 2;
				cmd->sidemove = -450.0f;
			}
			cmd->viewangles.Normalize();
		}
		*/

		G::CM_Start(cmd, pSendPacket);

		G::cmd->buttons |= IN_BULLRUSH;	//fast duck

		//SlowWalk
		if (false && G::Localplayer && G::Localplayer->GetHealth() > 0)
		{
			float maxSpeed = G::Localplayer->MaxAccurateSpeed();
			if (G::cmd->forwardmove && G::cmd->sidemove) {
				const float maxSpeedRoot = maxSpeed * static_cast<float>(M_SQRT1_2);
				G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
				G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeedRoot : maxSpeedRoot;
			}
			else if (G::cmd->forwardmove) {
				G::cmd->forwardmove = G::cmd->forwardmove < 0.0f ? -maxSpeed : maxSpeed;
			}
			else if (G::cmd->sidemove) {
				G::cmd->sidemove = G::cmd->sidemove < 0.0f ? -maxSpeed : maxSpeed;
			}
		}

		G::CM_MoveFixStart();

		antiaim->legit();

		//ez bhop
		if ((cmd->buttons & IN_JUMP) && (G::Localplayer->GetHealth() > 0) && !(G::Localplayer->GetFlags() & FL_ONGROUND)
			&& G::Localplayer->GetMoveType() != MOVETYPE_LADDER) {
			cmd->buttons &= ~IN_JUMP;
		}

		//toggle on and off third person
		static float lastUpdate = 0;
		if (GetAsyncKeyState(Config::visuals.ThirdPersonKey) &&
			fabsf(lastUpdate - I::globalvars->m_curTime) > 0.2f) {
			lastUpdate = I::globalvars->m_curTime;
			ThirdPersonToggle = !ThirdPersonToggle;
		}

		//manual shot
		if (cmd->buttons & IN_ATTACK)
			G::cmd->viewangles = G::CM_StartAngle;

		//aimbot->Legit();

		if (GetAsyncKeyState(VK_LMENU))
			aimbot->Rage();

		//backtrack->run();

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
	if (curStage == FRAME_RENDER_START)
	{
		if (I::engine->IsInGame()) {
			G::Localplayer = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());

			//this is for accurate angles (aa, etc)
			static DWORD offset = N::GetOffset("DT_CSPlayer", "deadflag");
			if (offset == 0)
				offset = N::GetOffset("DT_CSPlayer", "deadflag");

			if (I::input->m_fCameraInThirdPerson)
				*(Vec*)((DWORD)G::Localplayer + offset + 4) = G::FakeAngle;

			backtrack->update();

			*G::Localplayer->pGetFlashMaxAlpha() = 0;

			int LPIndex = I::engine->GetLocalPlayer();
			for (int i = 1; i <= I::engine->GetMaxClients(); i++) {
				Entity* entity = I::entitylist->GetClientEntity(i);
				if (!entity || i == LPIndex || entity->IsDormant() || !(entity->GetHealth() > 0)) continue;
				*reinterpret_cast<int*>(entity + 0xA28) = 1;							//visible???
				*reinterpret_cast<int*>(entity + 0xA30) = I::globalvars->m_frameCount; //sim time?
			}
		}
	}

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

		if (attacker == localIdx && userid != localIdx)
			I::engine->ClientCmd_Unrestricted("play buttons/arena_switch_press_02");
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

			//if not localplayer and not on our team...
			if (UserIndex != LocalIndex && G::Localplayer->GetTeam() != Entity->GetTeam() 
				&& UserIndex < 65 && UserIndex >= 0)
			{
				backtrack->Records[UserIndex].front().Value += 20; //better value
			}
				
		}
		break;
	}
	return oFireEvent(I::gameeventmanager, event, bDontBroadcast);
}

void __fastcall H::hkCamToFirstPeronHook()
{
	if (Config::visuals.Enable && Config::visuals.ThirdPerson && ThirdPersonToggle)
		return;
	ohkCamToFirstPeron(I::input);
}

Vec GetIdealCameraPos(float distance)
{
	Vec PlayerPos = G::Localplayer->GetEyePos();	//pleyer center position
	Vec Ideal = PlayerPos;							//Final ideal angle
	QAngle FPAng = G::CM_StartAngle;				//flipped player angle
	FPAng.x *= -1;
	FPAng.y += 180;
	FPAng.Normalize();

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

	if (I::engine->IsInGame()) {
		if (Config::visuals.Enable && Config::visuals.ThirdPerson && ThirdPersonToggle)
		{
			if (!(I::input->m_fCameraInThirdPerson))
				I::input->m_fCameraInThirdPerson = true;
			I::input->m_vecCameraOffset = Vec(G::CM_StartAngle.x, G::CM_StartAngle.y, GetCameraBoomLength(150.f));
		}
		else
		{
			if ((I::input->m_fCameraInThirdPerson))
				I::input->m_fCameraInThirdPerson = false;
			I::input->m_vecCameraOffset = Vec(G::CM_StartAngle.x, G::CM_StartAngle.y, 0);
		}
	}

	return oDoPostScreenEffects(I::clientmode, param);
}

void OverideMat(bool ignorez, bool wireframe, bool transparent, Color rgba,
	void* thisptr, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	static Material* normal = I::materialsystem->CreateMaterial("normal", KeyValues::FromString("VertexLitGeneric", nullptr));

	normal->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	if (transparent)
		normal->AlphaModulate(0.45);

	normal->SetMaterialVarFlag(MaterialVarFlag::IGNOREZ, ignorez);
	normal->SetMaterialVarFlag(MaterialVarFlag::WIREFRAME, wireframe);

	I::modelrender->ForcedMaterialOverride(normal);
	H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	if (ignorez)
	{
		I::modelrender->ForcedMaterialOverride(nullptr);
	}
}

void __fastcall H::DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	bool is_arm = strstr(info.model->name, "arms") != nullptr;
	bool is_player = strstr(info.model->name, "models/player") != nullptr;
	bool is_sleeve = strstr(info.model->name, "sleeve") != nullptr;
	bool is_flash = strstr(info.model->name, "flash") != nullptr;

	static Color color_blocked = Color(200, 100, 100);
	static Color color_visible(100, 200, 100);
	Entity* ent = I::entitylist->GetClientEntity(info.entityIndex);
	Entity* local = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	static player_info_t bInfo;

	if (ent && local && ent->GetHealth() > 0 && ent->IsPlayer() && I::engine->GetPlayerInfo(info.entityIndex, &bInfo))
	{
		bool isEnemy = ent->GetTeam() != local->GetTeam();

		if (isEnemy)
		{
			OverideMat(
				true,	//viz thru wall?
				false,	//wireframe?
				false,	//transparent?
				color_blocked,
				thisptr, ctx, state, info, customBoneToWorld);

			//oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);

			OverideMat(
				false,	//viz thru wall?
				false,	//wireframe?
				false,	//transparent?
				color_visible,
				thisptr, ctx, state, info, customBoneToWorld);

			//oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
		}
		else if(info.entityIndex == I::engine->GetLocalPlayer())
		{

			static Color color_local = Color(1, 250, 254);
			OverideMat(
				true,	//viz thru wall?
				false,	//wireframe?
				true,	//transparent?
				color_local,
				thisptr, ctx, state, info, customBoneToWorld);
		}
	}
	else
	{
		oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	}

}
