#include "TextService.hpp"

std::string TextService::ToLowercase(std::string Input)
{
	std::transform(Input.begin(), Input.end(), Input.begin(), [](unsigned char c) { return std::tolower(c); });
	return Input;
}

std::string TextService::RemoveWhitespace(std::string Input)
{
	Input.erase(std::remove_if(Input.begin(), Input.end(), ::isspace), Input.end());
	return Input;
}

bool TextService::Contains(std::string needle, std::string haystack, bool CaseSensitive)
{
	if (needle.size() > haystack.size()) return false;
	if (!CaseSensitive)
	{
		needle = ToLowercase(needle);
		haystack = ToLowercase(haystack);
	}

	for (size_t i = 0; i <= haystack.size() - needle.size(); i++)
		if (haystack.substr(i, needle.size()) == needle)
			return true;
	
	return false;
}

int TextService::CountOccurances(std::string needle, std::string haystack)
{
	if (needle.size() > haystack.size()) return 0;

	int count = 0;
	for (size_t i = 0; i <= haystack.size() - needle.size(); i++)
		if (haystack.substr(i, needle.size()) == needle)
			count++;

	return count;
}