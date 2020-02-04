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

bool Mouse::IsLeftButtonUp()    const { return !this->m_isLeftButtonDown;  }
bool Mouse::IsLeftButtonDown()  const { return this->m_isLeftButtonDown;   }
bool Mouse::IsRightButtonUp()   const { return !this->m_isRightButtonDown; }
bool Mouse::IsRightButtonDown() const { return this->m_isRightButtonDown;  }
bool Mouse::IsCursorInWindow()  const { return this->m_isCursorInWindow;   }

void Mouse::Show() const { ShowCursor(true);  }
void Mouse::Hide() const { ShowCursor(false); }

void Mouse::Clip(const RECT& rect) const { ClipCursor(&rect); }

void Mouse::__OnWindowUpdateBegin()
{
	this->m_wheelDelta = 0;
	this->m_deltaPosition = { 0, 0 };
	this->m_wasMouseMovedDuringUpdate = false;
	this->m_wasCursorMovedDuringUpdate = false;
}

bool Mouse::__HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INPUT:
	{
		UINT size = 0;

		// WINDOWS API LOGIC
		if (!GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
		{
			std::vector<char> rawBuffer(size);

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
			{
				const RAWINPUT& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

				if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
				{
					this->m_deltaPosition += Vec2i16{ static_cast<int16_t>(ri.data.mouse.lLastX), static_cast<int16_t>(ri.data.mouse.lLastY) };

					this->m_wasMouseMovedDuringUpdate = true;
				}
			}
		}

		return true;
	}

	break;
	case WM_MOUSELEAVE:
		this->m_isCursorInWindow = false;

		return true;
	case WM_MOUSEMOVE:
	{
		this->m_position = { static_cast<uint16_t>(GET_X_LPARAM(lParam)), static_cast<uint16_t>(GET_Y_LPARAM(lParam)) };
		this->m_wasCursorMovedDuringUpdate = true;
	}

	this->m_isCursorInWindow = true;

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

void Mouse::__OnWindowUpdateEnd()
{
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