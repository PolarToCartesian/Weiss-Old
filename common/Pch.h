#pragma once

#ifdef _WIN32

	#define __WEISS__OS_WINDOWS

	#ifdef _WIN64
		#define __WEISS__PLATFORM_X64
	#else // End Of #ifdef _WIN64
		#define __WEISS__PLATFORM_X86
	#endif

	#define _WINSOCKAPI_ // Stops Windows.h from including winsock2
	#define _WINSOCK_DEPRECATED_NO_WARNINGS // Lets us use htons()
	#define NOMINMAX // Stops The Importing Of min() And max() From Windef.h

	// Windows Includes
	#include <wrl.h>
	#include <strsafe.h>
	#include <Windows.h>
    #include <windowsx.h>
	
	// Windows Networking Includes
	#include <winhttp.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

	// Windows Image Includes
	#include <wincodec.h>

	// Windows DirectX 11 Includes
	#include <d3d11.h>
	#include <d3dcompiler.h>

	// Vulkan
	#define VK_USE_PLATFORM_WIN32_KHR

	// Linking
	#pragma comment (lib, "winmm.lib")
	#pragma comment (lib, "d3d11.lib")
	#pragma comment (lib, "user32.lib")
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "winhttp.lib")
	#pragma comment (lib, "kernel32.lib")
	#pragma comment (lib, "AdvApi32.lib")
	#pragma comment (lib, "D3DCompiler.lib")
	#pragma comment (lib, "windowscodecs.lib")

	#undef _WINSOCKAPI_
	#undef _WINSOCK_DEPRECATED_NO_WARNINGS
	#undef NOMINMAX

	// Windows Macros
	#define MESSAGE_BOX_ERROR(errorMsg) {\
		MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
	}

	#define ENABLE_CONSOLE() {\
		AllocConsole();\
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);\
	}

#elif defined __APPLE__

	#include <TargetConditionals.h>

	#ifdef TARGET_OS_OSX
	
		#define __WEISS__OS_OSX

	#else
		#error The Weiss Engine Is Not Supported On Your Platform
	#endif

	//#if TARGET_IPHONE_SIMULATOR
	//#elif TARGET_OS_IPHONE
	//#elif TARGET_OS_MAC

#elif defined __linux__
	
	#define __WEISS__OS_LINUX

	#ifdef __ANDROID__
		#define __WEISS__OS_ANDROID
	#endif

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h> 

#else
	#error The Weiss Engine Is Not Supported On Your Platform
#endif

// Vulkan Includes
#include <vulkan/vulkan.h>

#pragma comment(lib, "vulkan-1.lib")

// Standard C++17 Library Includes
#include <list>
#include <array>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <exception>
#include <algorithm>
#include <functional>