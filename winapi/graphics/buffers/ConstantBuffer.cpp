#include "ConstantBuffer.h"

template <typename T>
ConstantBuffer::ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const T& obj)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(T);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &obj;
	this->pDeviceRef->CreateBuffer(&cbd, &csd, &this->m_pBuffer);
}

void ConstantBuffer::Bind()
{
	this->m_pDeviceContextRef->VSSetConstantBuffers(0u, 1u, this->m_pBuffer.GetAddressOf());
}