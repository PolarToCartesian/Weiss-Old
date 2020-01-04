#pragma once

#include "../misc/Defines.h"
#include "../misc/Colors.h"
#include "../math/Vectors.h"

struct Colored2DVertex
{
	Vec2f    position;
	Coloru8  color;
	uint32_t flags = WEISS_COLORED_BATCH_RENDERER_2D_ALL_FLAGS;
};