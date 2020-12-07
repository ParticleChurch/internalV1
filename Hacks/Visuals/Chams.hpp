#pragma once
class Chams
{
private:
	void OverideMat(bool ignorez, int material, float transparent, Color rgba, void* thisptr, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);
	Material* flat;
	Material* normal;
	Material* animated;
	Material* glass;
	Material* crystal;
	Material* chrome;
	Material* pearlescent;

	Material* GetCurMaterial(int index)
	{
		switch (index) {
		case 0:
			return normal;
			break;
		case 1:
			return flat;
			break;
		case 2:
			return animated;
			break;
		case 3:
			return glass;
			break;
		case 4:
			return crystal;
			break;
		case 5:
			return chrome;
			break;
		case 6:
			return pearlescent;
			break;
		default:
			return flat;
			break;
		}
	}
public:
	void Init();
	void Run(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);
};

extern Chams* chams;