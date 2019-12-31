#ifndef __WEISS__
#define __WEISS__

#ifndef _WIN32
#error Weiss Is Windows Only! (for now)
#endif // _WIN32

// Project      : Weiss Engine
// Author       : Henry LE BERRE (PolarToCartesian)
// Repository   : https://github.com/PolarToCartesian/Weiss
// Requirements : Windows & DirectX 11 SDK & C++ 17
// Description  : Weiss is a 2D and 3D rendering engine for windows built with DirectX in c++.
//                It is intended to be a wrapper around low-level apis to make the developpement of 2D and 3D applications simpler and faster.

// LISCENCE     :

// Copyright (c) 2019 HENRY LE BERRE

// Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*

██          ██    ██████████████      ██████████      ██████████████    ██████████████
██          ██    ██                      ██          ██                ██
██          ██    ██                      ██          ██                ██
██    ██    ██    ██████████████          ██          ██████████████    ██████████████
██    ██    ██    ██                      ██                      ██                ██
██  ██  ██  ██    ██                      ██                      ██                ██
  ██      ██      ██████████████      ██████████      ██████████████    ██████████████
  
██████████████    ██          ██    ██████████████      ██████████      ██          ██    ██████████████
██                ████        ██    ██                      ██          ████        ██    ██
██                ██  ██      ██    ██                      ██          ██  ██      ██    ██
██████████████    ██    ██    ██    ██    ██████            ██          ██    ██    ██    ██████████████
██                ██      ██  ██    ██          ██          ██          ██      ██  ██    ██
██                ██        ████    ██          ██          ██          ██        ████    ██
██████████████    ██          ██    ████████████        ██████████      ██          ██    ██████████████

*/

// Upcoming Features
// --> Error Handling (Exceptions)  : DONE
// --> Server Sockets               : DONE
// --> Better Sound Engine          : TODO
// --> Abstracted 2D Renderer       : TODO
// --> Abstracted 3D Renderer       : TODO
// --> ...
// --> Physics???

// --> INCLUDES START
// --> INCLUDES

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

// --> LIBRARY LINKING START

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

// --> LIBRARY LINKING END
// --> ERROR HANDLING START

#define ENABLE_CONSOLE() {\
	AllocConsole();\
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);\
}

#ifdef __WEISS_SHOW_DEBUG_ERRORS
#define MESSAGE_BOX_ERROR(errorMsg) {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
}
#endif // __WEISS_SHOW_DEBUG_ERRORS

// --> ERROR HANDLING END
// --> WEISS DEFINES START

constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX = 0u;
constexpr const size_t WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE      = 1024u;
constexpr const size_t WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER  = 200u;
constexpr const size_t WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER   = 3u * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;
constexpr const size_t WEISS_NO_RESOURCE_INDEX                      = std::numeric_limits<size_t>::max();

// --> WEISS DEFINES END
// --> CLASS FORWARD DECLARATIONS START

class Engine;

// --> CLASS FORWARD DECLARATIONS END
// --> FUNCTION FORWARD DECLARATIONS START

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

// --> FUNCTION FORWARD DECLARATIONS END
// --> CLASS EXCEPTIONS & ERROR CODES START

enum class ClientSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	CONNECTION_FAILED = 1
};

class ClientSocketCreationException : public std::exception
{
private:
	const ClientSocketCreationExceptionErrorType m_errorType;

public:
	ClientSocketCreationException(const ClientSocketCreationExceptionErrorType errorType) : m_errorType(errorType) { }

	ClientSocketCreationExceptionErrorType getErrorType() const { return this->m_errorType; }
};

class ClientSocketReceiveException : public std::exception { };

class ClientSocketSendException : public std::exception { };

enum class ServerSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	BINDING_FAILED = 1,
	LISTENING_FAILED = 2
};

class ServerSocketCreationException : public std::exception
{
private:
	const ServerSocketCreationExceptionErrorType m_errorType;

public:
	ServerSocketCreationException(const ServerSocketCreationExceptionErrorType errorType) : m_errorType(errorType) { }

	ServerSocketCreationExceptionErrorType getErrorType() const { return this->m_errorType; }
};

class ServerSocketReceiveException     : public std::exception { };
class ServerSocketSendException        : public std::exception { };
class IndexBufferCreationException     : public std::exception { };
class VertexBufferCreationException    : public std::exception { };
class VertexBufferDataSettingException : public std::exception { };
class ConstantBufferCreationException  : public std::exception { };
class PixelShaderCreationException     : public std::exception { };
class VertexShaderCreationException    : public std::exception { };
class IconLoadingException             : public std::exception { };
class ImageLoadingException            : public std::exception { };
class Texture2DCreationException       : public std::exception { };
class EngineInitializationException    : public std::exception { };

enum class WindowCreationExceptionErrorType
{
	CLASS_REGISTRATION_ERROR,
	WINDOW_CREATION_ERROR
};

class WindowCreationException : public std::exception
{
private:
	const WindowCreationExceptionErrorType m_type;

public:
	WindowCreationException(const WindowCreationExceptionErrorType& type) : m_type(type) { }

	WindowCreationExceptionErrorType getErrorType() const { return this->m_type; }
};


// --> CLASS EXCEPTIONS & ERROR CODES END
// --> SOCKETS START
// --> SOCKETS --> CLIENT START

class ClientSocket {
private:
	SOCKET m_socket = INVALID_SOCKET;

public:
	ClientSocket()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
	}

	~ClientSocket() { this->Disconnect(); }

	void Connect(const char* host, const unsigned int port)
	{
		SOCKADDR_IN sockAddr;
		sockAddr.sin_addr.s_addr = inet_addr(host);
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(port);

		this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (this->m_socket == INVALID_SOCKET)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[CLIENT SOCKET] Socket Creation Failed");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ClientSocketCreationException(ClientSocketCreationExceptionErrorType::CREATION_FAILED);
		}

		if (connect(this->m_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[CLIENT SOCKET] Unable To Connect To Server Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ClientSocketCreationException(ClientSocketCreationExceptionErrorType::CONNECTION_FAILED);
		}
	}

	void Send(const char* data, int length = -1)
	{
		if (length < 0)
			length = strlen(data) + 1;

		if (send(this->m_socket, data, length, 0) == SOCKET_ERROR)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[CLIENT SOCKET] Error While Sending Data From Client Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ClientSocketSendException();
		}
	}
	
	[[nodiscard]] std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> Receive()
	{
		std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE> buffer;

		const int iResult = recv(this->m_socket, buffer.data(), WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE, 0);

		if (iResult == 0 || iResult == SOCKET_ERROR) {
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[CLIENT SOCKET] Error While Receiving Data From Server");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ClientSocketReceiveException();
		}

		return { buffer, (size_t)iResult };
	}

	void Disconnect() noexcept
	{
		this->m_socket = INVALID_SOCKET;

		closesocket(this->m_socket);
		WSACleanup();
	}
};

// --> SOCKETS --> CLIENT END
// --> SOCKETS --> SERVER START

class ServerSocket
{
private:
	SOCKET m_socket = INVALID_SOCKET;

	std::vector<SOCKET> m_clients;

public:
	ServerSocket()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
	}

	~ServerSocket() { this->Disconnect(); }

	void Bind(const unsigned int port)
	{
		this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (this->m_socket == INVALID_SOCKET)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[SERVER SOCKET] Socket Creation Failed");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::CREATION_FAILED);
		}

		SOCKADDR_IN sockAddr;
		sockAddr.sin_addr.s_addr = INADDR_ANY;
		sockAddr.sin_family      = AF_INET;
		sockAddr.sin_port        = htons(5555);

		if (bind(this->m_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Not Bind Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::BINDING_FAILED);
		}

		if (listen(this->m_socket, 0) == SOCKET_ERROR)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Make The Socket Listen");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::LISTENING_FAILED);
		}
	}

	[[nodiscard]] int Accept() noexcept
	{
		SOCKADDR_IN clientAddr;
		int clientAddrSize = 0;

		const SOCKET client = accept(this->m_socket, (SOCKADDR*)&clientAddr, &clientAddrSize);

		if (client != INVALID_SOCKET)
		{
			this->m_clients.push_back(client);

			return this->m_clients.size() - 1;
		}

		return -1;
	}

	void Send(const int clientID, const char* data, int length = -1)
	{
		if (length < 0)
			length = strlen(data) + 1;

		if (send(this->m_clients[clientID], data, length, 0) == SOCKET_ERROR)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Not Send Data To Client Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ServerSocketSendException();
		}
	}

	[[nodiscard]] std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> Receive(const int clientID)
	{
		std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE> buffer;

		const int iResult = recv(this->m_clients[clientID], buffer.data(), WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE, 0);

		if (iResult == 0 || iResult == SOCKET_ERROR) {
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[SERVER SOCKET] Error While Receiving Data From Client");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ServerSocketReceiveException();
		}

		return { buffer, (size_t)iResult };
	}

	void Disconnect() noexcept
	{
		this->m_socket = INVALID_SOCKET;

		closesocket(this->m_socket);
		WSACleanup();
	}
};

// --> SOCKETS --> SERVER END
// --> SOCKETS END

// --> MATH START
// --> MATH --> CONSTANTS START

constexpr float PI             = 3.14159265359f;
constexpr float TWO_PI         = 2.0f * PI;
constexpr float HALF_PI        = PI / 2.0f;
constexpr float QUARTER_PI     = PI / 4.0f;
constexpr float PI_DIV_180     = PI / 180.0f;
constexpr float PI_DIV_180_INV = 180.0f / PI;

constexpr float TAU         = TWO_PI;
constexpr float HALF_TAU    = PI;
constexpr float QUARTER_TAU = HALF_PI;

// --> MATH --> CONSTANTS END

// --> MATH --> VECTORS START
// --> MATH --> VECTORS --> TYPEDEFS START

template <typename T>
struct Vector2D
{
	T x;
	T y;
	
	template <typename K>
	void operator+=(const Vector2D<K>& v) { this->x += v.x; this->y += v.y; }

	template <typename K>
	void operator-=(const Vector2D<K>& v) { this->x -= v.x; this->y -= v.y; }

	template <typename K>
	void operator*=(const Vector2D<K>& v) { this->x *= v.x; this->y *= v.y; }

	template <typename K>
	void operator/=(const Vector2D<K>& v) { this->x /= v.x; this->y /= v.y; }

	template <typename K>
	void operator+=(const K& n) { this->x += n; this->y += n; }

	template <typename K>
	void operator-=(const K& n) { this->x -= n; this->y -= n; }

	template <typename K>
	void operator*=(const K& n) { this->x *= n; this->y *= n; }

	template <typename K>
	void operator/=(const K& n) { this->x /= n; this->y /= n; }
};

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator+(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x + b.x, a.y + b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator-(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x - b.x, a.y - b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator*(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x * b.x, a.y * b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator/(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x / b.x, a.y / b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator+(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x + n, v.y + n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator-(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x - n, v.y - n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator*(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x * n, v.y * n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator/(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x / n, v.y / n }; }

template <typename T>
struct Vector3D : Vector2D<T>
{
	T z;

	template <typename K>
	void operator+=(const Vector3D<K>& v) { this->x += v.x; this->y += v.y; this->z += v.z; }

	template <typename K>
	void operator-=(const Vector3D<K>& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }

	template <typename K>
	void operator*=(const Vector3D<K>& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }

	template <typename K>
	void operator/=(const Vector3D<K>& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }

	template <typename K>
	void operator+=(const K& n) { this->x += n; this->y += n; this->z += n; }

	template <typename K>
	void operator-=(const K& n) { this->x -= n; this->y -= n; this->z -= n; }

	template <typename K>
	void operator*=(const K& n) { this->x *= n; this->y *= n; this->z *= n; }

	template <typename K>
	void operator/=(const K& n) { this->x /= n; this->y /= n; this->z /= n; }
};

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator+(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x + b.x, a.y + b.y, a.z + b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator-(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x - b.x, a.y - b.y, a.z - b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator*(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x * b.x, a.y * b.y, a.z * b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator/(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x / b.x, a.y / b.y, a.z / b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator+(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x + n, v.y + n, v.z + n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator-(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x - n, v.y - n, v.z - n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator*(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x * n, v.y * n, v.z * n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator/(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x / n, v.y / n, v.z / n }; }

typedef Vector2D<float>    Vec2f;
typedef Vector2D<int8_t>   Vec2i8;
typedef Vector2D<int16_t>  Vec2i16;
typedef Vector2D<int32_t>  Vec2i32;
typedef Vector2D<int64_t>  Vec2i64;
typedef Vector2D<uint8_t>  Vec2u8;
typedef Vector2D<uint16_t> Vec2u16;
typedef Vector2D<uint32_t> Vec2u32;
typedef Vector2D<uint64_t> Vec2u64;

typedef Vector3D<float>    Vec3f;
typedef Vector3D<int8_t>   Vec3i8;
typedef Vector3D<int16_t>  Vec3i16;
typedef Vector3D<int32_t>  Vec3i32;
typedef Vector3D<int64_t>  Vec3i64;
typedef Vector3D<uint8_t>  Vec3u8;
typedef Vector3D<uint16_t> Vec3u16;
typedef Vector3D<uint32_t> Vec3u32;
typedef Vector3D<uint64_t> Vec3u64;

// --> MATH --> VECTORS --> TYPEDEFS END
// --> MATH --> VECTORS --> END

// --> MATH --> CONVERSIONS START

namespace Conversions
{
	[[nodiscard]] inline float DegreesToRadians(const float degrees) { return degrees * PI_DIV_180;     }
	[[nodiscard]] inline float RadiansToDegrees(const float radians) { return radians * PI_DIV_180_INV; }

	// If a polar point is stored as (θ, r)
	[[nodiscard]] inline Vec2f PolarToCartesian(const Vec2f polar)
	{
		const float x = polar.y * std::cos(polar.x);
		const float y = polar.y * std::sin(polar.x);

		return Vec2f{ x, y };
	}

	[[nodiscard]] inline Vec2f CartesianToPolar(const Vec2f cartesian)
	{
		float a = std::atan(cartesian.y / cartesian.x);

		if (cartesian.x < 0) a += PI;
		else if (cartesian.y < 0) a += TWO_PI;

		const float r = std::sqrt(std::pow(cartesian.x, 2) + std::pow(cartesian.y, 2));

		return Vec2f{ a, r };
	}
};

// --> MATH --> CONVERSIONS END
// --> MATH --> MISCELLANEOUS FUNCTIONS START 

namespace Misc
{
	[[nodiscard]] inline Vec3f GetTriangleSurfaceNormal(const Vec3f& a, const Vec3f& b, const Vec3f& c)
	{
		const Vec3f U = b - a;
		const Vec3f V = c - a;

		return Vec3f{
			U.y * V.z - U.z * V.y,
			U.z * V.x - U.x * V.z,
			U.x * V.y - U.y * V.x
		};
	}
};

// --> MATH --> MISCELLANEOUS FUNCTIONS END
// --> MATH END

// --> D3D11 START
// --> D3D11 --> MISC STRUCTS START

enum class ShaderBindingType {
	VERTEX,
	PIXEL,
	BOTH
};

// --> D3D11 --> MISC STRUCTS END

// --> D3D11 --> BUFFERS START
// --> D3D11 --> BUFFERS --> INDEX BUFFER START

struct IndexBufferDescriptor
{
	const std::vector<uint32_t> indices;
};

class IndexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	size_t m_nBytes;

public:
	IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const IndexBufferDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		this->m_nBytes = static_cast<uint64_t>(sizeof(uint32_t) * descriptor.indices.size());

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = static_cast<UINT>(this->m_nBytes);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = descriptor.indices.data();

		if (pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pIndexBuffer) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Unable To Create Index Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw IndexBufferCreationException();
		}
	}

	[[nodiscard]] size_t GetSize() const { return this->m_nBytes; }

	void Bind() const noexcept { this->m_pDeviceContextRef->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u); }
};

// --> D3D11 --> BUFFERS --> INDEX  BUFFER END
// --> D3D11 --> BUFFERS --> VERTEX BUFFER START

struct VertexBufferDescriptor
{
	const void* memoryPtr    = nullptr;
	const size_t nElements   = 0u;
	const size_t elementSize = 0u;
	const bool isUpdatable   = false;
};

class VertexBuffer {
private:
	size_t m_nElements;
	size_t m_elementSize;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexBufferDescriptor& descriptor)
		: m_nElements(descriptor.nElements), m_elementSize(descriptor.elementSize), m_pDeviceContextRef(pDeviceContextRef)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = (descriptor.isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = (descriptor.isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = static_cast<UINT>(this->m_elementSize * this->m_nElements);
		bd.StructureByteStride = static_cast<UINT>(this->m_elementSize);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = descriptor.memoryPtr;

		if (pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pVertexBuffer) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Unable To Create Vertex Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexBufferCreationException();
		}
	}

	[[nodiscard]] size_t GetElementCount() const noexcept { return this->m_nElements; }

	void SetData(const void* memoryPtr, const size_t nElements)
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		if (this->m_pDeviceContextRef->Map(this->m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Map VertexBuffer Memory");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexBufferDataSettingException();
		}

		this->m_nElements = nElements;
		
		std::memcpy(resource.pData, memoryPtr, this->m_nElements * this->m_elementSize);

		this->m_pDeviceContextRef->Unmap(this->m_pVertexBuffer.Get(), 0);
	}

	void Bind() const noexcept
	{
		const UINT stride = static_cast<UINT>(this->m_elementSize);
		const UINT offset = 0u;

		this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
};

// --> D3D11 --> BUFFERS --> VERTEX   BUFFER END
// --> D3D11 --> BUFFERS --> CONSTANT BUFFER START

struct ConstantBufferDescriptor
{
	const ShaderBindingType bindingType = ShaderBindingType::VERTEX;
	const size_t            objSize     = 0u;
	const UINT              slotVS      = 0u; // Ignored if ShaderBindingType is PIXEL
	const UINT              slotPS      = 0u; // Ignored if ShaderBindingType is VERTEX
};

class ConstantBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	ConstantBufferDescriptor m_descriptor;

public:
	ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				   const ConstantBufferDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.CPUAccessFlags = 0;
		cbd.ByteWidth = static_cast<UINT>(this->m_descriptor.objSize);

		cbd.StructureByteStride = 0u;

		if (pDeviceRef->CreateBuffer(&cbd, nullptr, &this->m_pConstantBuffer) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Unable To Create Constant Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ConstantBufferCreationException();
		}
	}

	void SetData(const void* objPtr) { this->m_pDeviceContextRef->UpdateSubresource(this->m_pConstantBuffer.Get(), 0, 0, objPtr, 0, 0); }

	void Bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetConstantBuffers(this->m_descriptor.slotVS, 1u, this->m_pConstantBuffer.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetConstantBuffers(this->m_descriptor.slotPS, 1u, this->m_pConstantBuffer.GetAddressOf());
	}
};

// --> D3D11 --> BUFFERS --> CONSTANT BUFFER END
// --> D3D11 --> BUFFERS END

// --> D3D11 --> SHADERS START
// --> D3D11 --> SHADER UTIL STRUCTS/ENUMS... START

enum class ShaderLoadingMethod {
	FROM_BINARY_FILE, // (.cso file)
	FROM_SOURCE_CODE  // (raw source code)
};

// --> D3D11 --> SHADER UTIL STRUCTS END
// --> D3D11 --> SHADERS --> PIXEL SHADER START

struct PixelShaderDescriptor
{
	const ShaderLoadingMethod loadingMethod;
	const wchar_t* binaryFilename;
	const char* sourceCode;
};

class PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				const PixelShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			if (D3DReadFileToBlob(descriptor.binaryFilename, &pBlob) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Read Pixel Shader File");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw PixelShaderCreationException();
			}
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			if (D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw PixelShaderCreationException();
			}
		}
		else
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Pixel Shaders");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw PixelShaderCreationException();
		}

		if (pDeviceRef->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw PixelShaderCreationException();
		}
	}

	void Bind() const noexcept { this->m_pDeviceContextRef->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u); }
};

// --> D3D11 --> SHADERS --> PIXEL  SHADER END
// --> D3D11 --> SHADERS --> VERTEX SHADER START

struct VertexShaderDescriptor
{
	const std::vector<std::pair<const char*, DXGI_FORMAT>> inputElementDescriptors = {};
	const ShaderLoadingMethod loadingMethod  = ShaderLoadingMethod::FROM_BINARY_FILE;
	const wchar_t*            binaryFilename = nullptr; // Ignore if ShaderLoadingMethod is FROM_SOURCE_CODE
	const char*               sourceCode     = nullptr; // Ignore if ShaderLoadingMethod is FROM_BINARY_FILE
};

class VertexShader {
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			if (D3DReadFileToBlob(descriptor.binaryFilename, &pBlob) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Read Vertex Shader File");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw VertexShaderCreationException();
			}
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			if (D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &pBlob, NULL) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Compile Vertex Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw VertexShaderCreationException();
			}
		}
		else
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Vertex Shaders");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}

		if (pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Vertex Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(descriptor.inputElementDescriptors.size());
		for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
		{
			inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
			inputElementDescriptors[i].SemanticName = descriptor.inputElementDescriptors[i].first;
			inputElementDescriptors[i].Format = descriptor.inputElementDescriptors[i].second;
			inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		}

		if (pDeviceRef->CreateInputLayout(inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Input Layout");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}
	}

	void Bind() const noexcept
	{
		this->m_pDeviceContextRef->IASetInputLayout(this->m_pInputLayout.Get());
		this->m_pDeviceContextRef->VSSetShader(this->m_pVertexShader.Get(), nullptr, 0u);
	}
};

// --> D3D11 --> SHADERS --> VERTEX SHADER END
// --> D3D11 --> SHADERS END
// --> D3D11 END

// --> MESHES START
// --> MESHES --> MESH START

struct Mesh {
	size_t vertexBufferIndex;

	size_t vertexShaderIndex;
	size_t pixelShaderIndex;

	std::optional<size_t> indexBufferIndex = { };
	std::vector<size_t> textureIndices = { };
	std::vector<size_t> textureSamplerIndices = { };

	std::vector<size_t> constantBufferIndices = { };

	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

// --> MESHES --> MESH END
// --> MESHES END

// --> TIMERS START

class Timer
{
private:
	std::chrono::system_clock::time_point m_start;

public:
	Timer() { this->m_start = std::chrono::system_clock::now(); }

	[[nodiscard]] float GetElapsedTimeMs()
	{
		const auto end = std::chrono::system_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

		return static_cast<float>(elapsed.count() * 0.001f);
	}
};

// --> TIMERS END

// --> PERIPHERAL DEVICES START
// --> PERIPHERAL DEVICES --> PERIPHERAL DEVICE START

class PeripheralDevice
{
public:
	virtual void __OnWindowUpdateBegin() = 0;
	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void __OnWindowUpdateEnd() = 0;
};

// --> PERIPHERAL DEVICES --> PERIPHERAL DEVICE END

// --> PERIPHERAL DEVICES --> MOUSE EVENT INTERFACES START

class MouseEventInterface
{
protected:
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;

	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

	std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

public:
	void OnLeftButtonUp   (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonUpFunctors.push_back(functor); }
	void OnLeftButtonDown (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonDownFunctors.push_back(functor); }
	void OnRightButtonUp  (const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonUpFunctors.push_back(functor); }
	void OnRightButtonDown(const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonDownFunctors.push_back(functor); }
	void OnWheelTurn      (const std::function<void(const int16_t)>& functor)                { this->m_onWheelTurnFunctors.push_back(functor); }
	void OnMouseMove      (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onMouseMoveFunctors.push_back(functor); }
	void OnCursorMove     (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onCursorMoveFunctors.push_back(functor); }
};

// --> PERIPHERAL DEVICES --> MOUSE EVENT INTERFACES END

// --> PERIPHERAL DEVICES --> MOUSE START

class Mouse : PeripheralDevice, public MouseEventInterface
{
private:
	Vec2u16 m_position{ 0, 0 };
	Vec2i16 m_deltaPosition{ 0, 0 };

	int16_t m_wheelDelta = 0;

	bool m_isLeftButtonDown  = false;
	bool m_isRightButtonDown = false;

	bool m_wasMouseMovedDuringUpdate  = false;
	bool m_wasCursorMovedDuringUpdate = false;

	bool m_isCursorInWindow = false;

public:
	Mouse()
	{
		RAWINPUTDEVICE mouseInputDevice;
		mouseInputDevice.usUsagePage = 0x01;
		mouseInputDevice.usUsage     = 0x02;
		mouseInputDevice.dwFlags     = 0;
		mouseInputDevice.hwndTarget  = nullptr;

		RegisterRawInputDevices(&mouseInputDevice, 1, sizeof(RAWINPUTDEVICE));
	}

	[[nodiscard]] bool IsLeftButtonUp()    const { return !this->m_isLeftButtonDown;  }
	[[nodiscard]] bool IsLeftButtonDown()  const { return this->m_isLeftButtonDown;   }
	[[nodiscard]] bool IsRightButtonUp()   const { return !this->m_isRightButtonDown; }
	[[nodiscard]] bool IsRightButtonDown() const { return this->m_isRightButtonDown;  }
	[[nodiscard]] bool IsCursorInWindow()  const { return this->m_isCursorInWindow;   }

	void Show() const { ShowCursor(true);  }
	void Hide() const { ShowCursor(false); }

	void Clip(const RECT& rect) const { ClipCursor(&rect); }

	virtual void __OnWindowUpdateBegin() override
	{
		this->m_wheelDelta = 0;
		this->m_deltaPosition = { 0, 0 };
		this->m_wasMouseMovedDuringUpdate = false;
		this->m_wasCursorMovedDuringUpdate = false;
	}

	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			UINT size = 0;

			// WINDOWS API LOGIC
			if (!GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
			{
				std::vector<char> rawBuffer(size);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					const RAWINPUT& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

					if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
					{
						this->m_deltaPosition += Vec2i16{ static_cast<int16_t>(ri.data.mouse.lLastX), static_cast<int16_t>(ri.data.mouse.lLastY) };

						this->m_wasMouseMovedDuringUpdate = true;
					}
				}
			}

			return true;
		}

		break;
		case WM_MOUSELEAVE:
			this->m_isCursorInWindow = false;

			return true;
		case WM_MOUSEMOVE:
		{
			this->m_position = { static_cast<uint16_t>(GET_X_LPARAM(lParam)), static_cast<uint16_t>(GET_Y_LPARAM(lParam)) };
			this->m_wasCursorMovedDuringUpdate = true;
		}

		this->m_isCursorInWindow = true;

		return true;
		case WM_LBUTTONDOWN:
			this->m_isLeftButtonDown = true;

			for (auto& functor : this->m_onLeftButtonDownFunctors)
				functor(this->m_position);

			return true;
		case WM_LBUTTONUP:
			this->m_isLeftButtonDown = false;

			for (auto& functor : this->m_onLeftButtonUpFunctors)
				functor(this->m_position);

			return true;
		case WM_RBUTTONDOWN:
			this->m_isRightButtonDown = true;

			for (auto& functor : this->m_onRightButtonDownFunctors)
				functor(this->m_position);

			return true;
		case WM_RBUTTONUP:
			this->m_isRightButtonDown = false;

			for (auto& functor : this->m_onRightButtonUpFunctors)
				functor(this->m_position);

			return true;
		case WM_MOUSEWHEEL:
			this->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
		default:
			return false;
		}
	}

	virtual void __OnWindowUpdateEnd() override
	{
		if (this->m_wasMouseMovedDuringUpdate)
			for (auto& functor : this->m_onMouseMoveFunctors)
				functor(this->m_position, this->m_deltaPosition);

		if (this->m_wasCursorMovedDuringUpdate)
			for (auto& functor : this->m_onCursorMoveFunctors)
				functor(this->m_position, this->m_deltaPosition);

		if (this->m_wheelDelta != 0)
			for (auto& functor : this->m_onWheelTurnFunctors)
				functor(this->m_wheelDelta);
	}
};

// --> PERIPHERAL DEVICES --> MOUSE END

// --> PERIPHERAL DEVICES --> KEYBOARD START

#define CHECK_BIT(var,pos) ((var) & ( 1 << (pos)))

class Keyboard : PeripheralDevice
{
private:
	std::vector<uint8_t> m_downKeys;

	std::vector<std::function<void(const uint8_t)>> m_onKeyUpFunctors;
	std::vector<std::function<void(const uint8_t)>> m_onKeyDownFunctors;

public:
	Keyboard()
	{

	}

	void OnKeyUp  (const std::function<void(const uint8_t)>& functor) { this->m_onKeyUpFunctors.push_back(functor);   }
	void OnKeyDown(const std::function<void(const uint8_t)>& functor) { this->m_onKeyDownFunctors.push_back(functor); }

	bool IsKeyDown(const uint8_t key) { return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key); }

	virtual void __OnWindowUpdateBegin() override { }

	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
			case WM_KEYDOWN:
			{
				const uint8_t keyCode = static_cast<uint8_t>(wParam);

				// Verify that the key was not down before the message was sent
				if (!CHECK_BIT(lParam, 30))
				{
					this->m_downKeys.push_back(keyCode);

					for (auto& functor : this->m_onKeyDownFunctors)
						functor(keyCode);
				}

				return true;
			}
			case WM_KEYUP:
			{
				const uint8_t keyCode = static_cast<uint8_t>(wParam);

				this->m_downKeys.erase(std::remove(this->m_downKeys.begin(), this->m_downKeys.end(), keyCode), this->m_downKeys.end());

				for (auto& functor : this->m_onKeyUpFunctors)
					functor(keyCode);

				return true;
			}
		}

		return false;
	}

	virtual void __OnWindowUpdateEnd() { }
};

// --> PERIPHERAL DEVICES --> KEYBOARD END

// --> PERIPHERAL DEVICES END
// --> WINDOW START

struct WindowDescriptor
{
	const uint16_t windowPositionX = 0u;
	const uint16_t windowPositionY = 0u;
	const uint16_t width, height;
	const char* title      = nullptr;
	const char* iconPath   = nullptr;
	const bool isResizable = true;
	const HINSTANCE hInstance;
};

class Window
{
friend LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

private:
	HWND m_handle = 0;

	Mouse    m_mouse;
	Keyboard m_keyboard;

	bool m_isRunning   = false;
	bool m_isMinimized = false;

	std::vector<std::function<void(const Vec2u16)>> m_onResizeFunctors;

public:
	Window(const WindowDescriptor& descriptor)
	{
		const WNDCLASSA wc{
			CS_HREDRAW | CS_VREDRAW,
			WindowProcessMessages,
			0,
			0,
			descriptor.hInstance,
			NULL,
			LoadCursor(nullptr, IDC_ARROW),
			(HBRUSH)COLOR_WINDOW, NULL,
			"WNDCLASSA"
		};

		if (!RegisterClassA(&wc))
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[WINDOW] Could Not Register Window Class");
#endif

			throw WindowCreationException(WindowCreationExceptionErrorType::CLASS_REGISTRATION_ERROR);
		}

		const uint32_t windowStyle = descriptor.isResizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

		RECT windowRect{ 0, 0, descriptor.width, descriptor.height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		this->m_handle = CreateWindowA("WNDCLASSA", descriptor.title, windowStyle,
						 descriptor.windowPositionX, descriptor.windowPositionY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
						 NULL, NULL, descriptor.hInstance, NULL);

		if (this->m_handle == NULL)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("[WINDOW] Could Not Create Window");
#endif

			throw WindowCreationException(WindowCreationExceptionErrorType::WINDOW_CREATION_ERROR);
		}

		ShowWindow(this->m_handle, SW_SHOW);
		UpdateWindow(this->m_handle);

		this->m_isRunning = true;

		if (descriptor.iconPath != nullptr)
			this->SetIcon(descriptor.iconPath);
	}

	// Misc
	void OnResize(const std::function<void(const Vec2u16)>& functor)
	{
		this->m_onResizeFunctors.push_back(functor);
	}

	// Getters
	[[nodiscard]] bool      IsRunning() const { return this->m_isRunning; }
	[[nodiscard]] Keyboard& GetKeyboard()     { return this->m_keyboard;  }
	[[nodiscard]] Mouse&    GetMouse()        { return this->m_mouse;     }
	[[nodiscard]] HWND      GetHandle() const { return this->m_handle;    }

	[[nodiscard]] uint16_t GetWindowPositionX() const { return static_cast<uint16_t>(this->GetWindowRectangle().left);   }
	[[nodiscard]] uint16_t GetWindowPositionY() const { return static_cast<uint16_t>(this->GetWindowRectangle().top);    }
	[[nodiscard]] uint16_t GetClientWidth()     const { return static_cast<uint16_t>(this->GetClientRectangle().right);  }
	[[nodiscard]] uint16_t GetClientHeight()    const { return static_cast<uint16_t>(this->GetClientRectangle().bottom); }

	[[nodiscard]] uint16_t GetWindowWidth()  const
	{
		const RECT rect = this->GetWindowRectangle();

		return static_cast<uint16_t>(rect.right - rect.left);
	}

	[[nodiscard]] uint16_t GetWindowHeight() const
	{
		const RECT rect = this->GetWindowRectangle();

		return static_cast<uint16_t>(rect.bottom - rect.top);
	}

	[[nodiscard]] RECT GetWindowRectangle() const
	{
		RECT result;
		GetWindowRect(this->m_handle, &result);
		
		return result;
	}

	[[nodiscard]] RECT GetClientRectangle() const
	{
		RECT result;
		GetClientRect(this->m_handle, &result);
		
		return result;
	}

	// Setters
	void SetWindowSize(const uint16_t width, const uint16_t height) { SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER); }

	void SetClientSize(const uint16_t width, const uint16_t height)
	{
		const uint16_t topBottomWindowPadding = this->GetWindowHeight() - this->GetClientHeight();
		const uint16_t leftRightWindowPadding = this->GetWindowWidth()  - this->GetClientWidth();

		this->SetWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
	}

	void SetTitle(const char* title) const noexcept { SetWindowTextA(this->m_handle, title); }

	void SetIcon(const char* pathname)
	{
		const HICON hIcon = (HICON)LoadImage(NULL, pathname, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		if (hIcon == NULL)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Load Icon");
#endif

			throw IconLoadingException();
		}

		SendMessage(this->m_handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}

	// MESSAGE HANDLING
	void Update()
	{
		this->m_mouse.__OnWindowUpdateBegin();
		this->m_keyboard.__OnWindowUpdateBegin();

		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		this->m_mouse.__OnWindowUpdateEnd();
		this->m_keyboard.__OnWindowUpdateEnd();
	}

	[[nodiscard]] LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_SIZE:
			{
				const Vec2u16 client_area_dimensions = {
					static_cast<uint16_t>(GET_X_LPARAM(lParam)),
					static_cast<uint16_t>(GET_Y_LPARAM(lParam))
				};

				this->m_isMinimized = (client_area_dimensions.x == 0 && client_area_dimensions.y == 0);

				for (auto& functor : this->m_onResizeFunctors)
					functor(client_area_dimensions);
			}

			return 0;
			case WM_DESTROY:
				this->Destroy();
				
				return 0;
		}

		// Dispatch Message To Peripheral Devices
		if (this->m_mouse.__HandleMessage(msg, wParam, lParam)) return 0;
		if (this->m_keyboard.__HandleMessage(msg, wParam, lParam)) return 0;

		// Otherwise Let Windows Handle The Message
		return DefWindowProc(this->m_handle, msg, wParam, lParam);
	}

	// DESTROYING
	void Destroy() { this->m_isRunning = !DestroyWindow(this->m_handle); }

	~Window() { this->Destroy(); }

public:
	static std::vector<Window> m_s_windows;

	// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
	static size_t CreateNewWindow(const WindowDescriptor& descriptor)
	{
		Window::m_s_windows.emplace_back(descriptor);

		return Window::m_s_windows.size() - 1u;
	}
};

// --> WINDOW END
// --> CAMERAS START
// --> CAMERAS --> CAMERA START

class Camera
{
protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	size_t m_transformConstantBufferIndex = 0;

public:
	Camera() { }

	[[nodiscard]] DirectX::XMMATRIX GetTransform()           const noexcept { return this->m_transform; }
	[[nodiscard]] DirectX::XMMATRIX GetTransposedTransform() const noexcept { return DirectX::XMMatrixTranspose(this->m_transform); }

	[[nodiscard]] Vec3f GetPosition() const noexcept { return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] }; }
	[[nodiscard]] Vec3f GetRotation() const noexcept { return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] }; }

	virtual void CalculateTransform() = 0;
};

// --> CAMERAS --> CAMERA END
// --> CAMERAS --> ORTHOGRAPHIC CAMERA START 

struct OrthographicCameraDescriptor
{
	const Vec2f position;
	const float ratation;
};

class OrthographicCamera : public Camera
{
private:
	float m_InvAspectRatio = 0.0f;

public:
	OrthographicCamera(Window& window, const OrthographicCameraDescriptor& descriptor)
	{
		this->m_position = DirectX::XMVectorSet(descriptor.position.x, descriptor.position.y, 0.0f, 0.0f);
		this->m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, descriptor.ratation, 0.0f);

		auto recalculateInvAspectRatio = [this](const Vec2u16& clientDims)
		{
			this->m_InvAspectRatio = clientDims.y / static_cast<float>(clientDims.x);
		};

		recalculateInvAspectRatio({ window.GetClientWidth(), window.GetClientHeight() });

		window.OnResize(recalculateInvAspectRatio);
	}

	void Rotate(const float angle) { this->m_rotation.m128_f32[2] += angle; }

	void SetPosition(const Vec2f& v) { this->m_position = DirectX::XMVectorSet(v.x, v.y, 0.0f, 0.0f); }

	void SetRotation(const Vec2f& v)
	{
		this->m_rotation.m128_f32[0] += v.x;
		this->m_rotation.m128_f32[1] += v.y;
	}

	void Translate(const Vec2f& v)
	{
		this->m_position.m128_f32[0] += v.x;
		this->m_position.m128_f32[1] += v.y;
	}

	virtual void CalculateTransform() override
	{
		this->m_transform = DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]) // Rotate
			* DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) // Translate
			* DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1.0f, 1.0f); // Correct For Aspect Ratio
	}

	void HandleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down)
	{
		if (keyboard.IsKeyDown(right))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] - speed, this->m_position.m128_f32[1], 0.0f, 0.0f);
		if (keyboard.IsKeyDown(left))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] + speed, this->m_position.m128_f32[1], 0.0f, 0.0f);

		if (keyboard.IsKeyDown(up))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] - speed, 0.0f, 0.0f);
		if (keyboard.IsKeyDown(down))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] + speed, 0.0f, 0.0f);
	}
};

// --> CAMERAS --> ORTHOGRAPHIC CAMERA END
// --> CAMERAS --> PERSPECTIVE  CAMERA START

struct PerspectiveCameraDescriptor
{
	const Vec3f position;
	const Vec3f rotation;

	const float fov;
	const float zNear;
	const float zFar;
};

class PerspectiveCamera : public Camera
{
private:
	const DirectX::XMVECTOR UP_VECTOR      = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR RIGHT_VECTOR   = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR m_forwardVector = FORWARD_VECTOR;
	DirectX::XMVECTOR m_rightVector   = RIGHT_VECTOR;

	float m_fov = 0.0f, m_aspectRatio = 0.0f, m_zNear = 0.0f, m_zFar = 0.0f;

public:
	PerspectiveCamera(Window& window, const PerspectiveCameraDescriptor& descriptor)
		: m_fov(descriptor.fov), m_zNear(descriptor.zNear), m_zFar(descriptor.zFar)
	{
		this->m_position = DirectX::XMVectorSet(descriptor.position.x, descriptor.position.y, descriptor.position.z, 0.0f);
		this->m_rotation = DirectX::XMVectorSet(descriptor.rotation.x, descriptor.rotation.y, descriptor.rotation.z, 0.0f);

		auto recalculateAspectRatio = [this](const Vec2u16& clientDims)
		{
			this->m_aspectRatio = clientDims.x / static_cast<float>(clientDims.y);
		};

		recalculateAspectRatio({ window.GetClientWidth(), window.GetClientHeight() });

		window.OnResize(recalculateAspectRatio);
	}

	void Translate(const Vec3f& v) { this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f)); }

	void SetPosition(const Vec3f& v) { this->m_position = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f); }

	void Rotate(const Vec3f& v)
	{
		const DirectX::XMVECTOR rotationDeltaVector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);

		this->m_rotation = DirectX::XMVectorAdd(this->m_rotation, rotationDeltaVector);

		// UP-DOWN Rotation Limit
		if (this->m_rotation.m128_f32[0] > HALF_PI)
			this->m_rotation.m128_f32[0] = HALF_PI;

		if (this->m_rotation.m128_f32[0] < -HALF_PI)
			this->m_rotation.m128_f32[0] = -HALF_PI;
	}

	void SetRotation(const Vec3f& v)
	{
		this->m_rotation = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);

		// UP-DOWN Rotation Limit
		if (this->m_rotation.m128_f32[0] > HALF_PI)
			this->m_rotation.m128_f32[0] = HALF_PI;

		if (this->m_rotation.m128_f32[0] < -HALF_PI)
			this->m_rotation.m128_f32[0] = -HALF_PI;
	}

	virtual void CalculateTransform() override { this->m_transform = this->GetViewMatrix() * this->GetProjectionMatrix(); }

	[[nodiscard]] DirectX::XMMATRIX GetViewMatrix()
	{
		const DirectX::XMMATRIX rotationYMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->m_rotation.m128_f32[1], 0.0f);

		this->m_forwardVector = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationYMatrix);
		this->m_rightVector = DirectX::XMVector3TransformCoord(RIGHT_VECTOR, rotationYMatrix);

		const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2]);
		const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(this->m_position, DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix));
		const DirectX::XMVECTOR upDirectionVec = DirectX::XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

		return DirectX::XMMatrixLookAtLH(this->m_position, lookAtPosition, upDirectionVec);
	}

	[[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix()
	{
		return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_zNear, m_zFar);
	}

	void HandleMouseMovements(Mouse& mouse, const float sensitivity)
	{
		mouse.OnMouseMove([sensitivity, this, &mouse](const Vec2u16 position, const Vec2i16 delta)
			{
				if (mouse.IsCursorInWindow())
					this->Rotate({ sensitivity * delta.y, sensitivity * delta.x, 0.0f });
			});
	}

	void HandleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
	{
		if (keyboard.IsKeyDown(forward))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.IsKeyDown(backward))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

		if (keyboard.IsKeyDown(right))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.IsKeyDown(left))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

		if (keyboard.IsKeyDown(up))
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		if (keyboard.IsKeyDown(down))
			this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	}

	[[nodiscard]] Vec3f GetKeyboardInputsDelta(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
	{
		DirectX::XMVECTOR delta = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		if (keyboard.IsKeyDown(forward))
			delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.IsKeyDown(backward))
			delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		if (keyboard.IsKeyDown(right))
			delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.IsKeyDown(left))
			delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		if (keyboard.IsKeyDown(up))
			delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
		if (keyboard.IsKeyDown(down))
			delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

		return Vec3f{ delta.m128_f32[0], delta.m128_f32[1], delta.m128_f32[2] };
	}
};

// --> CAMERAS --> PERSPECTIVE CAMERA END
// --> CAMERAS END

// --> IMAGES START
// --> IMAGES --> COLOR STRUCTS START

struct Coloru8
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

struct Colorf16
{
	float red;
	float green;
	float blue;
	float alpha;
};

// --> IMAGES --> COLOR STRUCTS END
// --> IMAGES --> IMAGE START

class Image {
private:
	std::unique_ptr<Coloru8[]> m_buff;

	uint16_t m_width   = 0, m_height = 0;
	uint32_t m_nPixels = 0;

public:
	Image(const wchar_t* filename)
	{
		Microsoft::WRL::ComPtr<IWICBitmapSource>      decodedConvertedFrame;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>     bitmapDecoder;
		Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;
		
		if (CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create IWICImagingFactory")
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (factory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapDecoder) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Read/Open Image");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (bitmapDecoder->GetFrame(0, &frameDecoder) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Get First Frame Of Image");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (frameDecoder->GetSize((UINT*)&this->m_width, (UINT*)&this->m_height) != S_OK)
		{

#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Get Image Width/Height");
#endif // __WEISS_SHOW_DEBUG_ERRORS
			
			throw ImageLoadingException();
		}

		this->m_nPixels = this->m_width * this->m_height;

		if (WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, frameDecoder.Get(), &decodedConvertedFrame) != S_OK)
		{

#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Bitmap Converter");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

		const WICRect sampleRect{ 0, 0, this->m_width, this->m_height };

		if (decodedConvertedFrame->CopyPixels(&sampleRect, this->m_width * sizeof(Coloru8), this->m_nPixels * sizeof(Coloru8), (BYTE*)this->m_buff.get()) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Copy Pixels From Bitmap");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}
	}

	[[nodiscard]] uint16_t GetWidth()                                 const { return this->m_width;                       }
	[[nodiscard]] uint16_t GetHeight()                                const { return this->m_height;                      }
	[[nodiscard]] uint32_t GetPixelCount()                            const { return this->m_nPixels;                     }
	[[nodiscard]] Coloru8* GetBuffer()                                const { return this->m_buff.get();                  }
	[[nodiscard]] Coloru8  Sample(const uint16_t x, const uint16_t y) const { return this->m_buff[y * this->m_width + x]; }

	void Set(const uint16_t x, const uint16_t y, const Coloru8& color) { this->m_buff[y * this->m_width + x] = color; }
};

// --> IMAGES --> IMAGE END
// --> IMAGES --> TEXTURE 2D START

struct Texture2DDescriptor
{
	const ShaderBindingType bindingType;
	const Image& image;
	const UINT slotVS;
	const UINT slotPS;
	const bool useMipMaps;
};

class Texture2D {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	Texture2DDescriptor m_descriptor;

public:
	Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor) : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = static_cast<void*>(descriptor.image.GetBuffer());
		subResourceData.SysMemPitch = this->m_descriptor.image.GetWidth() * sizeof(Coloru8);
		subResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
		texture2DDescriptor.Width = this->m_descriptor.image.GetWidth();
		texture2DDescriptor.Height = this->m_descriptor.image.GetHeight();
		texture2DDescriptor.MipLevels = this->m_descriptor.useMipMaps ? 0 : 1;
		texture2DDescriptor.ArraySize = 1;
		texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDescriptor.SampleDesc.Count = 1;
		texture2DDescriptor.SampleDesc.Quality = 0;
		texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
		texture2DDescriptor.BindFlags = this->m_descriptor.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
		texture2DDescriptor.MiscFlags = this->m_descriptor.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

		if (pDeviceRef->CreateTexture2D(&texture2DDescriptor, this->m_descriptor.useMipMaps ? nullptr : &subResourceData, &texture2D) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Texture 2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw Texture2DCreationException();
		}

		if (this->m_descriptor.useMipMaps)
			this->m_pDeviceContextRef->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData.pSysMem, subResourceData.SysMemPitch, 0u);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
		SRVDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDescriptor.Texture2D.MostDetailedMip = 0;
		SRVDescriptor.Texture2D.MipLevels = this->m_descriptor.useMipMaps ? -1 : 1;

		if (pDeviceRef->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create ShaderResourceView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw Texture2DCreationException();
		}

		if (this->m_descriptor.useMipMaps)
			this->m_pDeviceContextRef->GenerateMips(m_pShaderResourceView.Get());
	}

	void Bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
	}
};

// --> IMAGES --> TEXTURE 2D END
// --> IMAGES --> TEXTURE SAMPLER START

struct TextureSamplerDescriptor
{
	const ShaderBindingType bindingType;
	const UINT slotVS;
	const UINT slotPS;
	const D3D11_FILTER filter;
	const D3D11_TEXTURE_ADDRESS_MODE addressMode;
};

class TextureSampler {
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	TextureSamplerDescriptor m_descriptor;

public:
	TextureSampler(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const TextureSamplerDescriptor& descriptor) : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_SAMPLER_DESC samplerDescriptor;
		samplerDescriptor.Filter = descriptor.filter;
		samplerDescriptor.AddressU = descriptor.addressMode;
		samplerDescriptor.AddressV = descriptor.addressMode;
		samplerDescriptor.AddressW = descriptor.addressMode;
		samplerDescriptor.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
		samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDescriptor.MipLODBias = 0.0f;
		samplerDescriptor.MinLOD = 0.0f;
		samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;

		pDeviceRef->CreateSamplerState(&samplerDescriptor, &this->m_pSamplerState);
	}

	void Bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetSamplers(this->m_descriptor.slotVS, 1u, this->m_pSamplerState.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetSamplers(this->m_descriptor.slotPS, 1u, this->m_pSamplerState.GetAddressOf());
	}
};

// --> IMAGES --> TEXTURE SAMPLER END
// --> IMAGES END

// --> PHYSICS START
// --> PHYSICS --> PHYSICS OBJECT START

struct PhysicsObject
{
	Vec3f position{ 0.f, 0.f, 0.f };
	Vec3f velocity{ 0.f, 0.f, 0.f };
	Vec3f acceleration{ 0.f, 0.f, 0.f };

	void UpdatePhysicsObject(const float friction = 0.9f)
	{
		this->velocity += this->acceleration;
		this->velocity *= 0.9f;
		this->acceleration = Vec3f{ 0.f, 0.f, 0.f };

		this->position += this->velocity;
	}
};

// --> PHYSICS --> PHYSICS OBJECT END
// --> PHYSICS END

// --> ENGINE START
// --> ENGINE --> ENGINE DESCRIPTORS START

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

struct EngineDescriptor
{
	WindowDescriptor windowDesc;
	OrthographicCameraDescriptor orthographicCameraDesc;
	PerspectiveCameraDescriptor  perspectiveCameraDesc;
};

// --> ENGINE --> ENGINE DESCRIPTORS END
// --> ENGINE --> ENGINE STRUCTURES & DATA-TYPES START

struct Colored2DVertex {
	Vec2f    position;
	Coloru8  color;
	uint32_t doApplyTransform = true;
};

template <typename V>
struct Triangle
{
	V v1;
	V v2;
	V v3;
};

typedef Triangle<Colored2DVertex> ColoredTriangle2D;

// --> ENGINE --> ENGINE STRUCTURES & DATA-TYPES END
// --> ENGINE --> BATCH RENDERER BASE CLASS START

template <typename V>
class Batch2DRenderer
{
protected:
	std::vector<Triangle<V>> m_triangles;

	size_t m_vertexShaderIndex = WEISS_NO_RESOURCE_INDEX;
	size_t m_pixelShaderIndex  = WEISS_NO_RESOURCE_INDEX;
	size_t m_vertexBufferIndex = WEISS_NO_RESOURCE_INDEX;

	std::vector<size_t> m_meshes;

public:
	Triangle<V>& getTriangle(const size_t index) noexcept
	{
		return this->m_triangles[index];
	}

	size_t addTriangle(const Triangle<V>& tr) noexcept
	{
		this->m_triangles.push_back(tr);

		return this->m_triangles.size() - 1u;
	}

	virtual void Draw() = 0;
};
// --> ENGINE --> BATCH RENDERER BASE CLASS END
// --> ENGINE --> ENGINE CLASS START

class Engine
{
// --> ENGINE --> ENGINE CLASS --> CORE START
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOn;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOff;

	std::vector<Mesh>           meshes;

	std::vector<PixelShader>    pixelShaders;
	std::vector<VertexShader>   vertexShaders;

	std::vector<VertexBuffer>   vertexBuffers;
	std::vector<IndexBuffer>    indexBuffers;
	std::vector<ConstantBuffer> constantBuffers;

	std::vector<Texture2D>      textures;
	std::vector<TextureSampler> textureSamplers;

	size_t    windowIndex = 0u;
	Mouse* mouse = nullptr;
	Keyboard* keyboard = nullptr;

private:
	void CreateDeviceAndSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = this->GetWindow().GetHandle();
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Device And SwapChain");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	void CreateRenderTarget()
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

		if (this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Get BackBuffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}

		if (this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create RenderTargetView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	void CreateViewport()
	{
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<FLOAT>(this->GetWindow().GetClientWidth());
		vp.Height = static_cast<FLOAT>(this->GetWindow().GetClientHeight());
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		this->m_pDeviceContext->RSSetViewports(1u, &vp);
	}

	void CreateDepthStencilStates()
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOn) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create DepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}

		dsDesc.DepthEnable = FALSE;

		if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOff) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create EmptyDepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	void CreateDepthStencil()
	{
		// Create Depth Texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = this->GetWindow().GetClientWidth();
		descDepth.Height = this->GetWindow().GetClientHeight();
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		if (this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Texture2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}

		// Create Depth Stencil Texture View
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;

		if (this->m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create DepthStencilView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	void BindDepthStencil()
	{
		this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
	}

	/*
	 * This function initializes DirectX and creates key components
	*/
	void InitGraphics()
	{
		this->CreateDeviceAndSwapChain();
		this->CreateRenderTarget();
		this->CreateViewport();
		this->CreateDepthStencilStates();
		this->CreateDepthStencil();
		this->BindDepthStencil();
	}

	/* 
	 * This functions swaps the back buffer and the front buffer to show the frame to the user
	 * It also clears the depthStencilView
	 */
	void PresentFrame(const bool useVSync)
	{
		if (this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Present Frame");
#endif

			throw EngineInitializationException();
		}

		// Clear Depth Stencil
		this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void CreateDefaultConstantBuffers()
	{
		const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(DirectX::XMMATRIX), 0u, 0u };

		if (this->CreateConstantBuffer(cbd) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

public:
	Engine()
	{
		if (CoInitialize(NULL) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Initialize COM");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	~Engine()
	{
		// No Need To Destroy Window Because It Destroys Itself
		delete this->orthographicCamera;
		delete this->perspectiveCamera;
	}

	void InitEngine(const EngineDescriptor& descriptor)
	{
		this->windowIndex = Window::CreateNewWindow(descriptor.windowDesc);

		this->mouse = &(this->GetWindow().GetMouse());
		this->keyboard = &(this->GetWindow().GetKeyboard());

		this->orthographicCamera = new OrthographicCamera(this->GetWindow(), descriptor.orthographicCameraDesc);
		this->perspectiveCamera = new PerspectiveCamera(this->GetWindow(), descriptor.perspectiveCameraDesc);

		this->InitGraphics();
		this->CreateDefaultConstantBuffers();

		this->GetWindow().OnResize([this](const Vec2u16 dimensions)
			{
				this->InitGraphics();
			});
	}

	void Run(const bool useVSync = true, const uint16_t fps = 60)
	{
		Timer timer;
		uint32_t frames = 0;

		while (this->GetWindow().IsRunning())
		{
			const float elapsed = timer.GetElapsedTimeMs();

			if (!useVSync)
			{
				if (elapsed >= 1 / static_cast<float>(fps) * 1000)
				{
					timer = Timer();
				}
				else
				{
					std::this_thread::yield();
					continue;
				}
			}

			this->GetWindow().Update();

			this->OnRender(elapsed);

			this->PresentFrame(useVSync);
		}
	}

	virtual void OnRender(const float elapsed) = 0;

	void CaptureCursor()
	{
		// Clip Cursor
		const RECT windowRect = this->GetWindow().GetWindowRectangle();
		const RECT clientRect = this->GetWindow().GetClientRectangle();

		RECT boundingRect = windowRect;

		boundingRect.top += (windowRect.bottom - windowRect.top) - clientRect.bottom + 10; // padding
		boundingRect.left += 10; // padding
		boundingRect.right -= 10; // padding
		boundingRect.bottom -= 10; // padding

		this->mouse->Clip(boundingRect);

		// Hide Cursor
		this->mouse->Hide();
	}

	void PlayWavFile(const char* filename)
	{
		if (!PlaySound(TEXT(filename), NULL, SND_ASYNC | SND_FILENAME))
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Play Sound From File");
#endif // __WEISS_SHOW_DEBUG_ERRORS
		}
	}

	void DrawMesh(const size_t meshIndex, size_t count = 0u)
	{
		Mesh& mesh = this->meshes[meshIndex];

		this->vertexBuffers[mesh.vertexBufferIndex].Bind();
		this->vertexShaders[mesh.vertexShaderIndex].Bind();
		this->pixelShaders[mesh.pixelShaderIndex].Bind();

		for (const size_t textureIndex : mesh.textureIndices)
			this->textures[textureIndex].Bind();

		for (const size_t textureSamplerIndex : mesh.textureSamplerIndices)
			this->textureSamplers[textureSamplerIndex].Bind();

		for (const size_t cbIndex : mesh.constantBufferIndices)
			this->constantBuffers[cbIndex].Bind();

		this->m_pDeviceContext->IASetPrimitiveTopology(mesh.primitiveTopologyType);

		if (mesh.indexBufferIndex.has_value())
		{
			this->indexBuffers[mesh.indexBufferIndex.value()].Bind();

			if (count == 0u)
				count = this->indexBuffers[mesh.indexBufferIndex.value()].GetSize();

			this->m_pDeviceContext->DrawIndexed(count, 0u, 0u);
		}
		else
		{
			if (count == 0u)
				count = this->vertexBuffers[mesh.vertexBufferIndex].GetElementCount();

			this->m_pDeviceContext->Draw(count, 0u);
		}
	}

	[[nodiscard]] size_t CreateVertexShader(const VertexShaderDescriptor& descriptor)
	{
		this->vertexShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->vertexShaders.size() - 1;
	}

	[[nodiscard]] size_t CreatePixelShader(const PixelShaderDescriptor& descriptor)
	{
		this->pixelShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->pixelShaders.size() - 1;
	}

	[[nodiscard]] size_t CreateTextureFromImage(const Texture2DDescriptor& descriptor)
	{
		this->textures.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->textures.size() - 1;
	}

	[[nodiscard]] size_t CreateTextureSampler(const TextureSamplerDescriptor& descriptor)
	{
		this->textureSamplers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->textureSamplers.size() - 1;
	}

	[[nodiscard]] size_t CreateConstantBuffer(const ConstantBufferDescriptor& descriptor)
	{
		this->constantBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->constantBuffers.size() - 1;
	}

	[[nodiscard]] size_t CreateVertexBuffer(const VertexBufferDescriptor& descriptor)
	{
		this->vertexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->vertexBuffers.size() - 1;
	}

	[[nodiscard]] size_t CreateIndexBuffer(const IndexBufferDescriptor& descriptor)
	{
		this->indexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

		return this->indexBuffers.size() - 1;
	}

	[[nodiscard]] size_t CreateMeshFromVertices(const Mesh& mesh)
	{
		this->meshes.push_back(mesh);

		return this->meshes.size() - 1;
	}

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename)
	{
		std::ifstream infile(filename);

		std::vector<Vec3f>    vertices;
		std::vector<uint32_t> indices;

		std::string opcode;
		char junkChar;
		float junkFloat;
		std::string line;

		while (std::getline(infile, line))
		{
			std::istringstream iss(line);

			iss >> opcode;

			if (opcode == "v")
			{
				Vec3f vertex{ 0.f, 0.f, 0.f };

				iss >> vertex.x >> vertex.y >> vertex.z;

				vertices.push_back(vertex);
			}
			else if (opcode == "f")
			{
				const uint16_t spaceCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), ' '));
				const uint16_t slashCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), '/'));

				if (spaceCount == 3)
				{
					uint32_t i1 = 0, i2 = 0, i3 = 0;

					if (slashCount == 0)
						iss >> i1 >> i2 >> i3;
					else if (slashCount == 3)
						iss >> i1 >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> i3;
					else if (slashCount == 6)
						iss >> i1 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i3;

					indices.push_back(i1 - 1);
					indices.push_back(i2 - 1);
					indices.push_back(i3 - 1);
				}
			}
		}

		return { vertices, indices };
	}

	[[nodiscard]] Mesh&           GetMesh          (const size_t index) noexcept { return this->meshes[index];          }
	[[nodiscard]] Texture2D&      GetTexture       (const size_t index) noexcept { return this->textures[index];        }
	[[nodiscard]] VertexShader&   GetVertexShader  (const size_t index) noexcept { return this->vertexShaders[index];   }
	[[nodiscard]] PixelShader&    GetPixelShader   (const size_t index) noexcept { return this->pixelShaders[index];    }
	[[nodiscard]] ConstantBuffer& GetConstantBuffer(const size_t index) noexcept { return this->constantBuffers[index]; }
	[[nodiscard]] VertexBuffer&   GetVertexBuffer  (const size_t index) noexcept { return this->vertexBuffers[index];   }
	[[nodiscard]] IndexBuffer&    GetIndexBuffer   (const size_t index) noexcept { return this->indexBuffers[index];    }
	[[nodiscard]] TextureSampler& GetTextureSampler(const size_t index) noexcept { return this->textureSamplers[index]; }

	// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
	[[nodiscard]] Window&   GetWindow()   noexcept { return Window::m_s_windows[this->windowIndex]; }
	[[nodiscard]] Mouse&    GetMouse()    noexcept { return *this->mouse;                           }
	[[nodiscard]] Keyboard& GetKeybaord() noexcept { return *this->keyboard;                        }
	[[nodiscard]] Engine&   GetEngine()   noexcept { return *this;                                  }

	// --> ENGINE --> ENGINE CLASS --> CORE END
	// --> ENGINE --> ENGINE CLASS --> 2D (Abstracted) START
private:
	OrthographicCamera* orthographicCamera = nullptr;

	Batch2DRenderer<Colored2DVertex>* coloredBatch2DRenderer = nullptr;

private:
	void Init2DBatchRenderers();

public:
	void InitEngine2D()
	{
		this->Init2DBatchRenderers();
	}

	void SetRenderMode2D()
	{
		ConstantBuffer& cameraTransformConstantBuffer = this->constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
		const DirectX::XMMATRIX orthographicCameraTransposedTransform = this->orthographicCamera->GetTransposedTransform();
		cameraTransformConstantBuffer.SetData(&orthographicCameraTransposedTransform);
		cameraTransformConstantBuffer.Bind();

		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOff.Get(), 1u);
	}

	void Fill(const Coloru8& color)
	{
		float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

		this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
	}

	[[nodiscard]] OrthographicCamera&               GetOrthographicCamera()     noexcept { return *this->orthographicCamera; }
	[[nodiscard]] Batch2DRenderer<Colored2DVertex>& GetColoredBatchRenderer2D() noexcept { return *this->coloredBatch2DRenderer; }

	// --> ENGINE --> ENGINE CLASS --> 2D (Abstracted) END
	// --> ENGINE --> ENGINE CLASS --> 3D (Abstracted) START
private:
	PerspectiveCamera* perspectiveCamera = nullptr;

public:
	void SetRenderMode3D()
	{
		ConstantBuffer& cameraTransformConstantBuffer = this->constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
		const DirectX::XMMATRIX perspectiveCameraTransposedTransform = this->perspectiveCamera->GetTransposedTransform();
		cameraTransformConstantBuffer.SetData(&perspectiveCameraTransposedTransform);
		cameraTransformConstantBuffer.Bind();

		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
	}

	[[nodiscard]] PerspectiveCamera& GetPerspectiveCamera() noexcept { return *this->perspectiveCamera; }

	// --> ENGINE --> ENGINE CLASS --> 3D (Abstracted) END
};

// --> ENGINE --> ENGINE CLASS END
// --> ENGINE --> BATCH RENDERERS START

class ColoredBatch2DRenderer : public Batch2DRenderer<Colored2DVertex>
{
private:
	Engine& m_engine;

private:
	void CreateNewMesh()
	{
		Colored2DVertex junk[WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER] = {  };

		const VertexBufferDescriptor vbd{ junk, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER, sizeof(Colored2DVertex), true };
		this->m_vertexBufferIndex = this->m_engine.CreateVertexBuffer(vbd);

		Mesh mesh{ this->m_vertexBufferIndex, this->m_vertexShaderIndex, this->m_pixelShaderIndex };

		this->m_meshes.push_back(this->m_engine.CreateMeshFromVertices(mesh));
	}

public:
	ColoredBatch2DRenderer(Engine& engine)
		: m_engine(engine)
	{
		// Load Shaders
		std::vector<std::pair<const char*, DXGI_FORMAT>> ieds = {
			{ "Position",         DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT    },
			{ "Color",            DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM  },
			{ "DoApplyTransform", DXGI_FORMAT::DXGI_FORMAT_R32_UINT        } // 1 if you want to apply to the orthographic / perspective transform
		};

		const char* vsSource = "cbuffer cbuff { matrix transform; }\n"
			"struct VSoutput { float4 out_color : Color; float4 out_positionSV : SV_Position; };\n"
			"VSoutput main(float2 in_position : Position, float4 in_color : Color, uint doApplyTransform : DoApplyTransform) {"
				"VSoutput output;\n"
				"output.out_color = in_color;\n"
				"output.out_positionSV = float4(in_position, 0.0f, 1.0f);\n"
				"if (doApplyTransform == 1) { output.out_positionSV = mul(output.out_positionSV, transform); }\n"
			"return output;"
			"}";

		const VertexShaderDescriptor vsd = { ieds, ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, vsSource };

		this->m_vertexShaderIndex = this->m_engine.CreateVertexShader(vsd);

		const char* psSource = "float4 main(float4 color : Color) : SV_TARGET { return color; }";

		const PixelShaderDescriptor psd = { ShaderLoadingMethod::FROM_SOURCE_CODE, nullptr, psSource };

		this->m_pixelShaderIndex = this->m_engine.CreatePixelShader(psd);
	}

	virtual void Draw() override
	{
		const size_t nMeshToAdd = std::ceil((this->m_triangles.size() * 3u) / (float)WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER) - this->m_meshes.size();

		for (size_t i = 0u; i < nMeshToAdd; i++)
			this->CreateNewMesh();

		// Fill Vertex Buffers And Draw

		for (size_t i = 0u; i < this->m_meshes.size(); i++)
		{
			Colored2DVertex vertices[WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER];

			const void* srcPtr = this->m_triangles.data() + i * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;

			std::memcpy(vertices, srcPtr, sizeof(Colored2DVertex) * WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER);

			Mesh& mesh = this->m_engine.GetMesh(this->m_meshes[i]);
			VertexBuffer& vertexBuffer = this->m_engine.GetVertexBuffer(mesh.vertexBufferIndex);

			vertexBuffer.SetData(vertices, WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER);

			const size_t nVerticesToDraw = (i != this->m_meshes.size() - 1u) ? WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER : this->m_triangles.size() * 3u - i * WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER;

			this->m_engine.DrawMesh(this->m_meshes[i], nVerticesToDraw);
		}
	}
};

// --> ENGINE --> BATCH RENDERERS END
// --> ENGINE END

#endif // __WEISS__

#ifdef __WEISS_LAST_INCLUDE

std::vector<Window> Window::m_s_windows = std::vector<Window>();

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::vector<Window>::iterator it = std::find_if(Window::m_s_windows.begin(), Window::m_s_windows.end(), [&hwnd](const Window& window)
		{
			return window.m_handle == hwnd;
		});

	if (it != Window::m_s_windows.end())
		return it->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Engine::Init2DBatchRenderers()
{
	this->coloredBatch2DRenderer = new ColoredBatch2DRenderer(this->GetEngine());
}

#endif // __WEISS_LAST_INCLUDE

/*

██          ██    ██████████████      ██████████      ██████████████    ██████████████
██          ██    ██                      ██          ██                ██
██          ██    ██                      ██          ██                ██
██    ██    ██    ██████████████          ██          ██████████████    ██████████████
██    ██    ██    ██                      ██                      ██                ██
██  ██  ██  ██    ██                      ██                      ██                ██
  ██      ██      ██████████████      ██████████      ██████████████    ██████████████

██████████████    ██          ██    ██████████████      ██████████      ██          ██    ██████████████
██                ████        ██    ██                      ██          ████        ██    ██
██                ██  ██      ██    ██                      ██          ██  ██      ██    ██
██████████████    ██    ██    ██    ██    ██████            ██          ██    ██    ██    ██████████████
██                ██      ██  ██    ██          ██          ██          ██      ██  ██    ██
██                ██        ████    ██          ██          ██          ██        ████    ██
██████████████    ██          ██    ████████████        ██████████      ██          ██    ██████████████

*/