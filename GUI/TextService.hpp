#pragma once
#include <string>
#include <algorithm>

namespace TextService
{
	extern std::string ToLowercase(std::string Input);

	extern bool Contains(std::string needle, std::string haystack, bool CaseSensitive = true);
	extern int CountOccurances(std::string needle, std::string haystack);
}