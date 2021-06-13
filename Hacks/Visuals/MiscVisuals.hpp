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
	void DrawBeams(GameEvent* event);
	void NoAimViewPunchFSN(int stage);
	void RankRevealer();
	void GrenadePrediction();
	void NoScope();
	void NoFlash();
	void NoSmoke_DoPostScreenEffects();
	void NoSmokeFSN();
	void ChangeViewModel();
	void ForceCrosshair();
	std::vector<std::string> GetSpectators();
	//keybind visual
	struct KeybindVis
	{
		std::string name;
		bool val;
		KeybindVis(std::string name, bool val)
		{
			this->name = name;
			this->val = val;
		}
	};
	std::vector<KeybindVis> GetKeyBinds();

};

extern MiscVisuals* miscvisuals;