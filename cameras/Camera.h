#pragma once

#include "../Window.h"
#include "../math/Constants.h"

class Camera
{
	protected:
		DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

		DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		size_t m_transformConstantBufferIndex = 0;

	public:
		Camera();

		DirectX::XMMATRIX getTransform() const noexcept;
		DirectX::XMMATRIX getTransposedTransform() const noexcept;

		Vec3f getPosition() const noexcept;
		Vec3f getRotation() const noexcept;

		virtual void calculateTransform() = 0;
};