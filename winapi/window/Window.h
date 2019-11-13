#pragma once

#include "peripherals/Mouse.h"
#include "../graphics/Graphics.h"
#include "peripherals/Keyboard.h"

#include <vector>
#include <memory>
#include <functional>
#include <Windowsx.h>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

struct WindowDescriptor
{
	uint16_t windowPositionX, windowPositionY;
	uint16_t windowWidth, windowHeight;
	char* title;
	bool isResizable;
	HINSTANCE hInstance;
};

class Window
{
    friend LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

    private:
        HWND m_handle = 0;
        HINSTANCE m_hinstance;

		Mouse m_mouse;
		Keyboard m_keyboard;

        Vec2u m_position{0, 0};
        Vec2u m_dimensions{0, 0};

        bool m_isRunning   = false;
		bool m_isMinimized = false;

        std::function<void(const Vec2u)> m_onResizeFunctor = [](const Vec2u dim) {};

    public:
        Window(const WindowDescriptor descriptor)
        {
            this->m_dimensions = { descriptor.windowWidth, descriptor.windowHeight };
            this->m_hinstance  = descriptor.hInstance;

            const WNDCLASSA wc { 
                CS_HREDRAW | CS_VREDRAW,
                WindowProcessMessages,
                0,
                0,
                this->m_hinstance,
                NULL,
                LoadCursor(nullptr, IDC_ARROW),
                (HBRUSH)COLOR_WINDOW, NULL,
                "WNDCLASSA"
            };

            if (RegisterClassA(&wc))
            {
                const uint32_t windowStyle = descriptor.isResizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

                this->m_handle = CreateWindowA("WNDCLASSA", descriptor.title, windowStyle, 0, 0, descriptor.windowWidth, descriptor.windowHeight, NULL, NULL, descriptor.hInstance, NULL);

                ShowWindow(this->m_handle, SW_SHOW);
                UpdateWindow(this->m_handle);

				this->setPosition(descriptor.windowPositionX, descriptor.windowPositionY);

                this->m_isRunning = true;
            }
        }

        // Misc
        void onResize(const std::function<void(const Vec2u)>& functor) { this->m_onResizeFunctor = functor; }

		std::unique_ptr<Graphics> createGraphics() { return std::make_unique<Graphics>(this->m_handle); }

        // Getters
        inline uint16_t  getWidth()           const { return this->m_dimensions[0]; }
        inline uint16_t  getHeight()          const { return this->m_dimensions[1]; }
		inline uint16_t  getDimensionW()      const { return this->m_dimensions[0]; }
		inline uint16_t  getDimensionH()      const { return this->m_dimensions[1]; }
        inline Vec2u     getDimensions()      const { return this->m_dimensions;    }
        inline bool      isRunning()          const { return this->m_isRunning;     }
        inline uint16_t  getPositionX()       const { return this->m_position[0];   }
        inline uint16_t  getPositionY()       const { return this->m_position[1];   }
        inline Vec2u     getPosition()        const { return this->m_position;      }
			   Keyboard& getKeyboard()              { return this->m_keyboard; }
			   Mouse&    getMouse()                 { return this->m_mouse;         }

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

        void setTitle(const char* title) { SetWindowTextA(this->m_handle, title); }

        // MESSAGE HANDLING
        void update()
        {
            this->m_mouse.__onWindowUpdateBegin();
            this->m_keyboard.__onWindowUpdateBegin();

            MSG msg;

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            this->m_mouse.__onWindowUpdateEnd();
            this->m_keyboard.__onWindowUpdateEnd();
        }

        LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
                case WM_MOVE:
                    this->m_position   = {
						static_cast<uint16_t>(GET_X_LPARAM(lParam)), 
						static_cast<uint16_t>(GET_Y_LPARAM(lParam))
					};

                    return 0;
                case WM_SIZE:
                    this->m_dimensions = {
						static_cast<uint16_t>(GET_X_LPARAM(lParam)),
						static_cast<uint16_t>(GET_Y_LPARAM(lParam))
					};

                    this->m_onResizeFunctor(this->m_dimensions);

					this->m_isMinimized = (this->m_dimensions[0] == 0 && this->m_dimensions[1] == 0);

                    return 0;
                case WM_DESTROY:
                    this->destroy();
                    return 0;
            }

            // Dispatch Message To Peripheral Devices
            if (this->m_mouse.__handleMessage(msg, wParam, lParam)) return 0;
            if (this->m_keyboard.__handleMessage(msg, wParam, lParam)) return 0;

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

namespace WindowManager
{
    std::vector<Window> windows;

    Window* createWindow(const WindowDescriptor& descriptor)
    {
        windows.emplace_back(descriptor);

		return &windows[windows.size() - 1];
    }
}; // namespace WindowHandler

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::vector<Window>::iterator it = std::find_if(WindowManager::windows.begin(), WindowManager::windows.end(), [&hwnd](const Window &window) {
        return window.m_handle == hwnd;
    });

    if (it != WindowManager::windows.end())
        return it->handleMessage(msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}