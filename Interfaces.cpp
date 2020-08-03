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
}