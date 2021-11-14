#pragma once

class InputSystem {
public:
    DECLARE_VIRTUAL_METHOD(void, EnableInput, 11, (bool enable), (enable));
    DECLARE_VIRTUAL_METHOD(bool, IsButtonDown, 15, (int buttonCode), (buttonCode));
    DECLARE_VIRTUAL_METHOD(void, ResetInputState, 39, (), ());
    DECLARE_VIRTUAL_METHOD(const char*, ButtonCodeToString, 40, (int buttonCode), (buttonCode));

    // bro wtf is this lmao
    // looks like just a vfunc call...?
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