#pragma once
class RenderContext;
class KeyValues;

class MaterialSystem {
public:
    DECLARE_VIRTUAL_METHOD(Material*, CreateMaterial, 83, (const char* materialName, KeyValues* keyValues), (materialName, keyValues));
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(Material*, FindMaterial, 84, (const char* materialName), (char const*, const char*, bool, const char*), (materialName, nullptr, true, nullptr));
    DECLARE_VIRTUAL_METHOD(short, FirstMaterial, 86, (), ());
    DECLARE_VIRTUAL_METHOD(short, NextMaterial, 87, (short handle), (handle));
    DECLARE_VIRTUAL_METHOD(short, InvalidMaterial, 88, (), ());
    DECLARE_VIRTUAL_METHOD(Material*, GetMaterial, 89, (short handle), (handle));
    DECLARE_VIRTUAL_METHOD(RenderContext*, GetRenderContext, 115, (), ());
};