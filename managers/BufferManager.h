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
	[[nodiscard]] size_t CreateIndexBuffer(const IndexBufferDescriptor& descriptor);

	[[nodiscard]] size_t CreateVertexBuffer(const VertexBufferDescriptor& descriptor);

	[[nodiscard]] size_t CreateConstantBuffer(const ConstantBufferDescriptor& descriptor);
};