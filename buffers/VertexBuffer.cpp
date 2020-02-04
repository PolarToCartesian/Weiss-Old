#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>&		  pDeviceRef,
								 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const VertexBufferDescriptor&					  descriptor)
	: m_nElements(descriptor.nElements), m_elementSize(descriptor.elementSize), m_pDeviceContextRef(pDeviceContextRef)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = (descriptor.isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = (descriptor.isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = static_cast<UINT>(this->m_elementSize * this->m_nElements);
	bd.StructureByteStride = static_cast<UINT>(this->m_elementSize);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = descriptor.memoryPtr;

	if (pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pVertexBuffer) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Unable To Create Vertex Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw VertexBufferCreationException();
	}
}

size_t VertexBuffer::GetElementCount() const noexcept
{
	return this->m_nElements;
}

void VertexBuffer::SetData(const void* memoryPtr, const size_t nElements)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	if (this->m_pDeviceContextRef->Map(this->m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Map VertexBuffer Memory");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw VertexBufferDataSettingException();
	}

	this->m_nElements = nElements;

	std::memcpy(resource.pData, memoryPtr, this->m_nElements * this->m_elementSize);

	this->m_pDeviceContextRef->Unmap(this->m_pVertexBuffer.Get(), 0);
}

void VertexBuffer::Bind() const noexcept
{
	const UINT stride = static_cast<UINT>(this->m_elementSize);
	const UINT offset = 0u;

	this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
}