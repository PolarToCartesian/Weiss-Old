#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"

#include <optional>

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	size_t vsIndex;
	size_t psIndex;

	std::optional<size_t> t2dIndex;
	std::optional<size_t> tsIndex;

	std::vector<size_t> cbIndices;

	D3D_PRIMITIVE_TOPOLOGY pt;
};