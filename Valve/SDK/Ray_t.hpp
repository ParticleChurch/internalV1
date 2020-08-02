#pragma once
struct Ray_t {
    Vec start{ };
    float pad{ };
    Vec delta{ };
    std::byte pad2[40]{ };
    bool isRay{ true };
    bool isSwept{ };
    Ray_t(const Vec& src, const Vec& dest) {
        start = src;
        delta = Vec(src) - Vec(dest);
        isSwept = delta.x || delta.y || delta.z;
    }
};