#include "BufferManager.h"

BufferManager::~BufferManager()
{
	delete this->m_deviceInfo;
}

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

[[nodiscard]] IndexBuffer& BufferManager::GetIndexBuffer(const size_t index)    noexcept{
	return this->m_indexBuffers[index];
}

[[nodiscard]] VertexBuffer& BufferManager::GetVertexBuffer(const size_t index)   noexcept{
	return this->m_vertexBuffers[index];
}

[[nodiscard]] ConstantBuffer& BufferManager::GetConstantBuffer(const size_t index) noexcept{
	return this->m_constantBuffers[index];
}