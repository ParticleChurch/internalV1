#include "../../Include.hpp"

AntiAim* antiaim = new AntiAim();

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

void AntiAim::legit()
{
	if (!Config::GetBool("antiaim-legit-enable"))
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
	
	legit_left = Config::GetState("antiaim-legit-invert");
	legit_side = legit_left ? 1 : -1;

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle() * Config::GetFloat("antiaim-legit-max-angle") / 100.f;
	
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

	fake.NormalizeAngle();
	real.NormalizeAngle();
}

void AntiAim::rage()
{
	if (!Config::GetBool("antiaim-rage-enable"))
		return;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;

	//set up rage style anyway
	G::cmd->viewangles.y += 180;
	G::cmd->viewangles.NormalizeAngle();

	rage_left = Config::GetState("antiaim-rage-invert");
	rage_side = rage_left ? 1 : -1;

	static bool Switcher = false;
	if (*G::pSendPacket)
		Switcher = !Switcher;

	switch (Config::GetState("antiaim-rage-pitch"))
	{
	case 0:
		G::cmd->viewangles.x = -89; //up
		break;
	case 1:
		G::cmd->viewangles.x = 89; //down
		break;
	case 2:
		G::cmd->viewangles.x = Switcher ? 89:-89; //up down
		break;
	default:
		break;
	}

	G::cmd->viewangles.y += Config::GetFloat("antiaim-rage-real");
	G::cmd->viewangles.y += Config::GetFloat("antiaim-rage-real-jitter") * (Switcher ? -1 : 1);

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle() * Config::GetFloat("antiaim-rage-fake") / 100.f;

	Delta -= (Delta * (Config::GetFloat("antiaim-rage-fake-jitter") / 100.f) * (Switcher ? 1 : 0));

	G::cmd->viewangles.y += rage_left ? -Delta: Delta; //set it to rage style aa

	//QAngle angle = G::cmd->viewangles;
	//if (antiaim->GetBestHeadAngle(angle))
	//	G::cmd->viewangles.y = angle.y;

	//side by default = left
	if (!BreakLBY)
	{
		float amount = Delta * 2 * rage_side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = Delta * -1 * rage_side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket) {
		fake = G::cmd->viewangles;
		real.x = G::cmd->viewangles.x;
		real.y = G::cmd->viewangles.y + (Delta * rage_side);
	}
	else
	{
		if (!BreakLBY)
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * rage_side * -1);
		}
		else
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * rage_side);
		}
	}

	fake.NormalizeAngle();
	real.NormalizeAngle();
}

void AntiAim::rage2()
{
	if (!Config::GetBool("antiaim-rage-enable"))
		return;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;

	//set up rage style anyway
	G::cmd->viewangles.y += 180;
	G::cmd->viewangles.NormalizeAngle();

	static bool Switcher = false;
	if (*G::pSendPacket)
		Switcher = !Switcher;

	switch (Config::GetState("antiaim-rage-pitch"))
	{
	case 0:
		G::cmd->viewangles.x = -89; //up
		break;
	case 1:
		G::cmd->viewangles.x = 89; //down
		break;
	case 2:
		G::cmd->viewangles.x = Switcher ? 89 : -89; //up down
		break;
	default:
		break;
	}

	G::cmd->viewangles.y += Config::GetFloat("antiaim-rage-real");
	G::cmd->viewangles.y += Config::GetFloat("antiaim-rage-real-jitter") * (Switcher ? -1 : 1) / 2.f;

	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle() * Config::GetFloat("antiaim-rage-fake") / 100.f;
	rage_left = Config::GetState("antiaim-rage-invert");

	if (rage_left)
		G::cmd->viewangles.y -= Delta / 2;
	else
		G::cmd->viewangles.y += Delta / 2;

	QAngle angle = G::cmd->viewangles;
	if (antiaim->GetBestHeadAngle(angle))
		G::cmd->viewangles.y = angle.y;

	rage_side = rage_left ? 1 : -1;

	//side by default = left
	if (!BreakLBY)
	{
		float amount = Delta * 2 * rage_side;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = Delta * -1 * rage_side;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket) {
		fake = G::cmd->viewangles;
		real.x = G::cmd->viewangles.x;
		real.y = G::cmd->viewangles.y + (Delta * rage_side);
	}
	else
	{
		if (!BreakLBY)
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * rage_side * -1);
		}
		else
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * rage_side);
		}
	}

	fake.NormalizeAngle();
	real.NormalizeAngle();
}


/*
Random fake, freestanding real
*/
void AntiAim::rage3()
{
	if (!Config::GetBool("antiaim-rage-enable"))
		return;

	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayerAlive)
		return;

	//set up rage style anyway
	G::cmd->viewangles.y += 180;
	G::cmd->viewangles.NormalizeAngle();

	QAngle angle = G::cmd->viewangles;
	if (antiaim->GetBestHeadAngle(angle))
		G::cmd->viewangles.y = angle.y;

	static bool Switcher = false;
	if (*G::pSendPacket)
		Switcher = !Switcher;

	switch (Config::GetState("antiaim-rage-pitch"))
	{
	case 0:
		G::cmd->viewangles.x = -89; //up
		break;
	case 1:
		G::cmd->viewangles.x = 89; //down
		break;
	case 2:
		G::cmd->viewangles.x = Switcher ? 89 : -89; //up down
		break;
	default:
		break;
	}

	static float LastDelta = G::LocalPlayer->GetMaxDesyncAngle();
	bool BreakLBY = LBYBreak();
	float Delta = G::LocalPlayer->GetMaxDesyncAngle();
	if(*G::pSendPacket && !BreakLBY)
		Delta *= (rand() % 2 ? -1 : 1) * ((rand() % 100) / 100.f);
	/*G::cmd->viewangles.y += -Delta;*/

	//side by default = left
	if (!BreakLBY)
	{
		float amount = Delta * 2;
		G::cmd->viewangles.y += *G::pSendPacket ? 0 : amount;
	}

	if (BreakLBY)
	{
		float amount = LastDelta * -1;
		G::cmd->viewangles.y += amount;
		*G::pSendPacket = false;
	}

	if (*G::pSendPacket) {
		fake = G::cmd->viewangles;
		real.x = G::cmd->viewangles.x;
		real.y = G::cmd->viewangles.y + (Delta);
	}
	else
	{
		if (!BreakLBY)
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta * -1);
		}
		else
		{
			real.x = G::cmd->viewangles.x;
			real.y = G::cmd->viewangles.y + (Delta);
		}
	}
	LastDelta = Delta;

	fake.NormalizeAngle();
	real.NormalizeAngle();

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
	float step = M_PI * 2.f / 8.f;

	esp->points.clear();
	esp->points.resize(0);
	for (float a = 0; a < (M_PI * 2.0); a += step)
	{
		Vec location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);
		esp->points.push_back(location);

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
