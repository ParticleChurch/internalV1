#pragma once
struct ConVar {
    DECLARE_VIRTUAL_METHOD(float, GetFloat, 12, (), ());
    DECLARE_VIRTUAL_METHOD(int, GetInt, 13, (), ());
    DECLARE_VIRTUAL_METHOD(void, SetValue, 14, (const char* val), (val));
    DECLARE_VIRTUAL_METHOD(void, SetValue, 15, (float val), (val));
    DECLARE_VIRTUAL_METHOD(void, SetValue, 16, (int val), (val));

    std::byte pad[24];
    std::add_pointer_t<void __cdecl()> changeCallback;
    ConVar* parent;
    const char* defaultValue;
    char* string;
    std::byte pad1[28];
    UtlVector<void __cdecl()> onChangeCallbacks;
};

class CVar {
public:
    DECLARE_VIRTUAL_METHOD(ConVar*, FindVar, 15, (const char* name), (name));
};