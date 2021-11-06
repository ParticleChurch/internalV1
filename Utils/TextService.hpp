#pragma once
#include <string>
#include <algorithm>
#include <array>
#include <assert.h>
#include <vector>
#include "../Logger.hpp"
#include "StringEncryption.hpp"

#define IS_UPPER(c) ('A' <= (c) && (c) <= 'Z')
#define IS_LOWER(c) ('a' <= (c) && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define MAKE_LOWER(c) (IS_UPPER(c) ? (c) - ('A' - 'a') : (c))

namespace TextService
{
	extern std::string ToLowercase(std::string Input);
	extern std::string RemoveWhitespace(std::string Input);

	extern bool Contains(std::string needle, std::string haystack, bool CaseSensitive = true);
	extern int CountOccurances(std::string needle, std::string haystack);
	extern std::string lpad(std::string in, size_t length, const char padWith = ' ');
	extern std::string rpad(std::string in, size_t length, const char padWith = ' ');
	extern std::string cpad(std::string in, size_t length, const char padWith = ' ');
}