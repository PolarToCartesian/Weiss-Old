#pragma once

#include "../misc/Includes.h"
#include "../misc/ShaderBindingType.h"

struct TextureSamplerDescriptor
{
	const ShaderBindingType bindingType;
	const UINT slotVS;
	const UINT slotPS;
};

class TextureSampler {
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

		TextureSamplerDescriptor m_descriptor;

	public:
		TextureSampler(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const TextureSamplerDescriptor& descriptor);
	
		void bind() const noexcept;
};