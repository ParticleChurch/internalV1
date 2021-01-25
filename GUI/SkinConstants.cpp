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

	};

	std::vector<std::string> GloveNames = {

	};

	std::vector<PaintKit> PaintKits
	{
		PaintKit(0, "default", "-"),
		PaintKit(2, "so_olive", "Groundwater", {Weapon::Glock18, Weapon::Tec9}),
		PaintKit(3, "so_red", "Candy Apple", {Weapon::FiveSeven, Weapon::MAC10, Weapon::Nova, Weapon::Glock18, Weapon::PPBizon}),
		PaintKit(5, "hy_ddpat", "Forest DDPAT", {Weapon::MP7, Weapon::SawedOff}),
		PaintKit(6, "hy_arctic", "Arctic Camo", {Weapon::G3SG1}),
		PaintKit(8, "hy_desert", "Desert Storm", {Weapon::M4A4, Weapon::G3SG1}),
		PaintKit(9, "hy_tiger", "Bengal Tiger", {Weapon::AUG}),
		PaintKit(10, "hy_copperhead", "Copperhead", {Weapon::AUG}),
		PaintKit(11, "hy_skulls", "Skulls", {Weapon::MP7}),
		PaintKit(12, "hy_webs", "Crimson Web", {Weapon::CZ75, Weapon::R8Revolver}),
		PaintKit(13, "hy_splatter", "Blue Streak", {Weapon::PPBizon}),
		PaintKit(14, "hy_ak47lam", "Red Laminate", {Weapon::AK47}),
		PaintKit(15, "hy_gelpen", "Gunsmoke", {Weapon::P250, Weapon::UMP45, Weapon::MP7}),
		PaintKit(16, "hy_v_tiger", "Jungle Tiger", {Weapon::M4A1S}),
		PaintKit(17, "hy_ddpat_urb", "Urban DDPAT", {Weapon::MAC10, Weapon::UMP45, Weapon::M4A1S, Weapon::Tec9, Weapon::DesertEagle}),
		PaintKit(20, "hy_zombie", "Virus", {Weapon::P90}),
		PaintKit(21, "hy_granite", "Granite Marbleized", {Weapon::P2000}),
		PaintKit(22, "sp_spray", "Contrast Spray", {Weapon::FAMAS, Weapon::M249}),
		PaintKit(25, "sp_leaves", "Forest Leaves", {Weapon::Nova, Weapon::PPBizon, Weapon::USPS}),
		PaintKit(26, "sp_short_tape", "Lichen Dashed", {Weapon::SSG08}),
		PaintKit(27, "sp_tape", "Bone Mask", {Weapon::P250, Weapon::R8Revolver}),
		PaintKit(28, "an_navy", "Anodized Navy", {Weapon::DualBerettas, Weapon::Negev, Weapon::MP7, Weapon::SG553}),
		PaintKit(30, "sp_snake", "Snake Camo", {Weapon::AWP, Weapon::SawedOff}),
	};
};

/*
std::vector<std::vector<std::string>> Skins::WeaponGroups = {
	{"Pistol", "pistol",
		"Glock-18", "glock",
		"P2000", "hkp2000",
		"USP-S", "usp_silencer",
		"Dual Berettas", "elite", // lol wtf valve
		"P250", "p250",
		"Tec-9", "tec9",
		"Five-Seven", "fn57",
		"CZ-75", "cz75a",
		"Desert Eagle",	"deagle",
		"R8 Revolver", "revolver"
	},
	{"Heavy", "heavy",
		"Nova", "nove",
		"XM1014", "xm1014",
		"Sawed-Off", "sawedoff", // TODO check if this is right
		"MAG-7", "mag7",
		"M249", "m249",
		"Negev", "negev",
	},
	{"SMG", "smg",
		"MP9", "mp9", 
		"MAC-10", "mac10", 
		"MP7", "mp7", 
		"MP5", "mp5sd", 
		"UMP-45", "ump45", 
		"P90", "p90", 
		"PP-Bizon", "bizon"
	},
	{"Rifle", "rifle",
		"FAMAS", "famas",
		"Galil", "galilar",
		"M4A4", "m4a4",
		"AK-47", "ak47",
		"M4A1-S", "m4a1_silencer",
		"SSG 08", "ssg08",
		"AUG", "aug",
		"SG 553", "sg556", // valve why, wtf?
		"AWP", "awp",
		"SCAR-20", "scar20",
		"G3SG1", "g3sg1"
	},
	{"Other", "other",
		"Zeus x27", "taser",
		"C4", "c4",
		"Incendiary", "incgrenade",
		"Molotov", "molotov", 
		"Decoy", "decoy",
		"Flashbang", "flashbang",
		"HE Grenade", "hegrenade", 
		"Smoke", "smokegrenade"
	},
};
*/