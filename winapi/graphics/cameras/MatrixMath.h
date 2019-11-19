#pragma once

#include "../../misc/includes.h"
#include "../../../math/Vectors.h"

DirectX::XMMATRIX createRotationMatrix(const Vec3f& centerOfRotation, const Vec3f& rotation)
{
	return DirectX::XMMatrixTranslation(-centerOfRotation[0], -centerOfRotation[1], -centerOfRotation[2])
		 * DirectX::XMMatrixRotationX(rotation[0])
		 * DirectX::XMMatrixRotationY(rotation[1])
		 * DirectX::XMMatrixRotationZ(rotation[2])
		 * DirectX::XMMatrixTranslation( centerOfRotation[0],  centerOfRotation[1],  centerOfRotation[2]);
}

DirectX::XMMATRIX createViewMatrix(const Vec3f& eyePos, const Vec3f& lookAtPos)
{
	const DirectX::XMVECTOR eyePosition    = DirectX::XMVectorSet(eyePos[0],    eyePos[1],    eyePos[2],    0.0f);
	const DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorSet(lookAtPos[0], lookAtPos[1], lookAtPos[2], 0.0f);
	const DirectX::XMVECTOR upVector       = DirectX::XMVectorSet(0.0f,         1.0f,         0.0f,         0.0f);

	return DirectX::XMMatrixLookAtLH(eyePosition, lookAtPosition, upVector);
}

DirectX::XMMATRIX createPerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar)
{
	return DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, zNear, zFar);
}

DirectX::XMMATRIX createTransformMatrix(const Vec3f& eyePos, const Vec3f& lookAtPos, const Vec3f& centerOfRotation, const Vec3f& rotation, const float fov, const float aspectRatio, const float zNear, const float zFar)
{
	return createRotationMatrix(centerOfRotation, rotation) * createViewMatrix(eyePos, lookAtPos) * createPerspectiveMatrix(fov, aspectRatio, zNear, zFar);
}

DirectX::XMMATRIX createTransformMatrixTransposed(const Vec3f& eyePos, const Vec3f& lookAtPos, const Vec3f& centerOfRotation, const Vec3f& rotation, const float fov, const float aspectRatio, const float zNear, const float zFar)
{
	return DirectX::XMMatrixTranspose(createTransformMatrix(eyePos, lookAtPos, centerOfRotation, rotation, fov, aspectRatio, zNear, zFar));
}