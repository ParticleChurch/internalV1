#pragma once
static DWORD FindPattern(std::string module, std::string pattern) {

	//getting the given module
	HMODULE HModule = GetModuleHandleA(module.c_str());
	if (HModule == NULL) //if unable to get the module, return 0
	{
		return 0;
	}

	//getting information about given module
	MODULEINFO ModuleInfo;
	BOOL Success = GetModuleInformation(GetCurrentProcess(), HModule, &ModuleInfo, sizeof(ModuleInfo));
	if (!Success)	//if unable to get info about the module, return 0
	{
		return 0;
	}

	char* Data = (char*)HModule;						//given point data at a point
	unsigned int Size = ModuleInfo.SizeOfImage;			//total amount of data
	unsigned int i = 0;									//iterator (location) to go through data/pattern
	unsigned int IndexPattern;							//iterator (location) to go through the pattern
	unsigned int NumPattern;							//iterator (location) to go through data
	unsigned int PatternFound;							//boolean of if the pattern is found
	DWORD FirstOccurence = 0;							//the first occurance of the epattern
	char byte;											//byte to set data to for comparison against pattern

	while (i < Size - pattern.size()) {					//while its still possible for there to be a pattern
		PatternFound = false;
		IndexPattern = 0;
		NumPattern = 0;

		while (IndexPattern < pattern.size()) {			//go through data a patterns length
			if (pattern[IndexPattern] == ' ') {			//if ' ' ignore
				IndexPattern++;
				continue;
			}
			if (pattern[IndexPattern] == '?') {			//if '?' (WILDCARD)
				IndexPattern++;
				i++;
				continue;
			}

			byte = GetByte((&pattern[IndexPattern]));	//get a given byte

			if (Data[i + NumPattern] != byte) {			//if the byte at the point isn't equal to the pattern
				break;
			}
			else {										
				if (NumPattern == 0) {					//if pattern matches memory...
					FirstOccurence = (DWORD)Data + i;
				}
			}

			IndexPattern += 2;
			NumPattern++;

			if (IndexPattern >= pattern.size()) {		//if pattern matches memory...
				PatternFound = true;
				break;
			}
		}

		if (PatternFound) {								//return the location
			return FirstOccurence;
		}

		i++;
	}

	return 0;
}