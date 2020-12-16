#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>

namespace VK
{
	constexpr int VK_INVALID = 0;

	struct KeyInfo {
		int code;
		const char* name;
	};
	extern std::vector<KeyInfo> Keys;

	extern std::string GetName(int code);
	extern int GetCode(const char* name);
};