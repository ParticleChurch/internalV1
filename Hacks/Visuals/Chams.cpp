#include "../../Include.hpp"

Chams* chams = new Chams();

void Chams::OverideMat(bool ignorez, int material, float transparent, Color rgba, void* thisptr, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld)
{
	static Material* current;
	current = GetCurMaterial(material);
	current->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	current->AlphaModulate(transparent);
	current->SetMaterialVarFlag(MaterialVarFlag::IGNOREZ, ignorez);

	I::modelrender->ForcedMaterialOverride(current);
	H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	if (ignorez)
	{
		I::modelrender->ForcedMaterialOverride(nullptr);
	}
}

void Chams::Init()
{
	static bool DoOnce = true;
	if (!DoOnce)
		return;
	DoOnce = false;

	flat = I::materialsystem->CreateMaterial("flat", KeyValues::FromString("UnlitGeneric", nullptr));
	normal = I::materialsystem->CreateMaterial("normal", KeyValues::FromString("VertexLitGeneric", nullptr));
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
		/*	I::modelrender->ForcedMaterialOverride(nullptr);
			static Matrix3x4 BoneMatrix[MAXSTUDIOBONES];
			RotateBoneMatrix(Vec(0, antiaim->real.y - G::CM_StartAngle.y, 0), G::Localplayer->GetVecOrigin(), customBoneToWorld, BoneMatrix);
			H::oDrawModelExecute(thisptr, ctx, state, info, BoneMatrix);*/

			//H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);

			//static Color color_desync = Color(255, 255, 255);
			//OverideMat(
			//	false,	//viz thru wall?
			//	false,	//wireframe?
			//	false,	//transparent?
			//	color_desync,
			//	thisptr, ctx, state, info, G::FakeMatrix);

			//H::oDrawModelExecute(thisptr, ctx, state, info, G::FakeMatrix);
			H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
		}
		else if (isEnemy)
		{
			if (Config::GetBool("visuals-chams-enemy-hidden-enable"))
			{
				OverideMat(
					true,	//viz thru wall?
					Config::GetState("visuals-chams-enemy-hidden-material"),	//wireframe?
					1.f,	//transparent?
					Config::GetColor("visuals-chams-enemy-hidden-color"),
					thisptr, ctx, state, info, customBoneToWorld);

				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}

			if (Config::GetBool("visuals-chams-enemy-visible-enable"))
			{
				OverideMat(
					false,	//viz thru wall?
					Config::GetState("visuals-chams-enemy-visible-material"),	//wireframe?
					1.f,	//transparent?
					Config::GetColor("visuals-chams-enemy-visible-color"),
					thisptr, ctx, state, info, customBoneToWorld);

				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}
			else
			{
				return H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}

		}
		else
		{
			if (Config::GetBool("visuals-chams-friend-hidden-enable"))
			{
				OverideMat(
					true,	//viz thru wall?
					Config::GetState("visuals-chams-friend-hidden-material"),	//wireframe?
					1.f,	//transparent?
					Config::GetColor("visuals-chams-friend-hidden-color"),
					thisptr, ctx, state, info, customBoneToWorld);

				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}

			if (Config::GetBool("visuals-chams-friend-visible-enable"))
			{
				OverideMat(
					false,	//viz thru wall?
					Config::GetState("visuals-chams-friend-visible-material"),	//wireframe?
					1.f,	//transparent?
					Config::GetColor("visuals-chams-friend-visible-color"),
					thisptr, ctx, state, info, customBoneToWorld);

				H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}
			else
			{
				return H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
			}
		}
	}
	else
	{
		//I::modelrender->ForcedMaterialOverride(nullptr);
		return H::oDrawModelExecute(thisptr, ctx, state, info, customBoneToWorld);
	}
}
