#include "Logger.hpp"

namespace L
{
	std::string FilePath = "particle.log";
	std::ofstream File;
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;

	constexpr bool MustAllocateConsole =
		OutputMode == LogMode::Console || OutputMode == LogMode::Both ||
		VerboseMode == LogMode::Console || VerboseMode == LogMode::Both;
	constexpr bool MustOpenFile =
		OutputMode == LogMode::File || OutputMode == LogMode::Both ||
		VerboseMode == LogMode::File || VerboseMode == LogMode::Both;
}

void L::Init()
{

	if (MustAllocateConsole)
	{
		AllocConsole();
		freopen_s(&fpstdin, "CONIN$", "r", stdin);
		freopen_s(&fpstdout, "CONOUT$", "w", stdout);
		freopen_s(&fpstderr, "CONOUT$", "w", stderr);
	}

	if (MustOpenFile)
	{
		char exe_file[MAX_PATH];
		GetModuleFileName(NULL, exe_file, MAX_PATH);
		std::string csgo_directory(exe_file, strlen(exe_file) - strlen("csgo.exe"));

		File = std::ofstream(csgo_directory + FilePath, std::ios::trunc);
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
	constexpr bool ConsoleOutput = OutputMode == LogMode::Console || OutputMode == LogMode::Both;
	constexpr bool FileOutput = OutputMode == LogMode::File || OutputMode == LogMode::Both;

	if (ConsoleOutput)
	{
		std::cout << txt << end;
		if (flush) std::cout << std::flush;
	}

	if (FileOutput)
	{
		File << txt << end;
		if (flush)
			File.flush();
	}
}

__forceinline void L::Verbose(const char* txt, const char* end, bool flush)
{
	constexpr bool ConsoleOutput = VerboseMode == LogMode::Console || VerboseMode == LogMode::Both;
	constexpr bool FileOutput = VerboseMode == LogMode::File || VerboseMode == LogMode::Both;

	if (ConsoleOutput)
	{
		std::cout << txt << end;
		if (flush) std::cout << std::flush;
	}

	if (FileOutput)
	{
		File << txt << end;
		if (flush)
			File.flush();
	}
}

void L::Free()
{
	if (MustAllocateConsole)
	{
		HWND Console = GetConsoleWindow();
		FreeConsole();
		if (Console)
		{
			SendMessage(Console, WM_CLOSE, NULL, NULL);
		}
	}
	
	if (MustOpenFile)
	{
		File.close();
	}
}
