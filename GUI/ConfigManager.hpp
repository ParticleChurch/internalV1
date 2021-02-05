#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "../Include.hpp"
#include "ConfigConstants.hpp"

namespace ConfigManager
{

	struct ConfigData
	{
		bool Removable = true;
		std::string Name = "Particle Config";
		std::string Value = "";
	};
	extern std::vector<ConfigData> Configs;

	extern std::string GetOpenFile();
	extern std::string GetSaveFile();
	extern DWORD WINAPI ImportThreaded(void* _);
	extern DWORD WINAPI ExportThreaded(void* _);

	extern void Import();
	extern void Export();
	extern void Load(ConfigData cdata);
	extern bool Remove(size_t index);
}