#pragma once

#include "../math/Vectors.h"
#include "../misc/Defines.h"

struct Drawable
{
	size_t vertexBufferIndex = WEISS_NO_RESOURCE_INDEX;
	size_t vertexShaderIndex = WEISS_NO_RESOURCE_INDEX;
	size_t pixelShaderIndex  = WEISS_NO_RESOURCE_INDEX;

	std::optional<size_t> indexBufferIndex = { };
	std::vector<std::string> textureNames = { };
	std::vector<size_t> textureSamplerIndices = { };

	std::vector<size_t> constantBufferIndices = { };

	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};