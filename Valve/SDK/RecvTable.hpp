#pragma once
class RecvTable
{
public:
	RecvProp* m_pProps;
	int				        m_nProps;
	CRecvDecoder* m_pDecoder;
	char* m_pNetTableName;
	bool			        m_bInitialized;
	bool			        m_bInMainList;
};