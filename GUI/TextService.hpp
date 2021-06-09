#pragma once
#include <string>
#include <algorithm>
#include <array>
#include <assert.h>
#include <vector>
#include "../Logger.hpp"

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

/*
	STRING ENCODING!
*/
#define XOR_TAG_TEXT "!XOR!"
#define XOR_TAG_LENGTH (sizeof(XOR_TAG_TEXT) - 1) /* null not included */
#define XOR_TAG(str) (XOR_TAG_TEXT str)
#define XOR_STDSTR(str) (TextService::StringEncoding::runtimeDecrypt(sizeof(str), XOR_TAG(str)))
#define XOR(str) (XOR_STDSTR(str).c_str())

namespace TextService
{
	namespace StringEncoding {
		inline std::string runtimeDecrypt(const size_t size /* "!XOR!Hello" = 6 */, const volatile char* str)
		{
			if (*str == '^')
			{
				// this string has been encrypted, it must be decrypted
				uint32_t key = *(volatile uint32_t*)(str + 1);
				const char* keys = (char*)(&key);

				std::string workspace((char*)(str + XOR_TAG_LENGTH), size);

				for (size_t o = 0; o < size; o++)
				{
					char key = keys[o % 4];
					workspace[o] ^= key;
				}

				return workspace;
			}
			else if (*str != '!')
			{
				std::exit(0);
				return "\x69\x42\x0";
			}

			return (const char*)(str + XOR_TAG_LENGTH);
		}
	}
}