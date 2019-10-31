#pragma once

#include "devices/Mouse.h"
#include "devices/Keyboard.h"

#include <vector>
#include <Windowsx.h>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

class Window
{
    friend LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

    private:
        HWND m_handle;
        HINSTANCE m_hinstance;

        Vec2u m_position{0, 0};
        Vec2u m_dimensions{0, 0};

        bool m_isRunning = false;

    public:
        Mouse mouse;
        Keyboard keyboard;

    public:
        Window(const uint16_t width, const uint16_t height, const char *title, HINSTANCE hInstance)
        {
            this->m_dimensions = {width, height};
            this->m_hinstance  = hInstance;

            const WNDCLASSA wc { 
                CS_HREDRAW | CS_VREDRAW,
                WindowProcessMessages,
                0,
                0,
                hInstance,
                NULL,
                LoadCursor(nullptr, IDC_ARROW),
                (HBRUSH)COLOR_WINDOW, NULL,
                "WNDCLASSA"
            };

            if (RegisterClassA(&wc))
            {
                this->m_handle = CreateWindowA("WNDCLASSA", title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hInstance, NULL);

                ShowWindow(this->m_handle, SW_SHOW);
                UpdateWindow(this->m_handle);

                this->m_isRunning = true;
            }
        }

        // Getters
        inline uint16_t getWidth()           const { return this->m_dimensions[0]; }
        inline uint16_t getHeight()          const { return this->m_dimensions[1]; }
        inline Vec2u    getDimensions()      const { return this->m_dimensions;    }
        inline bool     isRunning()          const { return this->m_isRunning;     }
        inline uint16_t getWindowPositionX() const { return this->m_position[0];   }
        inline uint16_t getWindowPositionY() const { return this->m_position[1];   }
        inline Vec2u    getWindowPosition()  const { return this->m_position;      }

        // Setters
        inline void setWidth(const uint16_t width)   { this->setSize(width, this->m_dimensions[1]);  }
        inline void setHeight(const uint16_t height) { this->setSize(this->m_dimensions[0], height); }

        void setSize(const uint16_t width, const uint16_t height)
        {
            this->m_dimensions = { width, height };

            SetWindowPos(this->m_handle, NULL, 0, 0, this->m_dimensions[0], this->m_dimensions[1], SWP_NOMOVE | SWP_NOOWNERZORDER);
        }

        inline void setPositionX(const uint16_t x) { this->setPosition(x, this->m_position[1]); }
        inline void setPositionY(const uint16_t y) { this->setPosition(this->m_position[0], y); }

        void setPosition(const uint16_t x, const uint16_t y)
        {
            this->m_position = { x, y };
            SetWindowPos(this->m_handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        }

        void setTitle(const char* title)
        {
            SetWindowTextA(this->m_handle, title);
        }
        
        // MESSAGE HANDLING
        void update()
        {
            std::cout << "--------------------------------------------\n";
            this->mouse.__onWindowUpdateBegin();
            this->keyboard.__onWindowUpdateBegin();

            MSG msg;

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            this->mouse.__onWindowUpdateEnd();
            this->keyboard.__onWindowUpdateEnd();
        }

        LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
                case WM_MOVE:
                    this->m_position   = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                    return 0;
                case WM_SIZE:
                    this->m_dimensions = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                    return 0;
                case WM_DESTROY:
                    this->destroy();
                    return 0;
            }

            // Dispatch Message To Peripheral Devices
            if (this->mouse.__handleMessage(msg, wParam, lParam)) return 0;
            if (this->keyboard.__handleMessage(msg, wParam, lParam)) return 0;

            // Otherwise Let Windows Handle The Message
            return DefWindowProc(this->m_handle, msg, wParam, lParam);
        }

        // DESTROYING
        void destroy() { this->m_isRunning = !DestroyWindow(this->m_handle); }

        ~Window()
        {
            this->destroy();
        }
};

namespace WindowHandler
{
    std::vector<Window> windows;

    Window& createWindow(const uint16_t width, const uint16_t height, const char* title, HINSTANCE hInstance)
    {
        windows.emplace_back(width, height, title, hInstance);

        return windows[windows.size() - 1];
    }
}; // namespace WindowHandler

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::vector<Window>::iterator it = std::find_if(WindowHandler::windows.begin(), WindowHandler::windows.end(), [&hwnd](const Window &window) {
        return window.m_handle == hwnd;
    });

    if (it != WindowHandler::windows.end())
        return it->handleMessage(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}