#include "Include.hpp"

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

void ESP::DrawName(Vec TL, Vec BR, char name[128]) //NEED TO FINISH
{
	int FontSize = (BR.y - TL.y) / 4;

	static DWORD Font5 = I::surface->FontCreate();
	static DWORD Font6 = I::surface->FontCreate();
	static DWORD Font7 = I::surface->FontCreate();
	static DWORD Font8 = I::surface->FontCreate();
	static DWORD Font9 = I::surface->FontCreate();
	static DWORD Font10 = I::surface->FontCreate();
	static DWORD Font11 = I::surface->FontCreate();
	static DWORD Font12 = I::surface->FontCreate();
	static DWORD Font13 = I::surface->FontCreate();
	static DWORD Font14 = I::surface->FontCreate();
	static DWORD Font15 = I::surface->FontCreate();
	static DWORD Font16 = I::surface->FontCreate();
	static DWORD Font17 = I::surface->FontCreate();
	static DWORD Font18 = I::surface->FontCreate();
	static DWORD Font19 = I::surface->FontCreate();
	static DWORD Font20 = I::surface->FontCreate();
	

	static bool Once = true;

	if (Once)
	{
		Once = false;
		I::surface->SetFontGlyphSet(Font5, "Tahoma", 5, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font6, "Tahoma", 6, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font7, "Tahoma", 7, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font8, "Tahoma", 8, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font9, "Tahoma", 9, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font10, "Tahoma", 10, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font11, "Tahoma", 11, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font12, "Tahoma", 12, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font13, "Tahoma", 13, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font14, "Tahoma", 14, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font15, "Tahoma", 15, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font16, "Tahoma", 16, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font17, "Tahoma", 17, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font18, "Tahoma", 18, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font19, "Tahoma", 19, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		I::surface->SetFontGlyphSet(Font20, "Tahoma", 20, 100, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	}

	switch ((int)FontSize) {
	case 5:
		I::surface->DrawSetTextFont(Font5);
		break;
	case 6:
		I::surface->DrawSetTextFont(Font6);
		break;
	case 7:
		I::surface->DrawSetTextFont(Font7);
		break;
	case 8:
		I::surface->DrawSetTextFont(Font8);
		break;
	case 9:
		I::surface->DrawSetTextFont(Font9);
		break;
	case 10:
		I::surface->DrawSetTextFont(Font10);
		break;
	case 11:
		I::surface->DrawSetTextFont(Font11);
		break;
	case 12:
		I::surface->DrawSetTextFont(Font12);
		break;
	case 13:
		I::surface->DrawSetTextFont(Font13);
		break;
	case 14:
		I::surface->DrawSetTextFont(Font14);
		break;
	case 15:
		I::surface->DrawSetTextFont(Font15);
		break;
	case 16:
		I::surface->DrawSetTextFont(Font16);
		break;
	case 17:
		I::surface->DrawSetTextFont(Font17);
		break;
	case 18:
		I::surface->DrawSetTextFont(Font18);
		break;
	case 19:
		I::surface->DrawSetTextFont(Font19);
		break;
	case 20:
		I::surface->DrawSetTextFont(Font20);
		break;
	default:
		if(FontSize < 5)
			I::surface->DrawSetTextFont(Font5);
		else 
			I::surface->DrawSetTextFont(Font20);
		break;
	}

	if (FontSize < 20)
		TL.y -= FontSize;
	else
		TL.y -= 20;


	std::string TEXT = name;

	static std::wstring wide_string;
	wide_string = std::wstring(TEXT.begin(), TEXT.end());
	
	
	I::surface->DrawSetTextColor(Current);
	I::surface->DrawSetTextPos(TL.x, TL.y);
	I::surface->DrawPrintText(wide_string.c_str(), wcslen(wide_string.c_str()));
}

void ESP::DrawSnapLines(Vec TL, Vec BR)
{
	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);

	I::surface->DrawLine(xSize / 2, ySize, ((BR.x - TL.x) / 2) + TL.x, BR.y);
}

void ESP::Run()
{
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

		Vec bottom = Ent->GetVecOrigin();
		Vec top = Ent->GetBonePos(8);
		top.z += 10;

		Vec feet;
		Vec head;

		if (!WorldToScreen(top, head))
			continue;

		if (!WorldToScreen(bottom, feet))
			continue;

		float height = feet.y - head.y;
		float width = height / 2.5f;

		Vec topLeft;
		Vec bottomRight;

		topLeft.x = int(feet.x - width / 2);
		topLeft.y = head.y;

		bottomRight.x = int(feet.x + width / 2);
		bottomRight.y = feet.y;

		DrawBoxes(topLeft, bottomRight);
		DrawName(topLeft, bottomRight, PlayerInfo.name);
		DrawSnapLines(topLeft, bottomRight);
		
	}
}
