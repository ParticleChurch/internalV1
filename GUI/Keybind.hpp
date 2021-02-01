#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

namespace Keybind
{
	constexpr int nKeys = 91;
	extern int KeyMap[];
	extern bool KeyState[];
	extern std::string KeyNames[];
	extern bool Lock;

	struct KeyLogEntry
	{
		int Key;
		bool State;

		KeyLogEntry(int key, bool state)
		{
			this->Key = key;
			this->State = state;
		}
	};
	extern std::vector<KeyLogEntry> KeyChangeStack; // most recent entry last

	extern void ForceUpdate();
	extern void Init(bool* EjectSignal, bool* ThreadClosedSignal);
}