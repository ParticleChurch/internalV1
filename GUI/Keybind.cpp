#include "Keybind.hpp"
#include "../Logger.hpp"


namespace Keybind
{
    int KeyMap[nKeys] = {
        // Mouse buttons
        VK_LBUTTON,
        VK_RBUTTON,
        VK_MBUTTON,
        VK_XBUTTON1,
        VK_XBUTTON2,

        // Alphabet
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

        // Homerow numbers
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

        // Arrow keys
        VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,

        // numpad numbers
        VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,

        // function keys
        VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
        VK_F11, VK_F12, VK_F13, VK_F14, VK_F15, VK_F16, VK_F17, VK_F18, VK_F19, VK_F20,
        VK_F21, VK_F22, VK_F23, VK_F24,

        // "modifiers" (ctrl, alt, shift, etc.)
        VK_LMENU, VK_RMENU, // alt
        VK_LSHIFT, VK_RSHIFT, // shift
        VK_LCONTROL, VK_RCONTROL, // ctrl
        VK_LWIN, VK_RWIN, // windows
        VK_CAPITAL, // caps lock
        
        // other misc keys
        VK_TAB, VK_ESCAPE, VK_INSERT,
    };
    std::string KeyNames[nKeys] = {
        // Mouse buttons
        "mouse1",
        "mouse2",
        "mouse3",
        "mouse4",
        "mouse5",

        // Alphabet
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

        // Homerow numbers
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",

        // Arrow keys
        "left", "up", "right", "down",

        // numpad numbers
        "numpad0", "numpad1", "numpad2", "numpad3", "numpad4", "numpad5", "numpad6", "numpad7", "numpad8", "numpad9",

        // function keys
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
        "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",
        "F21", "F22", "F23", "F24",

        // "modifiers" (ctrl, alt, shift, etc.)
        "left alt", "right alt", // alt
        "left shift", "right shift", // shift
        "left ctrl", "right ctrl", // ctrl
        "left win", "right win", // windows
        "capslock", // caps lock

        // other misc keys
        "tab", "esc", "ins",
    };

    bool KeyState[nKeys] = {};
    std::vector<void*> Binds[nKeys] = {};
}

int Keybind::ReverseKeyMap(int KeyCode)
{
    for (int i = 0; i < nKeys; i++)
        if (KeyMap[i] == KeyCode)
            return i;
    return -1;
}