#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

namespace Keybind
{
	constexpr size_t nKeys = 91;
	extern int KeyMap[];
	extern bool KeyState[];
	extern std::vector<void*> Binds[];
	extern std::string KeyNames[];

	extern int ReverseKeyMap(int KeyCode);
}