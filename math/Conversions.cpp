#include "Conversions.h"

float Conversions::degreesToRadians(const float degrees) { return degrees * PI_DIV_180;     }
float Conversions::radiansToDegrees(const float radians) { return radians * PI_DIV_180_INV; }

Vec2f Conversions::polarToCartesian(const Vec2f polar)
{
	const float x = polar[1] * std::cos(polar[0]);
	const float y = polar[1] * std::sin(polar[0]);
	return Vec2f{ x, y };
}

Vec2f Conversions::cartesianToPolar(const Vec2f cartesian)
{
	float a = std::atan(cartesian[1] / cartesian[0]);

	if (cartesian[0] < 0) a += PI;
	else if (cartesian[1] < 0) a += TWO_PI;

	const float r = std::sqrt( std::pow(cartesian[0], 2) + std::pow(cartesian[1], 2));

	return Vec2f{ a, r };
}