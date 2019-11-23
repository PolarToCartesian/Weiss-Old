#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
							   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
							   const ConstantBufferDescriptor& descriptor)
	: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.CPUAccessFlags = 0;
	cbd.ByteWidth = static_cast<UINT>(this->m_descriptor.objSize);

	cbd.StructureByteStride = 0u;
	HRESULT_ERROR(pDeviceRef->CreateBuffer(&cbd, nullptr, &this->m_pConstantBuffer));
}

void ConstantBuffer::setData(const void* objPtr)
{
	this->m_pDeviceContextRef->UpdateSubresource(this->m_pConstantBuffer.Get(), 0, 0, objPtr, 0, 0);
}

void ConstantBuffer::Bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->VSSetConstantBuffers(this->m_descriptor.slotVS, 1u, this->m_pConstantBuffer.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL  || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->PSSetConstantBuffers(this->m_descriptor.slotPS, 1u, this->m_pConstantBuffer.GetAddressOf());
}