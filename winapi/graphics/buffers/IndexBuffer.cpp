#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<unsigned short>& indices)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	this->m_nBytes = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = this->m_nBytes;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	H_ERROR(pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pIndexBuffer));
}

UINT IndexBuffer::getSize() const { return this->m_nBytes; }

void IndexBuffer::Bind() const noexcept
{
	this->m_pDeviceContextRef->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}