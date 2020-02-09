#include "BufferManager.h"

void BufferManager::InitBufferManager(DeviceInfo& deviceInfo)
{
	this->m_deviceInfo = new DeviceInfo(deviceInfo);
}

[[nodiscard]] size_t BufferManager::CreateIndexBuffer(const IndexBufferDescriptor& descriptor)
{
	this->m_indexBuffers.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_indexBuffers.size() - 1u;
}

[[nodiscard]] size_t BufferManager::CreateVertexBuffer(const VertexBufferDescriptor& descriptor)
{
	this->m_vertexBuffers.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_vertexBuffers.size() - 1u;
}

[[nodiscard]] size_t BufferManager::CreateConstantBuffer(const ConstantBufferDescriptor& descriptor)
{
	this->m_constantBuffers.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_constantBuffers.size() - 1u;
}