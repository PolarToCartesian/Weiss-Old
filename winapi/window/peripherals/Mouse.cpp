#include "Mouse.h"

Mouse::Mouse() {}

void Mouse::onLeftButtonUp(const std::function<void(Vec2u)>& functor)    { this->m_onLeftButtonUpFunctors.push_back(functor); }
void Mouse::onLeftButtonDown(const std::function<void(Vec2u)>& functor)  { this->m_onLeftButtonDownFunctors.push_back(functor); }

void Mouse::onRightButtonUp(const std::function<void(Vec2u)>& functor)   { this->m_onRightButtonUpFunctors.push_back(functor); }
void Mouse::onRightButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onRightButtonDownFunctors.push_back(functor); }

void Mouse::onWheelTurn(const std::function<void(const int16_t)>& functor) { this->m_onWheelTurnFunctors.push_back(functor); }

void Mouse::onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor) { this->m_onCursorMoveFunctors.push_back(functor); }

bool Mouse::isLeftButtonUp()    const { return !this->m_isLeftButtonDown; }
bool Mouse::isLeftButtonDown()  const { return this->m_isLeftButtonDown; }

bool Mouse::isRightButtonUp()   const { return !this->m_isRightButtonDown; }
bool Mouse::isRightButtonDown() const { return this->m_isRightButtonDown; }

void Mouse::show() const { ShowCursor(true);  }

void Mouse::hide() const { ShowCursor(false); }

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
				this->m_wasCursorMovedDuringUpdate = true;

				this->m_deltaPosition = Vec2i{
					static_cast<int16_t>(this->m_deltaPosition[0] + static_cast<int16_t>(GET_X_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[0])),
					static_cast<int16_t>(this->m_deltaPosition[1] + static_cast<int16_t>(GET_Y_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[1]))
				};

				this->m_position = { (uint16_t)GET_X_LPARAM(lParam), (uint16_t)GET_Y_LPARAM(lParam) };
			}

			return true;
		case WM_LBUTTONDOWN:
			this->m_isLeftButtonDown = true;

			for (auto& functor : this->m_onLeftButtonDownFunctors)
				functor(this->m_position);
			
			return true;
		case WM_LBUTTONUP:
			this->m_isLeftButtonDown = false;

			for (auto& functor : this->m_onLeftButtonUpFunctors)
				functor(this->m_position);
			
			return true;
		case WM_RBUTTONDOWN:
			this->m_isRightButtonDown = true;

			for (auto& functor : this->m_onRightButtonDownFunctors)
				functor(this->m_position);

			return true;
		case WM_RBUTTONUP:
			this->m_isRightButtonDown = false;

			for (auto& functor : this->m_onRightButtonUpFunctors)
				functor(this->m_position);

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
		for (auto& functor : this->m_onCursorMoveFunctors)
			functor(this->m_position, this->m_deltaPosition);

	if (this->m_wheelDelta != 0)
		for (auto& functor : this->m_onWheelTurnFunctors)
			functor(this->m_wheelDelta);
}