#pragma once

#include "Vertices.h"

template <typename V>
struct Triangle
{
	V v1;
	V v2;
	V v3;
};

typedef Triangle<Colored2DVertex> ColoredTriangle2D;