#pragma once
class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn;
	CreateEventFn* m_pCreateEventFn;
	char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};