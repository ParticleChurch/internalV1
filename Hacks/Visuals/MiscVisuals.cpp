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
	I::enginetrace->TraceRay(Ray, MASK_SOLID & ~CONTENTS_MONSTER, &Filter, &Trace); // originaly mask all

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
		*G::LocalPlayer->GetObserverMode() = 0;
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
	else if(I::engine->IsInGame() && G::LocalPlayer)
	{
		I::input->m_fCameraInThirdPerson = false;
		
		if (Enable->Get() && *G::LocalPlayer->GetObserverMode() != 6)
		{
			I::input->CamInThirdPerson();
			*G::LocalPlayer->GetObserverMode() = 5;
		}
		else if(*G::LocalPlayer->GetObserverMode() != 6)
		{
			I::input->CamToFirstPerson();
			*G::LocalPlayer->GetObserverMode() = 4;
		}
	}
}

void MiscVisuals::DrawBeams(GameEvent* event)
{
	//userid
	//visuals-misc-beam

	static Config::CState* Enable = Config::GetState("visuals-misc-beam");

	if (!Enable->Get())
		return;

	int UserID = event->GetInt("userid");

	if (I::engine->GetPlayerForUserID(UserID) != G::LocalPlayerIndex)
		return;

	switch (StrHash::HashRuntime(event->GetName())) {
	case StrHash::Hash("bullet_impact"):
	{
		BeamInfo beamInfo;
		beamInfo.start = G::LocalPlayer->GetEyePos();
		beamInfo.end.x = event->GetFloat("x");
		beamInfo.end.y = event->GetFloat("y");
		beamInfo.end.z = event->GetFloat("z");

		Vec direction = beamInfo.start - beamInfo.end;
		direction /= direction.VecLength();
		direction *= 30;// force it to be 10 units ahead
		beamInfo.start += direction;

		beamInfo.modelName = "sprites/physbeam.vmt";
		beamInfo.modelIndex = -1;
		beamInfo.haloName = nullptr;
		beamInfo.haloIndex = -1;

		beamInfo.red = 255.0f;
		beamInfo.green = 0;
		beamInfo.blue = 0;
		beamInfo.brightness = 255.0f;

		beamInfo.type = 0;
		beamInfo.life = 0.0f;
		beamInfo.amplitude = 0.0f;
		beamInfo.segments = -1;
		beamInfo.renderable = true;
		beamInfo.speed = 0.2f;
		beamInfo.startFrame = 0;
		beamInfo.frameRate = 0.0f;
		beamInfo.width = 2.0f;
		beamInfo.endWidth = 2.0f;
		beamInfo.flags = 0x40;
		beamInfo.fadeLength = 20.0f;

		if (const auto beam = I::viewrenderbeams->createBeamPoints(beamInfo)) {
			constexpr auto FBEAM_FOREVER = 0x4000;
			beam->flags &= ~FBEAM_FOREVER;
			beam->die = I::globalvars->m_curTime + 2.0f;
		}
		break;
	}
	}
}

void MiscVisuals::NoAimViewPunchFSN(int stage)
{
	L::Debug("NoAimViewPunchFSN");
	static Config::CState* NoAimPunch = Config::GetState("visuals-misc-noAimPunch");
	static Config::CState* NoViewPunch = Config::GetState("visuals-misc-noViewPunch");

	static Vec aimPunch;
	static Vec viewPunch;

	L::Debug("NoAimViewPunchFSN - run test");
	if (!G::LocalPlayer || !G::LocalPlayerAlive || !G::IsInGame)
		return;

	if (stage == FRAME_RENDER_START) {
		aimPunch = G::LocalPlayer->GetAimPunchAngle();
		viewPunch = G::LocalPlayer->GetViewPunchAngle();

		if (NoAimPunch->Get())
			G::LocalPlayer->GetAimPunchAngle() = Vec{ };

		if (NoViewPunch->Get())
			G::LocalPlayer->GetViewPunchAngle() = Vec{ };

	}
	else if (stage == FRAME_RENDER_END) {
		G::LocalPlayer->GetAimPunchAngle() = aimPunch;
		G::LocalPlayer->GetViewPunchAngle() = viewPunch;
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

std::vector<MiscVisuals::KeybindVis> MiscVisuals::GetKeyBinds()
{
	std::vector<KeybindVis> KeyBindList = {};
	for (auto& a : Config::PropertyTable)
	{
		// If it is bound to some key
		if (Config::GetKeybind(a.first) != -1)
		{
			KeyBindList.push_back(KeybindVis(a.second->Name, Config::GetState(a.first)->Get()));
		}
	}
	return KeyBindList;
}
