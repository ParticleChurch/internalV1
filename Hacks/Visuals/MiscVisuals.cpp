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
	I::enginetrace->TraceRay(Ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_GRATE, &Filter, &Trace); // originaly mask all

	if (Trace.Fraction <= 1)
		return distance * Trace.Fraction * 0.9;
	else
		return distance;
}

void MiscVisuals::ThirdPerson_hkCamToFirstPeron()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-thirdperson");
	if (Enable->Get())
		return;
	H::ohkCamToFirstPeron(I::input);
}

void MiscVisuals::ThirdPerson_DoPostScreenEffects()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-thirdperson");
	static Config::CFloat* Dist = Config::GetFloat("visuals-misc-thirdperson-distance");
	if (I::engine->IsInGame() && G::LocalPlayer && G::LocalPlayerAlive) {
		if (Enable->Get())
		{
			I::input->m_fCameraInThirdPerson = true;
			I::input->m_vecCameraOffset = Vec(G::StartAngle.x, G::StartAngle.y, GetCameraBoomLength(Dist->Get()));
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
	static Config::CState* Enable = Config::GetState("visuals-misc-revealranks");
	if (!Enable->Get()) return;
	if (!(G::cmd->buttons & IN_SCORE)) return;

	I::client->dispatchUserMessage(50, 0, 0, nullptr);
}

void MiscVisuals::GrenadePrediction()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-grenadeprediction");
	static ConVar* nadeVar = I::cvar->FindVar("cl_grenadepreview");

	nadeVar->onChangeCallbacks.size = 0;
	nadeVar->SetValue(Enable->Get());
}

void MiscVisuals::NoScope()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-noscope");

	if (!G::LocalPlayerAlive)
		return;

	if (!Enable->Get())
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

void MiscVisuals::NoFlash()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-noflash");

	if (Enable->Get())
		*(G::LocalPlayer->pGetFlashMaxAlpha()) = 0.f;
}

void MiscVisuals::NoSmoke_DoPostScreenEffects()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-nosmoke");

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
		if (Enable->Get())
			material->SetMaterialVarFlag(MaterialVarFlag::WIREFRAME, true);
		else
			material->SetMaterialVarFlag(MaterialVarFlag::WIREFRAME, false);
	}
}

void MiscVisuals::NoSmokeFSN()
{
	static int* smokecount = *(int**)(FindPattern("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
	static Config::CState* Enable = Config::GetState("visuals-misc-nosmoke");

	if (Enable->Get()) *smokecount = 0;
}

void MiscVisuals::ChangeViewModel()
{
	/*static bool init = false;
	if (!init)
	{
		init = true;
		I::cvar->FindVar("viewmodel_fov")->onChangeCallbacks.size = 0;
		I::cvar->FindVar("viewmodel_offset_x")->onChangeCallbacks.size = 0;
		I::cvar->FindVar("viewmodel_offset_y")->onChangeCallbacks.size = 0;
		I::cvar->FindVar("viewmodel_offset_z")->onChangeCallbacks.size = 0;
		I::cvar->FindVar("fov_cs_debug")->onChangeCallbacks.size = 0;
		
	}

	I::cvar->FindVar("viewmodel_fov")->SetValue(Config::GetFloat("visuals-fov"));
	I::cvar->FindVar("fov_cs_debug")->SetValue(Config::GetFloat("visuals-fov"));
	I::cvar->FindVar("viewmodel_offset_x")->SetValue(Config::GetFloat("visuals-view-x"));
	I::cvar->FindVar("viewmodel_offset_y")->SetValue(Config::GetFloat("visuals-view-y"));
	I::cvar->FindVar("viewmodel_offset_z")->SetValue(Config::GetFloat("visuals-view-z"));*/
}

void MiscVisuals::ForceCrosshair()
{
	static Config::CState* Enable = Config::GetState("visuals-misc-forcecrosshair");

	static ConVar* m_iCrosshairData = I::cvar->FindVar("weapon_debug_spread_show");

	if (I::engine->IsInGame())
	{
		// if you are scoped or you aren't enabled
		if (G::LocalPlayer && G::LocalPlayerAlive)
			m_iCrosshairData->SetValue(!(G::LocalPlayer->IsScoped() || !Enable->Get()) ? 3 : 0);
	}
}

std::vector<std::string> MiscVisuals::GetSpectators()
{
	// If no localplayer or not in game, dont bother...
	if (!I::engine->IsInGame()) return std::vector<std::string>();
	if (!G::LocalPlayer) return std::vector<std::string>();

	Entity* Player = G::LocalPlayer;
	if (!G::LocalPlayer->IsAlive())
	{
		// if not observing crap, return nothing
		Entity* ObserverTarget = Player->GetObserverTarget();
		if (!ObserverTarget)
		{
			return std::vector<std::string>();
		}
			
		// set to person we are observing
		Player = ObserverTarget;
	}

	// Get people :D
	int spectators = 0;
	std::vector<std::string> SpecList = {};
	
	for (int i = 1; i <= I::globalvars->m_maxClients; i++) {
		Entity* ent = I::entitylist->GetClientEntity(i);

		if (!ent)
			continue;

		player_info_t info;

		if (!I::engine->GetPlayerInfo(i, &info))
			continue;

		Entity* SpecatedPlayer = ent->GetObserverTarget();

		if (!SpecatedPlayer)
			continue;

		// if the spectated player isn't the localplayer, don't bother
		if (SpecatedPlayer != G::LocalPlayer)
			continue;

		if (ent->IsAlive() || ent->IsDormant())
			continue;

		player_info_t spectated_player_info;

		if (!I::engine->GetPlayerInfo(SpecatedPlayer->Index(), &spectated_player_info))
			continue;

		SpecList.push_back(info.name);
		spectators++;
	}

	return SpecList;
}