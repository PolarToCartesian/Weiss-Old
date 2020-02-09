#pragma once

#include "Image.h"
#include "../misc/DeviceInfo.h"
#include "../shaders/ShaderBindingLoading.h"

class Texture2DCreationException : public std::exception { };

struct Texture2DSettings {
	ShaderBindingType bindingType;
	UINT slotVS;
	UINT slotPS;
	bool useMipMaps;
};

struct Texture2DDescriptor : Texture2DSettings
{
	Image image;
};

class Texture2D {
private:
	const DeviceInfo& m_deviceInfo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;

	Texture2DSettings m_settings;

public:
	Texture2D(const DeviceInfo& deviceInfo, const Texture2DDescriptor& descriptor);

	void Bind() const noexcept;
};