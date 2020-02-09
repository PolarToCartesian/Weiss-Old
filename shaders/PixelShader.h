#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"
#include "ShaderBindingLoading.h"

class PixelShaderCreationException : public std::exception { };

struct PixelShaderDescriptor
{
	ShaderLoadingMethod loadingMethod;
	const wchar_t*      binaryFilename;
	const char*         sourceCode;
};

class PixelShader {
private:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

public:
	PixelShader(const DeviceInfo& deviceInfo, const PixelShaderDescriptor& descriptor);

	void Bind() const noexcept;
};