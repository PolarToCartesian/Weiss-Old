#include "Camera.h"

Camera::Camera() {}

DirectX::XMMATRIX Camera::getTransform() const noexcept { return this->m_transform; }
DirectX::XMMATRIX Camera::getTransposedTransform() const noexcept { return DirectX::XMMatrixTranspose(this->m_transform); }

Vec3f Camera::getPosition() const noexcept { return Vec3f{ this->m_position.m128_f32[0], this->m_position.m128_f32[1], this->m_position.m128_f32[2] }; };
Vec3f Camera::getRotation() const noexcept { return Vec3f{ this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2] }; };