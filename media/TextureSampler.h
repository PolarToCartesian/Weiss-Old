#pragma once

#include "../misc/DeviceInfo.h"

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
	const DeviceInfo& m_deviceInfo;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;

	TextureSamplerDescriptor m_descriptor;

public:
	TextureSampler(const DeviceInfo& deviceInfo, const TextureSamplerDescriptor& descriptor);

	void Bind() const noexcept;
};