#pragma once

#include <chrono>

class Timer
{
	private:
		std::chrono::system_clock::time_point m_start;

	public:
		Timer();

		float getElapsedTimeMs();
};