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

    L::Init();    L::Log("DLLMain executed");

    Config::Init(); L::Log("Config::Init() complete");
    I::GUIInit(); L::Log("I::GUIInit() complete");
    G::GUIInit(); L::Log("G::GUIInit() complete");
    H::GUIInit(); L::Log("H::GUIInit() complete"); // hooks endscene, gui will now show up
    GUI::LoadProgress = 0.05f;

    I::Init();           L::Log("I::Init() complete");
    SkinChanger::Hook(); L::Log("SkinChanger::Hook() complete");
    N::Init();           L::Log("N::Init() complete");
    G::Init();           L::Log("G::Init() complete");
    H::Init();           L::Log("H::Init() complete");
    UserData::ConnectAPI();   L::Log("UserData::ConnectAPI() complete");
    GUI::LoadProgress = 1.f;
    //L::Log(N::Dump().c_str());

    L::Log("DLLMain complete. Now waiting for ejection");


    /*
    for (size_t i = 0; i < 2048; i++)
    {
        std::string byte = std::to_string((uint32_t)(functionOfInterest[i]));
        while (byte.length() < 3)
            byte = "0" + byte;
        L::Log(("\\" + byte).c_str(), "");
    }
    L::Log("");
    */

    struct { const volatile unsigned char* ptr; const size_t sz; uint32_t initialHash = 0; } hashables[] = {
        {(const volatile unsigned char*)UserData::AttemptLogin, 0x6a4},
        {(const volatile unsigned char*)UserData::GetUnauthenticatedSession, 0x461},
        {(const volatile unsigned char*)UserData::PingServer, 0x337},
        {(const volatile unsigned char*)UserData::ConnectAPI, 0x170e},
    };

    while (!G::KillDLL)
    {
        for (size_t i = 0; i < sizeof(hashables) / sizeof(hashables[0]); i++)
        {
            auto& h = hashables[i];
            uint32_t currentHash = StrHash::HashRuntime((const char*)h.ptr, h.sz);
            if (h.initialHash == 0)
            {
                h.initialHash = currentHash;
            }
            else if (h.initialHash != currentHash)
            {
                L::Log("HASH MISMATCH!!!");
            }
        }
        Sleep(100);
    }
    L::Log("Ejecting...");

    SkinChanger::UnHook(); L::Log("SkinChanger::UnHook(); complete");
    H::UnHook();           L::Log("H::UnHook(); complete");
    H::Free();             L::Log("H::Free(); complete");

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