#pragma once

class MaterialVar {
public:
    DECLARE_VIRTUAL_METHOD(void, SetValue, 4, (float value), (value));
    DECLARE_VIRTUAL_METHOD(void, SetVectorValue, 11, (float r, float g, float b), (r, g, b));
};

enum class MaterialVarFlag {
    NO_DRAW = 1 << 2,
    IGNOREZ = 1 << 15,
    WIREFRAME = 1 << 28
};

class Material {
public:
    DECLARE_VIRTUAL_METHOD(const char*, GetName, 0, (), ());
    DECLARE_VIRTUAL_METHOD(const char*, GetTextureGroupName, 1, (), ());
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(MaterialVar*, FindVar, 11, (const char* name), (const char*, bool*, bool), (name, nullptr, false));
    DECLARE_VIRTUAL_METHOD(void, IncrementReferenceCount, 12, (), ());
    DECLARE_VIRTUAL_METHOD(void, AlphaModulate, 27, (float alpha), (alpha));
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, ColorModulate, 28, (const float rgb[3]), (float, float, float), (rgb[0], rgb[1], rgb[2]));
    DECLARE_VIRTUAL_METHOD(void, ColorModulate, 28, (float r, float g, float b), (r, g, b));

    //for our color class
    void ColorModulate(Color a) noexcept
    {
        ColorModulate(a.r()/255.f, a.g()/255.f, a.b()/255.f);
    }

    DECLARE_VIRTUAL_METHOD(void, SetMaterialVarFlag, 29, (MaterialVarFlag flag, bool on), (flag, on));
    DECLARE_VIRTUAL_METHOD(bool, IsPrecached, 70, (), ());
};