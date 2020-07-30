#pragma once
typedef void* (*oInterface)(char*, int);
class InterfaceTools
{
public:
	void* GetInterface(char* moudleName, char* interfaceName)
	{
		oInterface original = (oInterface)GetProcAddress(GetModuleHandleA(moudleName), "CreateInterface");

		return (void*)original(interfaceName, 0);
	}
};