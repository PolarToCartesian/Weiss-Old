#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"
#include "../misc/ShaderBindingLoading.h"

class VertexShaderCreationException : public std::exception { };

struct VertexShaderDescriptor
{
	std::vector<std::pair<const char*, DXGI_FORMAT>> inputElementDescriptors = {};
	ShaderLoadingMethod								 loadingMethod           = ShaderLoadingMethod::FROM_BINARY_FILE;
	const wchar_t*									 binaryFilename          = nullptr; // Ignore if ShaderLoadingMethod is FROM_SOURCE_CODE
	const char*										 sourceCode              = nullptr; // Ignore if ShaderLoadingMethod is FROM_BINARY_FILE
};

class VertexShader {
private:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_pInputLayout;

public:
	VertexShader(const DeviceInfo& deviceInfo, const VertexShaderDescriptor& descriptor);

	void Bind() const noexcept;
};