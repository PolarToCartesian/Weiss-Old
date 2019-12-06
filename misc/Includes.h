#pragma once

#include <array>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <algorithm>
#include <functional>

#include <wrl.h>
#include <d3d11.h>
#include <strsafe.h>
#include <Windows.h>
#include <windowsx.h>
//#include <winhttp.h>
#include <wincodec.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "windowscodecs.lib")

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