#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
	private:
		float m_fov, m_aspectRatio, m_zNear, m_zFar;

		Vec3f m_lookAtPosition{ 0, 0, 1000.f };

	public:
		PerspectiveCamera(const Vec3f& position, const Vec3f& lookAtPosition, const float fov, const float aspectRatio, const float zNear, const float zFar)
			: m_fov(fov), m_lookAtPosition(lookAtPosition), m_aspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar)
		{
			this->m_position = position;

			this->calculateTransform();
		}

		virtual void calculateTransform() override
		{
			this->m_transform = createViewMatrix(m_position, m_lookAtPosition)
							  * createPerspectiveMatrix(m_fov, m_aspectRatio, m_zNear, m_zFar);
		}
};