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

	if (ent && local && ent->GetHealth() > 0 && ent->IsPlayer() && I::engine->GetPlayerInfo(info.entityIndex, &bInfo))
	{
		bool isEnemy = ent->GetTeam() != local->GetTeam();
		if (info.entityIndex == I::engine->GetLocalPlayer())
		{	
			if ((Config::GetBool("antiaim-legit-enable") || Config::GetBool("antiaim-rage-enable"))
				&& Config::GetBool("visuals-chams-localplayer-fake-enable") 
				&& Config::GetBool("visuals-misc-thirdperson"))
			{
				if(NewTick && G::pSendPacket && *G::pSendPacket && !fakelag->LaggingOnPeak)
					RotateBoneMatrix(Vec(0, antiaim->fake.y - antiaim->real.y, 0), G::LocalPlayer->GetVecOrigin(), customBoneToWorld, antiaim->FakeMatrix);
				OverideMat(
						false,	//viz thru wall?
						Config::GetState("visuals-chams-localplayer-fake-material"),		// material
						Config::GetFloat("visuals-chams-localplayer-fake-opacity") / 100.f,	//transparent?
						Config::GetColor("visuals-chams-localplayer-fake-color"),
						thisptr, ctx, state, info, antiaim->FakeMatrix);
				H::oDrawModelExecute(thisptr, ctx, state, info, antiaim->FakeMatrix);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (Config::GetBool("visuals-chams-localplayer-real-enable"))
			{
				OverideMat(
					false,	//viz thru wall?
					Config::GetState("visuals-chams-localplayer-real-material"),	//wireframe?
					Config::GetFloat("visuals-chams-localplayer-real-opacity") / 100.f,	//transparent?
					Config::GetColor("visuals-chams-localplayer-real-color"),
					thisptr, ctx, state, info, customBoneToWorld);
			}
		}
		else if (isEnemy)
		{
			if (Config::GetBool("visuals-chams-enemy-hidden-enable"))
			{
				OverideMat(
					true,	//viz thru wall?
					Config::GetState("visuals-chams-enemy-hidden-material"),	//wireframe?
					Config::GetFloat("visuals-chams-enemy-hidden-opacity") / 100.f,	//transparent?
					Config::GetColor("visuals-chams-enemy-hidden-color"),
					thisptr, ctx, state, info, customBoneToWorld);
				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (Config::GetBool("visuals-chams-enemy-visible-enable"))
			{
				OverideMat(
					false,	//viz thru wall?
					Config::GetState("visuals-chams-enemy-visible-material"),	//wireframe?
					Config::GetFloat("visuals-chams-enemy-visible-opacity") / 100.f,	//transparent?
					Config::GetColor("visuals-chams-enemy-visible-color"),
					thisptr, ctx, state, info, customBoneToWorld);
			}
		}
		else
		{
			if (Config::GetBool("visuals-chams-friend-hidden-enable"))
			{
				OverideMat(
					true,	//viz thru wall?
					Config::GetState("visuals-chams-friend-hidden-material"),	//wireframe?
					Config::GetFloat("visuals-chams-friend-hidden-opacity") / 100.f,	//transparent?
					Config::GetColor("visuals-chams-friend-hidden-color"),
					thisptr, ctx, state, info, customBoneToWorld);
				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
				I::modelrender->ForcedMaterialOverride(nullptr);
			}

			if (Config::GetBool("visuals-chams-friend-visible-enable"))
			{
				OverideMat(
					false,	//viz thru wall?
					Config::GetState("visuals-chams-friend-visible-material"),	//wireframe?
					Config::GetFloat("visuals-chams-friend-visible-opacity") / 100.f,	//transparent?
					Config::GetColor("visuals-chams-friend-visible-color"),
					thisptr, ctx, state, info, customBoneToWorld);
			}

		}
	}
	
	H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	I::modelrender->ForcedMaterialOverride(nullptr);
	
}