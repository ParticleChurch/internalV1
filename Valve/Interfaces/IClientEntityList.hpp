#pragma once
class IClientEntityList
{
public:
	DECLARE_VIRTUAL_METHOD(IClientNetworkable*, GetClientNetworkable, 2, (int index), (index));
	DECLARE_VIRTUAL_METHOD(Entity*, GetClientEntity, 3, (int index), (index));
	DECLARE_VIRTUAL_METHOD(Entity*, GetClientEntityFromHandle, 4, (HANDLE handle), (handle));
	DECLARE_VIRTUAL_METHOD(int, GetHighestEntityIndex, 6, (), ());
};