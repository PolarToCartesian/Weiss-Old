#pragma once

#include "../math/Vectors.h"

class MouseEventInterface
{
protected:
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;

	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
	std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

	std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
	std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

public:
	void OnLeftButtonUp   (const std::function<void(Vec2u16)>& functor);
	void OnLeftButtonDown (const std::function<void(Vec2u16)>& functor);
	void OnRightButtonUp  (const std::function<void(Vec2u16)>& functor);
	void OnRightButtonDown(const std::function<void(Vec2u16)>& functor);
	void OnWheelTurn      (const std::function<void(const int16_t)>& functor);
	void OnMouseMove      (const std::function<void(const Vec2u16, const Vec2i16)>& functor);
	void OnCursorMove     (const std::function<void(const Vec2u16, const Vec2i16)>& functor);
};