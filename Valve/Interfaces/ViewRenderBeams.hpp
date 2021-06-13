#pragma once
struct BeamInfo {
	int	type;
	Entity* startEnt;
	int startAttachment;
	Entity* endEnt;
	int	endAttachment;
	Vec start;
	Vec end;
	int modelIndex;
	const char* modelName;
	int haloIndex;
	const char* haloName;
	float haloScale;
	float life;
	float width;
	float endWidth;
	float fadeLength;
	float amplitude;
	float brightness;
	float speed;
	int	startFrame;
	float frameRate;
	float red;
	float green;
	float blue;
	bool renderable;
	int segments;
	int	flags;
	Vec ringCenter;
	float ringStartRadius;
	float ringEndRadius;
};

struct Beam {
	std::byte pad[52];
	int flags;
	std::byte pad2[144];
	float die;
};

class ViewRenderBeams {
public:

	//VIRTUAL_METHOD(Beam*, createBeamPoints, WIN32_LINUX(12, 9), (BeamInfo& beamInfo), (this, std::ref(beamInfo)))

	Beam* createBeamPoints(BeamInfo& beamInfo)
	{
		typedef Beam* (__thiscall* oCreateBeamPoints)(void*, BeamInfo&);
		return GetVFunc<oCreateBeamPoints>(this, 12)(this, beamInfo);
	}


};