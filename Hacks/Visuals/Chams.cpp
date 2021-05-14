#include "../../Include.hpp"

Chams* chams = new Chams();

void Chams::OverideMat(bool ignorez, int material, float transparent, Color rgba, void* thisptr, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	static Material* current;
	current = GetCurMaterial(material);

	if (material == 7 || material == 5 || material == 3 || material == 4)
		current->FindVar("$envmaptint")->SetVectorValue(rgba.r() / 255.0f,
			rgba.g() / 255.0f,
			rgba.b() / 255.0f);
	else
		current->ColorModulate(
			rgba.r() / 255.0f,
			rgba.g() / 255.0f,
			rgba.b() / 255.0f);

	current->AlphaModulate(transparent);
	current->SetMaterialVarFlag(MaterialVarFlag::IGNOREZ, ignorez);

	I::modelrender->ForcedMaterialOverride(current);
	/*H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);*/
	/*if (ignorez)
	{
		I::modelrender->ForcedMaterialOverride(nullptr);
	}*/
}

void Chams::Init()
{
	static bool DoOnce = true;
	if (!DoOnce)
		return;
	DoOnce = false;

	flat = I::materialsystem->CreateMaterial("flat", KeyValues::FromString("UnlitGeneric", nullptr));
	normal = I::materialsystem->CreateMaterial("normal", KeyValues::FromString("VertexLitGeneric", nullptr));
	glow = I::materialsystem->CreateMaterial("glow", KeyValues::FromString("VertexLitGeneric", "$additive 1 $envmap models/effects/cube_white $envmapfresnel 1 $alpha .7"));
	{
		const auto kv = KeyValues::FromString("VertexLitGeneric", "$envmap editor/cube_vertigo $envmapcontrast 1 $basetexture dev/zone_warning proxies { texturescroll { texturescrollvar $basetexturetransform texturescrollrate 0.6 texturescrollangle 90 } }");
		kv->SetString("$envmaptint", "[.7 .7 .7]");
		animated = I::materialsystem->CreateMaterial("animated", kv);
	}
	{
		const auto kv = KeyValues::FromString("VertexLitGeneric", "$baseTexture detail/dt_metal1 $additive 1 $envmap editor/cube_vertigo");
		kv->SetString("$color", "[.05 .05 .05]");
		glass = I::materialsystem->CreateMaterial("glass", kv);
	}
	{
		const auto kv = KeyValues::FromString("VertexLitGeneric", "$baseTexture black $bumpmap effects/flat_normal $translucent 1 $envmap models/effects/crystal_cube_vertigo_hdr $envmapfresnel 0 $phong 1 $phongexponent 16 $phongboost 2");
		kv->SetString("$phongtint", "[.2 .35 .6]");
		crystal = I::materialsystem->CreateMaterial("crystal", kv);
	}
	chrome = I::materialsystem->CreateMaterial("chrome", KeyValues::FromString("VertexLitGeneric", "$envmap env_cubemap"));
	pearlescent = I::materialsystem->CreateMaterial("pearlescent", KeyValues::FromString("VertexLitGeneric", "$ambientonly 1 $phong 1 $pearlescent 3 $basemapalphaphongmask 1"));
}

void Chams::Run(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	static Config::CState* LegitAA = Config::GetState("antiaim-legit-enable");
	static Config::CState* RageAA = Config::GetState("antiaim-rage-enable");
	static Config::CState* ThirdPerson = Config::GetState("visuals-misc-thirdperson");

	static Config::CState* EnemyVisEnable = Config::GetState("visuals-chams-enemy-visible-enable");
	static Config::CColor* EnemyVisColor = Config::GetColor("visuals-chams-enemy-visible-color");
	static Config::CState* EnemyVisMaterial = Config::GetState("visuals-chams-enemy-visible-material");

	static Config::CState* EnemyHidEnable = Config::GetState("visuals-chams-enemy-hidden-enable");
	static Config::CColor* EnemyHidColor = Config::GetColor("visuals-chams-enemy-hidden-color");
	static Config::CState* EnemyHidMaterial = Config::GetState("visuals-chams-enemy-hidden-material");

	static Config::CState* FriendVisEnable = Config::GetState("visuals-chams-friend-visible-enable");
	static Config::CColor* FriendVisColor = Config::GetColor("visuals-chams-friend-visible-color");
	static Config::CState* FriendVisMaterial = Config::GetState("visuals-chams-friend-visible-material");
							
	static Config::CState* FriendHidEnable = Config::GetState("visuals-chams-friend-hidden-enable");
	static Config::CColor* FriendHidColor = Config::GetColor("visuals-chams-friend-hidden-color");
	static Config::CState* FriendHidMaterial = Config::GetState("visuals-chams-friend-hidden-material");

	static Config::CState* LocalPlayerRealEnable = Config::GetState("visuals-chams-local-real-enable");
	static Config::CColor* LocalPlayerRealColor = Config::GetColor("visuals-chams-local-real-color");
	static Config::CState* LocalPlayerRealMaterial = Config::GetState("visuals-chams-local-real-material");

	static Config::CState* LocalPlayerFakeEnable = Config::GetState("visuals-chams-local-fake-enable");
	static Config::CColor* LocalPlayerFakeColor = Config::GetColor("visuals-chams-local-fake-color");
	static Config::CState* LocalPlayerFakeMaterial = Config::GetState("visuals-chams-local-fake-material");

	static int tick_count = I::globalvars->m_tickCount;
	bool NewTick = false;
	if (tick_count != I::globalvars->m_tickCount)
		NewTick = true;

	bool is_arm = strstr(info.model->name, "arms") != nullptr;
	bool is_player = strstr(info.model->name, "models/player") != nullptr;
	bool is_sleeve = strstr(info.model->name, "sleeve") != nullptr;
	bool is_flash = strstr(info.model->name, "flash") != nullptr;

	static Color color_blocked = Color(200, 100, 100);
	static Color color_visible(100, 200, 100);
	Entity* ent = I::entitylist->GetClientEntity(info.entityIndex);
	Entity* local = I::entitylist->GetClientEntity(I::engine->GetLocalPlayer());
	static player_info_t bInfo;
	bool ValidInfo = I::engine->GetPlayerInfo(info.entityIndex, &bInfo);

	if (ent && local && ent->GetHealth() > 0 && ent->IsPlayer() && ValidInfo)
	{
		L::Verbose("H::DrawModelExecuteHook - drawing a player differently");
		bool isEnemy = ent->GetTeam() != local->GetTeam();
		if (info.entityIndex == I::engine->GetLocalPlayer())
		{	
			
			if ((LegitAA->Get() || RageAA->Get())
				&& LocalPlayerFakeEnable->Get()
				&& ThirdPerson->Get())
			{
				if(!fakelag->LaggingOnPeak)
					RotateBoneMatrix(Vec(0, (antiaim->fake.y - antiaim->real.y), 0), G::LocalPlayer->GetVecOrigin(), customBoneToWorld, antiaim->FakeMatrix);
				
				float alpha = LocalPlayerFakeColor->GetA() / 255.f;
				alpha = G::LocalPlayer->IsScoped() ? alpha / 2 : alpha;
				OverideMat(
						false,	//viz thru wall?
						LocalPlayerFakeMaterial->Get(),		// material
						alpha,	//transparent?
						Color(LocalPlayerFakeColor->GetR(), LocalPlayerFakeColor->GetG(), LocalPlayerFakeColor->GetB()),
						thisptr, ctx, state, info, antiaim->FakeMatrix);
				H::oDrawModelExecute(thisptr, ctx, state, info, antiaim->FakeMatrix);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (LocalPlayerRealEnable->Get())
			{
				float alpha = LocalPlayerRealColor->GetA() / 255.f;
				alpha = G::LocalPlayer->IsScoped() ? alpha / 2 : alpha;
				OverideMat(
					false,	//viz thru wall?
					LocalPlayerRealMaterial->Get(),	//wireframe?
					alpha,	//transparent?
					Color(LocalPlayerRealColor->GetR(), LocalPlayerRealColor->GetG(), LocalPlayerRealColor->GetB()),
					thisptr, ctx, state, info, customBoneToWorld);
			}
		}
		else if (isEnemy)
		{
			if (EnemyHidEnable->Get())
			{
				OverideMat(
					true,	//viz thru wall?
					EnemyHidMaterial->Get(),	//wireframe?
					EnemyHidColor->GetA() / 255.f,	//transparent?
					Color(EnemyHidColor->GetR(), EnemyHidColor->GetG(), EnemyHidColor->GetB()),
					thisptr, ctx, state, info, customBoneToWorld);
				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (EnemyVisEnable->Get())
			{
				if (!lagcomp->PlayerList[bInfo.userid].Records.empty())
				{
					OverideMat(
						true,	//viz thru wall?
						1,		//material?
						0.5,	//transparent?
						Color(255, 0, 0),
						thisptr, ctx, state, info, lagcomp->PlayerList[bInfo.userid].Records.back().Matrix);
					H::oDrawModelExecute(thisptr, ctx, state, info, lagcomp->PlayerList[bInfo.userid].Records.back().Matrix);
					I::modelrender->ForcedMaterialOverride(nullptr);
				}
				

				OverideMat(
					false,	//viz thru wall?
					EnemyVisMaterial->Get(),	//wireframe?
					EnemyVisColor->GetA() / 255.f,	//transparent?
					Color(EnemyVisColor->GetR(), EnemyVisColor->GetG(), EnemyVisColor->GetB()),
					thisptr, ctx, state, info, customBoneToWorld);
			}
		}
		else
		{
			if (FriendHidEnable->Get())
			{
				OverideMat(
					true,	//viz thru wall?
					FriendHidMaterial->Get(),	//wireframe?
					FriendHidColor->GetA() / 255.f,	//transparent?
					Color(FriendHidColor->GetR(), FriendHidColor->GetG(), FriendHidColor->GetB()),
					thisptr, ctx, state, info, customBoneToWorld);
				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (FriendVisEnable->Get())
			{
				OverideMat(
					false,	//viz thru wall?
					FriendVisMaterial->Get(),	//wireframe?
					FriendVisColor->GetA() / 255.f,	//transparent?
					Color(FriendVisColor->GetR(), FriendVisColor->GetG(), FriendHidColor->GetB()),
					thisptr, ctx, state, info, customBoneToWorld);
			}

		}
	}
	
	H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	if(ValidInfo)
		I::modelrender->ForcedMaterialOverride(nullptr);
	
}