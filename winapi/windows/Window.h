#pragma once

#include "peripherals/Mouse.h"
#include "../graphics/Graphics.h"
#include "peripherals/Keyboard.h"

#include "../misc/includes.h"

#include <vector>
#include <memory>
#include <functional>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

struct WindowDescriptor
{
	uint16_t windowPositionX, windowPositionY;
	uint16_t width, height;
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

        bool m_isRunning   = false;
		bool m_isMinimized = false;

        std::function<void(const Vec2u)> m_onResizeFunctor = [](const Vec2u dim) {};

    public:
        Window(const WindowDescriptor& descriptor)
        {
            this->m_hinstance = descriptor.hInstance;

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

				RECT windowRect{ 0, 0, descriptor.width, descriptor.height };
				AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

                this->m_handle = CreateWindowA("WNDCLASSA", descriptor.title, windowStyle, 
												descriptor.windowPositionX, descriptor.windowPositionY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 
												NULL, NULL, descriptor.hInstance, NULL);

                ShowWindow(this->m_handle, SW_SHOW);
                UpdateWindow(this->m_handle);

                this->m_isRunning = true;
            }
        }

        // Misc
        void onResize(const std::function<void(const Vec2u)>& functor) { this->m_onResizeFunctor = functor; }

		std::unique_ptr<Graphics> createGraphics() { return std::make_unique<Graphics>(this->m_handle); }

        // Getters
        inline bool      isRunning()          const { return this->m_isRunning;     }
			   Keyboard& getKeyboard()              { return this->m_keyboard;		}
			   Mouse&    getMouse()                 { return this->m_mouse;         }
		inline HWND      getHandle()          const { return this->m_handle;		}

		RECT getWindowRect() const { RECT result; GetWindowRect(this->m_handle, &result); return result; }
		RECT getClientRect() const { RECT result; GetClientRect(this->m_handle, &result); return result; }

		uint16_t getWindowPositionX() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>( rect.left ); }
		uint16_t getWindowPositionY() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>( rect.top  ); }

		uint16_t getWindowWidth()  const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>( rect.right - rect.left ); }
		uint16_t getWindowHeight() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>( rect.right - rect.left ); }

		uint16_t getClientWidth()  const { const RECT rect = this->getClientRect(); return static_cast<uint16_t>(rect.right); }
		uint16_t getClientHeight() const { const RECT rect = this->getClientRect(); return static_cast<uint16_t>(rect.bottom); }

        // Setters
        void setWindowSize(const uint16_t width, const uint16_t height)
		{
			SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
		}

		void setClientSize(const uint16_t width, const uint16_t height)
		{
			const uint16_t topBottomWindowPadding = this->getWindowHeight() - this->getClientHeight();
			const uint16_t leftRightWindowPadding = this->getWindowWidth()  - this->getClientWidth();

			this->setWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
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
                case WM_SIZE:
					{
						const Vec2u client_area_dimensions = {
							static_cast<uint16_t>(GET_X_LPARAM(lParam)),
							static_cast<uint16_t>(GET_Y_LPARAM(lParam))
						};

						this->m_onResizeFunctor(client_area_dimensions);

						this->m_isMinimized = (client_area_dimensions[0] == 0 && client_area_dimensions[1] == 0);
					}

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