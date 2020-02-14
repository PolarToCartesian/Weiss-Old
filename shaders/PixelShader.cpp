#include "PixelShader.h"

PixelShader::PixelShader(const DeviceInfo& deviceInfo, const char* sourceFilename)
	: Shader<ID3D11PixelShader>(deviceInfo, sourceFilename)
{
	this->Load();
}

void PixelShader::Load()
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	std::ifstream fileStream(this->m_sourceFilename);
	std::string sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw PixelShaderCreationException();
	}

	if (this->m_deviceInfo.m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pResource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw PixelShaderCreationException();
	}
}

void PixelShader::Bind() const noexcept
{
	this->m_deviceInfo.m_pDeviceContext->PSSetShader(this->m_pResource.Get(), nullptr, 0u);
}