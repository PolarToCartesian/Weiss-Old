#pragma once

enum class ShaderBindingType {
	VERTEX,
	PIXEL,
	BOTH
};

enum class ShaderLoadingMethod {
	FROM_BINARY_FILE, // (.cso file)
	FROM_SOURCE_CODE  // (raw source code)
};