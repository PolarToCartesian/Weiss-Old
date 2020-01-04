#pragma once

#include "../math/Vectors.h"

struct PhysicsObject
{
	Vec3f position{ 0.f, 0.f, 0.f };
	Vec3f velocity{ 0.f, 0.f, 0.f };
	Vec3f acceleration{ 0.f, 0.f, 0.f };

	void UpdatePhysicsObject(const float friction = 0.9f)
	{
		this->velocity += this->acceleration;
		this->velocity *= 0.9f;
		this->acceleration = Vec3f{ 0.f, 0.f, 0.f };

		this->position += this->velocity;
	}
};