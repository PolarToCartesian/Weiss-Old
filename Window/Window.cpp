#include "Window.h"

WindowCreationException::WindowCreationException(const WindowCreationExceptionErrorType& type)
	: m_type(type) { }

WindowCreationExceptionErrorType WindowCreationException::getErrorType() const { return this->m_type; }

Window::Window(const WindowDescriptor& descriptor)
{
	const WNDCLASSA wc{
		CS_HREDRAW | CS_VREDRAW,
		WindowProcessMessages,
		0,
		0,
		GetModuleHandle(NULL),
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
		NULL, NULL, GetModuleHandle(NULL), NULL);

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
void Window::OnResize(const std::function<void(const Vec2u16)>& functor)
{
	this->m_onResizeFunctors.push_back(functor);
}

// Getters
bool      Window::IsRunning() const { return this->m_isRunning; }
Keyboard& Window::GetKeyboard()     { return this->m_keyboard;  }
Mouse&    Window::GetMouse()        { return this->m_mouse;     }
HWND      Window::GetHandle() const { return this->m_handle;    }

uint16_t Window::GetWindowPositionX() const { return static_cast<uint16_t>(this->GetWindowRectangle().left);   }
uint16_t Window::GetWindowPositionY() const { return static_cast<uint16_t>(this->GetWindowRectangle().top);    }
uint16_t Window::GetClientWidth()     const { return static_cast<uint16_t>(this->GetClientRectangle().right);  }
uint16_t Window::GetClientHeight()    const { return static_cast<uint16_t>(this->GetClientRectangle().bottom); }

uint16_t Window::GetWindowWidth()  const
{
	const RECT rect = this->GetWindowRectangle();

	return static_cast<uint16_t>(rect.right - rect.left);
}

uint16_t Window::GetWindowHeight() const
{
	const RECT rect = this->GetWindowRectangle();

	return static_cast<uint16_t>(rect.bottom - rect.top);
}

RECT Window::GetWindowRectangle() const
{
	RECT result;
	GetWindowRect(this->m_handle, &result);

	return result;
}

RECT Window::GetClientRectangle() const
{
	RECT result;
	GetClientRect(this->m_handle, &result);

	return result;
}

// Setters
void Window::SetWindowSize(const uint16_t width, const uint16_t height)
{
	SetWindowPos(this->m_handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void Window::SetClientSize(const uint16_t width, const uint16_t height)
{
	const uint16_t topBottomWindowPadding = this->GetWindowHeight() - this->GetClientHeight();
	const uint16_t leftRightWindowPadding = this->GetWindowWidth()  - this->GetClientWidth();

	this->SetWindowSize(width + leftRightWindowPadding, height + topBottomWindowPadding);
}

void Window::SetTitle(const char* title) const noexcept
{
	SetWindowTextA(this->m_handle, title);
}

void Window::SetIcon(const char* pathname)
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
void Window::Update()
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

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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
void Window::Destroy()
{
	this->m_isRunning = !DestroyWindow(this->m_handle);
}

Window::~Window()
{
	this->Destroy();
}

Window& Window::Create(const WindowDescriptor& windowDesc)
{
	Window::m_s_windows.emplace_back(windowDesc);

	return Window::m_s_windows.back();
}