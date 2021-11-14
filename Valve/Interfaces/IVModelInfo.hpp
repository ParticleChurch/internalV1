#pragma once
class IVModelInfo
{
public:
	DECLARE_VIRTUAL_METHOD(int, GetModelIndex, 2, (const char* name), (name));
	DECLARE_VIRTUAL_METHOD(studiohdr_t*, GetStudioModel, 32, (const Model* model), (model));
};