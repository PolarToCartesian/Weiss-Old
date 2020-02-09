#include "Texture2D.h"

Texture2D::Texture2D(const DeviceInfo& deviceInfo, const Texture2DDescriptor& descriptor)
	: m_deviceInfo(deviceInfo), m_settings(descriptor)
{
	D3D11_SUBRESOURCE_DATA* subResourceData = new D3D11_SUBRESOURCE_DATA();

	subResourceData->pSysMem          = static_cast<void*>(descriptor.image.GetBuffer());
	subResourceData->SysMemPitch      = descriptor.image.GetWidth() * sizeof(Coloru8);
	subResourceData->SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
	texture2DDescriptor.Width = descriptor.image.GetWidth();
	texture2DDescriptor.Height = descriptor.image.GetHeight();
	texture2DDescriptor.MipLevels = this->m_settings.useMipMaps ? 0 : 1;
	texture2DDescriptor.ArraySize = 1u;
	texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDescriptor.SampleDesc.Count   = 1u;
	texture2DDescriptor.SampleDesc.Quality = 0;
	texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
	texture2DDescriptor.BindFlags = this->m_settings.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
	texture2DDescriptor.MiscFlags = this->m_settings.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

	if (this->m_deviceInfo.m_pDevice->CreateTexture2D(&texture2DDescriptor, this->m_settings.useMipMaps ? nullptr : subResourceData, &texture2D) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Texture 2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw Texture2DCreationException();
	}

	if (this->m_settings.useMipMaps)
		this->m_deviceInfo.m_pDeviceContext->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData->pSysMem, subResourceData->SysMemPitch, 0u);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
	SRVDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescriptor.Texture2D.MostDetailedMip = 0;
	SRVDescriptor.Texture2D.MipLevels = this->m_settings.useMipMaps ? -1 : 1;

	if (this->m_deviceInfo.m_pDevice->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create ShaderResourceView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw Texture2DCreationException();
	}

	if (this->m_settings.useMipMaps)
		this->m_deviceInfo.m_pDeviceContext->GenerateMips(m_pShaderResourceView.Get());
}

void Texture2D::Bind() const noexcept
{
	if (this->m_settings.bindingType == ShaderBindingType::VERTEX || this->m_settings.bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->VSSetShaderResources(this->m_settings.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

	if (this->m_settings.bindingType == ShaderBindingType::PIXEL || this->m_settings.bindingType == ShaderBindingType::BOTH)
		this->m_deviceInfo.m_pDeviceContext->PSSetShaderResources(this->m_settings.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
}