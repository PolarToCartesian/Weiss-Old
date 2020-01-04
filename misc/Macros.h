#pragma once

#define CHECK_BIT(var,pos) ((var) & ( 1 << (pos)))

#define ENABLE_CONSOLE() {\
	AllocConsole();\
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);\
}

#ifdef __WEISS_SHOW_DEBUG_ERRORS
#define MESSAGE_BOX_ERROR(errorMsg) {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
}
#endif // __WEISS_SHOW_DEBUG_ERRORS