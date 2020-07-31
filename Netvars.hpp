#pragma once

namespace N {
	extern std::vector<RecvTable*> tables;

	extern std::vector<RecvTable*> getTables();
	RecvTable* GetTable(std::vector<RecvTable*> tables, const char* tableName);
	extern int GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char* propName);
	extern int GetOffset(const char* tableName, const char* propName);
	int GetRecvProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop = 0);
	int GetRecvProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop = 0);
}