#pragma once

#include "../misc/Pch.h"

class VertexBufferDataSettingException : public std::exception { };

class VertexBufferCreationException : public std::exception { };

struct VertexBufferDescriptor
{
	void* memoryPtr    = nullptr;
	size_t nElements   = 0u;
	size_t elementSize = 0u;
	bool isUpdatable   = false;
};

class VertexBuffer {
private:
	size_t m_nElements;
	size_t m_elementSize;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	     m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>&        pDeviceRef,
					   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexBufferDescriptor&					    descriptor);

	[[nodiscard]] size_t GetElementCount() const noexcept;

	void SetData(const void* memoryPtr, const size_t nElements);

	void Bind() const noexcept;
};