#pragma once

#include "Buffer.h"
#include "../../../math/Vertex.h"

#include <vector>

class IndexBuffer : Buffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<unsigned int>& indices);

		void Bind() override;
};