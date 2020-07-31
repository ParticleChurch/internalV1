#pragma once
class IClientEntityList
{
public:
	int GetHighestEntityIndex(void)
	{
		typedef int(__thiscall* oGetHighestIndex)(void*);

		return GetVFunc<oGetHighestIndex>(this, 6)(this);
	}

	Entity* GetClientEntity(int index)
	{
		typedef Entity* (__thiscall* oClientEnt)(void*, int);

		return GetVFunc<oClientEnt>(this, 3)(this, index);
	}

	Entity* GetClientEntityFromHandle(HANDLE handle)
	{
		typedef Entity* (__thiscall* oGetClientEntityFromHandle)(void*, HANDLE);

		return GetVFunc<oGetClientEntityFromHandle>(this, 4)(this, handle);
	}
};