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
		PerspectiveCamera(const Vec3f& position, const Vec3f& rotation, const float fov, Window& window, const float zNear, const float zFar)
			: m_fov(fov), m_zNear(zNear), m_zFar(zFar)
		{
			this->m_position = DirectX::XMVectorSet(position[0], position[1], position[2], 0.0f);
			this->m_rotation = DirectX::XMVectorSet(rotation[0], rotation[1], rotation[2], 0.0f);

			auto recalculateAspectRatio = [this, &window](const Vec2u& clientDims)
			{
				this->m_aspectRatio = clientDims[0] / static_cast<float>(clientDims[1]);
			};

			recalculateAspectRatio({ window.getClientWidth(), window.getClientHeight() });

			window.onResize(recalculateAspectRatio);
		}

		void translate(const Vec3f& v)
		{
			this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f));
		}

		void setPosition(const Vec3f& v) { this->m_position = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f); }

		void rotate(const Vec3f& v)
		{
			const DirectX::XMVECTOR rotationDeltaVector = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);
			
			this->m_rotation = DirectX::XMVectorAdd(this->m_rotation, rotationDeltaVector);

			// UP-DOWN Rotation Limit
			if (this->m_rotation.m128_f32[0] > HALF_PI)
				this->m_rotation.m128_f32[0] = HALF_PI;

			if (this->m_rotation.m128_f32[0] < -HALF_PI)
				this->m_rotation.m128_f32[0] = -HALF_PI;
		}

		void setRotation(const Vec3f& v)
		{
			this->m_rotation = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);

			// UP-DOWN Rotation Limit
			if (this->m_rotation.m128_f32[0] > HALF_PI)
				this->m_rotation.m128_f32[0] = HALF_PI;

			if (this->m_rotation.m128_f32[0] < -HALF_PI)
				this->m_rotation.m128_f32[0] = -HALF_PI;
		}

		virtual void calculateTransform() override
		{
			const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2]);
			const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(this->m_position, DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix));
			const DirectX::XMVECTOR upDirectionVec = DirectX::XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

			this->m_transform = DirectX::XMMatrixLookAtLH(this->m_position, lookAtPosition, upDirectionVec)
							  * DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_zNear, m_zFar);

			const DirectX::XMMATRIX rotationYMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->m_rotation.m128_f32[1], 0.0f);

			this->m_forwardVector = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationYMatrix);
			this->m_rightVector   = DirectX::XMVector3TransformCoord(RIGHT_VECTOR,   rotationYMatrix);
		}

		void handleMouseMovements(Mouse& mouse, const float sensitivity)
		{
			mouse.onMouseMove([sensitivity, this, &mouse](const Vec2u position, const Vec2i delta)
			{
				if (mouse.isCursorInWindow())
					this->rotate({ sensitivity * delta[1], sensitivity * delta[0], 0.0f });
			});
		}

		void handleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
		{
			if (keyboard.isKeyDown(forward))
				this->m_position = DirectX::XMVectorAdd(this->m_position,      DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
			if (keyboard.isKeyDown(backward))
				this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

			if (keyboard.isKeyDown(right))
				this->m_position = DirectX::XMVectorAdd(this->m_position,      DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
			if (keyboard.isKeyDown(left))
				this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

			if (keyboard.isKeyDown(up))
				this->m_position = DirectX::XMVectorAdd(this->m_position,      DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
			if (keyboard.isKeyDown(down))
				this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
		}
};