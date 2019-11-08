#pragma once

#include "MatrixMath.h"

class Camera
{
	protected:
		DirectX::XMMATRIX m_transform = DirectX::XMMatrixIdentity();

	public:
		Vec3f m_position{ 0, 0, 0 };

		Camera() {}

		Vec3f             getPosition()  const noexcept { return m_position;  }
		DirectX::XMMATRIX getTransform() const noexcept { return m_transform; }

		void translate(const Vec3f& delta) noexcept
		{
			this->m_position = { this->m_position[0] + delta[0],
								 this->m_position[1] + delta[1], 
								 this->m_position[2] + delta[2] };
		}

		virtual void calculateTransform() = 0;
};