#pragma once

#include "Buffer.h"

class VertexBufferMappingException  : public std::exception { };
class VertexBufferCreationException : public std::exception { };

class VertexBuffer : public Buffer<VertexBufferCreationException, VertexBufferMappingException> {
private:
	size_t m_nVertices;
	size_t m_vertexSize;

public:
	VertexBuffer(const DeviceInfo& deviceInfo, const void* buff, const size_t nVertices, const size_t vertexSize, const bool isUpdatable);

	template <typename CONTAINER>
	VertexBuffer(const DeviceInfo& deviceInfo, const CONTAINER& arr, const bool isUpdatable)
		: VertexBuffer(deviceInfo, arr.data(), arr.size(), sizeof(arr[0]), isUpdatable)
	{ }

	[[nodiscard]] size_t GetElementCount() const noexcept;

	void SetData(const void* buff, const size_t nVertices) const;

	template <typename CONTAINER>
	void SetData(const CONTAINER& arr) { this->SetData(arr.data(), arr.size()); }

	void Bind() const noexcept;
};