#pragma once

enum class WeaponId_ : int
{
    DesertEagle = 1,
    DualBerettas = 2,
    FiveSeveN = 3,
    Glock18 = 4,
    AK47 = 7,
    AUG = 8,
    AWP = 9,
    FAMAS = 10,
    G3SG1 = 11,
    GalilAR = 13,
    M249 = 14,
    M4A4 = 16,
    MAC10 = 17,
    P90 = 19,
    RepulsorDevice = 20,
    MP5SD = 23,
    UMP45 = 24,
    XM1014 = 25,
    PPBizon = 26,
    MAG7 = 27,
    Negev = 28,
    SawedOff = 29,
    Tec9 = 30,
    Zeusx27 = 31,
    P2000 = 32,
    MP7 = 33,
    MP9 = 34,
    Nova = 35,
    P250 = 36,
    BallisticShield = 37,
    SCAR20 = 38,
    SG553 = 39,
    SSG08 = 40,
    GoldenKnife = 41,
    TDefaultKnife = 42,
    Flashbang = 43,
    HighExplosiveGrenade = 44,
    SmokeGrenade = 45,
    Molotov = 46,
    DecoyGrenade = 47,
    IncendiaryGrenade = 48,
    C4Explosive = 49,
    KevlarVest = 50,
    KevlarHelmet = 51,
    HeavyAssaultSuit = 52,
    DefuseKit = 55,
    RescueKit = 56,
    MediShot = 57,
    MusicKit = 58,
    CTDefaultKnife = 59,
    M4A1S = 60,
    USPS = 61,
    TradeUpContract = 62,
    CZ75Auto = 63,
    R8Revolver = 64,
    TacticalAwarenessGrenade = 68,
    BareHands = 69,
    BreachCharge = 70,
    Tablet = 72,
    Melee = 74,
    Axe = 75,
    Hammer = 76,
    Wrench = 78,
    SpectralShiv = 80,
    FireBomb = 81,
    DiversionDevice = 82,
    FragGrenade = 83,
    Snowball = 84,
    BumpMine = 85,
    Bayonet = 500,
    ClassicKnife = 503,
    FlipKnife = 505,
    GutKnife = 506,
    Karambit = 507,
    M9Bayonet = 508,
    HuntsmanKnife = 509,
    FalchionKnife = 512,
    BowieKnife = 514,
    ButterflyKnife = 515,
    ShadowDaggers = 516,
    ParacordKnife = 517,
    SurvivalKnife = 518,
    UrsusKnife = 519,
    NavajaKnife = 520,
    NomadKnife = 521,
    StilettoKnife = 522,
    TalonKnife = 523,
    SkeletonKnife = 525,
    BrokenFangGloves = 4725,
    BloodhoundGloves = 5027,
    DefaultTGloves = 5028,
    DefaultCTGloves = 5029,
    SportGloves = 5030,
    DriverGloves = 5031,
    HandWraps = 5032,
    MotoGloves = 5033,
    SpecialistGloves = 5034,
    HydraGloves = 5035,
};

enum class WeaponId : short {
    Deagle = 1,
    Elite,
    Fiveseven,
    Glock,
    Ak47 = 7,
    Aug,
    Awp,
    Famas,
    G3SG1,
    GalilAr = 13,
    M249,
    M4A1 = 16,
    Mac10,
    P90 = 19,
    Mp5sd = 23,
    Ump45,
    Xm1014,
    Bizon,
    Mag7,
    Negev,
    Sawedoff,
    Tec9,
    Taser,
    Hkp2000,
    Mp7,
    Mp9,
    Nova,
    P250,
    Scar20 = 38,
    Sg553,
    Ssg08,
    WEAPON_FLASHBANG = 43,
    WEAPON_HEGRENADE = 44,
    WEAPON_SMOKEGRENADE = 45,
    WEAPON_MOLOTOV = 46,
    WEAPON_DECOY = 47,
    WEAPON_INCGRENADE = 48,
    GoldenKnife,
    M4a1_s = 60,
    Usp_s,
    Cz75a = 63,
    Revolver,
    GhostKnife = 80,
    Bayonet = 500,
    Flip = 505,
    Gut,
    Karambit,
    M9Bayonet,
    Huntsman,
    Falchion = 512,
    Bowie = 514,
    Butterfly,
    Daggers,
    Ursus = 519,
    Navaja,
    Stiletto = 522,
    Talon,
    GloveStuddedBloodhound = 5027,
    GloveT,
    GloveCT,
    GloveSporty,
    GloveSlick,
    GloveLeatherWrap,
    GloveMotorcycle,
    GloveSpecialist,
    GloveHydra
};

constexpr int GetWeaponIndex(WeaponId weaponId) noexcept
{
    switch (weaponId) {
    default: return 0;

    case WeaponId::Glock    : return 1;
    case WeaponId::Hkp2000  : return 2;
    case WeaponId::Usp_s    : return 3;
    case WeaponId::Elite    : return 4;
    case WeaponId::P250     : return 5;
    case WeaponId::Tec9     : return 6;
    case WeaponId::Fiveseven: return 7;
    case WeaponId::Cz75a    : return 8;
    case WeaponId::Deagle   : return 9;
    case WeaponId::Revolver : return 10;

    case WeaponId::Nova     : return 11;
    case WeaponId::Xm1014   : return 12;
    case WeaponId::Sawedoff : return 13;
    case WeaponId::Mag7     : return 14;
    case WeaponId::M249     : return 15;
    case WeaponId::Negev    : return 16;

    case WeaponId::Mac10    : return 17;
    case WeaponId::Mp9      : return 18;
    case WeaponId::Mp7      : return 19;
    case WeaponId::Mp5sd    : return 20;
    case WeaponId::Ump45    : return 21;
    case WeaponId::P90      : return 22;
    case WeaponId::Bizon    : return 23;

    case WeaponId::GalilAr  : return 24;
    case WeaponId::Famas    : return 25;
    case WeaponId::Ak47     : return 26;
    case WeaponId::M4A1     : return 27;
    case WeaponId::M4a1_s   : return 28;
    case WeaponId::Ssg08    : return 29;
    case WeaponId::Sg553    : return 30;
    case WeaponId::Aug      : return 31;
    case WeaponId::Awp      : return 32;
    case WeaponId::G3SG1    : return 33;
    case WeaponId::Scar20   : return 34;

    case WeaponId::Taser    : return 39;
    case WeaponId::WEAPON_FLASHBANG:return 41;
    case WeaponId::WEAPON_DECOY:return 42;
    case WeaponId::WEAPON_HEGRENADE:return 43;
    case WeaponId::WEAPON_INCGRENADE:return 44;
    case WeaponId::WEAPON_MOLOTOV:return 45;
    case WeaponId::WEAPON_SMOKEGRENADE: return 46;
    }
}
constexpr int GetWeaponClass(WeaponId weaponId) noexcept
{
    switch (weaponId) {
    default: return 0;

    case WeaponId::Glock:
    case WeaponId::Hkp2000:
    case WeaponId::Usp_s:
    case WeaponId::Elite:
    case WeaponId::P250:
    case WeaponId::Tec9:
    case WeaponId::Fiveseven:
    case WeaponId::Cz75a:
    case WeaponId::Deagle:
    case WeaponId::Revolver: return 35;

    case WeaponId::Nova:
    case WeaponId::Xm1014:
    case WeaponId::Sawedoff:
    case WeaponId::Mag7:
    case WeaponId::M249:
    case WeaponId::Negev: return 36;

    case WeaponId::Mac10:
    case WeaponId::Mp9:
    case WeaponId::Mp7:
    case WeaponId::Mp5sd:
    case WeaponId::Ump45:
    case WeaponId::P90:
    case WeaponId::Bizon: return 37;

    case WeaponId::GalilAr:
    case WeaponId::Famas:
    case WeaponId::Ak47:
    case WeaponId::M4A1:
    case WeaponId::M4a1_s:
    case WeaponId::Ssg08:
    case WeaponId::Sg553:
    case WeaponId::Aug:
    case WeaponId::Awp:
    case WeaponId::G3SG1:
    case WeaponId::Scar20: return 38;

    case WeaponId::WEAPON_FLASHBANG:
    case WeaponId::WEAPON_DECOY:
    case WeaponId::WEAPON_HEGRENADE:
    case WeaponId::WEAPON_INCGRENADE:
    case WeaponId::WEAPON_MOLOTOV:
    case WeaponId::WEAPON_SMOKEGRENADE: return 40;
    }
}