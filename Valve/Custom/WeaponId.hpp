#pragma once

enum class WeaponId : int
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
    CTDefaultKnife = 42,
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
    TDefaultKnife = 59,
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

constexpr int GetWeaponIndex(WeaponId weaponId) noexcept
{
    switch (weaponId) {
    default: return 0;

    case WeaponId::Glock18         : return 1;
    case WeaponId::P2000           : return 2;
    case WeaponId::USPS            : return 3;
    case WeaponId::DualBerettas    : return 4;
    case WeaponId::P250            : return 5;
    case WeaponId::Tec9            : return 6;
    case WeaponId::FiveSeveN       : return 7;
    case WeaponId::CZ75Auto        : return 8;
    case WeaponId::DesertEagle     : return 9;
    case WeaponId::R8Revolver      : return 10;

    case WeaponId::Nova            : return 11;
    case WeaponId::XM1014          : return 12;
    case WeaponId::SawedOff        : return 13;
    case WeaponId::MAG7            : return 14;
    case WeaponId::M249            : return 15;
    case WeaponId::Negev           : return 16;

    case WeaponId::MAC10           : return 17;
    case WeaponId::MP9             : return 18;
    case WeaponId::MP7             : return 19;
    case WeaponId::MP5SD           : return 20;
    case WeaponId::UMP45           : return 21;
    case WeaponId::P90             : return 22;
    case WeaponId::PPBizon         : return 23;

    case WeaponId::GalilAR         : return 24;
    case WeaponId::FAMAS           : return 25;
    case WeaponId::AK47            : return 26;
    case WeaponId::M4A4            : return 27;
    case WeaponId::M4A1S           : return 28;
    case WeaponId::SSG08           : return 29;
    case WeaponId::SG553           : return 30;
    case WeaponId::AUG             : return 31;
    case WeaponId::AWP             : return 32;
    case WeaponId::G3SG1           : return 33;
    case WeaponId::SCAR20          : return 34;

    case WeaponId::Zeusx27               : return 39;
    case WeaponId::Flashbang             : return 41;
    case WeaponId::DecoyGrenade          : return 42;
    case WeaponId::HighExplosiveGrenade  : return 43;
    case WeaponId::IncendiaryGrenade     : return 44;
    case WeaponId::Molotov               : return 45;
    case WeaponId::SmokeGrenade          : return 46;
    }
}
constexpr int GetWeaponClass(WeaponId weaponId) noexcept
{
    switch (weaponId) {
    default: return 0;
        
    case WeaponId::Glock18         : 
    case WeaponId::P2000           : 
    case WeaponId::USPS            : 
    case WeaponId::DualBerettas    : 
    case WeaponId::P250            : 
    case WeaponId::Tec9            : 
    case WeaponId::FiveSeveN       : 
    case WeaponId::CZ75Auto        : 
    case WeaponId::DesertEagle     : 
    case WeaponId::R8Revolver      : return 35;
        
    case WeaponId::Nova            : 
    case WeaponId::XM1014          : 
    case WeaponId::SawedOff        : 
    case WeaponId::MAG7            : 
    case WeaponId::M249            : 
    case WeaponId::Negev           : return 36;
        
    case WeaponId::MAC10           : 
    case WeaponId::MP9             : 
    case WeaponId::MP7             : 
    case WeaponId::MP5SD           : 
    case WeaponId::UMP45           : 
    case WeaponId::P90             : 
    case WeaponId::PPBizon         : return 37;
        
    case WeaponId::GalilAR         : 
    case WeaponId::FAMAS           : 
    case WeaponId::AK47            : 
    case WeaponId::M4A4            : 
    case WeaponId::M4A1S           : 
    case WeaponId::SSG08           : 
    case WeaponId::SG553           : 
    case WeaponId::AUG             : 
    case WeaponId::AWP             : 
    case WeaponId::G3SG1           : 
    case WeaponId::SCAR20          : return 38;
        
    case WeaponId::Flashbang             : 
    case WeaponId::DecoyGrenade          : 
    case WeaponId::HighExplosiveGrenade  : 
    case WeaponId::IncendiaryGrenade     : 
    case WeaponId::Molotov               : 
    case WeaponId::SmokeGrenade          : return 40;
    }
}