#include "Window.h"

// WINDOW CLASS START

Window::Window(const WindowDescriptor& descriptor)
{
	this->m_hinstance = descriptor.hInstance;

	const WNDCLASSA wc{
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
		
		if (descriptor.iconPath != nullptr)
			this->setIcon(descriptor.iconPath);
	}
}

// Misc
void Window::onResize(const std::function<void(const Vec2u)>& functor) { this->m_onResizeFunctor = functor; }

std::unique_ptr<Graphics> Window::createGraphics() { return std::make_unique<Graphics>(this->m_handle); }

// Getters
bool	  Window::isRunning()   const { return this->m_isRunning; }
Keyboard& Window::getKeyboard()       { return this->m_keyboard;  }
Mouse&	  Window::getMouse()	 	  { return this->m_mouse;     }
HWND	  Window::getHandle()   const { return this->m_handle;    }

RECT Window::getWindowRect() const { RECT result; GetWindowRect(this->m_handle, &result); return result; }
RECT Window::getClientRect() const { RECT result; GetClientRect(this->m_handle, &result); return result; }

uint16_t Window::getWindowPositionX() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>(rect.left); }
uint16_t Window::getWindowPositionY() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>(rect.top); }

uint16_t Window::getWindowWidth()  const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>(rect.right - rect.left); }
uint16_t Window::getWindowHeight() const { const RECT rect = this->getWindowRect(); return static_cast<uint16_t>(rect.right - rect.left); }

uint16_t Window::getClientWidth()  const { const RECT rect = this->getClientRect(); return static_cast<uint16_t>(rect.right); }
uint16_t Window::getClientHeight() const { const RECT rect = this->getClientRect(); return static_cast<uint16_t>(rect.bottom); }

// Setters
void Window::setWindowSize(const uint16_t width, const uint16_t height)
{
	SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void Window::setClientSize(const uint16_t width, const uint16_t height)
{
	const uint16_t topBottomWindowPadding = this->getWindowHeight() - this->getClientHeight();
	const uint16_t leftRightWindowPadding = this->getWindowWidth() - this->getClientWidth();

	this->setWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
}

void Window::setTitle(const char* title) { SetWindowTextA(this->m_handle, title); }

void Window::setIcon(const char* pathname) {
	const HICON hIcon = (HICON)LoadImage(NULL, pathname, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	ASSERT_ERROR(hIcon != NULL);
	
	SendMessage(this->m_handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

// MESSAGE HANDLING
void Window::update()
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

LRESULT Window::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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
void Window::destroy() { this->m_isRunning = !DestroyWindow(this->m_handle); }

Window::~Window() { this->destroy(); }

// WINDOW CLASS END

// NAMESPACE WINDOWMANAGER

std::vector<Window> WindowManager::windows = {};

Window* WindowManager::createWindow(const WindowDescriptor& descriptor)
{
	windows.emplace_back(descriptor);

	return &windows[windows.size() - 1];
}

// WindowProcessMessages Definition

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::vector<Window>::iterator it = std::find_if(WindowManager::windows.begin(), WindowManager::windows.end(), [&hwnd](const Window& window) {
		return window.m_handle == hwnd;
	});

	if (it != WindowManager::windows.end())
		return it->handleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}