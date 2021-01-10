#include "Include.hpp"

void Init()
{
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
    GUI2::LoadProgress = 0.1f;
    Config::Init();
    GUI2::LoadProgress = 0.15f;
    H::Init();
    GUI2::LoadProgress = 1.f;

    I::engine->ClientCmd_Unrestricted("echo Cheat Injected");

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