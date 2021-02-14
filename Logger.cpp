#include "Logger.hpp"

namespace L
{
	std::string FilePath = "particle.log";
	std::ofstream File;
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
}

void L::Init()
{
	if (Mode == LogMode::Console || Mode == LogMode::Both)
	{
		AllocConsole();
		freopen_s(&fpstdin, "CONIN$", "r", stdin);
		freopen_s(&fpstdout, "CONOUT$", "w", stdout);
		freopen_s(&fpstderr, "CONOUT$", "w", stderr);
	}

	if (Mode == LogMode::File || Mode == LogMode::Both)
	{
		File = std::ofstream(FilePath, std::ios::trunc);
		if (!File.is_open())
		{
			// lmao
			Log("!!!!!!!!ATTENTION!!!!!!!!");
			Log("FAILED TO OPEN LOG FILE !!!!!!!!!");
		}
	}
}

void L::Log(const char* txt, const char* end, bool flush)
{
	// since Mode is constexpr, hopefully the compiler will realize that
	// it can ignore all calls to this function, and not store string literals
	if (Mode == LogMode::None) return;

	switch (Mode)
	{
	case LogMode::File:
		File << txt << end;
		if (flush)
			File.flush();
		return;
	case LogMode::Console:
		std::cout << txt << end;
		if (flush)
			std::cout << std::flush;
		return;
	case LogMode::Both:
		File << txt << end;
		if (flush)
			File.flush();

		std::cout << txt << end;
		if (flush)
			std::cout << std::flush;
		return;
	}
}

__forceinline void L::Verbose(const char* txt, const char* end, bool flush)
{
	if (VerboseOutputEnabled)
		Log(txt, end, flush);
}

void L::Free()
{
	if (Mode == LogMode::Console)
	{
		HWND Console = GetConsoleWindow();
		FreeConsole();
		if (Console)
		{
			SendMessage(Console, WM_CLOSE, NULL, NULL);
		}
	}
	else if (Mode == LogMode::File)
	{
		File.close();
	}
	else if (Mode == LogMode::Both)
	{
		HWND Console = GetConsoleWindow();
		FreeConsole();
		if (Console)
		{
			SendMessage(Console, WM_CLOSE, NULL, NULL);
		}
		File.close();
	}
}
