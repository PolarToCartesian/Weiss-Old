#pragma once

#include "Includes.h"

class Timer
{
	private:
		std::chrono::system_clock::time_point m_start;

	public:
		Timer();

		float getElapsedTimeMs();
};