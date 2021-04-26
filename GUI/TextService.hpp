#pragma once
#include <string>
#include <algorithm>
#include <array>
#include <assert.h>
#include <vector>
#include "../Logger.hpp"

namespace TextService
{
	extern std::string ToLowercase(std::string Input);
	extern std::string RemoveWhitespace(std::string Input);

	extern bool Contains(std::string needle, std::string haystack, bool CaseSensitive = true);
	extern int CountOccurances(std::string needle, std::string haystack);
}

/*
	COMPILE-TIME ENCRYPTION
*/
namespace TextService
{
	namespace StringEncoding {
		struct string
		{
			char* str;

			string(size_t size, size_t tagSize, const volatile char* original) noexcept
			{
				this->str = new char[size];
				memcpy(this->str, (const char*)original + tagSize, size);

				if (original[0] == '^') // has it been encrypted?
				{
					uint32_t key = *(volatile uint32_t*)(original + 1);

					size_t offset = 0;
					for (; size >= 4 && offset <= size - 4; offset += 4)
						*(uint32_t*)(this->str + offset) ^= key;
					for (; offset < size; offset++)
						this->str[offset] ^= key & 0xff;
				}
			}
			~string()
			{
				delete[] str;
			}
		};
	};
}
#define XORTAG(str) (TextService::StringEncoding::string(sizeof(str), 5, "!XOR!" str))
#define XOR(string_literal) ([]{static auto s = XORTAG(string_literal); return s.str;}())

#define XORTAG_S(size, str) (TextService::StringEncoding::string(sizeof(str), 5 + sizeof(#size) - 1, "!XOR" #size "!" str))
#define XOR_S(string_size, string_literal) ([]{static auto s = XORTAG_S(string_size, string_literal); return s.str;}())