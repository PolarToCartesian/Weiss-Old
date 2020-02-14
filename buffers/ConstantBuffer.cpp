#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const DeviceInfo& deviceInfo, const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS)
	: Buffer(deviceInfo, buff, objSize, 0u, D3D11_BIND_CONSTANT_BUFFER, true),
	  m_bindingType(bindingType), m_slotVS(slotVS), m_slotPS(slotPS), m_objSize(objSize)
{
	
}

void ConstantBuffer::SetData(const void* objPtr) const
{
	D3D11_MAPPED_SUBRESOURCE resource;

	if (this->m_deviceInfo.m_pDeviceContext->Map(this->m_pResource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Map ConstantBuffer Memory");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ConstantBufferDataSettingException();
	}

	std::memcpy(resource.pData, objPtr, this->m_objSize);

	this->m_deviceInfo.m_pDeviceContext->Unmap(this->m_pResource.Get(), 0);
}

void ConstantBuffer::Bind() const noexcept
{
	if (this->m_bindingType == ShaderBindingType::VERTEX || this->m_bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->VSSetConstantBuffers(this->m_slotVS, 1u, this->m_pResource.GetAddressOf());

	if (this->m_bindingType == ShaderBindingType::PIXEL || this->m_bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->PSSetConstantBuffers(this->m_slotPS, 1u, this->m_pResource.GetAddressOf());
}