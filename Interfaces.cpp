#include "Include.hpp"

namespace I
{
	//InterfaceTools* NewInterface = new InterfaceTools;
	IBaseClientDLL*       client               = nullptr;
	ClientMode*           clientmode           = nullptr;
	GlobalVars*           globalvars           = nullptr;
	CInput*               input                = nullptr;
	IVEngineClient*       engine               = nullptr;
	IClientEntityList*    entitylist           = nullptr;
	Surface*              surface              = nullptr;
	Panel*                panel                = nullptr;
	IVModelInfo*          modelinfo            = nullptr;
	IEngineTrace*         enginetrace          = nullptr;
	PhysicsSurfaceProps*  physicssurfaceprops  = nullptr;
	GameEventManager*     gameeventmanager     = nullptr;
	InputSystem*          inputsystem          = nullptr;
	CVar*                 cvar                 = nullptr;
	ModelRender*          modelrender          = nullptr;
	StudioRender*         studiorender         = nullptr;
	MaterialSystem*       materialsystem       = nullptr;
	Sound*                sound                = nullptr;
	IVDebugOverlay*       debugoverlay         = nullptr;
	ClientState*          clientstate          = nullptr;
	IMemAlloc*            memalloc             = nullptr;
	CEffects*			  effects			   = nullptr;
	ViewRenderBeams*      viewrenderbeams      = nullptr;

	void GUIInit()
	{
		engine = GetInterface<IVEngineClient      >("engine.dll", "VEngineClient014");
		surface = GetInterface<Surface             >("vguimatsurface.dll", "VGUI_Surface031");
	}

	void Init()
	{
		//InterfaceTools* NewInterface = new InterfaceTools;
		client               = GetInterface<IBaseClientDLL>("client.dll", "VClient018");
		clientmode           = **(ClientMode***)((*(uintptr_t**)(client))[10] + 0x5);
		globalvars           = **(GlobalVars***)((*(uintptr_t**)(client))[11] + 10);
		input                = *(CInput**)((*(DWORD**)client)[16] + 0x1);
		entitylist           = GetInterface<IClientEntityList   >("client.dll",         "VClientEntityList003");
		panel                = GetInterface<Panel               >("vgui2.dll",          "VGUI_Panel009");
		modelinfo            = GetInterface<IVModelInfo         >("engine.dll",         "VModelInfoClient004");
		enginetrace          = GetInterface<IEngineTrace        >("engine.dll",         "EngineTraceClient004");
		physicssurfaceprops  = GetInterface<PhysicsSurfaceProps >("vphysics.dll",       "VPhysicsSurfaceProps001");
		gameeventmanager     = GetInterface<GameEventManager    >("engine.dll",         "GAMEEVENTSMANAGER002");
		inputsystem          = GetInterface<InputSystem         >("inputsystem.dll",    "InputSystemVersion001");
		cvar                 = GetInterface<CVar                >("vstdlib.dll",        "VEngineCvar007");
		modelrender          = GetInterface<ModelRender         >("engine.dll",         "VEngineModel016");
		studiorender         = GetInterface<StudioRender        >("studiorender.dll",   "VStudioRender026");
		materialsystem       = GetInterface<MaterialSystem      >("materialsystem.dll", "VMaterialSystem080");
		sound                = GetInterface<Sound               >("engine.dll",         "IEngineSoundClient003");
		debugoverlay         = GetInterface<IVDebugOverlay      >("engine.dll",         "VDebugOverlay004");
		clientstate          = **reinterpret_cast<ClientState***>(FindPattern("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		memalloc             = *reinterpret_cast<IMemAlloc**> (GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));
		effects				 = GetInterface<CEffects			>("engine.dll", "VEngineEffects001");
		viewrenderbeams		 = *reinterpret_cast<ViewRenderBeams**>(FindPattern("client.dll", "B9 ? ? ? ? 0F 11 44 24 ? C7 44 24 ? ? ? ? ? F3 0F 10 84 24") + 1);
	}
}