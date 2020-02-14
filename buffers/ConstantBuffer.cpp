#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const DeviceInfo& deviceInfo, const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS)
	: Buffer(deviceInfo, buff, objSize, 0u, D3D11_BIND_CONSTANT_BUFFER, true),
	  m_bindingType(bindingType), m_slotVS(slotVS), m_slotPS(slotPS), m_objSize(objSize)
{
	
}

void ConstantBuffer::SetData(const void* objPtr) const
{
	this->__Buffer__SetData(objPtr, this->m_objSize);
}

void ConstantBuffer::Bind() const noexcept
{
	if (this->m_bindingType == ShaderBindingType::VERTEX || this->m_bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->VSSetConstantBuffers(this->m_slotVS, 1u, this->m_pResource.GetAddressOf());

	if (this->m_bindingType == ShaderBindingType::PIXEL || this->m_bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->PSSetConstantBuffers(this->m_slotPS, 1u, this->m_pResource.GetAddressOf());
}