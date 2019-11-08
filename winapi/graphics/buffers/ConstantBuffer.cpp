#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
							   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
							   const ConstantBufferShaderBinding bindingType,
							   const size_t objSize)
	: m_pDeviceContextRef(pDeviceContextRef), m_bindingType(bindingType), m_objSize(objSize)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.CPUAccessFlags = 0;
	cbd.ByteWidth = static_cast<UINT>(objSize);

	cbd.StructureByteStride = 0u;
	H_ERROR(pDeviceRef->CreateBuffer(&cbd, nullptr, &this->m_pConstantBuffer));
}

void ConstantBuffer::setData(const void* objPtr)
{
	this->m_pDeviceContextRef->UpdateSubresource(this->m_pConstantBuffer.Get(), 0, 0, objPtr, 0, 0);
}

void ConstantBuffer::Bind() const noexcept
{
	if (m_bindingType == ConstantBufferShaderBinding::VERTEX || m_bindingType == ConstantBufferShaderBinding::BOTH)
		this->m_pDeviceContextRef->VSSetConstantBuffers(0u, 1u, this->m_pConstantBuffer.GetAddressOf());

	if (m_bindingType == ConstantBufferShaderBinding::PIXEL  || m_bindingType == ConstantBufferShaderBinding::BOTH)
		this->m_pDeviceContextRef->PSSetConstantBuffers(0u, 1u, this->m_pConstantBuffer.GetAddressOf());
}