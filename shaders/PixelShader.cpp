#include "PixelShader.h"

PixelShader::PixelShader(const DeviceInfo& deviceInfo, const PixelShaderDescriptor& descriptor)
	: m_deviceInfo(deviceInfo)
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

	if (this->m_deviceInfo.m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw PixelShaderCreationException();
	}
}

void PixelShader::Bind() const noexcept
{
	this->m_deviceInfo.m_pDeviceContext->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u);
}