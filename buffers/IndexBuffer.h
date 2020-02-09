#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"

class IndexBufferCreationException : public std::exception { };

struct IndexBufferDescriptor
{
	std::vector<uint32_t> indices;
};

class IndexBuffer {
private:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

	size_t m_nBytes;

public:
	IndexBuffer(const DeviceInfo& deviceInfo, const IndexBufferDescriptor& descriptor);

	[[nodiscard]] size_t GetSize() const;

	void Bind() const noexcept;
};