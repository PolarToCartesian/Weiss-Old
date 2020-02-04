#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(Window& window, const OrthographicCameraDescriptor& descriptor)
	: m_window(window)
{
	this->m_position = DirectX::XMVectorSet(descriptor.position.x, descriptor.position.y, 0.0f, 0.0f);
	this->m_rotation = DirectX::XMVectorSet(0.0f, 0.0f, descriptor.ratation, 0.0f);

	auto recalculateInvAspectRatio = [this](const Vec2u16& clientDims)
	{
		this->m_InvAspectRatio = clientDims.y / static_cast<float>(clientDims.x);
	};

	recalculateInvAspectRatio({ window.GetClientWidth(), window.GetClientHeight() });

	window.OnResize(recalculateInvAspectRatio);
}

void OrthographicCamera::Rotate(const float angle)
{
	this->m_rotation.m128_f32[2] += angle;
}

void OrthographicCamera::SetPosition(const Vec2f& v)
{
	this->m_position = DirectX::XMVectorSet(v.x, v.y, 0.0f, 0.0f);
}

void OrthographicCamera::SetRotation(const Vec2f& v)
{
	this->m_rotation.m128_f32[0] += v.x;
	this->m_rotation.m128_f32[1] += v.y;
}

void OrthographicCamera::Translate(const Vec2f& v)
{
	this->m_position.m128_f32[0] += v.x;
	this->m_position.m128_f32[1] += v.y;
}

void OrthographicCamera::CalculateTransform()
{
	this->m_transform = DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1, 1) *
		DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) *
		DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]);
}

void OrthographicCamera::HandleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down)
{
	if (keyboard.IsKeyDown(right))
		this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] - speed, this->m_position.m128_f32[1], 0.0f, 0.0f);
	if (keyboard.IsKeyDown(left))
		this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] + speed, this->m_position.m128_f32[1], 0.0f, 0.0f);

	if (keyboard.IsKeyDown(up))
		this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] - speed, 0.0f, 0.0f);
	if (keyboard.IsKeyDown(down))
		this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] + speed, 0.0f, 0.0f);
}