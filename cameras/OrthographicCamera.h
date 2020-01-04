#pragma once

#include "Camera.h"
#include "../Window.h"

struct OrthographicCameraDescriptor
{
	const Vec2f position;
	const float ratation;
};

class OrthographicCamera : public Camera
{
private:
	float m_InvAspectRatio = 0.0f;

	Window& m_window;

public:
	OrthographicCamera(Window& window, const OrthographicCameraDescriptor& descriptor)
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

	void Rotate(const float angle) { this->m_rotation.m128_f32[2] += angle; }

	void SetPosition(const Vec2f& v) { this->m_position = DirectX::XMVectorSet(v.x, v.y, 0.0f, 0.0f); }

	void SetRotation(const Vec2f& v)
	{
		this->m_rotation.m128_f32[0] += v.x;
		this->m_rotation.m128_f32[1] += v.y;
	}

	void Translate(const Vec2f& v)
	{
		this->m_position.m128_f32[0] += v.x;
		this->m_position.m128_f32[1] += v.y;
	}

	virtual void CalculateTransform() override
	{
		const DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) * DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]);
		this->m_transform = /*DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1, 1) **/ modelMatrix;
	}

	void HandleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down)
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
};