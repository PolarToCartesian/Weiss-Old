#include "Window.h"

#ifdef __WEISS__OS_WINDOWS

WindowsWindow::WindowsWindow(const WindowDescriptor& descriptor)
{
	WNDCLASSA wc;
	ZeroMemory(&wc, sizeof(WNDCLASSA));

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProcessMessages;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WEISS_WNDCLASSA";

	if (!RegisterClassA(&wc))
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[WINDOW] Could Not Register Window Class");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw std::runtime_error("[WINDOW] Could Not Register Window Class");
	}

	const uint32_t windowStyle = descriptor.isResizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

	RECT windowRect{ 0, 0, descriptor.width, descriptor.height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	this->m_handle = CreateWindowA("WEISS_WNDCLASSA", descriptor.title, windowStyle,
		descriptor.x, descriptor.y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL, wc.hInstance, NULL);

	if (this->m_handle == NULL)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS

		MESSAGE_BOX_ERROR("[WINDOW] Could Not Create Window");

#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw std::runtime_error("[WINDOW] Could Not Create Window");
	}


#ifdef __WEISS__PLATFORM_X64

	SetWindowLongPtr(this->m_handle, GWLP_USERDATA, (LONG_PTR)this);

#else

	SetWindowLong(this->m_handle, GWLP_USERDATA, (LONG)this);

#endif

	this->m_pMouse = new WindowsMouse();
	this->m_pKeyboard = new WindowsKeyboard();

	ShowWindow(this->m_handle, SW_SHOW);
	UpdateWindow(this->m_handle);

	this->m_isRunning = true;

	if (descriptor.iconPath != nullptr)
		this->SetIcon(descriptor.iconPath);
}

[[nodiscard]] HWND WindowsWindow::GetHandle() const { return this->m_handle; }

[[nodiscard]] Rect WindowsWindow::GetWindowRectangle() const
{
	RECT windowRect;
	GetWindowRect(this->m_handle, &windowRect);

	return Rect(windowRect);
}

[[nodiscard]] Rect WindowsWindow::GetClientRectangle() const
{
	RECT clientRect;
	GetClientRect(this->m_handle, &clientRect);

	return Rect(clientRect);
}

[[nodiscard]] LRESULT WindowsWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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
	if (reinterpret_cast<WindowsMouse*>(this->m_pMouse)->__HandleMessage(msg, wParam, lParam)) return 0;
	if (reinterpret_cast<WindowsKeyboard*>(this->m_pKeyboard)->__HandleMessage(msg, wParam, lParam)) return 0;

	// Otherwise Let Windows Handle The Message
	return DefWindowProc(this->m_handle, msg, wParam, lParam);
}

void WindowsWindow::SetWindowSize(const uint16_t width, const uint16_t height)
{
	SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void WindowsWindow::SetClientSize(const uint16_t width, const uint16_t height)
{
	const uint16_t topBottomWindowPadding = this->GetWindowHeight() - this->GetClientHeight();
	const uint16_t leftRightWindowPadding = this->GetWindowWidth() - this->GetClientWidth();

	this->SetWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
}

void WindowsWindow::SetTitle(const char* title) const noexcept
{
	SetWindowTextA(this->m_handle, title);
}

void WindowsWindow::SetIcon(const char* pathname)
{
	const HICON hIcon = (HICON)LoadImageA(NULL, pathname, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	if (hIcon == NULL)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Load Icon");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw std::runtime_error("Could Not Load Icon");
	}

	SendMessage(this->m_handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void WindowsWindow::Update()
{
	reinterpret_cast<WindowsMouse*>(this->m_pMouse)->__OnWindowUpdateBegin();
	reinterpret_cast<WindowsKeyboard*>(this->m_pKeyboard)->__OnWindowUpdateBegin();

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	reinterpret_cast<WindowsMouse*>(this->m_pMouse)->__OnWindowUpdateEnd();
	reinterpret_cast<WindowsKeyboard*>(this->m_pKeyboard)->__OnWindowUpdateEnd();
}

void WindowsWindow::Destroy() noexcept
{
	this->m_isRunning = !DestroyWindow(this->m_handle);
}

WindowsWindow::~WindowsWindow() { this->Destroy(); }

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef __WEISS__PLATFORM_X64

	WindowsWindow* window = (WindowsWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

#else

	WindowsWindow* window = (WindowsWindow*)GetWindowLong(hwnd, GWLP_USERDATA);

#endif

	if (window != NULL)
		return window->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif // __WEISS__OS_WINDOWS