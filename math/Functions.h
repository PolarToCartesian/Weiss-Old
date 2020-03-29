#pragma once

#include "Vector.h"
#include "Constants.h"

[[nodiscard]] inline float DegreesToRadians(const float degrees) { return degrees * PI_DIV_180;     }
[[nodiscard]] inline float RadiansToDegrees(const float radians) { return radians * PI_DIV_180_INV; }

// If a polar point is stored as (θ, r)
[[nodiscard]] inline Vec2f PolarToCartesian(const Vec2f polar)
{
	const float x = polar.y * std::cos(polar.x);
	const float y = polar.y * std::sin(polar.x);

	return Vec2f{ x, y };
}

[[nodiscard]] inline Vec2f CartesianToPolar(const Vec2f cartesian)
{
	float a = std::atan(cartesian.y / cartesian.x);

	if (cartesian.x < 0) a += PI;
	else if (cartesian.y < 0) a += TWO_PI;

	const float r = std::sqrt(std::pow(cartesian.x, 2) + std::pow(cartesian.y, 2));

	return Vec2f{ a, r };
}

[[nodiscard]] inline Vec3f GetTriangleSurfaceNormal(const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
	const Vec3f U = b - a;
	const Vec3f V = c - a;

	return Vec3f{
		U.y * V.z - U.z * V.y,
		U.z * V.x - U.x * V.z,
		U.x * V.y - U.y * V.x
	};
}