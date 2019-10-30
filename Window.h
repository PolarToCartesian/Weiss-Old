#pragma once

enum class Event
{
    WINDOW_CREATED,
    WINDOW_RESIZED,
    WINDOW_CLOSED,
    WINDOW_MOVED,
    CURSOR_MOVED,
    MOUSE_WHEEL_ROTATING,
    LEFT_MOUSE_BUTTON_CLICKED,
    RIGHT_MOUSE_BUTTON_CLICKED,
    LEFT_MOUSE_BUTTON_DOWN,
    LEFT_MOUSE_BUTTON_UP,
    RIGHT_MOUSE_BUTTON_DOWN,
    RIGHT_MOUSE_BUTTON_UP
};

#include "Types.h"

#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

#include <Windows.h>
#include <Windowsx.h>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

class Window
{
private:
    HWND m_windowHwnd;

    Vec2u m_windowPosition{0, 0};
    Vec2u m_windowDimensions{0, 0};

    bool m_isRunning = false;

    Vec2u m_mousePosition{0, 0};
    Vec2i m_mouseDeltaPosition{0, 0};

    int16_t m_mouseWheelDelta = 0;

    bool m_isLeftMouseButtonDown = false;
    bool m_isRightMouseButtonDown = false;

    // All Events
    std::vector<Event> m_windowEvents;

    // All Events That Are Not Catched By The WinAPI (they will be added to m_events)
    std::vector<Event> m_nonWinAPItriggeredWindowEvents;

public:
    Window(const uint16_t width, const uint16_t height, const char *title, HINSTANCE hInstance)
    {
        this->m_windowDimensions = {width, height};

        const WNDCLASS wc{CS_HREDRAW | CS_VREDRAW, WindowProcessMessages, 0, 0, hInstance, NULL, LoadCursor(nullptr, IDC_ARROW), (HBRUSH)COLOR_WINDOW, NULL, "A"};

        if (RegisterClass(&wc))
        {
            this->m_windowHwnd = CreateWindow("A", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, nullptr, nullptr);

            ShowWindow(this->m_windowHwnd, SW_SHOW);
            UpdateWindow(this->m_windowHwnd);

            this->m_isRunning = true;
        }
    }

    // Getters
    inline uint16_t getWidth() const { return this->m_windowDimensions[0]; }
    inline uint16_t getHeight() const { return this->m_windowDimensions[1]; }
    inline Vec2u getDimensions() const { return this->m_windowDimensions; }
    inline bool isRunning() const { return this->m_isRunning; }
    inline uint16_t getWindowPositionX() const { return this->m_windowPosition[0]; }
    inline uint16_t getWindowPositionY() const { return this->m_windowPosition[1]; }
    inline Vec2u getWindowPosition() const { return this->m_windowPosition; }
    inline Vec2u getMousePosition() const { return this->m_mousePosition; }
    inline Vec2i getMouseDeltaPosition() const { return this->m_mouseDeltaPosition; }
    inline int16_t getMouseWheelDelta() const { return this->m_mouseWheelDelta; }
    inline HWND getHWND() const { return this->m_windowHwnd; }

    // Setters
    inline void setWidth(const uint16_t width)   { this->setSize(width, this->m_windowDimensions[1]);  }
    inline void setHeight(const uint16_t height) { this->setSize(this->m_windowDimensions[0], height); }

    inline void setSize(const uint16_t width, const uint16_t height)
    {
        this->m_windowPosition[0] = width;
        this->m_windowPosition[1] = height;

        SetWindowPos(this->m_windowHwnd, NULL, 0, 0, this->m_windowPosition[0], this->m_windowPosition[1], SWP_NOMOVE | SWP_NOOWNERZORDER);

        this->m_nonWinAPItriggeredWindowEvents.emplace_back(Event::WINDOW_RESIZED);
    }

    inline void setPositionX(const uint16_t x) { this->setPosition(x, this->m_windowPosition[1]); }
    inline void setPositionY(const uint16_t y) { this->setPosition(this->m_windowPosition[0], y); }

    inline void setPosition(const uint16_t x, const uint16_t y)
    {
        this->m_windowPosition = {x, y};
        SetWindowPos(this->m_windowHwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        this->m_nonWinAPItriggeredWindowEvents.emplace_back(Event::WINDOW_MOVED);
    }

    std::vector<Event> &pullEvents()
    {
        // Reset Some Values
        this->m_mouseWheelDelta = 0;

        // Set m_events equal to m_nonWinAPItriggeredEvents
        this->m_windowEvents.clear();
        this->m_windowEvents.insert(this->m_windowEvents.end(), this->m_nonWinAPItriggeredWindowEvents.begin(), this->m_nonWinAPItriggeredWindowEvents.end());
        std::copy(this->m_nonWinAPItriggeredWindowEvents.begin(), this->m_nonWinAPItriggeredWindowEvents.end(), this->m_windowEvents.begin());
        this->m_nonWinAPItriggeredWindowEvents.clear();

        // Remember Previous State
        const bool wasLeftMouseButtonDown = this->m_isLeftMouseButtonDown;
        const bool wasRightMouseButtonDown = this->m_isRightMouseButtonDown;

        // Handle Messages
        MSG msg;

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Add Extra Events
        this->m_windowEvents.emplace_back(this->m_isLeftMouseButtonDown ? Event::LEFT_MOUSE_BUTTON_DOWN : Event::LEFT_MOUSE_BUTTON_UP);
        this->m_windowEvents.emplace_back(this->m_isRightMouseButtonDown ? Event::RIGHT_MOUSE_BUTTON_DOWN : Event::RIGHT_MOUSE_BUTTON_UP);

        if (wasLeftMouseButtonDown && !this->m_isLeftMouseButtonDown)
        {
            this->m_windowEvents.emplace_back(Event::LEFT_MOUSE_BUTTON_CLICKED);
        }

        if (wasRightMouseButtonDown && !this->m_isRightMouseButtonDown)
        {
            this->m_windowEvents.emplace_back(Event::RIGHT_MOUSE_BUTTON_CLICKED);
        }

        // Remove Duplicates
        std::sort(this->m_windowEvents.begin(), this->m_windowEvents.end());
        this->m_windowEvents.erase(std::unique(this->m_windowEvents.begin(), this->m_windowEvents.end()), this->m_windowEvents.end());

        return this->m_windowEvents;
    }

    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CREATE:
            this->m_windowEvents.emplace_back(Event::WINDOW_CREATED);
            break;
        case WM_MOVE:
            this->m_windowPosition = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            this->m_windowEvents.emplace_back(Event::WINDOW_MOVED);
            break;
        case WM_SIZE:
            this->m_windowDimensions = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            this->m_windowEvents.emplace_back(Event::WINDOW_RESIZED);
            break;
        case WM_MOUSEMOVE:
        {
            this->m_mouseDeltaPosition = {
                static_cast<int16_t>(static_cast<int16_t>(GET_X_LPARAM(lParam)) - static_cast<int16_t>(this->m_mousePosition[0])),
                static_cast<int16_t>(static_cast<int16_t>(GET_Y_LPARAM(lParam)) - static_cast<int16_t>(this->m_mousePosition[1]))
            };

            this->m_mousePosition = {(uint16_t)GET_X_LPARAM(lParam), (uint16_t)GET_Y_LPARAM(lParam)};
            this->m_windowEvents.emplace_back(Event::CURSOR_MOVED);
        }
        break;
        case WM_LBUTTONDOWN:
            this->m_isLeftMouseButtonDown = true;
            break;
        case WM_LBUTTONUP:
            this->m_isLeftMouseButtonDown = false;
            break;
        case WM_RBUTTONDOWN:
            this->m_isRightMouseButtonDown = true;
            break;
        case WM_RBUTTONUP:
            this->m_isRightMouseButtonDown = false;
            break;
        case WM_MOUSEWHEEL:
            this->m_mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            this->m_windowEvents.emplace_back(Event::MOUSE_WHEEL_ROTATING);
            break;
        case WM_DESTROY:
            this->destroy();
            this->m_windowEvents.emplace_back(Event::WINDOW_CLOSED);
            break;
        default:
            return DefWindowProc(this->m_windowHwnd, msg, wParam, lParam);
        }
    }

    void destroy() { this->m_isRunning = !DestroyWindow(this->m_windowHwnd); }

    ~Window() { this->destroy(); }
};

namespace WindowHandler
{
    std::vector<Window> windows;

    uint8_t addWindow(const uint16_t width, const uint16_t height, const char *title, HINSTANCE hInstance)
    {
        windows.emplace_back(width, height, title, hInstance);

        return windows.size() - 1;
    }
}; // namespace WindowHandler

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::vector<Window>::iterator it = std::find_if(WindowHandler::windows.begin(), WindowHandler::windows.end(), [&hwnd](const Window &window) {
        return window.getHWND() == hwnd;
    });

    if (it != WindowHandler::windows.end())
        return it->handleMessage(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}