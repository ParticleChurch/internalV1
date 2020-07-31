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

void ESP::DrawBoxes(Vec Top, Vec Bottom)
{
	Vec feet;
	Vec head;

	if (!WorldToScreen(Top, head))
		return;

	if (!WorldToScreen(Bottom, feet))
		return;

	float height = feet.y - head.y;
	float width = height / 2.5f;

	Vec topLeft;
	Vec bottomRight;

	topLeft.x = int(feet.x - width / 2);
	topLeft.y = head.y;

	bottomRight.x = int(feet.x + width / 2);
	bottomRight.y = feet.y;

	I::surface->DrawOutlinedRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

void ESP::DrawName(Vec Top, char name[128]) //NEED TO FINISH
{
	Vec head;

	if (!WorldToScreen(Top, head))
		return;
}

void ESP::DrawSnapLines(Vec Bottom)
{
	Vec feet;
	if (!WorldToScreen(Bottom, feet))
		return;

	int xSize;
	int ySize;
	I::engine->GetScreenSize(xSize, ySize);

	I::surface->DrawLine(xSize / 2, 0, feet.x, feet.y);
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
		top.z += 20;

		DrawBoxes(top, bottom);
		DrawName(top, PlayerInfo.name);
		DrawSnapLines(bottom);
	}
}
