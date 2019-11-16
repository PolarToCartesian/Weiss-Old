#pragma once

#include "../../../math/Vectors.h"
#include "PeripheralDevice.h"

#include "../../misc/Error.h"

#include <strsafe.h>
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

		bool m_wasMouseMovedDuringUpdate  = false;
        bool m_wasCursorMovedDuringUpdate = false;

		bool m_isCursorInWindow = false;

        std::vector<std::function<void(const Vec2u)>> m_onLeftButtonUpFunctors;
		std::vector<std::function<void(const Vec2u)>> m_onLeftButtonDownFunctors;
        
		std::vector<std::function<void(const Vec2u)>> m_onRightButtonUpFunctors;
		std::vector<std::function<void(const Vec2u)>> m_onRightButtonDownFunctors;

		std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

		std::vector<std::function<void(const Vec2u, const Vec2i)>> m_onMouseMoveFunctors;
		std::vector<std::function<void(const Vec2u, const Vec2i)>> m_onCursorMoveFunctors;

    public:
		Mouse();

        void onLeftButtonUp  (const std::function<void(Vec2u)>& functor);
        void onLeftButtonDown(const std::function<void(Vec2u)>& functor);

        void onRightButtonUp  (const std::function<void(Vec2u)>& functor);
		void onRightButtonDown(const std::function<void(Vec2u)>& functor);

        void onWheelTurn(const std::function<void(const int16_t)>& functor);
		
		void onMouseMove(const std::function<void(const Vec2u, const Vec2i)>& functor);
        void onCursorMove(const std::function<void(const Vec2u, const Vec2i)>& functor);
		
        bool isLeftButtonUp()   const;
		bool isLeftButtonDown() const;

        bool isRightButtonUp()   const;
		bool isRightButtonDown() const;

		bool isCursorInWindow() const;

		void show() const;
		void hide() const;

		void clip(uint16_t leftX, uint16_t leftY, uint16_t rightX, uint16_t rightY) const;

		virtual void __onWindowUpdateBegin() override;

		virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

		virtual void __onWindowUpdateEnd() override;
};