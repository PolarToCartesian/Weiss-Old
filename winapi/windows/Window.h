#pragma once

#include "../misc/includes.h"
#include "peripherals/Mouse.h"
#include "../graphics/Graphics.h"
#include "peripherals/Keyboard.h"

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
        Window(const WindowDescriptor& descriptor);

        // Misc
        void onResize(const std::function<void(const Vec2u)>& functor);

		std::unique_ptr<Graphics> createGraphics();

        // Getters
        bool      isRunning() const;
		Keyboard& getKeyboard();
		Mouse&    getMouse();
		HWND      getHandle() const;

		RECT getWindowRect() const;
		RECT getClientRect() const;

		uint16_t getWindowPositionX() const;
		uint16_t getWindowPositionY() const;

		uint16_t getWindowWidth()  const;
		uint16_t getWindowHeight() const;

		uint16_t getClientWidth()  const;
		uint16_t getClientHeight() const;

        // Setters
        void setWindowSize(const uint16_t width, const uint16_t height);

		void setClientSize(const uint16_t width, const uint16_t height);

        void setTitle(const char* title);

        // MESSAGE HANDLING
		void update();

		LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

        // DESTROYING
		void destroy();

		~Window();
};

namespace WindowManager
{
    extern std::vector<Window> windows;

	Window* createWindow(const WindowDescriptor& descriptor);
}; // namespace WindowHandler