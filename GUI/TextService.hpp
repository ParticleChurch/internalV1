#pragma once
#include <string>
#include <algorithm>
#include <array>

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
#ifdef _MSC_VER
#define INLINE __forceinline
#else
#define INLINE inline
#endif

namespace TextService
{
	namespace StringEncoding
	{
		// get the timestamp (hh:mm:ss) at the time of compilation
		constexpr unsigned char compile_time_hh = 10 * (__TIME__[0] - 48) + (__TIME__[1] - 48); // [0, 23]
		constexpr unsigned char compile_time_mm = 10 * (__TIME__[3] - 48) + (__TIME__[4] - 48); // [0, 59]
		constexpr unsigned char compile_time_ss = 10 * (__TIME__[6] - 48) + (__TIME__[7] - 48); // [0, 59]

		// generate a "random" number at compile time
		// although not random, its purpose is to be a different number everytime you compile
		// since strings will be XOR'd with this value, it will be random enough
		constexpr unsigned char xor_seed = 3 * compile_time_ss + compile_time_mm + compile_time_hh / 2; // 4 + [0, 3 * 59 + 59 + 11] = [4, 251]

		template <size_t N, int K>
		struct string
		{
			unsigned char seed;
			std::array<char, N + 1> data;

			constexpr char xor_char(char c) const
			{
				return c ^ seed;
			}

			// compile time
			template <size_t... indicies>
			constexpr INLINE string(const char* str, std::index_sequence<indicies...>) : seed((unsigned char)((K + xor_seed) % 256)), data{ { xor_char(str[indicies])... } } {}

			// runtime
			INLINE char* runtime_xor()
			{
				for (size_t i = 0; i < N; ++i)
				{
					data[i] = xor_char(data[i]);
				}
				data[N] = '\0';
				return data.data();
			}
			INLINE char* runtime_get()
			{
				return data.data();
			}
		};
	}
}

#define MAKE_ENCODED_STRING(s) (TextService::StringEncoding::string<sizeof(s) - 1, __COUNTER__>((s), std::make_index_sequence<sizeof(s) - 1>()))
#define ENCODED(s) (MAKE_ENCODED_STRING(s).runtime_get())
//#define XOR(s) (MAKE_ENCODED_STRING(s).runtime_xor())
#define XOR(s) s

#undef INLINE