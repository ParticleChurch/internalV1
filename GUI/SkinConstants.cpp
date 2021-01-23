#include "SkinConstants.hpp"

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
		"Sawed-Off", "unknown-sawed_off", // TODO
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

std::vector<std::string> Skins::KnifeSkins = {
	"None",
	"Doppler Phase 1",
	"Doppler Phase 2",
	"Doppler Phase 3",
	"Doppler Phase 4",
	"Ruby Doppler",
	"Sapphire Doppler",
	"Marble Fade",
	"Fade",
	"Tiger Tooth",
	"Ultraviolet",
};

std::vector<std::string> Skins::KnifeModels = {
	"None",
	"Talon",
	"Karambit",
	"Bowie",
	"Huntsman",
	"Shadow Daggers",
	"CT Default",
	"T Default",
};

//last because longest lmao
std::vector<std::string> Skins::WeaponSkins = {
	"None",
	"Candy Apple",
	"Forest DDPAT",
	"Arctic Camo",
	"Desert Storm",
	"Bengal Tiger",
	"Copperhead",
	"Skulls",
	"Crimson Web",
	"Blue Streak",
	"Red Laminate",
	"Gunsmoke",
	"Jungle Tiger",
	"Urban DDPAT",
	"Virus",
	"Granite Marbleized",
	"Contrast Spray",
	"Forest Leaves",
};