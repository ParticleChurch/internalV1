#pragma once
class MiscVisuals
{
private:
	// Thirdperson Functions/Vars
	Vec GetIdealCameraPos(float distance);
	float GetCameraBoomLength(float distance);
public:
	// Thirdperson Function/Vars
	void ThirdPerson_hkCamToFirstPeron();
	void ThirdPerson_DoPostScreenEffects();
	void RankRevealer();
	void GrenadePrediction();

};

extern MiscVisuals* miscvisuals;