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
	IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const IndexBufferDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		this->m_nBytes = static_cast<uint64_t>(sizeof(uint32_t) * descriptor.indices.size());

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = static_cast<UINT>(this->m_nBytes);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = descriptor.indices.data();

		if (pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pIndexBuffer) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Unable To Create Index Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw IndexBufferCreationException();
		}
	}

	[[nodiscard]] size_t GetSize() const { return this->m_nBytes; }

	void Bind() const noexcept { this->m_pDeviceContextRef->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u); }
};