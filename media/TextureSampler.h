#pragma once

#include "../misc/Pch.h"
#include "../misc/ShaderBindingLoading.h"

struct TextureSamplerDescriptor
{
	ShaderBindingType bindingType;
	UINT slotVS;
	UINT slotPS;
	D3D11_FILTER filter;
	D3D11_TEXTURE_ADDRESS_MODE addressMode;
};

class TextureSampler {
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState>   m_pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	TextureSamplerDescriptor m_descriptor;

public:
	TextureSampler(const Microsoft::WRL::ComPtr<ID3D11Device>&		  pDeviceRef,
						 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				   const TextureSamplerDescriptor&					  descriptor);

	void Bind() const noexcept;
};