#include "Include.hpp"

namespace I
{
	InterfaceTools* NewInterface = new InterfaceTools;

	IBaseClientDLL* client = (IBaseClientDLL*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClient018");
	ClientMode* clientmode = **(ClientMode***)((*(uintptr_t**)(client))[10] + 0x5);
	GlobalVars* globalvars = **(GlobalVars***)((*(uintptr_t**)(client))[11] + 10);
	IVEngineClient* engine = (IVEngineClient*)NewInterface->GetInterface((char*)("engine.dll"), (char*)("VEngineClient014"));
	IClientEntityList* entitylist = (IClientEntityList*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClientEntityList003");
	Surface* surface = (Surface*)NewInterface->GetInterface((char*)"vguimatsurface.dll", (char*)"VGUI_Surface031");
	Panel* panel = (Panel*)NewInterface->GetInterface((char*)"vgui2.dll", (char*)"VGUI_Panel009");
}