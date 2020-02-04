#include "Texture2D.h"

Texture2D::Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>&	    pDeviceRef,
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
					 const Texture2DDescriptor&						    descriptor)
	: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
{
	D3D11_SUBRESOURCE_DATA* subResourceDataArr = new D3D11_SUBRESOURCE_DATA[this->m_descriptor.images.size()];

	for (size_t i = 0u; i < this->m_descriptor.images.size(); i++) {
		const Image* image = descriptor.images.data() + i;
		D3D11_SUBRESOURCE_DATA& subResourceData = subResourceDataArr[i];

		subResourceData.pSysMem = static_cast<void*>(image->GetBuffer());
		subResourceData.SysMemPitch = image->GetWidth() * sizeof(Coloru8);
		subResourceData.SysMemSlicePitch = 0;
	}

	D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
	texture2DDescriptor.Width = this->m_descriptor.images[0].GetWidth();
	texture2DDescriptor.Height = this->m_descriptor.images[0].GetHeight();
	texture2DDescriptor.MipLevels = this->m_descriptor.useMipMaps ? 0 : 1;
	texture2DDescriptor.ArraySize = this->m_descriptor.images.size();
	texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDescriptor.SampleDesc.Count = this->m_descriptor.images.size();
	texture2DDescriptor.SampleDesc.Quality = 0;
	texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
	texture2DDescriptor.BindFlags = this->m_descriptor.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
	texture2DDescriptor.MiscFlags = this->m_descriptor.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

	if (pDeviceRef->CreateTexture2D(&texture2DDescriptor, this->m_descriptor.useMipMaps ? nullptr : subResourceDataArr, &texture2D) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Texture 2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw Texture2DCreationException();
	}

	if (this->m_descriptor.useMipMaps)
		for (size_t i = 0u; i < this->m_descriptor.images.size(); i++)
			this->m_pDeviceContextRef->UpdateSubresource(texture2D.Get(), i, nullptr, subResourceDataArr[i].pSysMem, subResourceDataArr[i].SysMemPitch, 0u);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor = {};
	SRVDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDescriptor.Texture2D.MostDetailedMip = 0;
	SRVDescriptor.Texture2D.MipLevels = this->m_descriptor.useMipMaps ? -1 : 1;

	if (pDeviceRef->CreateShaderResourceView(texture2D.Get(), &SRVDescriptor, this->m_pShaderResourceView.GetAddressOf()) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create ShaderResourceView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw Texture2DCreationException();
	}

	if (this->m_descriptor.useMipMaps)
		this->m_pDeviceContextRef->GenerateMips(m_pShaderResourceView.Get());
}

void Texture2D::Bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, this->m_descriptor.images.size(), this->m_pShaderResourceView.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, this->m_descriptor.images.size(), this->m_pShaderResourceView.GetAddressOf());
}