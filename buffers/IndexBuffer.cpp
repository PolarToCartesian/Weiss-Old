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
	D3D11_MAPPED_SUBRESOURCE resource;

	if (this->m_deviceInfo.m_pDeviceContext->Map(this->m_pResource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Map IndexBuffer Memory");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw IndexBufferDataSettingException();
	}

	std::memcpy(resource.pData, buff, nIndices * sizeof(uint32_t));

	this->m_deviceInfo.m_pDeviceContext->Unmap(this->m_pResource.Get(), 0);
}

void IndexBuffer::Bind() const noexcept
{
	this->m_deviceInfo.m_pDeviceContext->IASetIndexBuffer(this->m_pResource.Get(), DXGI_FORMAT_R32_UINT, 0u);
}