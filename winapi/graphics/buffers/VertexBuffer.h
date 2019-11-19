#pragma once

#include "../../misc/Error.h"
#include "../../misc/includes.h"
#include "../../../math/Vertex.h"

#include <vector>

struct VertexBufferDescriptor
{
	const void* memoryPtr;
	const uint32_t nElements;
	const uint32_t elementSize;
};

class VertexBuffer {
	private:
		size_t nVertices;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
					 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef,
					 const VertexBufferDescriptor& descriptor);

		void Bind() const noexcept;
};