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
	extern std::vector<void*> Binds[];
	extern std::string KeyNames[];
	extern bool Lock;
	extern bool UpdatorRunning;
	extern bool* WantMouseCapture;

	struct KeyLogEntry
	{
		int Key;
		int State; // 1 = pressed, 0 = released
		bool WantCaptureMouse;

		KeyLogEntry(int key, int state, bool wantmouse)
		{
			this->Key = key;
			this->State = state;
			this->WantCaptureMouse = wantmouse;
		}
	};
	extern std::vector<KeyLogEntry> KeyChangeStack; // most recent entry last

	extern void ForceUpdate();
	extern void Init(bool* EjectSignal, bool* ImGuiWantCaptureMouse);
}