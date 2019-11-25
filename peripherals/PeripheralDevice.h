#pragma once

#include "../misc/Includes.h"

class PeripheralDevice
{
    public:
        virtual void __onWindowUpdateBegin() = 0;
        virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void __onWindowUpdateEnd() = 0;
};