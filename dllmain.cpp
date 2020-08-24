#include "Include.hpp"

/* this is a comment (test git) */

/* this is another comment (other user test git) */

void Init()
{
    //Allocate Console
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    std::cout << "Successfully Injected\n";

    //ShellExecute(NULL, TEXT("open"), TEXT("https://www.youtube.com/watch?v=oHg5SJYRHA0"), NULL, NULL, 0);

    H::Init();

    I::engine->ClientCmd_Unrestricted("echo Cheat Injected");

    while (!G::KillDLL) Sleep(100);
    std::cout << "\nEjecting...\n";

    H::Eject();
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