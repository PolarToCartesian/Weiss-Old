#pragma once

#include "../math/Vectors.h"

class Camera
{
protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR m_position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	size_t m_transformConstantBufferIndex = 0;

public:
	Camera() { }

	[[nodiscard]] DirectX::XMMATRIX GetTransform()           const noexcept { return this->m_transform; }
	[[nodiscard]] DirectX::XMMATRIX GetTransposedTransform() const noexcept { return DirectX::XMMatrixTranspose(this->m_transform); }

	[[nodiscard]] Vec3f GetPosition() const noexcept { return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] }; }
	[[nodiscard]] Vec3f GetRotation() const noexcept { return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] }; }

	virtual void CalculateTransform() = 0;
};