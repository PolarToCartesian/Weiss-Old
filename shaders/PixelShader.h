#pragma once

#include "../misc/Pch.h"
#include "../misc/ShaderBindingLoading.h"

class PixelShaderCreationException : public std::exception { };

struct PixelShaderDescriptor
{
	ShaderLoadingMethod loadingMethod;
	const wchar_t* binaryFilename;
	const char* sourceCode;
};

class PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				const PixelShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			if (D3DReadFileToBlob(descriptor.binaryFilename, &pBlob) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Read Pixel Shader File");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw PixelShaderCreationException();
			}
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			if (D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw PixelShaderCreationException();
			}
		}
		else
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Pixel Shaders");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw PixelShaderCreationException();
		}

		if (pDeviceRef->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw PixelShaderCreationException();
		}
	}

	void Bind() const noexcept { this->m_pDeviceContextRef->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u); }
};