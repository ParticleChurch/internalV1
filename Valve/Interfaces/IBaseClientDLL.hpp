#pragma once
class IBaseClientDLL
{
public:
	DECLARE_VIRTUAL_METHOD(ClientClass*, GetAllClasses, 8, (), ());
	DECLARE_VIRTUAL_METHOD(bool, dispatchUserMessage, 38, (int messageType, int arg, int arg1, void* data), (messageType, arg, arg1, data));
};