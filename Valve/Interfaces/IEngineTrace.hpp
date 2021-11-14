#pragma once
class IEngineTrace {
public:
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(int, GetPointContents, 0, (const Vec& vecAbsPosition, int contentsMask = MASK_ALL), (const Vec&, int, Entity**), (vecAbsPosition, contentsMask, nullptr));

    DECLARE_VIRTUAL_METHOD(void, ClipRayToEntity, 3, (const Ray_t& ray, unsigned int mask, Entity* pEnt, trace_t* pTrace), (ray, mask, pEnt, pTrace));
    DECLARE_VIRTUAL_METHOD(void, TraceRay, 5, (const Ray_t& ray, unsigned int mask, CTraceFilter* pTraceFilter, trace_t* pTrace), (ray, mask, pTraceFilter, pTrace));
};