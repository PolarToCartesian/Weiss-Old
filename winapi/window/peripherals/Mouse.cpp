#include "Mouse.h"

Mouse::Mouse() {}

void Mouse::onLeftButtonUp(const std::function<void(Vec2u)>& functor) { this->m_onLeftButtonUpFunctor = functor; }
void Mouse::onLeftButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onLeftButtonDownFunctor = functor; }

void Mouse::onRightButtonUp(const std::function<void(Vec2u)>& functor) { this->m_onRightButtonUpFunctor = functor; }
void Mouse::onRightButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onRightButtonDownFunctor = functor; }

void Mouse::onWheelTurn(const std::function<void(const int16_t)>& functor) { this->m_onWheelTurnFunctor = functor; }

void Mouse::onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor) { this->m_onCursorMove = functor; }

bool Mouse::isLeftButtonUp()    const { return !this->m_isLeftButtonDown; }
bool Mouse::isLeftButtonDown()  const { return this->m_isLeftButtonDown; }

bool Mouse::isRightButtonUp()   const { return !this->m_isRightButtonDown; }
bool Mouse::isRightButtonDown() const { return this->m_isRightButtonDown; }

void Mouse::__onWindowUpdateBegin()
{
	this->m_wheelDelta = 0;
	this->m_deltaPosition = { 0, 0 };
	this->m_wasCursorMovedDuringUpdate = false;
}

bool Mouse::__handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_MOUSEMOVE:
			{
				this->m_deltaPosition = Vec2i{
					static_cast<int16_t>(this->m_deltaPosition[0] + static_cast<int16_t>(GET_X_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[0])),
					static_cast<int16_t>(this->m_deltaPosition[1] + static_cast<int16_t>(GET_Y_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[1]))
				};

				this->m_position = { (uint16_t)GET_X_LPARAM(lParam), (uint16_t)GET_Y_LPARAM(lParam) };

				this->m_wasCursorMovedDuringUpdate = true;
			}
			return true;
		case WM_LBUTTONDOWN:
			this->m_isLeftButtonDown = true;
			this->m_onLeftButtonDownFunctor(this->m_position);
			return true;
		case WM_LBUTTONUP:
			this->m_isLeftButtonDown = false;
			this->m_onLeftButtonUpFunctor(this->m_position);
			return true;
		case WM_RBUTTONDOWN:
			this->m_isRightButtonDown = true;
			this->m_onRightButtonDownFunctor(this->m_position);
			return true;
		case WM_RBUTTONUP:
			this->m_isRightButtonDown = false;
			this->m_onRightButtonUpFunctor(this->m_position);
			return true;
		case WM_MOUSEWHEEL:
			this->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
		default:
			return false;
	}
}

void Mouse::__onWindowUpdateEnd() {
	if (this->m_wasCursorMovedDuringUpdate)
		this->m_onCursorMove(this->m_position, this->m_deltaPosition);

	if (this->m_wheelDelta != 0)
		this->m_onWheelTurnFunctor(this->m_wheelDelta);
}