#include "Texture2D.h"

Texture2D::Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor)
 : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
{
	std::vector<Vec4u8> buff(this->m_descriptor.image.getNPixels());
	for (uint16_t i = 0; i < this->m_descriptor.image.getNPixels(); i++)
		buff[i] = Vec4u8{ descriptor.image.getBuffer()[i][0], descriptor.image.getBuffer()[i][1], descriptor.image.getBuffer()[i][2], 1 };

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem          = static_cast<void*>(buff.data());
	subResourceData.SysMemPitch      = this->m_descriptor.image.getWidth() * 4 * sizeof(uint8_t);
	subResourceData.SysMemSlicePitch = 0;
	
	D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
	texture2DDescriptor.Width            = this->m_descriptor.image.getWidth();
	texture2DDescriptor.Height           = this->m_descriptor.image.getHeight();
	texture2DDescriptor.MipLevels        = 1;
	texture2DDescriptor.ArraySize        = 1;
	texture2DDescriptor.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDescriptor.SampleDesc.Count = 1;
	texture2DDescriptor.Usage            = D3D11_USAGE_IMMUTABLE;
	texture2DDescriptor.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT_ERROR(pDeviceRef->CreateTexture2D(&texture2DDescriptor, &subResourceData, texture2D.GetAddressOf()), "Could Not Create Texture 2D");

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
	SRVDescriptor.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDescriptor.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescriptor.Texture2D.MipLevels = 1;

	HRESULT_ERROR(pDeviceRef->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()), "Could Not Create ShaderResourceView");
}

void Texture2D::bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL  || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
}