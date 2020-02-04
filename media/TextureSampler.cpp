#include "TextureSampler.h"

TextureSampler::TextureSampler(const Microsoft::WRL::ComPtr<ID3D11Device>&		  pDeviceRef,
									 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
							   const TextureSamplerDescriptor&					  descriptor)
	: m_pDeviceContextRef(pDeviceContextRef), m_descriptor(descriptor)
{
	D3D11_SAMPLER_DESC samplerDescriptor;
	samplerDescriptor.Filter = descriptor.filter;
	samplerDescriptor.AddressU = descriptor.addressMode;
	samplerDescriptor.AddressV = descriptor.addressMode;
	samplerDescriptor.AddressW = descriptor.addressMode;
	samplerDescriptor.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
	samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescriptor.MipLODBias = 0.0f;
	samplerDescriptor.MinLOD = 0.0f;
	samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;

	pDeviceRef->CreateSamplerState(&samplerDescriptor, &this->m_pSamplerState);
}

void TextureSampler::Bind() const noexcept
{
	if (this->m_descriptor.bindingType == ShaderBindingType::VERTEX || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->VSSetSamplers(this->m_descriptor.slotVS, 1u, this->m_pSamplerState.GetAddressOf());

	if (this->m_descriptor.bindingType == ShaderBindingType::PIXEL || this->m_descriptor.bindingType == ShaderBindingType::BOTH)
		this->m_pDeviceContextRef->PSSetSamplers(this->m_descriptor.slotPS, 1u, this->m_pSamplerState.GetAddressOf());
}