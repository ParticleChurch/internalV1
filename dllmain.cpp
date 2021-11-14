#include "Include.hpp"
#include <functional>

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
    GUI::LoadProgress = 0.f;

    L::init();
    L::Info("DLLMain executed");

    Config::Init(); L::Info("Config::Init() complete");
    I::GUIInit(); L::Info("I::GUIInit() complete");
    G::GUIInit(); L::Info("G::GUIInit() complete");
    H::GUIInit(); L::Info("H::GUIInit() complete"); // hooks endscene, gui will now show up
    GUI::LoadProgress = 0.05f;

    I::Init(); L::Info("I::Init() complete");
    SkinChanger::Hook(); L::Info("SkinChanger::Hook() complete");
    N::Init(); L::Info("N::Init() complete");
    G::Init(); L::Info("G::Init() complete");
    H::Init(); L::Info("H::Init() complete");
    UserData::ConnectAPI(); L::Info("UserData::ConnectAPI() complete");
    GUI::LoadProgress = 1.f;
    //L::Log(N::Dump().c_str());

    L::Info("DLLMain complete. Now waiting for ejection");

    while (!G::KillDLL)
    {
        Sleep(100);
    }
    L::Info("Ejecting...");

    SkinChanger::UnHook(); L::Info("SkinChanger::UnHook(); complete");
    H::UnHook(); L::Info("H::UnHook(); complete");
    H::Free(); L::Info("H::Free(); complete");

    L::Info("Freeing logger and FreeLibraryAndExitThread");

    L::free();
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