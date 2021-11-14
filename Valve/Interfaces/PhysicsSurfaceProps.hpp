#pragma once
class PhysicsSurfaceProps {
public:
    DECLARE_VIRTUAL_METHOD(SurfaceData*, getSurfaceData, 5, (int index), (index));
};