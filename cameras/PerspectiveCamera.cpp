#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(Window& window, const PerspectiveCameraDescriptor& descriptor)
	: m_fov(descriptor.fov), m_zNear(descriptor.zNear), m_zFar(descriptor.zFar)
{
	this->m_position = DirectX::XMVectorSet(descriptor.position.x, descriptor.position.y, descriptor.position.z, 0.0f);
	this->m_rotation = DirectX::XMVectorSet(descriptor.rotation.x, descriptor.rotation.y, descriptor.rotation.z, 0.0f);

	auto recalculateAspectRatio = [this](const Vec2u16& clientDims)
	{
		this->m_aspectRatio = clientDims.x / static_cast<float>(clientDims.y);
	};

	recalculateAspectRatio({ window.GetClientWidth(), window.GetClientHeight() });

	window.OnResize(recalculateAspectRatio);
}

void PerspectiveCamera::Translate(const Vec3f& v) { this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f)); }

void PerspectiveCamera::SetPosition(const Vec3f& v) { this->m_position = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f); }

void PerspectiveCamera::Rotate(const Vec3f& v)
{
	const DirectX::XMVECTOR rotationDeltaVector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);

	this->m_rotation = DirectX::XMVectorAdd(this->m_rotation, rotationDeltaVector);

	// UP-DOWN Rotation Limit
	if (this->m_rotation.m128_f32[0] > HALF_PI)
		this->m_rotation.m128_f32[0] = HALF_PI;

	if (this->m_rotation.m128_f32[0] < -HALF_PI)
		this->m_rotation.m128_f32[0] = -HALF_PI;
}

void PerspectiveCamera::SetRotation(const Vec3f& v)
{
	this->m_rotation = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);

	// UP-DOWN Rotation Limit
	if (this->m_rotation.m128_f32[0] > HALF_PI)
		this->m_rotation.m128_f32[0] = HALF_PI;

	if (this->m_rotation.m128_f32[0] < -HALF_PI)
		this->m_rotation.m128_f32[0] = -HALF_PI;
}

void PerspectiveCamera::CalculateTransform() { this->m_transform = this->GetViewMatrix() * this->GetProjectionMatrix(); }

DirectX::XMMATRIX PerspectiveCamera::GetViewMatrix()
{
	const DirectX::XMMATRIX rotationYMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->m_rotation.m128_f32[1], 0.0f);

	this->m_forwardVector = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationYMatrix);
	this->m_rightVector = DirectX::XMVector3TransformCoord(RIGHT_VECTOR, rotationYMatrix);

	const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2]);
	const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(this->m_position, DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix));
	const DirectX::XMVECTOR upDirectionVec = DirectX::XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

	return DirectX::XMMatrixLookAtLH(this->m_position, lookAtPosition, upDirectionVec);
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_zNear, m_zFar);
}

void PerspectiveCamera::HandleMouseMovements(Mouse& mouse, const float sensitivity)
{
	mouse.OnMouseMove([sensitivity, this, &mouse](const Vec2u16 position, const Vec2i16 delta)
		{
			if (mouse.IsCursorInWindow())
				this->Rotate({ sensitivity * delta.y, sensitivity * delta.x, 0.0f });
		});
}

void PerspectiveCamera::HandleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
{
	if (keyboard.IsKeyDown(forward))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.IsKeyDown(backward))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

	if (keyboard.IsKeyDown(right))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.IsKeyDown(left))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

	if (keyboard.IsKeyDown(up))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.IsKeyDown(down))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
}

Vec3f PerspectiveCamera::GetKeyboardInputsDelta(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
{
	DirectX::XMVECTOR delta = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	if (keyboard.IsKeyDown(forward))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
	if (keyboard.IsKeyDown(backward))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

	if (keyboard.IsKeyDown(right))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
	if (keyboard.IsKeyDown(left))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

	if (keyboard.IsKeyDown(up))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f)));
	if (keyboard.IsKeyDown(down))
		delta = DirectX::XMVectorAdd(delta, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f)));

	return Vec3f{ delta.m128_f32[0], delta.m128_f32[1], delta.m128_f32[2] };
}