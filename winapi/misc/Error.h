#pragma once

#include "../misc/includes.h"

#include <string>
#include <iostream>

#define MESSAGE_BOX_ERROR(errorMsg) {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
}

#define ASSERT_ERROR(v, errorMsg) {\
	if (!(v))\
		MESSAGE_BOX_ERROR(errorMsg)\
}

#define HRESULT_ERROR(hr, errorMsg) {\
	if (hr != S_OK)\
		MESSAGE_BOX_ERROR(errorMsg)\
}