#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const Vec3f& position, const Vec3f& rotation, const float fov, Window& window, const float zNear, const float zFar)
	: m_fov(fov), m_zNear(zNear), m_zFar(zFar)
{
	this->m_position = DirectX::XMVectorSet(position[0], position[1], position[2], 0.0f);
	this->m_rotation = DirectX::XMVectorSet(rotation[0], rotation[1], rotation[2], 0.0f);

	auto recalculateAspectRatio = [this, &window](const Vec2u16& clientDims)
	{
		this->m_aspectRatio = clientDims[0] / static_cast<float>(clientDims[1]);
	};

	recalculateAspectRatio({ window.getClientWidth(), window.getClientHeight() });

	window.onResize(recalculateAspectRatio);
}

void PerspectiveCamera::translate(const Vec3f& v)
{
	this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f));
}

void PerspectiveCamera::setPosition(const Vec3f& v) { this->m_position = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f); }

void PerspectiveCamera::rotate(const Vec3f& v)
{
	const DirectX::XMVECTOR rotationDeltaVector = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);

	this->m_rotation = DirectX::XMVectorAdd(this->m_rotation, rotationDeltaVector);

	// UP-DOWN Rotation Limit
	if (this->m_rotation.m128_f32[0] > HALF_PI)
		this->m_rotation.m128_f32[0] = HALF_PI;

	if (this->m_rotation.m128_f32[0] < -HALF_PI)
		this->m_rotation.m128_f32[0] = -HALF_PI;
}

void PerspectiveCamera::setRotation(const Vec3f& v)
{
	this->m_rotation = DirectX::XMVectorSet(v[0], v[1], v[2], 0.0f);

	// UP-DOWN Rotation Limit
	if (this->m_rotation.m128_f32[0] > HALF_PI)
		this->m_rotation.m128_f32[0] = HALF_PI;

	if (this->m_rotation.m128_f32[0] < -HALF_PI)
		this->m_rotation.m128_f32[0] = -HALF_PI;
}

void PerspectiveCamera::calculateTransform()
{

	this->m_transform = getViewMatrix() * getProjectionMatrix();
}

DirectX::XMMATRIX PerspectiveCamera::getViewMatrix()
{
	const DirectX::XMMATRIX rotationYMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->m_rotation.m128_f32[1], 0.0f);

	this->m_forwardVector = DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationYMatrix);
	this->m_rightVector = DirectX::XMVector3TransformCoord(RIGHT_VECTOR, rotationYMatrix);

	const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation.m128_f32[0], this->m_rotation.m128_f32[1], this->m_rotation.m128_f32[2]);
	const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(this->m_position, DirectX::XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix));
	const DirectX::XMVECTOR upDirectionVec = DirectX::XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

	return DirectX::XMMatrixLookAtLH(this->m_position, lookAtPosition, upDirectionVec);
}

DirectX::XMMATRIX PerspectiveCamera::getProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_zNear, m_zFar);
}

void PerspectiveCamera::handleMouseMovements(Mouse& mouse, const float sensitivity)
{
	mouse.onMouseMove([sensitivity, this, &mouse](const Vec2u16 position, const Vec2i16 delta)
	{
		if (mouse.isCursorInWindow())
			this->rotate({ sensitivity * delta[1], sensitivity * delta[0], 0.0f });
	});
}

void PerspectiveCamera::handleKeyboardInputs(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
{
	if (keyboard.isKeyDown(forward))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.isKeyDown(backward))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

	if (keyboard.isKeyDown(right))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.isKeyDown(left))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));

	if (keyboard.isKeyDown(up))
		this->m_position = DirectX::XMVectorAdd(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
	if (keyboard.isKeyDown(down))
		this->m_position = DirectX::XMVectorSubtract(this->m_position, DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(speed, speed, speed, 0.0f)));
}

Vec3f PerspectiveCamera::getKeyboardInputsDelta(Keyboard& keyboard, const float speed, const char forward, const char backward, const char left, const char right, const char up, const char down)
{
	DirectX::XMVECTOR delta = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	if (keyboard.isKeyDown(forward))
		delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
	if (keyboard.isKeyDown(backward))
		delta = DirectX::XMVectorMultiply(this->m_forwardVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

	if (keyboard.isKeyDown(right))
		delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
	if (keyboard.isKeyDown(left))
		delta = DirectX::XMVectorMultiply(this->m_rightVector, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

	if (keyboard.isKeyDown(up))
		delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(+speed, +speed, +speed, 0.0f));
	if (keyboard.isKeyDown(down))
		delta = DirectX::XMVectorMultiply(UP_VECTOR, DirectX::XMVectorSet(-speed, -speed, -speed, 0.0f));

	return Vec3f { delta.m128_f32[0], delta.m128_f32[1], delta.m128_f32[2] };
}