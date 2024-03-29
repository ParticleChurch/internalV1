#include "Include.hpp"

namespace N {
	std::vector<RecvTable*> Tables;

	void Init()
	{
		Tables = GetTables();
	}

	std::vector<RecvTable*> GetTables()
	{
		std::vector<RecvTable*> Tables;

		ClientClass* ClientClass = I::client->GetAllClasses();
		if (!ClientClass)
			return std::vector<RecvTable*>();

		while (ClientClass)
		{
			RecvTable* recvTable = ClientClass->m_pRecvTable;
			Tables.push_back(recvTable);

			ClientClass = ClientClass->m_pNext;
		}

		return Tables;
	}
	RecvTable* GetTable(std::vector<RecvTable*> Tables, const char* TableName)
	{
		if (Tables.empty())
			return NULL;

		for (unsigned long i = 0; i < Tables.size(); i++)
		{
			RecvTable* table = Tables[i];

			if (!table)
				continue;

			if (_strnicmp(table->m_pNetTableName, TableName, 128) == 0)
				return table;
		}

		return NULL;
	}
	int GetOffset(std::vector<RecvTable*> Tables, const char* TableName, const char* TropName)
	{
		int offset = GetRecvProp(Tables, TableName, TropName);
		if (!offset)
			return 0;

		return offset;
	}

	int GetOffset(const char* TableName, const char* TropName)
	{
		return GetOffset(Tables, TableName, TropName);
	}

	int GetRecvProp(std::vector<RecvTable*> Tables, const char* TableName, const char* PropName, RecvProp** Prop)
	{
		RecvTable* Table = GetTable(Tables, TableName);
		if (!Table)
			return 0;

		int offset = GetRecvProp(Tables, Table, PropName, Prop);
		if (!offset)
			return 0;

		return offset;
	}

	int GetRecvProp(std::vector<RecvTable*> Tables, RecvTable* Table, const char* PropName, RecvProp** Prop)
	{
		int ExtraOffset = 0;

		for (int i = 0; i < Table->m_nProps; ++i) {
			RecvProp* RProp = &Table->m_pProps[i];
			RecvTable* Child = RProp->m_pDataTable;

			if (Child && (Child->m_nProps > 0))
			{
				int tmp = GetRecvProp(Tables, Child, PropName, Prop);
				if (tmp)
					ExtraOffset += (RProp->m_Offset + tmp);
			}

			if (_strnicmp(RProp->m_pVarName, PropName, 128))
				continue;

			if (Prop)
				*Prop = RProp;

			return (RProp->m_Offset + ExtraOffset);
		}

		return ExtraOffset;
	}

	std::string DumpTable(RecvTable* Table, int Depth)
	{
		std::string str;

		std::string pre = "";

		for (int i = 0; i < Depth; i++)
			pre.append("\t");

		str.append(pre);
		str.append(Table->m_pNetTableName);
		str.append("\n");

		for (int i = 0; i < Table->m_nProps; i++) {
			RecvProp* prop = &Table->m_pProps[i];
			if (!prop)
				continue;

			std::string varName(prop->m_pVarName);

			if (varName.find("baseclass") == 0 || varName.find("0") == 0 || varName.find("1") == 0 || varName.find("2") == 0)
				continue;

			str.append("\t");
			str.append(pre);
			str.append(varName);
			str.append("\t");
			str.append(std::to_string(prop->m_Offset));
			str.append("\n");

			if (prop->m_pDataTable)
				str.append(DumpTable(prop->m_pDataTable, Depth + 1));
		}

		return str;
	}
	std::string DumpTable()
	{
		std::string str;
		for (auto a : Tables) {
			str.append("\n");
			str.append(DumpTable(a, 0));
		}
		return str;
	}

	std::string DumpRecvTable(RecvTable* table, std::map<RecvTable*, char>* alreadyPrinted, unsigned int depth = 0)
	{
		std::string padding = "";
		for (unsigned int d = 0; d < depth; d++)
			padding += "    ";

		if (alreadyPrinted->find(table) != alreadyPrinted->end())
		{
			return " -> " + std::string(table->m_pNetTableName) + " [already shown]\n";
		}
		else
		{
			alreadyPrinted->insert(std::pair<RecvTable*, bool>(table, 'Y'));
		}

		std::string str = " -> " + std::string(table->m_pNetTableName) + " {\n";
		std::string prefix = padding + "    ";

		for (int i = 0; i < table->m_nProps; i++)
		{
			RecvProp* pProp = table->m_pProps + i;
			if (!pProp)
			{
				str += prefix + "[NULL]\n";
				continue;
			}

			try {
				// if the var name is an integer, then don't print it
				int x = std::stoi(pProp->m_pVarName);
				continue;
			}
			catch (std::exception&) {}

			str += prefix + "[ " + TextService::cpad(std::to_string(pProp->m_Offset), 6) + " ] " + pProp->m_pVarName;
			if (pProp->m_pDataTable)
				str += DumpRecvTable(pProp->m_pDataTable, alreadyPrinted, depth + 1);
			else
				str += "\n";
		}
		str += padding + "}\n";

		return str;
	}

	std::string Dump()
	{
		std::map<RecvTable*, char>* alreadyPrinted = new std::map<RecvTable*, char>();

		std::string str = "";
		for (ClientClass* pClass = I::client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		{
			str += "CLASS: " + std::string(pClass->m_pNetworkName);
			str += DumpRecvTable(pClass->m_pRecvTable, alreadyPrinted);
		}

		delete alreadyPrinted;
		return str;
	}

	RecvProp* FindRecvPropByPath(std::vector<std::string> path)
	{
		if (path.size() < 2) return nullptr;

		RecvTable* table = nullptr;
		for (ClientClass* pClass = I::client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		{
			if (path[0] == pClass->m_pRecvTable->m_pNetTableName)
			{
				table = pClass->m_pRecvTable;
				break;
			}
		}

		if (!table) return nullptr;
		for (size_t p = 1; p < path.size(); p++)
		{
			std::string& search = path.at(p);
			bool last = p >= path.size() - 1;
			bool found = false;

			for (int i = 0; i < table->m_nProps; i++)
			{
				RecvProp* pProp = table->m_pProps + i;
				if (!pProp)
					continue;

				if (last)
				{
					if (search == pProp->m_pVarName)
						return pProp;
				}
				else if (pProp->m_pDataTable && search == pProp->m_pDataTable->m_pNetTableName)
				{
					table = pProp->m_pDataTable;
					found = true;
					break;
				}
			}

			if (!found)
				return nullptr;
		}
		
		return nullptr;
	}
}
