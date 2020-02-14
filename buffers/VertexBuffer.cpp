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
	this->__Buffer__SetData(buff, nVertices * this->m_vertexSize);
}

void VertexBuffer::Bind() const noexcept
{
	const UINT stride = static_cast<UINT>(this->m_vertexSize);
	const UINT offset = 0u;

	this->m_deviceInfo.m_pDeviceContext->IASetVertexBuffers(0u, 1u, this->m_pResource.GetAddressOf(), &stride, &offset);
}