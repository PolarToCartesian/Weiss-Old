#pragma once

#include "../../misc/Error.h"
#include "../../misc/includes.h"
#include "../../../math/Vertex.h"

#include <vector>

class IndexBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;
		
		uint64_t m_nBytes;

	public:
		IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<uint32_t>& indices);

		uint64_t getSize() const;

		void Bind() const noexcept;
};