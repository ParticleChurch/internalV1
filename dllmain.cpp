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
    srand(static_cast <unsigned> (time(0)));
    GUI2::LoadProgress = 0.f;

    L::Init();    L::Log("DLLMain executed");
    Config::Init();  L::Log("Config::Init() complete");
    Config2::Init(); L::Log("Config2::Init() complete");
    I::GUIInit(); L::Log("I::GUIInit() complete");
    G::GUIInit(); L::Log("G::GUIInit() complete");
    H::GUIInit(); L::Log("H::GUIInit() complete"); // hooks endscene, gui will now show up
    GUI2::LoadProgress = 0.05f;

    I::Init();       L::Log("I::Init() complete");
    N::Init();       L::Log("N::Init() complete");
    G::Init();       L::Log("G::Init() complete");
    H::Init();       L::Log("H::Init() complete");

    GUI2::LoadProgress = 1.f;

    L::Log("DLLMain complete. Now waiting for ejection");
    while (!G::KillDLL) Sleep(100);
    L::Log("Ejecting...");

    H::UnHook(); L::Log("H::UnHook(); complete");
    H::Free();   L::Log("H::Free(); complete");

    L::Log("Freeing logger and FreeLibraryAndExitThread");
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