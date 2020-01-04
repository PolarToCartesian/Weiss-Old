#pragma once

#include "../math/Vectors.h"

struct Mesh {
	size_t vertexBufferIndex;

	size_t vertexShaderIndex;
	size_t pixelShaderIndex;

	std::optional<size_t> indexBufferIndex = { };
	std::vector<size_t> textureIndices = { };
	std::vector<size_t> textureSamplerIndices = { };

	std::vector<size_t> constantBufferIndices = { };

	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};