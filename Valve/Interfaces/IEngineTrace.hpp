#pragma once
static float GetDamageMultiplier(int HitGroup) 
{
    switch (HitGroup) {
    case HITGROUP_HEAD:
        return 4.0f;
    case HITGROUP_STOMACH:
        return 1.25f;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        return 0.75f;
    default:
        return 1.0f;
    }
}

static bool IsArmored(int HitGroup, bool Helmet)
{
    switch (HitGroup) {
    case HITGROUP_HEAD:
        return Helmet;

    case HITGROUP_CHEST:
    case HITGROUP_STOMACH:
    case HITGROUP_LEFTARM:
    case HITGROUP_RIGHTARM:
        return true;
    default:
        return false;
    }
}

class IEngineTrace {
public:
    int GetPointContents(const Vec& vecAbsPosition, int contentsMask = MASK_ALL) {
        typedef int(__thiscall* oGetPointContents)(void*, const Vec&, int, Entity**);
        return GetVFunc<oGetPointContents>(this, 0)(this, vecAbsPosition, contentsMask, nullptr);
    }

    void TraceRay(const Ray_t& ray, unsigned int mask, CTraceFilter* pTraceFilter, trace_t* pTrace) {
        typedef void(__thiscall* oTraceRay)(void*, const Ray_t&, unsigned int, CTraceFilter*, trace_t*);
        return GetVFunc<oTraceRay>(this, 5)(this, ray, mask, pTraceFilter, pTrace);
    }
};