#include "BufferManager.h"

BufferManager::~BufferManager()
{
	delete this->m_deviceInfo;
}

void BufferManager::InitBufferManager(DeviceInfo& deviceInfo)
{
	this->m_deviceInfo = new DeviceInfo(deviceInfo);
}

[[nodiscard]] size_t BufferManager::CreateIndexBuffer(const void* buff, const UINT nIndices, const bool isUpdatable)
{
	this->m_indexBuffers.emplace_back(*this->m_deviceInfo, buff, nIndices, isUpdatable);

	return this->m_indexBuffers.size() - 1u;
}

[[nodiscard]] size_t BufferManager::CreateVertexBuffer(const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable)
{
	this->m_vertexBuffers.emplace_back(*this->m_deviceInfo, buff, nVertices, vertexSize, isUpdatable);

	return this->m_vertexBuffers.size() - 1u;
}

[[nodiscard]] size_t BufferManager::CreateConstantBuffer(const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS)
{
	this->m_constantBuffers.emplace_back(*this->m_deviceInfo, bindingType, buff, objSize, slotVS, slotPS);

	return this->m_constantBuffers.size() - 1u;
}

[[nodiscard]] IndexBuffer& BufferManager::GetIndexBuffer(const size_t index)    noexcept {
	return this->m_indexBuffers[index];
}

[[nodiscard]] VertexBuffer& BufferManager::GetVertexBuffer(const size_t index)   noexcept {
	return this->m_vertexBuffers[index];
}

[[nodiscard]] ConstantBuffer& BufferManager::GetConstantBuffer(const size_t index) noexcept {
	return this->m_constantBuffers[index];
}