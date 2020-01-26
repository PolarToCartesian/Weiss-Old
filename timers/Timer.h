#pragma once

#include "../misc/Pch.h"

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point m_start;

public:
	Timer() { this->m_start = std::chrono::high_resolution_clock::now(); }

	[[nodiscard]] float GetElapsedTimeMs()
	{
		const auto end = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

		return static_cast<float>(elapsed.count() * 0.001f);
	}
};