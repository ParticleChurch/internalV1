#pragma once

class ESP
{
private:
	void DrawBoxes(Vec TL, Vec BR);
	void DrawName(Vec TL, Vec BR, char Name[128], Color clr);
	void DrawWeapon(Vec TL, Vec BR, Entity*);
	void DrawIndicators(Entity* ent, Color clr);
	void DrawHealth(Vec TL, Vec BR, int Health, Color fg, Color bg);
	void DrawSkeleton(Entity*);
	void GetBounds(Entity* ent, Vec& TL, Vec& BR);

	std::map<WeaponId, wchar_t> WeapIcon = {
        {WeaponId::DesertEagle, 'F' },
        {WeaponId::DualBerettas, 'S' },
        {WeaponId::FiveSeveN, 'U' },
        {WeaponId::Glock18, 'C' },
        {WeaponId::AK47, 'B' },
        {WeaponId::AUG, 'E' },
        {WeaponId::AWP, 'R' },
        {WeaponId::FAMAS, 'T' },
        {WeaponId::G3SG1, 'I' },
        {WeaponId::GalilAR, 'V' },
        {WeaponId::M249, 'Z' },
        {WeaponId::M4A4, 'W' },
        {WeaponId::MAC10, 'L' },
        {WeaponId::P90, 'M' },
        {WeaponId::UMP45, 'Q' },
        {WeaponId::XM1014, ']' },
        {WeaponId::PPBizon, 'D' },
        {WeaponId::MAG7, 'K' },
        {WeaponId::Negev, 'Z' },
        {WeaponId::SawedOff, 'K' },
        {WeaponId::Tec9, 'C' },
        {WeaponId::Zeusx27, 'Y' },
        {WeaponId::P2000, 'Y' },
        {WeaponId::MP7, 'X' },
        {WeaponId::MP9, 'D' },
        {WeaponId::Nova, 'K' },
        {WeaponId::P250, 'Y' },
        {WeaponId::SCAR20, 'I' },
        {WeaponId::SG553, '[' },
        {WeaponId::SSG08, 'N' },
        {WeaponId::CTDefaultKnife, 'J' },
        {WeaponId::Flashbang, 'G' },
        {WeaponId::HighExplosiveGrenade, 'H' },
        {WeaponId::SmokeGrenade, 'P' },
        {WeaponId::Molotov, 'H' },
        {WeaponId::DecoyGrenade, 'G' },
        {WeaponId::FireBomb, 'H' },
        {WeaponId::C4Explosive, '\\' },
        {WeaponId::TDefaultKnife, 'J' },
        {WeaponId::M4A1S, 'W' },
        {WeaponId::USPS, 'Y' },
        {WeaponId::CZ75Auto, 'Y' },
        {WeaponId::R8Revolver, 'F' },
         {WeaponId::Bayonet, 'J' },
        {WeaponId::FlipKnife, 'J' },
        {WeaponId::GutKnife, 'J' },
        {WeaponId::Karambit, 'J' },
        {WeaponId::M9Bayonet, 'J' },
        {WeaponId::HuntsmanKnife, 'J' },
        {WeaponId::FalchionKnife, 'J' },
        {WeaponId::BowieKnife, 'J' },
        {WeaponId::ButterflyKnife, 'J' },
        {WeaponId::ShadowDaggers, 'J' }
	};

	struct BulletTracer
	{
		Vec src;
		Vec end;
		float SimTime;
	};
	std::vector<BulletTracer> traces;

public:
	std::vector<Vec> points;
	void Run_PaintTraverse();
	void RunFSN();
	void Run_GameEvent(GameEvent* event);

	
};

extern ESP* esp;