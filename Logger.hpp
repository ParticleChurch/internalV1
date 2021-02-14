#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

namespace L
{
	enum class LogMode
	{
		None = 0,
		File,
		Console,
		Both,
	};
	constexpr LogMode Mode = LogMode::Both;
	constexpr bool VerboseOutputEnabled = true;
	extern std::string FilePath;

	extern void Init();
	extern void Log(const char* txt, const char* end = "\n", bool flush = true);
	extern __forceinline void Verbose(const char* txt, const char* end = "\n", bool flush = true);
	extern void Free();
}