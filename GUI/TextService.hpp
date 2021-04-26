#pragma once
#include <string>
#include <algorithm>
#include <array>
#include <assert.h>

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
			const char* original;
			char* str;
			size_t size;

			string(size_t size, size_t tagLength, const char* src) noexcept
			{
				this->original = src;
				this->str = new char[size];
				this->size = size;
				if (((const volatile char*)this->original)[0] == '^') // this string has been altered by an external source
				{
					size_t i = 0;

					// xor 4 byte chunks
					uint32_t key = *(uint32_t*)(this->original + 1);
					for (; size >= 4 && i <= size - 4; i += 4)
					{
						*(uint32_t*)(this->str + i) = *(uint32_t*)(this->original + tagLength + i) ^ key;
					}

					// xor remaining bytes 1 at a time
					char smallkey = key & 0xff;
					for (; i < size; i++)
					{
						this->str[i] = this->original[tagLength + i] ^ smallkey;
					}
				}
				else
				{
					assert(this->original[0] == '!');

					// this string was not changed by an external source
					// just copy without XOR-ing
					for (size_t i = 0; i < size; i++)
						this->str[i] = this->original[tagLength + i];
				}
			}
			~string() { delete[] this->str; }

			__forceinline const char* runtime_get() noexcept { return str; }
		};
	};
}

#define MAKE_ENCODED(string_literal) (TextService::StringEncoding::string(sizeof(string_literal), 5, "!XOR!" string_literal))
#define XOR(string_literal) (MAKE_ENCODED(string_literal).runtime_get())

#define MAKE_ENCODED_S(str, strsize) (TextService::StringEncoding::string(sizeof(str), 5 + sizeof(#strsize) - 1, "!XOR" #strsize "!" str))
#define XOR_S(string_literal, string_size) (MAKE_ENCODED_S(string_literal, string_size).runtime_get())