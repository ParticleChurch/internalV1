#pragma once
enum class WeaponType {
    Knife = 0,
    Pistol,
    SubMachinegun,
    Rifle,
    Shotgun,
    SniperRifle,
    Machinegun,
    C4,
    Placeholder,
    Grenade,
    Unknown
};

struct WeaponData {
    std::byte pad[20];
    int MaxClip;
    std::byte pad1[112];
    char* Name;
    std::byte pad1_[60];
    WeaponType Type;
    std::byte pad2[4];
    int Price;
    std::byte pad9_[0x8];
    float Cycletime;
    std::byte pad2_[12];
    bool FullAuto;
    std::byte pad3[3];
    int Damage;
    float ArmorRatio;
    int Bullets;
    float Penetration;
    std::byte pad4[8];
    float Range;
    float RangeModifier;
    std::byte pad5[0x10];
    bool Silencer;
    std::byte pad6[15];
    float MaxSpeed;
    float MaxSpeedAlt;
    std::byte pad7[100];
    float RecoilMagnitude;
    float RecoilMagnitudeAlt;
    std::byte pad8[0x14];
    float RecoveryTimeStand;
};