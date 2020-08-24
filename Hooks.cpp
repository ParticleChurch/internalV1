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

	std::cout << "Endscene...";
	oEndScene = (EndScene)d3d9VMT.HookMethod((DWORD)&EndSceneHook, 42);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Endscene...Success!");

	Sleep(100);

	std::cout << "Reset...";
	oReset = (Reset)d3d9VMT.HookMethod((DWORD)&ResetHook, 16);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo Reset...Success!");

	Sleep(100);

	std::cout << "CreateMove...";
	oCreateMove = (CreateMove)clientmodeVMT.HookMethod((DWORD)&CreateMoveHook, 24);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo CreateMove...Success!");

	Sleep(100);

	std::cout << "PaintTraverse...";
	oPaintTraverse = (PaintTraverse)panelVMT.HookMethod((DWORD)&PaintTraverseHook, 41);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo PaintTraverse...Success!");

	Sleep(100);

	std::cout << "FrameStageNotify...";
	backtrack->Init();
	oFrameStageNotify = (FrameStageNotify)clientVMT.HookMethod((DWORD)&FrameStageNotifyHook, 37);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FrameStageNotify...Success!");

	Sleep(100);

	std::cout << "LockCursor...";
	oLockCursor = (LockCursor)surfaceVMT.HookMethod((DWORD)&LockCursorHook, 67);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo LockCursor...Success!");

	Sleep(100);

	std::cout << "FireEventClientSide...";
	oFireEventClientSide = (FireEventClientSide)gameeventmanagerVMT.HookMethod((DWORD)&FireEventClientSideHook, 9);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEventClientSide...Success!");

	Sleep(100);

	std::cout << "FireEvent...";
	oFireEvent = (FireEvent)gameeventmanagerVMT.HookMethod((DWORD)&FireEventHook, 8);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo FireEvent...Success!");

	Sleep(100);

	std::cout << "hkCamToFirstPeronVMT...";
	ohkCamToFirstPeron = (hkCamToFirstPeron)inputVMT.HookMethod((DWORD)&hkCamToFirstPeronHook, 36);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo hkCamToFirstPeronVMT...Success!");

	Sleep(100);

	std::cout << "DoPostScreenEffects...";
	oDoPostScreenEffects = (DoPostScreenEffects)clientmodeVMT.HookMethod((DWORD)&DoPostScreenEffectsHook, 44);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DoPostScreenEffects...Success!");

	Sleep(100);

	std::cout << "DrawModelExecute...";
	oDrawModelExecute = (DrawModelExecute)modelrenderVMT.HookMethod((DWORD)&DrawModelExecuteHook, 21);
	std::cout << "Success!" << std::endl;
	I::engine->ClientCmd_Unrestricted("echo DrawModelExecute...Success!");
}

void H::UnHook()
{
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
		float ServerTime = I::globalvars->ServerTime(cmd);

		if (GUI::ShowMenu) {
			cmd->buttons = 0;
			cmd->upmove = 0;
			return false;
		}

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pSendPacket;

		bSendPacket = I::engine->GetNetChannelInfo()->ChokedPackets >= 2;

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

		//AA FIX
		if (fabsf(G::cmd->sidemove) < 5.0f) {
			G::cmd->sidemove = G::cmd->tick_count & 1 ? 3.25f : -3.25f;
		}

		H::console.clear();
		H::console.resize(0);
		H::console.push_back(std::to_string(G::Localplayer->MaxAccurateSpeed()));

		G::CM_MoveFixStart();

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

		antiaim->legit();

		//dont aa if in going to attack this tick
		bool InPrimary = cmd->buttons & IN_ATTACK;
		bool InSecondary = cmd->buttons & IN_ATTACK2;
		float TimeToAttack = G::Localplayer->GetNextAttack() - ServerTime;
		//bool CanPrimary = (timeToAttack <= 0 && timeToPrimary <= 0 && (bulletsLeft > 0 || !ActiveWeapon->IsGun()));
		//bool CanSecondary = (timeToAttack <= 0 && timeToSecondary <= 0 && !ActiveWeapon->IsGun());
		// if (CanPrimary && InPrimary || InSecondary && InSecondary)
		//	 G::cmd->viewangles = G::CM_StartAngle;
		if (cmd->buttons & IN_ATTACK)
			G::cmd->viewangles = G::CM_StartAngle;

		//aimbot->Legit();

		

		//if (GetAsyncKeyState(VK_LMENU))
		//	aimbot->Rage();

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
		int HitGroup = event->GetInt("hitgroup");

		H::console.clear();
		H::console.resize(0);
		H::console.push_back("Hurt: " + std::to_string(HitGroup));


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

void OverideMat(bool ignorez, bool wireframe, bool transparent, Color rgba, void* thisptr, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
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

float DotProduct(const float* v1, const float* v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void SinCos(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

//-----------------------------------------------------------------------------
// Euler QAngle -> Basis Vectors.  Each vector is optional
//-----------------------------------------------------------------------------
void AngleVectors(const QAngle& angles, Vec* forward, Vec* right, Vec* up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

void VectorAngles(const Vec& forward, QAngle& angles)
{
	float	tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;
		if (forward.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward.y, forward.x) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2(-forward.z, tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}

void MatrixCopy(const Matrix3x4& source, Matrix3x4& target)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			target.c[i][j] = source[i][j];
		}
	}
}

void MatrixSetColumn(const Vec& in, int column, Matrix3x4& out)
{
	out.c[0][column] = in.x;
	out.c[1][column] = in.y;
	out.c[2][column] = in.z;
}

void MatrixMultiply(const Matrix3x4& src1, const Matrix3x4& src2, Matrix3x4& dst)
{
	typedef float VMatrixRaw_t[4]; // ok?

	// Make sure it works if src1 == dst or src2 == dst
	Matrix3x4 tmp1, tmp2;
	const VMatrixRaw_t* s1 = (&src1 == &dst) ? tmp1.c : src1.c;
	const VMatrixRaw_t* s2 = (&src2 == &dst) ? tmp2.c : src2.c;

	if (&src1 == &dst)
	{
		MatrixCopy(src1, tmp1);
	}
	if (&src2 == &dst)
	{
		MatrixCopy(src2, tmp2);
	}

	dst.c[0][0] = s1[0][0] * s2[0][0] + s1[0][1] * s2[1][0] + s1[0][2] * s2[2][0] + s1[0][3] * s2[3][0];
	dst.c[0][1] = s1[0][0] * s2[0][1] + s1[0][1] * s2[1][1] + s1[0][2] * s2[2][1] + s1[0][3] * s2[3][1];
	dst.c[0][2] = s1[0][0] * s2[0][2] + s1[0][1] * s2[1][2] + s1[0][2] * s2[2][2] + s1[0][3] * s2[3][2];
	dst.c[0][3] = s1[0][0] * s2[0][3] + s1[0][1] * s2[1][3] + s1[0][2] * s2[2][3] + s1[0][3] * s2[3][3];

	dst.c[1][0] = s1[1][0] * s2[0][0] + s1[1][1] * s2[1][0] + s1[1][2] * s2[2][0] + s1[1][3] * s2[3][0];
	dst.c[1][1] = s1[1][0] * s2[0][1] + s1[1][1] * s2[1][1] + s1[1][2] * s2[2][1] + s1[1][3] * s2[3][1];
	dst.c[1][2] = s1[1][0] * s2[0][2] + s1[1][1] * s2[1][2] + s1[1][2] * s2[2][2] + s1[1][3] * s2[3][2];
	dst.c[1][3] = s1[1][0] * s2[0][3] + s1[1][1] * s2[1][3] + s1[1][2] * s2[2][3] + s1[1][3] * s2[3][3];

	dst.c[2][0] = s1[2][0] * s2[0][0] + s1[2][1] * s2[1][0] + s1[2][2] * s2[2][0] + s1[2][3] * s2[3][0];
	dst.c[2][1] = s1[2][0] * s2[0][1] + s1[2][1] * s2[1][1] + s1[2][2] * s2[2][1] + s1[2][3] * s2[3][1];
	dst.c[2][2] = s1[2][0] * s2[0][2] + s1[2][1] * s2[1][2] + s1[2][2] * s2[2][2] + s1[2][3] * s2[3][2];
	dst.c[2][3] = s1[2][0] * s2[0][3] + s1[2][1] * s2[1][3] + s1[2][2] * s2[2][3] + s1[2][3] * s2[3][3];

	dst.c[3][0] = s1[3][0] * s2[0][0] + s1[3][1] * s2[1][0] + s1[3][2] * s2[2][0] + s1[3][3] * s2[3][0];
	dst.c[3][1] = s1[3][0] * s2[0][1] + s1[3][1] * s2[1][1] + s1[3][2] * s2[2][1] + s1[3][3] * s2[3][1];
	dst.c[3][2] = s1[3][0] * s2[0][2] + s1[3][1] * s2[1][2] + s1[3][2] * s2[2][2] + s1[3][3] * s2[3][2];
	dst.c[3][3] = s1[3][0] * s2[0][3] + s1[3][1] * s2[1][3] + s1[3][2] * s2[2][3] + s1[3][3] * s2[3][3];
}

void AngleMatrix(const QAngle& angles, Matrix3x4& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix.c[0][0] = cp * cy;
	matrix.c[1][0] = cp * sy;
	matrix.c[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;
	matrix.c[0][1] = sp * srcy - crsy;
	matrix.c[1][1] = sp * srsy + crcy;
	matrix.c[2][1] = sr * cp;

	matrix.c[0][2] = (sp * crcy + srsy);
	matrix.c[1][2] = (sp * crsy - srcy);
	matrix.c[2][2] = cr * cp;

	matrix.c[0][3] = 0.0f;
	matrix.c[1][3] = 0.0f;
	matrix.c[2][3] = 0.0f;
}

void AngleMatrix(const QAngle& angles, const Vec& position, Matrix3x4& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

void MatrixMultiply(Matrix3x4& in1, const Matrix3x4& in2)
{
	Matrix3x4 out;
	if (&in1 == &out)
	{
		Matrix3x4 in1b;
		MatrixCopy(in1, in1b);
		MatrixMultiply(in1b, in2);
		return;
	}
	if (&in2 == &out)
	{
		Matrix3x4 in2b;
		MatrixCopy(in2, in2b);
		MatrixMultiply(in1, in2b);
		return;
	}
	out.c[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out.c[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out.c[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out.c[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out.c[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out.c[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out.c[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out.c[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out.c[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out.c[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out.c[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out.c[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	in1 = out;
}

void VectorRotate(const float* in1, const Matrix3x4& in2, float* out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}

void VectorRotate(const Vec& in1, const Matrix3x4& in2, Vec& out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

void VectorRotate(const Vec& in1, const QAngle& in2, Vec& out)
{
	Matrix3x4 matRotate;
	AngleMatrix(in2, matRotate);
	VectorRotate(in1, matRotate, out);
}

void RotateBoneMatrix(float AAangle, Vec origin, Matrix3x4* pCustomBoneToWorld, Matrix3x4* BoneMatrix)
{
	Vec BonePos;
	Vec OutPos;
	for (int i = 0; i < MAXSTUDIOBONES; i++)
	{
		AngleMatrix(Vec(0, AAangle, 0), BoneMatrix[i]);
		MatrixMultiply(BoneMatrix[i], pCustomBoneToWorld[i]);
		BonePos = Vec(pCustomBoneToWorld[i][0][3] - origin.x, pCustomBoneToWorld[i][1][3] - origin.y, pCustomBoneToWorld[i][2][3] - origin.z);
		VectorRotate(BonePos, Vec(0, AAangle, 0), OutPos);
		OutPos += origin;
		BoneMatrix[i].c[0][3] = OutPos.x;
		BoneMatrix[i].c[1][3] = OutPos.y;
		BoneMatrix[i].c[2][3] = OutPos.z;
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
			
			static Matrix3x4 BoneMatrix[MAXSTUDIOBONES];
			RotateBoneMatrix(G::RealAngle.y - G::FakeAngle.y, G::Localplayer->GetVecOrigin(), customBoneToWorld, BoneMatrix);

			static Color color_local = Color(1, 250, 254);
			OverideMat(
				false,	//viz thru wall?
				false,	//wireframe?
				false,	//transparent?
				color_local,
				thisptr, ctx, state, info, BoneMatrix);

			static Color color_desync = Color(255, 255, 255);
			OverideMat(
				false,	//viz thru wall?
				true,	//wireframe?
				false,	//transparent?
				color_desync,
				thisptr, ctx, state, info, G::FakeMatrix);
				
			
			/*
			static Color color_local = Color(1, 250, 254);
			OverideMat(
				false,	//viz thru wall?
				false,	//wireframe?
				false,	//transparent?
				color_local,
				thisptr, ctx, state, info, customBoneToWorld);
			*/
			
			
		}
	}
	else
	{
		oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	}

}
