#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

namespace L
{
	enum class LogMode : int
	{
		None = 0,
		File,
		Console,
		Both,
	};
	constexpr LogMode OutputMode = LogMode::Console;
	constexpr LogMode VerboseMode = LogMode::File;
	extern std::string FilePath;
	extern std::ofstream File;

	extern void Init();
	extern void Log(const char* txt, const char* end = "\n", bool flush = true);
	extern __forceinline void Verbose(const char* txt, const char* end = "\n", bool flush = true);
	extern void Free();
}