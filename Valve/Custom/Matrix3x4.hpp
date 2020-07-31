#pragma once
class Matrix3x4 {
public:
    float c[3][4];
    constexpr auto operator[](int i) const noexcept { return c[i]; }
};