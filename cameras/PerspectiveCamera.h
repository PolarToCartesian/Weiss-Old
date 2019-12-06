#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
	private:
		const DirectX::XMVECTOR UP_VECTOR      = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const DirectX::XMVECTOR FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const DirectX::XMVECTOR RIGHT_VECTOR   = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		DirectX::XMVECTOR m_forwardVector = FORWARD_VECTOR;
		DirectX::XMVECTOR m_rightVector   = RIGHT_VECTOR;

		float m_fov = 0.0f, m_aspectRatio = 0.0f, m_zNear = 0.0f, m_zFar = 0.0f;

	public:
		PerspectiveCamera(const Vec3f& position, const Vec3f& rotation, const float fov, Window& window, const float zNear, const float zFar);

		void translate(const Vec3f& v);

		void setPosition(const Vec3f& v);

		void rotate(const Vec3f& v);

		void setRotation(const Vec3f& v);

		virtual void calculateTransform() override;

		DirectX::XMMATRIX getViewMatrix();
		DirectX::XMMATRIX getProjectionMatrix();

		void handleMouseMovements(Mouse& mouse, const float sensitivity);

		void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down);

		Vec3f getKeyboardInputsDelta(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down);
};