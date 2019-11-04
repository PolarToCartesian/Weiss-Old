#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"
#include "../buffers/ConstantBuffer.h"

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

struct Mesh {
	IndexBuffer ib;
	VertexBuffer vb;

	PixelShader ps;
	VertexShader vs;
};