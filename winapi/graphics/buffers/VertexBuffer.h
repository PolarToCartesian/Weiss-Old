#pragma once

#include "../../misc/Error.h"
#include "../../misc/includes.h"
#include "../../../math/Vertex.h"

#include <vector>

struct VertexBufferDescriptor
{
	const void* memoryPtr;
	const size_t nElements;
	const size_t elementSize;
	const bool isUpdatable;
};

class VertexBuffer {
	private:
		size_t nElements;
		size_t elementSize;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
					 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef,
					 const VertexBufferDescriptor& descriptor);

		void Bind() const noexcept;
};