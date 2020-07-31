#pragma once
//custom
class Vec;
class Color; 
class Matrix3x4;

//SDK
class CUserCmd;
struct player_info_t;
//recv
struct RecvProp;
class RecvTable;
class CRecvProxyData;
class CRecvDecoder;
typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);