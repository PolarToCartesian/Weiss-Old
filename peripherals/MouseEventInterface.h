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
	void OnLeftButtonUp   (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonUpFunctors.push_back(functor); }
	void OnLeftButtonDown (const std::function<void(Vec2u16)>& functor)                      { this->m_onLeftButtonDownFunctors.push_back(functor); }
	void OnRightButtonUp  (const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonUpFunctors.push_back(functor); }
	void OnRightButtonDown(const std::function<void(Vec2u16)>& functor)                      { this->m_onRightButtonDownFunctors.push_back(functor); }
	void OnWheelTurn      (const std::function<void(const int16_t)>& functor)                { this->m_onWheelTurnFunctors.push_back(functor); }
	void OnMouseMove      (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onMouseMoveFunctors.push_back(functor); }
	void OnCursorMove     (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onCursorMoveFunctors.push_back(functor); }
};