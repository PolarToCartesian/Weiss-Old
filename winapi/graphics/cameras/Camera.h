#pragma once

#include "../../windows/Window.h"
#include "../../../math/Constants.h"

class Camera
{
	protected:
		DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

		DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	public:
		Camera() {}

		DirectX::XMMATRIX getTransform() const noexcept { return m_transform; }

		virtual void calculateTransform() = 0;
};