#pragma once

#include "../math/Vectors.h"
#include "PeripheralDevice.h"

#include "../misc/Includes.h"

class Mouse : PeripheralDevice
{
    private:
        Vec2u16 m_position{0, 0};
        Vec2i16 m_deltaPosition{0, 0};

        int16_t m_wheelDelta = 0;

        bool m_isLeftButtonDown  = false;
        bool m_isRightButtonDown = false;

		bool m_wasMouseMovedDuringUpdate  = false;
        bool m_wasCursorMovedDuringUpdate = false;

		bool m_isCursorInWindow = false;

        std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonUpFunctors;
		std::vector<std::function<void(const Vec2u16)>> m_onLeftButtonDownFunctors;
        
		std::vector<std::function<void(const Vec2u16)>> m_onRightButtonUpFunctors;
		std::vector<std::function<void(const Vec2u16)>> m_onRightButtonDownFunctors;

		std::vector<std::function<void(const int16_t)>> m_onWheelTurnFunctors;

		std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onMouseMoveFunctors;
		std::vector<std::function<void(const Vec2u16, const Vec2i16)>> m_onCursorMoveFunctors;

    public:
		Mouse();

        void onLeftButtonUp  (const std::function<void(Vec2u16)>& functor);
        void onLeftButtonDown(const std::function<void(Vec2u16)>& functor);

        void onRightButtonUp  (const std::function<void(Vec2u16)>& functor);
		void onRightButtonDown(const std::function<void(Vec2u16)>& functor);

        void onWheelTurn(const std::function<void(const int16_t)>& functor);
		
		void onMouseMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor);
        void onCursorMove(const std::function<void(const Vec2u16, const Vec2i16)>& functor);
		
        bool isLeftButtonUp()   const;
		bool isLeftButtonDown() const;

        bool isRightButtonUp()   const;
		bool isRightButtonDown() const;

		bool isCursorInWindow() const;

		void show() const;
		void hide() const;

		void clip(const RECT& rect) const;

		virtual void __onWindowUpdateBegin() override;

		virtual bool __handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

		virtual void __onWindowUpdateEnd() override;
};