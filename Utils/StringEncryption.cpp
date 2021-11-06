#include "StringEncryption.hpp"

std::map<char*, std::unique_ptr<char>> StringEncryption::trackedStrings;
