#include "Include.hpp"

namespace I
{
	InterfaceTools* NewInterface = new InterfaceTools;

	IBaseClientDLL* client = (IBaseClientDLL*)NewInterface->GetInterface((char*)"client.dll", (char*)"VClient018");
}