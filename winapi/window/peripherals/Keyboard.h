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
        Keyboard() {}

        void onKeyUp  (const std::function<void(const uint8_t)>& functor) { this->m_onKeyUpFunctor   = functor; }
        void onKeyDown(const std::function<void(const uint8_t)>& functor) { this->m_onKeyDownFunctor = functor; }

        bool isKeyDown(const uint8_t key) { return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key); }

        virtual void __onWindowUpdateBegin() override
        {
        }

        virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
        {
            switch (msg)
            {
                case WM_KEYDOWN:
                {
                    const uint8_t keyCode = static_cast<uint8_t>(wParam);

                    // Verify that the key was not down before the message was sent
                    if (!CHECK_BIT(lParam, 30))
                    {
                        this->m_downKeys.push_back(keyCode);
                        this->m_onKeyDownFunctor(keyCode);
                    }

                    return true;
                }
                case WM_KEYUP:
                {
                    const uint8_t keyCode = static_cast<uint8_t>(wParam);

                    this->m_downKeys.erase(std::remove(this->m_downKeys.begin(), this->m_downKeys.end(), keyCode), this->m_downKeys.end());

                    this->m_onKeyUpFunctor(keyCode);

                    return true;
                }
            }

            return false;
        }

        virtual void __onWindowUpdateEnd() override {
            
        }
};