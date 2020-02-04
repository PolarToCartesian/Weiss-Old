#pragma once

#include "PeripheralDevice.h"
#include "MouseEventInterface.h"

class Mouse : PeripheralDevice, public MouseEventInterface
{
private:
	Vec2u16 m_position{ 0, 0 };
	Vec2i16 m_deltaPosition{ 0, 0 };

	int16_t m_wheelDelta = 0;

	bool m_isLeftButtonDown  = false;
	bool m_isRightButtonDown = false;

	bool m_wasMouseMovedDuringUpdate  = false;
	bool m_wasCursorMovedDuringUpdate = false;

	bool m_isCursorInWindow = false;

public:
	Mouse();

	[[nodiscard]] bool IsLeftButtonUp()    const;
	[[nodiscard]] bool IsLeftButtonDown()  const;
	[[nodiscard]] bool IsRightButtonUp()   const;
	[[nodiscard]] bool IsRightButtonDown() const;
	[[nodiscard]] bool IsCursorInWindow()  const;

	void Show() const;
	void Hide() const;

	void Clip(const RECT& rect) const;

	virtual void __OnWindowUpdateBegin() override;

	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

	virtual void __OnWindowUpdateEnd() override;
};
