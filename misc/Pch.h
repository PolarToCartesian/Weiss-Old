#pragma once

#define _WINSOCKAPI_ // Stops Windows.h from including winsock2
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Lets us use htons()
#define NOMINMAX // Stops The Importing Of min() And max() From Windef.h

#include <wrl.h>
#include <array>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <d3d11.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <exception>
#include <strsafe.h>
#include <Windows.h>
#include <algorithm>
#include <winhttp.h>
#include <functional>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wincodec.h>
#include <windowsx.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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