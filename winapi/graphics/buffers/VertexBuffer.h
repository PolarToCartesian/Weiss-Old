#pragma once

#include "Buffer.h"
#include "../../../math/Vertex.h"

#include <vector>

class VertexBuffer : Buffer {
	private:
		size_t nVertices;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<Vertex>& vertices);

		void Bind() override;
};