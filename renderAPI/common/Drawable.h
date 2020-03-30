#pragma once

#include "../../common/Include.h"

struct Drawable {
	size_t shaderPairIndex;
	size_t vertexBufferIndex;

	std::optional<size_t> indexBufferIndex;
};