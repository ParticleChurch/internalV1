#pragma once
struct ConVar {
    float GetFloat() noexcept
    {
        typedef float(__thiscall* ogetFloat)(void*);
        return GetVFunc<ogetFloat>(this, 12)(this);
    }
    int GetInt() noexcept
    {
        typedef float(__thiscall* ogetInt)(void*);
        return GetVFunc<ogetInt>(this, 13)(this);
    }

    void SetValue(const char* val) noexcept
    {
        typedef void(__thiscall* osetValue)(void*, const char*);
        return GetVFunc<osetValue>(this, 14)(this, val);
    }
    void SetValue(float val) noexcept
    {
        typedef void(__thiscall* osetValue)(void*, float);
        return GetVFunc<osetValue>(this, 15)(this, val);
    }
    void SetValue(int val) noexcept
    {
        typedef void(__thiscall* osetValue)(void*, int);
        return GetVFunc<osetValue>(this, 16)(this, val);
    }

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
    ConVar* FindVar(const char* name) noexcept
    {
        typedef ConVar* (__thiscall* ofinVar)(void*, const char*);
        return GetVFunc<ofinVar>(this, 15)(this, name);
    }
};