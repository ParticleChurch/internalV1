#pragma once
//custom
class Vec;
class Color; 
class Matrix3x4;
class Entity; //see globals.hpp for where its included

//SDK
class CUserCmd;
struct player_info_t;
//recv
struct RecvProp;
class RecvTable;
class CRecvProxyData;
class CRecvDecoder;
typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);
//clientclass
class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

//Interfaces
class ClientMode;
class IBaseClientDLL;