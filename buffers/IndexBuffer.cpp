#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const DeviceInfo& deviceInfo, const void* buff, const UINT nIndices, const bool isUpdatable)
	: Buffer(deviceInfo, buff, nIndices * sizeof(uint32_t), 0u, D3D11_BIND_INDEX_BUFFER, isUpdatable),
	  m_nIndices(nIndices)
{

}

[[nodiscard]] size_t IndexBuffer::GetIndexCount() const
{
	return this->m_nIndices;
}

void IndexBuffer::SetData(const void* buff, const UINT nIndices) const
{
	this->__Buffer__SetData(buff, nIndices * sizeof(uint32_t));
}

void IndexBuffer::Bind() const noexcept
{
	this->m_deviceInfo.m_pDeviceContext->IASetIndexBuffer(this->m_pResource.Get(), DXGI_FORMAT_R32_UINT, 0u);
}