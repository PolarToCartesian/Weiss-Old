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
		Mouse();

        void onLeftButtonUp  (const std::function<void(Vec2u)>& functor);
        void onLeftButtonDown(const std::function<void(Vec2u)>& functor);

        void onRightButtonUp  (const std::function<void(Vec2u)>& functor);
		void onRightButtonDown(const std::function<void(Vec2u)>& functor);

        void onWheelTurn(const std::function<void(const int16_t)>& functor);
		;
        void onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor);
		;
        bool isLeftButtonUp()   const;
		bool isLeftButtonDown() const;

        bool isRightButtonUp()   const;
		bool isRightButtonDown() const;

		virtual void __onWindowUpdateBegin() override;

		virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

		virtual void __onWindowUpdateEnd() override;
};