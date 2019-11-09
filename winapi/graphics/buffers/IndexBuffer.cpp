#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<uint32_t>& indices)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	this->m_nBytes = static_cast<uint64_t>(sizeof(uint32_t) * indices.size());

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = this->m_nBytes;

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	H_ERROR(pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pIndexBuffer));
}

uint64_t IndexBuffer::getSize() const { return this->m_nBytes; }

void IndexBuffer::Bind() const noexcept
{
	this->m_pDeviceContextRef->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}