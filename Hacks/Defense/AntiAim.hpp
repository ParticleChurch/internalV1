#pragma once
class AntiAim
{
public:
	//functions used for all
	bool LBYBreak();
	//functions for legit
	void legit();
	bool legit_left = true;
	int legit_side = 1;
	//functions for rage
	void rage();
	bool rage_left = true;
	int rage_side = 1;

	//Other variables/functions
	QAngle real;
	QAngle fake;
	Vec FakeAbsOrigin;
	Matrix3x4 FakeMatrix[128];

	float HEADEDGE = 70.f;
	bool GetBestHeadAngle(QAngle& angle);

	void Visualize();
};

extern AntiAim* antiaim;