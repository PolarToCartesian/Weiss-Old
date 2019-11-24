#include "Timer.h"

Timer::Timer()
{
	this->m_start = std::chrono::system_clock::now();
}

float Timer::getElapsedTimeMs()
{
	const auto end = std::chrono::system_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

	return static_cast<float>(elapsed.count() * 0.001f);
}