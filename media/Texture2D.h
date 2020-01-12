#include "Image.h"
#include "../misc/ShaderBindingLoading.h"

class Texture2DCreationException : public std::exception { };

struct Texture2DDescriptor
{
	ShaderBindingType bindingType;
	Image image;
	UINT slotVS;
	UINT slotPS;
	bool useMipMaps;
};

class Texture2D {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	Texture2DDescriptor m_descriptor;

public:
	Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
	{
		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = static_cast<void*>(descriptor.image.GetBuffer());
		subResourceData.SysMemPitch = this->m_descriptor.image.GetWidth() * sizeof(Coloru8);
		subResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC texture2DDescriptor = {};
		texture2DDescriptor.Width = this->m_descriptor.image.GetWidth();
		texture2DDescriptor.Height = this->m_descriptor.image.GetHeight();
		texture2DDescriptor.MipLevels = this->m_descriptor.useMipMaps ? 0 : 1;
		texture2DDescriptor.ArraySize = 1;
		texture2DDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2DDescriptor.SampleDesc.Count = 1;
		texture2DDescriptor.SampleDesc.Quality = 0;
		texture2DDescriptor.Usage = D3D11_USAGE_DEFAULT;
		texture2DDescriptor.BindFlags = this->m_descriptor.useMipMaps ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
		texture2DDescriptor.MiscFlags = this->m_descriptor.useMipMaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

		if (pDeviceRef->CreateTexture2D(&texture2DDescriptor, this->m_descriptor.useMipMaps ? nullptr : &subResourceData, &texture2D) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Texture 2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw Texture2DCreationException();
		}

		if (this->m_descriptor.useMipMaps)
			this->m_pDeviceContextRef->UpdateSubresource(texture2D.Get(), 0u, nullptr, subResourceData.pSysMem, subResourceData.SysMemPitch, 0u);

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

	void Bind() const noexcept
	{
		if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->VSSetShaderResources(this->m_descriptor.slotVS, 1u, this->m_pShaderResourceView.GetAddressOf());

		if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
			this->m_pDeviceContextRef->PSSetShaderResources(this->m_descriptor.slotPS, 1u, this->m_pShaderResourceView.GetAddressOf());
	}
};