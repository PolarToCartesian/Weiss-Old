#pragma once

#include "Mouse.h"
#include "Keyboard.h"

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam);

enum class WindowCreationExceptionErrorType
{
	CLASS_REGISTRATION_ERROR,
	WINDOW_CREATION_ERROR
};

class IconLoadingException : public std::exception { };

class WindowCreationException : public std::exception
{
private:
	const WindowCreationExceptionErrorType m_type;

public:
	WindowCreationException(const WindowCreationExceptionErrorType& type);

	WindowCreationExceptionErrorType getErrorType() const;
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
	Window(const WindowDescriptor& descriptor);

	// Misc
	void OnResize(const std::function<void(const Vec2u16)>& functor);

	// Getters
	[[nodiscard]] bool      IsRunning() const;
	[[nodiscard]] Keyboard& GetKeyboard();
	[[nodiscard]] Mouse&    GetMouse();
	[[nodiscard]] HWND      GetHandle() const;

	[[nodiscard]] uint16_t GetWindowPositionX() const;
	[[nodiscard]] uint16_t GetWindowPositionY() const;
	[[nodiscard]] uint16_t GetClientWidth()     const;
	[[nodiscard]] uint16_t GetClientHeight()    const;

	[[nodiscard]] uint16_t GetWindowWidth()  const;

	[[nodiscard]] uint16_t GetWindowHeight() const;

	[[nodiscard]] RECT GetWindowRectangle() const;

	[[nodiscard]] RECT GetClientRectangle() const;

	// Setters
	void SetWindowSize(const uint16_t width, const uint16_t height);

	void SetClientSize(const uint16_t width, const uint16_t height);

	void SetTitle(const char* title) const noexcept;

	void SetIcon(const char* pathname);

	// MESSAGE HANDLING
	void Update();

	[[nodiscard]] LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	// DESTROYING
	void Destroy();

	~Window();

public:
	static std::list<Window> m_s_windows;

	static Window& Create(const WindowDescriptor& windowDesc);
};