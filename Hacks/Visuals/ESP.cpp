#include "../../Include.hpp"

ESP* esp = new ESP();

bool WorldToScreen(Vec& in, Vec& out)
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

void ESP::DrawBoxes(Vec TL, Vec BR)
{
	I::surface->DrawOutlinedRect(TL.x, TL.y, BR.x, BR.y);
}

void ESP::DrawName(Vec TL, Vec BR, char Name[128], Color clr) //NEED TO FINISH
{
	static DWORD FONT = I::surface->FontCreate();
	static bool Once = true;
	if (Once)
	{
		Once = false;
		I::surface->SetFontGlyphSet(FONT, "Tahoma", 14, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	}

	std::string TEXT = Name;
	static std::wstring wide_string;
	wide_string = std::wstring(TEXT.begin(), TEXT.end());
	
	I::surface->DrawSetTextFont(FONT);
	I::surface->DrawSetTextColor(clr);
	I::surface->DrawSetTextPos(TL.x, TL.y - 14);
	I::surface->DrawPrintText(wide_string.c_str(), wcslen(wide_string.c_str()));
}

void ESP::DrawSnapLines(Vec TL, Vec BR)
{
	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);
	I::surface->DrawLine(xSize / 2, ySize, ((BR.x - TL.x) / 2) + TL.x, BR.y);
}

void ESP::DrawHealth(Vec TL, Vec BR, int Health, Color fg, Color bg)
{
	int Height = BR.y - TL.y;

	int HeightBar = (int)(Height * (Health / 100.0f));
	int WidthBar = (int)(3 + Height / 100.0f);

	I::surface->DrawSetColor(bg);		//red
	I::surface->DrawFilledRect(BR.x + 1, BR.y - Height, BR.x + WidthBar + 1, BR.y);

	I::surface->DrawSetColor(fg);		//green
	I::surface->DrawFilledRect(BR.x + 1, BR.y - HeightBar, BR.x + WidthBar + 1, BR.y);
}

void ESP::DrawSkeleton(Entity* Ent)
{
	//https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Bone_ESP 
	//p help, prob should implement properly but will do later
	studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(Ent->GetModel());
	if (!StudioModel)
		return;

	static Matrix3x4 BoneToWorldOut[128];
	if (Ent->SetupBones(BoneToWorldOut, 128, 256, 0))
	{
		for (int i = 0; i < StudioModel->numbones; i++)
		{
			mstudiobone_t* Bone = StudioModel->GetBone(i);
			if (!Bone || !(Bone->flags & 256) || Bone->parent == -1)
				continue;

			Vec Screen1;
			Vec loc1 = Vec(BoneToWorldOut[i][0][3], BoneToWorldOut[i][1][3], BoneToWorldOut[i][2][3]);
			if (!WorldToScreen(loc1, Screen1))
				continue;

			Vec Screen2;
			Vec loc2 = Vec(BoneToWorldOut[Bone->parent][0][3], BoneToWorldOut[Bone->parent][1][3], BoneToWorldOut[Bone->parent][2][3]);
			if (!WorldToScreen(loc2, Screen2))
				continue;

			I::surface->DrawLine(Screen1.x, Screen1.y, Screen2.x, Screen2.y);
		}
	}
}

void ESP::DrawBacktrackingDots()
{
	for (int i = 1; i < 65; i++) { //AIDS
		if (backtrack->Records[i].empty())
			continue;
		static bool start = true;
		start = true;
		for (auto tick : backtrack->Records[i])
		{
			static Vec LastScreen;
			if (start) {
				Vec screen;
				Vec loc = tick.Bone(8);
				if (WorldToScreen(loc, screen))
				{
					start = false;
					LastScreen = screen;
				}
			}
			else
			{
				Vec screen;
				Vec loc = tick.Bone(8);
				if (WorldToScreen(loc, screen) && 
					LastScreen.x > 0  && LastScreen.x < 1920
					&& LastScreen.y > 0 && LastScreen.y < 1080)
				{
					//if (!tick.Shooting)
					//	I::surface->DrawSetColor(Color(255, 255, 255, 255));
					//else
					I::surface->DrawSetColor(Color(255, 0, 0, 255));
					I::surface->DrawLine(screen.x, screen.y, LastScreen.x , LastScreen.y);
					LastScreen = screen;
				}
			}
		}
	}
}

void ESP::GetBounds(Entity* ent, Vec& TL, Vec& BR)
{
	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);

	Matrix3x4 trans = ent->GetTransform();
	Matrix3x4* transptr = &trans;
	
	Collideable* col = ent->GetCollideable();
	if (!col)
		return;
	Vec origin = ent->GetVecOrigin();
	Vec min = origin + col->obbMins();
	Vec max = origin + col->obbMaxs();
	
	static Vec screen[] = { Vec(), Vec(), Vec(), Vec(), Vec(), Vec(), Vec(), Vec() };
	Vec P[] = { Vec(min.x, min.y, min.z),
						Vec(min.x, max.y, min.z),
						Vec(max.x, max.y, min.z),
						Vec(max.x, min.y, min.z),
						Vec(max.x, max.y, max.z),
						Vec(min.x, max.y, max.z),
						Vec(min.x, min.y, max.z),
						Vec(max.x, min.y, max.z) };
	
	for (auto a : P)
	{
		a = a.transform(trans);
	}

	for (int i = 0; i < 8; i++)
	{
		if (!WorldToScreen(P[i], screen[i]))
			return;
	}
		

	
	int left = xSize;
	int right = 0;
	int top = ySize;
	int bottom = 0;
	for (auto a : screen)
	{
		if (a.x < left)
			left = a.x;
		if (a.x > right)
			right = a.x;
		if (a.y < top)
			top = a.y;
		if (a.y > bottom)
			bottom = a.y;
	}

	TL = Vec(left, top, 0);
	BR = Vec(right, bottom, 0);
}

void ESP::Run_PaintTraverse()
{
	
	antiaim->Visualize();
	I::surface->DrawSetColor(Color(255.f, 255.f, 255.f, 255.f));

	static DWORD FONT = I::surface->FontCreate();
	static bool Once = true;
	if (Once)
	{
		Once = false;
		I::surface->SetFontGlyphSet(FONT, "Tahoma", 20, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	}

	
	for (auto p : points)
	{
		Vec screen;
		if (WorldToScreen(p, screen))
		{
			I::surface->DrawSetColor(Color(255, 0, 0, 255));
			I::surface->DrawLine(screen.x, screen.y, screen.x+1, screen.y+1);
			/*static bool Once = true;
			if (Once)
			{
				Once = false;
				I::surface->SetFontGlyphSet(FONT, "Tahoma", 14, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
			}

			static std::string TEXT = "x";
			static std::wstring wide_string = std::wstring(TEXT.begin(), TEXT.end());

			I::surface->DrawSetTextFont(FONT);
			I::surface->DrawSetTextColor(Color(255,0,0,255));
			I::surface->DrawSetTextPos(screen.x, screen.y );
			I::surface->DrawPrintText(wide_string.c_str(), wcslen(wide_string.c_str()));*/
		}
	}
	
	


	for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
	{
		if (i == G::LocalPlayerIndex)
			continue;

		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent)
			continue;

		player_info_t PlayerInfo;
		if (!I::engine->GetPlayerInfo(i, &PlayerInfo))
			continue;

		if (!(Ent->GetHealth() > 0))
			continue;

		if (Ent->IsDormant())
			continue;

		Vec TopLeft;
		Vec BottomRight;
		GetBounds(Ent, TopLeft, BottomRight);


		//if on localplayer team...
		if (Ent->GetTeam() == I::entitylist->GetClientEntity(I::engine->GetLocalPlayer())->GetTeam())
		{
			//if friend esp not enabled
			if (!Config::GetBool("visuals-esp-friend-enable"))
				continue;

			if (Config::GetBool("visuals-esp-friend-bbox"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-friend-bbox-color"));
				DrawBoxes(TopLeft, BottomRight);
			}
			if (Config::GetBool("visuals-esp-friend-name"))
			{
				DrawName(TopLeft, BottomRight, PlayerInfo.name, Config::GetColor("visuals-esp-friend-name-color"));
			}
			if (Config::GetBool("visuals-esp-friend-snapline"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-friend-snapline-color"));
				DrawSnapLines(TopLeft, BottomRight);
			}
			if (Config::GetBool("visuals-esp-friend-health"))
			{
				DrawHealth(TopLeft, BottomRight, Ent->GetHealth(),
					Config::GetColor("visuals-esp-friend-health-color"),
					Config::GetColor("visuals-esp-friend-health-color-background"));
			}
			if (Config::GetBool("visuals-esp-friend-skeleton"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-friend-skeleton-color")); //white by defualt
				DrawSkeleton(Ent);
			}
		}
		else
		{
			//if enemy esp not enabled
			if (!Config::GetBool("visuals-esp-enemy-enable"))
				continue;

			if (Config::GetBool("visuals-esp-enemy-bbox"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-enemy-bbox-color"));
				DrawBoxes(TopLeft, BottomRight);
			}
			if (Config::GetBool("visuals-esp-enemy-name"))
			{
				DrawName(TopLeft, BottomRight, PlayerInfo.name, Config::GetColor("visuals-esp-enemy-name-color"));
			}
			if (Config::GetBool("visuals-esp-enemy-snapline"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-enemy-snapline-color"));
				DrawSnapLines(TopLeft, BottomRight);
			}
			if (Config::GetBool("visuals-esp-enemy-health"))
			{
				DrawHealth(TopLeft, BottomRight, Ent->GetHealth(), 
					Config::GetColor("visuals-esp-enemy-health-color"), 
					Config::GetColor("visuals-esp-enemy-health-color-background"));
			}
			if (Config::GetBool("visuals-esp-enemy-skeleton"))
			{
				I::surface->DrawSetColor(Config::GetColor("visuals-esp-enemy-skeleton-color")); //white by defualt
				DrawSkeleton(Ent);
			}
			
		}
		
	}
	//DrawBacktrackingDots();
}

void ESP::Run_FrameStageNotify(int stage)
{
	if (stage == FRAME_RENDER_START)
	{
		for (size_t i = 0; i < traces.size(); i++)
		{
			auto cur = traces[i];
			//draw a line from local player's head position to the hit point
			I::debugoverlay->AddLineOverlay(cur.src, cur.end, 125, 125, 1, true, -1);
			//draw a box at the hit point
			I::debugoverlay->AddBoxOverlay(cur.end, Vec(-2, -2, -2), Vec(2, 2, 2), Vec(0, 0, 0), 255, 0, 0, 127, -1.f);

			//if the item is older than 5 seconds, delete it
			if (fabs(I::globalvars->m_curTime - cur.SimTime) > 5.f)
				traces.erase(traces.begin() + i);
		}
		while (traces.size() > 8)
			traces.erase(traces.begin());
	}
}

void ESP::Run_GameEvent(GameEvent* event)
{
	switch (StrHash::HashRuntime(event->GetName())) {
	case StrHash::Hash("bullet_impact"): //0
	{
		/*
		bullet_impact
		Note: Every time a bullet hits something

		Name:	bullet_impact
		Structure:
		short	userid
		float	x
		float	y
		float	z
		*/
		int iUser = event->GetInt("userid");

		if (I::engine->GetPlayerForUserID(iUser) != G::LocalPlayerIndex) return;

		if (!G::LocalPlayer) return;

		if (!Config::GetBool("visuals-misc-tracers")) return;

		BulletTracer trace;
		trace.src = G::LocalPlayer->GetEyePos();
		trace.end =  Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		trace.SimTime = G::LocalPlayer->GetSimulationTime();
		traces.push_back(trace);
	}
	}
}
