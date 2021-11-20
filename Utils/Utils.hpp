#pragma once
#include "Macro.hpp" //for abusive macro calls (mainly having to do with math)
#include "PatternScan.hpp" //for scanning in memory for specific functions
#include "VMT.hpp" //useful for managing Virtual Method calls and hooks
#include "StrHash.hpp" //for hashing str to ints (ex fireeventclientside in hooks.cpp)
#include "time.hpp" // avoid having to use the <chrono> library