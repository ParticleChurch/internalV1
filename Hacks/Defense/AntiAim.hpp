#pragma once
class AntiAim
{
private:
	// For Manual Rage
	void ManualRage();
	// For Custom Rage
	void CustomRage();
	// Sets the YawBase
	void SetYawBase();
public:
	//functions that can be used for all
	bool LBYBreak();
	float AtCrosshair();
	float AtDistance();
	float AtFreestanding(); // crapy lol

	//Other variables/functions
	QAngle real;
	QAngle fake;
	Vec FakeAbsOrigin;
	Matrix3x4 FakeMatrix[128];

	// New Stuff for rage
	bool Invert = false;
	void rage();
	
	float HEADEDGE = 20.f;
	bool GetBestHeadAngle(QAngle& angle);
	
	//functions for legit
	void legit();
	bool legit_left = true;
	int legit_side = 1;

	// misc
	void Visualize();
};

extern AntiAim* antiaim;