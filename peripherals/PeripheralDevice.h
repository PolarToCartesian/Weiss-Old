#pragma once

#include "../misc/Pch.h"

class PeripheralDevice
{
public:
	virtual void __OnWindowUpdateBegin() = 0;
	virtual bool __HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void __OnWindowUpdateEnd() = 0;
};