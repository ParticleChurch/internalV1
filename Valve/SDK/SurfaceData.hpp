#pragma once
struct SurfaceData {
    std::byte _pad_[80];
    float MaxSpeedFactor;
    float JumpFactor;
    float PenetrationModifier;
    float DamageModifier;
    short Material;
    bool Climbable;
};