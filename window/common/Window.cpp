#include "Window.h"

Window::Window() {  }

Window::~Window()
{
	if (this->m_pMouse != nullptr)
		delete this->m_pMouse;

	if (this->m_pKeyboard != nullptr)
		delete this->m_pKeyboard;
}

void Window::OnResize(const std::function<void(const Vec2u16)>& functor)
{
	this->m_onResizeFunctors.push_back(functor);
}

[[nodiscard]] bool      Window::IsRunning() const { return this->m_isRunning; }
[[nodiscard]] Keyboard& Window::GetKeyboard() { return *this->m_pKeyboard; }
[[nodiscard]] Mouse&    Window::GetMouse() { return *this->m_pMouse; }

[[nodiscard]] uint16_t Window::GetWindowPositionX() const { return static_cast<uint16_t>(this->GetWindowRectangle().left); }
[[nodiscard]] uint16_t Window::GetWindowPositionY() const { return static_cast<uint16_t>(this->GetWindowRectangle().top); }
[[nodiscard]] uint16_t Window::GetClientWidth()     const { return static_cast<uint16_t>(this->GetClientRectangle().right); }
[[nodiscard]] uint16_t Window::GetClientHeight()    const { return static_cast<uint16_t>(this->GetClientRectangle().bottom); }

[[nodiscard]] uint16_t Window::GetWindowWidth() const
{
	const Rect rect = this->GetWindowRectangle();

	return static_cast<uint16_t>(rect.right - rect.left);
}

[[nodiscard]] uint16_t Window::GetWindowHeight() const
{
	const Rect rect = this->GetWindowRectangle();

	return static_cast<uint16_t>(rect.bottom - rect.top);
}