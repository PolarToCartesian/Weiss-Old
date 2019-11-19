#pragma once

#include "MatrixMath.h"
#include "../../../misc/Constants.h"
#include "../../windows/peripherals/Mouse.h"
#include "../../windows/peripherals/Keyboard.h"

class Camera
{
	protected:
		DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	public:
		DirectX::XMVECTOR m_position = DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

		Camera() {}

		DirectX::XMMATRIX getTransform() const noexcept { return m_transform; }

		virtual void calculateTransform() = 0;
		virtual void handleMouseMovements(Mouse& mouse, const float sensitivity) = 0;
		virtual void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down) = 0;
};