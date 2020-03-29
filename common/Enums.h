#pragma once

enum class ShaderInputElementType {
	FLOAT_32,
	UINT_32,
	VECTOR_2D_FLOAT_32,
	VECTOR_3D_FLOAT_32,
	VECTOR_4D_FLOAT_32
};

enum class PrimitiveTopology {
	TRIANGLES,
	TRIANGLE_STRIP
};

enum class RenderAPIName {
	DIRECTX11,
	VULKAN
};
