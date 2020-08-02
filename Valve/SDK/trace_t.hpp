#pragma once
struct trace_t {
    Vec Startpos;
    Vec Endpos;
    std::byte pad[20];
    float Fraction;
    int Contents;
    unsigned short DispFlags;
    bool AllSolid;
    bool StartSolid;
    std::byte pad1[4];
    struct Surface {
        const char* Name;
        short SurfaceProps;
        unsigned short Flags;
    } Surface;
    int Hitgroup;
    std::byte pad2[4];
    Entity* entity;
    int hitbox;
};