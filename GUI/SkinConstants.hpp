#pragma once
#include <vector>
#include <string>
#include "TextService.hpp"
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
		Bayonet,
		M9Bayonet,
		Navaja,
		ShadowDaggers,
		Stiletto,
		Talon,
		Ursus,
		TDefault,
		CTDefault,
		_COUNT,
	};
	extern std::vector<std::string> KnifeNames;

	enum class Glove {
		// todo
		_COUNT
	};
	extern std::vector<std::string> GloveNames;

	class PaintKit
	{
	private:
		uint64_t RawWeapons;
		uint64_t RawKnives;
		uint64_t RawGloves;

	public:
		int ID = 0;
		std::string VisibleName;

		// this paint kit is *supposed* to be applied to:
		// these vectors should be built at runtime to save dll size
		std::vector<Weapon> Weapons;
		std::vector<Knife> Knives;
		std::vector<Glove> Gloves;

		PaintKit(
			int ID,
			std::string VisibleName,
			uint64_t Weapons = 0,
			uint64_t Knives = 0,
			uint64_t Gloves = 0
		)
		{
			this->ID = ID;
			this->VisibleName = VisibleName;

			this->RawWeapons = Weapons;
			this->Weapons = {};
			for (uint64_t i = 0; i < (uint64_t)Weapon::_COUNT; i++)
				if (this->RawWeapons & (((uint64_t)1) << i))
					this->Weapons.push_back((Weapon)i);

			this->RawKnives = Knives;
			this->Knives = {};
			for (uint64_t i = 0; i < (uint64_t)Knife::_COUNT; i++)
				if (this->RawKnives & (((uint64_t)1) << i))
					this->Knives.push_back((Knife)i);

			this->RawGloves = Gloves;
			this->Gloves = {};
			for (uint64_t i = 0; i < (uint64_t)Glove::_COUNT; i++)
				if (this->RawGloves & (((uint64_t)1) << i))
					this->Gloves.push_back((Glove)i);
		}
	};
	extern std::vector<PaintKit> PaintKits;

	//extern std::vector<std::vector<std::string>> WeaponGroups;

	extern constexpr Weapon WeaponFromId(WeaponId id);
	extern constexpr Knife KnifeFromId(WeaponId id);
	extern constexpr WeaponId IdFromKnife(Knife k);
	extern constexpr const char* GetKnifeModel(Knife knife);
	extern PaintKit* PaintKitFromID(int ID);
}