#include "Include.hpp"

namespace I
{
	InterfaceTools* NewInterface = new InterfaceTools;

	IBaseClientDLL* client = (IBaseClientDLL*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClient018");
	ClientMode* clientmode = **(ClientMode***)((*(uintptr_t**)(client))[10] + 0x5);
	GlobalVars* globalvars = **(GlobalVars***)((*(uintptr_t**)(client))[11] + 10);
	CInput* input = *(CInput**)((*(DWORD**)client)[16] + 0x1);
	IVEngineClient* engine = (IVEngineClient*)NewInterface->GetInterface((char*)("engine.dll"), (char*)("VEngineClient014"));
	IClientEntityList* entitylist = (IClientEntityList*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClientEntityList003");
	Surface* surface = (Surface*)NewInterface->GetInterface((char*)"vguimatsurface.dll", (char*)"VGUI_Surface031");
	Panel* panel = (Panel*)NewInterface->GetInterface((char*)"vgui2.dll", (char*)"VGUI_Panel009");
	IVModelInfo* modelinfo = (IVModelInfo*)NewInterface->GetInterface((char*)"engine.dll", (char*)"VModelInfoClient004");
	IEngineTrace* enginetrace = (IEngineTrace*)NewInterface->GetInterface((char*)"engine.dll", (char*)"EngineTraceClient004");
	PhysicsSurfaceProps* physicssurfaceprops = (PhysicsSurfaceProps*)NewInterface->GetInterface((char*)"vphysics.dll", (char*)"VPhysicsSurfaceProps001");
	GameEventManager* gameeventmanager = (GameEventManager*)NewInterface->GetInterface((char*)"engine.dll", (char*)"GAMEEVENTSMANAGER002");
	InputSystem* inputsystem = (InputSystem*)NewInterface->GetInterface((char*)"inputsystem.dll", (char*)"InputSystemVersion001");
	CVar* cvar = (CVar*)NewInterface->GetInterface((char*)"vstdlib.dll", (char*)"VEngineCvar007");
	ModelRender* modelrender = (ModelRender*)NewInterface->GetInterface((char*)"engine.dll", (char*)"VEngineModel016");
	StudioRender* studiorender = (StudioRender*)NewInterface->GetInterface((char*)"studiorender.dll", (char*)"VStudioRender026");
	MaterialSystem* materialsystem = (MaterialSystem*)NewInterface->GetInterface((char*)"materialsystem.dll", (char*)"VMaterialSystem080");
	Sound* sound = (Sound*)NewInterface->GetInterface((char*)"engine.dll", (char*)"IEngineSoundClient003");
	IVDebugOverlay* debugoverlay = (IVDebugOverlay*)NewInterface->GetInterface((char*)"engine.dll", (char*)"VDebugOverlay004");
	ClientState* clientstate = **reinterpret_cast<ClientState***>(FindPattern("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	IMemAlloc* memalloc = *reinterpret_cast<IMemAlloc**> (GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));
}