#pragma once
#include <string>
#include <Windows.h>

namespace ConfigManager
{
	struct ConfigMetaData
	{
		std::string Name = "Particle Config";
		std::string Value = "";
	};

	extern std::string GetFile();
}