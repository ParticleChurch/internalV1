#include "Include.hpp"

namespace I
{
	InterfaceTools* NewInterface = new InterfaceTools;

	IBaseClientDLL* client = (IBaseClientDLL*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClient018");
	ClientMode* clientmode = **(ClientMode***)((*(uintptr_t**)(client))[10] + 0x5);
	IVEngineClient* engine = (IVEngineClient*)NewInterface->GetInterface((char*)("engine.dll"), (char*)("VEngineClient014"));
	IClientEntityList* entitylist = (IClientEntityList*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClientEntityList003");
}