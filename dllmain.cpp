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
    GUI2::LoadProgress = 0.f;
    srand(static_cast <unsigned> (time(0)));

    L::Init();
    L::Log("Entry point executed");
    GUI2::LoadProgress = 0.05f;

    L::Log("Finding convars");
    G::PatternConvarInit();
    GUI2::LoadProgress = 0.1f;

    L::Log("Initializing configs");
    Config::Init();
    Config2::Init();
    GUI2::LoadProgress = 0.15f;

    L::Log("Initializing hooks");
    H::Init();
    GUI2::LoadProgress = 0.85f;

    //L::Log(N::DumpTable().c_str());

    GUI2::LoadProgress = 1.f;

    L::Log("DLLMain complete. Now waiting for ejection");
    while (!G::KillDLL) Sleep(100);

    L::Log("Unhooking hooks...");
    while (!H::UnHooked) Sleep(100);

    L::Log("Freeing DLL");
    L::Free();
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