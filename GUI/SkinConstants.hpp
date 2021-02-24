#pragma once
#include <vector>
#include <string>

namespace Skins
{
	enum class Weapon : int
	{
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
		Nomad = 0,
		Skeleton,
		Survival,
		Paracord,
		Classic,
		Bowie,
		Butterfly,
		Falchion,
		Flip,
		Gut,
		Huntsman,
		Karambit,
		M9Bayonet,
		Navaja,
		ShadowDaggers,
		Stiletto,
		Talon,
		Ursus,
	};
	extern std::vector<std::string> KnifeNames;

	enum class Glove {
		// todo
	};
	extern std::vector<std::string> GloveNames;

	class PaintKit
	{
	public:
		int ID = 0;
		std::string Name;
		std::string VisibleName;

		// this paint kit is *supposed* to be applied to:
		std::vector<Weapon> Weapons;
		std::vector<Knife> Knives;
		std::vector<Glove> Gloves;

		PaintKit(
			int ID,
			std::string Name, std::string VisibleName,
			std::vector<Weapon> Weapons = {},
			std::vector<Knife> Knives = {},
			std::vector<Glove> Gloves = {}
		)
		{
			this->ID = ID;
			this->Name = Name;
			this->VisibleName = VisibleName;
			this->Weapons = Weapons;
			this->Knives = Knives;
			this->Gloves = Gloves;
		}
	};
	extern std::vector<PaintKit> PaintKits;

	//extern std::vector<std::vector<std::string>> WeaponGroups;
}