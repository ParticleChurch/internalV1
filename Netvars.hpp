#pragma once

namespace N {
	extern std::vector<RecvTable*> Tables;

	extern void Init();
	extern std::vector<RecvTable*> GetTables();
	extern RecvTable* GetTable(std::vector<RecvTable*> Tables, const char* TableName);
	extern int GetOffset(std::vector<RecvTable*> Tables, const char* TableName, const char* PropName);
	extern int GetOffset(const char* TableName, const char* PropName);
	extern int GetRecvProp(std::vector<RecvTable*> Tables, const char* TableName, const char* PropName, RecvProp** Prop = 0);
	extern int GetRecvProp(std::vector<RecvTable*> Tables, RecvTable* RecvTable, const char* PropName, RecvProp** Prop = 0);
	extern std::string DumpTable();
	extern std::string DumpTable(RecvTable* Table, int Depth);

	extern std::string Dump();
	extern RecvProp* FindRecvPropByPath(std::vector<std::string> path /* ex: {"DT_CSPlayer", "DT_CSLocalPlayerExclusive", "m_vecOrigin"} */);
}