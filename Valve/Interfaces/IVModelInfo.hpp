#pragma once
class IVModelInfo
{
public:

	int GetModelIndex(const char* name)
	{
		typedef int(__thiscall* oGetModelIndex)(void*, const char*);
		return GetVFunc< oGetModelIndex >(this, 2)(this, name);
	}

	studiohdr_t* GetStudioModel(const Model* model)
	{
		typedef studiohdr_t*(__thiscall* oGetStudioModel)(void*, const Model*);
		return GetVFunc< oGetStudioModel >(this, 32)(this, model);
	}
};