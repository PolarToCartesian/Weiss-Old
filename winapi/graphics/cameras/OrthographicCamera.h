#pragma once

#include "Camera.h"
#include "../../misc/includes.h"

class OrthographicCamera : public Camera
{
private:
	float m_InvAspectRatio = 0.0f;

public:
	OrthographicCamera(const Vec2f& position, const Vec2u& dimensions, const float ratation)
		: m_InvAspectRatio(dimensions[1] / static_cast<float>(dimensions[0]))
	{
		this->m_position = DirectX::XMVectorSet(position[0], position[1], 0.0f,     0.0f);
		this->m_rotation = DirectX::XMVectorSet(0.0f,        0.0f,        ratation, 0.0f);
	}

	void rotate(const float angle)   { this->m_rotation.m128_f32[2] += angle; }

	void setPosition(const Vec2f& v) { this->m_position = DirectX::XMVectorSet(v[0], v[1], 0.0f, 0.0f); }

	void setRotation(const Vec2f& v)
	{
		this->m_rotation.m128_f32[0] += v[0];
		this->m_rotation.m128_f32[1] += v[1];
	}

	void translate(const Vec2f& v)
	{
		this->m_position.m128_f32[0] += v[0];
		this->m_position.m128_f32[1] += v[1];
	}

	virtual void calculateTransform() override
	{
		this->m_transform = DirectX::XMMatrixRotationZ(this->m_rotation.m128_f32[2]) // Rotate
						  * DirectX::XMMatrixTranslation(-this->m_position.m128_f32[0], -this->m_position.m128_f32[1], 0.0f) // Translate
						  * DirectX::XMMatrixScaling(this->m_InvAspectRatio, 1.0f, 1.0f); // Correct For Aspect Ratio
	}

	void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char left, const char right, const char up, const char down)
	{
		if (keyboard.isKeyDown(right))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] + speed, this->m_position.m128_f32[1], 0.0f, 0.0f);
		if (keyboard.isKeyDown(left))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0] - speed, this->m_position.m128_f32[1], 0.0f, 0.0f);

		if (keyboard.isKeyDown(up))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] + speed, 0.0f, 0.0f);
		if (keyboard.isKeyDown(down))
			this->m_position = DirectX::XMVectorSet(this->m_position.m128_f32[0], this->m_position.m128_f32[1] - speed, 0.0f, 0.0f);
	}
};