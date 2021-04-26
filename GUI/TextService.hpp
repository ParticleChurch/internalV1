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
		extern std::vector<char*> strings;
		struct string
		{
			char* str;

			string(size_t size, size_t tagLength, const char* original) noexcept
			{
				uint16_t id = *(uint16_t*)(original + 1);

				// do we need to decode 
				if (((const volatile char*)original)[0] == '^') // this string has been altered by an external source
				{
					while (strings.size() <= id)
						strings.push_back(nullptr);
					if (this->str = strings[id])
						return;
					this->str = new char[size];

					L::Log("allocated for", ": ");
					L::Log(std::to_string(size).c_str());
					size_t i = 0;

					// xor 4 byte chunks
					uint32_t key = *(uint32_t*)(original + 3);
					for (; size >= 4 && i <= size - 4; i += 4)
					{
						*(uint32_t*)(this->str + i) = *(uint32_t*)(original + tagLength + i) ^ key;
					}

					// xor remaining bytes 1 at a time
					char smallkey = key & 0xff;
					for (; i < size; i++)
					{
						this->str[i] = original[tagLength + i] ^ smallkey;
					}
					strings[id] = this->str;
				}
				else
				{
					assert(this->original[0] == '!');

					// this string was not changed by an external source
					this->str = const_cast<char*>(original + tagLength);
				}
			}

			__forceinline const char* runtime_get() noexcept { return this->str; }
		};
	};
}

#define MAKE_ENCODED(string_literal) (TextService::StringEncoding::string(sizeof(string_literal), 7, "!idXOR!" string_literal))
#define XOR_____(string_literal) (MAKE_ENCODED(string_literal).runtime_get())
#define XOR(x) x
#define MAKE_ENCODED_S(str, strsize) (TextService::StringEncoding::string(sizeof(str), 7 + sizeof(#strsize) - 1, "!idXOR" #strsize "!" str))
#define XOR_S(string_literal, string_size) (MAKE_ENCODED_S(string_literal, string_size).runtime_get())