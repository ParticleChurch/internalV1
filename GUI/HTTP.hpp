#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
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

    extern std::string GET(std::string Host, std::string Directory, std::string URLArguments);
    extern std::string API(std::string CMD, Arguments Arguments);
}
