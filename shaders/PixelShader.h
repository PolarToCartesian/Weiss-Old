#pragma once

#include "../misc/Pch.h"
#include "../misc/ShaderBindingLoading.h"

class PixelShaderCreationException : public std::exception { };

struct PixelShaderDescriptor
{
	ShaderLoadingMethod loadingMethod;
	const wchar_t*      binaryFilename;
	const char*         sourceCode;
};

class PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>&        pDeviceRef,
					  Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				const PixelShaderDescriptor&                       descriptor);

	void Bind() const noexcept;
};