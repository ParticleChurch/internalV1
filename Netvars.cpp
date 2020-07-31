#include "Include.hpp"

namespace N {
	std::vector<RecvTable*> TABLES = getTables();

	std::vector<RecvTable*> getTables()
	{
		std::vector<RecvTable*> tables;

		ClientClass* clientClass = I::client->GetAllClasses();
		if (!clientClass)
			return std::vector<RecvTable*>();

		while (clientClass)
		{
			RecvTable* recvTable = clientClass->m_pRecvTable;
			tables.push_back(recvTable);

			clientClass = clientClass->m_pNext;
		}

		return tables;
	}
	RecvTable* GetTable(std::vector<RecvTable*> tables, const char* tableName)
	{
		if (tables.empty())
			return NULL;

		for (unsigned long i = 0; i < tables.size(); i++)
		{
			RecvTable* table = tables[i];

			if (!table)
				continue;

			if (_strnicmp(table->m_pNetTableName, tableName, 128) == 0)
				return table;
		}

		return NULL;
	}
	int GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char* propName)
	{
		int offset = GetRecvProp(tables, tableName, propName);
		if (!offset)
			return 0;

		return offset;
	}

	int GetOffset(const char* tableName, const char* propName)
	{
		return GetOffset(TABLES, tableName, propName);
	}

	int GetRecvProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop)
	{
		RecvTable* recvTable = GetTable(tables, tableName);
		if (!recvTable)
			return 0;

		int offset = GetRecvProp(tables, recvTable, propName, prop);
		if (!offset)
			return 0;

		return offset;
	}

	int GetRecvProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop)
	{
		int extraOffset = 0;

		for (int i = 0; i < recvTable->m_nProps; ++i) {
			RecvProp* recvProp = &recvTable->m_pProps[i];
			RecvTable* child = recvProp->m_pDataTable;

			if (child && (child->m_nProps > 0))
			{
				int tmp = GetRecvProp(tables, child, propName, prop);
				if (tmp)
					extraOffset += (recvProp->m_Offset + tmp);
			}

			if (_strnicmp(recvProp->m_pVarName, propName, 128))
				continue;

			if (prop)
				*prop = recvProp;

			return (recvProp->m_Offset + extraOffset);
		}

		return extraOffset;
	}
}
