#pragma once
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