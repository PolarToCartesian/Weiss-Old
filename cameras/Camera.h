#pragma once

#include "../Window.h"
#include "../math/Constants.h"

class Camera
{
	protected:
		DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

		DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	public:
		Camera() {}

		DirectX::XMMATRIX getTransform() const noexcept { return this->m_transform; }
		DirectX::XMMATRIX getTransposedTransform() const noexcept { return DirectX::XMMatrixTranspose(this->m_transform); }

		Vec3f getPosition() const noexcept { return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] }; };
		Vec3f getRotation() const noexcept { return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] }; };

		virtual void calculateTransform() = 0;
};