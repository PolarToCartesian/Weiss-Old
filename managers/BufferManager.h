#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"
#include "../buffers/ConstantBuffer.h"

class BufferManager
{
protected:
	std::vector<IndexBuffer>    m_indexBuffers;
	std::vector<VertexBuffer>   m_vertexBuffers;
	std::vector<ConstantBuffer> m_constantBuffers;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitBufferManager(DeviceInfo& deviceInfo);

public:
	~BufferManager();

	template <typename CONTAINER>
	[[nodiscard]] size_t CreateIndexBuffer(const CONTAINER& arr, const bool isUpdatable)
	{
		this->m_indexBuffers.emplace_back(*this->m_deviceInfo, arr, isUpdatable);

		return this->m_indexBuffers.size() - 1u;
	}

	[[nodiscard]] size_t CreateIndexBuffer(const void* buff, const UINT nIndices, const bool isUpdatable);

	template <typename CONTAINER>
	[[nodiscard]] size_t CreateVertexBuffer(const CONTAINER& arr, const bool isUpdatable)
	{
		this->m_vertexBuffers.emplace_back(*this->m_deviceInfo, arr, isUpdatable);

		return this->m_vertexBuffers.size() - 1u;
	}

	[[nodiscard]] size_t CreateVertexBuffer(const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable);

	[[nodiscard]] size_t CreateConstantBuffer(const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS);

	[[nodiscard]] IndexBuffer&    GetIndexBuffer(const size_t index)    noexcept;
	[[nodiscard]] VertexBuffer&   GetVertexBuffer(const size_t index)   noexcept;
	[[nodiscard]] ConstantBuffer& GetConstantBuffer(const size_t index) noexcept;
};