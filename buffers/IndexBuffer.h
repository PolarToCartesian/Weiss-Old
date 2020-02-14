#pragma once

#include "Buffer.h"

class IndexBufferCreationException : public std::exception { };
class IndexBufferMappingException  : public std::exception { };

class IndexBuffer : public Buffer<IndexBufferCreationException, IndexBufferMappingException> {
private:
	size_t m_nIndices;

public:
	IndexBuffer(const DeviceInfo& deviceInfo, const void* buff, const UINT nIndices, const bool isUpdatable);

	template <typename CONTAINER>
	IndexBuffer(const DeviceInfo& deviceInfo, const CONTAINER& arr, const bool isUpdatable)
		: IndexBuffer(deviceInfo, arr.data(), arr.size(), isUpdatable)
	{ }

	[[nodiscard]] size_t GetIndexCount() const;

	void SetData(const void* buff, const UINT nIndices) const;

	template <typename CONTAINER>
	void SetData(const CONTAINER& arr) { this->SetData(arr.data(), arr.size()); }

	virtual void Bind() const noexcept override;
};