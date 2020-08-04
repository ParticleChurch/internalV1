#include "../Include.hpp"

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

void ESP::DrawName(Vec TL, Vec BR, char Name[128]) //NEED TO FINISH
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
	I::surface->DrawSetTextColor(Current);
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

void ESP::DrawHealth(Vec TL, Vec BR, int Health)
{
	int Height = BR.y - TL.y;

	int HeightBar = (int)(Height * (Health / 100.0f));
	int WidthBar = (int)(3 + Height / 100.0f);

	I::surface->DrawSetColor(0, 255, 0, 255);		//green
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
	for (int i = 1; i < 65; i++) {
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
					if (!tick.Shooting)
						I::surface->DrawSetColor(Color(255, 255, 255, 255));
					else
						I::surface->DrawSetColor(Color(255, 0, 0, 255));
					I::surface->DrawLine(screen.x, screen.y, LastScreen.x , LastScreen.y);
					LastScreen = screen;
				}
			}
		}
	}
}

void ESP::DrawTraces()
{
	Vec Screen1;
	Vec Screen2;
	for (auto Trace : traces)
	{
		bool ST = WorldToScreen(Trace.Startpos, Screen1);
		bool END = WorldToScreen(Trace.Endpos, Screen2);
		if (ST && END)
		{
			I::surface->DrawSetColor(Color(255, 0, 0, 255));
			I::surface->DrawLine(Screen1.x, Screen1.y, Screen2.x, Screen2.y);
		}

		if (END)
		{
			I::surface->DrawSetColor(Color(0, 0, 255, 200));
			I::surface->DrawOutlinedCircle(Screen2.x, Screen2.y, 4, 8);
		}

	}
}

void ESP::Run()
{
	
	for (auto P : H::points)
	{
		Vec screen;
		if (WorldToScreen(P, screen))
		{
			static int lol = 0;
			if (lol > 255)
				lol = 0;
			lol++;
			I::surface->DrawSetColor(Color(255.f - lol, lol, 255.f, 255.f));
			I::surface->DrawOutlinedCircle(screen.x, screen.y, 4, 8);
		}
	}
	I::surface->DrawSetColor(Color(255.f, 255.f, 255.f, 255.f));

	for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
	{
		if (i == I::engine->GetLocalPlayer())
			continue;

		Entity* Ent = I::entitylist->GetClientEntity(i);
		if (!Ent)
			continue;

		player_info_t PlayerInfo;
		if (!I::engine->GetPlayerInfo(i, &PlayerInfo))
			continue;

		if (Ent->GetTeam() == I::entitylist->GetClientEntity(I::engine->GetLocalPlayer())->GetTeam())
			continue;

		if (!(Ent->GetHealth() > 0))
			continue;

		if (Ent->IsDormant())
			continue;

		/*
		//works only on workshop map idk why
		if (Ent->GetGunGameImmunity())
			continue;
			*/

		Vec Bottom = Ent->GetVecOrigin();
		Vec Top = Ent->GetBonePos(8);
		Top.z += 10;

		Vec Feet;
		Vec Head;

		if (!WorldToScreen(Top, Head))
			continue;

		if (!WorldToScreen(Bottom, Feet))
			continue;

		float Height = Feet.y - Head.y;
		float Width = Height / 2.5f;

		Vec TopLeft;
		Vec BottomRight;

		TopLeft.x = int(Feet.x - Width / 2);
		TopLeft.y = Head.y;

		BottomRight.x = int(Feet.x + Width / 2);
		BottomRight.y = Feet.y;

		I::surface->DrawSetColor(255, 255, 255, 255); //white
		DrawBoxes(TopLeft, BottomRight);
		DrawName(TopLeft, BottomRight, PlayerInfo.name);
		DrawSnapLines(TopLeft, BottomRight);
		DrawSkeleton(Ent);
		DrawHealth(TopLeft, BottomRight, Ent->GetHealth());
		
	}
	DrawTraces();
	DrawBacktrackingDots();

}
