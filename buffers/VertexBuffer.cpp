#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const VertexBufferDescriptor& descriptor)
	: nElements(descriptor.nElements), elementSize(descriptor.elementSize), m_pDeviceContextRef(pDeviceContextRef)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = (descriptor.isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = (descriptor.isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = static_cast<UINT>(this->elementSize * this->nElements);
	bd.StructureByteStride = static_cast<UINT>(this->elementSize);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = descriptor.memoryPtr;
	HRESULT_ERROR(pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pVertexBuffer), "Unable To Create Vertex Buffer");
}

void VertexBuffer::SetData(const void* memoryPtr, const size_t nElements, const size_t elementSize) const noexcept {
	D3D11_MAPPED_SUBRESOURCE resource;
	this->m_pDeviceContextRef->Map(this->m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, memoryPtr, nElements * elementSize);
	this->m_pDeviceContextRef->Unmap(this->m_pVertexBuffer.Get(), 0);
}

void VertexBuffer::Bind() const noexcept
{
	const UINT stride = static_cast<UINT>(this->elementSize);
	const UINT offset = 0u;

	this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
}