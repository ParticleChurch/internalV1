#pragma once
class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses()
	{
		typedef ClientClass* (__thiscall* oGetAllClasses)(void*);
		return GetVFunc< oGetAllClasses >(this, 8)(this);
	}

	constexpr bool dispatchUserMessage(int messageType, int arg, int arg1, void* data) noexcept
	{
		typedef bool(__thiscall* odispatchUserMessage)(void*, int, int, int, void*);
		return GetVFunc< odispatchUserMessage >(this, 38)(this, messageType, arg, arg1, data);
	}
};