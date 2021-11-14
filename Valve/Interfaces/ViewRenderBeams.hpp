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
	DECLARE_VIRTUAL_METHOD(Beam*, createBeamPoints, 12, (BeamInfo& beamInfo), (beamInfo));
};