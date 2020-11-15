#include "../../Include.hpp"

MiscVisuals* miscvisuals = new MiscVisuals();

Vec MiscVisuals::GetIdealCameraPos(float distance)
{
	Vec PlayerPos = G::Localplayer->GetEyePos();	//pleyer center position
	Vec Ideal = PlayerPos;							//Final ideal angle
	QAngle FPAng = G::StartAngle;				//flipped player angle
	FPAng.x *= -1;
	FPAng.y += 180;
	FPAng.NormalizeAngle();

	Ideal.z += distance * sin(DEG2RAD(FPAng.x));
	float Hz = distance * cos(DEG2RAD(FPAng.x)); //horizonal distance

	Ideal.x += Hz * cos(DEG2RAD(FPAng.y));
	Ideal.y += Hz * sin(DEG2RAD(FPAng.y));

	return Ideal;
}

float MiscVisuals::GetCameraBoomLength(float distance)
{
	Vec IdealCameraPos = GetIdealCameraPos(distance);	//ideal camera position
	Vec PlayerPos = G::Localplayer->GetEyePos();		//player center position

	trace_t Trace;
	Ray_t Ray(PlayerPos, IdealCameraPos);
	CTraceFilter Filter(I::entitylist->GetClientEntity(I::engine->GetLocalPlayer()));
	I::enginetrace->TraceRay(Ray, MASK_ALL, &Filter, &Trace);

	if (Trace.Fraction <= 1)
		return distance * Trace.Fraction * 0.9;
	else
		return distance;
}

void MiscVisuals::ThirdPerson_hkCamToFirstPeron()
{
	if (Config::GetBool("visuals-misc-thirdperson"))
		return;
	H::ohkCamToFirstPeron(I::input);
}

void MiscVisuals::ThirdPerson_DoPostScreenEffects()
{
	if (I::engine->IsInGame() && G::LocalPlayerAlive) {
		if (Config::GetBool("visuals-misc-thirdperson"))
		{
			I::input->m_fCameraInThirdPerson = true;
			I::input->m_vecCameraOffset = Vec(G::StartAngle.x, G::StartAngle.y, GetCameraBoomLength(150.f));
		}
		else
		{
			I::input->m_fCameraInThirdPerson = false;
			I::input->m_vecCameraOffset = Vec(G::StartAngle.x, G::StartAngle.y, 0);
		}
	}
}
