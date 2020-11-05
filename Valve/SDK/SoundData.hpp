#pragma once
struct SoundData {
	std::byte pad[4];
	int entityIndex;
	int channel;
	const char* soundEntry;
	std::byte pad1[8];
	float volume;
	std::byte pad2[44];
};