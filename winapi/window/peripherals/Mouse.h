#pragma once

#include "../../../math/Vectors.h"
#include "PeripheralDevice.h"

#include <windowsx.h>
#include <functional>

class Mouse : PeripheralDevice
{
    private:
        Vec2u m_position{0, 0};
        Vec2i m_deltaPosition{0, 0};

        int16_t m_wheelDelta = 0;

        bool m_isLeftButtonDown  = false;
        bool m_isRightButtonDown = false;

        bool m_wasCursorMovedDuringUpdate = false;

        std::function<void(const Vec2u)> m_onLeftButtonUpFunctor    = [](const Vec2u pos) {};
        std::function<void(const Vec2u)> m_onLeftButtonDownFunctor  = [](const Vec2u pos) {};
        
        std::function<void(const Vec2u)> m_onRightButtonUpFunctor   = [](const Vec2u pos) {};
        std::function<void(const Vec2u)> m_onRightButtonDownFunctor = [](const Vec2u pos) {};

        std::function<void(const int16_t)> m_onWheelTurnFunctor = [](const int16_t delta) {};

        std::function<void(const Vec2u, const Vec2i)> m_onCursorMove = [](const Vec2u pos, const Vec2i deltaPos) {};

    public:
        Mouse() {}

        void onLeftButtonUp  (const std::function<void(Vec2u)>& functor) { this->m_onLeftButtonUpFunctor   = functor; }
        void onLeftButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onLeftButtonDownFunctor = functor; }

        void onRightButtonUp  (const std::function<void(Vec2u)>& functor) { this->m_onRightButtonUpFunctor   = functor; }
        void onRightButtonDown(const std::function<void(Vec2u)>& functor) { this->m_onRightButtonDownFunctor = functor; }

        void onWheelTurn(const std::function<void(const int16_t)>& functor) { this->m_onWheelTurnFunctor = functor; }

        void onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor) { this->m_onCursorMove = functor; }

        bool isLeftButtonUp()   const { return !this->m_isLeftButtonDown; }
        bool isLeftButtonDown() const { return this->m_isLeftButtonDown;  }

        bool isRightButtonUp()   const { return !this->m_isRightButtonDown; }
        bool isRightButtonDown() const { return this->m_isRightButtonDown;  }

        virtual void __onWindowUpdateBegin() override
        {
            this->m_wheelDelta = 0;
            this->m_deltaPosition = {0, 0};
            this->m_wasCursorMovedDuringUpdate = false;
        }

        virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
        {
            switch (msg)
            {
                case WM_MOUSEMOVE:
                    {
                        this->m_deltaPosition += Vec2i{
                            static_cast<int16_t>(static_cast<int16_t>(GET_X_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[0])),
                            static_cast<int16_t>(static_cast<int16_t>(GET_Y_LPARAM(lParam)) - static_cast<int16_t>(this->m_position[1]))
                        };

                        this->m_position = {(uint16_t)GET_X_LPARAM(lParam), (uint16_t)GET_Y_LPARAM(lParam)};

                        this->m_wasCursorMovedDuringUpdate = true;
                    }
                    return true;
                case WM_LBUTTONDOWN:
                    this->m_isLeftButtonDown = true;
                    this->m_onLeftButtonDownFunctor(this->m_position);
                    return true;
                case WM_LBUTTONUP:
                    this->m_isLeftButtonDown = false;
                    this->m_onLeftButtonUpFunctor(this->m_position);
                    return true;
                case WM_RBUTTONDOWN:
                    this->m_isRightButtonDown = true;
                    this->m_onRightButtonDownFunctor(this->m_position);
                    return true;
                case WM_RBUTTONUP:
                    this->m_isRightButtonDown = false;
                    this->m_onRightButtonUpFunctor(this->m_position);
                    return true;
                case WM_MOUSEWHEEL:
                    this->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
                    return true;
                default:
                    return false;
            }
        }

        virtual void __onWindowUpdateEnd() override {
            if (this->m_wasCursorMovedDuringUpdate)
                this->m_onCursorMove(this->m_position, this->m_deltaPosition);
            
            if (this->m_wheelDelta != 0)
                this->m_onWheelTurnFunctor(this->m_wheelDelta);
        }
};