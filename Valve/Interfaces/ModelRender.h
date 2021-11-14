#pragma once
class ModelRender
{
public:
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, ForcedMaterialOverride, 1, (Material* mat), (Material*, int, int), (mat, 0, 0));
};