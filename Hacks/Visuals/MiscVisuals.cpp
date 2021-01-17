#include "../../Include.hpp"

MiscVisuals* miscvisuals = new MiscVisuals();

Vec MiscVisuals::GetIdealCameraPos(float distance)
{
	Vec PlayerPos = G::LocalPlayer->GetEyePos();	//pleyer center position
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
	Vec PlayerPos = G::LocalPlayer->GetEyePos();		//player center position

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
	if (I::engine->IsInGame() && G::LocalPlayer && G::LocalPlayerAlive) {
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

void MiscVisuals::RankRevealer()
{
	if (!Config::GetBool("visuals-misc-revealranks")) return;
	if (!(G::cmd->buttons & IN_SCORE)) return;

	I::client->dispatchUserMessage(50, 0, 0, nullptr);
}

void MiscVisuals::GrenadePrediction()
{
	static ConVar* nadeVar = I::cvar->FindVar("cl_grenadepreview");

	nadeVar->onChangeCallbacks.size = 0;
	nadeVar->SetValue(Config::GetBool("visuals-misc-grenadeprediction"));
}

void MiscVisuals::NoScope()
{
	if (!G::LocalPlayerAlive)
		return;

	if (!Config::GetBool("visuals-misc-noscope"))
		return;

	if (!G::LocalPlayer->IsScoped())
		return;

	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);
	I::surface->DrawSetColor(Color(0,0,0,255));
	I::surface->DrawLine(xSize / 2, 0, xSize / 2, ySize); //Top - Bottom
	I::surface->DrawLine(0, ySize / 2, xSize, ySize / 2); //Left - Right
}

void MiscVisuals::NoFlash(int stage)
{
	if (!Config::GetBool("visuals-misc-noflash"))
		return;

	if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!G::LocalPlayer || G::LocalPlayer->GetFlashMaxAlpha() < 0.1)
		return;

	*G::LocalPlayer->pGetFlashMaxAlpha() = 0;
}

void MiscVisuals::NoSmoke_DoPostScreenEffects()
{
	if (!G::LocalPlayer)
		return;
	if (!G::LocalPlayerAlive)
		return;
	if (!I::engine->IsInGame())
		return;

	static std::vector<const char*> smoke_materials = {
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	for (const auto mat : smoke_materials) {
		auto material = I::materialsystem->FindMaterial(mat);
		material->SetMaterialVarFlag(MaterialVarFlag::NO_DRAW, false);
		if (Config::GetBool("visuals-misc-nosmoke"))
			material->SetMaterialVarFlag(MaterialVarFlag::WIREFRAME, true);
		else
			material->SetMaterialVarFlag(MaterialVarFlag::WIREFRAME, false);
	}
}

void MiscVisuals::NoSmoke_FrameStageNotify()
{
	if (!Config::GetBool("visuals-misc-nosmoke"))
		return;
	if (!G::LocalPlayer)
		return;
	if (!G::LocalPlayerAlive)
		return;
	if (!I::engine->IsInGame())
		return;

	static int* smokecount = *(int**)(FindPattern("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
	if (smokecount) *smokecount = 0;
}
