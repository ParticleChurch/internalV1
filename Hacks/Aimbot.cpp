#include "../Include.hpp"

Aimbot* aimbot = new Aimbot();

Vec Aimbot::CalculateAngle(Vec Target)
{
	Vec EyePos = G::Localplayer->GetEyePos();//starting head position

	static float Distance;	//Total distance between target and starting head position
	Distance = sqrtf(pow(EyePos.x - Target.x, 2) + pow(EyePos.y - Target.y, 2) + pow(EyePos.z - Target.z, 2));

	static float Height;	//total difference in height between target and starting head position
	Height = EyePos.z - Target.z;

	static Vec Angle;
	Angle.x = RAD2DEG(asin(Height / Distance));									//yaw angle
	Angle.y = RAD2DEG(atan2(EyePos.y - Target.y, EyePos.x - Target.x)) - 180;	//pitch angle

	Angle.Normalize();	//making sure the angles are proper
	return Angle;
}

float Aimbot::CrosshairDist(Vec TargetAngle)
{
	static Vec cur;
	cur = G::CM_StartAngle;
	cur.Normalize();
	cur += G::Localplayer->GetAimPunchAngle();
	cur.Normalize();

	//turn to 0-360 degrees
	TargetAngle.y += 180;
	cur.y += 180;

	//finds both angles and chooses shortest yaw angle delta
	float a = fabsf(TargetAngle.y - cur.y);
	float b = 360 - fabsf(TargetAngle.y - cur.y);

	//gets the pitch angle delta 
	float c = abs(TargetAngle.x - cur.x);

	//gets the total distance (in degrees)
	return sqrtf(powf(min(a, b), 2) + powf(c, 2));
}
