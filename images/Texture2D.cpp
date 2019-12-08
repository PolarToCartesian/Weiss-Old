#include "Texture2D.h"

Texture2D::Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor)
 : m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
{
	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem          = static_cast<void*>(descriptor.image.getBuffer());
	subResourceData.SysMemPitch      = this->m_descriptor.image.getWidth() * sizeof(Coloru8);
	subResourceData.SysMemSlicePitch = 0;
	
	D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
	texture2DDescriptor.Width              = this->m_descriptor.image.getWidth();
	texture2DDescriptor.Height             = this->m_descriptor.image.getHeight();
	texture2DDescriptor.MipLevels          = this->m_descriptor.useMipMaps ? 0 : 1;
	texture2DDescriptor.ArraySize          = 1;
	texture2DDescriptor.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDescriptor.SampleDesc.Count   = 1;
	texture2DDescriptor.SampleDesc.Quality = 0;
	texture2DDescriptor.Usage              = D3D11_USAGE_DEFAULT;
	texture2DDescriptor.BindFlags          = this->m_descriptor.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
	texture2DDescriptor.MiscFlags          = this->m_descriptor.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT_ERROR(pDeviceRef->CreateTexture2D(&texture2DDescriptor, this->m_descriptor.useMipMaps ? nullptr : &subResourceData, &texture2D), "Could Not Create Texture 2D");

	if (this->m_descriptor.useMipMaps)
		this->m_pDeviceContextRef->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData.pSysMem, subResourceData.SysMemPitch, 0u);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
	SRVDescriptor.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDescriptor.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescriptor.Texture2D.MostDetailedMip = 0;
	SRVDescriptor.Texture2D.MipLevels       = this->m_descriptor.useMipMaps ? -1 : 1;

	HRESULT_ERROR(pDeviceRef->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()), "Could Not Create ShaderResourceView");

	if (this->m_descriptor.useMipMaps)
		this->m_pDeviceContextRef->GenerateMips(m_pShaderResourceView.Get());
}

void Texture2D::bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL  || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
}