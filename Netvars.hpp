#pragma once

namespace N {
	extern std::vector<RecvTable*> Tables;

	extern std::vector<RecvTable*> GetTables();
	RecvTable* GetTable(std::vector<RecvTable*> Tables, const char* TableName);
	extern int GetOffset(std::vector<RecvTable*> Tables, const char* TableName, const char* PropName);
	extern int GetOffset(const char* TableName, const char* PropName);
	int GetRecvProp(std::vector<RecvTable*> Tables, const char* TableName, const char* PropName, RecvProp** Prop = 0);
	int GetRecvProp(std::vector<RecvTable*> Tables, RecvTable* RecvTable, const char* PropName, RecvProp** Prop = 0);
	extern std::string DumpTable();
	extern std::string DumpTable(RecvTable* Table, int Depth);
}