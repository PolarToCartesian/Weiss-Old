#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	size_t vsIndex;
	size_t psIndex;

	std::vector<size_t> t2dIndices;
	std::optional<size_t> tsIndex;

	std::vector<size_t> cbIndices;

	D3D_PRIMITIVE_TOPOLOGY pt;
};