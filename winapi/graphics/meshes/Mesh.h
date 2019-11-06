#pragma once

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

#include <optional>

struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;

	VertexShader vs;
	PixelShader ps;

	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext) const
	{
		vb.Bind(pDeviceContext);
		ib.Bind(pDeviceContext);
		ps.Bind(pDeviceContext);
		vs.Bind(pDeviceContext);
	}
};