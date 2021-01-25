#pragma once
class Vec;

class Matrix3x4 {
public:
    float c[3][4];
    constexpr auto operator[](int i) const noexcept { return c[i]; }
    std::string str()
    {
        std::string ln1 = "[" + std::to_string(c[0][0]) + "] [" + std::to_string(c[0][1]) + "] [" + std::to_string(c[0][2]) + "] [" + std::to_string(c[0][3]) + "]";
        std::string ln2 = "[" + std::to_string(c[1][0]) + "] [" + std::to_string(c[1][1]) + "] [" + std::to_string(c[1][2]) + "] [" + std::to_string(c[1][3]) + "]";
        std::string ln3 = "[" + std::to_string(c[2][0]) + "] [" + std::to_string(c[2][1]) + "] [" + std::to_string(c[2][2]) + "] [" + std::to_string(c[2][3]) + "]";
        return ln1 + "\n" + ln2 + "\n" + ln3;
    }
};