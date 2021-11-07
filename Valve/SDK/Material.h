#pragma once
#include <tuple>

class MaterialVar {
public:
    constexpr auto SetValue(float value) noexcept
    {
        typedef void(__thiscall* osetValue)(void*, float);
        return VMT::GetVirtualMethod< osetValue >(this, 4)(this, value);
    }
    constexpr auto SetVectorValue(float r, float g, float b) noexcept
    {
        typedef void(__thiscall* osetVectorValue)(void*, float, float, float);
        return VMT::GetVirtualMethod< osetVectorValue >(this, 11)(this, r, g, b);
    }
};

enum class MaterialVarFlag {
    NO_DRAW = 1 << 2,
    IGNOREZ = 1 << 15,
    WIREFRAME = 1 << 28
};

class Material {
public:
    constexpr auto GetName() noexcept
    {
        typedef const char* (__thiscall* ogetName)(void*);
        return VMT::GetVirtualMethod< ogetName >(this, 0)(this);
    }

    constexpr auto GetTextureGroupName() noexcept
    {
        typedef const char* (__thiscall* ogetTextureGroupName)(void*);
        return VMT::GetVirtualMethod< ogetTextureGroupName >(this, 1)(this);
    }

    constexpr auto FindVar(const char* name) noexcept
    {
        typedef MaterialVar* (__thiscall* ofindVar)(void*, const char*, bool*, bool);
        return VMT::GetVirtualMethod< ofindVar >(this, 11)(this, name, nullptr, false);
    }

    constexpr int IncrementReferenceCount() noexcept
    {
        typedef void(__thiscall* ogetTextureGroupName)(void*);
        VMT::GetVirtualMethod< ogetTextureGroupName >(this, 12)(this);
        return 0;
    }


    constexpr void AlphaModulate(float alpha) noexcept
    {
        typedef void(__thiscall* oalphaModulate)(void*, float);
        return VMT::GetVirtualMethod< oalphaModulate >(this, 27)(this, alpha);
    }

    constexpr void ColorModulate(const float color[3]) noexcept
    {
        typedef void(__thiscall* ocolorModulate)(void*, float, float, float);
        return VMT::GetVirtualMethod< ocolorModulate >(this, 28)(this, color[0], color[1], color[2]);
    }

    constexpr void ColorModulate(float r, float g, float b) noexcept
    {
        typedef void(__thiscall* ocolorModulate)(void*, float, float, float);
        return VMT::GetVirtualMethod< ocolorModulate >(this, 28)(this, r, g, b);
    }

    constexpr void ColorModulate(const std::tuple<float, float, float>& color) noexcept
    {
        typedef void(__thiscall* ocolorModulate)(void*, float, float, float);
        return VMT::GetVirtualMethod< ocolorModulate >(this, 28)(this, std::get<0>(color), std::get<1>(color), std::get<2>(color));
    }

    //for our color class
    void ColorModulate(Color a) noexcept
    {
        ColorModulate(a.r()/255.f, a.g()/255.f, a.b()/255.f);
    }

    constexpr void SetMaterialVarFlag(MaterialVarFlag flag, bool on) noexcept
    {
        typedef void(__thiscall* osetMaterialVarFlag)(void*, MaterialVarFlag, bool);
        return VMT::GetVirtualMethod< osetMaterialVarFlag >(this, 29)(this, flag, on);
    }

    constexpr auto IsPrecached() noexcept
    {
        typedef bool(__thiscall* oisPrecached)(void*);
        return VMT::GetVirtualMethod< oisPrecached >(this, 70)(this);
    }

};