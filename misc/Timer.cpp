#include "Timer.h"

Timer::Timer()
{
	this->m_start = std::chrono::high_resolution_clock::now();
}

float Timer::GetElapsedTimeMs()
{
	const auto end = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

	return static_cast<float>(elapsed.count() * 0.001f);
}