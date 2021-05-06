#pragma once

template <typename T>
class UtlVector {
public:
    constexpr T& operator[](int i) noexcept { return memory[i]; };
    T* Base()
    {
        return memory;
    }
    int Count() const
    {
        return size; // might also be growsize well see
    }
    T* memory;
    int allocationCount;
    int growSize;
    int size;
    T* elements;
};