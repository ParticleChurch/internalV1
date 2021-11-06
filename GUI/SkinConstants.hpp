#pragma once
#include <vector>
#include <string>
#include "../Utils/TextService.hpp"
#include "KnifeSequence.hpp"
#include "../Valve/Custom/WeaponId.hpp"

namespace Skins
{
	enum class Weapon : uint64_t
	{
		INVALID = -1,
		Glock18 = 0,
		P2000,
		USPS,
		DualBerettas,
		P250,
		Tec9,
		FiveSeven,
		CZ75,
		DesertEagle,
		R8Revolver,
		Nova,
		XM1014,
		SawedOff,
		MAG7,
		M249,
		Negev,
		MP9,
		MAC10,
		MP7,
		MP5,
		UMP45,
		P90,
		PPBizon,
		FAMAS,
		Galil,
		M4A4,
		AK47,
		M4A1S,
		SSG08,
		AUG,
		SG553,
		AWP,
		SCAR20,
		G3SG1,
		Zeus,
		C4,
		Incendiary,
		Molotov,
		Decoy,
		Flashbang,
		HE,
		Smoke,
		_COUNT,
	};
	extern std::vector<std::string> WeaponNames;

	enum class WeaponGroup
	{
		Pistol = 0,
		Heavy,
		SMG,
		Rifle,
		Other,
		_COUNT
	};
	extern std::vector<std::string> WeaponGroupNames;

	// WeaponGroupWeapons[WeaponGroup][Weapon]
	extern std::vector<std::vector<Weapon>> WeaponGroupWeapons;

	enum class Knife : int
	{
		INVALID = -1,
		CTDefault = 0,
		TDefault,
		Bayonet,
		Bowie,
		Butterfly,
		Classic,
		Falchion,
		Flip,
		Gut,
		Huntsman,
		Karambit,
		M9Bayonet,
		Navaja,
		Nomad,
		Paracord,
		ShadowDaggers,
		Skeleton,
		Stiletto,
		Survival,
		Talon,
		Ursus,
		_COUNT,
	};
	extern std::vector<std::string> KnifeNames;

	enum class Glove {
		// todo
		INVALID = -1,
		_COUNT
	};
	extern std::vector<std::string> GloveNames;

	struct PaintKit
	{
		const int ID = 0;
		const char* VisibleName;

		// this paint kit is *supposed* to be applied to:
		// these vectors should be built at runtime to save dll size
		const Weapon* Weapons;
		const Knife* Knives;
		const Glove* Gloves;

		constexpr PaintKit(
			const int ID,
			const char* VisibleName,
			const Weapon Weapons[],
			const Knife Knives[],
			const Glove Gloves[]
		) : ID(ID), VisibleName(VisibleName), Weapons(Weapons), Knives(Knives), Gloves(Gloves)
		{}
	};
	constexpr size_t NPaintKits = 884;
	extern const PaintKit PaintKits[];

	//extern std::vector<std::vector<std::string>> WeaponGroups;

	extern constexpr Weapon WeaponFromId(WeaponId id);
	extern constexpr Knife KnifeFromId(WeaponId id);
	extern constexpr WeaponId IdFromKnife(Knife k);
	extern constexpr const char* GetKnifeModel(Knife knife);
	extern const PaintKit* PaintKitFromID(int ID);
}