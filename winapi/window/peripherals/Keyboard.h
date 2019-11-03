#pragma once

#include "PeripheralDevice.h"

#include <vector>
#include <algorithm>
#include <functional>

#define CHECK_BIT(var,pos) ((var) & ( 1 << (pos)))

class Keyboard : PeripheralDevice
{
    private:
        std::vector<uint8_t> m_downKeys;

        std::function<void(const uint8_t)> m_onKeyUpFunctor   = [](const uint8_t keyCode) {};
        std::function<void(const uint8_t)> m_onKeyDownFunctor = [](const uint8_t keyCode) {};

    public:
		Keyboard();

        void onKeyUp  (const std::function<void(const uint8_t)>& functor);
		void onKeyDown(const std::function<void(const uint8_t)>& functor);

		bool isKeyDown(const uint8_t key);

		virtual void __onWindowUpdateBegin() override;

		virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

		virtual void __onWindowUpdateEnd();
};