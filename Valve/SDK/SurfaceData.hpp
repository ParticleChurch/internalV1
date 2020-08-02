#pragma once
struct SurfaceData {
    std::byte _pad_[80];
    float maxspeedfactor;
    float jumpfactor;
    float penetrationmodifier;
    float damagemodifier;
    short material;
    bool climbable;
};