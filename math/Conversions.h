#pragma once

#include "Vectors.h"
#include "Constants.h"

namespace Conversions
{
	float degreesToRadians(const float degrees);
	float radiansToDegrees(const float radians);

	// If a polar point is stored as (θ, r)
	Vec2f polarToCartesian(const Vec2f polar);
	Vec2f cartesianToPolar(const Vec2f cartesian);
};