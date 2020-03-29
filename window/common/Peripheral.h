#pragma once

#include "../../math/Include.h"
#include "../../common/Include.h"

class PeripheralDevice {
public:
	virtual void __OnWindowUpdateBegin();
	virtual void __OnWindowUpdateEnd();
};

class MouseEventInterface {
protected:
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;

	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

	std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

public:
	void OnLeftButtonUp(const std::function<void(Vec2u16)>& functor);
	void OnLeftButtonDown(const std::function<void(Vec2u16)>& functor);
	void OnRightButtonUp(const std::function<void(Vec2u16)>& functor);
	void OnRightButtonDown(const std::function<void(Vec2u16)>& functor);
	void OnWheelTurn(const std::function<void(const int16_t)>& functor);
	void OnMouseMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor);
	void OnCursorMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor);
};

class Mouse : public MouseEventInterface {
protected:
	Vec2u16 m_position{ 0, 0 };
	Vec2i16 m_deltaPosition{ 0, 0 };

	int16_t m_wheelDelta = 0;

	bool m_isLeftButtonDown = false;
	bool m_isRightButtonDown = false;

	bool m_wasMouseMovedDuringUpdate = false;
	bool m_wasCursorMovedDuringUpdate = false;

	bool m_isCursorInWindow = false;

public:
	Mouse();

	virtual ~Mouse();

	[[nodiscard]] bool IsLeftButtonUp()    const;
	[[nodiscard]] bool IsLeftButtonDown()  const;

	[[nodiscard]] bool IsRightButtonUp()   const;
	[[nodiscard]] bool IsRightButtonDown() const;

	[[nodiscard]] bool IsCursorInWindow()  const;

	virtual void Show() const noexcept = 0;
	virtual void Hide() const noexcept = 0;

	virtual void Clip(const Rect& rect) const noexcept = 0;
};

class KeyboardEventInterface {
protected:
	std::vector<uint8_t> m_downKeys;

	std::vector<std::function<void(const uint8_t)>> m_onKeyUpFunctors;
	std::vector<std::function<void(const uint8_t)>> m_onKeyDownFunctors;

public:
	void OnKeyUp(const std::function<void(const uint8_t)>& functor);
	void OnKeyDown(const std::function<void(const uint8_t)>& functor);
};

class Keyboard : public KeyboardEventInterface {
protected:
	std::vector<uint8_t> m_downKeys;

public:
	Keyboard();

	virtual ~Keyboard();

	bool IsKeyDown(const uint8_t key) const noexcept;
};