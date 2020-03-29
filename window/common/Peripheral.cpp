#include "Peripheral.h"

void PeripheralDevice::__OnWindowUpdateBegin() { }
void PeripheralDevice::__OnWindowUpdateEnd() { }

void MouseEventInterface::OnLeftButtonUp(const std::function<void(Vec2u16)>& functor) { this->m_onLeftButtonUpFunctors.push_back(functor); }
void MouseEventInterface::OnLeftButtonDown(const std::function<void(Vec2u16)>& functor) { this->m_onLeftButtonDownFunctors.push_back(functor); }
void MouseEventInterface::OnRightButtonUp(const std::function<void(Vec2u16)>& functor) { this->m_onRightButtonUpFunctors.push_back(functor); }
void MouseEventInterface::OnRightButtonDown(const std::function<void(Vec2u16)>& functor) { this->m_onRightButtonDownFunctors.push_back(functor); }
void MouseEventInterface::OnWheelTurn(const std::function<void(const int16_t)>& functor) { this->m_onWheelTurnFunctors.push_back(functor); }
void MouseEventInterface::OnMouseMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onMouseMoveFunctors.push_back(functor); }
void MouseEventInterface::OnCursorMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onCursorMoveFunctors.push_back(functor); }

Mouse::Mouse() {  }

Mouse::~Mouse() { }

[[nodiscard]] bool Mouse::IsLeftButtonUp()    const { return !this->m_isLeftButtonDown; }
[[nodiscard]] bool Mouse::IsLeftButtonDown()  const { return this->m_isLeftButtonDown;  }

[[nodiscard]] bool Mouse::IsRightButtonUp()   const { return !this->m_isRightButtonDown; }
[[nodiscard]] bool Mouse::IsRightButtonDown() const { return this->m_isRightButtonDown;  }

[[nodiscard]] bool Mouse::IsCursorInWindow()  const { return this->m_isCursorInWindow;   }

void KeyboardEventInterface::OnKeyUp(const std::function<void(const uint8_t)>& functor) { this->m_onKeyUpFunctors.push_back(functor); }
void KeyboardEventInterface::OnKeyDown(const std::function<void(const uint8_t)>& functor) { this->m_onKeyDownFunctors.push_back(functor); }

Keyboard::Keyboard() {  }

Keyboard::~Keyboard() { }

bool Keyboard::IsKeyDown(const uint8_t key) const noexcept
{
	return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key);
}