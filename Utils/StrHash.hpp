#pragma once
namespace StrHash {
    constexpr uint32_t offsetBasis = 0x811c9dc5;
    constexpr uint32_t prime = 0x1000193;

    constexpr uint32_t Hash(const char* str, const uint32_t value = offsetBasis) noexcept
    {
        return *str ? Hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
    }

    constexpr uint32_t HashRuntime(const char* str) noexcept
    {
        auto value = offsetBasis;

        while (*str) {
            value ^= *str++;
            value *= prime;
        }
        return value;
    }

    constexpr uint32_t HashRuntime(const char* str, const size_t length) noexcept
    {
        auto value = offsetBasis;

        for (size_t i = 0; i < length; i++)
        {
            value ^= *str++;
            value *= prime;
        }
        return value;
    }
}