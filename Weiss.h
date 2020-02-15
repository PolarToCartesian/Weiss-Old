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

#ifndef __WEISS__
#define __WEISS__

#ifndef _WIN32
#error Weiss Is Windows Only! (for now)
#endif // _WIN32

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

#undef _WINSOCKAPI_
#undef _WINSOCK_DEPRECATED_NO_WARNINGS
#undef NOMINMAX

class WeissException : public std::exception {  };

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

	template<typename K>
	bool operator==(const Vector2D<K>& v) { return this->x == v.x && this->y == v.y; }

	template<typename K>
	bool operator!=(const Vector2D<K>& v) { return this->x != v.x || this->y != v.y; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ")";
	return os;
}

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

	template<typename K>
	bool operator==(const Vector3D<K>& v) { return this->x == v.x && this->y == v.y && this->z == v.z; }

	template<typename K>
	bool operator!=(const Vector3D<K>& v) { return this->x != v.x || this->y != v.y || this->z != v.z; }
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

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

constexpr float PI             = 3.14159265359f;
constexpr float TWO_PI         = 2.0f * PI;
constexpr float HALF_PI        = PI / 2.0f;
constexpr float QUARTER_PI     = PI / 4.0f;
constexpr float PI_DIV_180     = PI / 180.0f;
constexpr float PI_DIV_180_INV = 180.0f / PI;

constexpr float TAU         = TWO_PI;
constexpr float HALF_TAU    = PI;
constexpr float QUARTER_TAU = HALF_PI;

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

struct Coloru8
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

struct Colorf32
{
	float red;
	float green;
	float blue;
	float alpha;
};

class Timer {
private:
	std::chrono::high_resolution_clock::time_point m_start;

public:
	Timer()
    {
        this->m_start = std::chrono::high_resolution_clock::now();
    }

	[[nodiscard]] float GetElapsedTimeMs()
    {
        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - this->m_start);

        return static_cast<float>(elapsed.count() * 0.001f);
    }
};

class PeripheralDevice {
public:
	virtual void __OnWindowUpdateBegin() = 0;
	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void __OnWindowUpdateEnd() = 0;
};

class MouseEventInterface {
protected:
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;

	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

	std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

public:
	void OnLeftButtonUp   (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonUpFunctors.push_back(functor);    }
	void OnLeftButtonDown (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonDownFunctors.push_back(functor);  }
	void OnRightButtonUp  (const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonUpFunctors.push_back(functor);   }
	void OnRightButtonDown(const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonDownFunctors.push_back(functor); }
	void OnWheelTurn      (const std::function<void(const int16_t)>& functor)                { this->m_onWheelTurnFunctors.push_back(functor);       }
	void OnMouseMove      (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onMouseMoveFunctors.push_back(functor);       }
	void OnCursorMove     (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onCursorMoveFunctors.push_back(functor);      }
};

class Mouse : PeripheralDevice, public MouseEventInterface {
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
        mouseInputDevice.usUsage = 0x02;
        mouseInputDevice.dwFlags = 0;
        mouseInputDevice.hwndTarget = nullptr;

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

#define CHECK_BIT(var,pos) ((var) & ( 1 << (pos)))

#define ENABLE_CONSOLE() {\
	AllocConsole();\
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);\
}

#define MESSAGE_BOX_ERROR(errorMsg) {\
	MessageBox(NULL, (std::string("Error in File: ") + std::string(__FILE__) + std::string("\nAt line: ") + std::to_string(__LINE__) + std::string("\nIn Function: ") + std::string(__FUNCTION__) + std::string("\nError: ") + std::string(errorMsg)).c_str(), "Weiss Engine Error", MB_ABORTRETRYIGNORE);\
}


class Keyboard : PeripheralDevice {
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

	bool IsKeyDown(const uint8_t key)
    {
	    return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key);
    }

	virtual void __OnWindowUpdateBegin() override
    {

    }

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

	virtual void __OnWindowUpdateEnd() override
    {

    }
};

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

enum class WindowCreationExceptionErrorType
{
	CLASS_REGISTRATION_ERROR,
	WINDOW_CREATION_ERROR
};

class IconLoadingException : public WeissException { };

class WindowCreationException : public WeissException {
private:
	const WindowCreationExceptionErrorType m_type;

public:
	WindowCreationException(const WindowCreationExceptionErrorType& type)
        : m_type(type) { }

	WindowCreationExceptionErrorType getErrorType() const { return this->m_type; }
};

struct WindowDescriptor
{
	const uint16_t windowPositionX = 0u;
	const uint16_t windowPositionY = 0u;
	const uint16_t width, height;
	const char* title      = nullptr;
	const char* iconPath   = nullptr;
	const bool isResizable = true;
};

class Window {
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
        WNDCLASSA wc;
        ZeroMemory(&wc, sizeof(WNDCLASSA));

        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = WindowProcessMessages;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = "WEISS_WNDCLASSA";

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

        this->m_handle = CreateWindowA("WEISS_WNDCLASSA", descriptor.title, windowStyle,
            descriptor.windowPositionX, descriptor.windowPositionY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            NULL, NULL, wc.hInstance, NULL);

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

        #ifdef _WIN64
            SetWindowLongPtr(this->m_handle, GWLP_USERDATA, (LONG_PTR)this);
        #else
            SetWindowLong(this->m_handle, GWLP_USERDATA, (LONG)this);
        #endif
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

	[[nodiscard]] uint16_t GetWindowWidth() const
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
	void SetWindowSize(const uint16_t width, const uint16_t height)
    {
        SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
    }

	void SetClientSize(const uint16_t width, const uint16_t height)
    {
        const uint16_t topBottomWindowPadding = this->GetWindowHeight() - this->GetClientHeight();
        const uint16_t leftRightWindowPadding = this->GetWindowWidth()  - this->GetClientWidth();

        this->SetWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
    }

	void SetTitle(const char* title) const noexcept
    {
        SetWindowTextA(this->m_handle, title);
    }

	void SetIcon(const char* pathname)
    {
        const HICON hIcon = (HICON)LoadImageA(NULL, pathname, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

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
	void Destroy()
    {
        this->m_isRunning = !DestroyWindow(this->m_handle);
    }

	~Window()
    {
        this->Destroy();
    }

public:
	static std::list<Window> m_s_windows;

	static Window& Create(const WindowDescriptor& windowDesc)
    {
        Window::m_s_windows.emplace_back(windowDesc);

        return Window::m_s_windows.back();
    }
};

std::list<Window> Window::m_s_windows = std::list<Window>();

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#ifdef _WIN64
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	#else
		Window* window = (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
	#endif

	if (window != NULL)
		return window->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

constexpr const size_t WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE      = 1024u;
constexpr const size_t WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER  = 600u;
constexpr const size_t WEISS_MAX_VERTICES_PER_BATCH_VERTEX_BUFFER   = 3u * WEISS_MAX_TRIANGLES_PER_BATCH_VERTEX_BUFFER;
constexpr const size_t WEISS_NO_RESOURCE_INDEX                      = std::numeric_limits<size_t>::max();

constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX = 0u;
constexpr const size_t WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT  = 0u;

constexpr const size_t WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX = 1u;
constexpr const size_t WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_SLOT  = 1u;

constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_TRANSFORM = 1u << 0u;
constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_LIGHTING  = 1u << 1u;

constexpr const uint32_t WEISS_COLORED_BATCH_RENDERER_2D_ALL_FLAGS = WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_TRANSFORM | WEISS_COLORED_BATCH_RENDERER_2D_FLAG_APPLY_LIGHTING;

constexpr const uint16_t WEISS_SPRITE_SHEET_SIDE_LENGTH = 1024u; // 2^10

constexpr const char* WEISS_COLORED_BATCH_2D_RENDERER_VS_SOURCE = ""
"cbuffer cbuff0 { matrix transform; }\n"
"cbuffer cbuff1 { float4 ambiant; }\n"
"struct VSoutput { float4 out_color : Color; float4 out_positionSV : SV_Position; };\n"
"VSoutput main(float2 in_position : Position, float4 in_color : Color, uint in_flags : Flags) {"
	"VSoutput output;\n"
	"output.out_color = in_color;\n"
	"output.out_positionSV = float4(in_position, 0.0f, 1.0f);\n"
	"if ((in_flags & 2) > 0) { output.out_color = output.out_color + ambiant; }\n"
	"if ((in_flags & 1) > 0) { output.out_positionSV = mul(output.out_positionSV, transform); }\n"
	"return output;\n"
"}";

constexpr const char* WEISS_COLORED_BATCH_2D_RENDERER_PS_SOURCE = ""
"float4 main(float4 color : Color) : SV_TARGET {"
	"return color;\n"
"}";

const std::vector<std::pair<const char*, DXGI_FORMAT>> WEISS_COLORED_BATCH_2D_RENDERER_IEDS =
{
	{ "Position", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT   },
	{ "Color",    DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM },
	{ "Flags",    DXGI_FORMAT::DXGI_FORMAT_R32_UINT       },
};

struct Drawable
{
	size_t vertexBufferIndex = WEISS_NO_RESOURCE_INDEX;
	size_t vertexShaderIndex = WEISS_NO_RESOURCE_INDEX;
	size_t pixelShaderIndex  = WEISS_NO_RESOURCE_INDEX;

	std::optional<size_t> indexBufferIndex = { };
	std::vector<std::string> textureNames = { };
	std::vector<size_t> textureSamplerIndices = { };

	std::vector<size_t> constantBufferIndices = { };

	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct DeviceInfo
{
	Microsoft::WRL::ComPtr<ID3D11Device>& m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContext;
};

template <typename T>
class Bindable {
protected:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<T> m_pResource;

public:
	Bindable(const DeviceInfo& deviceInfo)
		: m_deviceInfo(deviceInfo) {  }

	virtual void Bind() const noexcept = 0;
};

template <typename M_T_CREATION_EXCEPTION, typename M_T_MAPPING_EXCEPTION>
class Buffer : public Bindable<ID3D11Buffer> {
protected:
	UINT m_byteWidth;
	UINT m_structuredByteStride;

protected:
	void __Buffer__SetData(const void* buff, const size_t nBytes) const
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		if (this->m_deviceInfo.m_pDeviceContext->Map(this->m_pResource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
		{
			throw M_T_MAPPING_EXCEPTION();
		}

		std::memcpy(resource.pData, buff, nBytes);

		this->m_deviceInfo.m_pDeviceContext->Unmap(this->m_pResource.Get(), 0);
	}

public:
	Buffer(const DeviceInfo& deviceInfo, const void* buff, const UINT byteWidth, const UINT structuredByteStride, const D3D11_BIND_FLAG bindFlags, const bool isUpdatable)
		: Bindable(deviceInfo), m_byteWidth(byteWidth), m_structuredByteStride(structuredByteStride)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = bindFlags;
		bd.Usage = (isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = (isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = this->m_byteWidth;
		bd.StructureByteStride = this->m_structuredByteStride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = buff;

		const D3D11_SUBRESOURCE_DATA* pInitialData = (buff != nullptr) ? &sd : nullptr;
		if (this->m_deviceInfo.m_pDevice->CreateBuffer(&bd, pInitialData, &this->m_pResource) != S_OK)
			throw M_T_CREATION_EXCEPTION();
	}
};

class IndexBufferCreationException : public WeissException { };
class IndexBufferMappingException  : public WeissException { };

class IndexBuffer : public Buffer<IndexBufferCreationException, IndexBufferMappingException> {
private:
	size_t m_nIndices;

public:
	IndexBuffer(const DeviceInfo& deviceInfo, const void* buff, const UINT nIndices, const bool isUpdatable)
    	: Buffer(deviceInfo, buff, nIndices * sizeof(uint32_t), 0u, D3D11_BIND_INDEX_BUFFER, isUpdatable),
	      m_nIndices(nIndices)
    {

    }

	template <typename CONTAINER>
	IndexBuffer(const DeviceInfo& deviceInfo, const CONTAINER& arr, const bool isUpdatable)
		: IndexBuffer(deviceInfo, arr.data(), arr.size(), isUpdatable)
	{ }

	[[nodiscard]] size_t GetIndexCount() const { return this->m_nIndices; }

	void SetData(const void* buff, const UINT nIndices) const
    {
	    this->__Buffer__SetData(buff, nIndices * sizeof(uint32_t));
    }

	template <typename CONTAINER>
	void SetData(const CONTAINER& arr) { this->SetData(arr.data(), arr.size()); }

	virtual void Bind() const noexcept override
    {
	    this->m_deviceInfo.m_pDeviceContext->IASetIndexBuffer(this->m_pResource.Get(), DXGI_FORMAT_R32_UINT, 0u);
    }
};

class VertexBufferMappingException  : public WeissException { };
class VertexBufferCreationException : public WeissException { };

class VertexBuffer : public Buffer<VertexBufferCreationException, VertexBufferMappingException> {
private:
	size_t m_nVertices;
	size_t m_vertexSize;

public:
	VertexBuffer(const DeviceInfo& deviceInfo, const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable)
    	: Buffer(deviceInfo, buff, vertexSize*nVertices, vertexSize, D3D11_BIND_VERTEX_BUFFER, isUpdatable),
	      m_nVertices(nVertices), m_vertexSize(vertexSize)
    {

    }

	template <typename CONTAINER>
	VertexBuffer(const DeviceInfo& deviceInfo, const CONTAINER& arr, const bool isUpdatable)
		: VertexBuffer(deviceInfo, arr.data(), arr.size(), sizeof(arr[0]), isUpdatable)
	{ }

	[[nodiscard]] size_t GetElementCount() const noexcept { return this->m_nVertices; }

	void SetData(const void* buff, const size_t nVertices) const
    {
        this->__Buffer__SetData(buff, nVertices * this->m_vertexSize);
    }

	template <typename CONTAINER>
	void SetData(const CONTAINER& arr) { this->SetData(arr.data(), arr.size()); }

	void Bind() const noexcept
    {
        const UINT stride = static_cast<UINT>(this->m_vertexSize);
        const UINT offset = 0u;

        this->m_deviceInfo.m_pDeviceContext->IASetVertexBuffers(0u, 1u, this->m_pResource.GetAddressOf(), &stride, &offset);
    }
};

enum class ShaderBindingType {
	VERTEX,
	PIXEL,
	BOTH
};

class ConstantBufferMapping           : public WeissException { };
class ConstantBufferCreationException : public WeissException { };

class ConstantBuffer : public Buffer<ConstantBufferCreationException, ConstantBufferMapping> {
private:
	ShaderBindingType m_bindingType;
	UINT m_slotVS;
	UINT m_slotPS;
	UINT m_objSize;

public:
	ConstantBuffer(const DeviceInfo& deviceInfo, const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS)
	    : Buffer(deviceInfo, buff, objSize, 0u, D3D11_BIND_CONSTANT_BUFFER, true),
	      m_bindingType(bindingType), m_slotVS(slotVS), m_slotPS(slotPS), m_objSize(objSize)
    {
        
    }

	void SetData(const void* objPtr) const
    {
        this->__Buffer__SetData(objPtr, this->m_objSize);
    }

	virtual void Bind() const noexcept override
    {
        if (this->m_bindingType == ShaderBindingType::VERTEX || this->m_bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->VSSetConstantBuffers(this->m_slotVS, 1u, this->m_pResource.GetAddressOf());

        if (this->m_bindingType == ShaderBindingType::PIXEL || this->m_bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->PSSetConstantBuffers(this->m_slotPS, 1u, this->m_pResource.GetAddressOf());
    }
};

class BufferManager {
protected:
	std::vector<IndexBuffer>    m_indexBuffers;
	std::vector<VertexBuffer>   m_vertexBuffers;
	std::vector<ConstantBuffer> m_constantBuffers;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitBufferManager(DeviceInfo& deviceInfo)
    {
        this->m_deviceInfo = new DeviceInfo(deviceInfo);
    }

public:
	~BufferManager()
    {
	    delete this->m_deviceInfo;
    }

	template <typename CONTAINER>
	[[nodiscard]] size_t CreateIndexBuffer(const CONTAINER& arr, const bool isUpdatable)
	{
		this->m_indexBuffers.emplace_back(*this->m_deviceInfo, arr, isUpdatable);

		return this->m_indexBuffers.size() - 1u;
	}

	[[nodiscard]] size_t CreateIndexBuffer(const void* buff, const UINT nIndices, const bool isUpdatable)
    {
        this->m_indexBuffers.emplace_back(*this->m_deviceInfo, buff, nIndices, isUpdatable);

        return this->m_indexBuffers.size() - 1u;
    }

	template <typename CONTAINER>
	[[nodiscard]] size_t CreateVertexBuffer(const CONTAINER& arr, const bool isUpdatable)
	{
		this->m_vertexBuffers.emplace_back(*this->m_deviceInfo, arr, isUpdatable);

		return this->m_vertexBuffers.size() - 1u;
	}

	[[nodiscard]] size_t CreateVertexBuffer(const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable)
    {
        this->m_vertexBuffers.emplace_back(*this->m_deviceInfo, buff, nVertices, vertexSize, isUpdatable);

        return this->m_vertexBuffers.size() - 1u;
    }

	[[nodiscard]] size_t CreateConstantBuffer(const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS)
    {
        this->m_constantBuffers.emplace_back(*this->m_deviceInfo, bindingType, buff, objSize, slotVS, slotPS);

        return this->m_constantBuffers.size() - 1u;
    }

	[[nodiscard]] IndexBuffer&    GetIndexBuffer(const size_t index)    noexcept {return this->m_indexBuffers[index];    }
	[[nodiscard]] VertexBuffer&   GetVertexBuffer(const size_t index)   noexcept {return this->m_vertexBuffers[index];   }
	[[nodiscard]] ConstantBuffer& GetConstantBuffer(const size_t index) noexcept {return this->m_constantBuffers[index]; }
};

template <typename T>
class Shader : public Bindable<T> {
protected:
	const char* m_sourceFilename;

public:
	Shader(const DeviceInfo& deviceInfo, const char* sourceFilename)
		: Bindable<T>(deviceInfo), m_sourceFilename(sourceFilename) {  }

	virtual void Load() = 0;
};

class PixelShaderCreationException : public WeissException { };

class PixelShader : public Shader<ID3D11PixelShader> {
public:
	PixelShader(const DeviceInfo& deviceInfo, const char* sourceFilename)
    	: Shader<ID3D11PixelShader>(deviceInfo, sourceFilename)
    {
        this->Load();
    }

	virtual void Load() override
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        std::ifstream fileStream(this->m_sourceFilename);
        std::string sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

        if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw PixelShaderCreationException();
        }

        if (this->m_deviceInfo.m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pResource) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw PixelShaderCreationException();
        }
    }

	virtual void Bind() const noexcept override
    {
        this->m_deviceInfo.m_pDeviceContext->PSSetShader(this->m_pResource.Get(), nullptr, 0u);
    }
};

class InputLayoutCreationException : public WeissException { };

class InputLayout : public Bindable<ID3D11InputLayout> {
public:
	InputLayout(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const Microsoft::WRL::ComPtr<ID3DBlob>& pBlob)
        : Bindable(deviceInfo)
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(ieds.size());
        for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
        {
            inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
            inputElementDescriptors[i].SemanticName = ieds[i].first;
            inputElementDescriptors[i].Format = ieds[i].second;
            inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        }

        if (this->m_deviceInfo.m_pDevice->CreateInputLayout(inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &this->m_pResource) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Input Layout");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw InputLayoutCreationException();
        }
    }

	virtual void Bind() const noexcept override
    {
	    this->m_deviceInfo.m_pDeviceContext->IASetInputLayout(this->m_pResource.Get());
    }
};

class VertexShaderCreationException : public WeissException { };

class VertexShader : public Shader<ID3D11VertexShader> {
private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;

	InputLayout* m_inputLayout;

public:
	VertexShader(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* sourceFilename)
    	: Shader<ID3D11VertexShader>(deviceInfo, sourceFilename)
    {
        this->Load();
        this->m_inputLayout = new InputLayout(deviceInfo, ieds, this->m_pBlob);
    }

	~VertexShader()
    {
        delete this->m_inputLayout;
    }

	virtual void Load() override   
    {
        std::ifstream fileStream(this->m_sourceFilename);
        std::string sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

        if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &this->m_pBlob, NULL) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw VertexShaderCreationException();
        }

        if (this->m_deviceInfo.m_pDevice->CreateVertexShader(this->m_pBlob->GetBufferPointer(), this->m_pBlob->GetBufferSize(), nullptr, &this->m_pResource) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw VertexShaderCreationException();
        }
    }

	virtual void Bind() const noexcept override
    {
        this->m_inputLayout->Bind();
        this->m_deviceInfo.m_pDeviceContext->VSSetShader(this->m_pResource.Get(), nullptr, 0u);
    }
};

class ShaderManager {
protected:
	std::vector<PixelShader>  m_pixelShaders;
	std::vector<VertexShader> m_vertexShaders;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitShaderManager(DeviceInfo& deviceInfo)
    {
	    this->m_deviceInfo = new DeviceInfo(deviceInfo);
    }

public:
	~ShaderManager()
    {
	    delete this->m_deviceInfo;
    }

	[[nodiscard]] size_t CreatePixelShader(const char* sourceFilename)
    {
        this->m_pixelShaders.emplace_back(*this->m_deviceInfo, sourceFilename);

        return this->m_pixelShaders.size() - 1u;
    }

	[[nodiscard]] size_t CreateVertexShader(const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* sourceFilename)
    {
        this->m_vertexShaders.emplace_back(*this->m_deviceInfo, ieds, sourceFilename);

        return this->m_vertexShaders.size() - 1u;
    }

	[[nodiscard]] PixelShader& GetPixelShader(const size_t index) noexcept
    {
        return this->m_pixelShaders[index];
    }

	[[nodiscard]] VertexShader& GetVertexShader(const size_t index) noexcept
    {
        return this->m_vertexShaders[index];
    }
};

class ImageLoadingException : public WeissException { };

class Image {
private:
	std::unique_ptr<Coloru8[]> m_buff;

	uint16_t m_width   = 0, m_height = 0;
	uint32_t m_nPixels = 0;

public:
	Image(const Image& img)
    	: m_width(img.m_width), m_height(img.m_height), m_nPixels(img.m_nPixels)
    {
        this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

        std::memcpy(this->m_buff.get(), img.m_buff.get(), this->m_nPixels * sizeof(Coloru8));
    }

	Image(const uint16_t width, const uint16_t height, Coloru8 fillColor = { 255u,255u,255u,255u })
    	: m_width(width), m_height(height), m_nPixels(width * height)
    {
        this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

        std::fill_n(this->m_buff.get(), this->m_nPixels, fillColor);
    }

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

	[[nodiscard]] uint16_t GetWidth()                                 const { return this->m_width;					      }
	[[nodiscard]] uint16_t GetHeight()                                const { return this->m_height;					  }
	[[nodiscard]] uint32_t GetPixelCount()                            const { return this->m_nPixels;					  }
	[[nodiscard]] Coloru8* GetBuffer()                                const { return this->m_buff.get();				  }
	[[nodiscard]] Coloru8  Sample(const uint16_t x, const uint16_t y) const { return this->m_buff[y * this->m_width + x]; }

	void Set(const uint16_t x, const uint16_t y, const Coloru8& color) { this->m_buff[y * this->m_width + x] = color; }
};

class SpriteSheet2DCreationException : public WeissException { };

struct SpriteSheet2DDescriptor
{
	std::vector<std::pair<std::string, Image>> sprites;
};

class SpriteSheet2D : public Image {
private:
	uint16_t m_spriteWidth = 0u, m_spriteHeight = 0u;

	std::unordered_map<std::string, Vec2u16> m_spriteLocations;

public:
	SpriteSheet2D(const SpriteSheet2DDescriptor& descriptor)
    	: Image(WEISS_SPRITE_SHEET_SIDE_LENGTH, WEISS_SPRITE_SHEET_SIDE_LENGTH)
    {
        this->m_spriteWidth  = descriptor.sprites[0].second.GetWidth();
        this->m_spriteHeight = descriptor.sprites[0].second.GetHeight();

        uint16_t spriteIndex = 0u;

        for (uint16_t y = 0u; y <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteHeight; y += this->m_spriteHeight)
        {
            for (uint16_t x = 0u; x <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteWidth; x += this->m_spriteWidth)
            {
                if (spriteIndex == descriptor.sprites.size())
                    break;

                this->m_spriteLocations.insert({ descriptor.sprites[spriteIndex].first, Vec2u16{ x, y } });

                for (uint16_t x2 = x; x2 < x + this->m_spriteWidth; x2++)
                    for (uint16_t y2 = y; y2 < y + this->m_spriteHeight; y2++)
                        this->Set(x2, y2, descriptor.sprites[spriteIndex].second.Sample(x2 - x, y2 - y));

                spriteIndex++;
            }
        }
    }

	[[nodiscard]] uint16_t GetSpriteWidth()  const noexcept { return this->m_spriteWidth;  }
	[[nodiscard]] uint16_t GetSpriteHeight() const noexcept { return this->m_spriteHeight; }
	[[nodiscard]] Vec2u16  GetSpriteLocation(const std::string& spriteName) const noexcept { return this->m_spriteLocations.at(spriteName); }
};


struct TextureSamplerDescriptor
{
	ShaderBindingType bindingType;
	UINT slotVS;
	UINT slotPS;
	D3D11_FILTER filter;
	D3D11_TEXTURE_ADDRESS_MODE addressMode;
};

class TextureSampler {
private:
	const DeviceInfo& m_deviceInfo;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;

	TextureSamplerDescriptor m_descriptor;

public:
	TextureSampler(const DeviceInfo& deviceInfo, const TextureSamplerDescriptor& descriptor)
    	: m_deviceInfo(deviceInfo), m_descriptor(descriptor)
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

        this->m_deviceInfo.m_pDevice->CreateSamplerState(&samplerDescriptor, &this->m_pSamplerState);
    }

	void Bind() const noexcept
    {
        if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->VSSetSamplers(this->m_descriptor.slotVS, 1u, this->m_pSamplerState.GetAddressOf());

        if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->PSSetSamplers(this->m_descriptor.slotPS, 1u, this->m_pSamplerState.GetAddressOf());
    }
};

class Texture2DCreationException : public WeissException { };

struct Texture2DSettings {
	ShaderBindingType bindingType;
	UINT slotVS;
	UINT slotPS;
	bool useMipMaps;
};

struct Texture2DDescriptor : Texture2DSettings
{
	Image image;
};

class Texture2D : public Bindable<ID3D11ShaderResourceView> {
private:
	Texture2DSettings m_settings;

public:
	Texture2D(const DeviceInfo& deviceInfo, const Texture2DDescriptor& descriptor)
    	: Bindable(deviceInfo), m_settings(descriptor)
    {
        D3D11_SUBRESOURCE_DATA* subResourceData = new D3D11_SUBRESOURCE_DATA();

        subResourceData->pSysMem          = static_cast<void*>(descriptor.image.GetBuffer());
        subResourceData->SysMemPitch      = descriptor.image.GetWidth() * sizeof(Coloru8);
        subResourceData->SysMemSlicePitch = 0;

        D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
        texture2DDescriptor.Width = descriptor.image.GetWidth();
        texture2DDescriptor.Height = descriptor.image.GetHeight();
        texture2DDescriptor.MipLevels = this->m_settings.useMipMaps ? 0 : 1;
        texture2DDescriptor.ArraySize = 1u;
        texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texture2DDescriptor.SampleDesc.Count   = 1u;
        texture2DDescriptor.SampleDesc.Quality = 0;
        texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
        texture2DDescriptor.BindFlags = this->m_settings.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
        texture2DDescriptor.MiscFlags = this->m_settings.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

        if (this->m_deviceInfo.m_pDevice->CreateTexture2D(&texture2DDescriptor, this->m_settings.useMipMaps ? nullptr : subResourceData, &texture2D) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Texture 2D");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw Texture2DCreationException();
        }

        if (this->m_settings.useMipMaps)
            this->m_deviceInfo.m_pDeviceContext->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData->pSysMem, subResourceData->SysMemPitch, 0u);

        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
        SRVDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        SRVDescriptor.Texture2D.MostDetailedMip = 0;
        SRVDescriptor.Texture2D.MipLevels = this->m_settings.useMipMaps ? -1 : 1;

        if (this->m_deviceInfo.m_pDevice->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pResource.GetAddressOf()) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create ShaderResourceView");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw Texture2DCreationException();
        }

        if (this->m_settings.useMipMaps)
            this->m_deviceInfo.m_pDeviceContext->GenerateMips(this->m_pResource.Get());
    }

	virtual void Bind() const noexcept override
    {
        if (this->m_settings.bindingType == ShaderBindingType::VERTEX || this->m_settings.bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->VSSetShaderResources(this->m_settings.slotVS, 1u, this->m_pResource.GetAddressOf());

        if (this->m_settings.bindingType == ShaderBindingType::PIXEL || this->m_settings.bindingType == ShaderBindingType::BOTH)
            this->m_deviceInfo.m_pDeviceContext->PSSetShaderResources(this->m_settings.slotPS, 1u, this->m_pResource.GetAddressOf());
    }
};

struct ImageTexturePair {
	std::variant<Image, SpriteSheet2D> image;
	Texture2DSettings settings;
	std::vector<Texture2D*> textures;
};

class TextureManager {
protected:
	std::vector<TextureSampler> m_textureSamplers;
	std::unordered_map<std::string, ImageTexturePair> m_imageTexturePairs;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitTextureManager(DeviceInfo& deviceInfo)
    {
        this->m_deviceInfo = new DeviceInfo(deviceInfo);
    }

public:
	~TextureManager()
    {
        delete this->m_deviceInfo;
    }

	template <typename T>
	void GenerateTexture(const std::string& textureName, const size_t textureIndex = 0u)
	{
		ImageTexturePair& pair = this->m_imageTexturePairs[textureName];
		Texture2DDescriptor t2dd{ pair.settings, std::get<T>(pair.image) };

		delete pair.textures[textureIndex];
		pair.textures[textureIndex] = new Texture2D(*this->m_deviceInfo, t2dd);
	}

	template <typename T>
	[[nodiscard]] void AddImageResource(const std::string& textureName, const Texture2DSettings& settings, const T& image)
	{
		Texture2DDescriptor t2dd{ settings, image };
		ImageTexturePair itp = { image, settings };

		itp.textures.push_back(new Texture2D(*this->m_deviceInfo, t2dd));

		this->m_imageTexturePairs.insert({ textureName, itp });
	}
	
	[[nodiscard]] size_t CreateTextureSampler(const TextureSamplerDescriptor& descriptor)
    {
        this->m_textureSamplers.emplace_back(*this->m_deviceInfo, descriptor);

        return this->m_textureSamplers.size() - 1u;
    }

	template <typename T>
	[[nodiscard]] T& GetImage(const std::string& textureName) noexcept
	{
		return std::get<T>(this->m_imageTexturePairs.at(textureName).image);
	}

	[[nodiscard]] Texture2D& GetTexture(const std::string& textureName, const size_t textureIndex = 0u) noexcept
    {
        return *this->m_imageTexturePairs.at(textureName).textures[textureIndex];
    }

	[[nodiscard]] TextureSampler& GetTextureSampler(const size_t index) noexcept
    {
        return this->m_textureSamplers[index];
    }
};

class LowLevelGraphicsInitializerException : public WeissException { };

class LowLevelGraphicsInitializer
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Device>            m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11BlendState>        m_pBlendState;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOn;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOff;

private:
	void CreateDeviceAndSwapChain(Window* window)
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
        scd.OutputWindow = window->GetHandle();
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Device And SwapChain");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw LowLevelGraphicsInitializerException();
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

            throw LowLevelGraphicsInitializerException();
        }

        if (this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create RenderTargetView");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw LowLevelGraphicsInitializerException();
        }
    }

	void CreateViewport(Window* window)
    {
        D3D11_VIEWPORT vp;
        vp.Width  = static_cast<FLOAT>(window->GetClientWidth());
        vp.Height = static_cast<FLOAT>(window->GetClientHeight());
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

            throw LowLevelGraphicsInitializerException();
        }

        dsDesc.DepthEnable = FALSE;

        if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOff) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create EmptyDepthStencilState");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw LowLevelGraphicsInitializerException();
        }

        this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
    }

	void CreateDepthStencil(Window* window)  
    {
        // Create Depth Texture
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
        D3D11_TEXTURE2D_DESC descDepth = {};
        descDepth.Width  = window->GetClientWidth();
        descDepth.Height = window->GetClientHeight();
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

            throw LowLevelGraphicsInitializerException();
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

            throw LowLevelGraphicsInitializerException();
        }

        this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
    }

	void CreateAndBindBlendState()
    {
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        if (this->m_pDevice->CreateBlendState(&blendDesc, &this->m_pBlendState) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could not create Blend State");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw LowLevelGraphicsInitializerException();
        }

        this->m_pDeviceContext->OMSetBlendState(this->m_pBlendState.Get(), nullptr, 0xFFFFFFFFu);
    }

protected:
	void InitializeLowLevelGraphics(Window* window)
    {
        if (CoInitialize(NULL) != S_OK)
        {
#ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Initialize COM");
#endif // __WEISS_SHOW_DEBUG_ERRORS
        }

        this->CreateDeviceAndSwapChain(window);
        this->CreateRenderTarget();
        this->CreateViewport(window);
        this->CreateDepthStencilStates();
        this->CreateDepthStencil(window);
        this->CreateAndBindBlendState();
    }

	DeviceInfo GetDeviceInfo() noexcept { return DeviceInfo { this->m_pDevice, this->m_pDeviceContext }; }
};


class LowLevelRenderer : public TextureManager, public ShaderManager, public BufferManager, public LowLevelGraphicsInitializer {
private:
	Window* m_window;
	
	DeviceInfo* m_deviceInfo;

	std::vector<Drawable> drawables;

public:
	void InitializeLowLevelRenderer(Window* window)
    {
        this->m_window = window;

        this->InitializeLowLevelGraphics(this->m_window);

        this->m_deviceInfo = new DeviceInfo(this->GetDeviceInfo());

        this->InitBufferManager(*this->m_deviceInfo);
        this->InitShaderManager(*this->m_deviceInfo);
        this->InitTextureManager(*this->m_deviceInfo);
    }

	~LowLevelRenderer()
    {
        delete this->m_deviceInfo;
    }

	void Run(const bool useVSync = true, const uint16_t fps = 60)
    {
        Timer timer;
        uint32_t frames = 0;

        while (this->m_window->IsRunning())
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

            this->m_window->Update();

            this->OnRender(elapsed);

            this->PresentFrame(useVSync);
        }
    }

	virtual void OnRender(const float elapsed) = 0;

	void Draw(const size_t drawableIndex, UINT count = 0u)
    {
        Drawable& drawable = this->drawables[drawableIndex];

        if (drawable.vertexBufferIndex == WEISS_NO_RESOURCE_INDEX) return;

        this->m_vertexBuffers[drawable.vertexBufferIndex].Bind();
        this->m_vertexShaders[drawable.vertexShaderIndex].Bind();
        this->m_pixelShaders[drawable.pixelShaderIndex].Bind();

        for (const std::string& textureName : drawable.textureNames)
            this->m_imageTexturePairs.at(textureName).textures[0]->Bind();

        for (const size_t textureSamplerIndex : drawable.textureSamplerIndices)
            this->m_textureSamplers[textureSamplerIndex].Bind();

        for (const size_t cbIndex : drawable.constantBufferIndices)
            this->m_constantBuffers[cbIndex].Bind();

        this->m_pDeviceContext->IASetPrimitiveTopology(drawable.primitiveTopologyType);

        if (drawable.indexBufferIndex.has_value())
        {
            this->m_indexBuffers[drawable.indexBufferIndex.value()].Bind();

            if (count == 0u)
                count = static_cast<UINT>(this->m_indexBuffers[drawable.indexBufferIndex.value()].GetIndexCount());

            this->m_pDeviceContext->DrawIndexed(count, 0u, 0u);
        }
        else
        {
            if (count == 0u)
                count = static_cast<UINT>(this->m_vertexBuffers[drawable.vertexBufferIndex].GetElementCount());

            this->m_pDeviceContext->Draw(count, 0u);
        }
    }

	void PresentFrame(const bool useVSync)
    {
        if (this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u) != S_OK)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Present Frame");
    #endif

            throw LowLevelGraphicsInitializerException();
        }

        // Clear Depth Stencil
        this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

	void TurnZBufferOn()  noexcept
    {
	    this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
    }

	void TurnZBufferOff() noexcept
    {
        this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOff.Get(), 1u);
    }

	[[nodiscard]] size_t AddDrawable(const Drawable& drawable)
    {
        this->drawables.push_back(drawable);

        return this->drawables.size() - 1;
    }

	[[nodiscard]] Drawable& GetDrawable(const size_t index) noexcept { return this->drawables[index]; }

	[[nodiscard]] LowLevelRenderer& GetLowLevelRenderer() noexcept { return *this; }
};

class Camera {
protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	size_t m_transformConstantBufferIndex = 0;

public:
	Camera()
    {

    }

	[[nodiscard]] DirectX::XMMATRIX GetTransform()           const noexcept { return this->m_transform; }
	[[nodiscard]] DirectX::XMMATRIX GetTransposedTransform() const noexcept { return DirectX::XMMatrixTranspose(this->m_transform); }

	[[nodiscard]] Vec3f GetPosition() const noexcept { return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] }; }
	[[nodiscard]] Vec3f GetRotation() const noexcept { return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] }; }

	virtual void CalculateTransform() = 0;
};

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
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
        if (keyboard.IsKeyDown(backward))
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

        if (keyboard.IsKeyDown(right))
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
        if (keyboard.IsKeyDown(left))
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

        if (keyboard.IsKeyDown(up))
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
        if (keyboard.IsKeyDown(down))
            delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

        return Vec3f{ delta.m128_f32[0], delta.m128_f32[1], delta.m128_f32[2] };
    }
};

struct OrthographicCameraDescriptor
{
	const Vec2f position;
	const float ratation;
};

class OrthographicCamera : public Camera {
private:
	float m_InvAspectRatio = 0.0f;

	Window& m_window;

public:
	OrthographicCamera(Window& window, const OrthographicCameraDescriptor& descriptor)
    	: m_window(window)
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

	void Rotate(const float angle)
    {
        this->m_rotation.m128_f32[2] += angle;
    }

	void SetPosition(const Vec2f& v)
    {
        this->m_position = DirectX::XMVectorSet(v.x, v.y, 0.0f, 0.0f);
    }

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
        this->m_transform = DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1, 1) *
                            DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) *
                            DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]);
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

class HighLevelRendererException : std::exception { };

struct HighLevelRenderer2DDescriptor
{
	const OrthographicCameraDescriptor orthographicCameraDesc;
};

struct HighLevelRenderer3DDescriptor
{
	const PerspectiveCameraDescriptor perspectiveCameraDesc;
};

struct HighLevelRendererDescriptor
{
	const HighLevelRenderer2DDescriptor renderer2DDesc;
	const HighLevelRenderer3DDescriptor renderer3DDesc;
};

class HighLevelRenderer : public LowLevelRenderer
{
private:
	Window* m_window;

	OrthographicCamera* m_orthographicCamera = nullptr;
	PerspectiveCamera*  m_perspectiveCamera  = nullptr;

private:
	void InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc)
    {
        this->m_orthographicCamera = new OrthographicCamera(*this->m_window, desc.orthographicCameraDesc);
    }

	void InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc)
    {
        this->m_perspectiveCamera = new PerspectiveCamera(*this->m_window, desc.perspectiveCameraDesc);
    }

public:
	~HighLevelRenderer()
    {
        delete this->m_perspectiveCamera;
        delete this->m_orthographicCamera;
    }

	void InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, Window* window)
    {
        this->m_window = window;

        this->InitializeLowLevelRenderer(this->m_window);

        if (this->CreateConstantBuffer(ShaderBindingType::VERTEX, nullptr, sizeof(DirectX::XMMATRIX), WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT, 0u) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
    #endif // __WEISS_SHOW_DEBUG_ERRORS

            throw HighLevelRendererException();
        }

        this->GetCameraConstantBuffer().Bind();
        this->TurnZBufferOn();

        this->InitializeHighLevelRenderer2D(desc.renderer2DDesc);
        this->InitializeHighLevelRenderer3D(desc.renderer3DDesc);
    }

	void Fill(const Coloru8& color)
    {
        float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

        this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
    }

	void UpdateCameraConstantBuffer(const Camera* cameraPtr) noexcept
    {
        this->GetCameraConstantBuffer().SetData(&cameraPtr->GetTransposedTransform());
    }

	[[nodiscard]] ConstantBuffer&     GetCameraConstantBuffer() noexcept { return this->GetConstantBuffer(WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX); }
	[[nodiscard]] HighLevelRenderer&  GetHighLevelRenderer()    noexcept { return *this; }
	[[nodiscard]] PerspectiveCamera&  GetPerspectiveCamera()    noexcept { return *this->m_perspectiveCamera;  }
	[[nodiscard]] OrthographicCamera& GetOrthographicCamera()   noexcept { return *this->m_orthographicCamera; }
};

struct EngineDescriptor
{
	const WindowDescriptor windowDesc;
	const HighLevelRendererDescriptor highLevelRendererDesc;
};

class EngineInitializationException : public WeissException { };

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

class Weiss : public HighLevelRenderer {
protected:
	Window* m_window = nullptr;

public:
	Weiss()
    {
        
    }

	void InitializeWeiss(const EngineDescriptor& desc)
    {
        this->m_window = &Window::Create(desc.windowDesc);

        this->InitializeHighLevelRenderer(desc.highLevelRendererDesc, this->m_window);
    }

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

        this->m_window->GetMouse().Clip(boundingRect);

        // Hide Cursor
        this->m_window->GetMouse().Hide();
    }

	void PlayWavFile(const char* filename)
    {
        if (!PlaySoundA(filename, NULL, SND_ASYNC | SND_FILENAME))
        {
    #ifdef __WEISS_SHOW_DEBUG_ERRORS
            MESSAGE_BOX_ERROR("Could Not Play Sound From File");
    #endif // __WEISS_SHOW_DEBUG_ERRORS
        }
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

	[[nodiscard]] Mouse&    GetMouse()    noexcept { return this->m_window->GetMouse(); }
	[[nodiscard]] Window&   GetWindow()   noexcept { return *this->m_window; }
	[[nodiscard]] Weiss&    GetWeiss()    noexcept { return *this; }
	[[nodiscard]] Keyboard& GetKeybaord() noexcept { return this->m_window->GetKeyboard(); }
};

enum class ClientSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	CONNECTION_FAILED = 1
};

class ClientSocketCreationException : public WeissException
{
private:
	const ClientSocketCreationExceptionErrorType m_errorType;

public:
	ClientSocketCreationException(const ClientSocketCreationExceptionErrorType errorType)
        : m_errorType(errorType) { }

	ClientSocketCreationExceptionErrorType getErrorType() const { return this->m_errorType; }
};

class ClientSocketReceiveException : public WeissException { };

class ClientSocketSendException : public WeissException { };

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
            length = static_cast<int>(strlen(data) + 1u);

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

class ServerSocketReceiveException : public WeissException { };
class ServerSocketSendException    : public WeissException { };

enum class ServerSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	BINDING_FAILED = 1,
	LISTENING_FAILED = 2
};

class ServerSocketCreationException : public WeissException
{
private:
	const ServerSocketCreationExceptionErrorType m_errorType;

public:
	ServerSocketCreationException(const ServerSocketCreationExceptionErrorType errorType)
    	: m_errorType(errorType) { }

	ServerSocketCreationExceptionErrorType getErrorType() const { return this->m_errorType; }
};

class ServerSocket {
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
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(5555);

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

            return static_cast<int>(this->m_clients.size() - 1u);
        }

        return -1;
    }

	void Send(const int clientID, const char* data, int length = -1)
    {
        if (length < 0)
            length = static_cast<int>(strlen(data) + 1);

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

#endif