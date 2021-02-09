#pragma once
#include <string>
#include <algorithm>

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
	// get the timestamp (hh:mm:ss) at the time of compilation
	constexpr unsigned char compile_time_hh = 10 * (__TIME__[0] - 48) + (__TIME__[1] - 48); // [0, 23]
	constexpr unsigned char compile_time_mm = 10 * (__TIME__[3] - 48) + (__TIME__[4] - 48); // [0, 59]
	constexpr unsigned char compile_time_ss = 10 * (__TIME__[6] - 48) + (__TIME__[7] - 48); // [0, 59]

	// generate a "random" number at compile time
	// although not random, its purpose is to be a different number everytime you compile
	// since strings will be XOR'd with this value, it will be random enough
	constexpr unsigned char xor_seed = 4 + 3 * compile_time_hh + 2 * compile_time_mm + compile_time_ss; // 4 + [0, 3 * 23 + 2 * 59 + 59] = [4, 250]

	// function to encrypt a single character (will execute at compile time!!)
	constexpr char encrypt_char_compiletime(char c, size_t index_in_string)
	{
		return c ^ (char)((index_in_string + xor_seed) % 256);
	}

	// class to contain our encrypted string, and the runtime function to decrypt it
	template <size_t size>
	class EncryptedString
	{
	public:
		const size_t length = size - 1;
		char data[size];

		// if all goes as planned, this will execute at compile time
		inline constexpr EncryptedString(const char* string) : data{}
		{
			for (size_t i = 0; i < size; i++)
				data[i] = encrypt_char_compiletime(string[i], i);
		}

		// XOR at runtime
		// note that the first hash() will decrypt, second will re-encrypt, third decrypts, etc. 
		// nasty const cast, but required to avoid making a copy
		const char* hash() const
		{
			char* string = const_cast<char*>(data);
			for (size_t i = 0; i < length; i++)
				string[i] = data[i] ^ (char)((i + xor_seed) % 256);
			string[length] = '\0';
			return string;
		}

		// for very large strings that do not fit on the stack
		// note: you will have to manually free() these
		const char* heaphash() const
		{
			char* string = (char*)malloc(size);
			for (size_t i = 0; i < length; i++)
				string[i] = data[i] ^ (char)((i + xor_seed) % 256);
			string[length] = '\0';
			return string;
		}
	};
}

// to get the raw, encrypted version of text. Not useful other than for testing
#define TXT_ENCRYPTED(text) []{constexpr TextService::EncryptedString<sizeof(text)> x(text); return x; }().data

// encrypt at compile time, decrypt at runtime
#define TXT(text) []{constexpr TextService::EncryptedString<sizeof(text)> x(text); return x; }().hash()

// same as TXT(), but places result on the heap instead of stack. you will have to manually free() it
#define HEAPTXT(text) []{constexpr TextService::EncryptedString<sizeof(text)> x(text); return x; }().heaphash()