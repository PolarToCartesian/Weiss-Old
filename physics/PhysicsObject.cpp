#include "PhysicsObject.h"

void PhysicsObject::UpdatePhysicsObject(const float friction)
{
	this->velocity += this->acceleration;
	this->velocity *= 0.9f;
	this->acceleration = Vec3f{ 0.f, 0.f, 0.f };

	this->position += this->velocity;
}