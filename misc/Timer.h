#pragma once

#include "../misc/Pch.h"

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point m_start;

public:
	Timer();

	[[nodiscard]] float GetElapsedTimeMs();
};