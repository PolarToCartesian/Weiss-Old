#pragma once

#include "Peripheral.h"
#include "../../common/Include.h"

struct WindowDescriptor
{
	uint16_t width = 500u, height = 500u;
	uint16_t x = 0u;
	uint16_t y = 0u;
	const char* title    = "Weiss Engine";
	const char* iconPath = nullptr;
	bool isResizable = true;
};

class Window {
protected:
	Mouse* m_pMouse = nullptr;
	Keyboard* m_pKeyboard = nullptr;

	bool m_isRunning = false;
	bool m_isMinimized = false;

	std::vector<std::function<void(const Vec2u16)>> m_onResizeFunctors;

public:
	Window();

	virtual ~Window();

	void OnResize(const std::function<void(const Vec2u16)>& functor);

	[[nodiscard]] bool      IsRunning() const;
	[[nodiscard]] Keyboard& GetKeyboard();
	[[nodiscard]] Mouse& GetMouse();

	[[nodiscard]] uint16_t GetWindowPositionX() const;
	[[nodiscard]] uint16_t GetWindowPositionY() const;
	[[nodiscard]] uint16_t GetClientWidth()     const;
	[[nodiscard]] uint16_t GetClientHeight()    const;

	[[nodiscard]] uint16_t GetWindowWidth() const;

	[[nodiscard]] uint16_t GetWindowHeight() const;

	[[nodiscard]] virtual Rect GetWindowRectangle() const = 0;
	[[nodiscard]] virtual Rect GetClientRectangle() const = 0;

	virtual void SetWindowSize(const uint16_t width, const uint16_t height) = 0;
	virtual void SetClientSize(const uint16_t width, const uint16_t height) = 0;
	virtual void SetTitle(const char* title) const = 0;
	virtual void SetIcon(const char* pathname) = 0;

	virtual void Update() = 0;

public:
	// To Be Defined Per Platform
	static Window* Create(const WindowDescriptor& descriptor = WindowDescriptor{});
};