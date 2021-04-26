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

	// Others
	void RankRevealer();
	void GrenadePrediction();
	void NoScope();
	void NoFlash();
	void NoSmoke_DoPostScreenEffects();
	void NoSmokeFSN();
	void ChangeViewModel();
	void ForceCrosshair();

};

extern MiscVisuals* miscvisuals;