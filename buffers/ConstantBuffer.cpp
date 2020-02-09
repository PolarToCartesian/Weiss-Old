#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const DeviceInfo& deviceInfo, const ConstantBufferDescriptor& descriptor)
	: m_deviceInfo(deviceInfo), m_descriptor(descriptor)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.CPUAccessFlags = 0;
	cbd.ByteWidth = static_cast<UINT>(this->m_descriptor.objSize);

	cbd.StructureByteStride = 0u;

	if (this->m_deviceInfo.m_pDevice->CreateBuffer(&cbd, nullptr, &this->m_pConstantBuffer) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Unable To Create Constant Buffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ConstantBufferCreationException();
	}
}

void ConstantBuffer::SetData(const void* objPtr)
{
	this->m_deviceInfo.m_pDeviceContext->UpdateSubresource(this->m_pConstantBuffer.Get(), 0, 0, objPtr, 0, 0);
}

void ConstantBuffer::Bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->VSSetConstantBuffers(this->m_descriptor.slotVS, 1u, this->m_pConstantBuffer.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->PSSetConstantBuffers(this->m_descriptor.slotPS, 1u, this->m_pConstantBuffer.GetAddressOf());
}