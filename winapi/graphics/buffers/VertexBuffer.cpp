#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const VertexBufferDescriptor& descriptor)
	: nVertices(descriptor.nElements), m_pDeviceContextRef(pDeviceContextRef)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = static_cast<UINT>(descriptor.elementSize * this->nVertices);
	bd.StructureByteStride = descriptor.elementSize;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = descriptor.memoryPtr;
	H_ERROR(pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pVertexBuffer));
}

void VertexBuffer::Bind() const noexcept
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
}