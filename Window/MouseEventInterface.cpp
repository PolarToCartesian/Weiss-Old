#include "MouseEventInterface.h"

void MouseEventInterface::OnLeftButtonUp   (const std::function<void(Vec2u16)>&       functor)                { this->m_onLeftButtonUpFunctors.push_back(functor);    }
void MouseEventInterface::OnLeftButtonDown (const std::function<void(Vec2u16)>&       functor)                { this->m_onLeftButtonDownFunctors.push_back(functor);  }
void MouseEventInterface::OnRightButtonUp  (const std::function<void(Vec2u16)>&       functor)                { this->m_onRightButtonUpFunctors.push_back(functor);   }
void MouseEventInterface::OnRightButtonDown(const std::function<void(Vec2u16)>&       functor)                { this->m_onRightButtonDownFunctors.push_back(functor); }
void MouseEventInterface::OnWheelTurn      (const std::function<void(const int16_t)>& functor)                { this->m_onWheelTurnFunctors.push_back(functor);       }
void MouseEventInterface::OnMouseMove      (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onMouseMoveFunctors.push_back(functor);       }
void MouseEventInterface::OnCursorMove     (const std::function<void(const Vec2u16, const Vec2i16)>& functor) { this->m_onCursorMoveFunctors.push_back(functor);      }