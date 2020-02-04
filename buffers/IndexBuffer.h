#pragma once

#include "../misc/Pch.h"

class IndexBufferCreationException : public std::exception { };

struct IndexBufferDescriptor
{
	std::vector<uint32_t> indices;
};

class IndexBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	size_t m_nBytes;

public:
	IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>&		   pDeviceRef, 
					  Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				const IndexBufferDescriptor&					   descriptor);

	[[nodiscard]] size_t GetSize() const;

	void Bind() const noexcept;
};