#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"
#include "../buffers/ConstantBuffer.h"

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

#include <optional>

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	VertexShader vs;
	PixelShader ps;

	std::optional<ConstantBuffer> cb;

	void Bind() const
	{
		vb.Bind();
		ib.Bind();
		ps.Bind();
		vs.Bind();

		if (cb.has_value())
			cb.value().Bind();
	}
};