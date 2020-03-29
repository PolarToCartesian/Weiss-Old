#pragma once

#include "../common/Include.h"
#include "../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class WindowsPeripheralDevice : public PeripheralDevice {
public:
	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

class WindowsMouse : public Mouse,
                     public WindowsPeripheralDevice {
public:
	WindowsMouse();

	virtual void Show() const noexcept override;
	virtual void Hide() const noexcept override;

	virtual void Clip(const Rect& rect) const noexcept override;

	virtual void __OnWindowUpdateBegin() override;

	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

	virtual void __OnWindowUpdateEnd() override;
};

class WindowsKeyboard : public Keyboard,
                        public WindowsPeripheralDevice {
public:
	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
};

#endif // __WEISS__OS_WINDOWS