#pragma once
typedef void* (*oInterface)(char*, int);
class InterfaceTools
{
public:
	void* GetInterface(char* MoudleName, char* InterfaceName)
	{
		oInterface Original = (oInterface)GetProcAddress(GetModuleHandleA(MoudleName), "CreateInterface");

		return (void*)Original(InterfaceName, 0);
	}
};