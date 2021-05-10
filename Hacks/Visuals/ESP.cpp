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

void ESP::DrawWeapon(Vec TL, Vec BR, Entity* ent)
{
	if (!ent) return;

	static Config::CColor* EnemyWeaponColor = Config::GetColor("visuals-esp-enemy-weapon-color");

	Entity* weap = ent->GetActiveWeapon();
	if (!weap) return;

	WeaponData* weapData = weap->GetWeaponData();
	if (!weapData) return;

	char* name = weapData->Name;
	if (!name) return;

	static DWORD FONT = I::surface->FontCreate();
	static bool Once = true;
	if (Once)
	{
		Once = false;
		I::surface->SetFontGlyphSet(FONT, "Counter-Strike", 14, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	}

	std::string TEXT = name;
	if (TEXT.length() > 13)
	{
		TEXT = TEXT.substr(13);
	}
	else // if potentially bad text, lets just return
		return;

	static std::wstring wide_string;
	wide_string = std::wstring(TEXT.begin(), TEXT.end());

	I::surface->DrawSetTextFont(FONT);
	I::surface->DrawSetTextColor(Color(EnemyWeaponColor->GetR(), EnemyWeaponColor->GetG(), EnemyWeaponColor->GetB(), EnemyWeaponColor->GetA()));
	I::surface->DrawSetTextPos(TL.x, BR.y + 7);
	I::surface->DrawPrintText(wide_string.c_str(), wcslen(wide_string.c_str()));
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

void rotate(std::array< Vec2, 3 >& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}

void ESP::DrawIndicators(Entity* ent, Color clr)
{
	Vec screen;
	Vec eyepos = ent->GetEyePos();
	if (WorldToScreen(eyepos, screen)) // if it's in FOV, return
		return;

	Vec viewangles;
	int width, height;

	I::engine->GetViewAngles(viewangles);
	I::engine->GetScreenSize(width, height);

	// more float division, i'm just gonna stop putting comments next to it at this point xD. ~chance
//	const auto screen_center = vec2_t( width / 2.f, height / 2.f );
	const auto screen_center = Vec(width * .5f, height * .5f, 0);

	const auto angle_yaw_rad = DEG2RAD(viewangles.y - aimbot->CalculateAngle(eyepos).y - 90);

	int radius = 100.f;
	int size = 15.f;

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	//Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()]) : Color(c_config::get().fov_arrows_esp_color_r, c_config::get().fov_arrows_esp_color_g, c_config::get().fov_arrows_esp_color_b, flPlayerAlpha[player->EntIndex()]);

	std::array< Vec2, 3 >points{ Vec2(new_point_x - size, new_point_y - size),
		Vec2(new_point_x + size, new_point_y),
		Vec2(new_point_x - size, new_point_y + size) };

	rotate(points, viewangles.y - aimbot->CalculateAngle(eyepos).y - 90);
	I::surface->draw_filled_triangle(points, clr);
}

void ESP::Run_PaintTraverse()
{
	static Config::CState* EnemyEnable			= Config::GetState("visuals-esp-enemy-enable");
	static Config::CState* EnemyBox			= Config::GetState("visuals-esp-enemy-bbox");
	static Config::CColor* EnemyBoxColor		= Config::GetColor("visuals-esp-enemy-bbox-color");
	static Config::CState* EnemyName			= Config::GetState("visuals-esp-enemy-name");
	static Config::CColor* EnemyNameColor		= Config::GetColor("visuals-esp-enemy-name-color");
	static Config::CState* EnemyIndicator		= Config::GetState("visuals-esp-enemy-indicator");
	static Config::CColor* EnemyIndicatorColor = Config::GetColor("visuals-esp-enemy-indicator-color");
	static Config::CState* EnemySkeleton		= Config::GetState("visuals-esp-enemy-skeleton");
	static Config::CColor* EnemySkeletonColor  = Config::GetColor("visuals-esp-enemy-skeleton-color");
	static Config::CState* EnemyHealth			= Config::GetState("visuals-esp-enemy-health");
	static Config::CColor* EnemyHealthFGColor	= Config::GetColor("visuals-esp-enemy-health-color");
	static Config::CColor* EnemyHealthBGColor	= Config::GetColor("visuals-esp-enemy-health-color-background");
	static Config::CState* EnemyResolver		= Config::GetState("visuals-esp-enemy-resolverflags");
	static Config::CColor* EnemyResolverColor  = Config::GetColor("visuals-esp-enemy-resolverflags-color");
	static Config::CState* EnemyWeapon			= Config::GetState("visuals-esp-enemy-weapon");
	

	static Config::CState* FriendEnable			= Config::GetState("visuals-esp-friend-enable");
	static Config::CState* FriendBox				= Config::GetState("visuals-esp-friend-bbox");
	static Config::CColor* FriendBoxColor			= Config::GetColor("visuals-esp-friend-bbox-color");
	static Config::CState* FriendName				= Config::GetState("visuals-esp-friend-name");
	static Config::CColor* FriendNameColor			= Config::GetColor("visuals-esp-friend-name-color");
	static Config::CState* FriendSkeleton			= Config::GetState("visuals-esp-friend-skeleton");
	static Config::CColor* FriendSkeletonColor		= Config::GetColor("visuals-esp-friend-skeleton-color");
	static Config::CState* FriendHealth			= Config::GetState("visuals-esp-friend-health");
	static Config::CColor* FriendHealthFGColor		= Config::GetColor("visuals-esp-friend-health-color");
	static Config::CColor* FriendHealthBGColor		= Config::GetColor("visuals-esp-friend-health-color-background");

	antiaim->Visualize();
	I::surface->DrawSetColor(Color(255.f, 255.f, 255.f, 255.f));
	/*DrawBacktrackingDots();*/

	static DWORD FONT = I::surface->FontCreate();
	static bool Once = true;
	if (Once)
	{
		Once = false;
		I::surface->SetFontGlyphSet(FONT, "Tahoma", 20, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
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
			if (!FriendEnable->Get())
				continue;

			if (FriendBox->Get())
			{
				I::surface->DrawSetColor(Color(FriendBoxColor->GetR(), FriendBoxColor->GetG(), FriendBoxColor->GetB(), FriendBoxColor->GetA()));
				DrawBoxes(TopLeft, BottomRight);
			}
			if (FriendName->Get())
			{
				DrawName(TopLeft, BottomRight, PlayerInfo.name, Color(FriendNameColor->GetR(), FriendNameColor->GetG(), FriendNameColor->GetB(), FriendNameColor->GetA()));
			}
			if (FriendHealth->Get())
			{
				DrawHealth(TopLeft, BottomRight, Ent->GetHealth(),
					Color(FriendHealthFGColor->GetR(), FriendHealthFGColor->GetG(), FriendHealthFGColor->GetB(), FriendHealthFGColor->GetA()),
					Color(FriendHealthBGColor->GetR(), FriendHealthBGColor->GetG(), FriendHealthBGColor->GetB(), FriendHealthBGColor->GetA()));
			}
			if (FriendSkeleton->Get())
			{
				I::surface->DrawSetColor(Color(FriendSkeletonColor->GetR(), FriendSkeletonColor->GetG(), FriendSkeletonColor->GetB(), FriendSkeletonColor->GetA())); //white by defualt
				DrawSkeleton(Ent);
			}
		}
		else
		{
			//if enemy esp not enabled
			if (!EnemyEnable->Get())
				continue;

			// DO FUCKING RESOLVER FLAGS
			static DWORD FONT = I::surface->FontCreate();
			static bool Once = true;
			if (Once)
			{
				Once = false;
				I::surface->SetFontGlyphSet(FONT, "Tahoma", 14, 1, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
			}

			//resolver->PlayerInfo[PlayerInfo.userid]
			if (resolver->PlayerInfo.find(PlayerInfo.userid) != resolver->PlayerInfo.end() && EnemyResolver->Get())
			{
				// ADD CHECK HERE
				std::string TEXT = resolver->PlayerInfo[PlayerInfo.userid].ResolverFlag;
				//TEXT += "\n" + std::to_string(I::globalvars->m_curTime - G::EntList[PlayerInfo.userid].LastShotTime);
				static std::wstring wide_string;
				wide_string = std::wstring(TEXT.begin(), TEXT.end());

				I::surface->DrawSetTextFont(FONT);
				I::surface->DrawSetTextColor(Color(EnemyResolverColor->GetR(), EnemyResolverColor->GetG(), EnemyResolverColor->GetB(), EnemyResolverColor->GetA()));
				I::surface->DrawSetTextPos(BottomRight.x + 8, (TopLeft.y + BottomRight.y) / 2);
				I::surface->DrawPrintText(wide_string.c_str(), wcslen(wide_string.c_str()));
			}
			

			if (EnemyBox->Get())
			{
				I::surface->DrawSetColor(Color(EnemyBoxColor->GetR(), EnemyBoxColor->GetG(), EnemyBoxColor->GetB(), EnemyBoxColor->GetA()));
				DrawBoxes(TopLeft, BottomRight);
			}
			if (EnemyName->Get())
			{
				DrawName(TopLeft, BottomRight, PlayerInfo.name, Color(EnemyNameColor->GetR(), EnemyNameColor->GetG(), EnemyNameColor->GetB(), EnemyNameColor->GetA()));
			}
			if (EnemyIndicator->Get())
			{
				DrawIndicators(Ent, Color(EnemyIndicatorColor->GetR(), EnemyIndicatorColor->GetG(), EnemyIndicatorColor->GetB(), EnemyIndicatorColor->GetA()));
			}
			if (EnemyHealth->Get())
			{
				DrawHealth(TopLeft, BottomRight, Ent->GetHealth(), 
					Color(EnemyHealthFGColor->GetR(), EnemyHealthFGColor->GetG(), EnemyHealthFGColor->GetB(), EnemyHealthFGColor->GetA()),
					Color(EnemyHealthBGColor->GetR(), EnemyHealthBGColor->GetG(), EnemyHealthBGColor->GetB(), EnemyHealthBGColor->GetA()));
			}
			if (EnemySkeleton->Get())
			{
				I::surface->DrawSetColor(Color(EnemySkeletonColor->GetR(), EnemySkeletonColor->GetG(), EnemySkeletonColor->GetB(), EnemySkeletonColor->GetA())); //white by defualt
				DrawSkeleton(Ent);
			}
			if (EnemyWeapon->Get())
			{
				DrawWeapon(TopLeft, BottomRight, Ent);
			}
			
		}
		
	}
	//DrawBacktrackingDots();
}

void ESP::RunFSN()
{
	// draw bullet tracers (traces = newer bullets at end of list)
	// only show most recent 8 tracers
	while (traces.size() > 8)
		traces.erase(traces.begin());
	for (int i = (int)traces.size() - 1; i >= 0; --i)
	{
		BulletTracer& bt = traces.at(i);

		// delete if older than 5 seconds
		if (I::globalvars->m_curTime - bt.SimTime > 5.f)
		{
			traces.erase(traces.begin() + i);
			continue;
		}

		// draw bullet tracer
		I::debugoverlay->AddLineOverlay(bt.src, bt.end, 125, 125, 1, true, -1);
		// draw penetration as cube
		I::debugoverlay->AddBoxOverlay(bt.end, Vec(-2, -2, -2), Vec(2, 2, 2), Vec(0, 0, 0), 255, 0, 0, 127, -1.f);

	}

	// draw bullet penetrations
	// only most recent 10
	while (points.size() > 10)
		points.erase(points.begin());
	for (size_t i = 0; i < points.size(); i++)
	{
		// cube at the position
		I::debugoverlay->AddBoxOverlay(points.at(i), Vec(-1, -1, -1), Vec(1, 1, 1), Vec(0, 0, 0), 0, 255, 255, 127, -1.f);
	}
}

void ESP::Run_GameEvent(GameEvent* event)
{
	static Config::CState* VisTracers = Config::GetState("visuals-misc-tracers");

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

		if (!VisTracers->Get()) return;

		BulletTracer trace;
		trace.src = G::LocalPlayer->GetEyePos();
		trace.end =  Vec(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		trace.SimTime = G::LocalPlayer->GetSimulationTime();
		traces.push_back(trace);
	}
	}
}
