#pragma once

#include "../../misc/Error.h"
#include "../../misc/includes.h"
#include "../../../math/Vertex.h"

#include <vector>

class IndexBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;
		
		size_t m_nBytes;

	public:
		IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<uint32_t>& indices);

		size_t getSize() const;

		void Bind() const noexcept;
};