#pragma once
class PhysicsSurfaceProps {
public:
    SurfaceData* getSurfaceData(int index) {
        typedef SurfaceData* (__thiscall* ogetSurfaceData)(void*, int);
        return GetVFunc<ogetSurfaceData>(this, 5)(this, index);
    }
};