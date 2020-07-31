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
    int maxClip;
    std::byte pad1[112];
    char* name;
    std::byte pad1_[60];
    WeaponType type;
    std::byte pad2[4];
    int price;
    std::byte pad9_[0x8];
    float cycletime;
    std::byte pad2_[12];
    bool fullAuto;
    std::byte pad3[3];
    int damage;
    float armorRatio;
    int bullets;
    float penetration;
    std::byte pad4[8];
    float range;
    float rangeModifier;
    std::byte pad5[0x10];
    bool silencer;
    std::byte pad6[15];
    float maxSpeed;
    float maxSpeedAlt;
    std::byte pad7[100];
    float recoilMagnitude;
    float recoilMagnitudeAlt;
    std::byte pad8[0x14];
    float recoveryTimeStand;
};