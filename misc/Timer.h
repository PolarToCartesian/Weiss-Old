#pragma once

#include <chrono>

class Timer
{
	private:
		std::chrono::system_clock::time_point m_start;

	public:
		Timer()
		{
			this->m_start = std::chrono::system_clock::now();
		}
		uint32_t getElapsedTime()
		{
			const auto end     = std::chrono::system_clock::now();
			const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->m_start);

			return elapsed.count();
		}
};