#pragma once

#include <string>
#include <Windows.h>

#define H_ERROR(hr) {\
	if (hr != S_OK)\
		MessageBox(NULL, std::to_string(__LINE__).c_str(), std::string(__FILE__).c_str(), MB_ABORTRETRYIGNORE);\
}