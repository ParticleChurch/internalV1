#pragma once

class InputSystem {
public:
    void EnableInput(bool enable) noexcept
    {
        typedef void(__thiscall* oenableInput)(void*, bool);
        return GetVFunc<oenableInput>(this, 11)(this, enable);
    }

    bool IsButtonDown(int buttonCode) noexcept
    {
        typedef bool(__thiscall* oisButtonDown)(void*, int);
        return GetVFunc<oisButtonDown>(this, 15)(this, buttonCode);
    }

    void ResetInputState() noexcept
    {
        typedef void(__thiscall* oresetInputState)(void*);
        return GetVFunc<oresetInputState>(this, 39)(this);
    }

    const char* ButtonCodeToString(int buttonCode) noexcept
    {
        typedef const char* (__thiscall* obuttonCodeToString)(void*, int);
        return GetVFunc<obuttonCodeToString>(this, 40)(this, buttonCode);
    }

    template <typename T, std::size_t Idx, typename ...Args>
    constexpr auto call(void* classBase, Args... args) noexcept
    {
        return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[Idx])(classBase, args...);
    }

    auto VirtualKeyToButtonCode(int virtualKey) noexcept
    {
        if (virtualKey <= VK_XBUTTON2) {
            if (virtualKey > VK_CANCEL) virtualKey--;
            return virtualKey + 106;
        }

        return call<int, 45>(this, virtualKey);
    }

    auto VirtualKeyToString(int virtualKey) noexcept
    {
        return ButtonCodeToString(VirtualKeyToButtonCode(virtualKey));
    }
};