#pragma once

#include "../misc/Macros.h"
#include "PeripheralDevice.h"

class Keyboard : PeripheralDevice
{
private:
	std::vector<uint8_t> m_downKeys;

	std::vector<std::function<void(const uint8_t)>> m_onKeyUpFunctors;
	std::vector<std::function<void(const uint8_t)>> m_onKeyDownFunctors;

public:
	Keyboard();

	void OnKeyUp  (const std::function<void(const uint8_t)>& functor);
	void OnKeyDown(const std::function<void(const uint8_t)>& functor);

	bool IsKeyDown(const uint8_t key);

	virtual void __OnWindowUpdateBegin() override;

	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

	virtual void __OnWindowUpdateEnd() override;
};