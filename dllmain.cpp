#include "Include.hpp"
HMODULE G::DLLModule = NULL; // gets set later, satisfy `extern` now

void Init()
{
    //Allocate Console
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    std::cout << "Successfully Injected\n";

    H::Init();
    while (!H::Dead) Sleep(100);

    std::cout << "Ejecting...\n";
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