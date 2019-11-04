#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<unsigned int>& indices)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(unsigned int) * indices.size();
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = &indices[0];
	pDeviceRef->CreateBuffer(&ibd, &isd, &this->m_pBuffer);
}

void IndexBuffer::Bind()
{
	this->m_pDeviceContextRef->IASetIndexBuffer(m_pBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}