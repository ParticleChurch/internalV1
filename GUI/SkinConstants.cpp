#include "SkinConstants.hpp"


namespace Skins
{

	std::vector<std::string> WeaponNames = {
		"Glock-18",
		"P2000",
		"USP-S",
		"Dual Berettas",
		"P250",
		"Tec-9",
		"Five-Seven",
		"CZ-75",
		"Desert Eagle",
		"R8 Revolver",
		"Nova",
		"XM1014",
		"Sawed-Off",
		"MAG-7",
		"M249",
		"Negev",
		"MP9",
		"MAC-10",
		"MP7",
		"MP5",
		"UMP-45",
		"P90",
		"PP-Bizon",
		"FAMAS",
		"Galil",
		"M4A4",
		"AK-47",
		"M4A1-S",
		"SSG 08",
		"AUG",
		"SG 553",
		"AWP",
		"SCAR-20",
		"G3SG1",
		"Zeus x27",
		"C4",
		"Incendiary",
		"Molotov",
		"Decoy",
		"Flashbang",
		"HE Grenade",
		"Smoke"
	};

	std::vector<std::string> WeaponGroupNames = {
		"Pistol",
		"Heavy",
		"SMG",
		"Rifle",
		"Other",
	};

	std::vector<std::vector<Weapon>> WeaponGroupWeapons = {
		// Pistol
		{
			Weapon::Glock18,
			Weapon::P2000,
			Weapon::USPS,
			Weapon::DualBerettas,
			Weapon::P250,
			Weapon::Tec9,
			Weapon::FiveSeven,
			Weapon::CZ75,
			Weapon::DesertEagle,
			Weapon::R8Revolver,
		},
		// Heavy
		{
			Weapon::Nova,
			Weapon::XM1014,
			Weapon::SawedOff,
			Weapon::MAG7,
			Weapon::M249,
			Weapon::Negev,
		},
		// SMG
		{
			Weapon::MP9,
			Weapon::MAC10,
			Weapon::MP7,
			Weapon::MP5,
			Weapon::UMP45,
			Weapon::P90,
			Weapon::PPBizon,
		},
		// Rifle
		{
			Weapon::FAMAS,
			Weapon::Galil,
			Weapon::M4A4,
			Weapon::AK47,
			Weapon::M4A1S,
			Weapon::SSG08,
			Weapon::AUG,
			Weapon::SG553,
			Weapon::AWP,
			Weapon::SCAR20,
			Weapon::G3SG1,
		},
		// Other
		{
			Weapon::C4,
			Weapon::Incendiary,
			Weapon::Molotov,
			Weapon::Decoy,
			Weapon::Flashbang,
			Weapon::HE,
			Weapon::Smoke,
		},
	};

	std::vector<std::string> KnifeNames = {
        "CT Default",
        "T Default",
        "Bayonet",
        "Bowie",
        "Butterfly",
        "Classic",
        "Falchion",
        "Flip",
        "Gut",
        "Huntsman",
        "Karambit",
        "M9 Bayonet",
        "Navaja",
        "Nomad",
        "Paracord",
        "ShadowDaggers",
        "Skeleton",
        "Stiletto",
        "Survival",
        "Talon",
        "Ursus",
	};

	std::vector<std::string> GloveNames = {

	};

    constexpr Weapon __pkw_empty[]{ Weapon::INVALID };
    constexpr Knife __pkk_empty[]{ Knife::INVALID };
    constexpr Glove __pkg_empty[]{ Glove::INVALID };

    constexpr Weapon __pkw2[]{ Weapon::Glock18, Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk2 = __pkk_empty; constexpr auto __pkg2 = __pkg_empty;
    constexpr Weapon __pkw3[]{ Weapon::Glock18, Weapon::FiveSeven, Weapon::Nova, Weapon::MAC10, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk3 = __pkk_empty; constexpr auto __pkg3 = __pkg_empty;
    constexpr Weapon __pkw5[]{ Weapon::SawedOff, Weapon::MP7, Weapon::INVALID }; constexpr Knife __pkk5[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg5 = __pkg_empty;
    constexpr Weapon __pkw6[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk6 = __pkk_empty; constexpr auto __pkg6 = __pkg_empty;
    constexpr Weapon __pkw8[]{ Weapon::M4A4, Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk8 = __pkk_empty; constexpr auto __pkg8 = __pkg_empty;
    constexpr Weapon __pkw9[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk9 = __pkk_empty; constexpr auto __pkg9 = __pkg_empty;
    constexpr Weapon __pkw10[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk10 = __pkk_empty; constexpr auto __pkg10 = __pkg_empty;
    constexpr Weapon __pkw11[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk11 = __pkk_empty; constexpr auto __pkg11 = __pkg_empty;
    constexpr Weapon __pkw12[]{ Weapon::CZ75, Weapon::R8Revolver, Weapon::INVALID }; constexpr Knife __pkk12[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg12 = __pkg_empty;
    constexpr Weapon __pkw13[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk13 = __pkk_empty; constexpr auto __pkg13 = __pkg_empty;
    constexpr Weapon __pkw14[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk14 = __pkk_empty; constexpr auto __pkg14 = __pkg_empty;
    constexpr Weapon __pkw15[]{ Weapon::P250, Weapon::MP7, Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk15 = __pkk_empty; constexpr auto __pkg15 = __pkg_empty;
    constexpr Weapon __pkw16[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk16 = __pkk_empty; constexpr auto __pkg16 = __pkg_empty;
    constexpr Weapon __pkw17[]{ Weapon::Tec9, Weapon::DesertEagle, Weapon::MAC10, Weapon::UMP45, Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk17 = __pkk_empty; constexpr auto __pkg17 = __pkg_empty;
    constexpr Weapon __pkw20[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk20 = __pkk_empty; constexpr auto __pkg20 = __pkg_empty;
    constexpr Weapon __pkw21[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk21 = __pkk_empty; constexpr auto __pkg21 = __pkg_empty;
    constexpr Weapon __pkw22[]{ Weapon::M249, Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk22 = __pkk_empty; constexpr auto __pkg22 = __pkg_empty;
    constexpr Weapon __pkw25[]{ Weapon::USPS, Weapon::Nova, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk25 = __pkk_empty; constexpr auto __pkg25 = __pkg_empty;
    constexpr Weapon __pkw26[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk26 = __pkk_empty; constexpr auto __pkg26 = __pkg_empty;
    constexpr Weapon __pkw27[]{ Weapon::P250, Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk27 = __pkk_empty; constexpr auto __pkg27 = __pkg_empty;
    constexpr Weapon __pkw28[]{ Weapon::DualBerettas, Weapon::Negev, Weapon::MP7, Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk28 = __pkk_empty; constexpr auto __pkg28 = __pkg_empty;
    constexpr Weapon __pkw30[]{ Weapon::SawedOff, Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk30 = __pkk_empty; constexpr auto __pkg30 = __pkg_empty;
    constexpr Weapon __pkw32[]{ Weapon::P2000, Weapon::CZ75, Weapon::MAG7, Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk32 = __pkk_empty; constexpr auto __pkg32 = __pkg_empty;
    constexpr Weapon __pkw33[]{ Weapon::MP9, Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk33 = __pkk_empty; constexpr auto __pkg33 = __pkg_empty;
    constexpr Weapon __pkw34[]{ Weapon::P250, Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk34 = __pkk_empty; constexpr auto __pkg34 = __pkg_empty;
    constexpr Weapon __pkw36[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk36 = __pkk_empty; constexpr auto __pkg36 = __pkg_empty;
    constexpr Weapon __pkw37[]{ Weapon::DesertEagle, Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk37 = __pkk_empty; constexpr auto __pkg37 = __pkg_empty;
    constexpr Weapon __pkw38[]{ Weapon::Glock18, Weapon::MAC10, Weapon::INVALID }; constexpr Knife __pkk38[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg38 = __pkg_empty;
    constexpr Weapon __pkw39[]{ Weapon::MAG7, Weapon::MP9, Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk39 = __pkk_empty; constexpr auto __pkg39 = __pkg_empty;
    constexpr Weapon __pkw40[]{ Weapon::Glock18, Weapon::DesertEagle, Weapon::R8Revolver, Weapon::INVALID }; constexpr Knife __pkk40[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::ShadowDaggers, Knife::INVALID }; constexpr auto __pkg40 = __pkg_empty;
    constexpr Weapon __pkw41[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk41 = __pkk_empty; constexpr auto __pkg41 = __pkg_empty;
    constexpr Weapon __pkw42[]{ Weapon::XM1014, Weapon::INVALID }; constexpr Knife __pkk42[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg42 = __pkg_empty;
    constexpr Weapon __pkw43[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr Knife __pkk43[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg43 = __pkg_empty;
    constexpr Weapon __pkw44[]{ Weapon::FiveSeven, Weapon::AK47, Weapon::INVALID }; constexpr Knife __pkk44[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg44 = __pkg_empty;
    constexpr Weapon __pkw46[]{ Weapon::DualBerettas, Weapon::FiveSeven, Weapon::AUG, Weapon::SCAR20, Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk46 = __pkk_empty; constexpr auto __pkg46 = __pkg_empty;
    constexpr Weapon __pkw47[]{ Weapon::DualBerettas, Weapon::FAMAS, Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk47 = __pkk_empty; constexpr auto __pkg47 = __pkg_empty;
    constexpr Weapon __pkw48[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk48 = __pkk_empty; constexpr auto __pkg48 = __pkg_empty;
    constexpr Weapon __pkw51[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk51 = __pkk_empty; constexpr auto __pkg51 = __pkg_empty;
    constexpr auto __pkw59 = __pkw_empty; constexpr Knife __pkk59[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg59 = __pkg_empty;
    constexpr Weapon __pkw60[]{ Weapon::USPS, Weapon::FAMAS, Weapon::M4A1S, Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk60 = __pkk_empty; constexpr auto __pkg60 = __pkg_empty;
    constexpr Weapon __pkw61[]{ Weapon::DesertEagle, Weapon::MP9, Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk61 = __pkk_empty; constexpr auto __pkg61 = __pkg_empty;
    constexpr Weapon __pkw62[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk62 = __pkk_empty; constexpr auto __pkg62 = __pkg_empty;
    constexpr Weapon __pkw67[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk67 = __pkk_empty; constexpr auto __pkg67 = __pkg_empty;
    constexpr Weapon __pkw70[]{ Weapon::MAG7, Weapon::UMP45, Weapon::PPBizon, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk70 = __pkk_empty; constexpr auto __pkg70 = __pkg_empty;
    constexpr Weapon __pkw71[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk71 = __pkk_empty; constexpr auto __pkg71 = __pkg_empty;
    constexpr Weapon __pkw72[]{ Weapon::AK47, Weapon::AWP, Weapon::G3SG1, Weapon::INVALID }; constexpr Knife __pkk72[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg72 = __pkg_empty;
    constexpr Weapon __pkw73[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk73 = __pkk_empty; constexpr auto __pkg73 = __pkg_empty;
    constexpr Weapon __pkw74[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk74 = __pkk_empty; constexpr auto __pkg74 = __pkg_empty;
    constexpr Weapon __pkw75[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk75 = __pkk_empty; constexpr auto __pkg75 = __pkg_empty;
    constexpr Weapon __pkw76[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk76 = __pkk_empty; constexpr auto __pkg76 = __pkg_empty;
    constexpr Weapon __pkw77[]{ Weapon::P250, Weapon::M4A1S, Weapon::INVALID }; constexpr Knife __pkk77[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg77 = __pkg_empty;
    constexpr Weapon __pkw78[]{ Weapon::P250, Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk78 = __pkk_empty; constexpr auto __pkg78 = __pkg_empty;
    constexpr Weapon __pkw83[]{ Weapon::SawedOff, Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk83 = __pkk_empty; constexpr auto __pkg83 = __pkg_empty;
    constexpr Weapon __pkw84[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk84 = __pkk_empty; constexpr auto __pkg84 = __pkg_empty;
    constexpr Weapon __pkw90[]{ Weapon::DesertEagle, Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk90 = __pkk_empty; constexpr auto __pkg90 = __pkg_empty;
    constexpr Weapon __pkw92[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk92 = __pkk_empty; constexpr auto __pkg92 = __pkg_empty;
    constexpr Weapon __pkw93[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk93 = __pkk_empty; constexpr auto __pkg93 = __pkg_empty;
    constexpr Weapon __pkw95[]{ Weapon::P2000, Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk95 = __pkk_empty; constexpr auto __pkg95 = __pkg_empty;
    constexpr Weapon __pkw96[]{ Weapon::XM1014, Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk96 = __pkk_empty; constexpr auto __pkg96 = __pkg_empty;
    constexpr Weapon __pkw98[]{ Weapon::MAC10, Weapon::SG553, Weapon::INVALID }; constexpr Knife __pkk98[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg98 = __pkg_empty;
    constexpr Weapon __pkw99[]{ Weapon::P250, Weapon::Nova, Weapon::MAG7, Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk99 = __pkk_empty; constexpr auto __pkg99 = __pkg_empty;
    constexpr Weapon __pkw100[]{ Weapon::MAG7, Weapon::MP9, Weapon::P90, Weapon::AUG, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk100 = __pkk_empty; constexpr auto __pkg100 = __pkg_empty;
    constexpr Weapon __pkw101[]{ Weapon::MAC10, Weapon::Galil, Weapon::M4A4, Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk101 = __pkk_empty; constexpr auto __pkg101 = __pkg_empty;
    constexpr Weapon __pkw102[]{ Weapon::P250, Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk102 = __pkk_empty; constexpr auto __pkg102 = __pkg_empty;
    constexpr Weapon __pkw104[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk104 = __pkk_empty; constexpr auto __pkg104 = __pkg_empty;
    constexpr Weapon __pkw107[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk107 = __pkk_empty; constexpr auto __pkg107 = __pkg_empty;
    constexpr Weapon __pkw110[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk110 = __pkk_empty; constexpr auto __pkg110 = __pkg_empty;
    constexpr Weapon __pkw111[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk111 = __pkk_empty; constexpr auto __pkg111 = __pkg_empty;
    constexpr Weapon __pkw116[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk116 = __pkk_empty; constexpr auto __pkg116 = __pkg_empty;
    constexpr Weapon __pkw119[]{ Weapon::SawedOff, Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk119 = __pkk_empty; constexpr auto __pkg119 = __pkg_empty;
    constexpr Weapon __pkw122[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk122 = __pkk_empty; constexpr auto __pkg122 = __pkg_empty;
    constexpr Weapon __pkw124[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk124 = __pkk_empty; constexpr auto __pkg124 = __pkg_empty;
    constexpr Weapon __pkw125[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk125 = __pkk_empty; constexpr auto __pkg125 = __pkg_empty;
    constexpr Weapon __pkw135[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk135 = __pkk_empty; constexpr auto __pkg135 = __pkg_empty;
    constexpr Weapon __pkw136[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk136 = __pkk_empty; constexpr auto __pkg136 = __pkg_empty;
    constexpr Weapon __pkw141[]{ Weapon::FiveSeven, Weapon::MP9, Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk141 = __pkk_empty; constexpr auto __pkg141 = __pkg_empty;
    constexpr auto __pkw143 = __pkw_empty; constexpr Knife __pkk143[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg143 = __pkg_empty;
    constexpr Weapon __pkw147[]{ Weapon::CZ75, Weapon::SSG08, Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk147 = __pkk_empty; constexpr auto __pkg147 = __pkg_empty;
    constexpr Weapon __pkw148[]{ Weapon::MP9, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk148 = __pkk_empty; constexpr auto __pkg148 = __pkg_empty;
    constexpr Weapon __pkw149[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk149 = __pkk_empty; constexpr auto __pkg149 = __pkg_empty;
    constexpr Weapon __pkw151[]{ Weapon::FiveSeven, Weapon::M249, Weapon::INVALID }; constexpr auto __pkk151 = __pkk_empty; constexpr auto __pkg151 = __pkg_empty;
    constexpr Weapon __pkw153[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk153 = __pkk_empty; constexpr auto __pkg153 = __pkg_empty;
    constexpr Weapon __pkw154[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk154 = __pkk_empty; constexpr auto __pkg154 = __pkg_empty;
    constexpr Weapon __pkw155[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk155 = __pkk_empty; constexpr auto __pkg155 = __pkg_empty;
    constexpr Weapon __pkw156[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk156 = __pkk_empty; constexpr auto __pkg156 = __pkg_empty;
    constexpr Weapon __pkw157[]{ Weapon::MAC10, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk157 = __pkk_empty; constexpr auto __pkg157 = __pkg_empty;
    constexpr Weapon __pkw158[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk158 = __pkk_empty; constexpr auto __pkg158 = __pkg_empty;
    constexpr Weapon __pkw159[]{ Weapon::Glock18, Weapon::Tec9, Weapon::PPBizon, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk159 = __pkk_empty; constexpr auto __pkg159 = __pkg_empty;
    constexpr Weapon __pkw162[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk162 = __pkk_empty; constexpr auto __pkg162 = __pkg_empty;
    constexpr Weapon __pkw164[]{ Weapon::P250, Weapon::Nova, Weapon::PPBizon, Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk164 = __pkk_empty; constexpr auto __pkg164 = __pkg_empty;
    constexpr Weapon __pkw165[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk165 = __pkk_empty; constexpr auto __pkg165 = __pkg_empty;
    constexpr Weapon __pkw166[]{ Weapon::Nova, Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk166 = __pkk_empty; constexpr auto __pkg166 = __pkg_empty;
    constexpr Weapon __pkw167[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk167 = __pkk_empty; constexpr auto __pkg167 = __pkg_empty;
    constexpr Weapon __pkw168[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk168 = __pkk_empty; constexpr auto __pkg168 = __pkg_empty;
    constexpr Weapon __pkw169[]{ Weapon::XM1014, Weapon::UMP45, Weapon::P90, Weapon::INVALID }; constexpr auto __pkk169 = __pkk_empty; constexpr auto __pkg169 = __pkg_empty;
    constexpr Weapon __pkw170[]{ Weapon::Nova, Weapon::M249, Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk170 = __pkk_empty; constexpr auto __pkg170 = __pkg_empty;
    constexpr Weapon __pkw171[]{ Weapon::SawedOff, Weapon::MAG7, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk171 = __pkk_empty; constexpr auto __pkg171 = __pkg_empty;
    constexpr Weapon __pkw172[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk172 = __pkk_empty; constexpr auto __pkg172 = __pkg_empty;
    constexpr Weapon __pkw174[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk174 = __pkk_empty; constexpr auto __pkg174 = __pkg_empty;
    constexpr Weapon __pkw175[]{ Weapon::MP7, Weapon::UMP45, Weapon::P90, Weapon::INVALID }; constexpr Knife __pkk175[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg175 = __pkg_empty;
    constexpr Weapon __pkw176[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk176 = __pkk_empty; constexpr auto __pkg176 = __pkg_empty;
    constexpr Weapon __pkw177[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk177 = __pkk_empty; constexpr auto __pkg177 = __pkg_empty;
    constexpr Weapon __pkw178[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk178 = __pkk_empty; constexpr auto __pkg178 = __pkg_empty;
    constexpr Weapon __pkw179[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk179 = __pkk_empty; constexpr auto __pkg179 = __pkg_empty;
    constexpr Weapon __pkw180[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk180 = __pkk_empty; constexpr auto __pkg180 = __pkg_empty;
    constexpr Weapon __pkw181[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk181 = __pkk_empty; constexpr auto __pkg181 = __pkg_empty;
    constexpr Weapon __pkw182[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk182 = __pkk_empty; constexpr auto __pkg182 = __pkg_empty;
    constexpr Weapon __pkw183[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk183 = __pkk_empty; constexpr auto __pkg183 = __pkg_empty;
    constexpr Weapon __pkw184[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk184 = __pkk_empty; constexpr auto __pkg184 = __pkg_empty;
    constexpr Weapon __pkw185[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk185 = __pkk_empty; constexpr auto __pkg185 = __pkg_empty;
    constexpr Weapon __pkw186[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk186 = __pkk_empty; constexpr auto __pkg186 = __pkg_empty;
    constexpr Weapon __pkw187[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk187 = __pkk_empty; constexpr auto __pkg187 = __pkg_empty;
    constexpr Weapon __pkw188[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk188 = __pkk_empty; constexpr auto __pkg188 = __pkg_empty;
    constexpr Weapon __pkw189[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk189 = __pkk_empty; constexpr auto __pkg189 = __pkg_empty;
    constexpr Weapon __pkw190[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk190 = __pkk_empty; constexpr auto __pkg190 = __pkg_empty;
    constexpr Weapon __pkw191[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk191 = __pkk_empty; constexpr auto __pkg191 = __pkg_empty;
    constexpr Weapon __pkw192[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk192 = __pkk_empty; constexpr auto __pkg192 = __pkg_empty;
    constexpr Weapon __pkw193[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk193 = __pkk_empty; constexpr auto __pkg193 = __pkg_empty;
    constexpr Weapon __pkw194[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk194 = __pkk_empty; constexpr auto __pkg194 = __pkg_empty;
    constexpr Weapon __pkw195[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk195 = __pkk_empty; constexpr auto __pkg195 = __pkg_empty;
    constexpr Weapon __pkw196[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk196 = __pkk_empty; constexpr auto __pkg196 = __pkg_empty;
    constexpr Weapon __pkw197[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk197 = __pkk_empty; constexpr auto __pkg197 = __pkg_empty;
    constexpr Weapon __pkw198[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk198 = __pkk_empty; constexpr auto __pkg198 = __pkg_empty;
    constexpr Weapon __pkw199[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk199 = __pkk_empty; constexpr auto __pkg199 = __pkg_empty;
    constexpr Weapon __pkw200[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk200 = __pkk_empty; constexpr auto __pkg200 = __pkg_empty;
    constexpr Weapon __pkw201[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk201 = __pkk_empty; constexpr auto __pkg201 = __pkg_empty;
    constexpr Weapon __pkw202[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk202 = __pkk_empty; constexpr auto __pkg202 = __pkg_empty;
    constexpr Weapon __pkw203[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk203 = __pkk_empty; constexpr auto __pkg203 = __pkg_empty;
    constexpr Weapon __pkw204[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk204 = __pkk_empty; constexpr auto __pkg204 = __pkg_empty;
    constexpr Weapon __pkw205[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk205 = __pkk_empty; constexpr auto __pkg205 = __pkg_empty;
    constexpr Weapon __pkw206[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk206 = __pkk_empty; constexpr auto __pkg206 = __pkg_empty;
    constexpr Weapon __pkw207[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk207 = __pkk_empty; constexpr auto __pkg207 = __pkg_empty;
    constexpr Weapon __pkw208[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk208 = __pkk_empty; constexpr auto __pkg208 = __pkg_empty;
    constexpr Weapon __pkw209[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk209 = __pkk_empty; constexpr auto __pkg209 = __pkg_empty;
    constexpr Weapon __pkw210[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk210 = __pkk_empty; constexpr auto __pkg210 = __pkg_empty;
    constexpr Weapon __pkw211[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk211 = __pkk_empty; constexpr auto __pkg211 = __pkg_empty;
    constexpr Weapon __pkw212[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk212 = __pkk_empty; constexpr auto __pkg212 = __pkg_empty;
    constexpr Weapon __pkw213[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk213 = __pkk_empty; constexpr auto __pkg213 = __pkg_empty;
    constexpr Weapon __pkw214[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk214 = __pkk_empty; constexpr auto __pkg214 = __pkg_empty;
    constexpr Weapon __pkw215[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk215 = __pkk_empty; constexpr auto __pkg215 = __pkg_empty;
    constexpr Weapon __pkw216[]{ Weapon::Tec9, Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk216 = __pkk_empty; constexpr auto __pkg216 = __pkg_empty;
    constexpr Weapon __pkw217[]{ Weapon::USPS, Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk217 = __pkk_empty; constexpr auto __pkg217 = __pkg_empty;
    constexpr Weapon __pkw218[]{ Weapon::CZ75, Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk218 = __pkk_empty; constexpr auto __pkg218 = __pkg_empty;
    constexpr Weapon __pkw219[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk219 = __pkk_empty; constexpr auto __pkg219 = __pkg_empty;
    constexpr Weapon __pkw220[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk220 = __pkk_empty; constexpr auto __pkg220 = __pkg_empty;
    constexpr Weapon __pkw221[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk221 = __pkk_empty; constexpr auto __pkg221 = __pkg_empty;
    constexpr Weapon __pkw222[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk222 = __pkk_empty; constexpr auto __pkg222 = __pkg_empty;
    constexpr Weapon __pkw223[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk223 = __pkk_empty; constexpr auto __pkg223 = __pkg_empty;
    constexpr Weapon __pkw224[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk224 = __pkk_empty; constexpr auto __pkg224 = __pkg_empty;
    constexpr Weapon __pkw225[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk225 = __pkk_empty; constexpr auto __pkg225 = __pkg_empty;
    constexpr Weapon __pkw226[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk226 = __pkk_empty; constexpr auto __pkg226 = __pkg_empty;
    constexpr Weapon __pkw227[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk227 = __pkk_empty; constexpr auto __pkg227 = __pkg_empty;
    constexpr Weapon __pkw228[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk228 = __pkk_empty; constexpr auto __pkg228 = __pkg_empty;
    constexpr Weapon __pkw229[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk229 = __pkk_empty; constexpr auto __pkg229 = __pkg_empty;
    constexpr Weapon __pkw230[]{ Weapon::Glock18, Weapon::P250, Weapon::INVALID }; constexpr auto __pkk230 = __pkk_empty; constexpr auto __pkg230 = __pkg_empty;
    constexpr Weapon __pkw231[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk231 = __pkk_empty; constexpr auto __pkg231 = __pkg_empty;
    constexpr Weapon __pkw232[]{ Weapon::DesertEagle, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk232 = __pkk_empty; constexpr auto __pkg232 = __pkg_empty;
    constexpr Weapon __pkw233[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk233 = __pkk_empty; constexpr auto __pkg233 = __pkg_empty;
    constexpr Weapon __pkw234[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk234 = __pkk_empty; constexpr auto __pkg234 = __pkg_empty;
    constexpr Weapon __pkw235[]{ Weapon::Tec9, Weapon::Galil, Weapon::M4A1S, Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk235 = __pkk_empty; constexpr auto __pkg235 = __pkg_empty;
    constexpr Weapon __pkw236[]{ Weapon::USPS, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk236 = __pkk_empty; constexpr auto __pkg236 = __pkg_empty;
    constexpr Weapon __pkw237[]{ Weapon::DesertEagle, Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk237 = __pkk_empty; constexpr auto __pkg237 = __pkg_empty;
    constexpr Weapon __pkw238[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk238 = __pkk_empty; constexpr auto __pkg238 = __pkg_empty;
    constexpr Weapon __pkw240[]{ Weapon::XM1014, Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk240 = __pkk_empty; constexpr auto __pkg240 = __pkg_empty;
    constexpr Weapon __pkw241[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk241 = __pkk_empty; constexpr auto __pkg241 = __pkg_empty;
    constexpr Weapon __pkw242[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk242 = __pkk_empty; constexpr auto __pkg242 = __pkg_empty;
    constexpr Weapon __pkw243[]{ Weapon::M249, Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk243 = __pkk_empty; constexpr auto __pkg243 = __pkg_empty;
    constexpr Weapon __pkw244[]{ Weapon::P90, Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk244 = __pkk_empty; constexpr auto __pkg244 = __pkg_empty;
    constexpr Weapon __pkw245[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk245 = __pkk_empty; constexpr auto __pkg245 = __pkg_empty;
    constexpr Weapon __pkw246[]{ Weapon::P2000, Weapon::SawedOff, Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk246 = __pkk_empty; constexpr auto __pkg246 = __pkg_empty;
    constexpr Weapon __pkw247[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk247 = __pkk_empty; constexpr auto __pkg247 = __pkg_empty;
    constexpr Weapon __pkw248[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk248 = __pkk_empty; constexpr auto __pkg248 = __pkg_empty;
    constexpr Weapon __pkw249[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk249 = __pkk_empty; constexpr auto __pkg249 = __pkg_empty;
    constexpr Weapon __pkw250[]{ Weapon::SawedOff, Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk250 = __pkk_empty; constexpr auto __pkg250 = __pkg_empty;
    constexpr Weapon __pkw251[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk251 = __pkk_empty; constexpr auto __pkg251 = __pkg_empty;
    constexpr Weapon __pkw252[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk252 = __pkk_empty; constexpr auto __pkg252 = __pkg_empty;
    constexpr Weapon __pkw253[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk253 = __pkk_empty; constexpr auto __pkg253 = __pkg_empty;
    constexpr Weapon __pkw254[]{ Weapon::FiveSeven, Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk254 = __pkk_empty; constexpr auto __pkg254 = __pkg_empty;
    constexpr Weapon __pkw255[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk255 = __pkk_empty; constexpr auto __pkg255 = __pkg_empty;
    constexpr Weapon __pkw256[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk256 = __pkk_empty; constexpr auto __pkg256 = __pkg_empty;
    constexpr Weapon __pkw257[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk257 = __pkk_empty; constexpr auto __pkg257 = __pkg_empty;
    constexpr Weapon __pkw258[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk258 = __pkk_empty; constexpr auto __pkg258 = __pkg_empty;
    constexpr Weapon __pkw259[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk259 = __pkk_empty; constexpr auto __pkg259 = __pkg_empty;
    constexpr Weapon __pkw260[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk260 = __pkk_empty; constexpr auto __pkg260 = __pkg_empty;
    constexpr Weapon __pkw261[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk261 = __pkk_empty; constexpr auto __pkg261 = __pkg_empty;
    constexpr Weapon __pkw262[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk262 = __pkk_empty; constexpr auto __pkg262 = __pkg_empty;
    constexpr Weapon __pkw263[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk263 = __pkk_empty; constexpr auto __pkg263 = __pkg_empty;
    constexpr Weapon __pkw264[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk264 = __pkk_empty; constexpr auto __pkg264 = __pkg_empty;
    constexpr Weapon __pkw265[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk265 = __pkk_empty; constexpr auto __pkg265 = __pkg_empty;
    constexpr Weapon __pkw266[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk266 = __pkk_empty; constexpr auto __pkg266 = __pkg_empty;
    constexpr Weapon __pkw267[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk267 = __pkk_empty; constexpr auto __pkg267 = __pkg_empty;
    constexpr Weapon __pkw268[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk268 = __pkk_empty; constexpr auto __pkg268 = __pkg_empty;
    constexpr Weapon __pkw269[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk269 = __pkk_empty; constexpr auto __pkg269 = __pkg_empty;
    constexpr Weapon __pkw270[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk270 = __pkk_empty; constexpr auto __pkg270 = __pkg_empty;
    constexpr Weapon __pkw271[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk271 = __pkk_empty; constexpr auto __pkg271 = __pkg_empty;
    constexpr Weapon __pkw272[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk272 = __pkk_empty; constexpr auto __pkg272 = __pkg_empty;
    constexpr Weapon __pkw273[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk273 = __pkk_empty; constexpr auto __pkg273 = __pkg_empty;
    constexpr Weapon __pkw274[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk274 = __pkk_empty; constexpr auto __pkg274 = __pkg_empty;
    constexpr Weapon __pkw275[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk275 = __pkk_empty; constexpr auto __pkg275 = __pkg_empty;
    constexpr Weapon __pkw276[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk276 = __pkk_empty; constexpr auto __pkg276 = __pkg_empty;
    constexpr Weapon __pkw277[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk277 = __pkk_empty; constexpr auto __pkg277 = __pkg_empty;
    constexpr Weapon __pkw278[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk278 = __pkk_empty; constexpr auto __pkg278 = __pkg_empty;
    constexpr Weapon __pkw279[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk279 = __pkk_empty; constexpr auto __pkg279 = __pkg_empty;
    constexpr Weapon __pkw280[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk280 = __pkk_empty; constexpr auto __pkg280 = __pkg_empty;
    constexpr Weapon __pkw281[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk281 = __pkk_empty; constexpr auto __pkg281 = __pkg_empty;
    constexpr Weapon __pkw282[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk282 = __pkk_empty; constexpr auto __pkg282 = __pkg_empty;
    constexpr Weapon __pkw283[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk283 = __pkk_empty; constexpr auto __pkg283 = __pkg_empty;
    constexpr Weapon __pkw284[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk284 = __pkk_empty; constexpr auto __pkg284 = __pkg_empty;
    constexpr Weapon __pkw285[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk285 = __pkk_empty; constexpr auto __pkg285 = __pkg_empty;
    constexpr Weapon __pkw286[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk286 = __pkk_empty; constexpr auto __pkg286 = __pkg_empty;
    constexpr Weapon __pkw287[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk287 = __pkk_empty; constexpr auto __pkg287 = __pkg_empty;
    constexpr Weapon __pkw288[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk288 = __pkk_empty; constexpr auto __pkg288 = __pkg_empty;
    constexpr Weapon __pkw289[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk289 = __pkk_empty; constexpr auto __pkg289 = __pkg_empty;
    constexpr Weapon __pkw290[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk290 = __pkk_empty; constexpr auto __pkg290 = __pkg_empty;
    constexpr Weapon __pkw291[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk291 = __pkk_empty; constexpr auto __pkg291 = __pkg_empty;
    constexpr Weapon __pkw293[]{ Weapon::Glock18, Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk293 = __pkk_empty; constexpr auto __pkg293 = __pkg_empty;
    constexpr Weapon __pkw294[]{ Weapon::Nova, Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk294 = __pkk_empty; constexpr auto __pkg294 = __pkg_empty;
    constexpr Weapon __pkw295[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk295 = __pkk_empty; constexpr auto __pkg295 = __pkg_empty;
    constexpr Weapon __pkw296[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk296 = __pkk_empty; constexpr auto __pkg296 = __pkg_empty;
    constexpr Weapon __pkw297[]{ Weapon::CZ75, Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk297 = __pkk_empty; constexpr auto __pkg297 = __pkg_empty;
    constexpr Weapon __pkw298[]{ Weapon::CZ75, Weapon::Nova, Weapon::Negev, Weapon::MP9, Weapon::SG553, Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk298 = __pkk_empty; constexpr auto __pkg298 = __pkg_empty;
    constexpr Weapon __pkw299[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk299 = __pkk_empty; constexpr auto __pkg299 = __pkg_empty;
    constexpr Weapon __pkw300[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk300 = __pkk_empty; constexpr auto __pkg300 = __pkg_empty;
    constexpr Weapon __pkw301[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk301 = __pkk_empty; constexpr auto __pkg301 = __pkg_empty;
    constexpr Weapon __pkw302[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk302 = __pkk_empty; constexpr auto __pkg302 = __pkg_empty;
    constexpr Weapon __pkw303[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk303 = __pkk_empty; constexpr auto __pkg303 = __pkg_empty;
    constexpr Weapon __pkw304[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk304 = __pkk_empty; constexpr auto __pkg304 = __pkg_empty;
    constexpr Weapon __pkw305[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk305 = __pkk_empty; constexpr auto __pkg305 = __pkg_empty;
    constexpr Weapon __pkw306[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk306 = __pkk_empty; constexpr auto __pkg306 = __pkg_empty;
    constexpr Weapon __pkw307[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk307 = __pkk_empty; constexpr auto __pkg307 = __pkg_empty;
    constexpr Weapon __pkw308[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk308 = __pkk_empty; constexpr auto __pkg308 = __pkg_empty;
    constexpr Weapon __pkw309[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk309 = __pkk_empty; constexpr auto __pkg309 = __pkg_empty;
    constexpr Weapon __pkw310[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk310 = __pkk_empty; constexpr auto __pkg310 = __pkg_empty;
    constexpr Weapon __pkw311[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk311 = __pkk_empty; constexpr auto __pkg311 = __pkg_empty;
    constexpr Weapon __pkw312[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk312 = __pkk_empty; constexpr auto __pkg312 = __pkg_empty;
    constexpr Weapon __pkw313[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk313 = __pkk_empty; constexpr auto __pkg313 = __pkg_empty;
    constexpr Weapon __pkw314[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk314 = __pkk_empty; constexpr auto __pkg314 = __pkg_empty;
    constexpr Weapon __pkw315[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk315 = __pkk_empty; constexpr auto __pkg315 = __pkg_empty;
    constexpr Weapon __pkw316[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk316 = __pkk_empty; constexpr auto __pkg316 = __pkg_empty;
    constexpr Weapon __pkw317[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk317 = __pkk_empty; constexpr auto __pkg317 = __pkg_empty;
    constexpr Weapon __pkw318[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk318 = __pkk_empty; constexpr auto __pkg318 = __pkg_empty;
    constexpr Weapon __pkw319[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk319 = __pkk_empty; constexpr auto __pkg319 = __pkg_empty;
    constexpr Weapon __pkw320[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk320 = __pkk_empty; constexpr auto __pkg320 = __pkg_empty;
    constexpr Weapon __pkw321[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk321 = __pkk_empty; constexpr auto __pkg321 = __pkg_empty;
    constexpr Weapon __pkw322[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk322 = __pkk_empty; constexpr auto __pkg322 = __pkg_empty;
    constexpr Weapon __pkw323[]{ Weapon::Nova, Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk323 = __pkk_empty; constexpr auto __pkg323 = __pkg_empty;
    constexpr Weapon __pkw325[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk325 = __pkk_empty; constexpr auto __pkg325 = __pkg_empty;
    constexpr Weapon __pkw326[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk326 = __pkk_empty; constexpr auto __pkg326 = __pkg_empty;
    constexpr Weapon __pkw327[]{ Weapon::P2000, Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk327 = __pkk_empty; constexpr auto __pkg327 = __pkg_empty;
    constexpr Weapon __pkw328[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk328 = __pkk_empty; constexpr auto __pkg328 = __pkg_empty;
    constexpr Weapon __pkw329[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk329 = __pkk_empty; constexpr auto __pkg329 = __pkg_empty;
    constexpr Weapon __pkw330[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk330 = __pkk_empty; constexpr auto __pkg330 = __pkg_empty;
    constexpr Weapon __pkw332[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk332 = __pkk_empty; constexpr auto __pkg332 = __pkg_empty;
    constexpr Weapon __pkw333[]{ Weapon::CZ75, Weapon::MAC10, Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk333 = __pkk_empty; constexpr auto __pkg333 = __pkg_empty;
    constexpr Weapon __pkw334[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk334 = __pkk_empty; constexpr auto __pkg334 = __pkg_empty;
    constexpr Weapon __pkw335[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk335 = __pkk_empty; constexpr auto __pkg335 = __pkg_empty;
    constexpr Weapon __pkw336[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk336 = __pkk_empty; constexpr auto __pkg336 = __pkg_empty;
    constexpr Weapon __pkw337[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk337 = __pkk_empty; constexpr auto __pkg337 = __pkg_empty;
    constexpr Weapon __pkw338[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk338 = __pkk_empty; constexpr auto __pkg338 = __pkg_empty;
    constexpr Weapon __pkw339[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk339 = __pkk_empty; constexpr auto __pkg339 = __pkg_empty;
    constexpr Weapon __pkw340[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk340 = __pkk_empty; constexpr auto __pkg340 = __pkg_empty;
    constexpr Weapon __pkw341[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk341 = __pkk_empty; constexpr auto __pkg341 = __pkg_empty;
    constexpr Weapon __pkw342[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk342 = __pkk_empty; constexpr auto __pkg342 = __pkg_empty;
    constexpr Weapon __pkw343[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk343 = __pkk_empty; constexpr auto __pkg343 = __pkg_empty;
    constexpr Weapon __pkw344[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk344 = __pkk_empty; constexpr auto __pkg344 = __pkg_empty;
    constexpr Weapon __pkw345[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk345 = __pkk_empty; constexpr auto __pkg345 = __pkg_empty;
    constexpr Weapon __pkw346[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk346 = __pkk_empty; constexpr auto __pkg346 = __pkg_empty;
    constexpr Weapon __pkw347[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk347 = __pkk_empty; constexpr auto __pkg347 = __pkg_empty;
    constexpr Weapon __pkw348[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk348 = __pkk_empty; constexpr auto __pkg348 = __pkg_empty;
    constexpr Weapon __pkw349[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk349 = __pkk_empty; constexpr auto __pkg349 = __pkg_empty;
    constexpr Weapon __pkw350[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk350 = __pkk_empty; constexpr auto __pkg350 = __pkg_empty;
    constexpr Weapon __pkw351[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk351 = __pkk_empty; constexpr auto __pkg351 = __pkg_empty;
    constexpr Weapon __pkw352[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk352 = __pkk_empty; constexpr auto __pkg352 = __pkg_empty;
    constexpr Weapon __pkw353[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk353 = __pkk_empty; constexpr auto __pkg353 = __pkg_empty;
    constexpr Weapon __pkw354[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk354 = __pkk_empty; constexpr auto __pkg354 = __pkg_empty;
    constexpr Weapon __pkw355[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk355 = __pkk_empty; constexpr auto __pkg355 = __pkg_empty;
    constexpr Weapon __pkw356[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk356 = __pkk_empty; constexpr auto __pkg356 = __pkg_empty;
    constexpr Weapon __pkw357[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk357 = __pkk_empty; constexpr auto __pkg357 = __pkg_empty;
    constexpr Weapon __pkw358[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk358 = __pkk_empty; constexpr auto __pkg358 = __pkg_empty;
    constexpr Weapon __pkw359[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk359 = __pkk_empty; constexpr auto __pkg359 = __pkg_empty;
    constexpr Weapon __pkw360[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk360 = __pkk_empty; constexpr auto __pkg360 = __pkg_empty;
    constexpr Weapon __pkw361[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk361 = __pkk_empty; constexpr auto __pkg361 = __pkg_empty;
    constexpr Weapon __pkw362[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk362 = __pkk_empty; constexpr auto __pkg362 = __pkg_empty;
    constexpr Weapon __pkw363[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk363 = __pkk_empty; constexpr auto __pkg363 = __pkg_empty;
    constexpr Weapon __pkw364[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk364 = __pkk_empty; constexpr auto __pkg364 = __pkg_empty;
    constexpr Weapon __pkw365[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk365 = __pkk_empty; constexpr auto __pkg365 = __pkg_empty;
    constexpr Weapon __pkw366[]{ Weapon::CZ75, Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk366 = __pkk_empty; constexpr auto __pkg366 = __pkg_empty;
    constexpr Weapon __pkw367[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk367 = __pkk_empty; constexpr auto __pkg367 = __pkg_empty;
    constexpr Weapon __pkw368[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk368 = __pkk_empty; constexpr auto __pkg368 = __pkg_empty;
    constexpr Weapon __pkw369[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk369 = __pkk_empty; constexpr auto __pkg369 = __pkg_empty;
    constexpr Weapon __pkw370[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk370 = __pkk_empty; constexpr auto __pkg370 = __pkg_empty;
    constexpr Weapon __pkw371[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk371 = __pkk_empty; constexpr auto __pkg371 = __pkg_empty;
    constexpr Weapon __pkw372[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk372 = __pkk_empty; constexpr auto __pkg372 = __pkg_empty;
    constexpr Weapon __pkw373[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk373 = __pkk_empty; constexpr auto __pkg373 = __pkg_empty;
    constexpr Weapon __pkw374[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk374 = __pkk_empty; constexpr auto __pkg374 = __pkg_empty;
    constexpr Weapon __pkw375[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk375 = __pkk_empty; constexpr auto __pkg375 = __pkg_empty;
    constexpr Weapon __pkw376[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk376 = __pkk_empty; constexpr auto __pkg376 = __pkg_empty;
    constexpr Weapon __pkw377[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk377 = __pkk_empty; constexpr auto __pkg377 = __pkg_empty;
    constexpr Weapon __pkw378[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk378 = __pkk_empty; constexpr auto __pkg378 = __pkg_empty;
    constexpr Weapon __pkw379[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk379 = __pkk_empty; constexpr auto __pkg379 = __pkg_empty;
    constexpr Weapon __pkw380[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk380 = __pkk_empty; constexpr auto __pkg380 = __pkg_empty;
    constexpr Weapon __pkw381[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk381 = __pkk_empty; constexpr auto __pkg381 = __pkg_empty;
    constexpr Weapon __pkw382[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk382 = __pkk_empty; constexpr auto __pkg382 = __pkg_empty;
    constexpr Weapon __pkw383[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk383 = __pkk_empty; constexpr auto __pkg383 = __pkg_empty;
    constexpr Weapon __pkw384[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk384 = __pkk_empty; constexpr auto __pkg384 = __pkg_empty;
    constexpr Weapon __pkw385[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk385 = __pkk_empty; constexpr auto __pkg385 = __pkg_empty;
    constexpr Weapon __pkw386[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk386 = __pkk_empty; constexpr auto __pkg386 = __pkg_empty;
    constexpr Weapon __pkw387[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk387 = __pkk_empty; constexpr auto __pkg387 = __pkg_empty;
    constexpr Weapon __pkw388[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk388 = __pkk_empty; constexpr auto __pkg388 = __pkg_empty;
    constexpr Weapon __pkw389[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk389 = __pkk_empty; constexpr auto __pkg389 = __pkg_empty;
    constexpr Weapon __pkw390[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk390 = __pkk_empty; constexpr auto __pkg390 = __pkg_empty;
    constexpr Weapon __pkw391[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk391 = __pkk_empty; constexpr auto __pkg391 = __pkg_empty;
    constexpr Weapon __pkw392[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk392 = __pkk_empty; constexpr auto __pkg392 = __pkg_empty;
    constexpr Weapon __pkw393[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk393 = __pkk_empty; constexpr auto __pkg393 = __pkg_empty;
    constexpr Weapon __pkw394[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk394 = __pkk_empty; constexpr auto __pkg394 = __pkg_empty;
    constexpr Weapon __pkw395[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk395 = __pkk_empty; constexpr auto __pkg395 = __pkg_empty;
    constexpr Weapon __pkw396[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk396 = __pkk_empty; constexpr auto __pkg396 = __pkg_empty;
    constexpr Weapon __pkw397[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk397 = __pkk_empty; constexpr auto __pkg397 = __pkg_empty;
    constexpr Weapon __pkw398[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk398 = __pkk_empty; constexpr auto __pkg398 = __pkg_empty;
    constexpr Weapon __pkw399[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk399 = __pkk_empty; constexpr auto __pkg399 = __pkg_empty;
    constexpr Weapon __pkw400[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk400 = __pkk_empty; constexpr auto __pkg400 = __pkg_empty;
    constexpr Weapon __pkw401[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk401 = __pkk_empty; constexpr auto __pkg401 = __pkg_empty;
    constexpr Weapon __pkw402[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk402 = __pkk_empty; constexpr auto __pkg402 = __pkg_empty;
    constexpr Weapon __pkw403[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk403 = __pkk_empty; constexpr auto __pkg403 = __pkg_empty;
    constexpr Weapon __pkw404[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk404 = __pkk_empty; constexpr auto __pkg404 = __pkg_empty;
    constexpr Weapon __pkw405[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk405 = __pkk_empty; constexpr auto __pkg405 = __pkg_empty;
    constexpr Weapon __pkw406[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk406 = __pkk_empty; constexpr auto __pkg406 = __pkg_empty;
    constexpr Weapon __pkw407[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk407 = __pkk_empty; constexpr auto __pkg407 = __pkg_empty;
    constexpr auto __pkw409 = __pkw_empty; constexpr Knife __pkk409[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg409 = __pkg_empty;
    constexpr auto __pkw410 = __pkw_empty; constexpr Knife __pkk410[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::INVALID }; constexpr auto __pkg410 = __pkg_empty;
    constexpr auto __pkw411 = __pkw_empty; constexpr Knife __pkk411[]{ Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Huntsman, Knife::M9Bayonet, Knife::ShadowDaggers, Knife::INVALID }; constexpr auto __pkg411 = __pkg_empty;
    constexpr auto __pkw413 = __pkw_empty; constexpr Knife __pkk413[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg413 = __pkg_empty;
    constexpr auto __pkw414 = __pkw_empty; constexpr Knife __pkk414[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg414 = __pkg_empty;
    constexpr auto __pkw415 = __pkw_empty; constexpr Knife __pkk415[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg415 = __pkg_empty;
    constexpr auto __pkw416 = __pkw_empty; constexpr Knife __pkk416[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg416 = __pkg_empty;
    constexpr auto __pkw417 = __pkw_empty; constexpr Knife __pkk417[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg417 = __pkg_empty;
    constexpr Weapon __pkw422[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk422 = __pkk_empty; constexpr auto __pkg422 = __pkg_empty;
    constexpr Weapon __pkw423[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk423 = __pkk_empty; constexpr auto __pkg423 = __pkg_empty;
    constexpr Weapon __pkw424[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk424 = __pkk_empty; constexpr auto __pkg424 = __pkg_empty;
    constexpr Weapon __pkw425[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk425 = __pkk_empty; constexpr auto __pkg425 = __pkg_empty;
    constexpr Weapon __pkw426[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk426 = __pkk_empty; constexpr auto __pkg426 = __pkg_empty;
    constexpr Weapon __pkw427[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk427 = __pkk_empty; constexpr auto __pkg427 = __pkg_empty;
    constexpr Weapon __pkw428[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk428 = __pkk_empty; constexpr auto __pkg428 = __pkg_empty;
    constexpr Weapon __pkw429[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk429 = __pkk_empty; constexpr auto __pkg429 = __pkg_empty;
    constexpr Weapon __pkw430[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk430 = __pkk_empty; constexpr auto __pkg430 = __pkg_empty;
    constexpr Weapon __pkw431[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk431 = __pkk_empty; constexpr auto __pkg431 = __pkg_empty;
    constexpr Weapon __pkw432[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk432 = __pkk_empty; constexpr auto __pkg432 = __pkg_empty;
    constexpr Weapon __pkw433[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk433 = __pkk_empty; constexpr auto __pkg433 = __pkg_empty;
    constexpr Weapon __pkw434[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk434 = __pkk_empty; constexpr auto __pkg434 = __pkg_empty;
    constexpr Weapon __pkw435[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk435 = __pkk_empty; constexpr auto __pkg435 = __pkg_empty;
    constexpr Weapon __pkw436[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk436 = __pkk_empty; constexpr auto __pkg436 = __pkg_empty;
    constexpr Weapon __pkw437[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk437 = __pkk_empty; constexpr auto __pkg437 = __pkg_empty;
    constexpr Weapon __pkw438[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk438 = __pkk_empty; constexpr auto __pkg438 = __pkg_empty;
    constexpr Weapon __pkw439[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk439 = __pkk_empty; constexpr auto __pkg439 = __pkg_empty;
    constexpr Weapon __pkw440[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk440 = __pkk_empty; constexpr auto __pkg440 = __pkg_empty;
    constexpr Weapon __pkw441[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk441 = __pkk_empty; constexpr auto __pkg441 = __pkg_empty;
    constexpr Weapon __pkw442[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk442 = __pkk_empty; constexpr auto __pkg442 = __pkg_empty;
    constexpr Weapon __pkw443[]{ Weapon::P2000, Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk443 = __pkk_empty; constexpr auto __pkg443 = __pkg_empty;
    constexpr Weapon __pkw444[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk444 = __pkk_empty; constexpr auto __pkg444 = __pkg_empty;
    constexpr Weapon __pkw445[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk445 = __pkk_empty; constexpr auto __pkg445 = __pkg_empty;
    constexpr Weapon __pkw446[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk446 = __pkk_empty; constexpr auto __pkg446 = __pkg_empty;
    constexpr Weapon __pkw447[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk447 = __pkk_empty; constexpr auto __pkg447 = __pkg_empty;
    constexpr Weapon __pkw448[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk448 = __pkk_empty; constexpr auto __pkg448 = __pkg_empty;
    constexpr Weapon __pkw449[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk449 = __pkk_empty; constexpr auto __pkg449 = __pkg_empty;
    constexpr Weapon __pkw450[]{ Weapon::DualBerettas, Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk450 = __pkk_empty; constexpr auto __pkg450 = __pkg_empty;
    constexpr Weapon __pkw451[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk451 = __pkk_empty; constexpr auto __pkg451 = __pkg_empty;
    constexpr Weapon __pkw452[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk452 = __pkk_empty; constexpr auto __pkg452 = __pkg_empty;
    constexpr Weapon __pkw453[]{ Weapon::DualBerettas, Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk453 = __pkk_empty; constexpr auto __pkg453 = __pkg_empty;
    constexpr Weapon __pkw454[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk454 = __pkk_empty; constexpr auto __pkg454 = __pkg_empty;
    constexpr Weapon __pkw455[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk455 = __pkk_empty; constexpr auto __pkg455 = __pkg_empty;
    constexpr Weapon __pkw456[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk456 = __pkk_empty; constexpr auto __pkg456 = __pkg_empty;
    constexpr Weapon __pkw457[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk457 = __pkk_empty; constexpr auto __pkg457 = __pkg_empty;
    constexpr Weapon __pkw458[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk458 = __pkk_empty; constexpr auto __pkg458 = __pkg_empty;
    constexpr Weapon __pkw459[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk459 = __pkk_empty; constexpr auto __pkg459 = __pkg_empty;
    constexpr Weapon __pkw460[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk460 = __pkk_empty; constexpr auto __pkg460 = __pkg_empty;
    constexpr Weapon __pkw462[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk462 = __pkk_empty; constexpr auto __pkg462 = __pkg_empty;
    constexpr Weapon __pkw463[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk463 = __pkk_empty; constexpr auto __pkg463 = __pkg_empty;
    constexpr Weapon __pkw464[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk464 = __pkk_empty; constexpr auto __pkg464 = __pkg_empty;
    constexpr Weapon __pkw465[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk465 = __pkk_empty; constexpr auto __pkg465 = __pkg_empty;
    constexpr Weapon __pkw466[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk466 = __pkk_empty; constexpr auto __pkg466 = __pkg_empty;
    constexpr Weapon __pkw467[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk467 = __pkk_empty; constexpr auto __pkg467 = __pkg_empty;
    constexpr Weapon __pkw468[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk468 = __pkk_empty; constexpr auto __pkg468 = __pkg_empty;
    constexpr Weapon __pkw469[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk469 = __pkk_empty; constexpr auto __pkg469 = __pkg_empty;
    constexpr Weapon __pkw470[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk470 = __pkk_empty; constexpr auto __pkg470 = __pkg_empty;
    constexpr Weapon __pkw471[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk471 = __pkk_empty; constexpr auto __pkg471 = __pkg_empty;
    constexpr Weapon __pkw472[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk472 = __pkk_empty; constexpr auto __pkg472 = __pkg_empty;
    constexpr Weapon __pkw473[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk473 = __pkk_empty; constexpr auto __pkg473 = __pkg_empty;
    constexpr Weapon __pkw474[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk474 = __pkk_empty; constexpr auto __pkg474 = __pkg_empty;
    constexpr Weapon __pkw475[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk475 = __pkk_empty; constexpr auto __pkg475 = __pkg_empty;
    constexpr Weapon __pkw476[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk476 = __pkk_empty; constexpr auto __pkg476 = __pkg_empty;
    constexpr Weapon __pkw477[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk477 = __pkk_empty; constexpr auto __pkg477 = __pkg_empty;
    constexpr Weapon __pkw478[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk478 = __pkk_empty; constexpr auto __pkg478 = __pkg_empty;
    constexpr Weapon __pkw479[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk479 = __pkk_empty; constexpr auto __pkg479 = __pkg_empty;
    constexpr Weapon __pkw480[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk480 = __pkk_empty; constexpr auto __pkg480 = __pkg_empty;
    constexpr Weapon __pkw481[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk481 = __pkk_empty; constexpr auto __pkg481 = __pkg_empty;
    constexpr Weapon __pkw482[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk482 = __pkk_empty; constexpr auto __pkg482 = __pkg_empty;
    constexpr Weapon __pkw483[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk483 = __pkk_empty; constexpr auto __pkg483 = __pkg_empty;
    constexpr Weapon __pkw484[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk484 = __pkk_empty; constexpr auto __pkg484 = __pkg_empty;
    constexpr Weapon __pkw485[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk485 = __pkk_empty; constexpr auto __pkg485 = __pkg_empty;
    constexpr Weapon __pkw486[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk486 = __pkk_empty; constexpr auto __pkg486 = __pkg_empty;
    constexpr Weapon __pkw487[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk487 = __pkk_empty; constexpr auto __pkg487 = __pkg_empty;
    constexpr Weapon __pkw488[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk488 = __pkk_empty; constexpr auto __pkg488 = __pkg_empty;
    constexpr Weapon __pkw489[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk489 = __pkk_empty; constexpr auto __pkg489 = __pkg_empty;
    constexpr Weapon __pkw490[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk490 = __pkk_empty; constexpr auto __pkg490 = __pkg_empty;
    constexpr Weapon __pkw491[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk491 = __pkk_empty; constexpr auto __pkg491 = __pkg_empty;
    constexpr Weapon __pkw492[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk492 = __pkk_empty; constexpr auto __pkg492 = __pkg_empty;
    constexpr Weapon __pkw493[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk493 = __pkk_empty; constexpr auto __pkg493 = __pkg_empty;
    constexpr Weapon __pkw494[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk494 = __pkk_empty; constexpr auto __pkg494 = __pkg_empty;
    constexpr Weapon __pkw495[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk495 = __pkk_empty; constexpr auto __pkg495 = __pkg_empty;
    constexpr Weapon __pkw496[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk496 = __pkk_empty; constexpr auto __pkg496 = __pkg_empty;
    constexpr Weapon __pkw497[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk497 = __pkk_empty; constexpr auto __pkg497 = __pkg_empty;
    constexpr Weapon __pkw498[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk498 = __pkk_empty; constexpr auto __pkg498 = __pkg_empty;
    constexpr Weapon __pkw499[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk499 = __pkk_empty; constexpr auto __pkg499 = __pkg_empty;
    constexpr Weapon __pkw500[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk500 = __pkk_empty; constexpr auto __pkg500 = __pkg_empty;
    constexpr Weapon __pkw501[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk501 = __pkk_empty; constexpr auto __pkg501 = __pkg_empty;
    constexpr Weapon __pkw502[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk502 = __pkk_empty; constexpr auto __pkg502 = __pkg_empty;
    constexpr Weapon __pkw503[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk503 = __pkk_empty; constexpr auto __pkg503 = __pkg_empty;
    constexpr Weapon __pkw504[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk504 = __pkk_empty; constexpr auto __pkg504 = __pkg_empty;
    constexpr Weapon __pkw505[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk505 = __pkk_empty; constexpr auto __pkg505 = __pkg_empty;
    constexpr Weapon __pkw506[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk506 = __pkk_empty; constexpr auto __pkg506 = __pkg_empty;
    constexpr Weapon __pkw507[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk507 = __pkk_empty; constexpr auto __pkg507 = __pkg_empty;
    constexpr Weapon __pkw508[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk508 = __pkk_empty; constexpr auto __pkg508 = __pkg_empty;
    constexpr Weapon __pkw509[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk509 = __pkk_empty; constexpr auto __pkg509 = __pkg_empty;
    constexpr Weapon __pkw510[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk510 = __pkk_empty; constexpr auto __pkg510 = __pkg_empty;
    constexpr Weapon __pkw511[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk511 = __pkk_empty; constexpr auto __pkg511 = __pkg_empty;
    constexpr Weapon __pkw512[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk512 = __pkk_empty; constexpr auto __pkg512 = __pkg_empty;
    constexpr Weapon __pkw514[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk514 = __pkk_empty; constexpr auto __pkg514 = __pkg_empty;
    constexpr Weapon __pkw515[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk515 = __pkk_empty; constexpr auto __pkg515 = __pkg_empty;
    constexpr Weapon __pkw516[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk516 = __pkk_empty; constexpr auto __pkg516 = __pkg_empty;
    constexpr Weapon __pkw517[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk517 = __pkk_empty; constexpr auto __pkg517 = __pkg_empty;
    constexpr Weapon __pkw518[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk518 = __pkk_empty; constexpr auto __pkg518 = __pkg_empty;
    constexpr Weapon __pkw519[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk519 = __pkk_empty; constexpr auto __pkg519 = __pkg_empty;
    constexpr Weapon __pkw520[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk520 = __pkk_empty; constexpr auto __pkg520 = __pkg_empty;
    constexpr Weapon __pkw521[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk521 = __pkk_empty; constexpr auto __pkg521 = __pkg_empty;
    constexpr Weapon __pkw522[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk522 = __pkk_empty; constexpr auto __pkg522 = __pkg_empty;
    constexpr Weapon __pkw523[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk523 = __pkk_empty; constexpr auto __pkg523 = __pkg_empty;
    constexpr Weapon __pkw524[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk524 = __pkk_empty; constexpr auto __pkg524 = __pkg_empty;
    constexpr Weapon __pkw525[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk525 = __pkk_empty; constexpr auto __pkg525 = __pkg_empty;
    constexpr Weapon __pkw526[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk526 = __pkk_empty; constexpr auto __pkg526 = __pkg_empty;
    constexpr Weapon __pkw527[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk527 = __pkk_empty; constexpr auto __pkg527 = __pkg_empty;
    constexpr Weapon __pkw528[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk528 = __pkk_empty; constexpr auto __pkg528 = __pkg_empty;
    constexpr Weapon __pkw529[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk529 = __pkk_empty; constexpr auto __pkg529 = __pkg_empty;
    constexpr Weapon __pkw530[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk530 = __pkk_empty; constexpr auto __pkg530 = __pkg_empty;
    constexpr Weapon __pkw532[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk532 = __pkk_empty; constexpr auto __pkg532 = __pkg_empty;
    constexpr Weapon __pkw533[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk533 = __pkk_empty; constexpr auto __pkg533 = __pkg_empty;
    constexpr Weapon __pkw534[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk534 = __pkk_empty; constexpr auto __pkg534 = __pkg_empty;
    constexpr Weapon __pkw535[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk535 = __pkk_empty; constexpr auto __pkg535 = __pkg_empty;
    constexpr Weapon __pkw536[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk536 = __pkk_empty; constexpr auto __pkg536 = __pkg_empty;
    constexpr Weapon __pkw537[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk537 = __pkk_empty; constexpr auto __pkg537 = __pkg_empty;
    constexpr Weapon __pkw538[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk538 = __pkk_empty; constexpr auto __pkg538 = __pkg_empty;
    constexpr Weapon __pkw539[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk539 = __pkk_empty; constexpr auto __pkg539 = __pkg_empty;
    constexpr Weapon __pkw540[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk540 = __pkk_empty; constexpr auto __pkg540 = __pkg_empty;
    constexpr Weapon __pkw541[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk541 = __pkk_empty; constexpr auto __pkg541 = __pkg_empty;
    constexpr Weapon __pkw542[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk542 = __pkk_empty; constexpr auto __pkg542 = __pkg_empty;
    constexpr Weapon __pkw543[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk543 = __pkk_empty; constexpr auto __pkg543 = __pkg_empty;
    constexpr Weapon __pkw544[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk544 = __pkk_empty; constexpr auto __pkg544 = __pkg_empty;
    constexpr Weapon __pkw545[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk545 = __pkk_empty; constexpr auto __pkg545 = __pkg_empty;
    constexpr Weapon __pkw546[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk546 = __pkk_empty; constexpr auto __pkg546 = __pkg_empty;
    constexpr Weapon __pkw547[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk547 = __pkk_empty; constexpr auto __pkg547 = __pkg_empty;
    constexpr Weapon __pkw548[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk548 = __pkk_empty; constexpr auto __pkg548 = __pkg_empty;
    constexpr Weapon __pkw549[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk549 = __pkk_empty; constexpr auto __pkg549 = __pkg_empty;
    constexpr Weapon __pkw550[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk550 = __pkk_empty; constexpr auto __pkg550 = __pkg_empty;
    constexpr Weapon __pkw551[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk551 = __pkk_empty; constexpr auto __pkg551 = __pkg_empty;
    constexpr Weapon __pkw552[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk552 = __pkk_empty; constexpr auto __pkg552 = __pkg_empty;
    constexpr Weapon __pkw553[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk553 = __pkk_empty; constexpr auto __pkg553 = __pkg_empty;
    constexpr Weapon __pkw554[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk554 = __pkk_empty; constexpr auto __pkg554 = __pkg_empty;
    constexpr Weapon __pkw555[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk555 = __pkk_empty; constexpr auto __pkg555 = __pkg_empty;
    constexpr Weapon __pkw556[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk556 = __pkk_empty; constexpr auto __pkg556 = __pkg_empty;
    constexpr Weapon __pkw557[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk557 = __pkk_empty; constexpr auto __pkg557 = __pkg_empty;
    constexpr auto __pkw558 = __pkw_empty; constexpr Knife __pkk558[]{ Knife::Bayonet, Knife::INVALID }; constexpr auto __pkg558 = __pkg_empty;
    constexpr auto __pkw559 = __pkw_empty; constexpr Knife __pkk559[]{ Knife::Flip, Knife::INVALID }; constexpr auto __pkg559 = __pkg_empty;
    constexpr auto __pkw560 = __pkw_empty; constexpr Knife __pkk560[]{ Knife::Gut, Knife::INVALID }; constexpr auto __pkg560 = __pkg_empty;
    constexpr auto __pkw561 = __pkw_empty; constexpr Knife __pkk561[]{ Knife::Karambit, Knife::INVALID }; constexpr auto __pkg561 = __pkg_empty;
    constexpr auto __pkw562 = __pkw_empty; constexpr Knife __pkk562[]{ Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg562 = __pkg_empty;
    constexpr auto __pkw563 = __pkw_empty; constexpr Knife __pkk563[]{ Knife::Bayonet, Knife::INVALID }; constexpr auto __pkg563 = __pkg_empty;
    constexpr auto __pkw564 = __pkw_empty; constexpr Knife __pkk564[]{ Knife::Flip, Knife::INVALID }; constexpr auto __pkg564 = __pkg_empty;
    constexpr auto __pkw565 = __pkw_empty; constexpr Knife __pkk565[]{ Knife::Gut, Knife::INVALID }; constexpr auto __pkg565 = __pkg_empty;
    constexpr auto __pkw566 = __pkw_empty; constexpr Knife __pkk566[]{ Knife::Karambit, Knife::INVALID }; constexpr auto __pkg566 = __pkg_empty;
    constexpr auto __pkw567 = __pkw_empty; constexpr Knife __pkk567[]{ Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg567 = __pkg_empty;
    constexpr auto __pkw568 = __pkw_empty; constexpr Knife __pkk568[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg568 = __pkg_empty;
    constexpr auto __pkw569 = __pkw_empty; constexpr Knife __pkk569[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg569 = __pkg_empty;
    constexpr auto __pkw570 = __pkw_empty; constexpr Knife __pkk570[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg570 = __pkg_empty;
    constexpr auto __pkw571 = __pkw_empty; constexpr Knife __pkk571[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg571 = __pkg_empty;
    constexpr auto __pkw572 = __pkw_empty; constexpr Knife __pkk572[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg572 = __pkg_empty;
    constexpr auto __pkw573 = __pkw_empty; constexpr Knife __pkk573[]{ Knife::Bayonet, Knife::INVALID }; constexpr auto __pkg573 = __pkg_empty;
    constexpr auto __pkw574 = __pkw_empty; constexpr Knife __pkk574[]{ Knife::Flip, Knife::INVALID }; constexpr auto __pkg574 = __pkg_empty;
    constexpr auto __pkw575 = __pkw_empty; constexpr Knife __pkk575[]{ Knife::Gut, Knife::INVALID }; constexpr auto __pkg575 = __pkg_empty;
    constexpr auto __pkw576 = __pkw_empty; constexpr Knife __pkk576[]{ Knife::Karambit, Knife::INVALID }; constexpr auto __pkg576 = __pkg_empty;
    constexpr auto __pkw577 = __pkw_empty; constexpr Knife __pkk577[]{ Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg577 = __pkg_empty;
    constexpr auto __pkw578 = __pkw_empty; constexpr Knife __pkk578[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::Karambit, Knife::INVALID }; constexpr auto __pkg578 = __pkg_empty;
    constexpr auto __pkw579 = __pkw_empty; constexpr Knife __pkk579[]{ Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg579 = __pkg_empty;
    constexpr auto __pkw580 = __pkw_empty; constexpr Knife __pkk580[]{ Knife::Bayonet, Knife::Flip, Knife::Gut, Knife::INVALID }; constexpr auto __pkg580 = __pkg_empty;
    constexpr auto __pkw581 = __pkw_empty; constexpr Knife __pkk581[]{ Knife::M9Bayonet, Knife::INVALID }; constexpr auto __pkg581 = __pkg_empty;
    constexpr auto __pkw582 = __pkw_empty; constexpr Knife __pkk582[]{ Knife::Karambit, Knife::INVALID }; constexpr auto __pkg582 = __pkg_empty;
    constexpr Weapon __pkw583[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk583 = __pkk_empty; constexpr auto __pkg583 = __pkg_empty;
    constexpr Weapon __pkw584[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk584 = __pkk_empty; constexpr auto __pkg584 = __pkg_empty;
    constexpr Weapon __pkw585[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk585 = __pkk_empty; constexpr auto __pkg585 = __pkg_empty;
    constexpr Weapon __pkw586[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk586 = __pkk_empty; constexpr auto __pkg586 = __pkg_empty;
    constexpr Weapon __pkw587[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk587 = __pkk_empty; constexpr auto __pkg587 = __pkg_empty;
    constexpr Weapon __pkw588[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk588 = __pkk_empty; constexpr auto __pkg588 = __pkg_empty;
    constexpr Weapon __pkw589[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk589 = __pkk_empty; constexpr auto __pkg589 = __pkg_empty;
    constexpr Weapon __pkw590[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk590 = __pkk_empty; constexpr auto __pkg590 = __pkg_empty;
    constexpr Weapon __pkw591[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk591 = __pkk_empty; constexpr auto __pkg591 = __pkg_empty;
    constexpr Weapon __pkw592[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk592 = __pkk_empty; constexpr auto __pkg592 = __pkg_empty;
    constexpr Weapon __pkw593[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk593 = __pkk_empty; constexpr auto __pkg593 = __pkg_empty;
    constexpr Weapon __pkw594[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk594 = __pkk_empty; constexpr auto __pkg594 = __pkg_empty;
    constexpr Weapon __pkw595[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk595 = __pkk_empty; constexpr auto __pkg595 = __pkg_empty;
    constexpr Weapon __pkw596[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk596 = __pkk_empty; constexpr auto __pkg596 = __pkg_empty;
    constexpr Weapon __pkw597[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk597 = __pkk_empty; constexpr auto __pkg597 = __pkg_empty;
    constexpr Weapon __pkw598[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk598 = __pkk_empty; constexpr auto __pkg598 = __pkg_empty;
    constexpr Weapon __pkw599[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk599 = __pkk_empty; constexpr auto __pkg599 = __pkg_empty;
    constexpr Weapon __pkw600[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk600 = __pkk_empty; constexpr auto __pkg600 = __pkg_empty;
    constexpr Weapon __pkw601[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk601 = __pkk_empty; constexpr auto __pkg601 = __pkg_empty;
    constexpr Weapon __pkw602[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk602 = __pkk_empty; constexpr auto __pkg602 = __pkg_empty;
    constexpr Weapon __pkw603[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk603 = __pkk_empty; constexpr auto __pkg603 = __pkg_empty;
    constexpr Weapon __pkw604[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk604 = __pkk_empty; constexpr auto __pkg604 = __pkg_empty;
    constexpr Weapon __pkw605[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk605 = __pkk_empty; constexpr auto __pkg605 = __pkg_empty;
    constexpr Weapon __pkw606[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk606 = __pkk_empty; constexpr auto __pkg606 = __pkg_empty;
    constexpr Weapon __pkw607[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk607 = __pkk_empty; constexpr auto __pkg607 = __pkg_empty;
    constexpr Weapon __pkw608[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk608 = __pkk_empty; constexpr auto __pkg608 = __pkg_empty;
    constexpr Weapon __pkw609[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk609 = __pkk_empty; constexpr auto __pkg609 = __pkg_empty;
    constexpr Weapon __pkw610[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk610 = __pkk_empty; constexpr auto __pkg610 = __pkg_empty;
    constexpr Weapon __pkw611[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk611 = __pkk_empty; constexpr auto __pkg611 = __pkg_empty;
    constexpr Weapon __pkw612[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk612 = __pkk_empty; constexpr auto __pkg612 = __pkg_empty;
    constexpr Weapon __pkw613[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk613 = __pkk_empty; constexpr auto __pkg613 = __pkg_empty;
    constexpr Weapon __pkw614[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk614 = __pkk_empty; constexpr auto __pkg614 = __pkg_empty;
    constexpr Weapon __pkw615[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk615 = __pkk_empty; constexpr auto __pkg615 = __pkg_empty;
    constexpr Weapon __pkw616[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk616 = __pkk_empty; constexpr auto __pkg616 = __pkg_empty;
    constexpr auto __pkw620 = __pkw_empty; constexpr Knife __pkk620[]{ Knife::Huntsman, Knife::INVALID }; constexpr auto __pkg620 = __pkg_empty;
    constexpr auto __pkw621 = __pkw_empty; constexpr Knife __pkk621[]{ Knife::Falchion, Knife::INVALID }; constexpr auto __pkg621 = __pkg_empty;
    constexpr Weapon __pkw622[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk622 = __pkk_empty; constexpr auto __pkg622 = __pkg_empty;
    constexpr Weapon __pkw623[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk623 = __pkk_empty; constexpr auto __pkg623 = __pkg_empty;
    constexpr Weapon __pkw624[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk624 = __pkk_empty; constexpr auto __pkg624 = __pkg_empty;
    constexpr Weapon __pkw625[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk625 = __pkk_empty; constexpr auto __pkg625 = __pkg_empty;
    constexpr Weapon __pkw626[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk626 = __pkk_empty; constexpr auto __pkg626 = __pkg_empty;
    constexpr Weapon __pkw627[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk627 = __pkk_empty; constexpr auto __pkg627 = __pkg_empty;
    constexpr Weapon __pkw628[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk628 = __pkk_empty; constexpr auto __pkg628 = __pkg_empty;
    constexpr Weapon __pkw629[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk629 = __pkk_empty; constexpr auto __pkg629 = __pkg_empty;
    constexpr Weapon __pkw630[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk630 = __pkk_empty; constexpr auto __pkg630 = __pkg_empty;
    constexpr Weapon __pkw631[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk631 = __pkk_empty; constexpr auto __pkg631 = __pkg_empty;
    constexpr Weapon __pkw632[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk632 = __pkk_empty; constexpr auto __pkg632 = __pkg_empty;
    constexpr Weapon __pkw633[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk633 = __pkk_empty; constexpr auto __pkg633 = __pkg_empty;
    constexpr Weapon __pkw634[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk634 = __pkk_empty; constexpr auto __pkg634 = __pkg_empty;
    constexpr Weapon __pkw635[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk635 = __pkk_empty; constexpr auto __pkg635 = __pkg_empty;
    constexpr Weapon __pkw636[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk636 = __pkk_empty; constexpr auto __pkg636 = __pkg_empty;
    constexpr Weapon __pkw637[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk637 = __pkk_empty; constexpr auto __pkg637 = __pkg_empty;
    constexpr Weapon __pkw638[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk638 = __pkk_empty; constexpr auto __pkg638 = __pkg_empty;
    constexpr Weapon __pkw639[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk639 = __pkk_empty; constexpr auto __pkg639 = __pkg_empty;
    constexpr Weapon __pkw640[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk640 = __pkk_empty; constexpr auto __pkg640 = __pkg_empty;
    constexpr Weapon __pkw641[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk641 = __pkk_empty; constexpr auto __pkg641 = __pkg_empty;
    constexpr Weapon __pkw642[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk642 = __pkk_empty; constexpr auto __pkg642 = __pkg_empty;
    constexpr Weapon __pkw643[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk643 = __pkk_empty; constexpr auto __pkg643 = __pkg_empty;
    constexpr Weapon __pkw644[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk644 = __pkk_empty; constexpr auto __pkg644 = __pkg_empty;
    constexpr Weapon __pkw645[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk645 = __pkk_empty; constexpr auto __pkg645 = __pkg_empty;
    constexpr Weapon __pkw646[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk646 = __pkk_empty; constexpr auto __pkg646 = __pkg_empty;
    constexpr Weapon __pkw647[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk647 = __pkk_empty; constexpr auto __pkg647 = __pkg_empty;
    constexpr Weapon __pkw648[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk648 = __pkk_empty; constexpr auto __pkg648 = __pkg_empty;
    constexpr Weapon __pkw649[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk649 = __pkk_empty; constexpr auto __pkg649 = __pkg_empty;
    constexpr Weapon __pkw650[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk650 = __pkk_empty; constexpr auto __pkg650 = __pkg_empty;
    constexpr Weapon __pkw651[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk651 = __pkk_empty; constexpr auto __pkg651 = __pkg_empty;
    constexpr Weapon __pkw652[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk652 = __pkk_empty; constexpr auto __pkg652 = __pkg_empty;
    constexpr Weapon __pkw653[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk653 = __pkk_empty; constexpr auto __pkg653 = __pkg_empty;
    constexpr Weapon __pkw654[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk654 = __pkk_empty; constexpr auto __pkg654 = __pkg_empty;
    constexpr Weapon __pkw655[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk655 = __pkk_empty; constexpr auto __pkg655 = __pkg_empty;
    constexpr Weapon __pkw656[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk656 = __pkk_empty; constexpr auto __pkg656 = __pkg_empty;
    constexpr Weapon __pkw657[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk657 = __pkk_empty; constexpr auto __pkg657 = __pkg_empty;
    constexpr Weapon __pkw658[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk658 = __pkk_empty; constexpr auto __pkg658 = __pkg_empty;
    constexpr Weapon __pkw659[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk659 = __pkk_empty; constexpr auto __pkg659 = __pkg_empty;
    constexpr Weapon __pkw660[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk660 = __pkk_empty; constexpr auto __pkg660 = __pkg_empty;
    constexpr Weapon __pkw661[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk661 = __pkk_empty; constexpr auto __pkg661 = __pkg_empty;
    constexpr Weapon __pkw662[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk662 = __pkk_empty; constexpr auto __pkg662 = __pkg_empty;
    constexpr Weapon __pkw663[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk663 = __pkk_empty; constexpr auto __pkg663 = __pkg_empty;
    constexpr Weapon __pkw664[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk664 = __pkk_empty; constexpr auto __pkg664 = __pkg_empty;
    constexpr Weapon __pkw665[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk665 = __pkk_empty; constexpr auto __pkg665 = __pkg_empty;
    constexpr Weapon __pkw666[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk666 = __pkk_empty; constexpr auto __pkg666 = __pkg_empty;
    constexpr Weapon __pkw667[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk667 = __pkk_empty; constexpr auto __pkg667 = __pkg_empty;
    constexpr Weapon __pkw668[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk668 = __pkk_empty; constexpr auto __pkg668 = __pkg_empty;
    constexpr Weapon __pkw669[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk669 = __pkk_empty; constexpr auto __pkg669 = __pkg_empty;
    constexpr Weapon __pkw670[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk670 = __pkk_empty; constexpr auto __pkg670 = __pkg_empty;
    constexpr Weapon __pkw671[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk671 = __pkk_empty; constexpr auto __pkg671 = __pkg_empty;
    constexpr Weapon __pkw672[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk672 = __pkk_empty; constexpr auto __pkg672 = __pkg_empty;
    constexpr Weapon __pkw673[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk673 = __pkk_empty; constexpr auto __pkg673 = __pkg_empty;
    constexpr Weapon __pkw674[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk674 = __pkk_empty; constexpr auto __pkg674 = __pkg_empty;
    constexpr Weapon __pkw675[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk675 = __pkk_empty; constexpr auto __pkg675 = __pkg_empty;
    constexpr Weapon __pkw676[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk676 = __pkk_empty; constexpr auto __pkg676 = __pkg_empty;
    constexpr Weapon __pkw677[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk677 = __pkk_empty; constexpr auto __pkg677 = __pkg_empty;
    constexpr Weapon __pkw678[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk678 = __pkk_empty; constexpr auto __pkg678 = __pkg_empty;
    constexpr Weapon __pkw679[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk679 = __pkk_empty; constexpr auto __pkg679 = __pkg_empty;
    constexpr Weapon __pkw680[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk680 = __pkk_empty; constexpr auto __pkg680 = __pkg_empty;
    constexpr Weapon __pkw681[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk681 = __pkk_empty; constexpr auto __pkg681 = __pkg_empty;
    constexpr Weapon __pkw682[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk682 = __pkk_empty; constexpr auto __pkg682 = __pkg_empty;
    constexpr Weapon __pkw683[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk683 = __pkk_empty; constexpr auto __pkg683 = __pkg_empty;
    constexpr Weapon __pkw684[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk684 = __pkk_empty; constexpr auto __pkg684 = __pkg_empty;
    constexpr Weapon __pkw685[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk685 = __pkk_empty; constexpr auto __pkg685 = __pkg_empty;
    constexpr Weapon __pkw686[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk686 = __pkk_empty; constexpr auto __pkg686 = __pkg_empty;
    constexpr Weapon __pkw687[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk687 = __pkk_empty; constexpr auto __pkg687 = __pkg_empty;
    constexpr Weapon __pkw688[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk688 = __pkk_empty; constexpr auto __pkg688 = __pkg_empty;
    constexpr Weapon __pkw689[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk689 = __pkk_empty; constexpr auto __pkg689 = __pkg_empty;
    constexpr Weapon __pkw690[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk690 = __pkk_empty; constexpr auto __pkg690 = __pkg_empty;
    constexpr Weapon __pkw691[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk691 = __pkk_empty; constexpr auto __pkg691 = __pkg_empty;
    constexpr Weapon __pkw692[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk692 = __pkk_empty; constexpr auto __pkg692 = __pkg_empty;
    constexpr Weapon __pkw693[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk693 = __pkk_empty; constexpr auto __pkg693 = __pkg_empty;
    constexpr Weapon __pkw694[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk694 = __pkk_empty; constexpr auto __pkg694 = __pkg_empty;
    constexpr Weapon __pkw695[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk695 = __pkk_empty; constexpr auto __pkg695 = __pkg_empty;
    constexpr Weapon __pkw696[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk696 = __pkk_empty; constexpr auto __pkg696 = __pkg_empty;
    constexpr Weapon __pkw697[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk697 = __pkk_empty; constexpr auto __pkg697 = __pkg_empty;
    constexpr Weapon __pkw698[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk698 = __pkk_empty; constexpr auto __pkg698 = __pkg_empty;
    constexpr Weapon __pkw699[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk699 = __pkk_empty; constexpr auto __pkg699 = __pkg_empty;
    constexpr Weapon __pkw700[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk700 = __pkk_empty; constexpr auto __pkg700 = __pkg_empty;
    constexpr Weapon __pkw701[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk701 = __pkk_empty; constexpr auto __pkg701 = __pkg_empty;
    constexpr Weapon __pkw702[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk702 = __pkk_empty; constexpr auto __pkg702 = __pkg_empty;
    constexpr Weapon __pkw703[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk703 = __pkk_empty; constexpr auto __pkg703 = __pkg_empty;
    constexpr Weapon __pkw704[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk704 = __pkk_empty; constexpr auto __pkg704 = __pkg_empty;
    constexpr Weapon __pkw705[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk705 = __pkk_empty; constexpr auto __pkg705 = __pkg_empty;
    constexpr Weapon __pkw706[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk706 = __pkk_empty; constexpr auto __pkg706 = __pkg_empty;
    constexpr Weapon __pkw707[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk707 = __pkk_empty; constexpr auto __pkg707 = __pkg_empty;
    constexpr Weapon __pkw708[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk708 = __pkk_empty; constexpr auto __pkg708 = __pkg_empty;
    constexpr Weapon __pkw709[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk709 = __pkk_empty; constexpr auto __pkg709 = __pkg_empty;
    constexpr Weapon __pkw710[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk710 = __pkk_empty; constexpr auto __pkg710 = __pkg_empty;
    constexpr Weapon __pkw711[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk711 = __pkk_empty; constexpr auto __pkg711 = __pkg_empty;
    constexpr Weapon __pkw712[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk712 = __pkk_empty; constexpr auto __pkg712 = __pkg_empty;
    constexpr Weapon __pkw713[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk713 = __pkk_empty; constexpr auto __pkg713 = __pkg_empty;
    constexpr Weapon __pkw714[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk714 = __pkk_empty; constexpr auto __pkg714 = __pkg_empty;
    constexpr Weapon __pkw715[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk715 = __pkk_empty; constexpr auto __pkg715 = __pkg_empty;
    constexpr Weapon __pkw716[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk716 = __pkk_empty; constexpr auto __pkg716 = __pkg_empty;
    constexpr Weapon __pkw717[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk717 = __pkk_empty; constexpr auto __pkg717 = __pkg_empty;
    constexpr Weapon __pkw718[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk718 = __pkk_empty; constexpr auto __pkg718 = __pkg_empty;
    constexpr Weapon __pkw719[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk719 = __pkk_empty; constexpr auto __pkg719 = __pkg_empty;
    constexpr Weapon __pkw720[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk720 = __pkk_empty; constexpr auto __pkg720 = __pkg_empty;
    constexpr Weapon __pkw721[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk721 = __pkk_empty; constexpr auto __pkg721 = __pkg_empty;
    constexpr Weapon __pkw722[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk722 = __pkk_empty; constexpr auto __pkg722 = __pkg_empty;
    constexpr Weapon __pkw723[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk723 = __pkk_empty; constexpr auto __pkg723 = __pkg_empty;
    constexpr Weapon __pkw724[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk724 = __pkk_empty; constexpr auto __pkg724 = __pkg_empty;
    constexpr Weapon __pkw725[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk725 = __pkk_empty; constexpr auto __pkg725 = __pkg_empty;
    constexpr Weapon __pkw726[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk726 = __pkk_empty; constexpr auto __pkg726 = __pkg_empty;
    constexpr Weapon __pkw727[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk727 = __pkk_empty; constexpr auto __pkg727 = __pkg_empty;
    constexpr Weapon __pkw728[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk728 = __pkk_empty; constexpr auto __pkg728 = __pkg_empty;
    constexpr Weapon __pkw729[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk729 = __pkk_empty; constexpr auto __pkg729 = __pkg_empty;
    constexpr Weapon __pkw730[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk730 = __pkk_empty; constexpr auto __pkg730 = __pkg_empty;
    constexpr Weapon __pkw731[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk731 = __pkk_empty; constexpr auto __pkg731 = __pkg_empty;
    constexpr Weapon __pkw732[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk732 = __pkk_empty; constexpr auto __pkg732 = __pkg_empty;
    constexpr Weapon __pkw733[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk733 = __pkk_empty; constexpr auto __pkg733 = __pkg_empty;
    constexpr Weapon __pkw734[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk734 = __pkk_empty; constexpr auto __pkg734 = __pkg_empty;
    constexpr auto __pkw735 = __pkw_empty; constexpr Knife __pkk735[]{ Knife::Nomad, Knife::Skeleton, Knife::Survival, Knife::Paracord, Knife::Classic, Knife::Navaja, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg735 = __pkg_empty;
    constexpr Weapon __pkw736[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk736 = __pkk_empty; constexpr auto __pkg736 = __pkg_empty;
    constexpr Weapon __pkw737[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk737 = __pkk_empty; constexpr auto __pkg737 = __pkg_empty;
    constexpr Weapon __pkw738[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk738 = __pkk_empty; constexpr auto __pkg738 = __pkg_empty;
    constexpr Weapon __pkw739[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk739 = __pkk_empty; constexpr auto __pkg739 = __pkg_empty;
    constexpr Weapon __pkw740[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk740 = __pkk_empty; constexpr auto __pkg740 = __pkg_empty;
    constexpr Weapon __pkw741[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk741 = __pkk_empty; constexpr auto __pkg741 = __pkg_empty;
    constexpr Weapon __pkw742[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk742 = __pkk_empty; constexpr auto __pkg742 = __pkg_empty;
    constexpr Weapon __pkw743[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk743 = __pkk_empty; constexpr auto __pkg743 = __pkg_empty;
    constexpr Weapon __pkw744[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk744 = __pkk_empty; constexpr auto __pkg744 = __pkg_empty;
    constexpr Weapon __pkw745[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk745 = __pkk_empty; constexpr auto __pkg745 = __pkg_empty;
    constexpr Weapon __pkw746[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk746 = __pkk_empty; constexpr auto __pkg746 = __pkg_empty;
    constexpr Weapon __pkw747[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk747 = __pkk_empty; constexpr auto __pkg747 = __pkg_empty;
    constexpr Weapon __pkw748[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk748 = __pkk_empty; constexpr auto __pkg748 = __pkg_empty;
    constexpr Weapon __pkw749[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk749 = __pkk_empty; constexpr auto __pkg749 = __pkg_empty;
    constexpr Weapon __pkw750[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk750 = __pkk_empty; constexpr auto __pkg750 = __pkg_empty;
    constexpr Weapon __pkw751[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk751 = __pkk_empty; constexpr auto __pkg751 = __pkg_empty;
    constexpr Weapon __pkw752[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk752 = __pkk_empty; constexpr auto __pkg752 = __pkg_empty;
    constexpr Weapon __pkw753[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk753 = __pkk_empty; constexpr auto __pkg753 = __pkg_empty;
    constexpr Weapon __pkw754[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk754 = __pkk_empty; constexpr auto __pkg754 = __pkg_empty;
    constexpr Weapon __pkw755[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk755 = __pkk_empty; constexpr auto __pkg755 = __pkg_empty;
    constexpr Weapon __pkw756[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk756 = __pkk_empty; constexpr auto __pkg756 = __pkg_empty;
    constexpr Weapon __pkw757[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk757 = __pkk_empty; constexpr auto __pkg757 = __pkg_empty;
    constexpr Weapon __pkw758[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk758 = __pkk_empty; constexpr auto __pkg758 = __pkg_empty;
    constexpr Weapon __pkw759[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk759 = __pkk_empty; constexpr auto __pkg759 = __pkg_empty;
    constexpr Weapon __pkw760[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk760 = __pkk_empty; constexpr auto __pkg760 = __pkg_empty;
    constexpr Weapon __pkw761[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk761 = __pkk_empty; constexpr auto __pkg761 = __pkg_empty;
    constexpr Weapon __pkw762[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk762 = __pkk_empty; constexpr auto __pkg762 = __pkg_empty;
    constexpr Weapon __pkw763[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk763 = __pkk_empty; constexpr auto __pkg763 = __pkg_empty;
    constexpr Weapon __pkw775[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk775 = __pkk_empty; constexpr auto __pkg775 = __pkg_empty;
    constexpr Weapon __pkw776[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk776 = __pkk_empty; constexpr auto __pkg776 = __pkg_empty;
    constexpr Weapon __pkw777[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk777 = __pkk_empty; constexpr auto __pkg777 = __pkg_empty;
    constexpr Weapon __pkw778[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk778 = __pkk_empty; constexpr auto __pkg778 = __pkg_empty;
    constexpr Weapon __pkw779[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk779 = __pkk_empty; constexpr auto __pkg779 = __pkg_empty;
    constexpr Weapon __pkw780[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk780 = __pkk_empty; constexpr auto __pkg780 = __pkg_empty;
    constexpr Weapon __pkw781[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk781 = __pkk_empty; constexpr auto __pkg781 = __pkg_empty;
    constexpr Weapon __pkw782[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk782 = __pkk_empty; constexpr auto __pkg782 = __pkg_empty;
    constexpr Weapon __pkw783[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk783 = __pkk_empty; constexpr auto __pkg783 = __pkg_empty;
    constexpr Weapon __pkw784[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk784 = __pkk_empty; constexpr auto __pkg784 = __pkg_empty;
    constexpr Weapon __pkw785[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk785 = __pkk_empty; constexpr auto __pkg785 = __pkg_empty;
    constexpr Weapon __pkw786[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk786 = __pkk_empty; constexpr auto __pkg786 = __pkg_empty;
    constexpr Weapon __pkw787[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk787 = __pkk_empty; constexpr auto __pkg787 = __pkg_empty;
    constexpr Weapon __pkw788[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk788 = __pkk_empty; constexpr auto __pkg788 = __pkg_empty;
    constexpr Weapon __pkw789[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk789 = __pkk_empty; constexpr auto __pkg789 = __pkg_empty;
    constexpr Weapon __pkw790[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk790 = __pkk_empty; constexpr auto __pkg790 = __pkg_empty;
    constexpr Weapon __pkw791[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk791 = __pkk_empty; constexpr auto __pkg791 = __pkg_empty;
    constexpr Weapon __pkw792[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk792 = __pkk_empty; constexpr auto __pkg792 = __pkg_empty;
    constexpr Weapon __pkw793[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk793 = __pkk_empty; constexpr auto __pkg793 = __pkg_empty;
    constexpr Weapon __pkw794[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk794 = __pkk_empty; constexpr auto __pkg794 = __pkg_empty;
    constexpr Weapon __pkw795[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk795 = __pkk_empty; constexpr auto __pkg795 = __pkg_empty;
    constexpr Weapon __pkw796[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk796 = __pkk_empty; constexpr auto __pkg796 = __pkg_empty;
    constexpr Weapon __pkw797[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk797 = __pkk_empty; constexpr auto __pkg797 = __pkg_empty;
    constexpr Weapon __pkw798[]{ Weapon::R8Revolver, Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk798 = __pkk_empty; constexpr auto __pkg798 = __pkg_empty;
    constexpr Weapon __pkw799[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk799 = __pkk_empty; constexpr auto __pkg799 = __pkg_empty;
    constexpr Weapon __pkw800[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk800 = __pkk_empty; constexpr auto __pkg800 = __pkg_empty;
    constexpr Weapon __pkw801[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk801 = __pkk_empty; constexpr auto __pkg801 = __pkg_empty;
    constexpr Weapon __pkw802[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk802 = __pkk_empty; constexpr auto __pkg802 = __pkg_empty;
    constexpr Weapon __pkw803[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk803 = __pkk_empty; constexpr auto __pkg803 = __pkg_empty;
    constexpr Weapon __pkw804[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk804 = __pkk_empty; constexpr auto __pkg804 = __pkg_empty;
    constexpr Weapon __pkw805[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk805 = __pkk_empty; constexpr auto __pkg805 = __pkg_empty;
    constexpr Weapon __pkw806[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk806 = __pkk_empty; constexpr auto __pkg806 = __pkg_empty;
    constexpr Weapon __pkw807[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk807 = __pkk_empty; constexpr auto __pkg807 = __pkg_empty;
    constexpr Weapon __pkw808[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk808 = __pkk_empty; constexpr auto __pkg808 = __pkg_empty;
    constexpr Weapon __pkw809[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk809 = __pkk_empty; constexpr auto __pkg809 = __pkg_empty;
    constexpr Weapon __pkw810[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk810 = __pkk_empty; constexpr auto __pkg810 = __pkg_empty;
    constexpr Weapon __pkw811[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk811 = __pkk_empty; constexpr auto __pkg811 = __pkg_empty;
    constexpr Weapon __pkw812[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk812 = __pkk_empty; constexpr auto __pkg812 = __pkg_empty;
    constexpr Weapon __pkw813[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk813 = __pkk_empty; constexpr auto __pkg813 = __pkg_empty;
    constexpr Weapon __pkw814[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk814 = __pkk_empty; constexpr auto __pkg814 = __pkg_empty;
    constexpr Weapon __pkw815[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk815 = __pkk_empty; constexpr auto __pkg815 = __pkg_empty;
    constexpr Weapon __pkw816[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk816 = __pkk_empty; constexpr auto __pkg816 = __pkg_empty;
    constexpr Weapon __pkw817[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk817 = __pkk_empty; constexpr auto __pkg817 = __pkg_empty;
    constexpr Weapon __pkw835[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk835 = __pkk_empty; constexpr auto __pkg835 = __pkg_empty;
    constexpr Weapon __pkw836[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk836 = __pkk_empty; constexpr auto __pkg836 = __pkg_empty;
    constexpr Weapon __pkw837[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk837 = __pkk_empty; constexpr auto __pkg837 = __pkg_empty;
    constexpr Weapon __pkw838[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk838 = __pkk_empty; constexpr auto __pkg838 = __pkg_empty;
    constexpr Weapon __pkw839[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk839 = __pkk_empty; constexpr auto __pkg839 = __pkg_empty;
    constexpr Weapon __pkw840[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk840 = __pkk_empty; constexpr auto __pkg840 = __pkg_empty;
    constexpr Weapon __pkw841[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk841 = __pkk_empty; constexpr auto __pkg841 = __pkg_empty;
    constexpr Weapon __pkw842[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk842 = __pkk_empty; constexpr auto __pkg842 = __pkg_empty;
    constexpr Weapon __pkw843[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk843 = __pkk_empty; constexpr auto __pkg843 = __pkg_empty;
    constexpr Weapon __pkw844[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk844 = __pkk_empty; constexpr auto __pkg844 = __pkg_empty;
    constexpr Weapon __pkw845[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk845 = __pkk_empty; constexpr auto __pkg845 = __pkg_empty;
    constexpr Weapon __pkw846[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk846 = __pkk_empty; constexpr auto __pkg846 = __pkg_empty;
    constexpr Weapon __pkw847[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk847 = __pkk_empty; constexpr auto __pkg847 = __pkg_empty;
    constexpr Weapon __pkw848[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk848 = __pkk_empty; constexpr auto __pkg848 = __pkg_empty;
    constexpr Weapon __pkw849[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk849 = __pkk_empty; constexpr auto __pkg849 = __pkg_empty;
    constexpr Weapon __pkw850[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk850 = __pkk_empty; constexpr auto __pkg850 = __pkg_empty;
    constexpr Weapon __pkw851[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk851 = __pkk_empty; constexpr auto __pkg851 = __pkg_empty;
    constexpr auto __pkw852 = __pkw_empty; constexpr Knife __pkk852[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg852 = __pkg_empty;
    constexpr auto __pkw853 = __pkw_empty; constexpr Knife __pkk853[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg853 = __pkg_empty;
    constexpr auto __pkw854 = __pkw_empty; constexpr Knife __pkk854[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg854 = __pkg_empty;
    constexpr auto __pkw855 = __pkw_empty; constexpr Knife __pkk855[]{ Knife::Bayonet, Knife::Bowie, Knife::Butterfly, Knife::Falchion, Knife::Flip, Knife::Gut, Knife::Huntsman, Knife::Karambit, Knife::M9Bayonet, Knife::Navaja, Knife::ShadowDaggers, Knife::Stiletto, Knife::Talon, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg855 = __pkg_empty;
    constexpr auto __pkw856 = __pkw_empty; constexpr Knife __pkk856[]{ Knife::Talon, Knife::INVALID }; constexpr auto __pkg856 = __pkg_empty;
    constexpr auto __pkw857 = __pkw_empty; constexpr Knife __pkk857[]{ Knife::Navaja, Knife::Stiletto, Knife::Ursus, Knife::INVALID }; constexpr auto __pkg857 = __pkg_empty;
    constexpr auto __pkw858 = __pkw_empty; constexpr Knife __pkk858[]{ Knife::Talon, Knife::INVALID }; constexpr auto __pkg858 = __pkg_empty;
    constexpr Weapon __pkw859[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk859 = __pkk_empty; constexpr auto __pkg859 = __pkg_empty;
    constexpr Weapon __pkw860[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk860 = __pkk_empty; constexpr auto __pkg860 = __pkg_empty;
    constexpr Weapon __pkw861[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk861 = __pkk_empty; constexpr auto __pkg861 = __pkg_empty;
    constexpr Weapon __pkw862[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk862 = __pkk_empty; constexpr auto __pkg862 = __pkg_empty;
    constexpr Weapon __pkw863[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk863 = __pkk_empty; constexpr auto __pkg863 = __pkg_empty;
    constexpr Weapon __pkw864[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk864 = __pkk_empty; constexpr auto __pkg864 = __pkg_empty;
    constexpr Weapon __pkw865[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk865 = __pkk_empty; constexpr auto __pkg865 = __pkg_empty;
    constexpr Weapon __pkw866[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk866 = __pkk_empty; constexpr auto __pkg866 = __pkg_empty;
    constexpr Weapon __pkw867[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk867 = __pkk_empty; constexpr auto __pkg867 = __pkg_empty;
    constexpr Weapon __pkw868[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk868 = __pkk_empty; constexpr auto __pkg868 = __pkg_empty;
    constexpr Weapon __pkw869[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk869 = __pkk_empty; constexpr auto __pkg869 = __pkg_empty;
    constexpr Weapon __pkw870[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk870 = __pkk_empty; constexpr auto __pkg870 = __pkg_empty;
    constexpr Weapon __pkw871[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk871 = __pkk_empty; constexpr auto __pkg871 = __pkg_empty;
    constexpr Weapon __pkw872[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk872 = __pkk_empty; constexpr auto __pkg872 = __pkg_empty;
    constexpr Weapon __pkw873[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk873 = __pkk_empty; constexpr auto __pkg873 = __pkg_empty;
    constexpr Weapon __pkw884[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk884 = __pkk_empty; constexpr auto __pkg884 = __pkg_empty;
    constexpr Weapon __pkw885[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk885 = __pkk_empty; constexpr auto __pkg885 = __pkg_empty;
    constexpr Weapon __pkw886[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk886 = __pkk_empty; constexpr auto __pkg886 = __pkg_empty;
    constexpr Weapon __pkw887[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk887 = __pkk_empty; constexpr auto __pkg887 = __pkg_empty;
    constexpr Weapon __pkw888[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk888 = __pkk_empty; constexpr auto __pkg888 = __pkg_empty;
    constexpr Weapon __pkw889[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk889 = __pkk_empty; constexpr auto __pkg889 = __pkg_empty;
    constexpr Weapon __pkw890[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk890 = __pkk_empty; constexpr auto __pkg890 = __pkg_empty;
    constexpr Weapon __pkw891[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk891 = __pkk_empty; constexpr auto __pkg891 = __pkg_empty;
    constexpr Weapon __pkw892[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk892 = __pkk_empty; constexpr auto __pkg892 = __pkg_empty;
    constexpr Weapon __pkw893[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk893 = __pkk_empty; constexpr auto __pkg893 = __pkg_empty;
    constexpr Weapon __pkw894[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk894 = __pkk_empty; constexpr auto __pkg894 = __pkg_empty;
    constexpr Weapon __pkw895[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk895 = __pkk_empty; constexpr auto __pkg895 = __pkg_empty;
    constexpr Weapon __pkw896[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk896 = __pkk_empty; constexpr auto __pkg896 = __pkg_empty;
    constexpr Weapon __pkw897[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk897 = __pkk_empty; constexpr auto __pkg897 = __pkg_empty;
    constexpr Weapon __pkw898[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk898 = __pkk_empty; constexpr auto __pkg898 = __pkg_empty;
    constexpr Weapon __pkw899[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk899 = __pkk_empty; constexpr auto __pkg899 = __pkg_empty;
    constexpr Weapon __pkw900[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk900 = __pkk_empty; constexpr auto __pkg900 = __pkg_empty;
    constexpr Weapon __pkw902[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk902 = __pkk_empty; constexpr auto __pkg902 = __pkg_empty;
    constexpr Weapon __pkw903[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk903 = __pkk_empty; constexpr auto __pkg903 = __pkg_empty;
    constexpr Weapon __pkw904[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk904 = __pkk_empty; constexpr auto __pkg904 = __pkg_empty;
    constexpr Weapon __pkw905[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk905 = __pkk_empty; constexpr auto __pkg905 = __pkg_empty;
    constexpr Weapon __pkw906[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk906 = __pkk_empty; constexpr auto __pkg906 = __pkg_empty;
    constexpr Weapon __pkw907[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk907 = __pkk_empty; constexpr auto __pkg907 = __pkg_empty;
    constexpr Weapon __pkw908[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk908 = __pkk_empty; constexpr auto __pkg908 = __pkg_empty;
    constexpr Weapon __pkw909[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk909 = __pkk_empty; constexpr auto __pkg909 = __pkg_empty;
    constexpr Weapon __pkw910[]{ Weapon::MP9, Weapon::INVALID }; constexpr auto __pkk910 = __pkk_empty; constexpr auto __pkg910 = __pkg_empty;
    constexpr Weapon __pkw911[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk911 = __pkk_empty; constexpr auto __pkg911 = __pkg_empty;
    constexpr Weapon __pkw913[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk913 = __pkk_empty; constexpr auto __pkg913 = __pkg_empty;
    constexpr Weapon __pkw914[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk914 = __pkk_empty; constexpr auto __pkg914 = __pkg_empty;
    constexpr Weapon __pkw915[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk915 = __pkk_empty; constexpr auto __pkg915 = __pkg_empty;
    constexpr Weapon __pkw916[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk916 = __pkk_empty; constexpr auto __pkg916 = __pkg_empty;
    constexpr Weapon __pkw917[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk917 = __pkk_empty; constexpr auto __pkg917 = __pkg_empty;
    constexpr Weapon __pkw918[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk918 = __pkk_empty; constexpr auto __pkg918 = __pkg_empty;
    constexpr Weapon __pkw919[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk919 = __pkk_empty; constexpr auto __pkg919 = __pkg_empty;
    constexpr Weapon __pkw920[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk920 = __pkk_empty; constexpr auto __pkg920 = __pkg_empty;
    constexpr Weapon __pkw941[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk941 = __pkk_empty; constexpr auto __pkg941 = __pkg_empty;
    constexpr Weapon __pkw942[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk942 = __pkk_empty; constexpr auto __pkg942 = __pkg_empty;
    constexpr Weapon __pkw943[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk943 = __pkk_empty; constexpr auto __pkg943 = __pkg_empty;
    constexpr Weapon __pkw944[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk944 = __pkk_empty; constexpr auto __pkg944 = __pkg_empty;
    constexpr Weapon __pkw945[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk945 = __pkk_empty; constexpr auto __pkg945 = __pkg_empty;
    constexpr Weapon __pkw946[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk946 = __pkk_empty; constexpr auto __pkg946 = __pkg_empty;
    constexpr Weapon __pkw947[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk947 = __pkk_empty; constexpr auto __pkg947 = __pkg_empty;
    constexpr Weapon __pkw948[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk948 = __pkk_empty; constexpr auto __pkg948 = __pkg_empty;
    constexpr Weapon __pkw949[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk949 = __pkk_empty; constexpr auto __pkg949 = __pkg_empty;
    constexpr Weapon __pkw950[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk950 = __pkk_empty; constexpr auto __pkg950 = __pkg_empty;
    constexpr Weapon __pkw951[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk951 = __pkk_empty; constexpr auto __pkg951 = __pkg_empty;
    constexpr Weapon __pkw952[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk952 = __pkk_empty; constexpr auto __pkg952 = __pkg_empty;
    constexpr Weapon __pkw953[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk953 = __pkk_empty; constexpr auto __pkg953 = __pkg_empty;
    constexpr Weapon __pkw954[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk954 = __pkk_empty; constexpr auto __pkg954 = __pkg_empty;
    constexpr Weapon __pkw955[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk955 = __pkk_empty; constexpr auto __pkg955 = __pkg_empty;
    constexpr Weapon __pkw956[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk956 = __pkk_empty; constexpr auto __pkg956 = __pkg_empty;
    constexpr Weapon __pkw957[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk957 = __pkk_empty; constexpr auto __pkg957 = __pkg_empty;
    constexpr Weapon __pkw958[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk958 = __pkk_empty; constexpr auto __pkg958 = __pkg_empty;
    constexpr Weapon __pkw959[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk959 = __pkk_empty; constexpr auto __pkg959 = __pkg_empty;
    constexpr Weapon __pkw960[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk960 = __pkk_empty; constexpr auto __pkg960 = __pkg_empty;
    constexpr Weapon __pkw961[]{ Weapon::MAG7, Weapon::INVALID }; constexpr auto __pkk961 = __pkk_empty; constexpr auto __pkg961 = __pkg_empty;
    constexpr Weapon __pkw962[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk962 = __pkk_empty; constexpr auto __pkg962 = __pkg_empty;
    constexpr Weapon __pkw963[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk963 = __pkk_empty; constexpr auto __pkg963 = __pkg_empty;
    constexpr Weapon __pkw964[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk964 = __pkk_empty; constexpr auto __pkg964 = __pkg_empty;
    constexpr Weapon __pkw965[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk965 = __pkk_empty; constexpr auto __pkg965 = __pkg_empty;
    constexpr Weapon __pkw966[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk966 = __pkk_empty; constexpr auto __pkg966 = __pkg_empty;
    constexpr Weapon __pkw967[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk967 = __pkk_empty; constexpr auto __pkg967 = __pkg_empty;
    constexpr Weapon __pkw968[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk968 = __pkk_empty; constexpr auto __pkg968 = __pkg_empty;
    constexpr Weapon __pkw969[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk969 = __pkk_empty; constexpr auto __pkg969 = __pkg_empty;
    constexpr Weapon __pkw970[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk970 = __pkk_empty; constexpr auto __pkg970 = __pkg_empty;
    constexpr Weapon __pkw971[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk971 = __pkk_empty; constexpr auto __pkg971 = __pkg_empty;
    constexpr Weapon __pkw972[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk972 = __pkk_empty; constexpr auto __pkg972 = __pkg_empty;
    constexpr Weapon __pkw973[]{ Weapon::PPBizon, Weapon::INVALID }; constexpr auto __pkk973 = __pkk_empty; constexpr auto __pkg973 = __pkg_empty;
    constexpr Weapon __pkw974[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk974 = __pkk_empty; constexpr auto __pkg974 = __pkg_empty;
    constexpr Weapon __pkw975[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk975 = __pkk_empty; constexpr auto __pkg975 = __pkg_empty;
    constexpr Weapon __pkw976[]{ Weapon::CZ75, Weapon::INVALID }; constexpr auto __pkk976 = __pkk_empty; constexpr auto __pkg976 = __pkg_empty;
    constexpr Weapon __pkw977[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk977 = __pkk_empty; constexpr auto __pkg977 = __pkg_empty;
    constexpr Weapon __pkw978[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk978 = __pkk_empty; constexpr auto __pkg978 = __pkg_empty;
    constexpr Weapon __pkw979[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk979 = __pkk_empty; constexpr auto __pkg979 = __pkg_empty;
    constexpr Weapon __pkw980[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk980 = __pkk_empty; constexpr auto __pkg980 = __pkg_empty;
    constexpr Weapon __pkw981[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk981 = __pkk_empty; constexpr auto __pkg981 = __pkg_empty;
    constexpr Weapon __pkw982[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk982 = __pkk_empty; constexpr auto __pkg982 = __pkg_empty;
    constexpr Weapon __pkw983[]{ Weapon::M249, Weapon::INVALID }; constexpr auto __pkk983 = __pkk_empty; constexpr auto __pkg983 = __pkg_empty;
    constexpr Weapon __pkw984[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk984 = __pkk_empty; constexpr auto __pkg984 = __pkg_empty;
    constexpr Weapon __pkw985[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk985 = __pkk_empty; constexpr auto __pkg985 = __pkg_empty;
    constexpr Weapon __pkw986[]{ Weapon::MP5, Weapon::INVALID }; constexpr auto __pkk986 = __pkk_empty; constexpr auto __pkg986 = __pkg_empty;
    constexpr Weapon __pkw987[]{ Weapon::Nova, Weapon::INVALID }; constexpr auto __pkk987 = __pkk_empty; constexpr auto __pkg987 = __pkg_empty;
    constexpr Weapon __pkw988[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk988 = __pkk_empty; constexpr auto __pkg988 = __pkg_empty;
    constexpr Weapon __pkw989[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk989 = __pkk_empty; constexpr auto __pkg989 = __pkg_empty;
    constexpr Weapon __pkw990[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk990 = __pkk_empty; constexpr auto __pkg990 = __pkg_empty;
    constexpr Weapon __pkw991[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk991 = __pkk_empty; constexpr auto __pkg991 = __pkg_empty;
    constexpr Weapon __pkw992[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk992 = __pkk_empty; constexpr auto __pkg992 = __pkg_empty;
    constexpr Weapon __pkw993[]{ Weapon::M4A4, Weapon::INVALID }; constexpr auto __pkk993 = __pkk_empty; constexpr auto __pkg993 = __pkg_empty;
    constexpr Weapon __pkw994[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk994 = __pkk_empty; constexpr auto __pkg994 = __pkg_empty;
    constexpr Weapon __pkw995[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk995 = __pkk_empty; constexpr auto __pkg995 = __pkg_empty;
    constexpr Weapon __pkw996[]{ Weapon::SSG08, Weapon::INVALID }; constexpr auto __pkk996 = __pkk_empty; constexpr auto __pkg996 = __pkg_empty;
    constexpr Weapon __pkw997[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk997 = __pkk_empty; constexpr auto __pkg997 = __pkg_empty;
    constexpr Weapon __pkw998[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk998 = __pkk_empty; constexpr auto __pkg998 = __pkg_empty;
    constexpr Weapon __pkw999[]{ Weapon::FAMAS, Weapon::INVALID }; constexpr auto __pkk999 = __pkk_empty; constexpr auto __pkg999 = __pkg_empty;
    constexpr Weapon __pkw1000[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk1000 = __pkk_empty; constexpr auto __pkg1000 = __pkg_empty;
    constexpr Weapon __pkw1001[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk1001 = __pkk_empty; constexpr auto __pkg1001 = __pkg_empty;
    constexpr Weapon __pkw1002[]{ Weapon::FiveSeven, Weapon::INVALID }; constexpr auto __pkk1002 = __pkk_empty; constexpr auto __pkg1002 = __pkg_empty;
    constexpr Weapon __pkw1003[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk1003 = __pkk_empty; constexpr auto __pkg1003 = __pkg_empty;
    constexpr Weapon __pkw1004[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk1004 = __pkk_empty; constexpr auto __pkg1004 = __pkg_empty;
    constexpr Weapon __pkw1005[]{ Weapon::DualBerettas, Weapon::INVALID }; constexpr auto __pkk1005 = __pkk_empty; constexpr auto __pkg1005 = __pkg_empty;
    constexpr Weapon __pkw1006[]{ Weapon::DesertEagle, Weapon::INVALID }; constexpr auto __pkk1006 = __pkk_empty; constexpr auto __pkg1006 = __pkg_empty;
    constexpr Weapon __pkw1007[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk1007 = __pkk_empty; constexpr auto __pkg1007 = __pkg_empty;
    constexpr Weapon __pkw1008[]{ Weapon::UMP45, Weapon::INVALID }; constexpr auto __pkk1008 = __pkk_empty; constexpr auto __pkg1008 = __pkg_empty;
    constexpr Weapon __pkw1009[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk1009 = __pkk_empty; constexpr auto __pkg1009 = __pkg_empty;
    constexpr Weapon __pkw1010[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk1010 = __pkk_empty; constexpr auto __pkg1010 = __pkg_empty;
    constexpr Weapon __pkw1011[]{ Weapon::R8Revolver, Weapon::INVALID }; constexpr auto __pkk1011 = __pkk_empty; constexpr auto __pkg1011 = __pkg_empty;
    constexpr Weapon __pkw1012[]{ Weapon::Negev, Weapon::INVALID }; constexpr auto __pkk1012 = __pkk_empty; constexpr auto __pkg1012 = __pkg_empty;
    constexpr Weapon __pkw1013[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk1013 = __pkk_empty; constexpr auto __pkg1013 = __pkg_empty;
    constexpr Weapon __pkw1014[]{ Weapon::SawedOff, Weapon::INVALID }; constexpr auto __pkk1014 = __pkk_empty; constexpr auto __pkg1014 = __pkg_empty;
    constexpr Weapon __pkw1015[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk1015 = __pkk_empty; constexpr auto __pkg1015 = __pkg_empty;
    constexpr Weapon __pkw1016[]{ Weapon::Glock18, Weapon::INVALID }; constexpr auto __pkk1016 = __pkk_empty; constexpr auto __pkg1016 = __pkg_empty;
    constexpr Weapon __pkw1017[]{ Weapon::M4A1S, Weapon::INVALID }; constexpr auto __pkk1017 = __pkk_empty; constexpr auto __pkg1017 = __pkg_empty;
    constexpr Weapon __pkw1018[]{ Weapon::AK47, Weapon::INVALID }; constexpr auto __pkk1018 = __pkk_empty; constexpr auto __pkg1018 = __pkg_empty;
    constexpr Weapon __pkw1019[]{ Weapon::P2000, Weapon::INVALID }; constexpr auto __pkk1019 = __pkk_empty; constexpr auto __pkg1019 = __pkg_empty;
    constexpr Weapon __pkw1020[]{ Weapon::P90, Weapon::INVALID }; constexpr auto __pkk1020 = __pkk_empty; constexpr auto __pkg1020 = __pkg_empty;
    constexpr Weapon __pkw1021[]{ Weapon::XM1014, Weapon::INVALID }; constexpr auto __pkk1021 = __pkk_empty; constexpr auto __pkg1021 = __pkg_empty;
    constexpr Weapon __pkw1022[]{ Weapon::SG553, Weapon::INVALID }; constexpr auto __pkk1022 = __pkk_empty; constexpr auto __pkg1022 = __pkg_empty;
    constexpr Weapon __pkw1023[]{ Weapon::MP7, Weapon::INVALID }; constexpr auto __pkk1023 = __pkk_empty; constexpr auto __pkg1023 = __pkg_empty;
    constexpr Weapon __pkw1024[]{ Weapon::Tec9, Weapon::INVALID }; constexpr auto __pkk1024 = __pkk_empty; constexpr auto __pkg1024 = __pkg_empty;
    constexpr Weapon __pkw1025[]{ Weapon::MAC10, Weapon::INVALID }; constexpr auto __pkk1025 = __pkk_empty; constexpr auto __pkg1025 = __pkg_empty;
    constexpr Weapon __pkw1026[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk1026 = __pkk_empty; constexpr auto __pkg1026 = __pkg_empty;
    constexpr Weapon __pkw1027[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk1027 = __pkk_empty; constexpr auto __pkg1027 = __pkg_empty;
    constexpr Weapon __pkw1028[]{ Weapon::SCAR20, Weapon::INVALID }; constexpr auto __pkk1028 = __pkk_empty; constexpr auto __pkg1028 = __pkg_empty;
    constexpr Weapon __pkw1029[]{ Weapon::AWP, Weapon::INVALID }; constexpr auto __pkk1029 = __pkk_empty; constexpr auto __pkg1029 = __pkg_empty;
    constexpr Weapon __pkw1030[]{ Weapon::P250, Weapon::INVALID }; constexpr auto __pkk1030 = __pkk_empty; constexpr auto __pkg1030 = __pkg_empty;
    constexpr Weapon __pkw1031[]{ Weapon::USPS, Weapon::INVALID }; constexpr auto __pkk1031 = __pkk_empty; constexpr auto __pkg1031 = __pkg_empty;
    constexpr Weapon __pkw1032[]{ Weapon::Galil, Weapon::INVALID }; constexpr auto __pkk1032 = __pkk_empty; constexpr auto __pkg1032 = __pkg_empty;
    constexpr Weapon __pkw1033[]{ Weapon::AUG, Weapon::INVALID }; constexpr auto __pkk1033 = __pkk_empty; constexpr auto __pkg1033 = __pkg_empty;
    constexpr Weapon __pkw1034[]{ Weapon::G3SG1, Weapon::INVALID }; constexpr auto __pkk1034 = __pkk_empty; constexpr auto __pkg1034 = __pkg_empty;

	const PaintKit PaintKits[NPaintKits]{
	    PaintKit(0, "None", __pkw_empty, __pkk_empty, __pkg_empty),
        PaintKit(2, "Groundwater", __pkw2, __pkk2, __pkg2),
        PaintKit(3, "Candy Apple", __pkw3, __pkk3, __pkg3),
        PaintKit(5, "Forest DDPAT", __pkw5, __pkk5, __pkg5),
        PaintKit(6, "Arctic Camo", __pkw6, __pkk6, __pkg6),
        PaintKit(8, "Desert Storm", __pkw8, __pkk8, __pkg8),
        PaintKit(9, "Bengal Tiger", __pkw9, __pkk9, __pkg9),
        PaintKit(10, "Copperhead", __pkw10, __pkk10, __pkg10),
        PaintKit(11, "Skulls", __pkw11, __pkk11, __pkg11),
        PaintKit(12, "Crimson Web", __pkw12, __pkk12, __pkg12),
        PaintKit(13, "Blue Streak", __pkw13, __pkk13, __pkg13),
        PaintKit(14, "Red Laminate", __pkw14, __pkk14, __pkg14),
        PaintKit(15, "Gunsmoke", __pkw15, __pkk15, __pkg15),
        PaintKit(16, "Jungle Tiger", __pkw16, __pkk16, __pkg16),
        PaintKit(17, "Urban DDPAT", __pkw17, __pkk17, __pkg17),
        PaintKit(20, "Virus", __pkw20, __pkk20, __pkg20),
        PaintKit(21, "Granite Marbleized", __pkw21, __pkk21, __pkg21),
        PaintKit(22, "Contrast Spray", __pkw22, __pkk22, __pkg22),
        PaintKit(25, "Forest Leaves", __pkw25, __pkk25, __pkg25),
        PaintKit(26, "Lichen Dashed", __pkw26, __pkk26, __pkg26),
        PaintKit(27, "Bone Mask", __pkw27, __pkk27, __pkg27),
        PaintKit(28, "Anodized Navy", __pkw28, __pkk28, __pkg28),
        PaintKit(30, "Snake Camo", __pkw30, __pkk30, __pkg30),
        PaintKit(32, "Silver", __pkw32, __pkk32, __pkg32),
        PaintKit(33, "Hot Rod", __pkw33, __pkk33, __pkg33),
        PaintKit(34, "Metallic DDPAT", __pkw34, __pkk34, __pkg34),
        PaintKit(36, "Ossified", __pkw36, __pkk36, __pkg36),
        PaintKit(37, "Blaze", __pkw37, __pkk37, __pkg37),
        PaintKit(38, "Fade", __pkw38, __pkk38, __pkg38),
        PaintKit(39, "Bulldozer", __pkw39, __pkk39, __pkg39),
        PaintKit(40, "Night", __pkw40, __pkk40, __pkg40),
        PaintKit(41, "Copper", __pkw41, __pkk41, __pkg41),
        PaintKit(42, "Blue Steel", __pkw42, __pkk42, __pkg42),
        PaintKit(43, "Stained", __pkw43, __pkk43, __pkg43),
        PaintKit(44, "Case Hardened", __pkw44, __pkk44, __pkg44),
        PaintKit(46, "Contractor", __pkw46, __pkk46, __pkg46),
        PaintKit(47, "Colony", __pkw47, __pkk47, __pkg47),
        PaintKit(48, "Dragon Tattoo", __pkw48, __pkk48, __pkg48),
        PaintKit(51, "Lightning Strike", __pkw51, __pkk51, __pkg51),
        PaintKit(59, "Slaughter", __pkw59, __pkk59, __pkg59),
        PaintKit(60, "Dark Water", __pkw60, __pkk60, __pkg60),
        PaintKit(61, "Hypnotic", __pkw61, __pkk61, __pkg61),
        PaintKit(62, "Bloomstick", __pkw62, __pkk62, __pkg62),
        PaintKit(67, "Cold Blooded", __pkw67, __pkk67, __pkg67),
        PaintKit(70, "Carbon Fiber", __pkw70, __pkk70, __pkg70),
        PaintKit(71, "Scorpion", __pkw71, __pkk71, __pkg71),
        PaintKit(72, "Safari Mesh", __pkw72, __pkk72, __pkg72),
        PaintKit(73, "Wings", __pkw73, __pkk73, __pkg73),
        PaintKit(74, "Polar Camo", __pkw74, __pkk74, __pkg74),
        PaintKit(75, "Blizzard Marbleized", __pkw75, __pkk75, __pkg75),
        PaintKit(76, "Winter Forest", __pkw76, __pkk76, __pkg76),
        PaintKit(77, "Boreal Forest", __pkw77, __pkk77, __pkg77),
        PaintKit(78, "Forest Night", __pkw78, __pkk78, __pkg78),
        PaintKit(83, "Orange DDPAT", __pkw83, __pkk83, __pkg83),
        PaintKit(84, "Pink DDPAT", __pkw84, __pkk84, __pkg84),
        PaintKit(90, "Mudder", __pkw90, __pkk90, __pkg90),
        PaintKit(92, "Cyanospatter", __pkw92, __pkk92, __pkg92),
        PaintKit(93, "Caramel", __pkw93, __pkk93, __pkg93),
        PaintKit(95, "Grassland", __pkw95, __pkk95, __pkg95),
        PaintKit(96, "Blue Spruce", __pkw96, __pkk96, __pkg96),
        PaintKit(98, "Ultraviolet", __pkw98, __pkk98, __pkg98),
        PaintKit(99, "Sand Dune", __pkw99, __pkk99, __pkg99),
        PaintKit(100, "Storm", __pkw100, __pkk100, __pkg100),
        PaintKit(101, "Tornado", __pkw101, __pkk101, __pkg101),
        PaintKit(102, "Whiteout", __pkw102, __pkk102, __pkg102),
        PaintKit(104, "Grassland Leaves", __pkw104, __pkk104, __pkg104),
        PaintKit(107, "Polar Mesh", __pkw107, __pkk107, __pkg107),
        PaintKit(110, "Condemned", __pkw110, __pkk110, __pkg110),
        PaintKit(111, "Glacier Mesh", __pkw111, __pkk111, __pkg111),
        PaintKit(116, "Sand Mesh", __pkw116, __pkk116, __pkg116),
        PaintKit(119, "Sage Spray", __pkw119, __pkk119, __pkg119),
        PaintKit(122, "Jungle Spray", __pkw122, __pkk122, __pkg122),
        PaintKit(124, "Sand Spray", __pkw124, __pkk124, __pkg124),
        PaintKit(125, "X-Ray", __pkw125, __pkk125, __pkg125),
        PaintKit(135, "Urban Perforated", __pkw135, __pkk135, __pkg135),
        PaintKit(136, "Waves Perforated", __pkw136, __pkk136, __pkg136),
        PaintKit(141, "Orange Peel", __pkw141, __pkk141, __pkg141),
        PaintKit(143, "Urban Masked", __pkw143, __pkk143, __pkg143),
        PaintKit(147, "Jungle Dashed", __pkw147, __pkk147, __pkg147),
        PaintKit(148, "Sand Dashed", __pkw148, __pkk148, __pkg148),
        PaintKit(149, "Urban Dashed", __pkw149, __pkk149, __pkg149),
        PaintKit(151, "Jungle", __pkw151, __pkk151, __pkg151),
        PaintKit(153, "Demolition", __pkw153, __pkk153, __pkg153),
        PaintKit(154, "Afterimage", __pkw154, __pkk154, __pkg154),
        PaintKit(155, "Bullet Rain", __pkw155, __pkk155, __pkg155),
        PaintKit(156, "Death by Kitty", __pkw156, __pkk156, __pkg156),
        PaintKit(157, "Palm", __pkw157, __pkk157, __pkg157),
        PaintKit(158, "Walnut", __pkw158, __pkk158, __pkg158),
        PaintKit(159, "Brass", __pkw159, __pkk159, __pkg159),
        PaintKit(162, "Splash", __pkw162, __pkk162, __pkg162),
        PaintKit(164, "Modern Hunter", __pkw164, __pkk164, __pkg164),
        PaintKit(165, "Splash Jam", __pkw165, __pkk165, __pkg165),
        PaintKit(166, "Blaze Orange", __pkw166, __pkk166, __pkg166),
        PaintKit(167, "Radiation Hazard", __pkw167, __pkk167, __pkg167),
        PaintKit(168, "Nuclear Threat", __pkw168, __pkk168, __pkg168),
        PaintKit(169, "Fallout Warning", __pkw169, __pkk169, __pkg169),
        PaintKit(170, "Predator", __pkw170, __pkk170, __pkg170),
        PaintKit(171, "Irradiated Alert", __pkw171, __pkk171, __pkg171),
        PaintKit(172, "Black Laminate", __pkw172, __pkk172, __pkg172),
        PaintKit(174, "BOOM", __pkw174, __pkk174, __pkg174),
        PaintKit(175, "Scorched", __pkw175, __pkk175, __pkg175),
        PaintKit(176, "Faded Zebra", __pkw176, __pkk176, __pkg176),
        PaintKit(177, "Memento", __pkw177, __pkk177, __pkg177),
        PaintKit(178, "Doomkitty", __pkw178, __pkk178, __pkg178),
        PaintKit(179, "Nuclear Threat", __pkw179, __pkk179, __pkg179),
        PaintKit(180, "Fire Serpent", __pkw180, __pkk180, __pkg180),
        PaintKit(181, "Corticera", __pkw181, __pkk181, __pkg181),
        PaintKit(182, "Emerald Dragon", __pkw182, __pkk182, __pkg182),
        PaintKit(183, "Overgrowth", __pkw183, __pkk183, __pkg183),
        PaintKit(184, "Corticera", __pkw184, __pkk184, __pkg184),
        PaintKit(185, "Golden Koi", __pkw185, __pkk185, __pkg185),
        PaintKit(186, "Wave Spray", __pkw186, __pkk186, __pkg186),
        PaintKit(187, "Zirka", __pkw187, __pkk187, __pkg187),
        PaintKit(188, "Graven", __pkw188, __pkk188, __pkg188),
        PaintKit(189, "Bright Water", __pkw189, __pkk189, __pkg189),
        PaintKit(190, "Black Limba", __pkw190, __pkk190, __pkg190),
        PaintKit(191, "Tempest", __pkw191, __pkk191, __pkg191),
        PaintKit(192, "Shattered", __pkw192, __pkk192, __pkg192),
        PaintKit(193, "Bone Pile", __pkw193, __pkk193, __pkg193),
        PaintKit(194, "Spitfire", __pkw194, __pkk194, __pkg194),
        PaintKit(195, "Demeter", __pkw195, __pkk195, __pkg195),
        PaintKit(196, "Emerald", __pkw196, __pkk196, __pkg196),
        PaintKit(197, "Anodized Navy", __pkw197, __pkk197, __pkg197),
        PaintKit(198, "Hazard", __pkw198, __pkk198, __pkg198),
        PaintKit(199, "Dry Season", __pkw199, __pkk199, __pkg199),
        PaintKit(200, "Mayan Dreams", __pkw200, __pkk200, __pkg200),
        PaintKit(201, "Palm", __pkw201, __pkk201, __pkg201),
        PaintKit(202, "Jungle DDPAT", __pkw202, __pkk202, __pkg202),
        PaintKit(203, "Rust Coat", __pkw203, __pkk203, __pkg203),
        PaintKit(204, "Mosaico", __pkw204, __pkk204, __pkg204),
        PaintKit(205, "Jungle", __pkw205, __pkk205, __pkg205),
        PaintKit(206, "Tornado", __pkw206, __pkk206, __pkg206),
        PaintKit(207, "Facets", __pkw207, __pkk207, __pkg207),
        PaintKit(208, "Sand Dune", __pkw208, __pkk208, __pkg208),
        PaintKit(209, "Groundwater", __pkw209, __pkk209, __pkg209),
        PaintKit(210, "Anodized Gunmetal", __pkw210, __pkk210, __pkg210),
        PaintKit(211, "Ocean Foam", __pkw211, __pkk211, __pkg211),
        PaintKit(212, "Graphite", __pkw212, __pkk212, __pkg212),
        PaintKit(213, "Ocean Foam", __pkw213, __pkk213, __pkg213),
        PaintKit(214, "Graphite", __pkw214, __pkk214, __pkg214),
        PaintKit(215, "X-Ray", __pkw215, __pkk215, __pkg215),
        PaintKit(216, "Blue Titanium", __pkw216, __pkk216, __pkg216),
        PaintKit(217, "Blood Tiger", __pkw217, __pkk217, __pkg217),
        PaintKit(218, "Hexane", __pkw218, __pkk218, __pkg218),
        PaintKit(219, "Hive", __pkw219, __pkk219, __pkg219),
        PaintKit(220, "Hemoglobin", __pkw220, __pkk220, __pkg220),
        PaintKit(221, "Serum", __pkw221, __pkk221, __pkg221),
        PaintKit(222, "Blood in the Water", __pkw222, __pkk222, __pkg222),
        PaintKit(223, "Nightshade", __pkw223, __pkk223, __pkg223),
        PaintKit(224, "Water Sigil", __pkw224, __pkk224, __pkg224),
        PaintKit(225, "Ghost Camo", __pkw225, __pkk225, __pkg225),
        PaintKit(226, "Blue Laminate", __pkw226, __pkk226, __pkg226),
        PaintKit(227, "Electric Hive", __pkw227, __pkk227, __pkg227),
        PaintKit(228, "Blind Spot", __pkw228, __pkk228, __pkg228),
        PaintKit(229, "Azure Zebra", __pkw229, __pkk229, __pkg229),
        PaintKit(230, "Steel Disruption", __pkw230, __pkk230, __pkg230),
        PaintKit(231, "Cobalt Disruption", __pkw231, __pkk231, __pkg231),
        PaintKit(232, "Crimson Web", __pkw232, __pkk232, __pkg232),
        PaintKit(233, "Tropical Storm", __pkw233, __pkk233, __pkg233),
        PaintKit(234, "Ash Wood", __pkw234, __pkk234, __pkg234),
        PaintKit(235, "VariCamo", __pkw235, __pkk235, __pkg235),
        PaintKit(236, "Night Ops", __pkw236, __pkk236, __pkg236),
        PaintKit(237, "Urban Rubble", __pkw237, __pkk237, __pkg237),
        PaintKit(238, "VariCamo Blue", __pkw238, __pkk238, __pkg238),
        PaintKit(240, "CaliCamo", __pkw240, __pkk240, __pkg240),
        PaintKit(241, "Hunting Blind", __pkw241, __pkk241, __pkg241),
        PaintKit(242, "Army Mesh", __pkw242, __pkk242, __pkg242),
        PaintKit(243, "Gator Mesh", __pkw243, __pkk243, __pkg243),
        PaintKit(244, "Teardown", __pkw244, __pkk244, __pkg244),
        PaintKit(245, "Army Recon", __pkw245, __pkk245, __pkg245),
        PaintKit(246, "Amber Fade", __pkw246, __pkk246, __pkg246),
        PaintKit(247, "Damascus Steel", __pkw247, __pkk247, __pkg247),
        PaintKit(248, "Red Quartz", __pkw248, __pkk248, __pkg248),
        PaintKit(249, "Cobalt Quartz", __pkw249, __pkk249, __pkg249),
        PaintKit(250, "Full Stop", __pkw250, __pkk250, __pkg250),
        PaintKit(251, "Pit Viper", __pkw251, __pkk251, __pkg251),
        PaintKit(252, "Silver Quartz", __pkw252, __pkk252, __pkg252),
        PaintKit(253, "Acid Fade", __pkw253, __pkk253, __pkg253),
        PaintKit(254, "Nitro", __pkw254, __pkk254, __pkg254),
        PaintKit(255, "Asiimov", __pkw255, __pkk255, __pkg255),
        PaintKit(256, "The Kraken", __pkw256, __pkk256, __pkg256),
        PaintKit(257, "Guardian", __pkw257, __pkk257, __pkg257),
        PaintKit(258, "Mehndi", __pkw258, __pkk258, __pkg258),
        PaintKit(259, "Redline", __pkw259, __pkk259, __pkg259),
        PaintKit(260, "Pulse", __pkw260, __pkk260, __pkg260),
        PaintKit(261, "Marina", __pkw261, __pkk261, __pkg261),
        PaintKit(262, "Rose Iron", __pkw262, __pkk262, __pkg262),
        PaintKit(263, "Rising Skull", __pkw263, __pkk263, __pkg263),
        PaintKit(264, "Sandstorm", __pkw264, __pkk264, __pkg264),
        PaintKit(265, "Kami", __pkw265, __pkk265, __pkg265),
        PaintKit(266, "Magma", __pkw266, __pkk266, __pkg266),
        PaintKit(267, "Cobalt Halftone", __pkw267, __pkk267, __pkg267),
        PaintKit(268, "Tread Plate", __pkw268, __pkk268, __pkg268),
        PaintKit(269, "The Fuschia Is Now", __pkw269, __pkk269, __pkg269),
        PaintKit(270, "Victoria", __pkw270, __pkk270, __pkg270),
        PaintKit(271, "Undertow", __pkw271, __pkk271, __pkg271),
        PaintKit(272, "Titanium Bit", __pkw272, __pkk272, __pkg272),
        PaintKit(273, "Heirloom", __pkw273, __pkk273, __pkg273),
        PaintKit(274, "Copper Galaxy", __pkw274, __pkk274, __pkg274),
        PaintKit(275, "Red FragCam", __pkw275, __pkk275, __pkg275),
        PaintKit(276, "Panther", __pkw276, __pkk276, __pkg276),
        PaintKit(277, "Stainless", __pkw277, __pkk277, __pkg277),
        PaintKit(278, "Blue Fissure", __pkw278, __pkk278, __pkg278),
        PaintKit(279, "Asiimov", __pkw279, __pkk279, __pkg279),
        PaintKit(280, "Chameleon", __pkw280, __pkk280, __pkg280),
        PaintKit(281, "Corporal", __pkw281, __pkk281, __pkg281),
        PaintKit(282, "Redline", __pkw282, __pkk282, __pkg282),
        PaintKit(283, "Trigon", __pkw283, __pkk283, __pkg283),
        PaintKit(284, "Heat", __pkw284, __pkk284, __pkg284),
        PaintKit(285, "Terrain", __pkw285, __pkk285, __pkg285),
        PaintKit(286, "Antique", __pkw286, __pkk286, __pkg286),
        PaintKit(287, "Pulse", __pkw287, __pkk287, __pkg287),
        PaintKit(288, "Sergeant", __pkw288, __pkk288, __pkg288),
        PaintKit(289, "Sandstorm", __pkw289, __pkk289, __pkg289),
        PaintKit(290, "Guardian", __pkw290, __pkk290, __pkg290),
        PaintKit(291, "Heaven Guard", __pkw291, __pkk291, __pkg291),
        PaintKit(293, "Death Rattle", __pkw293, __pkk293, __pkg293),
        PaintKit(294, "Green Apple", __pkw294, __pkk294, __pkg294),
        PaintKit(295, "Franklin", __pkw295, __pkk295, __pkg295),
        PaintKit(296, "Meteorite", __pkw296, __pkk296, __pkg296),
        PaintKit(297, "Tuxedo", __pkw297, __pkk297, __pkg297),
        PaintKit(298, "Army Sheen", __pkw298, __pkk298, __pkg298),
        PaintKit(299, "Caged Steel", __pkw299, __pkk299, __pkg299),
        PaintKit(300, "Emerald Pinstripe", __pkw300, __pkk300, __pkg300),
        PaintKit(301, "Atomic Alloy", __pkw301, __pkk301, __pkg301),
        PaintKit(302, "Vulcan", __pkw302, __pkk302, __pkg302),
        PaintKit(303, "Isaac", __pkw303, __pkk303, __pkg303),
        PaintKit(304, "Slashed", __pkw304, __pkk304, __pkg304),
        PaintKit(305, "Torque", __pkw305, __pkk305, __pkg305),
        PaintKit(306, "Antique", __pkw306, __pkk306, __pkg306),
        PaintKit(307, "Retribution", __pkw307, __pkk307, __pkg307),
        PaintKit(308, "Kami", __pkw308, __pkk308, __pkg308),
        PaintKit(309, "Howl", __pkw309, __pkk309, __pkg309),
        PaintKit(310, "Curse", __pkw310, __pkk310, __pkg310),
        PaintKit(311, "Desert Warfare", __pkw311, __pkk311, __pkg311),
        PaintKit(312, "Cyrex", __pkw312, __pkk312, __pkg312),
        PaintKit(313, "Orion", __pkw313, __pkk313, __pkg313),
        PaintKit(314, "Heaven Guard", __pkw314, __pkk314, __pkg314),
        PaintKit(315, "Poison Dart", __pkw315, __pkk315, __pkg315),
        PaintKit(316, "Jaguar", __pkw316, __pkk316, __pkg316),
        PaintKit(317, "Bratatat", __pkw317, __pkk317, __pkg317),
        PaintKit(318, "Road Rash", __pkw318, __pkk318, __pkg318),
        PaintKit(319, "Detour", __pkw319, __pkk319, __pkg319),
        PaintKit(320, "Red Python", __pkw320, __pkk320, __pkg320),
        PaintKit(321, "Master Piece", __pkw321, __pkk321, __pkg321),
        PaintKit(322, "Nitro", __pkw322, __pkk322, __pkg322),
        PaintKit(323, "Rust Coat", __pkw323, __pkk323, __pkg323),
        PaintKit(325, "Chalice", __pkw325, __pkk325, __pkg325),
        PaintKit(326, "Knight", __pkw326, __pkk326, __pkg326),
        PaintKit(327, "Chainmail", __pkw327, __pkk327, __pkg327),
        PaintKit(328, "Hand Cannon", __pkw328, __pkk328, __pkg328),
        PaintKit(329, "Dark Age", __pkw329, __pkk329, __pkg329),
        PaintKit(330, "Briar", __pkw330, __pkk330, __pkg330),
        PaintKit(332, "Royal Blue", __pkw332, __pkk332, __pkg332),
        PaintKit(333, "Indigo", __pkw333, __pkk333, __pkg333),
        PaintKit(334, "Twist", __pkw334, __pkk334, __pkg334),
        PaintKit(335, "Module", __pkw335, __pkk335, __pkg335),
        PaintKit(336, "Desert-Strike", __pkw336, __pkk336, __pkg336),
        PaintKit(337, "Tatter", __pkw337, __pkk337, __pkg337),
        PaintKit(338, "Pulse", __pkw338, __pkk338, __pkg338),
        PaintKit(339, "Caiman", __pkw339, __pkk339, __pkg339),
        PaintKit(340, "Jet Set", __pkw340, __pkk340, __pkg340),
        PaintKit(341, "First Class", __pkw341, __pkk341, __pkg341),
        PaintKit(342, "Leather", __pkw342, __pkk342, __pkg342),
        PaintKit(343, "Commuter", __pkw343, __pkk343, __pkg343),
        PaintKit(344, "Dragon Lore", __pkw344, __pkk344, __pkg344),
        PaintKit(345, "First Class", __pkw345, __pkk345, __pkg345),
        PaintKit(346, "Coach Class", __pkw346, __pkk346, __pkg346),
        PaintKit(347, "Pilot", __pkw347, __pkk347, __pkg347),
        PaintKit(348, "Red Leather", __pkw348, __pkk348, __pkg348),
        PaintKit(349, "Osiris", __pkw349, __pkk349, __pkg349),
        PaintKit(350, "Tigris", __pkw350, __pkk350, __pkg350),
        PaintKit(351, "Conspiracy", __pkw351, __pkk351, __pkg351),
        PaintKit(352, "Fowl Play", __pkw352, __pkk352, __pkg352),
        PaintKit(353, "Water Elemental", __pkw353, __pkk353, __pkg353),
        PaintKit(354, "Urban Hazard", __pkw354, __pkk354, __pkg354),
        PaintKit(355, "Desert-Strike", __pkw355, __pkk355, __pkg355),
        PaintKit(356, "Koi", __pkw356, __pkk356, __pkg356),
        PaintKit(357, "Ivory", __pkw357, __pkk357, __pkg357),
        PaintKit(358, "Supernova", __pkw358, __pkk358, __pkg358),
        PaintKit(359, "Asiimov", __pkw359, __pkk359, __pkg359),
        PaintKit(360, "Cyrex", __pkw360, __pkk360, __pkg360),
        PaintKit(361, "Abyss", __pkw361, __pkk361, __pkg361),
        PaintKit(362, "Labyrinth", __pkw362, __pkk362, __pkg362),
        PaintKit(363, "Traveler", __pkw363, __pkk363, __pkg363),
        PaintKit(364, "Business Class", __pkw364, __pkk364, __pkg364),
        PaintKit(365, "Olive Plaid", __pkw365, __pkk365, __pkg365),
        PaintKit(366, "Green Plaid", __pkw366, __pkk366, __pkg366),
        PaintKit(367, "Reactor", __pkw367, __pkk367, __pkg367),
        PaintKit(368, "Setting Sun", __pkw368, __pkk368, __pkg368),
        PaintKit(369, "Nuclear Waste", __pkw369, __pkk369, __pkg369),
        PaintKit(370, "Bone Machine", __pkw370, __pkk370, __pkg370),
        PaintKit(371, "Styx", __pkw371, __pkk371, __pkg371),
        PaintKit(372, "Nuclear Garden", __pkw372, __pkk372, __pkg372),
        PaintKit(373, "Contamination", __pkw373, __pkk373, __pkg373),
        PaintKit(374, "Toxic", __pkw374, __pkk374, __pkg374),
        PaintKit(375, "Radiation Hazard", __pkw375, __pkk375, __pkg375),
        PaintKit(376, "Chemical Green", __pkw376, __pkk376, __pkg376),
        PaintKit(377, "Hot Shot", __pkw377, __pkk377, __pkg377),
        PaintKit(378, "Fallout Warning", __pkw378, __pkk378, __pkg378),
        PaintKit(379, "Cerberus", __pkw379, __pkk379, __pkg379),
        PaintKit(380, "Wasteland Rebel", __pkw380, __pkk380, __pkg380),
        PaintKit(381, "Grinder", __pkw381, __pkk381, __pkg381),
        PaintKit(382, "Murky", __pkw382, __pkk382, __pkg382),
        PaintKit(383, "Basilisk", __pkw383, __pkk383, __pkg383),
        PaintKit(384, "Griffin", __pkw384, __pkk384, __pkg384),
        PaintKit(385, "Firestarter", __pkw385, __pkk385, __pkg385),
        PaintKit(386, "Dart", __pkw386, __pkk386, __pkg386),
        PaintKit(387, "Urban Hazard", __pkw387, __pkk387, __pkg387),
        PaintKit(388, "Cartel", __pkw388, __pkk388, __pkg388),
        PaintKit(389, "Fire Elemental", __pkw389, __pkk389, __pkg389),
        PaintKit(390, "Highwayman", __pkw390, __pkk390, __pkg390),
        PaintKit(391, "Cardiac", __pkw391, __pkk391, __pkg391),
        PaintKit(392, "Delusion", __pkw392, __pkk392, __pkg392),
        PaintKit(393, "Tranquility", __pkw393, __pkk393, __pkg393),
        PaintKit(394, "Cartel", __pkw394, __pkk394, __pkg394),
        PaintKit(395, "Man-o'-war", __pkw395, __pkk395, __pkg395),
        PaintKit(396, "Urban Shock", __pkw396, __pkk396, __pkg396),
        PaintKit(397, "Naga", __pkw397, __pkk397, __pkg397),
        PaintKit(398, "Chatterbox", __pkw398, __pkk398, __pkg398),
        PaintKit(399, "Catacombs", __pkw399, __pkk399, __pkg399),
        PaintKit(400, "Dragon King", __pkw400, __pkk400, __pkg400),
        PaintKit(401, "System Lock", __pkw401, __pkk401, __pkg401),
        PaintKit(402, "Malachite", __pkw402, __pkk402, __pkg402),
        PaintKit(403, "Deadly Poison", __pkw403, __pkk403, __pkg403),
        PaintKit(404, "Muertos", __pkw404, __pkk404, __pkg404),
        PaintKit(405, "Serenity", __pkw405, __pkk405, __pkg405),
        PaintKit(406, "Grotto", __pkw406, __pkk406, __pkg406),
        PaintKit(407, "Quicksilver", __pkw407, __pkk407, __pkg407),
        PaintKit(409, "Tiger Tooth", __pkw409, __pkk409, __pkg409),
        PaintKit(410, "Damascus Steel", __pkw410, __pkk410, __pkg410),
        PaintKit(411, "Damascus Steel", __pkw411, __pkk411, __pkg411),
        PaintKit(413, "Marble Fade", __pkw413, __pkk413, __pkg413),
        PaintKit(414, "Rust Coat", __pkw414, __pkk414, __pkg414),
        PaintKit(415, "Ruby Doppler", __pkw415, __pkk415, __pkg415),
        PaintKit(416, "Sapphire Doppler", __pkw416, __pkk416, __pkg416),
        PaintKit(417, "Black Pearl Doppler", __pkw417, __pkk417, __pkg417),
        PaintKit(422, "Elite Build", __pkw422, __pkk422, __pkg422),
        PaintKit(423, "Armor Core", __pkw423, __pkk423, __pkg423),
        PaintKit(424, "Worm God", __pkw424, __pkk424, __pkg424),
        PaintKit(425, "Bronze Deco", __pkw425, __pkk425, __pkg425),
        PaintKit(426, "Valence", __pkw426, __pkk426, __pkg426),
        PaintKit(427, "Monkey Business", __pkw427, __pkk427, __pkg427),
        PaintKit(428, "Eco", __pkw428, __pkk428, __pkg428),
        PaintKit(429, "Djinn", __pkw429, __pkk429, __pkg429),
        PaintKit(430, "Hyper Beast", __pkw430, __pkk430, __pkg430),
        PaintKit(431, "Heat", __pkw431, __pkk431, __pkg431),
        PaintKit(432, "Man-o'-war", __pkw432, __pkk432, __pkg432),
        PaintKit(433, "Neon Rider", __pkw433, __pkk433, __pkg433),
        PaintKit(434, "Origami", __pkw434, __pkk434, __pkg434),
        PaintKit(435, "Pole Position", __pkw435, __pkk435, __pkg435),
        PaintKit(436, "Grand Prix", __pkw436, __pkk436, __pkg436),
        PaintKit(437, "Twilight Galaxy", __pkw437, __pkk437, __pkg437),
        PaintKit(438, "Chronos", __pkw438, __pkk438, __pkg438),
        PaintKit(439, "Hades", __pkw439, __pkk439, __pkg439),
        PaintKit(440, "Icarus Fell", __pkw440, __pkk440, __pkg440),
        PaintKit(441, "Minotaur's Labyrinth", __pkw441, __pkk441, __pkg441),
        PaintKit(442, "Asterion", __pkw442, __pkk442, __pkg442),
        PaintKit(443, "Pathfinder", __pkw443, __pkk443, __pkg443),
        PaintKit(444, "Daedalus", __pkw444, __pkk444, __pkg444),
        PaintKit(445, "Hot Rod", __pkw445, __pkk445, __pkg445),
        PaintKit(446, "Medusa", __pkw446, __pkk446, __pkg446),
        PaintKit(447, "Duelist", __pkw447, __pkk447, __pkg447),
        PaintKit(448, "Pandora's Box", __pkw448, __pkk448, __pkg448),
        PaintKit(449, "Poseidon", __pkw449, __pkk449, __pkg449),
        PaintKit(450, "Moon in Libra", __pkw450, __pkk450, __pkg450),
        PaintKit(451, "Sun in Leo", __pkw451, __pkk451, __pkg451),
        PaintKit(452, "Shipping Forecast", __pkw452, __pkk452, __pkg452),
        PaintKit(453, "Emerald", __pkw453, __pkk453, __pkg453),
        PaintKit(454, "Para Green", __pkw454, __pkk454, __pkg454),
        PaintKit(455, "Akihabara Accept", __pkw455, __pkk455, __pkg455),
        PaintKit(456, "Hydroponic", __pkw456, __pkk456, __pkg456),
        PaintKit(457, "Bamboo Print", __pkw457, __pkk457, __pkg457),
        PaintKit(458, "Bamboo Shadow", __pkw458, __pkk458, __pkg458),
        PaintKit(459, "Bamboo Forest", __pkw459, __pkk459, __pkg459),
        PaintKit(460, "Aqua Terrace", __pkw460, __pkk460, __pkg460),
        PaintKit(462, "Counter Terrace", __pkw462, __pkk462, __pkg462),
        PaintKit(463, "Terrace", __pkw463, __pkk463, __pkg463),
        PaintKit(464, "Neon Kimono", __pkw464, __pkk464, __pkg464),
        PaintKit(465, "Orange Kimono", __pkw465, __pkk465, __pkg465),
        PaintKit(466, "Crimson Kimono", __pkw466, __pkk466, __pkg466),
        PaintKit(467, "Mint Kimono", __pkw467, __pkk467, __pkg467),
        PaintKit(468, "Midnight Storm", __pkw468, __pkk468, __pkg468),
        PaintKit(469, "Sunset Storm One", __pkw469, __pkk469, __pkg469),
        PaintKit(470, "Sunset Storm Two", __pkw470, __pkk470, __pkg470),
        PaintKit(471, "Daybreak", __pkw471, __pkk471, __pkg471),
        PaintKit(472, "Impact Drill", __pkw472, __pkk472, __pkg472),
        PaintKit(473, "Seabird", __pkw473, __pkk473, __pkg473),
        PaintKit(474, "Aquamarine Revenge", __pkw474, __pkk474, __pkg474),
        PaintKit(475, "Hyper Beast", __pkw475, __pkk475, __pkg475),
        PaintKit(476, "Yellow Jacket", __pkw476, __pkk476, __pkg476),
        PaintKit(477, "Neural Net", __pkw477, __pkk477, __pkg477),
        PaintKit(478, "Rocket Pop", __pkw478, __pkk478, __pkg478),
        PaintKit(479, "Bunsen Burner", __pkw479, __pkk479, __pkg479),
        PaintKit(480, "Evil Daimyo", __pkw480, __pkk480, __pkg480),
        PaintKit(481, "Nemesis", __pkw481, __pkk481, __pkg481),
        PaintKit(482, "Ruby Poison Dart", __pkw482, __pkk482, __pkg482),
        PaintKit(483, "Loudmouth", __pkw483, __pkk483, __pkg483),
        PaintKit(484, "Ranger", __pkw484, __pkk484, __pkg484),
        PaintKit(485, "Handgun", __pkw485, __pkk485, __pkg485),
        PaintKit(486, "Elite Build", __pkw486, __pkk486, __pkg486),
        PaintKit(487, "Cyrex", __pkw487, __pkk487, __pkg487),
        PaintKit(488, "Riot", __pkw488, __pkk488, __pkg488),
        PaintKit(489, "Torque", __pkw489, __pkk489, __pkg489),
        PaintKit(490, "Frontside Misty", __pkw490, __pkk490, __pkg490),
        PaintKit(491, "Dualing Dragons", __pkw491, __pkk491, __pkg491),
        PaintKit(492, "Survivor Z", __pkw492, __pkk492, __pkg492),
        PaintKit(493, "Flux", __pkw493, __pkk493, __pkg493),
        PaintKit(494, "Stone Cold", __pkw494, __pkk494, __pkg494),
        PaintKit(495, "Wraiths", __pkw495, __pkk495, __pkg495),
        PaintKit(496, "Nebula Crusader", __pkw496, __pkk496, __pkg496),
        PaintKit(497, "Golden Coil", __pkw497, __pkk497, __pkg497),
        PaintKit(498, "Rangeen", __pkw498, __pkk498, __pkg498),
        PaintKit(499, "Cobalt Core", __pkw499, __pkk499, __pkg499),
        PaintKit(500, "Special Delivery", __pkw500, __pkk500, __pkg500),
        PaintKit(501, "Wingshot", __pkw501, __pkk501, __pkg501),
        PaintKit(502, "Green Marine", __pkw502, __pkk502, __pkg502),
        PaintKit(503, "Big Iron", __pkw503, __pkk503, __pkg503),
        PaintKit(504, "Kill Confirmed", __pkw504, __pkk504, __pkg504),
        PaintKit(505, "Scumbria", __pkw505, __pkk505, __pkg505),
        PaintKit(506, "Point Disarray", __pkw506, __pkk506, __pkg506),
        PaintKit(507, "Ricochet", __pkw507, __pkk507, __pkg507),
        PaintKit(508, "Fuel Rod", __pkw508, __pkk508, __pkg508),
        PaintKit(509, "Corinthian", __pkw509, __pkk509, __pkg509),
        PaintKit(510, "Retrobution", __pkw510, __pkk510, __pkg510),
        PaintKit(511, "The Executioner", __pkw511, __pkk511, __pkg511),
        PaintKit(512, "Royal Paladin", __pkw512, __pkk512, __pkg512),
        PaintKit(514, "Power Loader", __pkw514, __pkk514, __pkg514),
        PaintKit(515, "Imperial", __pkw515, __pkk515, __pkg515),
        PaintKit(516, "Shapewood", __pkw516, __pkk516, __pkg516),
        PaintKit(517, "Yorick", __pkw517, __pkk517, __pkg517),
        PaintKit(518, "Outbreak", __pkw518, __pkk518, __pkg518),
        PaintKit(519, "Tiger Moth", __pkw519, __pkk519, __pkg519),
        PaintKit(520, "Avalanche", __pkw520, __pkk520, __pkg520),
        PaintKit(521, "Teclu Burner", __pkw521, __pkk521, __pkg521),
        PaintKit(522, "Fade", __pkw522, __pkk522, __pkg522),
        PaintKit(523, "Amber Fade", __pkw523, __pkk523, __pkg523),
        PaintKit(524, "Fuel Injector", __pkw524, __pkk524, __pkg524),
        PaintKit(525, "Elite Build", __pkw525, __pkk525, __pkg525),
        PaintKit(526, "Photic Zone", __pkw526, __pkk526, __pkg526),
        PaintKit(527, "Kumicho Dragon", __pkw527, __pkk527, __pkg527),
        PaintKit(528, "Cartel", __pkw528, __pkk528, __pkg528),
        PaintKit(529, "Valence", __pkw529, __pkk529, __pkg529),
        PaintKit(530, "Triumvirate", __pkw530, __pkk530, __pkg530),
        PaintKit(532, "Royal Legion", __pkw532, __pkk532, __pkg532),
        PaintKit(533, "The Battlestar", __pkw533, __pkk533, __pkg533),
        PaintKit(534, "Lapis Gator", __pkw534, __pkk534, __pkg534),
        PaintKit(535, "Praetorian", __pkw535, __pkk535, __pkg535),
        PaintKit(536, "Impire", __pkw536, __pkk536, __pkg536),
        PaintKit(537, "Hyper Beast", __pkw537, __pkk537, __pkg537),
        PaintKit(538, "Necropos", __pkw538, __pkk538, __pkg538),
        PaintKit(539, "Jambiya", __pkw539, __pkk539, __pkg539),
        PaintKit(540, "Lead Conduit", __pkw540, __pkk540, __pkg540),
        PaintKit(541, "Fleet Flock", __pkw541, __pkk541, __pkg541),
        PaintKit(542, "Judgement of Anubis", __pkw542, __pkk542, __pkg542),
        PaintKit(543, "Red Astor", __pkw543, __pkk543, __pkg543),
        PaintKit(544, "Ventilators", __pkw544, __pkk544, __pkg544),
        PaintKit(545, "Orange Crash", __pkw545, __pkk545, __pkg545),
        PaintKit(546, "Firefight", __pkw546, __pkk546, __pkg546),
        PaintKit(547, "Spectre", __pkw547, __pkk547, __pkg547),
        PaintKit(548, "Chantico's Fire", __pkw548, __pkk548, __pkg548),
        PaintKit(549, "Bioleak", __pkw549, __pkk549, __pkg549),
        PaintKit(550, "Oceanic", __pkw550, __pkk550, __pkg550),
        PaintKit(551, "Asiimov", __pkw551, __pkk551, __pkg551),
        PaintKit(552, "Fubar", __pkw552, __pkk552, __pkg552),
        PaintKit(553, "Atlas", __pkw553, __pkk553, __pkg553),
        PaintKit(554, "Ghost Crusader", __pkw554, __pkk554, __pkg554),
        PaintKit(555, "Re-Entry", __pkw555, __pkk555, __pkg555),
        PaintKit(556, "Primal Saber", __pkw556, __pkk556, __pkg556),
        PaintKit(557, "Black Tie", __pkw557, __pkk557, __pkg557),
        PaintKit(558, "Lore", __pkw558, __pkk558, __pkg558),
        PaintKit(559, "Lore", __pkw559, __pkk559, __pkg559),
        PaintKit(560, "Lore", __pkw560, __pkk560, __pkg560),
        PaintKit(561, "Lore", __pkw561, __pkk561, __pkg561),
        PaintKit(562, "Lore", __pkw562, __pkk562, __pkg562),
        PaintKit(563, "Black Laminate", __pkw563, __pkk563, __pkg563),
        PaintKit(564, "Black Laminate", __pkw564, __pkk564, __pkg564),
        PaintKit(565, "Black Laminate", __pkw565, __pkk565, __pkg565),
        PaintKit(566, "Black Laminate", __pkw566, __pkk566, __pkg566),
        PaintKit(567, "Black Laminate", __pkw567, __pkk567, __pkg567),
        PaintKit(568, "Emerald Gamma Doppler", __pkw568, __pkk568, __pkg568),
        PaintKit(569, "Gamma Doppler Phase 1", __pkw569, __pkk569, __pkg569),
        PaintKit(570, "Gamma Doppler Phase 2", __pkw570, __pkk570, __pkg570),
        PaintKit(571, "Gamma Doppler Phase 3", __pkw571, __pkk571, __pkg571),
        PaintKit(572, "Gamma Doppler Phase 4", __pkw572, __pkk572, __pkg572),
        PaintKit(573, "Autotronic", __pkw573, __pkk573, __pkg573),
        PaintKit(574, "Autotronic", __pkw574, __pkk574, __pkg574),
        PaintKit(575, "Autotronic", __pkw575, __pkk575, __pkg575),
        PaintKit(576, "Autotronic", __pkw576, __pkk576, __pkg576),
        PaintKit(577, "Autotronic", __pkw577, __pkk577, __pkg577),
        PaintKit(578, "Bright Water", __pkw578, __pkk578, __pkg578),
        PaintKit(579, "Bright Water", __pkw579, __pkk579, __pkg579),
        PaintKit(580, "Freehand", __pkw580, __pkk580, __pkg580),
        PaintKit(581, "Freehand", __pkw581, __pkk581, __pkg581),
        PaintKit(582, "Freehand", __pkw582, __pkk582, __pkg582),
        PaintKit(583, "Aristocrat", __pkw583, __pkk583, __pkg583),
        PaintKit(584, "Phobos", __pkw584, __pkk584, __pkg584),
        PaintKit(585, "Violent Daimyo", __pkw585, __pkk585, __pkg585),
        PaintKit(586, "Wasteland Rebel", __pkw586, __pkk586, __pkg586),
        PaintKit(587, "Mecha Industries", __pkw587, __pkk587, __pkg587),
        PaintKit(588, "Desolate Space", __pkw588, __pkk588, __pkg588),
        PaintKit(589, "Carnivore", __pkw589, __pkk589, __pkg589),
        PaintKit(590, "Exo", __pkw590, __pkk590, __pkg590),
        PaintKit(591, "Imperial Dragon", __pkw591, __pkk591, __pkg591),
        PaintKit(592, "Iron Clad", __pkw592, __pkk592, __pkg592),
        PaintKit(593, "Chopper", __pkw593, __pkk593, __pkg593),
        PaintKit(594, "Harvester", __pkw594, __pkk594, __pkg594),
        PaintKit(595, "Reboot", __pkw595, __pkk595, __pkg595),
        PaintKit(596, "Limelight", __pkw596, __pkk596, __pkg596),
        PaintKit(597, "Bloodsport", __pkw597, __pkk597, __pkg597),
        PaintKit(598, "Aerial", __pkw598, __pkk598, __pkg598),
        PaintKit(599, "Ice Cap", __pkw599, __pkk599, __pkg599),
        PaintKit(600, "Neon Revolution", __pkw600, __pkk600, __pkg600),
        PaintKit(601, "Syd Mead", __pkw601, __pkk601, __pkg601),
        PaintKit(602, "Imprint", __pkw602, __pkk602, __pkg602),
        PaintKit(603, "Directive", __pkw603, __pkk603, __pkg603),
        PaintKit(604, "Roll Cage", __pkw604, __pkk604, __pkg604),
        PaintKit(605, "Scumbria", __pkw605, __pkk605, __pkg605),
        PaintKit(606, "Ventilator", __pkw606, __pkk606, __pkg606),
        PaintKit(607, "Weasel", __pkw607, __pkk607, __pkg607),
        PaintKit(608, "Petroglyph", __pkw608, __pkk608, __pkg608),
        PaintKit(609, "Airlock", __pkw609, __pkk609, __pkg609),
        PaintKit(610, "Dazzle", __pkw610, __pkk610, __pkg610),
        PaintKit(611, "Grim", __pkw611, __pkk611, __pkg611),
        PaintKit(612, "Powercore", __pkw612, __pkk612, __pkg612),
        PaintKit(613, "Triarch", __pkw613, __pkk613, __pkg613),
        PaintKit(614, "Fuel Injector", __pkw614, __pkk614, __pkg614),
        PaintKit(615, "Briefing", __pkw615, __pkk615, __pkg615),
        PaintKit(616, "Slipstream", __pkw616, __pkk616, __pkg616),
        PaintKit(620, "Ultraviolet", __pkw620, __pkk620, __pkg620),
        PaintKit(621, "Ultraviolet", __pkw621, __pkk621, __pkg621),
        PaintKit(622, "Polymer", __pkw622, __pkk622, __pkg622),
        PaintKit(623, "Ironwork", __pkw623, __pkk623, __pkg623),
        PaintKit(624, "Dragonfire", __pkw624, __pkk624, __pkg624),
        PaintKit(625, "Royal Consorts", __pkw625, __pkk625, __pkg625),
        PaintKit(626, "Mecha Industries", __pkw626, __pkk626, __pkg626),
        PaintKit(627, "Cirrus", __pkw627, __pkk627, __pkg627),
        PaintKit(628, "Stinger", __pkw628, __pkk628, __pkg628),
        PaintKit(629, "Black Sand", __pkw629, __pkk629, __pkg629),
        PaintKit(630, "Sand Scale", __pkw630, __pkk630, __pkg630),
        PaintKit(631, "Flashback", __pkw631, __pkk631, __pkg631),
        PaintKit(632, "Buzz Kill", __pkw632, __pkk632, __pkg632),
        PaintKit(633, "Sonar", __pkw633, __pkk633, __pkg633),
        PaintKit(634, "Gila", __pkw634, __pkk634, __pkg634),
        PaintKit(635, "Turf", __pkw635, __pkk635, __pkg635),
        PaintKit(636, "Shallow Grave", __pkw636, __pkk636, __pkg636),
        PaintKit(637, "Cyrex", __pkw637, __pkk637, __pkg637),
        PaintKit(638, "Wasteland Princess", __pkw638, __pkk638, __pkg638),
        PaintKit(639, "Bloodsport", __pkw639, __pkk639, __pkg639),
        PaintKit(640, "Fever Dream", __pkw640, __pkk640, __pkg640),
        PaintKit(641, "Jungle Slipstream", __pkw641, __pkk641, __pkg641),
        PaintKit(642, "Blueprint", __pkw642, __pkk642, __pkg642),
        PaintKit(643, "Xiangliu", __pkw643, __pkk643, __pkg643),
        PaintKit(644, "Decimator", __pkw644, __pkk644, __pkg644),
        PaintKit(645, "Oxide Blaze", __pkw645, __pkk645, __pkg645),
        PaintKit(646, "Capillary", __pkw646, __pkk646, __pkg646),
        PaintKit(647, "Crimson Tsunami", __pkw647, __pkk647, __pkg647),
        PaintKit(648, "Emerald Poison Dart", __pkw648, __pkk648, __pkg648),
        PaintKit(649, "Akoben", __pkw649, __pkk649, __pkg649),
        PaintKit(650, "Ripple", __pkw650, __pkk650, __pkg650),
        PaintKit(651, "Last Dive", __pkw651, __pkk651, __pkg651),
        PaintKit(652, "Scaffold", __pkw652, __pkk652, __pkg652),
        PaintKit(653, "Neo-Noir", __pkw653, __pkk653, __pkg653),
        PaintKit(654, "Seasons", __pkw654, __pkk654, __pkg654),
        PaintKit(655, "Zander", __pkw655, __pkk655, __pkg655),
        PaintKit(656, "Orbit Mk01", __pkw656, __pkk656, __pkg656),
        PaintKit(657, "Blueprint", __pkw657, __pkk657, __pkg657),
        PaintKit(658, "Cobra Strike", __pkw658, __pkk658, __pkg658),
        PaintKit(659, "Macabre", __pkw659, __pkk659, __pkg659),
        PaintKit(660, "Hyper Beast", __pkw660, __pkk660, __pkg660),
        PaintKit(661, "Sugar Rush", __pkw661, __pkk661, __pkg661),
        PaintKit(662, "Oni Taiji", __pkw662, __pkk662, __pkg662),
        PaintKit(663, "Briefing", __pkw663, __pkk663, __pkg663),
        PaintKit(664, "Hellfire", __pkw664, __pkk664, __pkg664),
        PaintKit(665, "Aloha", __pkw665, __pkk665, __pkg665),
        PaintKit(666, "Hard Water", __pkw666, __pkk666, __pkg666),
        PaintKit(667, "Woodsman", __pkw667, __pkk667, __pkg667),
        PaintKit(668, "Red Rock", __pkw668, __pkk668, __pkg668),
        PaintKit(669, "Death Grip", __pkw669, __pkk669, __pkg669),
        PaintKit(670, "Death's Head", __pkw670, __pkk670, __pkg670),
        PaintKit(671, "Cut Out", __pkw671, __pkk671, __pkg671),
        PaintKit(672, "Metal Flowers", __pkw672, __pkk672, __pkg672),
        PaintKit(673, "Morris", __pkw673, __pkk673, __pkg673),
        PaintKit(674, "Triqua", __pkw674, __pkk674, __pkg674),
        PaintKit(675, "The Empress", __pkw675, __pkk675, __pkg675),
        PaintKit(676, "High Roller", __pkw676, __pkk676, __pkg676),
        PaintKit(677, "Hunter", __pkw677, __pkk677, __pkg677),
        PaintKit(678, "See Ya Later", __pkw678, __pkk678, __pkg678),
        PaintKit(679, "Goo", __pkw679, __pkk679, __pkg679),
        PaintKit(680, "Off World", __pkw680, __pkk680, __pkg680),
        PaintKit(681, "Leaded Glass", __pkw681, __pkk681, __pkg681),
        PaintKit(682, "Oceanic", __pkw682, __pkk682, __pkg682),
        PaintKit(683, "Llama Cannon", __pkw683, __pkk683, __pkg683),
        PaintKit(684, "Cracked Opal", __pkw684, __pkk684, __pkg684),
        PaintKit(685, "Jungle Slipstream", __pkw685, __pkk685, __pkg685),
        PaintKit(686, "Phantom", __pkw686, __pkk686, __pkg686),
        PaintKit(687, "Tacticat", __pkw687, __pkk687, __pkg687),
        PaintKit(688, "Exposure", __pkw688, __pkk688, __pkg688),
        PaintKit(689, "Ziggy", __pkw689, __pkk689, __pkg689),
        PaintKit(690, "Stymphalian", __pkw690, __pkk690, __pkg690),
        PaintKit(691, "Mortis", __pkw691, __pkk691, __pkg691),
        PaintKit(692, "Night Riot", __pkw692, __pkk692, __pkg692),
        PaintKit(693, "Flame Test", __pkw693, __pkk693, __pkg693),
        PaintKit(694, "Moonrise", __pkw694, __pkk694, __pkg694),
        PaintKit(695, "Neo-Noir", __pkw695, __pkk695, __pkg695),
        PaintKit(696, "Bloodsport", __pkw696, __pkk696, __pkg696),
        PaintKit(697, "Black Sand", __pkw697, __pkk697, __pkg697),
        PaintKit(698, "Lionfish", __pkw698, __pkk698, __pkg698),
        PaintKit(699, "Wild Six", __pkw699, __pkk699, __pkg699),
        PaintKit(700, "Urban Hazard", __pkw700, __pkk700, __pkg700),
        PaintKit(701, "Grip", __pkw701, __pkk701, __pkg701),
        PaintKit(702, "Aloha", __pkw702, __pkk702, __pkg702),
        PaintKit(703, "SWAG-7", __pkw703, __pkk703, __pkg703),
        PaintKit(704, "Arctic Wolf", __pkw704, __pkk704, __pkg704),
        PaintKit(705, "Cortex", __pkw705, __pkk705, __pkg705),
        PaintKit(706, "Oxide Blaze", __pkw706, __pkk706, __pkg706),
        PaintKit(707, "Neon Rider", __pkw707, __pkk707, __pkg707),
        PaintKit(708, "Amber Slipstream", __pkw708, __pkk708, __pkg708),
        PaintKit(709, "Eco", __pkw709, __pkk709, __pkg709),
        PaintKit(710, "Shred", __pkw710, __pkk710, __pkg710),
        PaintKit(711, "Code Red", __pkw711, __pkk711, __pkg711),
        PaintKit(712, "High Seas", __pkw712, __pkk712, __pkg712),
        PaintKit(713, "Warhawk", __pkw713, __pkk713, __pkg713),
        PaintKit(714, "Nightmare", __pkw714, __pkk714, __pkg714),
        PaintKit(715, "Capillary", __pkw715, __pkk715, __pkg715),
        PaintKit(716, "Toy Soldier", __pkw716, __pkk716, __pkg716),
        PaintKit(717, "Traction", __pkw717, __pkk717, __pkg717),
        PaintKit(718, "PAW", __pkw718, __pkk718, __pkg718),
        PaintKit(719, "Powercore", __pkw719, __pkk719, __pkg719),
        PaintKit(720, "Devourer", __pkw720, __pkk720, __pkg720),
        PaintKit(721, "Survivalist", __pkw721, __pkk721, __pkg721),
        PaintKit(722, "Snek-9", __pkw722, __pkk722, __pkg722),
        PaintKit(723, "Eye of Athena", __pkw723, __pkk723, __pkg723),
        PaintKit(724, "Wild Lotus", __pkw724, __pkk724, __pkg724),
        PaintKit(725, "Day Lily", __pkw725, __pkk725, __pkg725),
        PaintKit(726, "Sunset Lily", __pkw726, __pkk726, __pkg726),
        PaintKit(727, "Midnight Lily", __pkw727, __pkk727, __pkg727),
        PaintKit(728, "Teal Blossom", __pkw728, __pkk728, __pkg728),
        PaintKit(729, "Crimson Blossom", __pkw729, __pkk729, __pkg729),
        PaintKit(730, "Dark Blossom", __pkw730, __pkk730, __pkg730),
        PaintKit(731, "Banana Leaf", __pkw731, __pkk731, __pkg731),
        PaintKit(732, "Synth Leaf", __pkw732, __pkk732, __pkg732),
        PaintKit(733, "Rust Leaf", __pkw733, __pkk733, __pkg733),
        PaintKit(734, "Wild Lily", __pkw734, __pkk734, __pkg734),
        PaintKit(735, "Night Stripe", __pkw735, __pkk735, __pkg735),
        PaintKit(736, "The Prince", __pkw736, __pkk736, __pkg736),
        PaintKit(737, "Cinquedea", __pkw737, __pkk737, __pkg737),
        PaintKit(738, "Orange Murano", __pkw738, __pkk738, __pkg738),
        PaintKit(739, "Violet Murano", __pkw739, __pkk739, __pkg739),
        PaintKit(740, "Navy Murano", __pkw740, __pkk740, __pkg740),
        PaintKit(741, "Dark Filigree", __pkw741, __pkk741, __pkg741),
        PaintKit(742, "Red Filigree", __pkw742, __pkk742, __pkg742),
        PaintKit(743, "Orange Filigree", __pkw743, __pkk743, __pkg743),
        PaintKit(744, "Baroque Red", __pkw744, __pkk744, __pkg744),
        PaintKit(745, "Baroque Purple", __pkw745, __pkk745, __pkg745),
        PaintKit(746, "Baroque Orange", __pkw746, __pkk746, __pkg746),
        PaintKit(747, "Twin Turbo", __pkw747, __pkk747, __pkg747),
        PaintKit(748, "Calf Skin", __pkw748, __pkk748, __pkg748),
        PaintKit(749, "Vino Primo", __pkw749, __pkk749, __pkg749),
        PaintKit(750, "Integrale", __pkw750, __pkk750, __pkg750),
        PaintKit(751, "Hand Brake", __pkw751, __pkk751, __pkg751),
        PaintKit(752, "Fade", __pkw752, __pkk752, __pkg752),
        PaintKit(753, "Dirt Drop", __pkw753, __pkk753, __pkg753),
        PaintKit(754, "Rust Coat", __pkw754, __pkk754, __pkg754),
        PaintKit(755, "Slide", __pkw755, __pkk755, __pkg755),
        PaintKit(756, "Gungnir", __pkw756, __pkk756, __pkg756),
        PaintKit(757, "Emerald Jormungandr", __pkw757, __pkk757, __pkg757),
        PaintKit(758, "Flame Jormungandr", __pkw758, __pkk758, __pkg758),
        PaintKit(759, "Astral Jormungandr", __pkw759, __pkk759, __pkg759),
        PaintKit(760, "Frost Borre", __pkw760, __pkk760, __pkg760),
        PaintKit(761, "Copper Borre", __pkw761, __pkk761, __pkg761),
        PaintKit(762, "Red Stone", __pkw762, __pkk762, __pkg762),
        PaintKit(763, "Mjolnir", __pkw763, __pkk763, __pkg763),
        PaintKit(775, "Facility Sketch", __pkw775, __pkk775, __pkg775),
        PaintKit(776, "Facility Negative", __pkw776, __pkk776, __pkg776),
        PaintKit(777, "Facility Draft", __pkw777, __pkk777, __pkg777),
        PaintKit(778, "Facility Dark", __pkw778, __pkk778, __pkg778),
        PaintKit(779, "Random Access", __pkw779, __pkk779, __pkg779),
        PaintKit(780, "Mainframe", __pkw780, __pkk780, __pkg780),
        PaintKit(781, "Co-Processor", __pkw781, __pkk781, __pkg781),
        PaintKit(782, "Motherboard", __pkw782, __pkk782, __pkg782),
        PaintKit(783, "Bulkhead", __pkw783, __pkk783, __pkg783),
        PaintKit(784, "Coolant", __pkw784, __pkk784, __pkg784),
        PaintKit(785, "Mandrel", __pkw785, __pkk785, __pkg785),
        PaintKit(786, "Exchanger", __pkw786, __pkk786, __pkg786),
        PaintKit(787, "Core Breach", __pkw787, __pkk787, __pkg787),
        PaintKit(788, "Acheron", __pkw788, __pkk788, __pkg788),
        PaintKit(789, "Nuclear Garden", __pkw789, __pkk789, __pkg789),
        PaintKit(790, "Cold Fusion", __pkw790, __pkk790, __pkg790),
        PaintKit(791, "Remote Control", __pkw791, __pkk791, __pkg791),
        PaintKit(792, "Control Panel", __pkw792, __pkk792, __pkg792),
        PaintKit(793, "Converter", __pkw793, __pkk793, __pkg793),
        PaintKit(794, "Sweeper", __pkw794, __pkk794, __pkg794),
        PaintKit(795, "Safety Net", __pkw795, __pkk795, __pkg795),
        PaintKit(796, "Check Engine", __pkw796, __pkk796, __pkg796),
        PaintKit(797, "Brake Light", __pkw797, __pkk797, __pkg797),
        PaintKit(798, "Nitro", __pkw798, __pkk798, __pkg798),
        PaintKit(799, "High Beam", __pkw799, __pkk799, __pkg799),
        PaintKit(800, "Lab Rats", __pkw800, __pkk800, __pkg800),
        PaintKit(801, "Asiimov", __pkw801, __pkk801, __pkg801),
        PaintKit(802, "Momentum", __pkw802, __pkk802, __pkg802),
        PaintKit(803, "Neo-Noir", __pkw803, __pkk803, __pkg803),
        PaintKit(804, "Modest Threat", __pkw804, __pkk804, __pkg804),
        PaintKit(805, "Mecha Industries", __pkw805, __pkk805, __pkg805),
        PaintKit(806, "Scavenger", __pkw806, __pkk806, __pkg806),
        PaintKit(807, "Signal", __pkw807, __pkk807, __pkg807),
        PaintKit(808, "Oxide Blaze", __pkw808, __pkk808, __pkg808),
        PaintKit(809, "Wood Fired", __pkw809, __pkk809, __pkg809),
        PaintKit(810, "Phosphor", __pkw810, __pkk810, __pkg810),
        PaintKit(811, "Magnesium", __pkw811, __pkk811, __pkg811),
        PaintKit(812, "Pipe Down", __pkw812, __pkk812, __pkg812),
        PaintKit(813, "Nevermore", __pkw813, __pkk813, __pkg813),
        PaintKit(814, "Black Sand", __pkw814, __pkk814, __pkg814),
        PaintKit(815, "Danger Close", __pkw815, __pkk815, __pkg815),
        PaintKit(816, "Fubar", __pkw816, __pkk816, __pkg816),
        PaintKit(817, "Flashback", __pkw817, __pkk817, __pkg817),
        PaintKit(835, "Crypsis", __pkw835, __pkk835, __pkg835),
        PaintKit(836, "Uncharted", __pkw836, __pkk836, __pkg836),
        PaintKit(837, "Angry Mob", __pkw837, __pkk837, __pkg837),
        PaintKit(838, "Atheris", __pkw838, __pkk838, __pkg838),
        PaintKit(839, "Bamboozle", __pkw839, __pkk839, __pkg839),
        PaintKit(840, "Whitefish", __pkw840, __pkk840, __pkg840),
        PaintKit(841, "Light Rail", __pkw841, __pkk841, __pkg841),
        PaintKit(842, "Akoben", __pkw842, __pkk842, __pkg842),
        PaintKit(843, "Skull Crusher", __pkw843, __pkk843, __pkg843),
        PaintKit(844, "The Emperor", __pkw844, __pkk844, __pkg844),
        PaintKit(845, "Momentum", __pkw845, __pkk845, __pkg845),
        PaintKit(846, "Gauss", __pkw846, __pkk846, __pkg846),
        PaintKit(847, "Mischief", __pkw847, __pkk847, __pkg847),
        PaintKit(848, "Verdigris", __pkw848, __pkk848, __pkg848),
        PaintKit(849, "Off World", __pkw849, __pkk849, __pkg849),
        PaintKit(850, "Incinegator", __pkw850, __pkk850, __pkg850),
        PaintKit(851, "Moonrise", __pkw851, __pkk851, __pkg851),
        PaintKit(852, "Doppler Phase 1", __pkw852, __pkk852, __pkg852),
        PaintKit(853, "Doppler Phase 2", __pkw853, __pkk853, __pkg853),
        PaintKit(854, "Doppler Phase 3", __pkw854, __pkk854, __pkg854),
        PaintKit(855, "Doppler Phase 4", __pkw855, __pkk855, __pkg855),
        PaintKit(856, "Marble Fade", __pkw856, __pkk856, __pkg856),
        PaintKit(857, "Damascus Steel", __pkw857, __pkk857, __pkg857),
        PaintKit(858, "Damascus Steel", __pkw858, __pkk858, __pkg858),
        PaintKit(859, "Emerald Quartz", __pkw859, __pkk859, __pkg859),
        PaintKit(860, "Pyre", __pkw860, __pkk860, __pkg860),
        PaintKit(861, "Barricade", __pkw861, __pkk861, __pkg861),
        PaintKit(862, "Moss Quartz", __pkw862, __pkk862, __pkg862),
        PaintKit(863, "Night Borre", __pkw863, __pkk863, __pkg863),
        PaintKit(864, "Candy Apple", __pkw864, __pkk864, __pkg864),
        PaintKit(865, "Stone Mosaico", __pkw865, __pkk865, __pkg865),
        PaintKit(866, "Canal Spray", __pkw866, __pkk866, __pkg866),
        PaintKit(867, "Stained Glass", __pkw867, __pkk867, __pkg867),
        PaintKit(868, "Sea Calico", __pkw868, __pkk868, __pkg868),
        PaintKit(869, "Sundown", __pkw869, __pkk869, __pkg869),
        PaintKit(870, "Jungle Thicket", __pkw870, __pkk870, __pkg870),
        PaintKit(871, "Surfwood", __pkw871, __pkk871, __pkg871),
        PaintKit(872, "Bamboo Garden", __pkw872, __pkk872, __pkg872),
        PaintKit(873, "Seabird", __pkw873, __pkk873, __pkg873),
        PaintKit(884, "Embargo", __pkw884, __pkk884, __pkg884),
        PaintKit(885, "Rat Rod", __pkw885, __pkk885, __pkg885),
        PaintKit(886, "Arctic Wolf", __pkw886, __pkk886, __pkg886),
        PaintKit(887, "Containment Breach", __pkw887, __pkk887, __pkg887),
        PaintKit(888, "Acid Wash", __pkw888, __pkk888, __pkg888),
        PaintKit(889, "Decimator", __pkw889, __pkk889, __pkg889),
        PaintKit(890, "Plume", __pkw890, __pkk890, __pkg890),
        PaintKit(891, "Black Sand", __pkw891, __pkk891, __pkg891),
        PaintKit(892, "Memento", __pkw892, __pkk892, __pkg892),
        PaintKit(893, "Neon Ply", __pkw893, __pkk893, __pkg893),
        PaintKit(894, "Obsidian", __pkw894, __pkk894, __pkg894),
        PaintKit(895, "Balance", __pkw895, __pkk895, __pkg895),
        PaintKit(896, "Torn", __pkw896, __pkk896, __pkg896),
        PaintKit(897, "Colony IV", __pkw897, __pkk897, __pkg897),
        PaintKit(898, "Stalker", __pkw898, __pkk898, __pkg898),
        PaintKit(899, "Bloodshot", __pkw899, __pkk899, __pkg899),
        PaintKit(900, "Warbird", __pkw900, __pkk900, __pkg900),
        PaintKit(902, "Aztec", __pkw902, __pkk902, __pkg902),
        PaintKit(903, "Elite 1.6", __pkw903, __pkk903, __pkg903),
        PaintKit(904, "Decommissioned", __pkw904, __pkk904, __pkg904),
        PaintKit(905, "Flash Out", __pkw905, __pkk905, __pkg905),
        PaintKit(906, "Buddy", __pkw906, __pkk906, __pkg906),
        PaintKit(907, "Inferno", __pkw907, __pkk907, __pkg907),
        PaintKit(908, "Classic Crate", __pkw908, __pkk908, __pkg908),
        PaintKit(909, "Popdog", __pkw909, __pkk909, __pkg909),
        PaintKit(910, "Hydra", __pkw910, __pkk910, __pkg910),
        PaintKit(911, "Nostalgia", __pkw911, __pkk911, __pkg911),
        PaintKit(913, "Death by Puppy", __pkw913, __pkk913, __pkg913),
        PaintKit(914, "Assault", __pkw914, __pkk914, __pkg914),
        PaintKit(915, "Agent", __pkw915, __pkk915, __pkg915),
        PaintKit(916, "Plastique", __pkw916, __pkk916, __pkg916),
        PaintKit(917, "Wildfire", __pkw917, __pkk917, __pkg917),
        PaintKit(918, "Sacrifice", __pkw918, __pkk918, __pkg918),
        PaintKit(919, "Commemoration", __pkw919, __pkk919, __pkg919),
        PaintKit(920, "Boroque Sand", __pkw920, __pkk920, __pkg920),
        PaintKit(941, "Phantom Disruptor", __pkw941, __pkk941, __pkg941),
        PaintKit(942, "Tom Cat", __pkw942, __pkk942, __pkg942),
        PaintKit(943, "Capillary", __pkw943, __pkk943, __pkg943),
        PaintKit(944, "Distressed", __pkw944, __pkk944, __pkg944),
        PaintKit(945, "Blue Ply", __pkw945, __pkk945, __pkg945),
        PaintKit(946, "Player Two", __pkw946, __pkk946, __pkg946),
        PaintKit(947, "Disco Tech", __pkw947, __pkk947, __pkg947),
        PaintKit(948, "Justice", __pkw948, __pkk948, __pkg948),
        PaintKit(949, "Desert Strike", __pkw949, __pkk949, __pkg949),
        PaintKit(950, "Prototype", __pkw950, __pkk950, __pkg950),
        PaintKit(951, "Acid Etched", __pkw951, __pkk951, __pkg951),
        PaintKit(952, "Bone Forged", __pkw952, __pkk952, __pkg952),
        PaintKit(953, "Apocalypto", __pkw953, __pkk953, __pkg953),
        PaintKit(954, "Enforcer", __pkw954, __pkk954, __pkg954),
        PaintKit(955, "Darkwing", __pkw955, __pkk955, __pkg955),
        PaintKit(956, "Fever Dream", __pkw956, __pkk956, __pkg956),
        PaintKit(957, "Bullet Queen", __pkw957, __pkk957, __pkg957),
        PaintKit(958, "Ultralight", __pkw958, __pkk958, __pkg958),
        PaintKit(959, "Legion of Anubis", __pkw959, __pkk959, __pkg959),
        PaintKit(960, "Gnarled", __pkw960, __pkk960, __pkg960),
        PaintKit(961, "Monster Call", __pkw961, __pkk961, __pkg961),
        PaintKit(962, "Printstream", __pkw962, __pkk962, __pkg962),
        PaintKit(963, "Vogue", __pkw963, __pkk963, __pkg963),
        PaintKit(964, "Brother", __pkw964, __pkk964, __pkg964),
        PaintKit(965, "Allure", __pkw965, __pkk965, __pkg965),
        PaintKit(966, "Ol' Rusty", __pkw966, __pkk966, __pkg966),
        PaintKit(967, "Mainframe 001", __pkw967, __pkk967, __pkg967),
        PaintKit(968, "Cassette", __pkw968, __pkk968, __pkg968),
        PaintKit(969, "Freight", __pkw969, __pkk969, __pkg969),
        PaintKit(970, "Entombed", __pkw970, __pkk970, __pkg970),
        PaintKit(971, "Tooth Fairy", __pkw971, __pkk971, __pkg971),
        PaintKit(972, "Connexion", __pkw972, __pkk972, __pkg972),
        PaintKit(973, "Runic", __pkw973, __pkk973, __pkg973),
        PaintKit(974, "Kitbash", __pkw974, __pkk974, __pkg974),
        PaintKit(975, "Exoskeleton", __pkw975, __pkk975, __pkg975),
        PaintKit(976, "Vendetta", __pkw976, __pkk976, __pkg976),
        PaintKit(977, "Cocoa Rampage", __pkw977, __pkk977, __pkg977),
        PaintKit(978, "Dezastre", __pkw978, __pkk978, __pkg978),
        PaintKit(979, "Fairy Tale", __pkw979, __pkk979, __pkg979),
        PaintKit(980, "Digital Mesh", __pkw980, __pkk980, __pkg980),
        PaintKit(981, "Vandal", __pkw981, __pkk981, __pkg981),
        PaintKit(982, "Contaminant", __pkw982, __pkk982, __pkg982),
        PaintKit(983, "Deep Relief", __pkw983, __pkk983, __pkg983),
        PaintKit(984, "Printstream", __pkw984, __pkk984, __pkg984),
        PaintKit(985, "Cyber Security", __pkw985, __pkk985, __pkg985),
        PaintKit(986, "Condition Zero", __pkw986, __pkk986, __pkg986),
        PaintKit(987, "Clear Polymer", __pkw987, __pkk987, __pkg987),
        PaintKit(988, "Neo-Noir", __pkw988, __pkk988, __pkg988),
        PaintKit(989, "Parallax", __pkw989, __pkk989, __pkg989),
        PaintKit(990, "Gold Bismuth", __pkw990, __pkk990, __pkg990),
        PaintKit(991, "Monster Mashup", __pkw991, __pkk991, __pkg991),
        PaintKit(992, "The Bronze", __pkw992, __pkk992, __pkg992),
        PaintKit(993, "Global Offensive", __pkw993, __pkk993, __pkg993),
        PaintKit(994, "Charter", __pkw994, __pkk994, __pkg994),
        PaintKit(995, "Surveillance", __pkw995, __pkk995, __pkg995),
        PaintKit(996, "Threat Detected", __pkw996, __pkk996, __pkg996),
        PaintKit(997, "Dispatch", __pkw997, __pkk997, __pkg997),
        PaintKit(998, "Switch Board", __pkw998, __pkk998, __pkg998),
        PaintKit(999, "Prime Conspiracy", __pkw999, __pkk999, __pkg999),
        PaintKit(1000, "Run and Hide", __pkw1000, __pkk1000, __pkg1000),
        PaintKit(1001, "Welcome to the Jungle", __pkw1001, __pkk1001, __pkg1001),
        PaintKit(1002, "Berries And Cherries", __pkw1002, __pkk1002, __pkg1002),
        PaintKit(1003, "Crime Scene", __pkw1003, __pkk1003, __pkg1003),
        PaintKit(1004, "X-Ray", __pkw1004, __pkk1004, __pkg1004),
        PaintKit(1005, "Heist", __pkw1005, __pkk1005, __pkg1005),
        PaintKit(1006, "Night Heist", __pkw1006, __pkk1006, __pkg1006),
        PaintKit(1007, "Vault Heist", __pkw1007, __pkk1007, __pkg1007),
        PaintKit(1008, "Houndstooth", __pkw1008, __pkk1008, __pkg1008),
        PaintKit(1009, "Hot Snakes", __pkw1009, __pkk1009, __pkg1009),
        PaintKit(1010, "Phoenix Chalk", __pkw1010, __pkk1010, __pkg1010),
        PaintKit(1011, "Phoenix Marker", __pkw1011, __pkk1011, __pkg1011),
        PaintKit(1012, "Phoenix Stencil", __pkw1012, __pkk1012, __pkg1012),
        PaintKit(1013, "Phoenix Blacklight", __pkw1013, __pkk1013, __pkg1013),
        PaintKit(1014, "Clay Ambush", __pkw1014, __pkk1014, __pkg1014),
        PaintKit(1015, "Tiger Pit", __pkw1015, __pkk1015, __pkg1015),
        PaintKit(1016, "Franklin", __pkw1016, __pkk1016, __pkg1016),
        PaintKit(1017, "Blue Phosphor", __pkw1017, __pkk1017, __pkg1017),
        PaintKit(1018, "Panthera onca", __pkw1018, __pkk1018, __pkg1018),
        PaintKit(1019, "Panther Camo", __pkw1019, __pkk1019, __pkg1019),
        PaintKit(1020, "Ancient Earth", __pkw1020, __pkk1020, __pkg1020),
        PaintKit(1021, "Ancient Lore", __pkw1021, __pkk1021, __pkg1021),
        PaintKit(1022, "Lush Ruins", __pkw1022, __pkk1022, __pkg1022),
        PaintKit(1023, "Tall Grass", __pkw1023, __pkk1023, __pkg1023),
        PaintKit(1024, "Blast From the Past", __pkw1024, __pkk1024, __pkg1024),
        PaintKit(1025, "Gold Brick", __pkw1025, __pkk1025, __pkg1025),
        PaintKit(1026, "Fade", __pkw1026, __pkk1026, __pkg1026),
        PaintKit(1027, "Target Acquired", __pkw1027, __pkk1027, __pkg1027),
        PaintKit(1028, "Magna Carta", __pkw1028, __pkk1028, __pkg1028),
        PaintKit(1029, "Silk Tiger", __pkw1029, __pkk1029, __pkg1029),
        PaintKit(1030, "Bengal Tiger", __pkw1030, __pkk1030, __pkg1030),
        PaintKit(1031, "Ancient Visions", __pkw1031, __pkk1031, __pkg1031),
        PaintKit(1032, "Dusk Ruins", __pkw1032, __pkk1032, __pkg1032),
        PaintKit(1033, "Carved Jade", __pkw1033, __pkk1033, __pkg1033),
        PaintKit(1034, "Ancient Ritual", __pkw1034, __pkk1034, __pkg1034),
	};
    const PaintKit* PaintKitFromID(int ID)
    {
        for (size_t i = 0; i < NPaintKits; i++)
            if (PaintKits[i].ID == ID)
                return PaintKits + i;
        return nullptr;
    }

    constexpr Weapon WeaponFromId(WeaponId id)
    {
        switch (id)
        {
        default: return Weapon::INVALID;
        case WeaponId::DesertEagle:   return Weapon::DesertEagle;
        case WeaponId::DualBerettas:  return Weapon::DualBerettas;
        case WeaponId::FiveSeveN:     return Weapon::FiveSeven;
        case WeaponId::Glock18:       return Weapon::Glock18;
        case WeaponId::AK47:          return Weapon::AK47;
        case WeaponId::AUG:           return Weapon::AUG;
        case WeaponId::AWP:           return Weapon::AWP;
        case WeaponId::FAMAS:         return Weapon::FAMAS;
        case WeaponId::G3SG1:         return Weapon::G3SG1;
        case WeaponId::GalilAR:       return Weapon::Galil;
        case WeaponId::M249:          return Weapon::M249;
        case WeaponId::M4A4:          return Weapon::M4A4;
        case WeaponId::MAC10:         return Weapon::MAC10;
        case WeaponId::P90:           return Weapon::P90;
        case WeaponId::MP5SD:         return Weapon::MP5;
        case WeaponId::UMP45:         return Weapon::UMP45;
        case WeaponId::XM1014:        return Weapon::XM1014;
        case WeaponId::PPBizon:       return Weapon::PPBizon;
        case WeaponId::MAG7:          return Weapon::MAG7;
        case WeaponId::Negev:         return Weapon::Negev;
        case WeaponId::SawedOff:      return Weapon::SawedOff;
        case WeaponId::Tec9:          return Weapon::Tec9;
        case WeaponId::Zeusx27:       return Weapon::Zeus;
        case WeaponId::P2000:         return Weapon::P2000;
        case WeaponId::MP7:           return Weapon::MP7;
        case WeaponId::MP9:           return Weapon::MP9;
        case WeaponId::Nova:          return Weapon::Nova;
        case WeaponId::P250:          return Weapon::P250;
        case WeaponId::SCAR20:        return Weapon::SCAR20;
        case WeaponId::SG553:         return Weapon::SG553;
        case WeaponId::SSG08:         return Weapon::SSG08;
        case WeaponId::M4A1S:         return Weapon::M4A1S;
        case WeaponId::USPS:          return Weapon::USPS;
        case WeaponId::CZ75Auto:      return Weapon::CZ75;
        case WeaponId::R8Revolver:    return Weapon::R8Revolver;
         // other...
        case WeaponId::C4Explosive:           return Weapon::C4;
        case WeaponId::IncendiaryGrenade:     return Weapon::Incendiary;
        case WeaponId::Molotov:               return Weapon::Molotov;
        case WeaponId::DecoyGrenade:          return Weapon::Decoy;
        case WeaponId::Flashbang:             return Weapon::Flashbang;
        case WeaponId::HighExplosiveGrenade:  return Weapon::HE;
        case WeaponId::SmokeGrenade:          return Weapon::Smoke;
        }
    }
    constexpr Knife KnifeFromId(WeaponId id)
    {
        switch (id)
        {
        default: return Knife::INVALID;

        case WeaponId::NomadKnife: return Knife::Nomad;
        case WeaponId::SkeletonKnife: return Knife::Skeleton;
        case WeaponId::SurvivalKnife: return Knife::Survival;
        case WeaponId::ParacordKnife: return Knife::Paracord;
        case WeaponId::ClassicKnife: return Knife::Classic;
        case WeaponId::BowieKnife: return Knife::Bowie;
        case WeaponId::ButterflyKnife: return Knife::Butterfly;
        case WeaponId::FalchionKnife: return Knife::Falchion;
        case WeaponId::FlipKnife: return Knife::Flip;
        case WeaponId::GutKnife: return Knife::Gut;
        case WeaponId::HuntsmanKnife: return Knife::Huntsman;
        case WeaponId::Karambit: return Knife::Karambit;
        case WeaponId::Bayonet: return Knife::Bayonet;
        case WeaponId::M9Bayonet: return Knife::M9Bayonet;
        case WeaponId::NavajaKnife: return Knife::Navaja;
        case WeaponId::ShadowDaggers: return Knife::ShadowDaggers;
        case WeaponId::StilettoKnife: return Knife::Stiletto;
        case WeaponId::TalonKnife: return Knife::Talon;
        case WeaponId::UrsusKnife: return Knife::Ursus;
        case WeaponId::TDefaultKnife: return Knife::TDefault;
        case WeaponId::CTDefaultKnife: return Knife::CTDefault;
        }
    }
    constexpr WeaponId IdFromKnife(Knife k)
    {
        switch (k)
        {
        default: return WeaponId::INVALID;

        case Knife::Nomad:         return WeaponId::NomadKnife;
        case Knife::Skeleton:      return WeaponId::SkeletonKnife;
        case Knife::Survival:      return WeaponId::SurvivalKnife;
        case Knife::Paracord:      return WeaponId::ParacordKnife;
        case Knife::Classic:       return WeaponId::ClassicKnife;
        case Knife::Bowie:         return WeaponId::BowieKnife;
        case Knife::Butterfly:     return WeaponId::ButterflyKnife;
        case Knife::Falchion:      return WeaponId::FalchionKnife;
        case Knife::Flip:          return WeaponId::FlipKnife;
        case Knife::Gut:           return WeaponId::GutKnife;
        case Knife::Huntsman:      return WeaponId::HuntsmanKnife;
        case Knife::Karambit:      return WeaponId::Karambit;
        case Knife::Bayonet:       return WeaponId::Bayonet;
        case Knife::M9Bayonet:     return WeaponId::M9Bayonet;
        case Knife::Navaja:        return WeaponId::NavajaKnife;
        case Knife::ShadowDaggers: return WeaponId::ShadowDaggers;
        case Knife::Stiletto:      return WeaponId::StilettoKnife;
        case Knife::Talon:         return WeaponId::TalonKnife;
        case Knife::Ursus:         return WeaponId::UrsusKnife;
        case Knife::TDefault:      return WeaponId::TDefaultKnife;
        case Knife::CTDefault:     return WeaponId::CTDefaultKnife;
        }
    }
    constexpr const char* GetKnifeModel(Knife knife)
    {
        switch (knife)
        {
        default: return "";

        case Knife::Nomad:         return "models/weapons/v_knife_outdoor.mdl";
        case Knife::Skeleton:      return "models/weapons/v_knife_skeleton.mdl";
        case Knife::Survival:      return "models/weapons/v_knife_canis.mdl";
        case Knife::Paracord:      return "models/weapons/v_knife_cord.mdl";
        case Knife::Classic:       return "models/weapons/v_knife_css.mdl";
        case Knife::Bowie:         return "models/weapons/v_knife_survival_bowie.mdl";
        case Knife::Butterfly:     return "models/weapons/v_knife_butterfly.mdl";
        case Knife::Falchion:      return "models/weapons/v_knife_falchion_advanced.mdl";
        case Knife::Flip:          return "models/weapons/v_knife_flip.mdl";
        case Knife::Gut:           return "models/weapons/v_knife_gut.mdl";
        case Knife::Huntsman:      return "models/weapons/v_knife_tactical.mdl";
        case Knife::Karambit:      return "models/weapons/v_knife_karam.mdl";
        case Knife::Bayonet:       return "models/weapons/v_knife_bayonet.mdl";
        case Knife::M9Bayonet:     return "models/weapons/v_knife_m9_bay.mdl";
        case Knife::Navaja:        return "models/weapons/v_knife_gypsy_jackknife.mdl";
        case Knife::ShadowDaggers: return "models/weapons/v_knife_push.mdl";
        case Knife::Stiletto:      return "models/weapons/v_knife_stiletto.mdl";
        case Knife::Talon:         return "models/weapons/v_knife_widowmaker.mdl";
        case Knife::Ursus:         return "models/weapons/v_knife_ursus.mdl";
        case Knife::TDefault:      return "models/weapons/v_knife_default_t.mdl";
        case Knife::CTDefault:     return "models/weapons/v_knife_default_ct.mdl";
        }
    }
};