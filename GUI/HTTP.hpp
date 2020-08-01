#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Config.hpp"
#pragma comment(lib, "wininet.lib")

namespace HTTP {
    extern std::string URLEncode(const std::string& value);

    struct Arguments {
        unsigned int Count = 0;
        std::string String = "";

        void Add(std::string Key, std::string Value)
        {
            if (this->Count > 0)
                this->String += "&";
            this->String += URLEncode(Key) + "=" + URLEncode(Value);

            this->Count++;
        }
    };

    template <class T>
    extern bool StructuredGET(std::string Host, std::string Directory, std::string URLArguments, T* output);
    template <class T>
    extern bool APIGET(std::string Cmd, Arguments Data, T* Output);

    extern APIResponseFormat_LoginAttempt APILogin(std::string Email, std::string Password);
}
