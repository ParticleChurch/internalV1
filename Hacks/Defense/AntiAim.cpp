#include "../../Include.hpp"

AntiAim* antiaim = new AntiAim();

/*
bool AntiAim::LBYBreak()
{
	static float NextUpdate = 0;
	float velocity = G::LocalPlayer->GetVecVelocity().VecLength2D();
	float time = G::LocalPlayer->GetServerTime();
	if (!(G::LocalPlayer->GetFlags() & 1) || (!(G::LocalPlayer->IsAlive()))) {
		return false;
	}
	if (velocity > 0.1f) {
		NextUpdate = time + 0.22f;
	}
	if (NextUpdate <= time)
	{
		NextUpdate = time + 1.1f;
		return true;
	}

	return false;
}
*/

bool AntiAim::LBYBreak()
{
	static int NextUpdate = 0;
	float velocity = G::LocalPlayer->GetVecVelocity().VecLength2D();
	float time = I::globalvars->m_tickCount;
	if (!(G::LocalPlayer->GetFlags() & 1) || (!(G::LocalPlayer->IsAlive()))) {
		NextUpdate = 0;
		return false;
	}

	if (velocity > 0.1f) {
		NextUpdate = time + TimeToTicks(0.22f);
	}
	if (NextUpdate <= time)
	{
		NextUpdate = time + TimeToTicks(1.1f);
		return true;
	}
	return false;
}

float AntiAim::AtCrosshair()
{
	float BestFOV = FLT_MAX;
	QAngle BestAng = G::StartAngle;

	/*
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		//it->first is key	  (User Id)
		//it->second is value (Player struct)
	}
	*/

	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		auto a = it->second;

		if (a.index == G::LocalPlayerIndex)
			continue;

		if (!a.entity)
			continue;

		if (!(a.health > 0))
			continue;

		if (a.dormant)
			continue;

		if (a.team == G::LocalPlayerTeam)
			continue;

		QAngle AngToTarg = aimbot->CalculateAngle(a.EyePos);
		Vec delta = AngToTarg - G::StartAngle;
		delta.NormalizeAngle();

		float fov = min(sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f)), 180.0f);

		if (fov < BestFOV)
		{
			BestFOV = fov;
			BestAng = AngToTarg;
		}
	}

	return BestAng.y + 180.f;
}

float AntiAim::AtDistance()
{
	float BestDist = FLT_MAX;
	QAngle BestAng = G::StartAngle;

	/*
	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		//it->first is key	  (User Id)
		//it->second is value (Player struct)
	}
	*/


	std::map<int, Player>::iterator it;
	for (it = G::EntList.begin(); it != G::EntList.end(); it++)
	{
		auto a = it->second;

		if (a.index == G::LocalPlayerIndex)
			continue;

		if (!a.entity)
			continue;

		if (!(a.health > 0))
			continue;

		if (a.dormant)
			continue;

		if (a.team == G::LocalPlayerTeam)
			continue;

		QAngle AngToTarg = aimbot->CalculateAngle(a.EyePos);
		Vec delta = a.EyePos - G::LocalPlayer->GetEyePos();

		if (delta.VecLength2D() < BestDist)
		{
			BestDist = delta.VecLength2D();
			BestAng = AngToTarg;
		}
	}

	return BestAng.y + 180.f;
}

float AntiAim::AtFreestanding()
{
	float Distance = AtDistance();
	float Crosshair = AtCrosshair();
	float Delta = fabsf(Distance - Crosshair);
	
	// we could either prioritize crosshair, or distance. 
	// I'm assuming the user has a better idea so...
	float OptimalAngle = Crosshair;
	NormalizeYaw(OptimalAngle);
	
	// If they are close together (within 60 deg)
	if (Delta < 60)
	{
		OptimalAngle = (Crosshair + Distance) / 2;
		NormalizeYaw(OptimalAngle);
	}

	// If we should freestand at ALL
	QAngle BestHeadAng;
	if (GetBestHeadAngle(BestHeadAng))
	{
		NormalizeYaw(BestHeadAng.y);
		Delta = fabsf(OptimalAngle - BestHeadAng.y);
		

		// If they are close together (within 120 deg)
		if (Delta < 120) {
			OptimalAngle = (BestHeadAng.y + OptimalAngle) / 2;
			NormalizeYaw(OptimalAngle);
		}
	}

	return OptimalAngle;
}


void AntiAim::legit()
{
	// boolean is a cstate with true=1,false=0
	static Config2::CState* Enable = Config2::GetState("antiaim-legit-enable");
	static Config2::CState* AADirection = Config2::GetState("antiaim-legit-invert"); // terrible property name btw
	static Config2::CFloat* MaxDesync = Config2::GetFloat("antiaim-legit-max-angle");

	// make it so it only updates on a new tick :D
	static int tick_count = G::cmd->tick_count;
	if (tick_count == G::cmd->tick_count)
		return;
	tick_count = G::cmd->tick_count;

	if (!Enable->Get())
		return;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;

	fake = G::cmd->viewangles;
	real = G::cmd->viewangles;

	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		return;

	legit_left = AADirection->Get();
	legit_side = legit_left ? 1 : -1;

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle() * MaxDesync->Get() / 100.f;
	
	//side by default = left
	if (!BreakLBY)
	{
		float amount = Delta * 2 * legit_side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = Delta * -1 * legit_side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket) {
		fake = G::cmd->viewangles;
		real.x = G::cmd->viewangles.x;
		real.y = G::cmd->viewangles.y + (Delta * legit_side);
	}
	else
	{
		if (!BreakLBY)
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * legit_side * -1);
		}
		else
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * legit_side);
		}
	}

	if (*G::pSendPacket) 
	{
		Matrix3x4 Temp[128];
		G::LocalPlayer->SetupBones(FakeMatrix, 128, 0x100, 0);
		G::LocalPlayer->SetupBones(Temp, 128, 0x100, 0);
		RotateBoneMatrix(Vec(0, antiaim->fake.y - antiaim->real.y, 0), G::LocalPlayer->GetVecOrigin(), Temp, FakeMatrix);
		FakeAbsOrigin = G::LocalPlayer->GetVecOrigin();
	}
		

	fake.NormalizeAngle();
	real.NormalizeAngle();
}

void AntiAim::ManualRage()
{
	// First fight with direction booleans
	static Config2::CState* Left	= Config2::GetState("antiaim-manual-left");
	static Config2::CState* Back	= Config2::GetState("antiaim-manual-back");
	static Config2::CState* Right	= Config2::GetState("antiaim-manual-right");
	static Config2::CFloat* Max		= Config2::GetFloat("antiaim-manual-max");

	static int PrevLeft = 0;
	static int PrevBack = 1;
	static int PrevRight = 0;

	
	static int MODE = 1; // left, back, right (0, 1, 2)

	// If they are all off, reset back to last state
	if (!Left->Get() && !Back->Get() && !Right->Get())
	{
		Left->Set(PrevLeft);
		Back->Set(PrevBack);
		Right->Set(PrevRight);
	}
	// If user pressed left...
	else if (!PrevLeft && Left->Get())
	{
		Back->Set(0);
		Right->Set(0);
		MODE = 0;
	} 
	// If user pressed back...
	else if (!PrevBack && Back->Get())
	{
		Left->Set(0);
		Right->Set(0);
		MODE = 1;
	}
	// If user pressed right...
	else if (!PrevRight && Right->Get())
	{
		Left->Set(0);
		Back->Set(0);
		MODE = 2;
	}

	PrevLeft = Left->Get();
	PrevBack = Back->Get();
	PrevRight = Right->Get();

	switch (MODE)
	{
	case 0:
		G::cmd->viewangles.y += -90;
		break;
	case 1:
		break;
	case 2:
		G::cmd->viewangles.y += 90;
		break;
	default:
		break;
	}

	// Because we are doing move fix, we can desync in a smaller, but better way :D
	static bool LastSendPacket = (G::pSendPacket && *G::pSendPacket);

	// 2 correct send packets in a row... --> stay at targets :D
	if (LastSendPacket == true && (G::pSendPacket && *G::pSendPacket))
	{
		fake = G::cmd->viewangles;
		real = G::cmd->viewangles;
		return;
	}

	// Now we attempt to desync

	// Amount we are desyncing
	float amount = G::LocalPlayer->GetMaxDesyncAngle() * Max->Get() / 100.f;

	// INVERTED: Left, Right, Back
	// REG: Left, Right Back

	// Move head back to prev position 
	// DESYNC ON LEFT SIDE OF REAL: 0.2f
	// DESYNC ON RIGHT SIDE OF REAL: 0.6f

	// If shit is inverted...
	static bool Flip = false;
	if (this->Invert)
	{
		switch (MODE)
		{
		case 0: // Left
			// Set up Yaw
			G::cmd->viewangles.y += (0.6 * amount);
			// Set real
			fake = G::cmd->viewangles;
			// Set our FAKE more towards orig yaw pos
			if (G::pSendPacket && !*G::pSendPacket)
			{
				// If not choking, do this
				G::cmd->viewangles.y += amount;
				// Set Fake
				real = G::cmd->viewangles;
			}
			break;
		case 1: // Back
			
			// If we arent sending a packet, go brrrr
			if (G::pSendPacket && !*G::pSendPacket)
			{
				// Set real
				G::cmd->viewangles.y += Flip ? -amount : amount;
				real = G::cmd->viewangles;
			}
			// If we are sending packet, change nothing
			else if(G::pSendPacket && *G::pSendPacket)
			{
				Flip = !Flip;
				// Set Fake
				fake = G::cmd->viewangles;
			}
				
			break;
		case 2: // RIGHT
			// Set up Yaw
			G::cmd->viewangles.y += -(0.2 * amount);
			
			// Set fake
			fake = G::cmd->viewangles;
			// Set our REAL more towards orig yaw pos
			if (G::pSendPacket && !*G::pSendPacket)
			{
				// If not choking, do this
				G::cmd->viewangles.y -= amount;
				// Set real
				real = G::cmd->viewangles;
			}
			break;
			// Back?
		default:
			break;
		}
	}
	// If NOT inverted
	else
	{
		switch (MODE)
		{
		case 0: // Left
			// Set up Yaw
			G::cmd->viewangles.y += -(0.2 * amount);
			// Set real
			real = G::cmd->viewangles;
			// Set our FAKE more towards orig yaw pos
			if (G::pSendPacket && *G::pSendPacket)
			{
				// If choking, do this
				G::cmd->viewangles.y += amount;
				// Set Fake
				fake = G::cmd->viewangles;
			}
			break;
		case 1: // Back
			// Set real
			G::cmd->viewangles.y += Flip ? (0.9 * amount) : (-0.9 * amount);
			real = G::cmd->viewangles;
			
			// Set our FAKE more towards orig yaw pos
			if (G::pSendPacket && *G::pSendPacket)
			{
				// If choking, do this
				G::cmd->viewangles.y += Flip ? -amount : amount;
				// Set Fake
				fake = G::cmd->viewangles;
				Flip = !Flip;
			}
			break;
		case 2: // RIGHT
			// Set up Yaw
			G::cmd->viewangles.y += (0.6 * amount);
			// Set real
			real = G::cmd->viewangles;
			// Set our REAL more towards orig yaw pos
			if (G::pSendPacket && *G::pSendPacket)
			{
				// If choking, do this
				G::cmd->viewangles.y -= amount;
				// Set Fake
				fake = G::cmd->viewangles;
			}
			break;
			// Back?
		default:
			break;
		}
	}

	AnimState* animstate = G::LocalPlayer->GetAnimstate();
	float backupgoalfeet = 0;
	if (animstate)
	{
		backupgoalfeet = animstate->m_flGoalFeetYaw;
		animstate->m_flGoalFeetYaw = antiaim->real.y;
	}

	// Deal with fake matrix crap
	if (*G::pSendPacket)
	{
		Matrix3x4 Temp[128];
		G::LocalPlayer->SetupBones(FakeMatrix, 128, 0x100, 0);
		G::LocalPlayer->SetupBones(Temp, 128, 0x100, 0);
		RotateBoneMatrix(Vec(0, antiaim->fake.y - antiaim->real.y, 0), G::LocalPlayer->GetVecOrigin(), Temp, FakeMatrix);
		FakeAbsOrigin = G::LocalPlayer->GetVecOrigin();
	}

	if (animstate)
	{
		animstate->m_flGoalFeetYaw = backupgoalfeet;
	}

	// Deal with real/fake crap
	fake.NormalizeAngle();
	real.NormalizeAngle();

	// Deal with sending two packets at once
	LastSendPacket = (G::pSendPacket && *G::pSendPacket);
}

void AntiAim::CustomRage()
{
	static Config2::CFloat* RealOffset = Config2::GetFloat("antiaim-custom-real");
	static Config2::CFloat* RealJitter = Config2::GetFloat("antiaim-custom-real-jitter");
	static Config2::CFloat* FakeOffset = Config2::GetFloat("antiaim-custom-fake");
	static Config2::CFloat* FakeJitter = Config2::GetFloat("antiaim-custom-fake-jitter");

	// Because we are doing move fix, we can desync in a smaller, but better way :D
	static bool LastSendPacket = (G::pSendPacket && *G::pSendPacket);

	// Deal with Real Offset/Real Jitter
	// Deal with the offset
	G::cmd->viewangles.y += RealOffset->Get();
	// Deal with the FLIP
	static float FlipTime = I::globalvars->m_curTime;
	static bool RealJitterFlip = false;
	if (fabsf(I::globalvars->m_curTime - FlipTime) > 0.1) //200ms flip
	{
		FlipTime = I::globalvars->m_curTime;
		RealJitterFlip = !RealJitterFlip;
	}
	G::cmd->viewangles.y += RealJitterFlip ? RealJitter->Get() : -RealJitter->Get();

	// 2 correct send packets in a row... --> stay at targets :D
	if (LastSendPacket == true && (G::pSendPacket && *G::pSendPacket))
	{
		fake = G::cmd->viewangles;
		real = G::cmd->viewangles;
		return;
	}

	// Now we attempt to desync
	float amount = G::LocalPlayer->GetMaxDesyncAngle() * FakeOffset->Get() / 100.f;

	// Deal with Jitter
	static bool Flip = false;
	if (G::pSendPacket && *G::pSendPacket)
		Flip = !Flip;
	amount *= Flip ? 1 : 1 - (FakeJitter->Get() / 100);

	// Set real
	G::cmd->viewangles.y += this->Invert ? (0.9 * amount) : (-0.9 * amount);
	real = G::cmd->viewangles;

	// Set our FAKE more towards orig yaw pos
	if (G::pSendPacket && *G::pSendPacket)
	{
		// If choking, do this
		G::cmd->viewangles.y += this->Invert ? -amount : amount;
		// Set Fake
		fake = G::cmd->viewangles;
	}

	// Deal with fake matrix crap
	if (*G::pSendPacket)
	{
		Matrix3x4 Temp[128];
		G::LocalPlayer->SetupBones(FakeMatrix, 128, 0x100, 0);
		G::LocalPlayer->SetupBones(Temp, 128, 0x100, 0);
		RotateBoneMatrix(Vec(0, antiaim->fake.y - antiaim->real.y, 0), G::LocalPlayer->GetVecOrigin(), Temp, FakeMatrix);
		FakeAbsOrigin = G::LocalPlayer->GetVecOrigin();
	}

	// Deal with real/fake crap
	fake.NormalizeAngle();
	real.NormalizeAngle();

	// Deal with sending two packets at once
	LastSendPacket = (G::pSendPacket && *G::pSendPacket);
}

void AntiAim::SetYawBase()
{
	static Config2::CState* YawBase = Config2::GetState("visuals-rage-yawbase");
	switch (YawBase->Get())
	{
	case 0:
		G::cmd->viewangles.y += 180.f;
		break;
	case 1:
		G::cmd->viewangles.y = AtCrosshair();
		break;
	case 2:
		G::cmd->viewangles.y = AtDistance();
		break;
	default:
		G::cmd->viewangles.y = AtFreestanding();
		break;
	}
}

void AntiAim::rage()
{
	// boolean is a cstate with true=1,false=0
	static Config2::CState* Enable = Config2::GetState("antiaim-rage-enable");
	static Config2::CFloat* Pitch = Config2::GetFloat("antiaim-rage-pitch");
	static Config2::CState* Invert = Config2::GetState("antiaim-rage-invert");
	static Config2::CState* Mode = Config2::GetState("antiaim-type");

	if (!Enable->Get()) return;

	// make it so it only updates on a new tick :D
	static int tick_count = G::cmd->tick_count;
	if (tick_count == G::cmd->tick_count)
		return;
	tick_count = G::cmd->tick_count;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;

	// Get the invert status
	this->Invert = Invert->Get();

	// Set pitch angles (and log them)
	G::cmd->viewangles.x = Pitch->Get();

	//Set the yaw base
	SetYawBase();

	// Do move fix
	if (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
	{
		/*
		IN_FORWARD
		IN_BACK
		IN_LEFT
		IN_RIGHT
		IN_MOVELEFT
		IN_MOVERIGHT
		*/
		return; // WILL DO LATER! for now no aa on ladders :D
	}

	// do different stuff depending on selected aa crap
	switch (Mode->Get())
	{
	case 0:
		ManualRage();
		break;
	case 1:
		CustomRage();
		break;
	default:
		break;
	}
}

bool WorldToScreen2(Vec& in, Vec& out)
{
	auto matrix = I::engine->WorldToScreenMatrix().c;

	float w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

	if (w > 0.001f) {
		static int width = -1;
		static int height = -1;
		I::surface->GetScreenSize(width, height);
		out.x = width / 2 * (1 + (matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3]) / w);
		out.y = height / 2 * (1 - (matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3]) / w);
		out.z = 0.0f;
		return true;
	}
	return false;
}

bool AntiAim::GetBestHeadAngle(QAngle& angle)
{
	Vec position = G::LocalPlayer->GetEyePos();

	float closest_distance = 100.0f;

	float radius = HEADEDGE + 0.1f;
	float step = (float)M_PI * 2.f / 8.f;

	for (float a = 0; a < (M_PI * 2.0); a += step)
	{
		Vec location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		trace_t tr;
		Ray_t ray(position, location);
		CTraceFilter traceFilter(G::LocalPlayer);
		//I::enginetrace->TraceRay(ray, 0x4600400B, &traceFilter, &tr);
		I::enginetrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vec delta = position - tr.Endpos;

		float distance = delta.VecLength();

		if (distance < closest_distance)
		{
			closest_distance = distance;
			angle.y = RAD2DEG(a);
		}
	}

	return closest_distance < HEADEDGE;
}

void AntiAim::Visualize()
{
	return;
	if (!G::LocalPlayer) return;
	if (!G::LocalPlayerAlive) return;
	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);

	Vec origin = G::LocalPlayer->GetVecOrigin();
	float DeltaAng = real.y;
	float yDelta = 10 * sinf(DEG2RAD(DeltaAng));
	float xDelta = 10 * cosf(DEG2RAD(DeltaAng));
	Vec point = Vec(origin.x + xDelta, origin.y + yDelta, origin.z);

	Vec sPoint1;
	Vec sPoint2;

	if (WorldToScreen2(origin, sPoint1) && WorldToScreen2(point, sPoint2))
	{
		I::surface->DrawSetColor(Color(255.f, 255.f, 255.f, 255.f));
		I::surface->DrawLine(sPoint1.x, sPoint1.y, sPoint2.x, sPoint2.y);
	}

	 DeltaAng = fake.y;
	 yDelta = 10 * sinf(DEG2RAD(DeltaAng));
	 xDelta = 10 * cosf(DEG2RAD(DeltaAng));
	 point = Vec(origin.x + xDelta, origin.y + yDelta, origin.z);

	if (WorldToScreen2(origin, sPoint1) && WorldToScreen2(point, sPoint2))
	{
		I::surface->DrawSetColor(Color(255.f, 0, 0, 255.f));
		I::surface->DrawLine(sPoint1.x, sPoint1.y, sPoint2.x, sPoint2.y);
	}

	// Draw LagOnPeak stuff
	Vec EyePos = G::LocalPlayer->GetEyePos();
	if (fakelag->LaggingOnPeak && WorldToScreen2(fakelag->PrevPos, sPoint1) && WorldToScreen2(EyePos, sPoint2))
	{
		I::surface->DrawSetColor(Color(253.f, 106.f, 2.f, 255.f));
		I::surface->DrawLine(sPoint1.x, sPoint1.y, sPoint2.x, sPoint2.y);
	}

	

	
}


