#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	uint16_t vsIndex;
	uint16_t psIndex;

	std::vector<uint16_t> cbIndices;
};