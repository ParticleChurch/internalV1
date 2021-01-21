#include "Include.hpp"

template<typename ...Args>
void ConsoleColorMsg(const Color& color, const char* fmt, Args ...args)
{
    using ConColorMsg = void(*)(const Color&, const char*, ...);
    static ConColorMsg con_color_msg = nullptr;
    if (!con_color_msg) {
        con_color_msg = reinterpret_cast<ConColorMsg>(GetProcAddress(
            GetModuleHandleA("tier0.dll"),
            "?ConColorMsg@@YAXABVColor@@PBDZZ")
            );
    }

    con_color_msg(color, fmt, args...);
}

void Init()
{
    I::engine->ClientCmd_Unrestricted("toggleconsole");
    I::engine->ClientCmd_Unrestricted("clear");
    ConsoleColorMsg(Color(255, 128, 0, 255), "\n\nInjecting...\n");
    GUI2::LoadProgress = 0.f;

    if (GUI2::ConsoleOutput)
    {
        AllocConsole();
        FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
        freopen_s(&fpstdin, "CONIN$", "r", stdin);
        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
        freopen_s(&fpstderr, "CONOUT$", "w", stderr);
    }

    std::cout << "Successfully Injected\n";
    std::cout << N::DumpTable() << std::endl;

    GUI2::LoadProgress = 0.05f;
    G::PatternConvarInit();
    ConsoleColorMsg(Color(0, 255, 0, 255), "Patterns Found!\n");
    GUI2::LoadProgress = 0.1f;
    Config::Init();
    ConsoleColorMsg(Color(0, 255, 0, 255), "Config Created!\n");
    Config2::Init();
    GUI2::LoadProgress = 0.15f;
    H::Init();
    GUI2::LoadProgress = 1.f;

    ConsoleColorMsg(Color(50, 153, 255, 255), "Cheat Injected\n\n");
    I::engine->ClientCmd_Unrestricted("toggleconsole");

    std::cout << "Initialization successful" << std::endl;
    while (!G::KillDLL) Sleep(100);
    std::cout << "\nEjecting...\n";

    FreeConsole();
    FreeLibraryAndExitThread(G::DLLModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        G::DLLModule = hModule;
        CreateThread(0, 0, LPTHREAD_START_ROUTINE(Init), 0, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}