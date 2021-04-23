#pragma once
#include "../../Utils/GetVFunc.hpp"

class IClientNetworkable {
public:
	void PreDataUpdate(int updateType)
	{
		typedef void(__thiscall* PreDataUpdateFn)(void*, int);
		static PreDataUpdateFn oPreDataUpdate = GetVFunc<PreDataUpdateFn>(this, 6);
		return oPreDataUpdate(this, updateType);
	}
};