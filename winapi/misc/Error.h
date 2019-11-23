#pragma once

#include "../misc/includes.h"

#include <string>
#include <iostream>

#define MESSAGE_BOX_ERROR() {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__)).c_str(), "Window / DirectX Error!", MB_ABORTRETRYIGNORE);\
}

#define ASSERT_ERROR(v) {\
	if (!(v))\
		MESSAGE_BOX_ERROR()\
}

#define HRESULT_ERROR(hr) {\
	if (hr != S_OK)\
		MESSAGE_BOX_ERROR()\
}