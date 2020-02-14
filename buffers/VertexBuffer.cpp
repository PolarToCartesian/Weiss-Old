#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const DeviceInfo& deviceInfo, const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable)
	: Buffer(deviceInfo, buff, vertexSize*nVertices, vertexSize, D3D11_BIND_VERTEX_BUFFER, isUpdatable),
	  m_nVertices(nVertices), m_vertexSize(vertexSize)
{

}

size_t VertexBuffer::GetElementCount() const noexcept
{
	return this->m_nVertices;
}

void VertexBuffer::SetData(const void* buff, const size_t nVertices) const
{
	D3D11_MAPPED_SUBRESOURCE resource;

	if (this->m_deviceInfo.m_pDeviceContext->Map(this->m_pResource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Map VertexBuffer Memory");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw VertexBufferDataSettingException();
	}

	std::memcpy(resource.pData, buff, nVertices * this->m_vertexSize);

	this->m_deviceInfo.m_pDeviceContext->Unmap(this->m_pResource.Get(), 0);
}

void VertexBuffer::Bind() const noexcept
{
	const UINT stride = static_cast<UINT>(this->m_vertexSize);
	const UINT offset = 0u;

	this->m_deviceInfo.m_pDeviceContext->IASetVertexBuffers(0u, 1u, this->m_pResource.GetAddressOf(), &stride, &offset);
}