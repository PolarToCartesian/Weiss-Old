#pragma once

#include <string>
#include <Windows.h>
#include <iostream>

#define H_ERROR(hr) {\
	if (hr != S_OK)\
		MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__)).c_str(), "Window / DirectX Error!", MB_ABORTRETRYIGNORE);\
}