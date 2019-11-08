#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"
#include "../buffers/ConstantBuffer.h"

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

#include <DirectXMath.h>

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	VertexShader vs;
	PixelShader ps;

	std::vector<ConstantBuffer> cbs;
	
	void Bind() const
	{
		vb.Bind();
		ib.Bind();
		ps.Bind();
		vs.Bind();

		for (const ConstantBuffer& cb : cbs)
			cb.Bind();
	}
};