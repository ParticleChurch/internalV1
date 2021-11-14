#pragma once
#include "../../Utils/VMT.hpp"

class IClientNetworkable {
public:
	DECLARE_VIRTUAL_METHOD(void, PreDataUpdate, 6, (int updateType), (updateType));
};