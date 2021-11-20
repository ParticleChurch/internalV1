#pragma once
#include <chrono>

inline float getCurrentTimestamp()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000000.f;
}