#pragma once
class AntiAim
{
public:
	//functions used for all
	bool LBYBreak();
	//functions for legit
	void legit();
	bool Left = true;
	int side = 1;
	//functions for rage
	void rage();

	//Other variables/functions
	QAngle real;
	QAngle fake;

	void Visualize();
};

extern AntiAim* antiaim;