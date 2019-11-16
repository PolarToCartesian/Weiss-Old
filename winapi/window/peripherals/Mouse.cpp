#include "Mouse.h"

Mouse::Mouse()
{
	RAWINPUTDEVICE mouseInputDevice;
	mouseInputDevice.usUsagePage = 0x01;
	mouseInputDevice.usUsage = 0x02;
	mouseInputDevice.dwFlags = 0;
	mouseInputDevice.hwndTarget = nullptr;

	RegisterRawInputDevices(&mouseInputDevice, 1, sizeof(RAWINPUTDEVICE));
}

void Mouse::onLeftButtonUp(const std::function<void(Vec2u)>& functor)    { this->m_onLeftButtonUpFunctors.push_back(functor); }
void Mouse::onLeftButtonDown(const std::function<void(Vec2u)>& functor)  { this->m_onLeftButtonDownFunctors.push_back(functor); }

void Mouse::onRightButtonUp(const std::function<void(Vec2u)>& functor)   { this->m_onRightButtonUpFunctors.push_back(functor); }
void Mouse::onRightButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onRightButtonDownFunctors.push_back(functor); }

void Mouse::onWheelTurn(const std::function<void(const int16_t)>& functor) { this->m_onWheelTurnFunctors.push_back(functor); }

void Mouse::onMouseMove(const std::function<void(const Vec2u, const Vec2i)>& functor)  { this->m_onMouseMoveFunctors.push_back(functor); }
void Mouse::onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor) { this->m_onCursorMoveFunctors.push_back(functor); }

bool Mouse::isLeftButtonUp()    const { return !this->m_isLeftButtonDown; }
bool Mouse::isLeftButtonDown()  const { return this->m_isLeftButtonDown; }

bool Mouse::isRightButtonUp()   const { return !this->m_isRightButtonDown; }
bool Mouse::isRightButtonDown() const { return this->m_isRightButtonDown; }

void Mouse::show() const { ShowCursor(true);  }

void Mouse::hide() const { ShowCursor(false); }

void Mouse::clip(uint16_t leftX, uint16_t leftY, uint16_t rightX, uint16_t rightY) const
{
	RECT rect{ leftX, leftY, rightX, rightY };

	ClipCursor(&rect);
}

void Mouse::__onWindowUpdateBegin()
{
	this->m_wheelDelta = 0;
	this->m_deltaPosition = { 0, 0 };
	this->m_wasMouseMovedDuringUpdate  = false;
	this->m_wasCursorMovedDuringUpdate = false;
}

bool Mouse::__handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INPUT:
			{
				UINT size = 0;

				if (!GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
				{
					std::vector<char> rawBuffer(size);

					if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
					{
						const RAWINPUT& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

						if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
						{
							this->m_deltaPosition = {
								this->m_deltaPosition[0] + static_cast<uint16_t>(ri.data.mouse.lLastX),
								this->m_deltaPosition[1] + static_cast<uint16_t>(ri.data.mouse.lLastY)
							};

							this->m_wasMouseMovedDuringUpdate = true;
						}
					}
				}

				return true;
			}
			
			break;
		case WM_MOUSEMOVE:
			{
				this->m_position = { (uint16_t)GET_X_LPARAM(lParam), (uint16_t)GET_Y_LPARAM(lParam) };
				this->m_wasCursorMovedDuringUpdate = true;
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
	if (this->m_wasMouseMovedDuringUpdate)
		for (auto& functor : this->m_onMouseMoveFunctors)
			functor(this->m_position, this->m_deltaPosition);

	if (this->m_wasCursorMovedDuringUpdate)
		for (auto& functor : this->m_onCursorMoveFunctors)
			functor(this->m_position, this->m_deltaPosition);

	if (this->m_wheelDelta != 0)
		for (auto& functor : this->m_onWheelTurnFunctors)
			functor(this->m_wheelDelta);
}